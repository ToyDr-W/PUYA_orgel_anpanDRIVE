;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ﾊﾟﾄｶｰｻｲﾚﾝ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
	bgn_mac
;
;ﾊﾟｰﾄ0の音符ﾃﾞｰﾀﾃｰﾌﾞﾙ
;
PATCAR0_onpu_tbl
	vel_mac	127		;ﾍﾞﾛｼﾃｨ設定
	piw_mac	pitch_PATCAR	;ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形
	pia_mac	pitch_PATCAR
	pit_mac	40		;ﾋﾟｯﾁﾍﾞﾝﾄﾞﾀｲﾑ設定
PATCAR0_onpu_tbl_1
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	onp_mac	0,kK2,3,Fa
	jmp_mac PATCAR0_onpu_tbl_1-PATCAR0_onpu_tbl

;
;ﾊﾟｰﾄ1の音符ﾃﾞｰﾀﾃｰﾌﾞﾙ
;
PATCAR1_onpu_tbl
	vel_mac	127		;ﾍﾞﾛｼﾃｨ設定
	piw_mac	pitch_PATCAR	;ﾋﾟｯﾁﾍﾞﾝﾄﾞ波形
	pia_mac	pitch_PATCAR
	pit_mac	40		;ﾋﾟｯﾁﾍﾞﾝﾄﾞﾀｲﾑ設定
PATCAR1_onpu_tbl_1
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	onp_mac	0,kK2,3,La
	jmp_mac PATCAR1_onpu_tbl_1-PATCAR1_onpu_tbl
	end_mac

	bgn_mac
;
;ｿﾝｸﾞﾃﾞｰﾀﾃｰﾌﾞﾙ
;
song_PATCAR
	ptr_mac NULL		;単曲演奏
	ops_mac 60		;ﾃﾝﾎﾟ[4分音符/分]
	kys_mac 0		;移調度[半音階]
	pns_mac 2		;実装ﾊﾟｰﾄ数
	ptr_mac	PATCAR0_onpu_tbl;ﾊﾟｰﾄ0音符ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	ptr_mac	wave_SQUARE	;ﾊﾟｰﾄ0音源ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	eps_mac 0		;ﾊﾟｰﾄ0ｴﾝﾍﾞﾛｰﾌﾟﾀｲﾑ ﾃﾝﾎﾟ=100基準[us/value/step]
	ptr_mac 0		;ﾊﾟｰﾄ0ｴﾝﾍﾞﾛｰﾌﾟﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	pps_mac 1000		;ﾊﾟｰﾄ0ﾋﾟｯﾁﾍﾞﾝﾄﾞﾀｲﾑ ﾃﾝﾎﾟ=100基準[us/value/step]
	ptr_mac pitch_PATCAR	;ﾊﾟｰﾄ0ﾋﾟｯﾁﾍﾞﾝﾄﾞﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	ptr_mac	PATCAR1_onpu_tbl;ﾊﾟｰﾄ1音符ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	ptr_mac	wave_SHEPARD	;ﾊﾟｰﾄ1音源ﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	eps_mac 0		;ﾊﾟｰﾄ1ｴﾝﾍﾞﾛｰﾌﾟﾀｲﾑ ﾃﾝﾎﾟ=100基準[us/value/step]
	ptr_mac 0		;ﾊﾟｰﾄ1ｴﾝﾍﾞﾛｰﾌﾟﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	pps_mac 1000		;ﾊﾟｰﾄ1ﾋﾟｯﾁﾍﾞﾝﾄﾞﾀｲﾑ ﾃﾝﾎﾟ=100基準[us/value/step]
	ptr_mac pitch_PATCAR	;ﾊﾟｰﾄ1ﾋﾟｯﾁﾍﾞﾝﾄﾞﾃｰﾌﾞﾙﾎﾟｲﾝﾀ
	end_mac
	end