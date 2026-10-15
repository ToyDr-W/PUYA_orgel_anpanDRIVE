//#define	SHDBS		//ﾃﾞﾊﾞｸﾞ信号を出力するときに宣言する
//#define	SHDBG		//ﾃﾞﾊﾞｸﾞ情報を出力するときに宣言する
//#define	SHBPS	115200	//ﾃﾞﾊﾞｸﾞ情報のﾎﾞｰﾚｰﾄを宣言する(ﾃﾞﾌｫﾙﾄ値は38400bps)
//#define	SHDBG_SOFT	//ﾃﾞﾊﾞｸﾞ情報のUARTをｿﾌﾄ実装するときに宣言する		

#define		SLEEP_EN	//Sleep機能を実装するときに宣言する


/*
PUYA32電子ｵﾙｺﾞｰﾙ演奏+音声再生ver1_4をﾍﾞｰｽに
ｱﾝﾊﾟﾝﾏﾝﾄﾞﾗｲﾌﾞﾊﾝﾄﾞﾙ(ｱｶﾞﾂﾏ)旧版のﾏｲｺﾝ換装用にｶｽﾀﾏｲｽﾞした｡
これは根のｿｰｽｺｰﾄﾞ

・ﾀｰｹﾞｯﾄはPY32F002A_16ﾋﾟﾝ(内部48MHzｸﾛｯｸ)

・操作SWのﾏﾄﾘｸｽ構成は以下とする｡SEL､SNSとも負論理､内部ﾌﾟﾙｱｯﾌﾟ｡

------+--------+---------+---------+--------+--------
　　　| SNS0　 | SNS1 　 | SNS2　  | SNS3　 | SNS4
------+--------+---------+---------+--------+--------
 SEL0 |        | ｳｨﾝｶｰ右 | ｳｨﾝｶｰ左 | ｸﾗｸｼｮﾝ |
------+--------+---------+---------+--------+--------
 SEL1 | 音符   | 救急車  | ﾊﾟﾄｶｰ   | 消防車 | ｱﾝﾊﾟﾝﾏﾝ
------+--------+---------+---------+--------+--------

・ｴﾝｼﾞﾝSWはﾍﾞﾀ読みする｡負論理､内部ﾌﾟﾙｱｯﾌﾟ｡

・Sleep時はSEL0とSEL1をLとし､SNS0～4の立下り変化でEXTIによりWakeupさせる｡


//==============================================
//PY32のﾋﾟﾝ接
//==============================================
PY32F002A(16ﾋﾟﾝ)のﾎﾟｰﾄの割当て
1:(PA7)ｳｨﾝｶｰ左駆動(正論理)
2:(PB0)ｳｨﾝｶｰ右駆動(正論理)
3:(PB1)/(TIM1_CH3N)逆相出力
4:Vcc
5:(PA9)/(TIM1_CH2)正相出力
6:(PA13-SWD)SW_ENG
7:(PA14-SWC)ﾃﾞﾊﾞｸﾞ情報/(USART1_TX)ﾃﾞﾊﾞｸﾞ情報
8:(PF2-NRST)麦球駆動(正論理)､ﾃﾞﾊﾞｸﾞ信号=====>PF2はGPIO機能に設定しておくこと
9:(PF0-OSCIN)SW_SEL0(負論理)
10:(PF1-OSCOUT)SW_SEL1(負論理)
11:(PA0)SW_SNS0(負論理)
12:(PA1)BOOT0代替､SW_SNS1(負論理)
13:Vss
14:(PA2)SW_SNS2(負論理)
15:(PA3)SW_SNS3(負論理)
16:(PA6)SW_SNS4(負論理)
*/


//==============================================
//ﾍｯﾀﾞﾌｧｲﾙの呼込み
//==============================================
#include "py32f0xx.h"				//ｼｽﾃﾑのﾍｯﾀﾞﾌｧｲﾙ
#include "orgel_cnf.h"				//構成情報
#include "../orgel.h"				//ｱﾌﾟﾘのﾍｯﾀﾞﾌｧｲﾙ


