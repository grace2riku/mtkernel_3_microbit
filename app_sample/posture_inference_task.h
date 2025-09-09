/*
 * posture_inference_task.h
 *
 *  Created on: 2025/09/09
 *      Author: k-abe
 */

#ifndef APP_SAMPLE_POSTURE_INFERENCE_TASK_H_
#define APP_SAMPLE_POSTURE_INFERENCE_TASK_H_

// イベントフラグのビットパターン
#define INFERENCE_START			(1 << 0)	// 姿勢推定スタート

void init_posture_inference_task(void);
ID get_posture_inference_task_flgid(void);

#endif /* APP_SAMPLE_POSTURE_INFERENCE_TASK_H_ */
