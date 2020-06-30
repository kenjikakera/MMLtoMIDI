/*
	msdrv4 ’è‹`
*/


#define TIME_BASE_EQU	0x80	/* set time base ( 80h, time_base_low, time_base_high ) */
#define SB16_CMD_EQU	0x81	/* sb16 command 
					81h,00h,00h,00h = 2op mode
				 	81h,00h,00h,01h = 4op mode */
#define VC		0x82	/*m—‚n‚u‚n‚h‚b‚d@‚b‚`‚g‚m‚f‚d	i‚O|‚e‚e‚gj */
#define PRESS_DATA EQU	0x83	/* press data ( 83h, start pointer, end pointer ) */
#define PRESS_END_EQU	0x84	/* press end */
#define VOL		0x85	/* ƒ{ƒŠƒ…[ƒ€
					m‚u‚nƒ{ƒŠƒ…[ƒ€			i‚O|‚O‚e‚gj
					m—‚u‚nƒ{ƒŠƒ…[ƒ€			i‚O|‚V‚e‚gj
				*/
#define TEMPO		0x8A	/*m‚s‚nƒeƒ“ƒ|ƒZƒbƒg			i‚O|‚e‚O‚gj*/
#define DRIVER_MODE	0x8B	/* driver mode data0 = ( 0 = 4 byte mode / 1 = 3 byte mode ) */
#define SSG_PROGRAM	0x8c	/* ssg program, program num, data num, data
					data number ( 0-15 = data / 128-139 = name ) */
#define SSG_PACKET	0x8d	/* ssg program, program num, data num, data len, data
					data number ( 0-15 = data / 128-139 = name ) */
#define OPN_PROGRAM	0x8e	/* opn program, program num, data num, data
					data number ( 0-47 = data / 128-139 = name ) */
#define OPN_PACKET	0x8f	/* opn program, program num, data num, data len, data
					data number ( 0-47 = data / 128-139 = name ) */

#define YCOM		0x94	/*m‚x‚C‚„n‚xƒRƒ}ƒ“ƒhi‚x‚C‚„j‚e‚lƒŒƒWƒXƒ^’¼‘‚« */
#define NOOP		0x96	/* mFnƒm[ƒIƒyƒŒ[ƒVƒ‡ƒ“		i‚O|‚e‚e‚gj */
#define MLOOP		0x9B	/* ‚c‚a@‚l‚k‚n‚n‚oAƒ‹[ƒv” */
#define MLOOPS		0x9C	/* ‚k‚n‚n‚o‚ÌÅ‰ */
#define CH_DT		0x9D	/* ƒ`ƒƒƒ“ƒlƒ‹ƒfƒBƒ`ƒ…[ƒ“ */
#define NOP1		0x9E	/* ‚m‚n‚o‚Ì‚PƒoƒCƒg */
#define PAN		0x9F	/* ‚r‚d‚sQ‚o‚`‚m@ƒf[ƒ^ */
#define PITCH_BEND 	0xA4	/* ƒsƒbƒ`ƒxƒ“ƒh */
#define HARD_LFO_FREQ	0xA6	/* ƒn[ƒhƒEƒGƒA‚k‚e‚n‚Ì‚n‚m^‚n‚e‚e‚Æü”g” i‚O`‚Wj */
#define START_EFFECT	0xA7	/* Œø‰Ê‰¹”­¶ */
#define MAX_TRC	EQU	0xA8	/* Å‘åg—pƒgƒ‰ƒbƒNiƒfƒtƒHƒ‹ƒg@FM=6/½ÃÚµFM=10/MIDI=10j */
#define CH3_MODE EQU	0xA9	/* ‚b‚g‚R‚Ìƒ‚[ƒh */
#define CH3_OP_MASK 	0xAA	/* ‚b‚g‚R‚ÌƒIƒyƒŒ[ƒ^[ƒ}ƒXƒN */
#define LFO_PITCH 	0xAB	/* ü”g”•Ï’²i‚k‚e‚n@‚o‚h‚s‚b‚gj */
#define LFO_AMP		0xAC	/* ‰¹—Ê•Ï’²i‚k‚e‚n@‚`‚l‚oj */
#define PORTAMENTO_VAL  0xAD	/* ƒ|ƒ‹ƒ^ƒƒ“ƒgƒoƒŠƒ…[ƒZƒbƒg */
#define PORTAMENTO_END  0xAE	/* ƒ|ƒ‹ƒ^ƒƒ“ƒgI—¹ƒZƒbƒg */
#define PORTAMENTO_NOW  0xAF	/* Œ»İ‚Ìƒ|ƒ‹ƒ^ƒƒ“ƒgƒZƒbƒg */


#define MUSIC_TIMING	0xC1	/* ‰¹Šyƒhƒ‰ƒCƒo[‚©‚ç‘—‚ç‚ê‚Ä‚­‚éM† */

#define EX_CLEAR_SUM	0xC2	/*  exclusive clear check sum */
#define EX_PARA		0xC3	/*  exclusive put para, data */
#define EX_PUT_SUM 	0xC4	/*  exclusive clear put sum */
#define EX_PACKET 	0xC5	/*  exclusive packet, length ( word ), data .... */


#define RHYTHM_SET	0xD0	/* ƒŠƒYƒ€‰¹Œ¹ƒL[ƒIƒ“@‚r‚s‚d‚oAƒtƒ‰ƒO */
#define RHYTHM_PAN_VOL0 0xD1	/* ƒŠƒYƒ€‰¹Œ¹‚Ì‚a‚c‚Ì‚o‚`‚m‚Æ‚u‚n‚k */
#define RHYTHM_PAN_VOL1 0xD2	/* ƒŠƒYƒ€‰¹Œ¹‚Ì‚r‚c‚Ì‚o‚`‚m‚Æ‚u‚n‚k */
#define RHYTHM_PAN_VOL2 0xD3	/* ƒŠƒYƒ€‰¹Œ¹‚Ì‚s‚n‚o‚Ì‚o‚`‚m‚Æ‚u‚n‚k */
#define RHYTHM_PAN_VOL3 0xD4	/* ƒŠƒYƒ€‰¹Œ¹‚Ì‚g‚g‚Ì‚o‚`‚m‚Æ‚u‚n‚k */
#define RHYTHM_PAN_VOL4 0xD5	/* ƒŠƒYƒ€‰¹Œ¹‚Ì‚s‚n‚l‚Ì‚o‚`‚m‚Æ‚u‚n‚k */
#define RHYTHM_PAN_VOL5 0xD6	/* ƒŠƒYƒ€‰¹Œ¹‚Ì‚q‚h‚l‚Ì‚o‚`‚m‚Æ‚u‚n‚k */

#define ROLBASE		0xDD	/* ‚q‚n‚k‚`‚m‚c */
#define ROLPARA		0xDE	/* ‚q‚n‚k‚`‚m‚c */
#define ROLDEV		0xDF	/* ‚q‚n‚k‚`‚m‚c */
 
#define BANK_PRG 	0xE2	/* ‚r‚s‚d‚oA‰¹F”Ô†A‚a‚`‚m‚j”Ô† */
#define MIDI_CH		0xE6	/* ‚r‚s‚d‚oA‚l‚h‚c‚ho—Íƒ`ƒƒƒ“ƒlƒ‹i‚O`‚P‚Tj */
#define TEMPO2		0xE7	/* ‚r‚s‚d‚oAƒeƒ“ƒ|AƒOƒ‰ƒf[ƒVƒ‡ƒ“ */
#define AFTER_C		0xEA	/* ‚r‚s‚d‚oAƒ`ƒƒƒ“ƒlƒ‹EƒvƒŒƒbƒVƒƒ[i‚O`‚V‚e‚gj */
#define MIDI_CONTROL	0xEB	/* ‚l‚h‚c‚hƒRƒ“ƒgƒ[ƒ‹ */
#define PROGRAM		0xEC	/* ‚r‚s‚d‚oA */
#define AFTER_K		0xED	/* ‚r‚s‚d‚oA‚j‚d‚xAƒL[EƒvƒŒƒbƒVƒƒ[i‚O`‚V‚e‚gj */
#define PITCH		0xEE	/* ‚r‚s‚d‚oA */

#define ENDC		0xFE
#define ENDEND		0xFF
