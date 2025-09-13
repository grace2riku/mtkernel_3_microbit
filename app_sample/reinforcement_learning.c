/*
 * reinforcement_learning.c
 *
 *  Created on: 2025/08/03
 *      Author: k-abe
 */
#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"

#include <stdlib.h>	// srand, rand
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "reinforcement_learning.h"
#include "ad.h"
#include "Sensor.h"
#include "motor_drv.h"
#include "button.h"

// Qtable
// 1次元目: 状態の数
// 2次元目: 行動の数
static float mQtable[Q_TABLE_STATES_NUM][Q_TABLE_ACTIONS_NUM];

LOCAL void rl_learning_task(INT stacd, void *exinf);
LOCAL void rl_action_task(INT stacd, void *exinf);

LOCAL T_CTSK rl_learning_task_ctsk = {
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= rl_learning_task,
	.tskatr		= TA_HLNG | TA_RNG3,
};

EXPORT T_CTSK* get_rl_learning_task_ctsk_addr(void)
{
	return &rl_learning_task_ctsk;
}

LOCAL T_CTSK rl_action_task_ctsk = {
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= rl_action_task,
	.tskatr		= TA_HLNG | TA_RNG3,
};

// タスクID
LOCAL ID rl_learning_tskid;
LOCAL ID rl_action_tskid;

// イベントフラグID
LOCAL ID rl_flgid;
// イベントフラグの生成情報
LOCAL const T_CFLG crlflg = {0, (TA_TFIFO | TA_WMUL), 0};

static const float alpha = 0.5;	//学習係数
static const float gamma = 0.9;	//減衰係数
static int epsilon = 5;			//行動を無作為に選ぶ確率[%]
static int trial_max = 1000;	//試行回数

EXPORT void rl_init(void) {
	// イベントフラグ生成
	rl_flgid = tk_cre_flg(&crlflg);

	// Q値の初期化
//	rl_init_Qtable();	// 0で初期化
	rl_set_example_Qtable_memcpy();	// 20250809 強化学習してライントレースできたときのQtable値で初期化

	// 乱数の初期化
	rl_init_epsilon_greedy();

	rl_learning_tskid = tk_cre_tsk(&rl_learning_task_ctsk);
	tk_sta_tsk(rl_learning_tskid, 0);

	rl_action_tskid = tk_cre_tsk(&rl_action_task_ctsk);
	tk_sta_tsk(rl_action_tskid, 0);
}

EXPORT ID rl_get_flgid(void) {
	return rl_flgid;
}


EXPORT T_CTSK* get_rl_action_task_ctsk_addr(void)
{
	return &rl_action_task_ctsk;
}


LOCAL void rl_learning_task(INT stacd, void *exinf)
{
	UINT flgptn, flgptn_resume;
	float Q_max = 0;	// Ｑ値の最大値
	float reward = 0;	// 報酬
	int action = 0;		// 行動
	int state = 0;		// 状態
	int next_state = 0;	// 行動後に遷移した状態
	int i;

	while (1) {
		tm_printf("rl_learning_task() enter tk_wai_flg() waiptn = RL_START\n");
		// 強化学習スタート指示待ち
		tk_wai_flg(rl_flgid, RL_START, (TWF_ORW | TWF_CLR), &flgptn, TMO_FEVR);
		tm_printf("rl_learning_task() exit tk_wai_flg() waiptn = RL_START\n");

		Q_max = 0;		// Ｑ値の最大値
		reward = 0;		// 報酬
		action = 0;		// 行動
		state = 0;		// 状態
		next_state = 0;	// 行動後に遷移した状態

		//初期状態の観測
		state = rl_get_state();

		// 試行開始
		for (i = 0; i < trial_max; i++) {
			tk_dly_tsk(20);
			if ( !(i % 100) ) {
				tm_printf("rl_learning_task() trial count = %d...\n", i);
			}
			// 行動の選択
			action = rl_epsilon_greedy(epsilon, state, Q_TABLE_ACTIONS_NUM, mQtable);
			// 行動の実行
			rl_move(action);
			// 状態の観測
			next_state = rl_get_state();
			// 報酬の獲得
			reward = rl_get_reward();

			// 行動後状態のQ値の最大値を求める
			Q_max = rl_max_Qval(next_state, Q_TABLE_ACTIONS_NUM, mQtable);
			// Q値の更新
			mQtable[state][action] = (1 - alpha) * mQtable[state][action] + alpha * (reward + gamma * Q_max);

			state = next_state;

			if (reward < 0) {
				state = 0;
				motor_stop();

				tm_printf("rl_learning_task() enter tk_wai_flg() waiptn = RL_TRAIN_RESUME\n");
				tm_printf("Place the robot on the line and then press the B button.\n");
				tk_wai_flg(rl_flgid, RL_TRAIN_RESUME, (TWF_ORW | TWF_CLR), &flgptn_resume, TMO_FEVR);
				tm_printf("rl_learning_task() exit tk_wai_flg() waiptn = RL_TRAIN_RESUME\n");

				state = rl_get_state();
			}
		} // for

		motor_stop();
		tm_printf("rl_learning_task() trial count = %d.\n", i);
		rl_output_Qtable();
		rl_output_rlsetqtbl_cmd_Qtable_format();
	} // while
}


LOCAL void rl_action_task(INT stacd, void *exinf)
{
	UINT flgptn;
	int action = 0;		// 行動
	int state = 0;		// 状態

	while (1) {
		tm_printf("rl_action_task() enter tk_wai_flg()\n");
		// 強化学習で得た政策実行の指示待ち
		tk_wai_flg(rl_flgid, RL_ACTION_START, (TWF_ORW | TWF_CLR), &flgptn, TMO_FEVR);
		tm_printf("rl_action_task() exit tk_wai_flg()\n");

		// Bボタンが押されるまでは強化学習で得た政策実行しライントレースし続ける
		while( !isButtonBPressed() ) {
			tk_dly_tsk(20);
			state = rl_get_state();
			action = rl_select_action(state, Q_TABLE_ACTIONS_NUM, mQtable);
			rl_move(action);
		}
		motor_stop();
	}

}

