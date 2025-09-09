/*
 * posture_inference.c
 *
 *  Created on: 2025/09/09
 *      Author: k-abe
 */

// posture_infer.c
#include <math.h>
#include <string.h>
#include "MainRuntime_inference.h"
#include "MainRuntime_parameters.h"
#include "posture_inference.h"

// ------- ラベル名（必要に応じて学習データのクラス名に変更してください） -------
static const char* s_labels[] = {
    "normal(label_id_0)", "right(label_id_1)", "left(label_id_2)", "front(label_id_3)", "back(label_id_4)"
};
// ---------------------------------------------------------------------------

_Static_assert(
    (int)NNABLART_MAINRUNTIME_OUTPUT0_SIZE == (int)(sizeof(s_labels)/sizeof(s_labels[0])),
    "Label size must match NNABLART_MAINRUNTIME_OUTPUT0_SIZE. Please edit s_labels."
);

#define NUM_LABELS ((int)(sizeof(s_labels)/sizeof(s_labels[0])))
#define INPUT_SIZE  ((int)NNABLART_MAINRUNTIME_INPUT0_SIZE)
#define OUTPUT_SIZE ((int)NNABLART_MAINRUNTIME_OUTPUT0_SIZE)

static void *s_ctx = NULL;
static float *s_in  = NULL;  // 入力3要素 (ax,ay,az)
static float *s_out = NULL;  // 出力 (ロジット) OUTPUT_SIZE 要素

static inline void softmax_stable(const float *x, int n, float *y) {
    float xmax = x[0];
    for (int i = 1; i < n; ++i) if (x[i] > xmax) xmax = x[i];
    float sum = 0.0f;
    for (int i = 0; i < n; ++i) {
        y[i] = expf(x[i] - xmax);
        sum += y[i];
    }
    if (sum > 0.0f) {
        float inv = 1.0f / sum;
        for (int i = 0; i < n; ++i) y[i] *= inv;
    }
}

static inline void normalize3(float *x, float *y, float *z) {
    const float eps = 1e-8f;
    float nx = *x, ny = *y, nz = *z;
    float m = sqrtf(nx*nx + ny*ny + nz*nz);
    if (m < eps) {
        // ほぼ0の場合はそのまま
        return;
    }
    *x = nx / m; *y = ny / m; *z = nz / m;
}

int posture_infer_num_labels(void) {
    return NUM_LABELS;
}

int posture_infer_init(void) {
    if (s_ctx) return 0; // すでに初期化済み

    // NNCエクスポート済みのパラメータ配列を渡してコンテキスト確保
    s_ctx = nnablart_mainruntime_allocate_context(MainRuntime_parameters);
    if (!s_ctx) return -1;

    // 入出力バッファのポインタ取得
    s_in  = nnablart_mainruntime_input_buffer(s_ctx, 0);
    s_out = nnablart_mainruntime_output_buffer(s_ctx, 0);
    if (!s_in || !s_out) {
        nnablart_mainruntime_free_context(s_ctx);
        s_ctx = NULL; s_in = NULL; s_out = NULL;
        return -2;
    }
    return 0;
}

void posture_infer_deinit(void) {
    if (s_ctx) {
        nnablart_mainruntime_free_context(s_ctx);
        s_ctx = NULL; s_in = NULL; s_out = NULL;
    }
}

int posture_infer_predict_g(float ax_g, float ay_g, float az_g, float *probs_out) {
    if (!s_ctx || !s_in || !s_out) return -1;
    if (INPUT_SIZE != 3) return -2; // 本モデルは3軸入力前提

    // g単位の方向（正規化ベクトル）に変換
    float nx = ax_g, ny = ay_g, nz = az_g;
    normalize3(&nx, &ny, &nz);

    // 入力セット
    s_in[0] = nx;
    s_in[1] = ny;
    s_in[2] = nz;

    // 推論実行
    if (nnablart_mainruntime_inference(s_ctx) != 0) {
        return -3;
    }

    // softmaxで確率に変換
    float probs[OUTPUT_SIZE];
    softmax_stable(s_out, OUTPUT_SIZE, probs);

    // Top-1
    int top = 0;
    float best = probs[0];
    for (int i = 1; i < OUTPUT_SIZE; ++i) {
        if (probs[i] > best) { best = probs[i]; top = i; }
    }

    // 呼び出し側に必要なら確率を渡す
    if (probs_out) {
        memcpy(probs_out, probs, sizeof(float)*OUTPUT_SIZE);
    }
    return top;
}

const char* posture_infer_label_name(int id) {
    if (id >= 0 && id < NUM_LABELS) return s_labels[id];
    return "unknown";
}
