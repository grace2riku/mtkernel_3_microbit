/*********************************************************
 * ファイル名 : SysCtrl.h
 * 責務       : システム全体に関する処理
 * 作成日     : 2016.01.18
 * 作成者     : System-san
 *********************************************************/
#ifndef SYSCONTROL_H
#define SYSCONTROL_H

/*********************************************************
 * extern関数宣言
 *********************************************************/
extern void sc_init(void);
extern void sc_term(void);
extern void sc_runningMain(void);
extern void sc_traceMain(void);
extern void sc_endDiag(void);

#endif /* SYSCONTROL_H */
