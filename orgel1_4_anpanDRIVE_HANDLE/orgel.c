//PUYA電子ｵﾙｺﾞｰﾙVer1_4のｴﾝｼﾞﾝ部
//音価･音高･音符ﾃﾞｰﾀの設計は｢ドキュメント\電子オルゴール.xls｣を参照
//音声ﾃﾞｰﾀと曲ﾃﾞｰﾀの枠組みはorgel.hを参照
//ｿﾝｸﾞ･音符ﾃﾞｰﾀの枠組みはorgel.incを参照
//ﾎﾟｰﾄ割当て､ﾀｲﾐﾝｸﾞ設定は固有処理部を参照


//==============================================
//KB音量ﾃｰﾌﾞﾙ
//==============================================
#if KB_N>0
static const unsigned short kb_vol[]={
	KB0_VOL*PWM_STEP/100,
	KB1_VOL*PWM_STEP/100
};
#endif


//==============================================
//音声N音量ﾃｰﾌﾞﾙ
//==============================================
#if VOICE_N>0
static const unsigned short voiceN_vol[]={
	VOICEN0_VOL*PWM_STEP/100,
	VOICEN1_VOL*PWM_STEP/100,
	VOICEN2_VOL*PWM_STEP/100
};
#endif


//==============================================
//音声S音量ﾃｰﾌﾞﾙ
//==============================================
#if VOICE_S>0
static const unsigned short voiceS_vol[]={
	VOICES0_VOL*PWM_STEP/100,
	VOICES1_VOL*PWM_STEP/100,
	VOICES2_VOL*PWM_STEP/100
};
#endif


//==============================================
//音価ﾃｰﾌﾞﾙ(音価ｺｰﾄﾞをﾌﾟﾘｽｹｰﾙ値に変換する)
//==============================================
static const unsigned char onka_tbl[]={
	2,	//1=32分3連符		=kR32
	4,	//2=16分3連符		=kR16
	6,	//3=32分音符		=kK32
	8,	//4=8分3連符		=kR8
	9,	//5=付点32分音符	=kP32
	12,	//6=16分音符		=kK16
	16,	//7=4分3連符		=kR4
	18,	//8=付点16分音符	=kP16
	24,	//9=8分音符		=kK8
	32,	//10=2分3連符		=kR2
	36,	//11=付点8分音符	=kP8
	48,	//12=4分音符		=kK4
	64,	//13=全3連符		=kR1
	72,	//14=付点4分音符	=kP4
	96,	//15=2分音符		=kK2
	144,	//16=付点2分音符	=kP2
	192,	//17=全音符		=kK1
	80,	//18=kW80		=kW80
	84,	//19=kW84		=kW84
	168,	//20=kW168		=kW168
	255,	//21=kW255		=kW255
};


//==============================================
//ﾎﾟﾙﾀﾒﾝﾄﾃｰﾌﾞﾙ(音価ｺｰﾄﾞを音源飛び数増減量率/ﾌﾟﾘｽｹｰﾙに変換する)
//==============================================
static const unsigned char port_tbl[]={
	0,		//1=32分3連符		=kR32
	0,		//2=16分3連符		=kR16
	0,		//3=32分音符		=kK32
	0,		//4=8分3連符		=kR8
	0,		//5=付点32分音符	=kP32
	0,		//6=16分音符		=kK16
	0,		//7=4分3連符		=kR4
	4096/(18-1),	//8=付点16分音符	=kP16
	4096/(24-1),	//9=8分音符		=kK8
	4096/(32-1),	//10=2分3連符		=kR2
	4096/(36-1),	//11=付点8分音符	=kP8
	4096/(48-1),	//12=4分音符		=kK4
	4096/(64-1),	//13=全3連符		=kR1
	4096/(72-1),	//14=付点4分音符	=kP4
	4096/(96-1),	//15=2分音符		=kK2
	4096/(144-1),	//16=付点2分音符	=kP2
	4096/(192-1),	//17=全音符		=kK1
	4096/(80-1),	//18=kW80		=kW80
	4096/(84-1),	//19=kW84		=kW84
	4096/(168-1),	//20=kW168		=kW168
	4096/(255-1),	//21=kW255		=kW255
};


//==============================================
//音高ｺｰﾄﾞをｵｸﾀｰﾌﾞ･音名に変換するﾃｰﾌﾞﾙ
//　上位ﾆﾌﾞﾙ:ｵｸﾀｰﾌﾞ番号(0起算)(音符ﾃﾞｰﾀ上のｵｸﾀｰﾌﾞ番号とは逆順になっていることに留意すること)
//　下位ﾆﾌﾞﾙ:音名番号(0起算)(音符ﾃﾞｰﾀ上の音名番号とは境界が異なることに留意すること)
//==============================================
static const unsigned char onkou_mei_tbl[]={
	0x58,	//0:Do
	0x59,	//1:DoS/ReF
	0x5a,	//2:Re
	0x5b,	//3:ReS/MiF
	0x40,	//4:Mi
	0x41,	//5:Fa
	0x42,	//6:FaS/SoF
	0x43,	//7:So
	0x44,	//8:SoS/LaF
	0x45,	//9:La
	0x46,	//10:LaS/SiF
	0x47,	//11:Si
	0x48,	//12:Do
	0x49,	//13:DoS/ReF
	0x4a,	//14:Re
	0x4b,	//15:ReS/MiF
	0x30,	//16:Mi
	0x31,	//17:Fa
	0x32,	//18:FaS/SoF
	0x33,	//19:So
	0x34,	//20:SoS/LaF
	0x35,	//21:La
	0x36,	//22:LaS/SiF
	0x37,	//23:Si
	0x38,	//24:Do
	0x39,	//25:DoS/ReF
	0x3a,	//26:Re
	0x3b,	//27:ReS/MiF
	0x20,	//28:Mi
	0x21,	//29:Fa
	0x22,	//30:FaS/SoF
	0x23,	//31:So
	0x24,	//32:SoS/LaF
	0x25,	//33:La
	0x26,	//34:LaS/SiF
	0x27,	//35:Si
	0x28,	//36:Do
	0x29,	//37:DoS/ReF
	0x2a,	//38:Re
	0x2b,	//39:ReS/MiF
	0x10,	//40:Mi
	0x11,	//41:Fa
	0x12,	//42::FaS/SoF
	0x13,	//43:So
	0x14,	//44:SoS/LaF
	0x15,	//45:La
	0x16,	//46:LaS/SiF
	0x17,	//47:Si
	0x18,	//48:Do
	0x19,	//49:DoS/ReF
	0x1a,	//50:Re
	0x1b,	//51:ReS/MiF
	0x00,	//52:Mi
	0x01,	//53:Fa
	0x02,	//54:FaS/SoF
	0x03,	//55:So
	0x04,	//56:SoS/LaF
	0x05,	//57:La
	0x06,	//58:LaS/SiF
	0x07,	//59:Si
	0x08,	//60:Do
	0x09,	//61:DoS/ReF
	0x0a,	//62:Re
	0x0b,	//63:ReS/MiF
};


//==============================================
//音高ﾃｰﾌﾞﾙ
//==============================================
#define	smp_mac(fff)	((unsigned long)fff*256/10*256/25*PWM_PR/10*SONG_PS/4000+5)/10
						//fff=周波数[0.01Hz]
static const unsigned short onkou_tbl[]={
	smp_mac(131851),	//4:Mi
	smp_mac(139691),	//5:Fa
	smp_mac(147998),	//6:FaS/SoF
	smp_mac(156798),	//7:So
	smp_mac(166122),	//8:SoS/LaF
	smp_mac(176000),	//9:La
	smp_mac(186466),	//10:LaS/SiF
	smp_mac(197553),	//11:Si
	smp_mac(209300),	//0:Do
	smp_mac(221746),	//1:DoS/ReF
	smp_mac(234932),	//2:Re
	smp_mac(248902),	//3:ReS/MiF
};


//==============================================
//PWM出力ﾊﾞｯﾌｧ
//==============================================
static volatile short buf[OUT_BUF_N];		//ﾊﾞｯﾌｧ
static volatile unsigned char buf_write,buf_read;
						//ﾊﾞｯﾌｧの書込み､読込み位置


//==============================================
//制御ﾃｰﾌﾞﾙ
//==============================================
static struct KYOTUU kyotuu;			//共通制御ﾃｰﾌﾞﾙ
#if	PART_N+KB_N>0
static struct SONG song;			//演奏制御ﾃｰﾌﾞﾙ
static struct PART part[PART_N+KB_N];		//ﾊﾟｰﾄ制御ﾃｰﾌﾞﾙ
#endif
#if	VOICE_N>0
static struct VOICEN voiceN[VOICE_N];		//音声N再生制御ﾃｰﾌﾞﾙ
#endif
#if	VOICE_I>0
static struct VOICEI voiceI;			//音声I再生制御ﾃｰﾌﾞﾙ
#endif
#if	VOICE_C>0
static struct VOICEC voiceC;			//音声C再生制御ﾃｰﾌﾞﾙ
#endif
#if	VOICE_S>0
static struct VOICES voiceS[VOICE_S];		//音声S再生制御ﾃｰﾌﾞﾙ
#endif
#if VOICE_S==1				//単一ch実装のとき
static unsigned char spi_addr;			//SPIｱﾄﾞﾚｽ設定(0=未､1=済)
#endif


//==============================================
//ﾐｭｰﾄ設定処理
//==============================================
//未実装


#if PART_N>0
//==============================================
//曲ﾃﾞｰﾀ初期化処理
//==============================================
static void song_init(void)
{
	struct PART *part_pt;
	unsigned short m,n;
	unsigned char *q;

//DBG_C('\n');
	song.key=song.song_hdr->key;		//移調度を取込む
//DBG_B(song.key);
	song.onka_psn=song.song_hdr->onka_psn;	//音価ﾌﾟﾘｽｹｰﾙ値を取込む
//DBG_S(song.onka_psn);
	song.song_ps=0;				//演奏ﾌﾟﾘｽｹｰﾗをｸﾘｱ
	song.onka_ps=0;				//音価ﾌﾟﾘｽｹｰﾗをｸﾘｱ
//DBG_D(&song,sizeof(song));
	for(m=0,part_pt=part;m<PART_N;m++,part_pt++)
						//ﾊﾟｰﾄ個別制御ﾃｰﾌﾞﾙの初期設定
	{
//DBG_C('\n');
//DBG_C('m');
//DBG_B(m);
//DBG_L(part_pt);
		if(m<song.song_hdr->part_su)	//実装ﾊﾟｰﾄのとき
		{
			for(n=0,q=(unsigned char *)part_pt;n<sizeof(struct PART);n++,q++)
				*q=0;		//先ずｸﾘｱしておく
			part_pt->song_part=&(song.song_hdr->part[m]);
						//ﾊﾟｰﾄ情報へのﾎﾟｲﾝﾀを取得する
//DBG_L(part_pt->song_part);
			part_pt->onpu=(unsigned short *)
				(part_pt->song_part->onpu+(unsigned long)&SONG_IDX);
						//音符ﾃﾞｰﾀへのﾎﾟｲﾝﾀを取得する
//DBG_L(part_pt->onpu);
			part_pt->ongen=(struct WAVE_HDR *)
				(part_pt->song_part->ongen+(unsigned long)&SONG_IDX);
						//音源波形へのﾎﾟｲﾝﾀを取得する
//DBG_L(part_pt->ongen);
			part_pt->enve=(struct ENVE_HDR *)
				(part_pt->song_part->enve+(unsigned long)&SONG_IDX);
						//ｴﾝﾍﾞﾛｰﾌﾟ波形へのﾎﾟｲﾝﾀを取得する
//DBG_L(part_pt->enve);
			part_pt->pitch=(struct PITCH_HDR *)
				(part_pt->song_part->pitch+(unsigned long)&SONG_IDX);
						//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形へのﾎﾟｲﾝﾀを取得する
//DBG_L(part_pt->pitch);
//DBG_D(&part[m],sizeof(part[m]));
		}
		else part_pt->song_part=0;	//未実装ﾊﾟｰﾄは演奏終了にしておく
	}
}


