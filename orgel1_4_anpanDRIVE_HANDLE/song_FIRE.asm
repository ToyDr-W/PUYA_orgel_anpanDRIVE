;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Á–hÔ»²Úİ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
	bgn_mac
;
;Êß°Ä0‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
FIRE0_onpu_tbl
	vel_mac	63		;ÍŞÛ¼Ã¨İ’è
	piw_mac	pitch_FIRE	;Ëß¯ÁÍŞİÄŞ”gŒ`
	pia_mac	pitch_FIRE
	pit_mac	50		;Ëß¯ÁÍŞİÄŞÀ²Ñİ’è
FIRE0_onpu_tbl_1
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	onp_mac	0,kK1,3,Mi
	jmp_mac FIRE0_onpu_tbl_1-FIRE0_onpu_tbl

;
;Êß°Ä1‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
FIRE1_onpu_tbl
	vel_mac	127		;ÍŞÛ¼Ã¨İ’è
FIRE1_onpu_tbl_1
	onp_mac	0,kK2,0,0
	efw_mac	kK8,effect_KANE,4000
	efa_mac	kK8,effect_KANE,4000
	efw_mac	kK8,effect_KANE,4000
	efa_mac	kK8,effect_KANE,4000
	efw_mac	kK8,effect_KANE,4000
	efa_mac	kK8,effect_KANE,4000
	onp_mac	0,kK4,0,0
	jmp_mac FIRE1_onpu_tbl_1-FIRE1_onpu_tbl

;
;Êß°Ä2‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
FIRE2_onpu_tbl
	vel_mac	63		;ÍŞÛ¼Ã¨İ’è
	piw_mac	pitch_FIRE	;Ëß¯ÁÍŞİÄŞ”gŒ`
	pia_mac	pitch_FIRE
	pit_mac	50		;Ëß¯ÁÍŞİÄŞÀ²Ñİ’è
FIRE2_onpu_tbl_1
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	onp_mac	0,kK1,3,SoS
	jmp_mac FIRE2_onpu_tbl_1-FIRE2_onpu_tbl
	end_mac

	bgn_mac
;
;¿İ¸ŞÃŞ°ÀÃ°ÌŞÙ
;
song_FIRE
	ptr_mac NULL		;’P‹È‰‰‘t
	ops_mac 60		;ÃİÎß[4•ª‰¹•„/•ª]
	kys_mac 0		;ˆÚ’²“x[”¼‰¹ŠK]
	pns_mac 3		;À‘•Êß°Ä”
	ptr_mac	FIRE0_onpu_tbl	;Êß°Ä0‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	wave_SQUARE	;Êß°Ä0‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä0´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä0´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 1000		;Êß°Ä0Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac pitch_FIRE	;Êß°Ä0Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	ptr_mac	FIRE1_onpu_tbl	;Êß°Ä1‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	0		;Êß°Ä1‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä1´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä1´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 0		;Êß°Ä1Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä1Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	ptr_mac	FIRE2_onpu_tbl	;Êß°Ä2‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	wave_SHEPARD	;Êß°Ä2‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä2´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä2´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 1000		;Êß°Ä2Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac pitch_FIRE	;Êß°Ä2Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	end_mac
	end