#include <tk/tkernel.h>
#include <tm/tmonitor.h>

//-------- SAADCのレジスタ定義 ------------------------
// SAADCのレジスタ定義
#define SAADC(r)		(SAADC_BASE + SAADC_##r)
#define SAADC_BASE		0x40007000

#define SAADC_TASKS_START	0x000
#define SAADC_TASKS_SAMPLE	0x004
#define SAADC_TASKS_STOP	0x008
#define SAADC_EVENTS_END	0x104
#define SAADC_EVENTS_DONE	0x108
#define SAADC_STATUS		0x400
#define SAADC_ENABLE		0x500

#define SAADC_PSELP(n)	 	(0x510 + (n) * 0x10)
#define SAADC_PSELN(n)	 	(0x514 + (n) * 0x10)
#define SAADC_CONFIG(n)		(0x518 + (n) * 0x10)

#define SAADC_RESOLUTION	0x5F0
#define SAADC_RESULT_PTR	0x62C
#define SAADC_RESULT_MAXCNT	0x630


//-------- SAADCを使ったA/D変換 ---------------------------------
// AD変換の結果を入れるメモリ
#define	SAADC_DATACNT	3

INT	adc_poll_cnt;			// A/D変換の終了までのポーリング回数

// 3チャンネルのアナログ入力値をデジタル値に変換
EXPORT void analogRead3(_H* adc_data){

	out_w(SAADC(PSELP(0)), 1);			// CH[0]のP入力の設定
	out_w(SAADC(CONFIG(0)), 0);			// CH[0]のCONFIGの設定
	
	out_w(SAADC(PSELP(1)), 2);			// CH[1]のP入力の設定
	out_w(SAADC(CONFIG(1)), 0);			// CH[1]のCONFIGの設定

	out_w(SAADC(PSELP(2)), 3);			// CH[2]のP入力の設定
	out_w(SAADC(CONFIG(2)), 0);			// CH[2]のCONFIGの設定

	out_w(SAADC(RESULT_PTR), (UW)adc_data);	// 結果を入れるRAMアドレス
	out_w(SAADC(RESULT_MAXCNT), SAADC_DATACNT);	// 結果を入れるRAMのデータ数

	out_w(SAADC(ENABLE), 1);			// SAADCのイネーブル
	out_w(SAADC(RESOLUTION), 1);		// resolution=10bitの設定

	out_w(SAADC(EVENTS_END), 0);		// 書込完了のフラグをクリア
	out_w(SAADC(TASKS_START), 1);		// A/D変換の開始
	out_w(SAADC(TASKS_SAMPLE), 1);		// サンプル入力
	adc_poll_cnt = 0;

	for(;;){							// A/D変換の書込完了を待つループ
		if(in_w(SAADC(EVENTS_END)))		// 書込完了のレジスタを確認
			 return;
		adc_poll_cnt++;					// ポーリング回数をプラス1
	}
}