int rl_get_state(void) {
	int state = 3;
	_H adc_data[3];

    // adc_data[0] == P0(左側), adc_data[1] == P1(中央), adc_data[2] == P2(右側)
	analogRead3(adc_data);

	if (adc_data[2] > BLACK_THRESHORD) {
		state = 1;	// 右のセンサーが反応
	} else if (adc_data[0] > GRAY_THRESHORD) {
		state = 2;	// 左のセンサーが反応
	} else if (adc_data[1] > BLACK_THRESHORD){
		state = 0;	// 中央のセンサーが反応
	}

	return state;
}


int rl_get_reward(void) {
	int reword = 0;
	_H adc_data[3];

    // adc_data[0] == P0(左側), adc_data[1] == P1(中央), adc_data[2] == P2(右側)
	analogRead3(adc_data);

	if (adc_data[0] > GRAY_THRESHORD || adc_data[2] > BLACK_THRESHORD) {
		reword = 0;	// 左または右のセンサーが反応
	} else if (adc_data[1] > BLACK_THRESHORD){
		reword = 1;	// 中央のセンサーが反応
	} else {
		reword = -100;
	}

	return reword;
}


void rl_move(int action) {
	if (action == 0) {
		// 前進
		motor_drive(30, 30);
	} else if (action == 1) {
		// 右旋回
		motor_drive(0, 30);
	} else {
		// 右旋回
		motor_drive(30, 0);
	}
}

void rl_init_Qtable(void) {
	int i, j;

	//Q値の初期化
	for(i=0;i < Q_TABLE_STATES_NUM; i++){
		for(j=0;j < Q_TABLE_ACTIONS_NUM; j++){
			mQtable[i][j] = 0;
		}
	}
}

float rl_max_Qval(int state, int num_actions, QtablePtr Qtable) {
	float max;
	int i = 0;

	max = Qtable[state][0];
	for (i = 1; i < num_actions; i++) {
		if (Qtable[state][i] > max) {
			max = Qtable[state][i];
		}
	}
	return max;
}

QtablePtr rl_get_Qtable_address(void) {
	return mQtable;
}


int rl_select_action(int state, int num_actions, QtablePtr Qtable) {
	float max;
	int i = 0;
	int i_max;
	int action;

	max = Qtable[state][0];
	i_max = 0;

	for (i = 1; i < num_actions; i++) {
		if (Qtable[state][i] > max) {
			max = Qtable[state][i];
			i_max = i;
		}
	}
	action = i_max;
	return action;
}


void rl_init_epsilon_greedy(void) {
	SYSTIM	tim;
	tk_get_tim(&tim);		/* 現在時刻を取得 */

//	tm_printf("rl_init_epsilon_greedy() seed(tk_get_tim() output parameter SYSTIM Lower 32bit)= %d\n", tim.lo);

	srand(tim.lo);
}


int rl_epsilon_greedy(int epsilon, int state, int num_action, QtablePtr Qtable) {
	int action;

	if (epsilon > rand() % 100) {
		// 無作為に行動を選択する
		action = rand() % num_action;

//		tm_printf("rl_epsilon_greedy() rand action = %d\n", action);
	} else {
		// 最大のQ値を持つ行動を選択する
		action = rl_select_action(state, num_action, Qtable);

//		tm_printf("rl_epsilon_greedy() select action = %d\n", action);
	}
	return action;
}

void rl_output_Qtable(void) {
	int i, j;

	for (i = 0; i < Q_TABLE_STATES_NUM; i++) {
		for (j = 0; j < Q_TABLE_ACTIONS_NUM; j++) {
			xprintf("Qtable[%d][%d] = %f\n", i, j, mQtable[i][j]);
		}
	}
}

void rl_output_rlsetqtbl_cmd_Qtable_format(void) {
	int i, j;

	xprintf("rlsetqtbl command setting format\n");
	xprintf("rlsetqtbl ");

	for (i = 0; i < Q_TABLE_STATES_NUM; i++) {
		for (j = 0; j < Q_TABLE_ACTIONS_NUM; j++) {
			xprintf("%f ", mQtable[i][j]);
		}
	}
	xprintf("\n");
}

void rl_set_example_Qtable_memcpy(void) {
	float example_Qtable[Q_TABLE_STATES_NUM][Q_TABLE_ACTIONS_NUM];

	// 20250809 強化学習で得た政策でライントレースできたときのQtable値
	example_Qtable[0][0] = -49.403526;
	example_Qtable[0][1] = -97.823532;
	example_Qtable[0][2] = -113.177238;
	example_Qtable[1][0] = -113.157005;
	example_Qtable[1][1] = -108.166664;
	example_Qtable[1][2] = -89.119911;
	example_Qtable[2][0] = -72.646233;
	example_Qtable[2][1] = -33.338005;
	example_Qtable[2][2] = -106.562347;
	example_Qtable[3][0] = -129.607758;
	example_Qtable[3][1] = -142.625000;
	example_Qtable[3][2] = -164.059372;

	rl_Qtable_memcpy(example_Qtable);
}

void rl_Qtable_memcpy(QtablePtr srcQtable) {
	int i, j;

	for (i = 0; i < Q_TABLE_STATES_NUM; i++) {
		for (j = 0; j < Q_TABLE_ACTIONS_NUM; j++) {
			mQtable[i][j] = srcQtable[i][j];
		}
	}
}
