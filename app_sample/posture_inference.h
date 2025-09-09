/*
 * posture_inference.h
 *
 *  Created on: 2025/09/09
 *      Author: k-abe
 */

#ifndef APP_SAMPLE_POSTURE_INFERENCE_H_
#define APP_SAMPLE_POSTURE_INFERENCE_H_

// posture_inference.h
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ラベル数はエクスポート済みの MainRuntime_inference.h の出力サイズに一致します
// (例: 5クラスなら 5)
int  posture_infer_num_labels(void);

// 初期化/終了
// 成功: 0, 失敗: 負数
int  posture_infer_init(void);
void posture_infer_deinit(void);

// 予測 (g単位の加速度を受け取り、方向ベクトルに正規化して推論)
// ax_g, ay_g, az_g : [g] 単位 (例: 0.0〜±2.0程度)
// probs_out        : NULL可。確率(softmax後)を [num_labels] 要素で返す
// 戻り値           : 予測ラベルID (0..num_labels-1), 失敗時は負数
int  posture_infer_predict_g(float ax_g, float ay_g, float az_g, float *probs_out);

// ラベル名の取得（任意）
// id: 0..num_labels-1。未知IDは "unknown" を返す。
const char* posture_infer_label_name(int id);

#ifdef __cplusplus
}
#endif


#endif /* APP_SAMPLE_POSTURE_INFERENCE_H_ */
