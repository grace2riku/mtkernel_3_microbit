# μT-Kernel × AIの学習環境（micro:bit）
TRONプログラミングコンテスト2025の開発環境・ツール部門に応募する【μT-Kernel × AIの学習環境】の紹介です。

個人で購入可能で安価なライントレースロボットを使用し、
* μT-Kernel
* AI

を気軽に学習、試行錯誤できる学習環境を作成しました。

主要な機能は以下の3つです。
1. ライントレース
2. 強化学習
3. ロボットの姿勢（転倒状態）推論

## 主要機能1. ライントレース
ライントレースは下記のようにロボット制御の基本をバランスよく体験できるテーマだと思い決めました。

* 入力: フォトリフレクターで黒ラインを検出
* 演算: ラインとロボットのズレから進行方向を決定
* 制御: 進行方向にモーター制御

![Image](https://github.com/user-attachments/assets/a64d2d4b-8781-4030-9416-3b7345ba3e38)

## 主要機能2. 強化学習
強化学習は機械学習の学びはじめのテーマとして丁度よいと思い決めました。
強化学習とライントレースの組み合わせで以下が体感できます。

* ロボットがライントレースを強化学習する過程でラインを外れたらロボットを再度ラインに乗せて強化学習を再開する
* 強化学習完了後に、強化学習で得た政策で実際にライントレースする

上のように強化学習の過程と強化学習の結果がライントレースという形で実際に目に見えて、体感できます。

## 主要機能3. ロボットの転倒状態の推論
ロボットの転倒状態の推論は、以下のステップで簡単なエッジAIを体験できる仕組みとしました。

* ロボットの5つの転倒状態（転倒なし、右に転倒、左に転倒、前に転倒、後に転倒）の加速度センサー値を収集する
* 収集したデータをファイル化し整形する
* SonyのNeural Network Console（以下NNC）で収集したデータを学習、評価し転倒状態推定モデルを作成する
* NNCでマイコンに組込み可能なCソースコードを出力する
* 出力したCソースコードをマイコンに組込む。
* ロボットの転倒状態を推論できることを確認する

![μT-Kernel × AIの学習環境_推論実行時](img/μT-Kernel%20×%20AIの学習環境_推論実行時.001.png)

すべてのソースコードは公開し、開発環境構築手順・操作手順を[マニュアル](./operation_manual.md)に書いています。
μT-KernelとAIをライントレースロボットをとおして学び、体験する環境です。


## 操作マニュアル
操作マニュアルは[こちら](./operation_manual.md)です。

## ライセンス

確認中...

| No | ソースコード名称 | OSS名称 | OSSバージョン | ライセンス | OSS参照先 | 著作権者 | 変更有無（変更していたら変更理由） |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | [app_sample/acceleration_sensor.c](./app_sample/acceleration_sensor.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 2 | [app_sample/acceleration_sensor.h](./app_sample/acceleration_sensor.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 3 | [app_sample/accessor.c](./app_sample/accessor.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 4 | [app_sample/accessor.h](./app_sample/accessor.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 5 | [app_sample/ad.c](./app_sample/ad.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 6 | [app_sample/ad.h](./app_sample/ad.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 7 | [app_sample/affine.c](./app_sample/affine.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 8 | [app_sample/affine_fixed.h](./app_sample/affine_fixed.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 9 | [app_sample/affine_fixed16.c](./app_sample/affine_fixed16.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 10 | [app_sample/affine_fixed8.c](./app_sample/affine_fixed8.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 11 | [app_sample/affine_generic.c](./app_sample/affine_generic.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 12 | [app_sample/affine_generic.h](./app_sample/affine_generic.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 13 | [app_sample/affine_internal.h](./app_sample/affine_internal.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 14 | [app_sample/app_main.c](./app_sample/app_main.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 14 | [app_sample/button.c](./app_sample/button.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 15 | [app_sample/button.h](./app_sample/button.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 16 | [app_sample/config.h](./app_sample/config.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 17 | [app_sample/context.h](./app_sample/context.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 18 | [app_sample/Course.c](./app_sample/Course.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 19 | [app_sample/Course.h](./app_sample/Course.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 20 | [app_sample/device_id.c](./app_sample/device_id.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 21 | [app_sample/device_id.h](./app_sample/device_id.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 22 | [app_sample/Drive.c](./app_sample/Drive.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 23 | [app_sample/Drive.h](./app_sample/Drive.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 24 | [app_sample/fixedpoint.c](./app_sample/fixedpoint.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 25 | [app_sample/fixedpoint.h](./app_sample/fixedpoint.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 26 | [app_sample/functions.h](./app_sample/functions.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 27 | [app_sample/gpio.c](./app_sample/gpio.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 28 | [app_sample/gpio.h](./app_sample/gpio.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 29 | [app_sample/Hook.c](./app_sample/Hook.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 30 | [app_sample/Hook.h](./app_sample/Hook.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 31 | [app_sample/iic.h](./app_sample/iic.h) | μT-Kernel × AIの学習環境（micro:bit） |  | ※要確認 |  |  |  |
| 32 | [app_sample/iic_reg.c](./app_sample/iic_reg.c) | μT-Kernel × AIの学習環境（micro:bit） |  | ※要確認 |  | Personal Media Corporation |  |
| 33 | [app_sample/led.c](./app_sample/led.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 35 | [app_sample/led.h](./app_sample/led.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 36 | [app_sample/list.c](./app_sample/list.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 37 | [app_sample/list.h](./app_sample/list.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 38 | [app_sample/log.c](./app_sample/log.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 39 | [app_sample/log.h](./app_sample/log.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 40 | [app_sample/MainRuntime_inference.c](./app_sample/MainRuntime_inference.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 41 | [app_sample/MainRuntime_inference.h](./app_sample/MainRuntime_inference.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 42 | [app_sample/MainRuntime_parameters.c](./app_sample/MainRuntime_parameters.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 43 | [app_sample/MainRuntime_parameters.h](./app_sample/MainRuntime_parameters.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 44 | [app_sample/motor.c](./app_sample/motor.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 45 | [app_sample/motor.h](./app_sample/motor.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 46 | [app_sample/motor_drv.c](./app_sample/motor_drv.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 47 | [app_sample/motor_drv.h](./app_sample/motor_drv.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 48 | [app_sample/Navi.c](./app_sample/Navi.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 49 | [app_sample/Navi.h](./app_sample/Navi.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 50 | [app_sample/network.h](./app_sample/network.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 51 | [app_sample/nrf5_iic.c](./app_sample/nrf5_iic.c) | μT-Kernel × AIの学習環境（micro:bit） |  | ※要確認 |  | Personal Media Corporation |  |
| 52 | [app_sample/ntconf.h](./app_sample/ntconf.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 53 | [app_sample/ntint.h](./app_sample/ntint.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 54 | [app_sample/ntlibc.c](./app_sample/ntlibc.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 55 | [app_sample/ntlibc.h](./app_sample/ntlibc.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 56 | [app_sample/ntopt.c](./app_sample/ntopt.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 57 | [app_sample/ntopt.h](./app_sample/ntopt.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 58 | [app_sample/ntahell.c](./app_sample/ntshell.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 59 | [app_sample/ntshell.h](./app_sample/ntshell.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 60 | [app_sample/ntshell_task.c](./app_sample/ntshell_task.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 61 | [app_sample/ntstdio.c](./app_sample/ntstdio.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 62 | [app_sample/ntstdio.h](./app_sample/ntstdio.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 63 | [app_sample/posture_inference.c](./app_sample/posture_inference.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 64 | [app_sample/posture_inference.h](./app_sample/posture_inference.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 65 | [app_sample/posture_inference_task.c](./app_sample/posture_inference_task.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 66 | [app_sample/posture_inference_task.h](./app_sample/posture_inference_task.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 67 | [app_sample/pwm.c](./app_sample/pwm.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 68 | [app_sample/pwm.h](./app_sample/pwm.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 69 | [app_sample/reinforcement_learning.c](./app_sample/reinforcement_learning.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 70 | [app_sample/reinforcement_learning.h](./app_sample/reinforcement_learning.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 71 | [app_sample/relu.c](./app_sample/relu.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 72 | [app_sample/Road.c](./app_sample/Road.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 73 | [app_sample/Road.h](./app_sample/Road.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 74 | [app_sample/runtime.h](./app_sample/runtime.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 75 | [app_sample/runtime_internal.c](./app_sample/runtime_internal.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 76 | [app_sample/runtime_internal.h](./app_sample/runtime_internal.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 77 | [app_sample/Sensor.c](./app_sample/Sensor.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 78 | [app_sample/Sensor.h](./app_sample/Sensor.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 79 | [app_sample/shape.c](./app_sample/shape.c) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) |  |
| 80 | [app_sample/shape.h](./app_sample/shape.h) | nnabla-c-runtime | 1.38.0 | Apache License, Version 2.0 | [nnabla-c-runtime](https://github.com/sony/nnabla-c-runtime) | [sony](https://github.com/sony) | インクルードファイルのパス変更 |
| 81 | [app_sample/speaker.c](./app_sample/speaker.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 82 | [app_sample/speaker.h](./app_sample/speaker.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 83 | [app_sample/syscalls_min.c](./app_sample/syscalls_min.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 84 | [app_sample/SysCtrl.c](./app_sample/SysCtrl.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 85 | [app_sample/SysCtrl.h](./app_sample/SysCtrl.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 86 | [app_sample/temperature_sensor.c](./app_sample/temperature_sensor.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 87 | [app_sample/temperature_sensor.h](./app_sample/temperature_sensor.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 88 | [app_sample/text_editor.c](./app_sample/text_editor.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 89 | [app_sample/text_editor.h](./app_sample/text_editor.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 90 | [app_sample/text_history.c](./app_sample/text_history.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 91 | [app_sample/text_history.h](./app_sample/text_history.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 92 | [app_sample/time.c](./app_sample/time.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 93 | [app_sample/time.h](./app_sample/time.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 94 | [app_sample/Trace.c](./app_sample/Trace.c) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 95 | [app_sample/Trace.h](./app_sample/Trace.h) | μT-Kernel × AIの学習環境（micro:bit） |  | MIT | [mtkernel_3_microbit](https://github.com/grace2riku/mtkernel_3_microbit) | [grace2riku](https://github.com/grace2riku) |  |
| 96 | [app_sample/usrcmd.c](./app_sample/usrcmd.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 97 | [app_sample/usrcmd.h](./app_sample/usrcmd.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 98 | [app_sample/vtrecv.c](./app_sample/vtrecv.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 99 | [app_sample/vtrecv.h](./app_sample/vtrecv.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 100 | [app_sample/vtsend.c](./app_sample/vtsend.c) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 101 | [app_sample/vtsend.h](./app_sample/vtsend.h) | NT-Shell | 0.3.1 | MIT | [NT-Shell](https://cubeatsystems.com/ntshell/index.html) | [Shinichiro Nakamura (CuBeatSystems)](https://shinta-main-jp.blogspot.com/) |  |
| 102 | [app_sample/xprintf.c](./app_sample/xprintf.c) | xprintf | 2021. 7. 31 | [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/) | [xprintf](https://elm-chan.org/fsw/strf/xprintf_j.html) | [ChaN](https://elm-chan.org/index_j.html) |  |
| 103 | [app_sample/xprintf.h](./app_sample/xprintf.h) | xprintf | 2021. 7. 31 | [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/) | [xprintf](https://elm-chan.org/fsw/strf/xprintf_j.html) | [ChaN](https://elm-chan.org/index_j.html) |  |

