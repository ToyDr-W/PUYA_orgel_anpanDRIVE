//==============================================
//音声ｲﾝﾃﾞｯｸｽ(VOICE_N用)生成ﾏｸﾛ
//　音声再生処理周期のPWM周期に対するｻﾝﾌﾟﾙ飛び数[1/256]を生成する
//　音声ﾃﾞｰﾀのｱﾄﾞﾚｽはﾌﾟﾛｸﾞﾗﾑﾒﾓﾘのﾊﾞｲﾄｱﾄﾞﾚｽを指定すること
//aaa:音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾊﾞｲﾄ]､bbb:長さ[ﾊﾞｲﾄ]､sss:再生ｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps]
//==============================================
#define	von_mac(aaa,bbb,sss)	{aaa,bbb,(unsigned long)256*sss*PWM_PR/1000000},


//==============================================
//音声ｲﾝﾃﾞｯｸｽ(VOICE_I用)生成ﾏｸﾛ
//　音声再生処理周期のPWM周期に対するｻﾝﾌﾟﾙ飛び数[1/256]を生成する
//　音声ﾃﾞｰﾀのｱﾄﾞﾚｽはI2Cﾌﾗｯｼｭ上のﾊﾞｲﾄｱﾄﾞﾚｽを指定すること
//　ｽﾚｰﾌﾞｱﾄﾞﾚｽはI2Cｽﾚｰﾌﾞｱﾄﾞﾚｽ(ｺﾝﾄﾛｰﾙｺｰﾄﾞ+Rﾋﾞｯﾄ付き)(例.0xa0)を指定すること
//aaa:音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾊﾞｲﾄ]､bbb:長さ[ﾊﾞｲﾄ]､sss:ｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps]､ccc:ｽﾚｰﾌﾞｱﾄﾞﾚｽ
//==============================================
#define	voi_mac(aaa,bbb,sss,ccc)	{aaa,bbb,(unsigned long)256*sss*PWM_PR/1000000,ccc},


//==============================================
//音声ｲﾝﾃﾞｯｸｽ(VOICE_C用)生成ﾏｸﾛ
//　音声再生処理周期のPWM周期に対するｻﾝﾌﾟﾙ飛び数[1/256]を生成する
//　音声ﾃﾞｰﾀのｱﾄﾞﾚｽはSDｶｰﾄﾞ上のﾍﾟｰｼﾞｱﾄﾞﾚｽを指定すること
//　音声ﾃﾞｰﾀの長さはSDｶｰﾄﾞ上のﾍﾟｰｼﾞ数を指定すること
//aaa:音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾍﾟｰｼﾞ]､bbb:長さ[ﾍﾟｰｼﾞ]､sss:ｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps]
//==============================================
#define	voc_mac(aaa,ppp,sss)	{aaa,ppp,(unsigned long)256*sss*PWM_PR/1000000},


//==============================================
//音声ｲﾝﾃﾞｯｸｽ(VOICE_S用)生成ﾏｸﾛ
//　音声再生処理周期のPWM周期に対するｻﾝﾌﾟﾙ飛び数[1/256]を生成する
//　音声ﾃﾞｰﾀのｱﾄﾞﾚｽはSPIﾌﾗｯｼｭ上のﾊﾞｲﾄｱﾄﾞﾚｽを指定すること
//aaa:音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾊﾞｲﾄ]､bbb:長さ[ﾊﾞｲﾄ]､sss:ｻﾝﾌﾟﾘﾝｸﾞﾚｰﾄ[sps]
//==============================================
#define	vos_mac(aaa,bbb,sss)	{aaa,bbb,(unsigned long)256*sss*PWM_PR/1000000},


//==============================================
//音源波形ﾃﾞｰﾀの枠組み(wave_に適用する)
//==============================================
struct WAVE_HDR					//音源波形ﾃﾞｰﾀのﾍｯﾀﾞ
{
	unsigned char data[256];		//波形ﾃﾞｰﾀ(要素数は256固定)
};


//==============================================
//効果音波形ﾃﾞｰﾀの枠組み(efct_に適用する)
//==============================================
struct EFCT_HDR					//効果音波形ﾃﾞｰﾀのﾍｯﾀﾞ
{
	unsigned short smp;			//波形ﾃﾞｰﾀｻﾝﾌﾟﾙ数-1
	unsigned char data[1];			//波形ﾃﾞｰﾀ(要素数はsmp+1)
};