//==============================================
//構成情報(ｱﾌﾟﾘｹｰｼｮﾝとﾀｰｹﾞｯﾄﾃﾞﾊﾞｲｽに関わるもの)
//==============================================
//ｼｽﾃﾑｸﾛｯｸ
#define	SYS_CLK		48			//ｼｽﾃﾑｸﾛｯｸ周波数[MHz]

//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ定義
#define	DBS_GPIO	GPIOF			//ﾃﾞﾊﾞｸﾞ信号のﾎﾟｰﾄ
#define	DBS_BIT		2			//ﾃﾞﾊﾞｸﾞ信号のﾋﾞｯﾄ

//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ定義
#define	DBG_GPIO	GPIOA			//ﾃﾞﾊﾞｸﾞ情報のﾎﾟｰﾄ
#define	DBG_BIT		14			//ﾃﾞﾊﾞｸﾞ情報のﾋﾞｯﾄ
#define	DBG_AF		1			//UARTの交代機能番号

//PWMのﾎﾟｰﾄ定義
#define	PWM_F_GPIO	GPIOA			//PWM正相のﾎﾟｰﾄ
#define	PWM_F_BIT	9			//PWM正相のﾋﾞｯﾄ
#define	PWM_F_AF	2			//PWM正相の交代機能番号
#define	PWM_R_GPIO	GPIOB			//PWM逆相のﾎﾟｰﾄ
#define	PWM_R_BIT	1			//PWM逆相のﾋﾞｯﾄ
#define	PWM_R_AF	2			//PWM逆相の交代機能番号

//SW_SEL0のﾎﾟｰﾄ定義
#define	SW_SEL0_GPIO	GPIOF			//SW_SEL0のﾎﾟｰﾄ
#define	SW_SEL0_BIT	0			//SW_SEL0のﾋﾞｯﾄ

//SW_SEL1のﾎﾟｰﾄ定義
#define	SW_SEL1_GPIO	GPIOF			//SW_SEL1のﾎﾟｰﾄ
#define	SW_SEL1_BIT	1			//SW_SEL1のﾋﾞｯﾄ

//SW_SNS0のﾎﾟｰﾄ定義
#define	SW_SNS0_GPIO	GPIOA			//SW_SNS0のﾎﾟｰﾄ
#define	SW_SNS0_BIT	0			//SW_SNS0のﾋﾞｯﾄ
#define	SW_ONPU_MSK	(1<<SW_SNS0_BIT)	//音符ﾎﾞﾀﾝのﾏｽｸ

//SW_SNS1のﾎﾟｰﾄ定義
#define	SW_SNS1_GPIO	GPIOA			//SW_SNS1のﾎﾟｰﾄ
#define	SW_SNS1_BIT	1			//SW_SNS1のﾋﾞｯﾄ
#define	SW_WIN_R_MSK	(1<<SW_SNS1_BIT)	//ｳｨﾝｶｰ右SWのﾏｽｸ
#define	SW_AMB_MSK	(1<<SW_SNS1_BIT)	//救急車ﾎﾞﾀﾝのﾏｽｸ

//SW_SNS2のﾎﾟｰﾄ定義
#define	SW_SNS2_GPIO	GPIOA			//SW_SNS2のﾎﾟｰﾄ
#define	SW_SNS2_BIT	2			//SW_SNS2のﾋﾞｯﾄ
#define	SW_WIN_L_MSK	(1<<SW_SNS2_BIT)	//ｳｨﾝｶｰ左SWのﾏｽｸ
#define	SW_PAT_MSK	(1<<SW_SNS2_BIT)	//ﾊﾟﾄｶｰﾎﾞﾀﾝのﾏｽｸ

//SW_SNS3のﾎﾟｰﾄ定義
#define	SW_SNS3_GPIO	GPIOA			//SW_SNS3のﾎﾟｰﾄ
#define	SW_SNS3_BIT	3			//SW_SNS3のﾋﾞｯﾄ
#define	SW_HORN_MSK	(1<<SW_SNS3_BIT)	//ｸﾗｸｼｮﾝﾎﾞﾀﾝのﾏｽｸ
#define	SW_FIRE_MSK	(1<<SW_SNS3_BIT)	//消防車ﾎﾞﾀﾝのﾏｽｸ

