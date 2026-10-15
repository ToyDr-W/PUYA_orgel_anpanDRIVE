//ｱﾝﾊﾟﾝﾏﾝﾄﾞﾗｲﾌﾞﾊﾝﾄﾞﾙを制御するｱﾌﾟﾘｹｰｼｮﾝ


//==============================================
//構成情報(個別ｱﾌﾟﾘｹｰｼｮﾝだけで使うもの)
//==============================================
#define	SLEEP_TMR	1000			//Sleepに入る前の時限[ms]
						//　操作の評価時間よりも長くすること


//==============================================
//固有の作業域
//==============================================
static unsigned char eng_sts;			//ｴﾝｼﾞﾝ状態(0=停止中､1=始動音中､2=定常音中)
static unsigned char wink_sts;			//ｳｨﾝｶｰ状態(0=停止中､1=再生中､
						//　2以上=再生終了[固有処理実行周期])


//==============================================
//ｱﾌﾟﾘの初期設定
//==============================================
static void apl_init(void)
{

DBG_C('\n');
DBG_C('$');

	//LEDﾎﾟｰﾄ設定
	GPIO_OUT(LED_L_GPIO,LED_L_BIT);		//出力ﾓｰﾄﾞ
	GPIO_OUT(LED_R_GPIO,LED_R_BIT);		//出力ﾓｰﾄﾞ
	GPIO_OUT(LED_M_GPIO,LED_M_BIT);		//出力ﾓｰﾄﾞ

	//SWﾎﾟｰﾄ設定
DBG_C('p');
	RCC->APBENR2|=RCC_APBENR2_SYSCFGEN;
					//SYSCFGにｸﾛｯｸ供給
	GPIO_OUT(SW_SEL0_GPIO,SW_SEL0_BIT);	//出力ﾓｰﾄﾞ
	GPIO_OUT(SW_SEL1_GPIO,SW_SEL1_BIT);	//出力ﾓｰﾄﾞ
	GPIO_INPU(SW_SNS0_GPIO,SW_SNS0_BIT);	//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
	GPIO_INPU(SW_SNS1_GPIO,SW_SNS1_BIT);	//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
	GPIO_INPU(SW_SNS2_GPIO,SW_SNS2_BIT);	//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
	GPIO_INPU(SW_SNS3_GPIO,SW_SNS3_BIT);	//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
	GPIO_INPU(SW_SNS4_GPIO,SW_SNS4_BIT);	//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする
	GPIO_INPU(SW_ENG_GPIO,SW_ENG_BIT);	//入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟする

	//Wakeup設定
					//　EXTI0～15(SW)割込みでWakeupする
DBG_C('s');
	//EXTI設定
	//SWの読込みﾎﾟｰﾄは全てGPIOAに収容のためEXTICRは初期値(0x00000000)のままでOK
	EXTI->FTSR=(1<<SW_SNS0_BIT)|
			(1<<SW_SNS1_BIT)|
			(1<<SW_SNS2_BIT)|
			(1<<SW_SNS3_BIT)|
			(1<<SW_SNS4_BIT)|
			(1<<SW_ENG_BIT);	//EXTI割込み許可
DBG_L(EXTI->FTSR);
	NVIC->ISER[0]=
		(1<<EXTI0_1_IRQn)|		//EXTI0_1割込み許可
		(1<<EXTI2_3_IRQn)|		//EXTI2_3割込み許可
		(1<<EXTI4_15_IRQn);		//EXTI4_15割込み許可
DBG_C('-');
DBG_L(NVIC->ISER[0]);
DBG_C('>');
}


//==============================================
//EXTI割込みﾊﾝﾄﾞﾗ
//==============================================
void EXTI0_1_IRQHandler(void)
{

	EXTI->PR=0x00000003;			//EXTI0-1割込みﾌﾗｸﾞをｸﾘｱする
}

void EXTI2_3_IRQHandler(void)
{

	EXTI->PR=0x0000000c;			//EXTI2-3割込みﾌﾗｸﾞをｸﾘｱする
}

void EXTI4_15_IRQHandler(void)
{

	EXTI->PR=0x0000fff0;			//EXTI4-115割込みﾌﾗｸﾞをｸﾘｱする
}