//==============================================
//ｴﾝﾍﾞﾛｰﾌﾟ波形ﾃﾞｰﾀの枠組み(enve_に適用する)
//==============================================
struct ENVE_HDR					//ｴﾝﾍﾞﾛｰﾌﾟ波形ﾃﾞｰﾀのﾍｯﾀﾞ
{
	unsigned char smp;			//波形ﾃﾞｰﾀｻﾝﾌﾟﾙ数-1
	unsigned char data[1];			//波形ﾃﾞｰﾀ(要素数はsmp+1)
};


//==============================================
//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ﾃﾞｰﾀの枠組み(pitch_に適用する)
//==============================================
struct PITCH_HDR				//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ﾃﾞｰﾀのﾍｯﾀﾞ
{
	unsigned char smp;			//波形ﾃﾞｰﾀｻﾝﾌﾟﾙ数-1
	unsigned char data[1];			//波形ﾃﾞｰﾀ(要素数はsmp+1)
};


//==============================================
//曲ｲﾝﾃﾞｯｸｽの枠組み
//==============================================
struct SONG_IDX
{
	unsigned char no;			//曲数
	unsigned char dmy1;
	unsigned short dmy2;
	unsigned long song_hdr[1];		//SONG_IDXから曲ﾍｯﾀﾞへのｵﾌｾｯﾄ
};


//==============================================
//曲ﾊﾟｰﾄの枠組み
//==============================================
struct SONG_PART				//ﾊﾟｰﾄ情報
{
	unsigned long onpu;			//SONG_IDXから音符ﾃﾞｰﾀへのｵﾌｾｯﾄ
	unsigned long ongen;			//SONG_IDXから音源ﾃﾞｰﾀへのｵﾌｾｯﾄ
	unsigned long enve_psn;			//ｴﾝﾍﾞﾛｰﾌﾟ処理ﾌﾟﾘｽｹｰﾙ値
	unsigned long enve;			//SONG_IDXからｴﾝﾍﾞﾛｰﾌﾟ波形ﾍｯﾀﾞへのｵﾌｾｯﾄ
	unsigned long pitch_psn;		//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理ﾌﾟﾘｽｹｰﾙ値
	unsigned long pitch;			//SONG_IDXからﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ﾍｯﾀﾞへのｵﾌｾｯﾄ
};


//==============================================
//曲ﾍｯﾀﾞの枠組み
//==============================================
struct SONG_HDR					//曲ﾍｯﾀﾞ
{
	unsigned long next;			//SONG_IDXから次の曲ﾍｯﾀﾞへのﾎﾟｲﾝﾀ
						//　単曲ではNULL
	unsigned short onka_psn;		//音価ﾌﾟﾘｽｹｰﾙ値
	char key;				//移調度
	unsigned char part_su;			//実装ﾊﾟｰﾄ数
	struct SONG_PART part[1];		//ﾊﾟｰﾄ情報
};


//==============================================
//音声Nﾍｯﾀﾞの枠組み
//==============================================
struct VOICEN_HDR
{
	const unsigned char *adr;		//音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾊﾞｲﾄ]
	unsigned long len;			//音声ﾃﾞｰﾀの長さ[ﾊﾞｲﾄ]
	unsigned short tobi;			//再生時のｻﾝﾌﾟﾙ飛び数[1/256]
};


//==============================================
//音声Iﾍｯﾀﾞの枠組み
//==============================================
struct VOICEI_HDR
{
	unsigned long adr;			//音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾊﾞｲﾄ]
	unsigned long len;			//音声ﾃﾞｰﾀの長さ[ﾊﾞｲﾄ]
	unsigned short tobi;			//再生時のｻﾝﾌﾟﾙ飛び数[1/256]
	unsigned char sla;			//I2Cｽﾚｰﾌﾞｱﾄﾞﾚｽ
};


//==============================================
//音声Cﾍｯﾀﾞの枠組み
//==============================================
struct VOICEC_HDR
{
	unsigned long adr;			//音声ﾃﾞｰﾀのﾍﾟｰｼﾞｱﾄﾞﾚｽ
	unsigned long len;			//音声ﾃﾞｰﾀのﾍﾟｰｼﾞ数
	unsigned short tobi;			//再生時のｻﾝﾌﾟﾙ飛び数[1/256]
};


