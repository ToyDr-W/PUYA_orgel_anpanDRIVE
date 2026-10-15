;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;‹~‹}Ô»²Úİ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
	bgn_mac
;
;Êß°Ä0‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
AMBULANCE0_onpu_tbl
	vel_mac	127		;ÍŞÛ¼Ã¨İ’è
AMBULANCE0_onpu_tbl_1
	onp_mac	0,kK4,3,Si
	onp_mac	0,kK4,3,So
	jmp_mac AMBULANCE0_onpu_tbl_1-AMBULANCE0_onpu_tbl

;
;Êß°Ä1‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
AMBULANCE1_onpu_tbl
	vel_mac	127		;ÍŞÛ¼Ã¨İ’è
	piw_mac	pitch_AMBULANCE	;Ëß¯ÁÍŞİÄŞ”gŒ`
	pia_mac	pitch_AMBULANCE
	pit_mac	100		;Ëß¯ÁÍŞİÄŞÀ²Ñİ’è
AMBULANCE1_onpu_tbl_1
	onp_mac	0,kK4,2,Si
	onp_mac	0,kK4,2,So
	jmp_mac AMBULANCE1_onpu_tbl_1-AMBULANCE1_onpu_tbl
	end_mac

	bgn_mac
;
;¿İ¸ŞÃŞ°ÀÃ°ÌŞÙ
;
song_AMBULANCE
	ptr_mac NULL		;’P‹È‰‰‘t
	ops_mac 60		;ÃİÎß[4•ª‰¹•„/•ª]
	kys_mac 0		;ˆÚ’²“x[”¼‰¹ŠK]
	pns_mac 2		;À‘•Êß°Ä”
	ptr_mac	AMBULANCE0_onpu_tbl	;Êß°Ä0‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	wave_SIN14	;Êß°Ä0‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä0´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä0´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 0		;Êß°Ä0Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä0Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	ptr_mac	AMBULANCE1_onpu_tbl	;Êß°Ä1‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	wave_SHEPARD	;Êß°Ä1‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä1´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä1´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 1000		;Êß°Ä1Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac pitch_AMBULANCE	;Êß°Ä1Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	end_mac
	end