//==============================================
//曲選択処理(API関数)
//==============================================
static void SONG_SEL(
	unsigned char no)			//曲番号(0は演奏中止)
{
	unsigned n;

//DBG_C('\n');
//DBG_C('s');
//DBG_B(no);
//DBG_L(&SONG_IDX);
//DBG_D(&SONG_IDX,16);
//DBG_C('\n');
//DBG_B(song.no);
	if(no)					//演奏開始のとき
	{
//DBG_B(SONG_IDX.no);
		if(song.no==no) return;		//演奏中の曲声番号と同じときは戻る
		if(no>SONG_IDX.no) return;	//実装曲数を超えているときは戻る
		song.no=no;			//曲番号を記憶
//DBG_B(song.no);
//DBG_L(SONG_IDX.song_hdr[song.no-1]);
		song.song_hdr=(struct SONG_HDR *)
			(SONG_IDX.song_hdr[song.no-1]+(unsigned long)&SONG_IDX);
						//曲ﾍｯﾀﾞへのﾎﾟｲﾝﾀを取得
//DBG_L(song.song_hdr);
//DBG_D(song.song_hdr,64);
		song_init();			//曲ﾃﾞｰﾀを初期化
	}
	else					//演奏中止のとき
	{
		for(n=0;n<PART_N;n++) part[n].song_part=0;
						//全ﾊﾟｰﾄを演奏終了にする
		song.no=0;			//演奏終了にする
		song.pcm=0;			//全ﾊﾟｰﾄ分のPWMﾃﾞｭｰﾃｨ値を
						//　ｸﾘｱしておく
	}
//DBG_C('>');
}
#endif


#if KB_N>0
//==============================================
//KB演奏開始処理(API関数)
//==============================================
static void KB_START(
	unsigned char kb,			//KB演奏を開始するﾊﾟｰﾄ番号(0起算)
						//制御表はpart[PART_N+kb]にｱｸｾｽする
	const struct WAVE_HDR *wave,		//音源波形ﾃｰﾌﾞﾙﾍｯﾀﾞ
	const struct ENVE_HDR *enve,		//ｴﾝﾍﾞﾛｰﾌﾟ波形ﾃｰﾌﾞﾙﾍｯﾀﾞ
	const struct PITCH_HDR *pitch)		//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ﾃｰﾌﾞﾙﾍｯﾀﾞ
{
	unsigned short n;
	unsigned char *p;

//DBG_C('\n');
//DBG_C('w');
//DBG_B(kb);
//DBG_L(wave);
//DBG_L(enve);
//DBG_L(pitch);
	if(kb>=KB_N) return;			//KB用ﾊﾟｰﾄ数を超えているときは戻る
//DBG_C('t');
	for(n=0,p=(unsigned char *)&part[PART_N+kb];n<sizeof(part[0]);n++,p++) *p=0;
						//先ずｸﾘｱしておく
	song.onka_kb_psn=KB_PSN;		//KB演奏用音価ﾌﾟﾘｽｹｰﾙ値を設定する
//DBG_S(song.onka_kb_psn);
	part[PART_N+kb].ongen=wave;		//音源ﾃﾞｰﾀｱﾄﾞﾚｽを設定する
	part[PART_N+kb].enve=enve;		//ｴﾝﾍﾞﾛｰﾌﾟﾃﾞｰﾀｱﾄﾞﾚｽを設定する
	part[PART_N+kb].pitch=pitch;		//ﾋﾟｯﾁﾍﾞﾝﾄﾞﾃﾞｰﾀｱﾄﾞﾚｽを設定する
	part[PART_N+kb].velo=kb_vol[kb];	//音量を設定する
	part[PART_N+kb].enve_psn=KB_EPSENV;	//ｴﾝﾍﾞﾛｰﾌﾟ処理実行のﾌﾟﾘｽｹｰﾙ値を設定する
	part[PART_N+kb].pitch_psn=KB_PPSPIT;	//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理実行のﾌﾟﾘｽｹｰﾙ値を設定する
						//　(未実装)
	part[PART_N+kb].song_part=(const struct SONG_PART *)0xffff;
						//演奏中を設定する
//DBG_C('>');
}


//==============================================
//KB演奏停止処理(API関数)
//==============================================
static void KB_STOP(
	unsigned char kb)			//KB演奏を終了するﾊﾟｰﾄ番号(0起算)
						//制御表はpart[PART_N+kb]にｱｸｾｽする
{

//DBG_C('\n');
//DBG_C('p');
	if(kb>=KB_N) return;			//KB用ﾊﾟｰﾄ数を超えているときは戻る
	part[PART_N+kb].song_part=0;		//演奏終了を設定する
//DBG_C('>');
}


//==============================================
//KB演奏音符設定処理(API関数)
//==============================================
static void KB_ONPU(
	unsigned char kb,			//KB演奏に切替えるﾊﾟｰﾄ番号(0起算)
						//制御表はpart[PART_N+kb]にｱｸｾｽする
	unsigned short onpu)			//音符ｺｰﾄﾞ(0は演奏停止)
{

//DBG_C('\n');
//DBG_C('o');
//DBG_S(onpu);
	if(kb>=KB_N) return;			//KB用ﾊﾟｰﾄ数を超えているときは戻る
	part[PART_N+kb].onpu_kb=onpu;		//KB演奏用ﾊﾞｯﾌｧに音符ｺｰﾄﾞを設定する
	part[PART_N+kb].onka_zan=0;		//音価残をｸﾘｱしておく
//DBG_C('>');
}
#endif