//==============================================
//音声Sﾍｯﾀﾞの枠組み
//==============================================
struct VOICES_HDR
{
	unsigned long adr;			//音声ﾃﾞｰﾀのｱﾄﾞﾚｽ[ﾊﾞｲﾄ]
	unsigned long len;			//音声ﾃﾞｰﾀの長さ[ﾊﾞｲﾄ]
	unsigned short tobi;			//再生時のｻﾝﾌﾟﾙ飛び数[1/256]
};


//==============================================
//共通制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct KYOTUU					//共通制御ﾃｰﾌﾞﾙ
{
	unsigned short ps;			//固有処理ｺｰﾙﾊﾞｯｸ周期ﾌﾟﾘｽｹｰﾗ
	short pcm;				//出力PCM値
};


//==============================================
//演奏制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct SONG					//演奏制御ﾃｰﾌﾞﾙ
{
	const struct SONG_HDR *song_hdr;	//曲ﾍｯﾀﾞへのﾎﾟｲﾝﾀ
	unsigned short onka_psn;		//ｵﾙｺﾞｰﾙ音価ﾌﾟﾘｽｹｰﾙ値
						//　(初期値はｿﾝｸﾞﾍｯﾀﾞから設定)
	unsigned short onka_ps;			//ｵﾙｺﾞｰﾙ音価ﾌﾟﾘｽｹｰﾗ
	unsigned short onka_kb_psn;		//KB音価ﾌﾟﾘｽｹｰﾙ値(ﾃﾝﾎﾟ=60を設定)
	unsigned short onka_kb_ps;		//KB音価ﾌﾟﾘｽｹｰﾗ
	short pcm;				//全ﾊﾟｰﾄ分のPCM値
	unsigned char no;			//曲番号(1起算､0は演奏終了)
	char key;				//ｷｰ設定値(初期値はｿﾝｸﾞﾍｯﾀﾞから設定)
	unsigned char song_ps;			//演奏ﾌﾟﾘｽｹｰﾗ
};