//==============================================
//Sleep処理
//==============================================
static void apl_sleep(void)
{

DBG_C('\n');
DBG_C('s');

	//LED消灯
	GPIO_L(LED_L_GPIO,LED_L_BIT);		//L出力
	GPIO_L(LED_R_GPIO,LED_R_BIT);		//L出力
	GPIO_L(LED_M_GPIO,LED_M_BIT);		//L出力

	//PWMを停止
DBG_C('c');
	dev_pwm_stop();				//PWMを停止する

DBG_C('i');

	//SW_SELをL
	GPIO_L(SW_SEL0_GPIO,SW_SEL0_BIT);	//L出力
	GPIO_L(SW_SEL1_GPIO,SW_SEL1_BIT);	//L出力

	//STOPﾓｰﾄﾞへ移行
	EXTI->IMR=(1<<SW_SNS0_BIT)|
			(1<<SW_SNS1_BIT)|
			(1<<SW_SNS2_BIT)|
			(1<<SW_SNS3_BIT)|
			(1<<SW_SNS4_BIT)|
			(1<<SW_ENG_BIT);	//EXTI割込み許可
DBG_L(EXTI->IMR);
DBG_SYNC();
	PWR->CR1|=PWR_CR1_LPR|PWR_CR1_VOS|PWR_CR1_MRRDY_TIME_Msk;
						//ﾚｷﾞｭﾚｰﾀを低電力､VDD=1.0V､MRRDY_TIMEは5us
	RCC->CFGR=0x00000000;			//SYSCLKはHSI
	__WFI();				//STOPﾓｰﾄﾞへ移行
	EXTI->IMR=0x00000000;			//EXTI割込み禁止
	SystemInit();				//ｼｽﾃﾑｸﾛｯｸを再設定する
DBG_C('j');

	//SW_SELをH
	GPIO_H(SW_SEL0_GPIO,SW_SEL0_BIT);	//H出力
	GPIO_H(SW_SEL1_GPIO,SW_SEL1_BIT);	//H出力

	//PWMを再稼働
DBG_C('k');
	dev_pwm_start();			//PWMを開始する

DBG_C('>');
}