#if PART_N+KB_N>0
//==============================================
//ｵﾙｺﾞｰﾙ及びKB演奏処理
//==============================================
static void song_exe(void)
{
	struct PART *part_pt;			//ﾊﾟｰﾄ個別制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	unsigned char n,m,c,k;
	unsigned short u;
	short j,s;
	union					//8ﾋﾞｯﾄｼﾌﾄ演算の効率化
	{
		unsigned short sss;
		unsigned char bbb[2];
	} sssbbb;

//DBG_C('<');
	//初期化
	song.pcm=0;				//全ﾊﾟｰﾄ分のPWM値を0にしておく
	m=0;					//ｵﾙｺﾞｰﾙ演奏中のﾊﾟｰﾄ数をｸﾘｱしておく

#if PART_N>0
	//ｵﾙｺﾞｰﾙ音価ﾌﾟﾘｽｹｰﾗを更新
//DBG_S(song.onka_ps);
	if(song.onka_ps) song.onka_ps--;	//ｵﾙｺﾞｰﾙ音価ﾌﾟﾘｽｹｰﾗが残っていればﾃﾞｸﾘ
	else song.onka_ps=song.onka_psn;	//満了していたら初期化する
#endif

#if KB_N>0
	//KB音価ﾌﾟﾘｽｹｰﾗを更新
//DBG_S(song.onka_kb_ps);
	if(song.onka_kb_ps) song.onka_kb_ps--;	//KB音価ﾌﾟﾘｽｹｰﾗが残っていればﾃﾞｸﾘ
	else song.onka_kb_ps=song.onka_kb_psn;	//満了していたら初期化する
#endif

	//ﾊﾟｰﾄ演奏処理
	for(n=0,part_pt=part;n<PART_N+KB_N;n++,part_pt++)
						//ｵﾙｺﾞｰﾙとｷｰﾎﾞｰﾄﾞのﾊﾟｰﾄを繰返す
	{
//DBG_B(n);
//DBG_L(part_pt->song_part);
		if(!part_pt->song_part) continue;
						//演奏が終了しているﾊﾟｰﾄは飛ばす
#if PART_N>0
		if(n<PART_N) m++;		//ｵﾙｺﾞｰﾙ演奏中のﾊﾟｰﾄ数をｲﾝｸﾘ
#endif

		//音符を取出す
//DBG_C('t');
		while(!part_pt->onka_zan)	//音価残が無くなったら次の音符へ進む
						//　通常音符が現れるか､音符終了まで繰返す
						//　つまり､制御音符は一気に処理する
		{
//DBG_C('\n');
//DBG_C('>');
//DBG_B(n);
//DBG_C('+');
#if PART_N>0
			if(n<PART_N)		//ｵﾙｺﾞｰﾙ演奏のとき
			{
//DBG_C('o');
//DBG_L(part_pt->song_part->onpu);
//DBG_S(part_pt->onpu_of);
				part_pt->onpu_cd=*(part_pt->onpu+part_pt->onpu_of++);
						//音符ｺｰﾄﾞを取得し､音符ｽﾃｯﾌﾟを進めておく
			}
#endif
#if KB_N>0
			if(n>=PART_N)		//KB演奏のとき
			{
//DBG_C('k');
				part_pt->onpu_cd=part_pt->onpu_kb;
						//KB演奏用ﾊﾞｯﾌｧから音符ｺｰﾄﾞを取出す
				part_pt->onpu_kb=0;
						//KB演奏用ﾊﾞｯﾌｧをｸﾘｱしておく
			}
#endif
//DBG_S(part_pt->onpu_cd);
			part_pt->ongen_tobi=0;	//ｻﾝﾌﾟﾙ飛び数を0にしておく
//DBG_S(part_pt->ongen_tobi);
			part_pt->port_tobi_ps=0;//ﾎﾟﾙﾀﾒﾝﾄｻﾝﾌﾟﾙ飛び数増減量を0にしておく
//DBG_S(part_pt->port_tobi_ps);
			if(part_pt->onpu_cd&0x2000)
						//機能ｺｰﾄﾞ2のとき
			{
#if PART_N>0
				if(part_pt->onpu_cd&0x1000)
						//ﾘﾋﾟｰﾄ分岐以外のとき
				{
					if(part_pt->onpu_cd&0x0800)
							//波形切換えのとき
					{
//DBG_C('w');
						switch(part_pt->onpu_cd&0x0700)
						{
						case 0x0000:	//音源波形のとき
//DBG_C('o');
							part_pt->ongen=(struct WAVE_HDR *)
								(*(part_pt->onpu+
								part_pt->onpu_of++)+
								(unsigned long)&SONG_IDX);
								//音源波形ｱﾄﾞﾚｽを取得し､
								//　音符ｽﾃｯﾌﾟを進めておく
//DBG_L(part_pt->ongen);
							break;
						case 0x0100:	//ｴﾝﾍﾞﾛｰﾌﾟ波形のとき
//DBG_C('e');
							part_pt->enve=(struct ENVE_HDR *)
								(*(part_pt->onpu+
								part_pt->onpu_of++)+
								(unsigned long)&SONG_IDX);
								//ｴﾝﾍﾞﾛｰﾌﾟ波形ｱﾄﾞﾚｽを取得し､
								//　音符ｽﾃｯﾌﾟを進めておく
//DBG_L(part_pt->enve);
							break;
						case 0x0200:	//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形のとき
//DBG_C('p');
							part_pt->pitch=(struct PITCH_HDR *)
								(*(part_pt->onpu+
								part_pt->onpu_of++)+
								(unsigned long)&SONG_IDX);
								//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ｱﾄﾞﾚｽを取得し､
								//　音符ｽﾃｯﾌﾟを進めておく
//DBG_L(part_pt->pitch);
							break;
						}
					}
					else if(part_pt->onpu_cd&0x0400)
							//各種値設定のとき
					{
//DBG_C('k');
						switch(part_pt->onpu_cd&0x0300)
						{
						case 0x0000:	//ﾍﾞﾛｼﾃｨ設定のとき
//DBG_C('v');
							part_pt->velo=
								((unsigned long)
								(part_pt->onpu_cd&0x00ff)*
								PWM_STEP)/256;
//DBG_S(part_pt->velo);
							break;
						case 0x0100:	//ｴﾝﾍﾞﾛｰﾌﾟﾀｲﾑ設定のとき
//DBG_C('e');
							part_pt->enve_psn=
								part_pt->song_part->enve_psn*
								(part_pt->onpu_cd&0x00ff);
//DBG_S(part_pt->enve_psn);
							break;
						case 0x0200:	//ﾃﾝﾎﾟ設定のとき
//DBG_C('t');

							song.onka_psn=
								(song.song_hdr->onka_psn*
								(part_pt->onpu_cd&0x00ff))>>7;
//DBG_S(song.onka_psn);
							break;
						case 0x0300:	//ﾋﾟｯﾁﾍﾞﾝﾄﾞﾀｲﾑ設定のとき
//DBG_C('\n');
//DBG_C('p');
							part_pt->pitch_psn=
								part_pt->song_part->pitch_psn*
								(part_pt->onpu_cd&0x00ff);
//DBG_L(part_pt->pitch_psn);
							part_pt->pitch_ps=0;
//DBG_S(part_pt->pitch_ps);
							part_pt->pitch_of=0;
//DBG_S(part_pt->pitch_of);
							break;												}
					}
					else		//無条件分岐のとき
					{
//DBG_C('j');
						part_pt->onpu_of=part_pt->onpu_cd&0x03ff;
							//分岐先を設定する
//DBG_S(part_pt->onpu_of);
					}
				}
				else		//ﾘﾋﾟｰﾄ分岐のとき
				{
//DBG_C('r');
					c=(part_pt->onpu_cd&0x0c00)>>10;
							//ﾘﾋﾟｰﾄ番号を得る
//DBG_B(c);
//DBG_B(part_pt->rpt[c]);
					if(part_pt->rpt[c])
							//ﾘﾋﾟｰﾄ回数残があるとき
					{
//DBG_C('z');
						part_pt->rpt[c]--;
							//ﾘﾋﾟｰﾄ回数残をﾃﾞｸﾘする
//DBG_B(part_pt->rpt[c]);
						part_pt->onpu_of=part_pt->onpu_cd&0x03ff;
							//分岐先を設定する
//DBG_S(part_pt->onpu_of);
					}
				}
#endif
			}
			else if(part_pt->onpu_cd&0x1f00)
						//演奏音符のとき
			{
//DBG_C('\n');
//DBG_S(part_pt->onpu_cd);
				sssbbb.sss=part_pt->onpu_cd;
						//音符の音価ｺｰﾄﾞのﾋﾞｯﾄ分離
				k=(sssbbb.bbb[1]&0x1f)-1;
						//音価ｺｰﾄﾞ(0起算)を得る
//DBG_B(k);
				part_pt->onka_zan=onka_tbl[k];
						//音価残を設定する	
//DBG_B(part_pt->onka_zan);
				if(part_pt->onpu_cd&0x003f)
						//発音音符のとき
				{
//DBG_C('\n');
					if(part_pt->onpu_cd&0x4000)
							//効果音発声のとき
					{
//DBG_C('e');
						part_pt->efct_tobi=part_pt->onpu_cd&0x00ff;
								//ｻﾝﾌﾟﾙ飛び数を設定
//DBG_S(part_pt->efct_tobi);
						part_pt->efct_of=0;
								//ｻﾝﾌﾟﾙｵﾌｾｯﾄを初期化する
//DBG_L(part_pt->efct_of);
						part_pt->efct=(struct EFCT_HDR *)
							(*(part_pt->onpu+
							part_pt->onpu_of++)+
							(unsigned long)&SONG_IDX);
								//効果音波形ｱﾄﾞﾚｽを取得し､
								//　音符ｽﾃｯﾌﾟを進めておく
//DBG_S(part_pt->efct->smp);
//DBG_L(part_pt->efct);
					}
					else		//通常演奏のとき
					{
				
//DBG_B(song.key);
						c=((part_pt->onpu_cd&0x003f)+song.key)&0x3f;
							//音高に移調度を加味して､範囲内に規制する
//DBG_B(c);
						c=onkou_mei_tbl[c];
							//ｵｸﾀｰﾌﾞ番号と音名ｺｰﾄﾞを得る
//DBG_B(c);
						u=onkou_tbl[c&0x0f];
							//音名のｻﾝﾌﾟﾙ飛び数を得る
						if(c>>=4) u>>=c;
							//ｵｸﾀｰﾌﾞ番号でｼﾌﾄする
						part_pt->ongen_tobi=u;
							//音源飛び数を設定
//DBG_S(part_pt->ongen_tobi);
						if(part_pt->onpu_cd&0x0080)
							//ﾎﾟﾙﾀﾒﾝﾄのとき
						{
//DBG_C('P');
							k=port_tbl[k];
								//音価ｺｰﾄﾞから音源飛び数増減量率を得る
//DBG_B(k);
							u=*(part_pt->onpu+part_pt->onpu_of);
								//次の音符ｺｰﾄﾞを取得する
//DBG_S(u);
							if((u&0xff80)==0x0080) part_pt->onpu_of++;
								//次の音符がﾎﾟﾙﾀﾒﾝﾄ終着のときは
								//　音符ﾃﾞｰﾀのｵﾌｾｯﾄを進めておく
//DBG_S(part_pt->onpu_of);
							if(k&&!(u&0xe000)&&(u&0x1f00||u&0x0080)&&u&0x003f)
								//音源飛び数増減量率が有効且つ､
								//　次の音符の音高が有効のとき
							{
								c=((u&0x003f)+song.key)&0x3f;
									//音高に移調度を加味して､
									//　範囲内に規制する
//DBG_B(c);
								c=onkou_mei_tbl[c];
									//ｵｸﾀｰﾌﾞ番号と音名ｺｰﾄﾞを得る
//DBG_B(c);
								u=onkou_tbl[c&0x0f];
									//音名のｻﾝﾌﾟﾙ飛び数を得る
								if(c>>=4) u>>=c;
									//ｵｸﾀｰﾌﾞ番号でｼﾌﾄする
								part_pt->port_tobi_gl=u;
									//ﾎﾟﾙﾀﾒﾝﾄの飛び数目標値を設定
//DBG_S(part_pt->port_tobi_gl);
								if(part_pt->port_tobi_gl>part_pt->ongen_tobi)
									//飛び数増加のとき
								{
//DBG_C('U');
									part_pt->port_tobi_ps=
										(unsigned long)
										(part_pt->port_tobi_gl-
										part_pt->ongen_tobi)*k/4096;
								}
								else	//飛び数減少のとき
								{
//DBG_C('D');
									part_pt->port_tobi_ps=
										(unsigned long)
										(part_pt->ongen_tobi-
										part_pt->port_tobi_gl)*k/4096;
								}
//DBG_S(part_pt->port_tobi_ps);
							}
						}
					}
				}
				if(!(part_pt->onpu_cd&0x0040))
						//ﾀｲ･ｽﾗｰでないとき
				{
//DBG_C('c');
					part_pt->enve_of=0;
						//ｴﾝﾍﾞﾛｰﾌﾟ波形ﾃｰﾌﾞﾙｵﾌｾｯﾄをｸﾘｱ
					part_pt->enve_ps=0;
						//ｴﾝﾍﾞﾛｰﾌﾟ処理対応のﾌﾟﾘｽｹｰﾗを初期化
					part_pt->pitch_of=0;
						//ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形ﾃｰﾌﾞﾙｵﾌｾｯﾄをｸﾘｱ
					part_pt->pitch_ps=0;
						//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理対応のﾌﾟﾘｽｹｰﾗを初期化
				}
			}
#if PART_N>0
			else if(part_pt->onpu_cd&0x0040)
						//ﾘﾋﾟｰﾄ回数設定のとき
			{
//DBG_C('r');
				c=(part_pt->onpu_cd&0x0030)>>4;
						//ﾘﾋﾟｰﾄ番号を得る
//DBG_B(c);
				part_pt->rpt[c]=part_pt->onpu_cd&0x000f;
//DBG_B(part_pt->rpt[c]);
			}
			else if(part_pt->onpu_cd&0x0020)
						//転調設定のとき
			{
//DBG_C('i');
				if((c=part_pt->onpu_cd&0x001f)==0)
					song.key=song.song_hdr->key;
							//増減値が0のときは
							//　ｿﾝｸﾞﾍｯﾀﾞのｷｰに戻す
				else
				{			//増減値が0でなければ
					song.key+=c;	//ｶﾚﾝﾄのｷｰから増減する
					if(c>15) song.key-=32;
							//負数のときに是正する
				}
//DBG_B(song.key);
			}
#endif
			else break;		//演奏終了のとき
		}

		//音符の発音
//DBG_C('h');
//DBG_B(part_pt->onka_zan);
		if(part_pt->onka_zan)		//音価残があれば(音符終了でなければ)
						//　演奏処理を実行する
		{

			//音価を更新
//DBG_C('a');
//DBG_S(song.onka_ps);
			if(((n<PART_N)&&(!song.onka_ps))||
				((n>=PART_N)&&(!song.onka_kb_ps)))

						//ｵﾙｺﾞｰﾙ演奏で音価ﾌﾟﾘｽｹｰﾗが満了､又は
						//　KB演奏でKB用音価ﾌﾟﾘｽｹｰﾗが満了していたら
			{
				part_pt->onka_zan--;
						//音価残をﾃﾞｸﾘする
//DBG_B(part_pt->onka_zan);

				//ﾎﾟﾙﾀﾒﾝﾄ処理
				if(part_pt->port_tobi_ps)
						//ﾎﾟﾙﾀﾒﾝﾄ演奏のとき
				{
//DBG_C('p');
//DBG_S(part_pt->ongen_tobi);
					if(part_pt->port_tobi_gl>part_pt->ongen_tobi)
						//飛び数増加のとき
					{
//DBG_C('u');
						if((part_pt->ongen_tobi+=part_pt->port_tobi_ps)>
							part_pt->port_tobi_gl)
							part_pt->ongen_tobi=part_pt->port_tobi_gl;
						//飛び数を増加して､目標値に規制する
					}
					else	//飛び数減少のとき
					{
//DBG_C('d');
						if((part_pt->ongen_tobi-=part_pt->port_tobi_ps)<
							part_pt->port_tobi_gl)
							part_pt->ongen_tobi=part_pt->port_tobi_gl;
						//飛び数を減少して､目標値に規制する
					}
//DBG_S(part_pt->ongen_tobi);
				}
			}

			//発音処理
//DBG_C('h');
//DBG_S(part_pt->onpu_cd);
			if(part_pt->onpu_cd&0x003f)
						//発音音符のとき
			{

				//ｴﾝﾍﾞﾛｰﾌﾟ処理
//DBG_C('\n');
//DBG_C('e');
//DBG_S(part_pt->enve_psn);
				if(part_pt->enve_psn)
						//ｴﾝﾍﾞﾛｰﾌﾟが設定されているときは
						//　ｴﾝﾍﾞﾛｰﾌﾟ処理を行う
				{
//DBG_C('s');
//DBG_S(part_pt->enve_ps);
					if(!part_pt->enve_ps)
						//ｴﾝﾍﾞﾛｰﾌﾟ処理のﾀｲﾐﾝｸﾞのとき
					{
//DBG_C('t');
//DBG_S(part_pt->velo);
//DBG_B(part_pt->enve_of);
//DBG_B(part_pt->enve->data[part_pt->enve_of]);
						part_pt->velo_enve=((unsigned long)
							part_pt->velo*
							part_pt->enve->
							data[part_pt->enve_of])/256;
						//ｴﾝﾍﾞﾛｰﾌﾟ処理後のﾍﾞﾛｼﾃｨ値を求める
//DBG_S(part_pt->velo_enve);

//DBG_B(part_pt->enve->smp);
						//ｻﾝﾌﾟﾙｵﾌｾｯﾄを進める
						if(part_pt->enve_of<
							part_pt->enve->smp)
							part_pt->enve_of++;
							//ｻﾝﾌﾟﾙｵﾌｾｯﾄを進めて､最後に規制する
//DBG_B(part_pt->enve_of);
					}
					if(part_pt->enve_ps++>part_pt->enve_psn)
					{
						//ｴﾝﾍﾞﾛｰﾌﾟのﾌﾟﾘｽｹｰﾗをｲﾝｸﾘして満了したら
						part_pt->enve_ps=0;
							//ｴﾝﾍﾞﾛｰﾌﾟのﾌﾟﾘｽｹｰﾗをｸﾘｱ
					}
				}
				else		//ｴﾝﾍﾞﾛｰﾌﾟが設定されていないときは
				{
					part_pt->velo_enve=part_pt->velo;
						//　ﾍﾞﾛｼﾃｨ設定値をそのまま使う
				}
//DBG_S(part_pt->velo_enve);

				//音源ｻﾝﾌﾟﾙ値を得る
//DBG_C('\n');
//DBG_C('w');
				if(part_pt->onpu_cd&0x4000)
						//効果音発声のとき
				{
//DBG_C('e');
//DBG_L(part_pt->efct_of);
					s=(short)(part_pt->efct->data[
						(unsigned short)(part_pt->efct_of/256)])-128;
							//効果音ｻﾝﾌﾟﾙ値を得る
//DBG_S(s);
					if((part_pt->efct_of+=part_pt->efct_tobi)>
						part_pt->efct->smp*256)
							//ｵﾌｾｯﾄを進めて､ｻﾝﾌﾟﾙ数を超えたら
						part_pt->efct_of-=part_pt->efct_tobi;
							//　戻す
//DBG_L(part_pt->efct_of);
				}
				else		//通常演奏のとき
				{



//DBG_S(part_pt->ongen_of);
					sssbbb.sss=part_pt->ongen_of;
//DBG_B(sssbbb.bbb[1]);
					s=(short)(part_pt->ongen->data[sssbbb.bbb[1]])-128;
							//音源ｻﾝﾌﾟﾙ値を得る
//DBG_S(s);
//DBG_C('o');
//DBG_S(part_pt->ongen_of);
//DBG_S(part_pt->ongen_tobi);
					part_pt->ongen_of+=part_pt->ongen_tobi;
							//波形ﾃﾞｰﾀのｵﾌｾｯﾄを進めて､
							//　ｻﾝﾌﾟﾙ数は256固定のため
							//　ｵｰﾊﾞﾌﾛｰは無視する
//DBG_S(part_pt->ongen_of);

					//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理
//DBG_C('\n');
//DBG_C('p');
					if(part_pt->pitch_psn)
							//ﾋﾟｯﾁﾍﾞﾝﾄﾞが設定されているときは
							//　ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理を行う
					{
//DBG_C('\n');
//DBG_S(part_pt->pitch_ps);
						if(!part_pt->pitch_ps)
							//ﾋﾟｯﾁﾍﾞﾝﾄﾞ処理のﾀｲﾐﾝｸﾞのとき
						{
//DBG_C('\n');
//DBG_C('p');
							j=part_pt->pitch->data
								[part_pt->pitch_of];
								//ﾋﾟｯﾁﾍﾞﾝﾄﾞｻﾝﾌﾟﾙ値を得る
//DBG_S(j);
							if(j>128) j-=256;
								//signedにする
//DBG_S(j);
//DBG_S(part_pt->ongen_tobi);
							part_pt->ongen_tobi=
								(signed)part_pt->ongen_tobi+j;
								//音源ｻﾝﾌﾟﾙ飛び数を増減する
//DBG_S(part_pt->ongen_tobi);
							if(++part_pt->pitch_of>
								part_pt->pitch->smp)
								part_pt->pitch_of--;
								//ｻﾝﾌﾟﾙｵﾌｾｯﾄを進めて､最後に規制する
//DBG_S(part_pt->pitch_of);
						}
						if(++part_pt->pitch_ps>=part_pt->pitch_psn)
							part_pt->pitch_ps=0;
								//ﾌﾟﾘｽｹをｲﾝｸﾘして､満了したら
								//　初期化する
//DBG_S(part_pt->pitch_ps);
					}
				}

				//PWMﾃﾞｭｰﾃｨ値を合算
				s=((long)part_pt->velo_enve*s)/256;
						//ﾍﾞﾛｼﾃｨを加味する
//DBG_S(s);
				song.pcm+=s;	//全ﾊﾟｰﾄ分のPWMﾃﾞｭｰﾃｨ値に､
						//　このﾊﾟｰﾄ分を加算する
//DBG_S(song.pcm);
			}
		}

		else				//音符終了のとき
		{
			//ﾊﾟｰﾄ終了処理
//DBG_C('t');
			if(n<PART_N)		//ｵﾙｺﾞｰﾙ演奏のときは
			{
				part_pt->song_part=0;
						//ﾊﾟｰﾄ演奏終了を表示する
			}
		}
	}
#if PART_N>0
	if(song.no)				//ｵﾙｺﾞｰﾙ演奏していたとき
	{
//DBG_B(m);
		if(!m)				//全ﾊﾟｰﾄが終了したら
		{
//DBG_L(song.song_hdr->next);
			if(song.song_hdr->next)	//連続演奏するとき
			{
//DBG_C('r');
				song.song_hdr=(struct SONG_HDR *)
					(song.song_hdr->next+(unsigned long)&SONG_IDX);
						//曲ﾍｯﾀﾞへのﾎﾟｲﾝﾀを取得
//DBG_L(song.song_hdr);
//DBG_D(song.song_hdr,64);
				song_init();	//曲ﾃﾞｰﾀを初期化
			}
			else			//単曲演奏のとき
			{
//DBG_C('s');
				song.no=0;	//曲演奏終了の表示
				CB_SONG_END();	//演奏終了時ｺｰﾙﾊﾞｯｸ関数を呼ぶ
			}
		}
	}
#endif
//DBG_C('>');
}
#endif