//SW_SNS4のﾎﾟｰﾄ定義
#define	SW_SNS4_GPIO	GPIOA			//SW_SNS4のﾎﾟｰﾄ
#define	SW_SNS4_BIT	6			//SW_SNS4のﾋﾞｯﾄ
#define	SW_ANPAN_MSK	(1<<SW_SNS4_BIT)	//ｱﾝﾊﾟﾝﾏﾝﾎﾞﾀﾝのﾏｽｸ

//SW_ENGINEのﾎﾟｰﾄ定義
#define	SW_ENG_GPIO	GPIOA			//SW_ENGINEのﾎﾟｰﾄ
#define	SW_ENG_BIT	13			//SW_ENGINEのﾋﾞｯﾄ
#define	SW_ENG_MSK	(1<<SW_ENG_BIT)		//ｴﾝｼﾞﾝSWのﾏｽｸ

//LED駆動のﾎﾟｰﾄ定義
#define	LED_L_GPIO	GPIOA			//ｳｨﾝｶｰ左のﾎﾟｰﾄ
#define	LED_L_BIT	7			//ｳｨﾝｶｰ左のﾋﾞｯﾄ
#define	LED_R_GPIO	GPIOB			//ｳｨﾝｶｰ右のﾎﾟｰﾄ
#define	LED_R_BIT	0			//ｳｨﾝｶｰ右のﾋﾞｯﾄ
#define	LED_M_GPIO	GPIOF			//麦球のﾎﾟｰﾄ
#define	LED_M_BIT	2			//麦球のﾋﾞｯﾄ


#if	PART_N>0
//==============================================
//ｿﾝｸﾞﾃﾞｰﾀのｲﾝﾃﾞｯｸｽ
//==============================================
extern const struct SONG_IDX SONG_IDX;		//曲ｲﾝﾃﾞｯｸｽ実体
#endif


#if	VOICE_N>0
//==============================================
//音声ﾃﾞｰﾀ
//==============================================
#include	"..\voice_ENG_START.c"
#include	"..\voice_ENG_ANPAN.c"
#include	"..\voice_ENG_IDLE.c"
#include	"..\voice_WINKER.c"
#include	"..\voice_BOKU.c"
#include	"..\voice_ISSHONI.c"
//#include	"..\voice_ANZEN.c"
//#include	"..\voice_GENKI.c"


//==============================================
//音声ﾃﾞｰﾀ(ﾌﾟﾛｸﾞﾗﾑﾌﾗｯｼｭ)のｲﾝﾃﾞｯｸｽ
//==============================================
static const struct VOICEN_HDR voiceN_idx[]=
{
	von_mac(voice_ENG_START,sizeof(voice_ENG_START),4000)	//1
	von_mac(voice_ENG_ANPAN,sizeof(voice_ENG_ANPAN),4000)	//2
	von_mac(voice_ENG_IDLE,sizeof(voice_ENG_IDLE),4000)	//3
	von_mac(voice_WINKER,sizeof(voice_WINKER),4000)		//4
	von_mac(voice_BOKU,sizeof(voice_BOKU),4000)		//5
	von_mac(voice_ISSHONI,sizeof(voice_ISSHONI),4000)	//6
//	von_mac(voice_ANZEN,sizeof(voice_ANZEN),4000)		//7
//	von_mac(voice_GENKI,sizeof(voice_GENKI),4000)		//8
};
#define VOICEN_SU	(sizeof(voiceN_idx)/sizeof(voiceN_idx[0]))
#endif


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#include "../dev_F002A_16.c"			//F002A_16ﾋﾟﾝﾃﾞﾊﾞｲｽｺｰﾄﾞ
#include "../apl_SW-DRIVE.c"			//ﾄﾞﾗｲﾌﾞﾊﾝﾄﾞﾙのｱﾌﾟﾘｹｰｼｮﾝ
