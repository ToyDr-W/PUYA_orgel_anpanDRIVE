echo off
del orgel_cnf.h
del orgel_cnf.inc
rem//==============================================
rem//構成情報(ｵﾙｺﾞｰﾙ演奏+音声再生処理に関わるもの)
rem//==============================================
call make_cnf.bat	BTL 		3	//BTL出力指定
rem						//　0=ｼﾝｸﾞﾙ出力
rem						//　1=ｺﾝﾌﾟﾘ出力
rem						//　2=ﾌﾞﾘｯｼﾞ出力
rem						//　3=ﾌﾞﾚｰｷ出力
call make_cnf.bat	PWM_STEP	999	//PWMｽﾃｯﾌﾟ数
rem						//　PWM周波数=48MHz/1000=48KHz
call make_cnf.bat	PWM_DB_CMP	0x10	//PWMﾃﾞｯﾄﾞﾊﾞﾝﾄﾞ(ｺﾝﾌﾟﾘ出力時)[BDTRb7-0]
call make_cnf.bat	PWM_DB_BRG	10	//PWMﾃﾞｯﾄﾞﾊﾞﾝﾄﾞ(ﾌﾞﾘｯｼﾞ出力時)[0.021us]
call make_cnf.bat	OUT_BUF_N	50	//出力ﾊﾞｯﾌｧｻｲｽﾞ
call make_cnf.bat	SPI_BUF_N	8	//SPIﾊﾞｯﾌｧｻｲｽﾞ
call make_cnf.bat	PWM_PR		21	//PWM周期[us]
call make_cnf.bat	SONG_PS		2	//演奏処理ﾌﾟﾘｽｹｰﾙ値
rem						//　演奏処理周期=PWM_PR*SONG_PS
call make_cnf.bat	KOYUU_PR	50	//固有処理のｺｰﾙﾊﾞｯｸ周期[ms]
call make_cnf.bat	VOICEN0_VOL	80	//音声N0のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICEN1_VOL	80	//音声N1のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICEN2_VOL	80	//音声N2のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICEI_VOL	100	//音声Iのﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICEC_VOL	100	//音声Cのﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICES0_VOL	100	//音声S0のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICES1_VOL	100	//音声S1のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICES2_VOL	100	//音声S2のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	ORGEL_VOL	80	//ｵﾙｺﾞｰﾙ演奏のﾎﾞﾘｭｰﾑ配分[%]
call make_cnf.bat	VOICE_N		3	//音声N発声数の最大値(最大値3､
rem						//　0のときは音声再生機能を実装しない)
call make_cnf.bat	VOICE_I		0	//音声I発声数の最大値(最大値1､
rem						//　0のときは音声再生機能を実装しない)
call make_cnf.bat	VOICE_C		0	//音声C発声数の最大値(最大値1､
rem						//　0のときは音声再生機能を実装しない)
call make_cnf.bat	VOICE_S		0	//音声S発声数の最大値(
rem						//　0のときは音声再生機能を実装しない)
call make_cnf.bat	PART_N		4	//演奏ﾊﾟｰﾄ数の最大値(最大値4､
rem						//　0のときはｵﾙｺﾞｰﾙ演奏機能を
rem						//　実装しない)
call make_cnf.bat	KB_N		0	//KB演奏有効ﾊﾟｰﾄ数の最大値
rem						//　(0のときはKB演奏機能を実装しない)
echo; end >> orgel_cnf.inc