#if VOICE_N>0
//==============================================
//音声N選択処理(API関数)
//==============================================
static void VOICEN_SEL(				//戻り値無し
	unsigned char ch,			//ch番号
	unsigned char no)			//音声番号(0は再生中止)
{
	struct VOICEN *voiceN_pt;		//音声N再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ

//DBG_C('\n');
//DBG_C('s');
//DBG_B(ch);
//DBG_B(no);
//DBG_D(voiceN_idx,sizeof(voiceN_idx));
//DBG_C('i');
	voiceN_pt=voiceN+ch;			//音声N再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀを設定
	if(no)					//再生開始のとき
	{
		if(no>sizeof(voiceN_idx)/sizeof(voiceN_idx[0])) return;
						//実装数を超えているときは戻る
		if(voiceN_pt->no!=no)		//発声中の音声番号と異なるときのみ
						//　音声を切り替える
		{
			voiceN_pt->no=no--;	//音声番号を設定
			voiceN_pt->begin=(unsigned long)voiceN_idx[no].adr;
						//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽを設定
//DBG_L(voiceN_pt->begin);
			voiceN_pt->end=voiceN_pt->begin+voiceN_idx[no].len;
						//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽを設定
//DBG_L(voiceN_pt->end);
			voiceN_pt->tobi=voiceN_idx[no].tobi;
						//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]を設定
//DBG_S(voiceN_pt->tobi);
			voiceN_pt->of=0;	//音声ﾃﾞｰﾀのｵﾌｾｯﾄを初期化しておく
//DBG_S(voiceN_pt->of);
			voiceN_pt->pcm=0;	//PCM値を初期化
//DBG_S(voiceN_pt->pcm);
		}
	}
	else					//再生中断のとき
	{
		voiceN_pt->no=0;		//再生終了にする
	}
//DBG_D(voiceN_pt,sizeof(voiceN[0]));
}


//==============================================
//音声N再生処理ｻﾝﾌﾟﾙﾚｰﾄ設定(API関数)
//==============================================
static void VOICEN_SMP(				//戻り値無し
	unsigned char ch,			//ch番号
	unsigned smp)				//再生処理ｻﾝﾌﾟﾙﾚｰﾄ[sps]
{
//DBG_C('\n');
//DBG_C('f');

	voiceN[ch].tobi=(unsigned long)256*smp*PWM_PR/1000000;
//DBG_S(voiceN[ch].tobi);
}


//==============================================
//音声N再生処理
//==============================================
static void voiceN_exe(
	unsigned char ch)			//ch番号
{
	struct VOICEN *voiceN_pt;		//音声N再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ

//DBG_C('\n');
//DBG_C('p');
//DBG_B(ch);
	voiceN_pt=voiceN+ch;			//音声N再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀを設定する
	voiceN_pt->of+=voiceN_pt->tobi;		//音声ﾃﾞｰﾀのｵﾌｾｯﾄを進める
//DBG_S(voiceN_pt->of);
	voiceN_pt->begin+=voiceN_pt->of/256;	//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽを進める
//DBG_L(voiceN_pt->begin);
	voiceN_pt->of&=0x00ff;			//ｵﾌｾｯﾄの整数部をﾏｽｸしておく
	if(voiceN_pt->begin>=voiceN_pt->end)	//音声ﾃﾞｰﾀが終わるとき
	{
//DBG_C('e');
		voiceN_pt->no=0;		//再生終了にする
		CB_VOICEN_END(ch);		//音声再生終了処理(ｺｰﾙﾊﾞｯｸ関数)を呼出す
	}
	else
	{
//DBG_C('c');
		voiceN_pt->pcm=*(unsigned char *)(voiceN_pt->begin)-128;
						//音声ﾃﾞｰﾀを取り出して､
						//　8ﾋﾞｯﾄPCM形式をsignedに変換
//DBG_S(voiceN_pt->pcm);
//DBG_S(voiceN_vol[ch]);
		voiceN_pt->pcm=(long)voiceN_pt->pcm*voiceN_vol[ch]/128;
						//音量配分を掛ける
//DBG_S(voiceN_pt->pcm);
	}
}
#endif