//==============================================
//ﾊﾟｰﾄ個別制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct PART					//ﾊﾟｰﾄ制御ﾃｰﾌﾞﾙ
{
	const struct SONG_PART *song_part;	//ﾊﾟｰﾄ情報へのﾎﾟｲﾝﾀ(NULLは演奏終了)
	const unsigned short *onpu;		//音符ﾃﾞｰﾀへのﾎﾟｲﾝﾀ
	const struct WAVE_HDR *ongen;		//音源ﾃﾞｰﾀへのﾎﾟｲﾝﾀ
	const struct ENVE_HDR *enve;		//ｴﾝﾍﾞﾛｰﾌﾟ波形ﾍｯﾀﾞへのﾎﾟｲﾝﾀ
	const struct PITCH_HDR *pitch;		//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ﾍｯﾀﾞへのﾎﾟｲﾝﾀ
	const struct EFCT_HDR *efct;		//効果音波形ﾍｯﾀﾞへのﾎﾟｲﾝﾀ
	unsigned long efct_of;			//効果音ｻﾝﾌﾟﾙｵﾌｾｯﾄ
						//　[1/256](整数部24ﾋﾞｯﾄ+端数部8ﾋﾞｯﾄ)
	unsigned short efct_tobi;		//効果音ｻﾝﾌﾟﾙ飛び数
						//　[1/16](整数部8ﾋﾞｯﾄ+端数部8ﾋﾞｯﾄ)
	unsigned short onpu_cd;			//音符ｺｰﾄﾞ
	unsigned short onpu_of;			//音符ﾃｰﾌﾞﾙｵﾌｾｯﾄ
	unsigned short onpu_kb;			//KB演奏用音符ｺｰﾄﾞﾊﾞｯﾌｧ
	unsigned short ongen_of;		//音源波形ｻﾝﾌﾟﾙｵﾌｾｯﾄ
						//　[1/256](整数部8ﾋﾞｯﾄ+端数部8ﾋﾞｯﾄ)
	unsigned short ongen_tobi;		//音源波形ｻﾝﾌﾟﾙ飛び数/
						//　(PWM周期*実行ﾌﾟﾘｽｹｰﾙ値)[1/256]
						//　(整数部8ﾋﾞｯﾄ+端数部8ﾋﾞｯﾄ)
	unsigned short port_tobi_gl;		//音源波形ｻﾝﾌﾟﾙ飛び数ｺﾞｰﾙ/
						//　(PWM周期*実行ﾌﾟﾘｽｹｰﾙ値)[1/256]
						//　(整数部8ﾋﾞｯﾄ+端数部8ﾋﾞｯﾄ)
	unsigned short port_tobi_ps;		//音源波形ｻﾝﾌﾟﾙ飛び数増減量/ﾌﾟﾘｽｹｰﾙ/
						//　(PWM周期*実行ﾌﾟﾘｽｹｰﾙ値)[1/256]
						//　(整数部8ﾋﾞｯﾄ+端数部8ﾋﾞｯﾄ)
	unsigned short velo;			//ﾍﾞﾛｼﾃｨ値
						//　音符ﾃﾞｰﾀの指定値にPWM_PRを反映して設定
						//　音符ﾃﾞｰﾀにはORGEL_VOLが反映されている
	unsigned short velo_enve;		//ｴﾝﾍﾞﾛ-ﾌﾟ処理反映後のﾍﾞﾛｼﾃｨ値
						//　PWM_PRを反映する
	unsigned short enve_psn;		//ｴﾝﾍﾞﾛｰﾌﾟ処理実行のﾌﾟﾘｽｹｰﾙ値
						//　ｿﾝｸﾞﾍｯﾀﾞと音符ﾃﾞｰﾀの積を設定
						//　(0のときはﾀｲ･ｽﾗｰ演奏)
	unsigned short enve_ps;			//ｴﾝﾍﾞﾛｰﾌﾟ処理実行のﾌﾟﾘｽｹｰﾗ
	unsigned short pitch_psn;		//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理実行のﾌﾟﾘｽｹｰﾙ値
						//　ｿﾝｸﾞﾍｯﾀﾞと音符ﾃﾞｰﾀの積を設定
						//　(0のときはﾋﾟｯﾁﾍﾞﾝﾄﾞしない)
	unsigned short pitch_ps;		//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理実行のﾌﾟﾘｽｹｰﾗ
	unsigned char onka_zan;			//音価ｶｳﾝﾄ残
	unsigned char enve_of;			//ｴﾝﾍﾞﾛｰﾌﾟ波形ｻﾝﾌﾟﾙｵﾌｾｯﾄ
	unsigned char pitch_of;			//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ｻﾝﾌﾟﾙｵﾌｾｯﾄ
	unsigned char rpt[4];			//ﾘﾋﾟｰﾄｶｳﾝﾀ
};


//==============================================
//音声N再生制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct VOICEN					//音声N再生制御ﾃｰﾌﾞﾙ
{
	unsigned long begin;			//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽ[1/256]
	unsigned long end;			//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽ[1/256]
	unsigned short of;			//再生処理のｻﾝﾌﾟﾙｵﾌｾｯﾄ[1/256]
	unsigned short tobi;			//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]
	short pcm;				//PCM値(PWM_PRを反映した値)
	unsigned char no;			//音声番号(0は再生終了)
};


//==============================================
//音声I再生制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct VOICEI					//音声I再生制御ﾃｰﾌﾞﾙ
{
	unsigned long begin;			//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽ[1/256]
	unsigned long end;			//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽ[1/256]
	unsigned short of;			//再生処理のｻﾝﾌﾟﾙｵﾌｾｯﾄ[1/256]
	unsigned short tobi;			//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]
	short pcm;				//PCM値
						//　PWM_PRを反映する
	unsigned short no;			//音声番号(0は再生終了)
	unsigned char sla;			//I2Cｽﾚｰﾌﾞｱﾄﾞﾚｽ
};


//==============================================
//音声C再生制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct VOICEC					//音声C再生制御ﾃｰﾌﾞﾙ
{
	unsigned long begin;			//音声ﾃﾞｰﾀの開始ﾍﾟｰｼﾞｱﾄﾞﾚｽ(1ﾍﾟｰｼﾞは256ﾊﾞｲﾄ)
	unsigned long end;			//音声ﾃﾞｰﾀの終了ﾍﾟｰｼﾞｱﾄﾞﾚｽ(1ﾍﾟｰｼﾞは256ﾊﾞｲﾄ)
	unsigned short bct;			//ﾌﾞﾛｯｸ内ﾃﾞｰﾀｱﾄﾞﾚｽｶｳﾝﾀ
	unsigned short of;			//再生処理のｻﾝﾌﾟﾙｵﾌｾｯﾄ[1/256]
	unsigned short tobi;			//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]
	short pcm;				//PCM値
						//　PWM_PRを反映する
	unsigned short no;			//音声番号(0は再生終了)
};


