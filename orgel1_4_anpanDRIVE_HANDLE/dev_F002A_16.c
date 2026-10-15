//==============================================
//ﾎﾟｰﾄｱｸｾｽのﾏｸﾛ定義
//==============================================
#define	GPIO_OUT(gpio,bit)	gpio->MODER=gpio->MODER&~(3<<bit*2)|1<<bit*2
#define	GPIO_OUTOD(gpio,bit)	gpio->OTYPER|=1<<bit,\
				gpio->MODER=gpio->MODER&~(3<<bit*2)|1<<bit*2
#define	GPIO_H(gpio,bit)	gpio->BSRR=1<<bit
#define	GPIO_L(gpio,bit)	gpio->BRR=1<<bit
#define	GPIO_R(gpio,bit)	(gpio->IDR&1<<bit)
#define	GPIO_IN(gpio,bit)	gpio->MODER&=~(3<<bit*2),\
				gpio->PUPDR&=~(3<<bit*2)
#define	GPIO_PU(gpio,bit)	gpio->PUPDR=gpio->PUPDR&~(3<<bit*2)|1<<bit*2
#define	GPIO_PUX(gpio,bit)	gpio->PUPDR&=~(3<<bit*2)
#define	GPIO_INPU(gpio,bit)	gpio->MODER&=~(3<<bit*2),\
				gpio->PUPDR=gpio->PUPDR&~(3<<bit*2)|1<<bit*2
#define	GPIO_INPD(gpio,bit)	gpio->MODER&=~(3<<bit*2),\
				gpio->PUPDR=gpio->PUPDR&~(3<<bit*2)|2<<bit*2
#define	GPIO_AF(gpio,bit,af)	gpio->MODER=gpio->MODER&~(3<<bit*2)|2<<bit*2,\
				gpio->AFR[bit>>3]|=af<<(bit&7)*4


//==============================================
//ｼｽﾃﾑｸﾛｯｸ設定
//==============================================
void SystemInit(void)
{

	FLASH->ACR|=FLASH_ACR_LATENCY;		//ﾌﾗｯｼｭﾚｲﾃﾝｼは1wait
	RCC->ICSCR=RCC->ICSCR&0xffff0000|*(uint32_t *)(0x1fff0f10);
						//HSIは24MHzの工場校正値
//	RCC->ICSCR=RCC->ICSCR&0xffff0000|*(uint32_t *)(0x1fff0f04);
						//HSIは8MHzの工場校正値
	RCC->CR|=(1<<24);;			//PLLｵﾝ
	while(!(RCC->CR&(1<<25))){};		//PLLﾚﾃﾞｨを待つ
	RCC->CFGR|=0x00000002;			//SYSCLKはPLL
	while((RCC->CFGR&0x00000038)!=0x00000010){};
						//PLLに切替るまで待つ
}


//==============================================
//共通ｺｰﾄﾞの呼込み
//==============================================
#include "common.c"		//共通ｺｰﾄﾞを呼込む
#include "orgel.c"		//orgelｴﾝｼﾞﾝを呼込む


//==============================================
//PWM周期割込み処理
//==============================================
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	short pcm;				//PCM値

//DBS_H;					//割込み処理開始を表示する
	TIM1->SR=0;				//割込みﾌﾗｸﾞをｸﾘｱする

	//PCM値を取得
	if(buf_read==buf_write) return;		//ﾃﾞｰﾀが無かったら飛ばす
	pcm=buf[buf_read];			//PCM値を取出す
	if(buf_read==(OUT_BUF_N-1)) buf_read=0;	//ﾊﾞｯﾌｧの後尾になったら先頭に戻す
	else buf_read++;			//ﾊﾞｯﾌｧを進める

	//ﾃﾞｭｰﾃｨｻｲｸﾙを設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
	TIM1->CCR2=PWM_STEP/2+pcm;		//正相を設定する
#endif
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
	TIM1->CCR2=PWM_STEP/2+pcm;		//正相･逆相を設定する