#if VOICE_I>0				//I2Cﾒﾓﾘを使うとき
#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
//==============================================
//I2Cｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
//==============================================
static void i2c_start(void)
{

//DBG_C('s');
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SCLがHのままSDAをL
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	wait_1us();
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH(High-Z)
	wait_1us();
}


//==============================================
//I2Cｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ
//==============================================
static void i2c_stop(void)
{

//DBG_C('p');
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SCLがHのままSDAをH(High-Z)
	wait_1us();
}


//==============================================
//I2C送信
//==============================================
static void i2c_send(
	unsigned char data)			//送信ﾃﾞｰﾀ
{
	unsigned char n=8;

//DBG_C('d');
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		if(data&0x80) GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);
						//送信ﾋﾞｯﾄを設定する
		else GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);
		data<<=1;			//ﾋﾞｯﾄを進める
		wait_1us();
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH(High-Z)
		wait_1us();
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
		wait_1us();
	}
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをHigh-Z(ACK受信)
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	wait_1us();
}


#define	i2c_addr(addr)	i2c_send(addr)		//i2c_addr()はi2c_send()と同じ


//==============================================
//I2C受信+ACK応答
//==============================================
static unsigned char i2c_recv_ack(void)		//受信ﾃﾞｰﾀを返す
{
	unsigned char n=8,data=0;

//DBG_C('k');
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH(High-Z)
		wait_1us();
		data<<=1;			//ﾋﾞｯﾄを進める
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) data++;
						//受信ﾋﾞｯﾄを取込む
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
		wait_1us();
	}
	GPIO_L(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをL(ACK)
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH(High-Z)
	wait_1us();
	return(data);				//受信ﾃﾞｰﾀを返す
}


//==============================================
//I2C受信+NOACK応答
//==============================================
static unsigned char i2c_recv_noack(void)	//受信ﾃﾞｰﾀを返す
{
	unsigned char n=8,data=0;

//DBG_C('n');
	while(n--)				//8ﾋﾞｯﾄ分繰返す
	{
		GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをH(High-Z)
		wait_1us();
		data<<=1;			//ﾋﾞｯﾄを進める
		if(GPIO_R(I2C_SDA_GPIO,I2C_SDA_BIT)) data++;
						//受信ﾋﾞｯﾄを取込む
		GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);
						//SCLをL
		wait_1us();
	}
	GPIO_H(I2C_SDA_GPIO,I2C_SDA_BIT);	//SDAをH(High-Z)(NOACK)
	wait_1us();
	GPIO_H(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをH(High-Z)
	wait_1us();
	GPIO_L(I2C_SCL_GPIO,I2C_SCL_BIT);	//SCLをL
	wait_1us();
	return(data);				//受信ﾃﾞｰﾀを返す
}
#endif


//==============================================
//音声I選択処理(API関数)
//==============================================
static void VOICEI_SEL(				//戻り値無し
	unsigned short no)			//音声番号(0は再生中止)
{

//DBS_H;
//DBG_C('\n');
//DBG_C('s');
//DBG_S(no);
//DBG_D(voiceI_idx,sizeof(voiceI_idx));
//DBG_C('i');
	if(voiceI.no!=no)			//発声中の音声番号と異なるときは
	{
		if(voiceI.no)			//再生中のときは
		{
#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
			i2c_recv_noack();	//受信+NOACK応答
			i2c_stop();		//ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ
#else					//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//DBG_C('a');
			if(I2C1->CR1&I2C_CR1_ACK)
						//ACKがｾｯﾄされていたら
			{
//DBG_C('b');
				while(!(I2C1->SR1&I2C_SR1_RXNE)){};
						//受信を待つ
				I2C1->CR1&=~I2C_CR1_ACK;
						//ACK無効
				I2C1->DR;	//ﾃﾞｰﾀを空読み
			}
//DBG_C('c');
			while(!(I2C1->SR1&I2C_SR1_RXNE)){};
						//受信を待つ
			I2C1->DR;		//ﾃﾞｰﾀを空読み
			I2C1->CR1|=I2C_CR1_STOP;//ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ
			while(I2C1->SR2&I2C_SR2_BUSY){};
						//ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝを待つ
			I2C1->CR1&=~I2C_CR1_PE;	//I2C無効
//DBG_C('d');
#endif
			voiceI.no=0;		//再生停止にする
			voiceI.pcm=0;		//PCM値を初期化
//DBG_B(voiceI.pcm);
		}
	}

	if(no)					//再生開始のとき
	{
//DBG_C('e');
		if(no>sizeof(voiceI_idx)/sizeof(voiceI_idx[0])) return;
						//実装数を超えているときは戻る
		if(voiceI.no==no) return;	//発声中の音声番号と同じ場合は戻る
		voiceI.no=no--;			//音声番号を設定して､番号を0起算にする
//DBG_S(voiceI.no);
		voiceI.begin=voiceI_idx[no].adr;//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽを設定
//DBG_L(voiceI.begin);
		voiceI.end=voiceI.begin+voiceI_idx[no].len;
						//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽを設定
//DBG_L(voiceI.end);
		voiceI.tobi=voiceI_idx[no].tobi;//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]を設定
//DBG_S(voiceI.tobi);
		voiceI.of=255;			//再生開始時にﾃﾞｰﾀを取得するようにしておく
//DBG_S(voiceI.of);
		voiceI.pcm=0;			//PCM値を初期化
//DBG_S(voiceI.pcm);
		voiceI.sla=voiceI_idx[no].sla;	//I2Cｽﾚｰﾌﾞｱﾄﾞﾚｽを設定
//DBG_B(voiceI.sla);
#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
//DBG_C('f');
		i2c_start();			//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
		i2c_addr(voiceI.sla);		//ｽﾚｰﾌﾞｱﾄﾞﾚｽ+書込み指定送信
		i2c_send(voiceI.begin>>8);	//ｱﾄﾞﾚｽ上位送信
		i2c_send(voiceI.begin);		//ｱﾄﾞﾚｽ下位送信
		i2c_start();			//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
		i2c_addr(voiceI.sla|1);		//ｽﾚｰﾌﾞｱﾄﾞﾚｽ+読込み指定送信
#else					//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//DBG_C('1');
		I2C1->CR1|=I2C_CR1_PE;		//I2C有効
		I2C1->CR1|=I2C_CR1_START;	//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
		while(!(I2C->SR1&I2C_SR1_SB)){};//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝを待つ
//DBG_C('2');
		I2C1->DR=0xa0;			//ｽﾚｰﾌﾞｱﾄﾞﾚｽ(W)送信
		while(!(I2C->SR1&I2C_SR1_ADDR)){};
						//ｽﾚｰﾌﾞｱﾄﾞﾚｽ送信を待つ
		I2C->SR2;
		while(!(I2C->SR1&I2C_SR1_TXE)){};
						//送信ﾊﾞｯﾌｧ空きを待つ
//DBG_C('4');
		I2C1->DR=0x01;			//読出しｱﾄﾞﾚｽ上位送信
		while(!(I2C->SR1&I2C_SR1_TXE)){};
						//送信ﾊﾞｯﾌｧ空きを待つ
//DBG_C('5');
		I2C1->DR=0x30;			//読出しｱﾄﾞﾚｽ下位送信
		while(!(I2C->SR1&I2C_SR1_TXE)){};
						//送信ﾊﾞｯﾌｧ空きを待つ
//DBG_C('6');
		while(!(I2C->SR1&I2C_SR1_BTF)){};
						//送信完了を待つ
//DBG_C('7');
		I2C1->CR1|=I2C_CR1_ACK;		//ACK有効
		I2C1->CR1|=I2C_CR1_START;	//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝ
		while(!(I2C->SR1&I2C_SR1_SB)){};//ｽﾀｰﾄｺﾝﾃﾞｨｼｮﾝを待つ
//DBG_C('8');
		I2C1->DR=0xa0|1;		//ｽﾚｰﾌﾞｱﾄﾞﾚｽ(R)送信
		while(!(I2C->SR1&I2C_SR1_ADDR)){};
						//ｽﾚｰﾌﾞｱﾄﾞﾚｽ送信を待つ
//DBG_C('9');
		I2C->SR2;
#endif
	}
//DBG_D(&voiceI,sizeof(voiceI));
//DBS_L;
}


//==============================================
//音声I再生処理ｻﾝﾌﾟﾙﾚｰﾄ設定(API関数)
//==============================================
static void VOICEI_SMP(				//戻り値無し
	unsigned smp)				//再生処理ｻﾝﾌﾟﾙﾚｰﾄ[sps]
{
//DBG_C('\n');
//DBG_C('p');

	voiceI.tobi=(unsigned long)256*smp*PWM_PR/1000000;
//DBG_S(voiceI.tobi);
}


//==============================================
//音声I再生処理
//==============================================
static void voiceI_exe(void)
{
	unsigned char data;			//音声ﾃﾞｰﾀ

//DBG_C('\n');
//DBG_C('p');
//DBG_L(voiceI.begin);
	voiceI.of+=voiceI.tobi;			//音声ﾃﾞｰﾀのｵﾌｾｯﾄを進める
//DBG_S(voiceI.of);
	if(!(voiceI.of&0xff00)) return;		//ｵﾌｾｯﾄの整数部が同じだったら戻る
	voiceI.of&=0x00ff;			//ｵﾌｾｯﾄの整数部をｸﾘｱしておく
//DBG_S(voiceI.of);
#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
	if(++voiceI.begin<voiceI.end)		//音声ﾃﾞｰﾀが続くとき
	{
		data=i2c_recv_ack();		//受信+ACK応答
	}
	else					//音声ﾃﾞｰﾀが終わるとき
	{
		data=i2c_recv_noack();		//受信+NOACK応答
	}
#else					//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//DBG_C('a');
	while(!(I2C->SR1&I2C_SR1_RXNE)){};	//受信を待つ
//DBG_C('b');
	if(++voiceI.begin>=voiceI.end-1)	//音声ﾃﾞｰﾀが終わるとき
	{
//DBG_C('d');
		I2C1->CR1&=~I2C_CR1_ACK;	//ACK無効
	}
	data=I2C1->DR;				//受信ﾃﾞｰﾀを取込む
//DBG_B(data);
#endif
//DBG_C('e');
//DBG_B(data);
	voiceI.pcm=(short)data-128;		//音声ﾃﾞｰﾀをsignedに変換
//DBG_S(voiceI.pcm);
	voiceI.pcm=(long)voiceI.pcm*(VOICEI_VOL*PWM_STEP/100)/128;
						//音量配分を掛ける
//DBG_S(voiceI.pcm);
	if(voiceI.begin>=voiceI.end)		//音声ﾃﾞｰﾀが終わるとき
	{
//DBG_C('f');
#ifdef I2C_SOFT				//I2Cをｿﾌﾄ実装するとき
//DBG_C('g');
		i2c_stop();			//ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ
#else					//内蔵I2Cﾓｼﾞｭｰﾙを使うとき
//DBG_C('h');
		I2C1->CR1|=I2C_CR1_STOP;	//ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝ
		while(I2C1->SR2&I2C_SR2_BUSY){};//ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝを待つ
		I2C1->CR1&=~I2C_CR1_PE;		//I2C無効
#endif
//DBG_C('i');
		voiceI.no=0;			//再生終了にする
		voiceI.pcm=0;			//PCM値を初期化
		CB_VOICEI_END();		//音声再生終了処理(ｺｰﾙﾊﾞｯｸ関数)を呼出す
	}
//DBG_C('j');
}
#endif


