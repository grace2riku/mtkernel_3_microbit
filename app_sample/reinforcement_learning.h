/*
 * reinforcement_learning.h
 *
 *  Created on: 2025/08/03
 *      Author: k-abe
 */

#ifndef REINFORCEMENT_LEARNING_H
#define REINFORCEMENT_LEARNING_H

#define Q_TABLE_STATES_NUM	(4)	// 状態の数
#define Q_TABLE_ACTIONS_NUM	(3)	// 行動の数

// イベントフラグのビットパターン
#define RL_START			(1 << 0)	// 強化学習スタート
#define RL_TRAIN_RESUME		(1 << 1)	// 強化学習リスタート（報酬獲得失敗したから強化学習の再スタート）
#define RL_STOP				(1 << 2)	// 強化学習ストップ
#define RL_ACTION_START		(1 << 3)	// 強化学習で得た政策を実行
#define RL_ACTION_STOP		(1 << 4)	// 強化学習で得た政策を実行を停止

// Qtableの型定義
typedef float (*QtablePtr)[Q_TABLE_ACTIONS_NUM];

extern void rl_init(void);
extern ID rl_get_flgid(void);
extern int rl_get_state(void);
extern int rl_get_reward(void);
extern void rl_move(int action);
extern void rl_init_Qtable(void);
extern float rl_max_Qval(int state, int num_actions, QtablePtr Qtable);
extern QtablePtr rl_get_Qtable_address(void);
extern int rl_select_action(int state, int num_actions, QtablePtr Qtable);
extern void rl_init_epsilon_greedy(void);
extern int rl_epsilon_greedy(int epsilon, int state, int num_action, QtablePtr Qtable);
extern T_CTSK* get_rl_learning_task_ctsk_addr(void);
extern T_CTSK* get_rl_action_task_ctsk_addr(void);

#endif /* REINFORCEMENT_LEARNING_H */