#endif
#if BTL>1				//ﾌﾞﾘｯｼﾞ･ﾌﾞﾚｰｷ出力のとき
	if(pcm>0)				//正値のとき
	{
		TIM1->CCR3=0;			//逆相を止める
		TIM1->CCR2=pcm*2;		//正相を設定する
	}
	else if(pcm<0)				//負値のとき
	{
		TIM1->CCR2=0;			//正相を止める
		TIM1->CCR3=(-pcm)*2;		//逆相を設定する
	}
	else					//停止のとき
	{
		TIM1->CCR2=0;			//正相を止める
		TIM1->CCR3=0;			//逆相を止める
	}
#endif
//DBS_L;					//割込み処理終了を表示する
}


//==============================================
//PWM周期を待つ
//==============================================
static void wait_pwm(void)
{

	TIM1->SR=0;				//ｲﾍﾞﾝﾄﾌﾗｸﾞをｸﾘｱする
	while(!(TIM1->SR&TIM_SR_UIF));		//更新ｲﾍﾞﾝﾄを待つ
}


#if VOICE_C+VOICE_S>0			//SPIを使うとき
//==============================================
//SPIﾎﾟｰﾄをｾｯﾄする
//==============================================
static void spi_set(void)
{

	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	GPIO_OUT(SPI_SS_GPIO,SPI_SS_BIT);	//SSを出力ﾓｰﾄﾞ
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
#ifdef SPI_SOFT				//SPIをｿﾌﾄ実装するとき
#ifndef SHSPI2				//2線SPIでないとき
	GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);	//SCKをL
	GPIO_OUT(SPI_SCK_GPIO,SPI_SCK_BIT);	//SCKを出力ﾓｰﾄﾞ
#endif
#if !defined(SHSPI3)&&!defined(SHSPI2)	//4線SPIのとき
	GPIO_L(SPI_MOSI_GPIO,SPI_MOSI_BIT);	//MOSIをL
	GPIO_OUT(SPI_MOSI_GPIO,SPI_MOSI_BIT);	//MOSIを出力ﾓｰﾄﾞ
#endif
#ifdef SPI_MISO_PU			//MISOをﾌﾟﾙｱｯﾌﾟするとき
	GPIO_INPU(SPI_MISO_GPIO,SPI_MISO_BIT);	//MISOをﾌﾟﾙｱｯﾌﾟ入力
#endif
#ifdef SPI_MISO_PD			//MISOをﾌﾟﾙﾀﾞｳﾝするとき
	GPIO_INPD(SPI_MISO_GPIO,SPI_MISO_BIT);	//MISOをﾌﾟﾙﾀﾞｳﾝ入力
#endif
#if !defined(SPI_MISO_PU)&&!defined(SPI_MISO_PD)
					//MISOのﾌﾟﾙﾀﾞｳﾝ/ﾀﾞｳﾝが無指定のとき
	GPIO_IN(SPI_MISO_GPIO,SPI_MISO_BIT);	//MISOを入力
#endif
#else					//内蔵SPIﾓｼﾞｭｰﾙを使うとき
	GPIO_AF(SPI_SCK_GPIO,SPI_SCK_BIT,SPI_SCK_AF);
						//SCKを交代機能に設定
	GPIO_AF(SPI_MOSI_GPIO,SPI_MOSI_BIT,SPI_MOSI_AF);
						//MOSIを交代機能に設定
	GPIO_AF(SPI_MISO_GPIO,SPI_MISO_BIT,SPI_MISO_AF);
						//MISOを交代機能に設定
#endif
#if VOICE_S==1				//単一ch実装のとき
	spi_addr=0;				//SPIｱﾄﾞﾚｽ設定未にする
#endif
}
#endif


#if VOICE_C+VOICE_S>0&&(defined(SLEEP_EN)||defined(SW_EN))
					//SPIを使うとき
