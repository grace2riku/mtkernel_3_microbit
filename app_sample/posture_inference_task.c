/*
 * posture_inference_task.c
 *
 *  Created on: 2025/09/09
 *      Author: k-abe
 */
#define PROHIBIT_DEF_SIZE_T
#include "xprintf.h"

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "posture_inference.h"
#include "acceleration_sensor.h"
#include "posture_inference_task.h"

// タスクID
LOCAL ID posture_inference_tskid;

// イベントフラグID
LOCAL ID posture_inference_flgid;
// イベントフラグの生成情報
LOCAL const T_CFLG posture_inference_cflg = {0, (TA_TFIFO | TA_WMUL), 0};

LOCAL void posture_inference_task(INT stacd, void *exinf);	// task execution function
LOCAL T_CTSK posture_inference_task_ctsk = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= posture_inference_task,
	.tskatr		= TA_HLNG | TA_RNG3,
};


EXPORT void init_posture_inference_task(void) {
	// イベントフラグ生成
	posture_inference_flgid = tk_cre_flg(&posture_inference_cflg);

	posture_inference_tskid = tk_cre_tsk(&posture_inference_task_ctsk);
	tk_sta_tsk(posture_inference_tskid, 0);
}

EXPORT ID get_posture_inference_task_flgid(void) {
	return posture_inference_flgid;
}


EXPORT T_CTSK* get_posture_inference_task_ctsk_addr()
{
	return &posture_inference_task_ctsk;
}


LOCAL void posture_inference_task(INT stacd, void *exinf)
{
	UINT flgptn;

	tm_printf("posture_inference_task() start.\n");

	if (posture_infer_init() != 0) {
        // 初期化失敗
		tm_printf("posture_infer_init() failed.\r\n");
    }

    for (;;) {
    	tm_printf("posture_inference_task() enter tk_wai_flg() waiptn = INFERENCE_START\n");
    	// 姿勢推定スタート指示待ち
    	tk_wai_flg(posture_inference_flgid, INFERENCE_START, (TWF_ORW | TWF_CLR), &flgptn, TMO_FEVR);
    	tm_printf("posture_inference_task() exit tk_wai_flg() waiptn = INFERENCE_START\n");

    	int x, y, z;
        float ax, ay, az;      // [g] 単位で取得

    	acceleration_sensor_read(&x, &y, &z);
    	ax = (float)(x * 4) / 1000;
    	ay = (float)(y * 4) / 1000;
    	az = (float)(z * 4) / 1000;

        float probs[16];       // 出力クラス数ぶん確保（例: 5）
        int id = posture_infer_predict_g(ax, ay, az, probs);

        if (id >= 0) {
            xprintf("[posture] id=%d (%s)  p=%.2f  ax=%.3fg ay=%.3fg az=%.3fg\r\n",
                   id, posture_infer_label_name(id), probs[id], ax, ay, az);
        }
    }

    // 到達しないが、終了時に後処理
    posture_infer_deinit();
}