//==============================================
//固有の処理(ｺｰﾙﾊﾞｯｸ関数)
//
//動作仕様
//・ｴﾝｼﾞﾝ音は音声N0
//・ｳｨﾝｶｰ音は音声N1
//・ｱﾝﾊﾟﾝﾏﾝは音声N2
//・救急車･ﾊﾟﾄｶｰ･消防車･ｸﾗｸｼｮﾝ･音符はｵﾙｺﾞｰﾙ演奏
//・上記の4つの出力ﾁｬﾈﾙはｺﾝｶﾚﾝﾄに実行する
//・ｴﾝｼﾞﾝSWがｵﾝすると､ｱﾝﾊﾟﾝﾏﾝが動作中のときはｱﾝﾊﾟﾝﾏﾝを止める
//　始動音→出発進行→定常音連続と進めて､SWｵﾌで停止する
//・ｳｨﾝｶｰSWがｵﾝのときはｳｨﾝｶｰ音を鳴らして､ｳｨﾝｶｰを点滅させる
//・ｸﾗｸｼｮﾝ･ｱﾝﾊﾟﾝﾏﾝ･音符･救急車･ﾊﾟﾄｶｰ･消防車のSWがｵﾝのときは対応する音を鳴らす
//　但し､ｴﾝｼﾞﾝ(始動音と出発)を再生中はｱﾝﾊﾟﾝﾏﾝを動作させない
//・ｴﾝｼﾞﾝ(定常音除く)･ｱﾝﾊﾟﾝﾏﾝ･音符･救急車･ﾊﾟﾄｶｰ･消防車を動作中は顔の麦球を点滅させる
//
//音声
//1.ｴﾝｼﾞﾝ始動
//2.｢出発します｣
//3.ｱｲﾄﾞﾘﾝｸﾞ
//4.ｳｨﾝｶｰ
//5.｢僕ｱﾝﾊﾟﾝﾏﾝ｣
//6以降.収容音声
//
//ｵﾙｺﾞｰﾙ
//1.救急車
//2.ﾊﾟﾄｶｰ
//3.消防車
//4.ｸﾗｸｼｮﾝ
//5以降.収容曲
//==============================================
static void CB_KOYUU(void)
{
	static unsigned short sleep_tmr=0;	//Sleepﾀｲﾏｰ[固有処理実行周期]
	unsigned short sw0_sts,sw1_sts;		//操作SWの状態
	unsigned char res;
	static unsigned char anpan_sts=5;	//ｱﾝﾊﾟﾝﾏﾝの音声番号
	static unsigned char onpu_sts=5;	//音符の曲番号
	static unsigned char hana_sts;		//鼻の点灯時間

	//稼働ﾁｪｯｸ
DBG_C('\n');
DBG_C('e');
	res=0;					//仮に非稼働にしておく
	res|=song.no;				//稼働しているときは稼働中の設定
	res|=voiceN[0].no;
	res|=voiceN[1].no;
	res|=voiceN[2].no;
	res|=wink_sts;
DBG_B(res);
	if(res)					//稼働しているときは
	{
		sleep_tmr=0;			//　Sleep時限をｸﾘｱする

		//鼻麦球の制御
		if(song.no|voiceN[2].no)	//鼻を点滅させるときは
		{
			if(hana_sts<7) GPIO_H(LED_M_GPIO,LED_M_BIT);
						//鼻を点滅する
			else GPIO_L(LED_M_GPIO,LED_M_BIT);
			if(++hana_sts>14) hana_sts=0;
						//鼻の点滅時間を進めて､
						//　時限を超えたら最初に戻す
		}
		else				//鼻を点滅させないときは
		{
			GPIO_L(LED_M_GPIO,LED_M_BIT);
						//鼻を消灯する
			hana_sts=0;		//鼻の点灯時間を初期化する
		}
	}
	else					//稼働していないときは
	{
DBG_C('X');
DBG_S(sleep_tmr);
		GPIO_L(LED_M_GPIO,LED_M_BIT);	//鼻を消灯する
		hana_sts=0;			//鼻の点灯時間を初期化する
		if(++sleep_tmr>SLEEP_TMR/KOYUU_PR)
						//　Sleep時限を超えたら
		{
			apl_sleep();		//　Sleepする
			sleep_tmr=0;		//　Sleep時限をｸﾘｱする
		}
	}

	//操作SW読込み
	//SWは全てGPIOAに収容のため纏めて読む
	GPIO_L(SW_SEL0_GPIO,SW_SEL0_BIT);	//L出力
	wait_us(10);				//電圧安定時間
	sw0_sts=~GPIOA->IDR;			//SNSを読込み､正論理にする
DBG_S(sw0_sts);
	GPIO_H(SW_SEL0_GPIO,SW_SEL0_BIT);	//H出力
	GPIO_L(SW_SEL1_GPIO,SW_SEL1_BIT);	//L出力
	wait_us(10);				//電圧安定時間
	sw1_sts=~GPIOA->IDR;			//SNSを読込み､正論理にする
	GPIO_H(SW_SEL1_GPIO,SW_SEL1_BIT);	//H出力
DBG_C('-');
DBG_S(sw1_sts);

	//ｴﾝｼﾞﾝ音の制御
DBG_C('e');
	if(sw0_sts&SW_ENG_MSK)			//ｴﾝｼﾞﾝSWがｵﾝのとき
	{
DBG_C('o');
DBG_B(eng_sts);
		if(!eng_sts)			//ｴﾝｼﾞﾝ停止中だったら
		{
			VOICEN_SEL(2,0);	//ｱﾝﾊﾟﾝﾏﾝを止める
			VOICEN_SEL(0,1);	//始動音を開始する
			eng_sts=1;		//始動音中にする
		}
	}
	else					//ｴﾝｼﾞﾝSWがｵﾌのとき
	{
DBG_C('x');
		if(eng_sts)			//ｴﾝｼﾞﾝ動作中だったら
		{
			VOICEN_SEL(0,0);	//ｴﾝｼﾞﾝ音を停止する
			eng_sts=0;		//ｴﾝｼﾞﾝ停止中にする
		}
	}

	//ｳｨﾝｶｰの制御
DBG_C('w');
	if(sw0_sts&(SW_WIN_R_MSK|SW_WIN_L_MSK))	//ｳｨﾝｶｰSWがｵﾝのとき
	{
DBG_C('o');
DBG_B(wink_sts);
		if(wink_sts==0)			//ｳｨﾝｶｰ停止中だったとき
		{
DBG_C('#');
			VOICEN_SEL(1,4);	//ｳｨﾝｶｰ音を開始する
			if(sw0_sts&SW_WIN_R_MSK)//ｳｨﾝｶｰSW右がｵﾝのとき
			{
				GPIO_H(LED_R_GPIO,LED_R_BIT);
						//ｳｨﾝｶｰSW右を点灯する
			}
			if(sw0_sts&SW_WIN_L_MSK)//ｳｨﾝｶｰSW左がｵﾝのとき
			{
				GPIO_H(LED_L_GPIO,LED_L_BIT);
						//ｳｨﾝｶｰSW左を点灯する
			}
		}
		else if(wink_sts==7)		//点灯時間が満了したら
		{
DBG_C('$');
			GPIO_L(LED_R_GPIO,LED_R_BIT);
						//右を消灯する
			GPIO_L(LED_L_GPIO,LED_L_BIT);
						//左を消灯する
		}
		if(++wink_sts>14) wink_sts=0;	//経過時間を進めて､
						//　周期が満了したら最初に戻す
	}
	else					//ｳｨﾝｶｰSWがｵﾌのとき
	{
DBG_C('x');
DBG_B(wink_sts);
		if(wink_sts)			//ｳｨﾝｶｰ動作中だったとき
		{
DBG_C('%');
			VOICEN_SEL(1,0);	//ｳｨﾝｶｰ音を停止する
			GPIO_L(LED_R_GPIO,LED_R_BIT);
						//右を消灯する
			GPIO_L(LED_L_GPIO,LED_L_BIT);
						//左を消灯する
			wink_sts=0;		//ｳｨﾝｶｰを停止中にする
		}
	}

	//ｸﾗｸｼｮﾝの制御
DBG_C('h');
	if(sw0_sts&SW_HORN_MSK)			//ｸﾗｸｼｮﾝSWがｵﾝのとき
	{
DBG_B(song.no);
		if(song.no!=4) SONG_SEL(4);	//ｸﾗｸｼｮﾝを演奏中でなければ
						//　ｸﾗｸｼｮﾝを鳴らす
	}
	else					//ｸﾗｸｼｮﾝSWがｵﾌのとき
	{
		if(song.no==4) SONG_SEL(0);	//ｸﾗｸｼｮﾝを演奏中のときは停止する
	}

	//ｱﾝﾊﾟﾝﾏﾝの制御
DBG_C('a');
	if(sw1_sts&SW_ANPAN_MSK)		//ｱﾝﾊﾟﾝﾏﾝSWがｵﾝのとき
	{
DBG_C('o');
DBG_B(voiceN[2].no);
		if(!voiceN[2].no)		//ｱﾝﾊﾟﾝﾏﾝを再生していないとき
		{
			VOICEN_SEL(2,anpan_sts);//ｱﾝﾊﾟﾝﾏﾝを開始する
			if(++anpan_sts>VOICEN_SU) anpan_sts=5;
						//ｱﾝﾊﾟﾝﾏﾝを進めて､
						//　実装数を超えたら最初に戻す
		}
	}

	//音符の制御
DBG_C('o');
	if(sw1_sts&SW_ONPU_MSK)			//音符SWがｵﾝのとき
	{
DBG_C('o');
DBG_B(song.no);
		if(song.no!=onpu_sts)		//目的の曲を演奏していなければ
		{
			SONG_SEL(onpu_sts);	//　曲を開始する
		}
	}
	else
	{
		if(song.no==onpu_sts)		//目的の曲を演奏しているとき
		{
			if(++onpu_sts>SONG_IDX.no) onpu_sts=5;
						//曲を進めて､
						//　実装数を超えたら最初に戻す
		}
	}

	//救急車の制御
DBG_C('k');
	if(sw1_sts&SW_AMB_MSK)			//救急車SWがｵﾝのとき
	{
DBG_C('o');
DBG_B(song.no);
		if(song.no!=1) SONG_SEL(1);	//救急車を演奏していなければ
						//　救急車を演奏する
	}
	else					//救急車SWがｵﾌのとき
	{
DBG_C('x');
DBG_B(song.no);
		if(song.no==1) SONG_SEL(0);	//救急車を演奏していたら停止する
	}

	//ﾊﾟﾄｶｰの制御
DBG_C('p');
	if(sw1_sts&SW_PAT_MSK)			//ﾊﾟﾄｶｰSWがｵﾝのとき
	{
DBG_C('o');
DBG_B(song.no);
		if(song.no!=2) SONG_SEL(2);	//ﾊﾟﾄｶｰを演奏していなければ
						//　ﾊﾟﾄｶｰを演奏する
	}
	else					//ﾊﾟﾄｶｰSWがｵﾌのとき
	{
DBG_C('x');
		if(song.no==2) SONG_SEL(0);	//ﾊﾟﾄｶｰを演奏していたら停止する
	}

	//消防車の制御
DBG_C('f');
	if(sw1_sts&SW_FIRE_MSK)			//消防車SWがｵﾝのとき
	{
DBG_C('o');
DBG_B(song.no);
		if(song.no!=3) SONG_SEL(3);	//消防車を演奏していなければ
						//　消防車を演奏する
	}
	else					//消防車SWがｵﾌのとき
	{
DBG_C('x');
		if(song.no==3) SONG_SEL(0);	//消防車を演奏していたら停止する
	}

DBG_C('>');
}


//==============================================
//演奏終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_SONG_END(void)
{
}


//==============================================
//音声N再生終了時の処理(ｺｰﾙﾊﾞｯｸ関数)
//==============================================
static void CB_VOICEN_END(
	unsigned char ch)			//音声Nのch番号
{

	//ｴﾝｼﾞﾝの制御
	if(ch==0)
	{
		if(eng_sts==1)			//始動音中だったとき
		{
			VOICEN_SEL(0,2);	//｢出発します｣を開始する
			eng_sts=2;		//｢出発します｣中にする
		}
		else if(eng_sts==2)		//｢出発します｣中だったとき
		{
			VOICEN_SEL(0,3);	//定常音を開始する
			eng_sts=3;		//定常音中にする
		}
		else if(eng_sts==3)		//定常音中だったとき
		{
			VOICEN_SEL(0,3);	//定常音を連続再生する
		}
	}
}