#if VOICE_C+VOICE_S>0			//SDかSPIﾒﾓﾘを使うとき
//==============================================
//SSのﾈｹﾞｰﾄ待ちのﾏｸﾛ置換え
//==============================================
#ifdef SHSPI2				//2線SPIのとき
#define	SPI_SS_WAIT()	wait_us(SPI_SS_TIME)	//SSがﾈｹﾞｰﾄするまで待つ
#else					//その他のとき
#define	SPI_SS_WAIT()				//SSﾈｹﾞｰﾄ待ちをしない
#endif


#ifdef SPI_SOFT				//SPIをｿﾌﾄ実装するとき
#if VOICE_C>0				//SDを使うとき
//==============================================
//SPI送受信
//SDで使用する
//==============================================
static unsigned char spi_trans(			//受信したﾃﾞｰﾀを返す
	unsigned char data)			//送信するﾃﾞｰﾀ
{
	unsigned char c=8; 
	unsigned short s;

//DBG_C('<');
//DBG_B(data);

	s=data;					//送信するﾃﾞｰﾀを取り込む
	while(c--)
	{
		s<<=1;				//ﾃﾞｰﾀを左ｼﾌﾄしておく
		if(GPIO_R(SPI_MISO_GPIO,SPI_MISO_BIT)) s|=1;
						//MISOをﾃﾞｰﾀに反映する
		if(s&0x0100) GPIO_H(SPI_MOSI_GPIO,SPI_MOSI_BIT);
						//MSBからMOSIに
		else GPIO_L(SPI_MOSI_GPIO,SPI_MOSI_BIT);
						//　反映する
#if defined(SHSPI3)||defined(SHSPI2)	//3線SPIか2線SPIのとき
		GPIO_OUT(SPI_MOSI_GPIO,SPI_MOSI_BIT);
						//MOSIを出力ﾓｰﾄﾞ
#endif
#ifdef SHSPI2				//2線SPIのとき
		__disable_irq();		//割込み禁止
#endif
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをH
		GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをL
#ifdef SHSPI2				//2線SPIのとき
		__enable_irq();			//割込み許可
#endif
#if defined(SHSPI3)||defined(SHSPI2)	//3線SPIか2線SPIのとき
#ifdef SPI_MISO_PU			//MISOをﾌﾟﾙｱｯﾌﾟするとき
		GPIO_INPU(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟ
#endif
#ifdef SPI_MISO_PD			//MISOをﾌﾟﾙﾀﾞｳﾝするとき
		GPIO_INPD(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝ
#endif
#if !defined(SPI_MISO_PU)&&!defined(SPI_MISO_PD)
					//ﾌﾟﾙｱｯﾌﾟﾀﾞｳﾝが無指定のとき
		GPIO_IN(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞ
#endif
#endif
	}

//DBG_S(s);
//DBG_C('>');

	return((unsigned char)s);		//受信ﾃﾞｰﾀを返す
}
#endif


#if VOICE_S>0				//SPIﾒﾓﾘを使うとき
//==============================================
//SCK扱いのﾏｸﾛ置換え
//==============================================
#ifdef SHSPI2 				//2線SPIのとき
#define spi_send(x,y) spi_send_(x,y)		//最終ﾋﾞｯﾄのSCK扱いを有効にする
#else					//3線SPIまたは4線SPIのとき
#define spi_send(x,y) spi_send_(x)		//最終ﾋﾞｯﾄのSCK扱いを無視する
#endif


//==============================================
//SPI半二重送信
//SPIﾒﾓﾘで使用する
//受信ﾃﾞｰﾀは取込まない
//最初にMOSIを出力ﾓｰﾄﾞに設定する
//最後のSCKをLにする直前にMISOを入力ﾓｰﾄﾞに設定する(W25QのDIとDOを直結可能)
//2線SPIでSCK扱いが指定されているときは最終のSCKをHに保持する(ﾊﾟﾜｰﾀﾞｳﾝ設定の対応)
//==============================================
static void spi_send_(
#ifdef SHSPI2 				//2線SPIのとき
	unsigned char data,			//送信するﾃﾞｰﾀ
	unsigned char sck_h)			//最終ﾋﾞｯﾄのSCKの扱い
						//　(2線SPI時のﾊﾟﾜｰﾀﾞｳﾝのため)
						//　0=Lにする､1=Hを保持する
#else					//3線SPIまたは4線SPIのとき
	unsigned char data)			//送信するﾃﾞｰﾀ
#endif
{
	unsigned char c=8;

//DBS_H;
//DBG_C('+');
//DBG_B(data);
#if defined(SHSPI3)||defined(SHSPI2)	//2線SPIまたは3線SPIのとき
		GPIO_OUT(SPI_MOSI_GPIO,SPI_MOSI_BIT);
						//MOSIを出力ﾓｰﾄﾞ
#endif
	while(c--)
	{
		if(data&0x80) GPIO_H(SPI_MOSI_GPIO,SPI_MOSI_BIT);
						//ﾃﾞｰﾀﾋﾞｯﾄを
		else GPIO_L(SPI_MOSI_GPIO,SPI_MOSI_BIT);
						//　MOSIに乗せる
		data<<=1;			//ﾃﾞｰﾀを次に進める
#if !defined(SHSPI3)&&!defined(SHSPI2)	//4線SPIのとき
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをH
		GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをL
#endif
#ifdef SHSPI3				//3線SPIのとき
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをH
		if(!c)				//最終ﾋﾞｯﾄのとき
		{
#ifdef SPI_MISO_PU			//MISOをﾌﾟﾙｱｯﾌﾟするとき
			GPIO_INPU(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟ
#endif
#ifdef SPI_MISO_PD			//MISOをﾌﾟﾙﾀﾞｳﾝするとき
			GPIO_INPD(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝ
#endif
#if !defined(SPI_MISO_PU)&&!defined(SPI_MISO_PD)
					//ﾌﾟﾙｱｯﾌﾟﾀﾞｳﾝが無指定のとき
			GPIO_IN(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞ
#endif
		}
		GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをL
#endif
#ifdef SHSPI2 				//2線SPIのとき
		__disable_irq();		//割込み禁止
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをH
		if(!c)				//最終ﾋﾞｯﾄのとき
		{
#ifdef SPI_MISO_PU			//MISOをﾌﾟﾙｱｯﾌﾟするとき
			GPIO_INPU(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞﾌﾟﾙｱｯﾌﾟ
#endif
#ifdef SPI_MISO_PD			//MISOをﾌﾟﾙﾀﾞｳﾝするとき
			GPIO_INPD(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞﾌﾟﾙﾀﾞｳﾝ
#endif
#if !defined(SPI_MISO_PU)&&!defined(SPI_MISO_PD)
					//ﾌﾟﾙｱｯﾌﾟﾀﾞｳﾝが無指定のとき
			GPIO_IN(SPI_MISO_GPIO,SPI_MISO_BIT);
						//MISOを入力ﾓｰﾄﾞ
#endif
			if(!sck_h) GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//ﾊﾟﾜｰﾀﾞｳﾝ以外はSCKをL
		}
		else			//最終ﾋﾞｯﾄ以外は
		{
			GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをL
		}
		__enable_irq();			//割込み許可
#endif
	};
//DBS_L;
}


//==============================================
//SPI半二重受信
//SPIﾒﾓﾘで使用する
//MISOのﾓｰﾄﾞ設定は行わない(入力ﾓｰﾄﾞになっていることが前提)
//MOSIは駆動しない
//==============================================
static unsigned char spi_recv(void)		//受信したﾃﾞｰﾀを返す
{
	unsigned char c=8;
	volatile unsigned char data;

//DBS_H;
//DBG_C('-');
	while(c--)
	{
		data<<=1;			//ﾃﾞｰﾀを次に進める
		if(GPIO_R(SPI_MISO_GPIO,SPI_MISO_BIT)) data|=1;
						//MISOをﾃﾞｰﾀに取込む
#ifdef SHSPI2 				//2線SPIのとき
		__disable_irq();		//割込み禁止
#endif
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
		GPIO_H(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをH
		GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをL
#ifdef SHSPI2 				//2線SPIのとき
		__enable_irq();			//割込み許可
#endif
	};
//DBG_B(data);
//DBS_L;
	return(data);				//受信したﾃﾞｰﾀを返す
}
#endif


#else					//内蔵SPIﾓｼﾞｭｰﾙを使うとき
//==============================================
//内蔵SPIﾓｼﾞｭｰﾙを使うときのﾏｸﾛ置換え
//==============================================
#define spi_send(x,y) spi_trans(x)		//最終ﾋﾞｯﾄのSCK扱いを無視する
#define spi_recv() spi_trans(0)			//ﾀﾞﾐｰの送信ﾃﾞｰﾀを付加する


//==============================================
//SPI送受信
//==============================================
static unsigned char spi_trans(			//受信したﾃﾞｰﾀを返す
	unsigned char data)			//送信するﾃﾞｰﾀ
{

//DBS_H;
//DBG_C('(');
//DBG_B(data);
	*(__IO uint8_t *)&(SPI1->DR)=data;	//送信ﾊﾞｯﾌｧに1ﾊﾞｲﾄを送る
						//　1ﾊﾞｲﾄ巾でｱｸｾｽすることが重要
	while(!(SPI1->SR&SPI_SR_RXNE)){};	//受信完了を待つ
	data=SPI1->DR;				//受信したﾃﾞｰﾀを取込む
//DBG_C('-');
//DBG_B(data);
//DBG_C(')');
//DBS_L;
	return(data);				//受信したﾃﾞｰﾀを返す
}
#endif
#endif


#if VOICE_C>0				//SDを使うとき
//==============================================
//SDからｺﾏﾝﾄﾞﾚｽﾎﾟﾝｽを受信する
//==============================================
static unsigned char mmsd_resp(void)		//ﾚｽﾎﾟﾝｽを返す(0=OK､0x11=ﾀｲﾑｱｳﾄ)
{
	unsigned char n=255,res;

//DBG_C('!');
	while(n--)
	{
		res=spi_trans(0xff);
//DBG_B(res);
		if((res&0x80)==0) break;	//b7が0のときは抜ける
	}
	return(res);				//ﾚｽﾎﾟﾝｽを返す
}


//==============================================
//SDｱｸｾｽを初期化する
//==============================================
static void mmsd_init(void)			//ｴﾗｰのときはこの中でﾘﾄﾗｲする
{
	unsigned char res;

	//ﾘﾄﾗｲ用ｴﾝﾄﾘ
mmsd_init_retry:

	//SDを非選択にする
//DBG_C('\n');
//DBG_C('u');
	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ

	//ﾀﾞﾐｰｸﾛｯｸを送信する
//DBG_C('d');
#ifndef SHSPI2				//2線SPIでないときは
	spi_trans(0xff);			//おまじない
#endif

	//SDを選択する
//DBG_C('s');
	GPIO_L(SPI_SS_GPIO,SPI_SS_BIT);		//ﾁｯﾌﾟｾﾚｸﾄ

//DBG_C('r');
	spi_trans(CMD0);			//CMD0を送信する
	spi_trans(0);
	spi_trans(0);
	spi_trans(0);
	spi_trans(0);
	spi_trans(CMD0CRC);
	res=mmsd_resp();			//ﾚｽﾎﾟﾝｽを得る
//DBG_B(res);
	if(res!=0x01) goto mmsd_init_retry;	//ｱｲﾄﾞﾙでなかったらﾘﾄﾗｲする

	//初期化の完了を待つ
//DBG_C('i');
	do
	{
		spi_trans(0xff);		//ﾀﾞﾐｰ転送しないとCMD1が0x05になる
		spi_trans(CMD1);		//CMD1を送信する
		spi_trans(0);
		spi_trans(0);
		spi_trans(0);
		spi_trans(0);
		spi_trans(1);
		res=mmsd_resp();		//ﾚｽﾎﾟﾝｽを得る
//DBG_B(res);
		if(res&0x80) goto mmsd_init_retry;
						//ﾚｽﾎﾟﾝｽのMSBが1のときはﾘﾄﾗｲする
	}
	while(res==0x01);			//ｱｲﾄﾞﾙのときは繰返す
	if(res) goto mmsd_init_retry;		//ﾚｽﾎﾟﾝｽが0でなかったらﾘﾄﾗｲする

	//ﾌﾞﾛｯｸ長(512[ﾊﾞｲﾄ]設定)を送信する
//DBG_C('l');
	
	spi_trans(0xff);			//ﾀﾞﾐｰ転送しないとCMD16が0x01になる
	spi_trans(CMD16);			//CMD16を送信する
	spi_trans(0);
	spi_trans(0);
	spi_trans(2);
	spi_trans(0);
	spi_trans(1);
	res=mmsd_resp();			//ﾚｽﾎﾟﾝｽを得る
//DBG_B(res);
	if(res) goto mmsd_init_retry;		//ﾚｽﾎﾟﾝｽが0でなかったらﾘﾄﾗｲする
}


//==============================================
//SD読込み開始
//==============================================
static void mmsd_read(void)
{

	//ﾚﾃﾞｨﾁｪｯｸ
//DBG_C('\n');
//DBG_C('r');
//DBG_L(voiceC.begin);
	do
	{
		while(spi_trans(0xff)!=0xff){};	//0xffが返るまでﾀﾞﾐｰﾃﾞｰﾀを送る

	//ｼﾝｸﾞﾙﾌﾞﾛｯｸﾘｰﾄﾞ設定
//DBG_C('b');
		spi_trans(CMD17);		//CMD17を送信する
		spi_trans(voiceC.begin>>16);	//SDｶｰﾄﾞのｱﾄﾞﾚｽ(MSB)
		spi_trans(voiceC.begin>>8);	//SDｶｰﾄﾞのｱﾄﾞﾚｽ
		spi_trans(voiceC.begin);	//SDｶｰﾄﾞのｱﾄﾞﾚｽ
		spi_trans(0);			//SDｶｰﾄﾞのｱﾄﾞﾚｽ(LSB)
		spi_trans(1);			//CRC
	}
	while(mmsd_resp());			//ﾚｽﾎﾟﾝｽが0になるまで繰返す

	//ﾃﾞｰﾀﾄｰｸﾝ開始待ち
//DBG_C('t');
	while(spi_trans(0xff)!=0xfe){};		//ﾃﾞｰﾀ受信開始まで繰返す
//DBG_C('o');
}


//==============================================
//音声C選択処理(API関数)
//==============================================
static void VOICEC_SEL(				//戻り値無し
	unsigned short no)			//音声番号(0は再生中止)
{

//DBG_C('\n');
//DBG_C('s');
//DBG_S(no);
//DBG_D(voiceC_idx,40);
//DBG_C('i');
	if(no)					//再生開始のとき
	{
		if(no>sizeof(voiceC_idx)/sizeof(voiceC_idx[0])) return;
						//実装数を超えているときは戻る
		if(voiceC.no==no) return;	//発声中の音声番号と同じ場合は戻る
		voiceC.no=no--;			//音声番号を設定して､番号を0起算にする
//DBG_S(voiceC.no);
		voiceC.begin=voiceC_idx[no].adr;//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽを設定
//DBG_L(voiceC.begin);
		voiceC.end=voiceC.begin+voiceC_idx[no].len*2;
						//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽを設定
//DBG_L(voiceC.end);
		voiceC.bct=0;			//ﾌﾞﾛｯｸ内ﾃﾞｰﾀｱﾄﾞﾚｽｶｳﾝﾀを初期化
//DBG_S(voiceC.bct);
		voiceC.tobi=voiceC_idx[no].tobi;//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]を設定
//DBG_S(voiceC.tobi);
		voiceC.of=255;			//再生開始時にﾃﾞｰﾀを取得するようにしておく
//DBG_S(voiceC.of);
		voiceC.pcm=0;			//PCM値を初期化
//DBG_B(voiceC.pcm);
	}
	else					//再生中断のとき
	{
		if(voiceC.no)			//再生中だったとき
		{
			voiceC.begin=voiceC.end-2;
						//最終ﾍﾟｰｼﾞにしておく
			voiceC.no=0xffff;	//再生終了中にする
		}
	}
//DBG_D(&voiceC,sizeof(voiceC));
}


//==============================================
//音声C再生処理ｻﾝﾌﾟﾙﾚｰﾄ設定(API関数)
//==============================================
static void VOICEC_SMP(				//戻り値無し
	unsigned smp)				//再生処理ｻﾝﾌﾟﾙﾚｰﾄ[sps]
{
//DBG_C('\n');
//DBG_C('p');

	voiceC.tobi=(unsigned long)256*smp*PWM_PR/1000000;
//DBG_S(voiceC.tobi);
}


//==============================================
//音声C再生処理
//==============================================
static void voiceC_exe(void)
{

//DBG_C('\n');
//DBG_C('p');
//DBG_S(voiceC.bct);
//DBG_L(voiceC.begin);
	voiceC.of+=voiceC.tobi;			//音声ﾃﾞｰﾀのｵﾌｾｯﾄを進める
//DBG_S(voiceC.of);
	if(!(voiceC.of&0xff00)) return;		//ｵﾌｾｯﾄの整数部が同じだったら戻る
	voiceC.of&=0x00ff;			//ｵﾌｾｯﾄの整数部をｸﾘｱしておく
//DBG_S(voiceC.of);
	if(!(voiceC.bct++))			//ﾍﾟｰｼﾞ先頭のとき
	{
//DBG_C('r');
		mmsd_read();			//SD読込み開始を設定する
	}
	voiceC.pcm=(short)spi_trans(0xff)-128;	//音声ﾃﾞｰﾀを取出して､
						//　8ﾋﾞｯﾄPCM形式をsignedに変換
//DBG_S(voiceC.pcm);
	voiceC.pcm=(long)voiceC.pcm*(VOICEC_VOL*PWM_STEP/100)/128;
						//音量配分を掛ける
//DBG_S(voiceC.pcm);
	if(voiceC.bct==512)			//ﾌﾞﾛｯｸの最後になったら
	{
		voiceC.bct=0;			//ﾌﾞﾛｯｸ内ﾊﾞｲﾄｱﾄﾞﾚｽをｸﾘｱする
		if((voiceC.begin+=2)==voiceC.end)
						//音声ﾃﾞｰﾀの最後になったら
		{
//DBG_C('e');
//DBG_S(voiceC.no);

			//SDを初期化
			mmsd_init();		//SDCを初期化する

			//再生終了
			if(voiceC.no!=0xffff)
			{
				voiceC.no=0;	//再生終了にする
				CB_VOICEC_END();//音声再生終了処理(ｺｰﾙﾊﾞｯｸ関数)を呼出す
			}
			voiceC.no=0;		//再生終了にする
		}
	}
}
#endif


#if VOICE_S>0				//SPIﾒﾓﾘを使うとき
#ifdef SLEEP_EN				//Sleep機能を実装するとき
//==============================================
//W25をﾊﾟﾜｰﾀﾞｳﾝ(API関数)
//==============================================
static void W25_PWR_DOWN(void)
{

	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
	GPIO_L(SPI_SS_GPIO,SPI_SS_BIT);		//ﾁｯﾌﾟｾﾚｸﾄ
	spi_send(W25_PwrDwn,1);			//ﾊﾟﾜｰﾀﾞｳﾝｺﾏﾝﾄﾞ
	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
	wait_us(3);				//ﾊﾟﾜｰﾀﾞｳﾝ時間を待つ
}
#endif


//==============================================
//W25をﾊﾟﾜｰｱｯﾌﾟ(API関数)
//==============================================
static void W25_PWR_UP(void)
{

	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
	GPIO_L(SPI_SS_GPIO,SPI_SS_BIT);		//ﾁｯﾌﾟｾﾚｸﾄ
	spi_send(W25_Re_PwrDwn,0);		//ﾊﾟﾜｰﾀﾞｳﾝ解除ｺﾏﾝﾄﾞ
	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//SSをﾈｹﾞｰﾄする
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
	wait_us(3);				//ﾊﾟﾜｰｱｯﾌﾟ時間を待つ
}


//==============================================
//音声S選択処理(API関数)
//==============================================
static void VOICES_SEL(				//戻り値無し
	unsigned char ch,			//ch番号
	unsigned short no)			//音声番号(0は再生中止)
{
	struct VOICES *voiceS_pt;		//音声S再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ

//DBG_C('\n');
//DBG_C('s');
//DBG_S(no);
//DBG_D(voiceS_idx,sizeof(voiceS_idx));
//DBG_C('i');
	voiceS_pt=voiceS+ch;			//音声S再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀを設定
	if(no)					//再生開始のとき
	{
		if(no>sizeof(voiceS_idx)/sizeof(voiceS_idx[0])) return;
						//実装数を超えているときは戻る
		if(voiceS_pt->no==no) return;	//発声中の音声番号と同じ場合は戻る
		voiceS_pt->no=no--;		//音声番号を設定して､番号を0起算にする
//DBG_S(voiceS_pt->no);
		voiceS_pt->begin=voiceS_idx[no].adr;
						//音声ﾃﾞｰﾀの開始ｱﾄﾞﾚｽを設定
//DBG_L(voiceS_pt->begin);
		voiceS_pt->end=voiceS_pt->begin+voiceS_idx[no].len;
						//音声ﾃﾞｰﾀの終了ｱﾄﾞﾚｽを設定
//DBG_L(voiceS_pt->end);
		voiceS_pt->tobi=voiceS_idx[no].tobi;
						//再生処理のｻﾝﾌﾟﾙ飛び数[1/256]を設定
//DBG_S(voiceS_pt->tobi);
		voiceS_pt->of=255;		//再生開始時にﾃﾞｰﾀを取得するようにしておく
//DBG_S(voiceS_pt->of);
		voiceS_pt->pcm=0;		//PCM値を初期化
//DBG_S(voiceS_pt->pcm);
#if VOICE_S==1				//単一ch実装のとき
		spi_addr=0;		//SPIｱﾄﾞﾚｽ設定未にする
		GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);
						//SSをﾈｹﾞｰﾄする
		SPI_SS_WAIT();		//SSﾈｹﾞｰﾄ時間を待つ
#endif
	}
	else					//再生中断のとき
	{
		voiceS_pt->no=0;		//再生終了にする
#if VOICE_S<2				//単一ch実装のとき
		GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);	//SSをﾈｹﾞｰﾄする
		SPI_SS_WAIT();			//SSﾈｹﾞｰﾄ時間を待つ
#endif
	}
//DBG_D(&voiceS,sizeof(voiceS));
}


//==============================================
//音声S再生処理ｻﾝﾌﾟﾙﾚｰﾄ設定(API関数)
//==============================================
static void VOICES_SMP(				//戻り値無し
	unsigned char ch,			//ch番号
	unsigned smp)				//再生処理ｻﾝﾌﾟﾙﾚｰﾄ[sps]
{
//DBG_C('\n');
//DBG_C('p');

	voiceS[ch].tobi=(unsigned long)256*smp*PWM_PR/1000000;
//DBG_S(voiceS[ch].tobi);
}


//==============================================
//音声S再生処理
//==============================================
static void voiceS_exe(
	unsigned char ch)			//ch番号
{
	struct VOICES *voiceS_pt;		//音声S再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
#if VOICE_S>1&&SPI_BUF_N>1		//SPI音声複数ch且つSPIﾊﾞｯﾌｧ複数のとき
	unsigned char n;
#endif

//DBG_C('\n');
//DBG_C('p');
//DBG_B(ch);
	voiceS_pt=voiceS+ch;			//音声S再生制御ﾃｰﾌﾞﾙﾎﾟｲﾝﾀを設定する
//DBG_L(voiceS_pt->begin);
	voiceS_pt->of+=voiceS_pt->tobi;		//音声ﾃﾞｰﾀのｵﾌｾｯﾄを進める
//DBG_S(voiceS_pt->of);
	if(!(voiceS_pt->of&0xff00)) return;	//ｵﾌｾｯﾄの整数部が同じだったら戻る
	voiceS_pt->of&=0x00ff;			//ｵﾌｾｯﾄの整数部をｸﾘｱしておく
//DBG_S(voiceS_pt->of);
#if VOICE_S==1				//単一ch実装のとき
	if(!spi_addr)				//SPIｱﾄﾞﾚｽ設定未のとき
#endif
#if VOICE_S>1&&SPI_BUF_N>1		//SPI音声複数ch且つSPIﾊﾞｯﾌｧ複数のとき
	if(!voiceS_pt->spi_buf_n)		//SPIﾊﾞｯﾌｧにﾃﾞｰﾀが無いとき
#endif
	{
		GPIO_L(SPI_SCK_GPIO,SPI_SCK_BIT);
						//SCKをL
		GPIO_L(SPI_SS_GPIO,SPI_SS_BIT);	//ﾁｯﾌﾟｾﾚｸﾄ
		spi_send(W25_ReadData,0);	//ReadDataｺﾏﾝﾄﾞ
		spi_send(voiceS_pt->begin>>16,0);
						//ｱﾄﾞﾚｽ(MSB)
		spi_send(voiceS_pt->begin>>8,0);//ｱﾄﾞﾚｽ
		spi_send(voiceS_pt->begin,0);	//ｱﾄﾞﾚｽ(LSB)
#if VOICE_S==1				//単一ch実装のとき
		spi_addr=1;			//SPIｱﾄﾞﾚｽ設定済にする
#endif
#if VOICE_S>1&&SPI_BUF_N>1		//SPI音声複数ch且つSPIﾊﾞｯﾌｧ複数のとき
		for(n=0;n<SPI_BUF_N;n++)
		{
			voiceS_pt->spi_buf[n]=spi_recv();
		}
		voiceS_pt->spi_buf_n=SPI_BUF_N;
#endif
	}
#if VOICE_S>1&&SPI_BUF_N>1		//SPI音声複数ch且つSPIﾊﾞｯﾌｧ複数のとき
	voiceS_pt->pcm=(short)(voiceS_pt->spi_buf[SPI_BUF_N-voiceS_pt->spi_buf_n--])-128;
						//音声ﾃﾞｰﾀを取出して､
						//　8ﾋﾞｯﾄPCM形式をsignedに変換
#else					//SPI音声単一又はSPIﾊﾞｯﾌｧ無しのとき
	voiceS_pt->pcm=(short)spi_recv()-128;	//音声ﾃﾞｰﾀを取出して､
						//　8ﾋﾞｯﾄPCM形式をsignedに変換
#endif
#if VOICE_S>1				//複数ch実装のとき
	GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);		//ﾁｯﾌﾟﾃﾞｾﾚｸﾄ
	SPI_SS_WAIT();				//SSﾈｹﾞｰﾄ時間を待つ
#endif
//DBG_S(voiceS_pt->pcm);
	voiceS_pt->pcm=(long)voiceS_pt->pcm*voiceS_vol[ch]/128;
						//音量配分を掛ける
//DBG_S(voiceS_pt->pcm);
	if(++voiceS_pt->begin>=voiceS_pt->end)	//音声ﾃﾞｰﾀが終わるとき
	{
//DBG_C('e');
		voiceS_pt->no=0;		//再生終了にする
#if VOICE_S==1				//単一ch実装のとき
		GPIO_H(SPI_SS_GPIO,SPI_SS_BIT);	//SSをﾈｹﾞｰﾄする
		SPI_SS_WAIT();			//SSﾈｹﾞｰﾄ時間を待つ
#endif
		CB_VOICES_END(ch);		//音声再生終了処理(ｺｰﾙﾊﾞｯｸ関数)を呼出す
	}
}
#endif


//==============================================
//ﾊﾞｯﾌｧへ詰める
//ﾊﾞｯﾌｧﾌﾙの場合は待合せる
//==============================================
static void put_buf(
	short pcm)				//PCMﾃﾞｰﾀ
{
	unsigned char n;

//DBG_S(pcm);
	n=buf_write;				//書込みﾎﾟｲﾝﾀを取得する
	if(++n==OUT_BUF_N) n=0;			//書込み位置を仮に進める
	while(n==buf_read);			//ﾊﾞｯﾌｧが空くのを待つ
	buf[buf_write]=pcm;			//ﾃﾞｰﾀをﾊﾞｯﾌｧに書込む
	buf_write=n;				//書込みﾎﾟｲﾝﾀを更新する
}


//==============================================
//ﾒｲﾝ処理
//==============================================
int main(void)
{

	CB_INIT();				//ﾃﾞﾊﾞｲｽの初期設定

#if PART_N+KB_N>0				//ｵﾙｺﾞｰﾙ演奏･KB演奏を実装するとき
	//&SONG_IDXをﾘﾝｸするためのおまじない
	void song_idx_dmy(void);		//song_idxのｴﾝﾄﾘ
	song_idx_dmy();				//ﾀﾞﾐｰｺｰﾙする
#endif


//==============================================
//ﾒｲﾝﾙｰﾌﾟ
//==============================================
	while(1)
	{
//DBG_C('\n');
//DBG_C('m');


//==============================================
//固有処理
//==============================================
//DBG_S(kyotuu.ps);
		if(!(kyotuu.ps--))		//固有処理のﾀｲﾐﾝｸﾞになったら
		{
//DBG_C('k');
			kyotuu.ps=(unsigned long)KOYUU_PR*1000/PWM_PR-1;
						//固有処理ﾌﾟﾘｽｹｰﾗを初期化
			CB_KOYUU();		//固有処理(ｺｰﾙﾊﾞｯｸ関数)を呼出す
		}


//==============================================
//出力事前処理
//==============================================
		kyotuu.pcm=0;			//PCM合算値をｸﾘｱしておく
//DBG_C('c');


#if PART_N+KB_N>0
//==============================================
//ｵﾙｺﾞｰﾙ演奏処理
//==============================================
//DBG_B(song.song_ps);
		if(!(song.song_ps--))	//演奏処理のﾀｲﾐﾝｸﾞになったら
		{
//DBG_C('o');
			song.song_ps=SONG_PS-1;	//演奏処理ﾌﾟﾘｽｹｰﾗを初期化
			song_exe();		//演奏処理を実行する
		}
		kyotuu.pcm+=song.pcm;		//PCM値を反映する
#endif


//DBG_C('n');
#if VOICE_N>0
//==============================================
//音声0再生処理
//==============================================
		if(voiceN[0].no)		//再生中のとき
		{
			voiceN_exe(0);		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceN[0].pcm;	//PCM値を反映する
#endif


#if VOICE_N>1
//==============================================
//音声1再生処理
//==============================================
		if(voiceN[1].no)		//再生中のとき
		{
			voiceN_exe(1);		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceN[1].pcm;	//PCM値を反映する
#endif


#if VOICE_N>2
//==============================================
//音声2再生処理
//==============================================
		if(voiceN[2].no)		//再生中のとき
		{
			voiceN_exe(2);		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceN[2].pcm;	//PCM値を反映する
#endif


#if VOICE_I>0
//==============================================
//音声I再生処理
//==============================================
//DBG_C('i');
		if(voiceI.no)			//再生中のとき
		{
			voiceI_exe();		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceI.pcm;		//PCM値を反映する
#endif


#if VOICE_C>0
//==============================================
//音声C再生処理
//==============================================
//DBG_C('c');
		if(voiceC.no)			//再生中のとき
		{
			voiceC_exe();	//演奏処理を実行する
		}
		kyotuu.pcm+=voiceC.pcm;	//PCM値を反映する
#endif


#if VOICE_S>0
//DBG_C('s');
//==============================================
//音声S0再生処理
//==============================================
		if(voiceS[0].no)		//再生中のとき
		{
			voiceS_exe(0);		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceS[0].pcm;	//PCM値を反映する
#endif


#if VOICE_S>1
//==============================================
//音声S1再生処理
//==============================================
		if(voiceS[1].no)		//再生中のとき
		{
			voiceS_exe(1);		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceS[1].pcm;	//PCM値を反映する
#endif


#if VOICE_S>2
//==============================================
//音声S2再生処理
//==============================================
		if(voiceS[2].no)		//再生中のとき
		{
			voiceS_exe(2);		//演奏処理を実行する
		}
		kyotuu.pcm+=voiceS[2].pcm;	//PCM値を反映する
#endif


//==============================================
//ﾊﾞｯﾌｧ書出し
//==============================================
//DBG_C('b');
//DBG_S(kyotuu.pcm);
		if(kyotuu.pcm>=(PWM_STEP+1)/2) kyotuu.pcm=(PWM_STEP+1)/2-1;
						//有効な範囲に規制する
		else if(kyotuu.pcm<=-PWM_STEP/2) kyotuu.pcm=-PWM_STEP/2+1;
		put_buf(kyotuu.pcm);		//PCM形式でﾊﾞｯﾌｧに出力する
	}
}