//==============================================
//音声S再生制御ﾃｰﾌﾞﾙの枠組み
//==============================================
struct VOICES					//音声S再生制御ﾃｰﾌﾞﾙ
{
	unsigned long begin;			//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽ[1/256]
	unsigned long end;			//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽ[1/256]
	unsigned short of;			//再生処理のｻﾝﾌﾟﾙｵﾌｾｯﾄ[1/256]
	unsigned short tobi;			//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]
	short pcm;				//PCM値
						//　PWM_PRを反映する
	unsigned short no;			//音声番号(0は再生終了､0xffffは再生終了中)
#if VOICE_S>1&&SPI_BUF_N>1		//SPI音声複数ch且つSPIﾊﾞｯﾌｧ複数のとき
	unsigned char spi_buf_n;		//SPIﾊﾞｯﾌｧﾃﾞｰﾀ個数
	unsigned char spi_buf[SPI_BUF_N];	//SPIﾊﾞｯﾌｧ
#endif
};


//==============================================
//ｺｰﾙﾊﾞｯｸ関数等のﾌﾟﾛﾄﾀｲﾌﾟ宣言
//==============================================
static void CB_INIT(void);			//固有の初期設定
static void CB_KOYUU(void);			//固有の処理
#if PART_N>0				//ｵﾙｺﾞｰﾙ演奏を実装するとき
static void CB_SONG_END(void);			//演奏終了時の処理
#endif
#if VOICE_N>0				//音声N再生を実装するとき
static void CB_VOICEN_END(			//音声N再生終了時の処理
	unsigned char);				//　ch番号
#endif
#if VOICE_I>0				//音声I再生を実装するとき
static void CB_VOICEI_END(void);		//音声I再生終了時の処理
#endif
#if VOICE_C>0				//音声C再生を実装するとき
static void CB_VOICEC_END(void);		//音声C再生終了時の処理
#endif
#if VOICE_S>0				//音声S再生を実装するとき
static void CB_VOICES_END(			//音声S再生終了時の処理
	unsigned char);				//　ch番号
#endif
static void apl_init(void);			//ｱﾌﾟﾘｹｰｼｮﾝの初期化処理


//==============================================
//W25Qｼﾘｰｽﾞ関連定数
//==============================================
#define	W25_ReadData	0x03			//ReadDataｺﾏﾝﾄﾞ
#define	W25_PwrDwn	0xb9			//PowerDownｺﾏﾝﾄﾞ
#define	W25_Re_PwrDwn	0xab			//ReleasePowerDownｺﾏﾝﾄﾞ


//==============================================
//SDｲﾝﾀﾌｪｰｽ関連定数
//==============================================
#define	CMD0		0x40			//ﾘｾｯﾄ
#define	CMD0CRC		0x95			//CMD0のCRC
#define	CMD1		0x41			//MMC初期化完了ﾎﾟｰﾘﾝｸﾞ
#define	CMD1CRC		0x87			//CMD1のCRC
#define	CMD8		0x48			//動作電圧の確認とSDCのﾊﾞｰｼﾞｮﾝﾁｪｯｸ
#define	CMD12		0x4C			//ﾃﾞｰﾀ読込みを停止させるｺﾏﾝﾄﾞ
#define	CMD13		0x4D			//書込みの状態を問い合わせるｺﾏﾝﾄﾞ
#define	CMD16		0x50			//ﾌﾞﾛｯｸｻｲｽﾞ設定
#define	CMD17		0x51			//ｼﾝｸﾞﾙﾌﾞﾛｯｸ読込み要求
#define	CMD24		0x58			//ｼﾝｸﾞﾙﾌﾞﾛｯｸ書込み要求
#define	ACMD41		0x69			//SDの初期化完了ﾎﾟｰﾘﾝｸﾞ
#define	CMD55		0x77			//ACMD41/ACMD23とｾｯﾄで使用するｺﾏﾝﾄﾞ
#define	CMD58		0x7A			//OCRの読出し