//==============================================
//SPIﾎﾟｰﾄをﾘｾｯﾄする
//==============================================
static void spi_reset(void)
{

	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
#ifndef SHSPI2				//2線SPIでないとき
#ifdef SPI_SCK_PU			//SCKをﾌﾟﾙｱｯﾌﾟするとき
	GPIO_INPU(SPI_SCK_GPIO,SPI_SCK_BIT);	//SCKをﾌﾟﾙｱｯﾌﾟ入力
#endif
#ifdef SPI_SCK_PD			//SCKをﾌﾟﾙﾀﾞｳﾝするとき
	GPIO_INPD(SPI_SCK_GPIO,SPI_SCK_BIT);	//SCKをﾌﾟﾙﾀﾞｳﾝ入力
#endif
#if !defined(SPI_SCK_PU)&&!defined(SPI_SCK_PD)
					//SCKのﾌﾟﾙﾀﾞｳﾝ/ﾀﾞｳﾝが無指定のとき
	GPIO_IN(SPI_SCK_GPIO,SPI_SCK_BIT);	//SCKを入力
#endif
#ifdef SPI_MOSI_PU			//MOSIをﾌﾟﾙｱｯﾌﾟするとき
	GPIO_INPU(SPI_MOSI_GPIO,SPI_MOSI_BIT);	//MOSIをﾌﾟﾙｱｯﾌﾟ入力
#endif
#ifdef SPI_MOSI_PD			//MOSIをﾌﾟﾙﾀﾞｳﾝするとき
	GPIO_INPD(SPI_MOSI_GPIO,SPI_MOSI_BIT);	//MOSIをﾌﾟﾙﾀﾞｳﾝ入力
#endif
#if !defined(SPI_MOSI_PU)&&!defined(SPI_MOSI_PD)
					//MOSIのﾌﾟﾙﾀﾞｳﾝ/ﾀﾞｳﾝが無指定のとき
	GPIO_IN(SPI_MOSI_GPIO,SPI_MOSI_BIT);	//MOSIを入力
#endif
#endif
#if !defined(SHSPI3)&&!defined(SHSPI2)	//4線SPIのとき
#ifdef SPI_MISO_PU			//MISOをﾌﾟﾙｱｯﾌﾟするとき
	GPIO_INPU(SPI_MISO_GPIO,SPI_MISO_BIT);	//MISOをﾌﾟﾙｱｯﾌﾟ入力
#endif
#ifdef SPI_MISO_PD			//MISOをﾌﾟﾙﾀﾞｳﾝするとき
	GPIO_INPD(SPI_MISO_GPIO,SPI_MISO_BIT);	//MISOをﾌﾟﾙﾀﾞｳﾝ入力
#endif
#if !defined(SPI_MISO_PU)&&!defined(SPI_MISO_PD)
					//MISOのﾌﾟﾙﾀﾞｳﾝ/ﾀﾞｳﾝが無指定のとき
	GPIO_IN(SPI_MISO_GPIO,SPI_MISO_BIT);	//MISOを入力
#endif
#endif
}
#endif


