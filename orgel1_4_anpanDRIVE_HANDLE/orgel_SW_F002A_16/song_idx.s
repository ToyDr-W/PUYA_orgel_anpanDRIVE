	include	orgel_cnf.inc		;\¬î•ñ
	include	../orgel.inc		;±ÌßØ‚ÌÍ¯ÀÞÌ§²Ù
	if PART_N+KB_N>0		;µÙºÞ°Ù‰‰‘t¥KB‰‰‘t‚ðŽÀ‘•‚·‚é‚Æ‚«
	area	|.text|,code,readonly


;==============================================
;C‚Ö‚ÌØÝ¸±ÄÞÚ½
;==============================================
	align
	export	song_idx_dmy
song_idx_dmy
	bx	LR


	if PART_N>0			;µÙºÞ°Ù‰‰‘t‚ðŽÀ‘•‚·‚é‚Æ‚«
;==============================================
;¿Ý¸Þ²ÝÃÞ¯¸½
;==============================================
	align
	export	SONG_IDX
SONG_IDX
	dcw	(song_idx_end-song_idx_bgn)/SONG_IDX_SZ	;´ÝÄØ”
	dcw	0
song_idx_bgn

 son_mac song_AMBULANCE,song_AMBULANCE_DEF 	;1
 son_mac song_PATCAR,song_PATCAR_DEF 		;2
 son_mac song_FIRE,song_FIRE_DEF 		;3
 son_mac song_HORN,song_HORN_DEF 		;4
 son_mac song_ANPANMARCH,song_ANPANMARCH_DEF 	;5
 son_mac song_ANPANTAISO,song_ANPANTAISO_DEF 	;6

song_idx_end
	endif

;==============================================
;”gŒ`ÃÞ°À‚ÌŒÄž‚Ý
;==============================================
;	include	../enve_EXP.asm
	include	../enve_RSN.asm
;	include	../enve_VIB.asm
;	include	../enve_TREMOLO.asm
;	include	../enve_ELEGANTE.asm
	include	../wave_SIN14.asm
	include	../wave_SQUARE.asm
	include	../wave_SHEPARD.asm
;	include	../wave_RING4.asm
 if :def:song_AMBULANCE_DEF 
	include	../pitch_AMBULANCE.asm
 endif
 if :def:song_FIRE_DEF 
	include	../pitch_FIRE.asm
	include	../effect_KANE.asm
 endif
 if :def:song_PATCAR_DEF 
	include	../pitch_PATCAR.asm
 endif


;==============================================
;¿Ý¸ÞÃÞ°À‚ÌŒÄž‚Ý
;==============================================
	if PART_N>0			;µÙºÞ°Ù‰‰‘t‚ðŽÀ‘•‚·‚é‚Æ‚«
	include ../song.inc
	endif
	endif
	end
