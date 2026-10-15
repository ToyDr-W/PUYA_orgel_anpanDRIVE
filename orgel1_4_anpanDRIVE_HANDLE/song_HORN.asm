;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;©“®ÔÎ°İ
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
	bgn_mac
;
;Êß°Ä0‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
HORN0_onpu_tbl
	vel_mac	127		;ÍŞÛ¼Ã¨İ’è
HORN0_onpu_tbl_1
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	onp_mac	0,kK2,2,So
	jmp_mac HORN0_onpu_tbl_1-HORN0_onpu_tbl

;
;Êß°Ä1‚Ì‰¹•„ÃŞ°ÀÃ°ÌŞÙ
;
HORN1_onpu_tbl
	vel_mac	127		;ÍŞÛ¼Ã¨İ’è
HORN1_onpu_tbl_1
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	onp_mac	0,kK2,3,ReS
	jmp_mac HORN1_onpu_tbl_1-HORN1_onpu_tbl
	end_mac

	bgn_mac
;
;¿İ¸ŞÃŞ°ÀÃ°ÌŞÙ
;
song_HORN
	ptr_mac NULL		;’P‹È‰‰‘t
	ops_mac 60		;ÃİÎß[4•ª‰¹•„/•ª]
	kys_mac 0		;ˆÚ’²“x[”¼‰¹ŠK]
	pns_mac 2		;À‘•Êß°Ä”
	ptr_mac	HORN0_onpu_tbl;Êß°Ä0‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	wave_SIN14	;Êß°Ä0‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä0´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä0´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 0		;Êß°Ä0Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä0Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	ptr_mac	HORN1_onpu_tbl;Êß°Ä1‰¹•„Ã°ÌŞÙÎß²İÀ
	ptr_mac	wave_SHEPARD	;Êß°Ä1‰¹Œ¹Ã°ÌŞÙÎß²İÀ
	eps_mac 0		;Êß°Ä1´İÍŞÛ°ÌßÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä1´İÍŞÛ°ÌßÃ°ÌŞÙÎß²İÀ
	pps_mac 0		;Êß°Ä1Ëß¯ÁÍŞİÄŞÀ²Ñ ÃİÎß=100Šî€[us/value/step]
	ptr_mac 0		;Êß°Ä1Ëß¯ÁÍŞİÄŞÃ°ÌŞÙÎß²İÀ
	end_mac
	end