//==============================================
//固有の初期設定
//==============================================
static void CB_INIT(void)
{

	//ﾀﾞｳﾝﾛｰﾄﾞ猶予時間
//	wait_ms(2000);

	//低電力設定
	RCC->APBENR1|=RCC_APBENR1_PWREN;	//電源ｲﾝﾀﾌｪｰｽにｸﾛｯｸ供給
	SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk;	//SLEEPDEEP設定

	//GPIO設定
	RCC->IOPENR|=
		RCC_IOPENR_GPIOAEN|		//GPIOA､GPIOB､GPIOFにｸﾛｯｸ供給
		RCC_IOPENR_GPIOBEN|
		RCC_IOPENR_GPIOFEN;

	//PA1ﾁｪｯｸ
	GPIO_INPD(GPIOA,1);			//PA1をﾌﾟﾙﾀﾞｳﾝ入力ﾓｰﾄﾞ
	wait_ms(1);				//電圧安定時間
	if(GPIO_R(GPIOA,1))			//PA1がHだったら
	{
		wait_ms(2000);			//2秒待つ(この間にﾃﾞﾊﾞｶﾞを繋ぐ)
	}

	//GPIO設定の続き
	GPIOA->MODER=GPIOF->MODER=0xffffffff;	//全ﾎﾟｰﾄをｱﾅﾛｸﾞﾓｰﾄﾞにしておく
	GPIOA->PUPDR=GPIOF->PUPDR=0x00000000;	//全ﾎﾟｰﾄのﾌﾟﾙｱｯﾌﾟ/ﾀﾞｳﾝを無効にしておく

	//ﾃﾞﾊﾞｸﾞ信号設定
#ifdef SHDBS				//ﾃﾞﾊﾞｸﾞ信号を出力するとき
	dbs_init();				//ﾃﾞﾊﾞｸﾞ情報を初期化する
//待ち時間のﾃｽﾄ
//for(;;){wait_1us();DBS_H;wait_1us();DBS_L;}
//for(;;){wait_us(10);DBS_H;wait_us(10);DBS_L;}
#endif

	//ﾃﾞﾊﾞｸﾞ情報設定
#if defined(SHDBG)||defined(SHDBG_IN)	//ﾃﾞﾊﾞｸﾞ情報出力か入力するとき
	dbg_init();				//ﾃﾞﾊﾞｸﾞ情報を初期化する
#ifdef SHDBG				//ﾃﾞﾊﾞｸﾞ情報を出力するとき
//ﾃﾞﾊﾞｸﾞ情報出力のﾃｽﾄ
//for(;;){DBG_C('U');}
#endif
#endif

//DBG_C('\n');
//DBG_C('i');

	//I2Cﾎﾟｰﾄ設定
#if VOICE_I>0				//I2Cを使うとき
//DBG_C('a');
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH
	GPIO_OUTOD(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをOD出力
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH
	GPIO_OUTOD(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをOD出力
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH

//DBG_C('b');
	for(;;)					//I2Cｽﾚｰﾌﾞを同期する
	{
		wait_us(1);
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) break;
						//SDAがHになったら抜ける
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH
		wait_us(1);
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
	}
//DBG_C('c');
	wait_us(1);
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL
	wait_us(1);
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH
	wait_us(1);
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH
	wait_us(1);
//DBG_C('d');
#ifndef I2C_SOFT			//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
	RCC->APBENR1|=RCC_APBENR1_I2CEN;	//I2C1にｸﾛｯｸ供給
	GPIO_AF(I2C_SCL_GPIO,I2C_SCL_BIT,I2C_SCL_AF);
						//SCLを交代機能に設定
	GPIO_AF(I2C_SDA_GPIO,I2C_SDA_BIT,I2C_SDA_AF);
						//SDAを交代機能に設定
//DBG_C('e');
	I2C1->CR1=I2C_CR1_SWRST;		//I2Cｿﾌﾄﾘｾｯﾄ
	I2C1->CCR=I2C_CCR_FS|I2C_CCR_DUTY|4;	//FASTﾓｰﾄﾞ､Dutyは1､4分周､約478kbps
//	I2C1->CCR=I2C_CCR_FS|I2C_CCR_DUTY|5;	//FASTﾓｰﾄﾞ､Dutyは1､5分周､約383kbps
	I2C1->CR2=48;				//ｸﾛｯｸは48MHz
	I2C->TRISE=49;				//SCLrise1000ns
	I2C1->CR1|=I2C_CR1_PE;			//I2C有効
//DBG_C('f');
#endif
#endif

	//SPIﾎﾟｰﾄ設定
#if VOICE_C+VOICE_S>0			//SPIを使うとき
	spi_set();				//SPIﾎﾟｰﾄをｾｯﾄする
#ifndef SPI_SOFT			//内蔵SPIﾓｼﾞｭｰﾙを使うとき
	RCC->APBENR2|=RCC_APBENR2_SPI1EN;	//SPI1にｸﾛｯｸ供給
	SPI1->CR1=SPI_CR1_MSTR|			//ﾏｽﾀﾓｰﾄﾞ
		SPI_CR1_SSM|SPI_CR1_SSI|	//SSはｿﾌﾄ管理
		SPI_CR1_BR_0;			//ﾎﾞｰﾚｰﾄ設定(Fpclk/4=12Mbps)
//		SPI_CR1_BR_2;			//ﾎﾞｰﾚｰﾄ設定(Fpclk/32=1.5Mbps)
	SPI1->CR2|=SPI_CR2_FRXTH;		//受信ﾊﾞｯﾌｧ閾値(8ﾋﾞｯﾄ)
	SPI1->CR1|=SPI_CR1_SPE;			//SPIを有効
#endif
#if VOICE_C>0				//音声Cを実装するとき
	mmsd_init();				//SDCを初期化する
#endif
#if VOICE_S>0				//音声Sを実装するとき
	W25_PWR_UP();				//W25をﾊﾟﾜｰｱｯﾌﾟ
#endif
#endif

//DBG_C('p');

	//PWM設定(TIM1)
//DBG_C('p');
	RCC->APBENR2|=RCC_APBENR2_TIM1EN;	//TIM1にｸﾛｯｸ供給
	//TIM1->CR1のCMSﾋﾞｯﾄは00(ｴｯｼﾞｱﾗｲﾝﾄﾞﾓｰﾄﾞ)初期値
	//TIM1->SMCRのSMSﾋﾞｯﾄは000(ｽﾚｰﾌﾞﾓｰﾄﾞ無効)初期値
	//TIM1->PSCは0(ｸﾛｯｸ分周しない)初期値	//ｸﾛｯｸは48MHz
#if BTL==2				//ﾌﾞﾘｯｼﾞ出力のとき
	TIM1->ARR=PWM_STEP-1+PWM_DB_BRG;	//PWM周期(ﾃﾞｯﾄﾞﾊﾞﾝﾄﾞを考慮)
#else
	TIM1->ARR=PWM_STEP-1;			//PWM周期
#endif
	TIM1->BDTR|=TIM_BDTR_AOE;		//自動出力可
	TIM1->CNT=0;				//ｶｳﾝﾀをｸﾘｱ
	TIM1->CR1|=TIM_CR1_CEN;			//ｶｳﾝﾀ有効
//DBG_C('t');

	//PWM設定(TIM1_CH2)
	TIM1->CCMR1|=(TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2PE);
						//CH2をPWMﾓｰﾄﾞ1､ﾌﾟﾘﾛｰﾄﾞ有効
	TIM1->CCER|=TIM_CCER_CC2E;		//CH2をﾋﾟﾝ出力


	GPIO_AF(PWM_F_GPIO,PWM_F_BIT,PWM_F_AF);	//PWM正相を交代機能設定
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
//DBG_C('s');
	while(TIM1->CCR2++<PWM_STEP/2)		//DCが50%になるまでｲﾝｸﾘする
	{
		wait_pwm();			//PWM周期を待つ
	}
#endif
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
//DBG_C('c');
	TIM1->CCR2=PWM_STEP/2;			//CH2のDCを50%
	TIM1->CCER|=TIM_CCER_CC2NE;		//CH2Nをﾋﾟﾝ出力
	GPIO_AF(PWM_C_GPIO,PWM_C_BIT,PWM_C_AF);	//PWMｺﾝﾌﾟﾘを交代機能設定
#endif

	//PWM設定(TIM1_CH3N)
#if BTL>1				//ﾌﾞﾘｯｼﾞ出力､ﾌﾞﾚｰｷ出力のとき
//DBG_C('b');
	TIM1->CCMR2|=TIM_CCMR2_OC3M_2|TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3PE;
						//CH3をPWMﾓｰﾄﾞ1､ﾌﾟﾘﾛｰﾄﾞ有効
	TIM1->CCER|=TIM_CCER_CC3NE;		//CH3Nをﾋﾟﾝ出力
	GPIO_AF(PWM_R_GPIO,PWM_R_BIT,PWM_R_AF);	//PWM逆相を交代機能設定
#endif
#if BTL==3				//ﾌﾞﾚｰｷ出力のとき
//DBG_C('k');
	TIM1->CCER|=TIM_CCER_CC2P|TIM_CCER_CC3NP;//CH2とCH3をｱｸﾃｨﾌﾞLにする
#endif
//DBG_C('q');

	//PWM周期割込み設定
	TIM1->DIER=TIM_DIER_UIE;		//TIM1更新割込み許可
	NVIC->ISER[0]=(1<<TIM1_BRK_UP_TRG_COM_IRQn);
						//TIM1割込み許可
//DBG_C('i');

	//ADC設定(ADC1)
	RCC->APBENR2|=RCC_APBENR2_ADCEN;	//ADCにｸﾛｯｸ供給
	//ADC1->CFGR1(ﾘｾｯﾄ値)			//ｼﾝｸﾞﾙﾓｰﾄﾞ､結果右詰､12ﾋﾞｯﾄ分解能
	ADC1->CFGR2=0x80000000;			//ADCｸﾛｯｸはHSI=24MHz
	ADC->CCR|=ADC_CCR_VREFEN;		//Vref有効
//DBG_C('a');

	//ｱﾌﾟﾘの初期設定(LED設定はｱﾌﾟﾘの初期化で実施)
//DBG_C('p');
	apl_init();

//DBG_C('!');
}


#ifdef	SLEEP_EN			//Sleep機能を実装するとき
//==============================================
//PWMを停止
//==============================================
static void dev_pwm_stop(void)
{

//DBG_C('\n');
//DBG_C('s');
	//DC更新を待つ
//DBG_C('1');
	wait_ms(10);				//10ms間はPWM割込み処理を回す

	//PWMを停止
//DBG_C('2');
	TIM1->DIER=0;				//TIM1割込み禁止

#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
//DBG_C('3');
	//ﾗﾝﾌﾟ下降
	while(TIM1->CCR2)			//DCが0になるまで繰返す
	{
		TIM1->CCR2--;			//DCをﾃﾞｸﾘする
		wait_pwm();			//PWM周期を待つ
	}
#endif
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
//DBG_C('4');
	TIM1->CCR2=PWM_STEP/2;			//DCを50%にする
	wait_pwm();				//PWM周期を待つ
	TIM1->CCER=0;				//PWM出力を止める
#endif
#if BTL>1				//ﾌﾞﾘｯｼﾞ出力､ﾌﾞﾚｰｷ出力のとき
//DBG_C('5');
	TIM1->CCR2=TIM1->CCR3=0;		//DCを0%にする
	wait_pwm();				//PWM周期を待つ
#endif
#if BTL==3				//ﾌﾞﾚｰｷ出力のとき
	TIM1->CCER&=~(TIM_CCER_CC2P|TIM_CCER_CC3NP);
						//CH2とCH3NをｱｸﾃｨﾌﾞHにする
#endif
//DBG_C('6');
}


//==============================================
//PWMを再開
//==============================================
static void dev_pwm_start(void)
{

//DBG_C('\n');
//DBG_C('r');
	//ﾗﾝﾌﾟ上昇
#if BTL==0				//ｼﾝｸﾞﾙ出力のとき
//DBG_C('1');
	while(TIM1->CCR2++<PWM_STEP/2)		//DCが50%になるまでｲﾝｸﾘする
	{
		wait_pwm();			//PWM周期を待つ
	}
#endif
#if BTL==1				//ｺﾝﾌﾟﾘ出力のとき
//DBG_C('2');
	TIM1->CCER|=TIM_CCER_CC2E|TIM_CCER_CC2NE;
						//CH2とCH2Nをﾋﾟﾝ出力
#endif
#if BTL==3				//ﾌﾞﾚｰｷ出力のとき
//DBG_C('3');
	TIM1->CCER|=TIM_CCER_CC2P|TIM_CCER_CC3NP;
						//CH2とCH3をｱｸﾃｨﾌﾞLにする
#endif

	//PWMを再開
//DBG_C('4');
	TIM1->DIER=TIM_DIER_UIE;		//TIM1更新割込み許可
//DBG_C('5');
}
#endif
