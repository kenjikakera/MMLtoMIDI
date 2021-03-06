/*
	msdrv4 定義
*/


#define TIME_BASE_EQU	0x80	/* set time base ( 80h, time_base_low, time_base_high ) */
#define SB16_CMD_EQU	0x81	/* sb16 command 
					81h,00h,00h,00h = 2op mode
				 	81h,00h,00h,01h = 4op mode */
#define VC		0x82	/*［＠ｎ］ＶＯＩＣＥ　ＣＡＨＮＧＥ	（０−ＦＦＨ） */
#define PRESS_DATA EQU	0x83	/* press data ( 83h, start pointer, end pointer ) */
#define PRESS_END_EQU	0x84	/* press end */
#define VOL		0x85	/* ボリューム
					［Ｖｎ］ボリューム			（０−０ＦＨ）
					［＠Ｖｎ］ボリューム			（０−７ＦＨ）
				*/
#define TEMPO		0x8A	/*［Ｔｎ］テンポセット			（０−Ｆ０Ｈ）*/
#define DRIVER_MODE	0x8B	/* driver mode data0 = ( 0 = 4 byte mode / 1 = 3 byte mode ) */
#define SSG_PROGRAM	0x8c	/* ssg program, program num, data num, data
					data number ( 0-15 = data / 128-139 = name ) */
#define SSG_PACKET	0x8d	/* ssg program, program num, data num, data len, data
					data number ( 0-15 = data / 128-139 = name ) */
#define OPN_PROGRAM	0x8e	/* opn program, program num, data num, data
					data number ( 0-47 = data / 128-139 = name ) */
#define OPN_PACKET	0x8f	/* opn program, program num, data num, data len, data
					data number ( 0-47 = data / 128-139 = name ) */

#define YCOM		0x94	/*［Ｙｎ，ｄ］Ｙコマンド（Ｙｎ，ｄ）ＦＭレジスタ直書き */
#define NOOP		0x96	/* ［：］ノーオペレーション		（０−ＦＦＨ） */
#define MLOOP		0x9B	/* ＤＢ　ＭＬＯＯＰ、ループ数 */
#define MLOOPS		0x9C	/* ＬＯＯＰの最初 */
#define CH_DT		0x9D	/* チャンネルディチューン */
#define NOP1		0x9E	/* ＮＯＰの１バイト */
#define PAN		0x9F	/* ＳＥＴ＿ＰＡＮ　データ */
#define PITCH_BEND 	0xA4	/* ピッチベンド */
#define HARD_LFO_FREQ	0xA6	/* ハードウエアＬＦＯのＯＮ／ＯＦＦと周波数 （０〜８） */
#define START_EFFECT	0xA7	/* 効果音発生 */
#define MAX_TRC	EQU	0xA8	/* 最大使用トラック（デフォルト　FM=6/ｽﾃﾚｵFM=10/MIDI=10） */
#define CH3_MODE EQU	0xA9	/* ＣＨ３のモード */
#define CH3_OP_MASK 	0xAA	/* ＣＨ３のオペレーターマスク */
#define LFO_PITCH 	0xAB	/* 周波数変調（ＬＦＯ　ＰＩＴＣＨ） */
#define LFO_AMP		0xAC	/* 音量変調（ＬＦＯ　ＡＭＰ） */
#define PORTAMENTO_VAL  0xAD	/* ポルタメントバリューセット */
#define PORTAMENTO_END  0xAE	/* ポルタメント終了セット */
#define PORTAMENTO_NOW  0xAF	/* 現在のポルタメントセット */


#define MUSIC_TIMING	0xC1	/* 音楽ドライバーから送られてくる信号 */

#define EX_CLEAR_SUM	0xC2	/*  exclusive clear check sum */
#define EX_PARA		0xC3	/*  exclusive put para, data */
#define EX_PUT_SUM 	0xC4	/*  exclusive clear put sum */
#define EX_PACKET 	0xC5	/*  exclusive packet, length ( word ), data .... */


#define RHYTHM_SET	0xD0	/* リズム音源キーオン　ＳＴＥＰ、フラグ */
#define RHYTHM_PAN_VOL0 0xD1	/* リズム音源のＢＤのＰＡＮとＶＯＬ */
#define RHYTHM_PAN_VOL1 0xD2	/* リズム音源のＳＤのＰＡＮとＶＯＬ */
#define RHYTHM_PAN_VOL2 0xD3	/* リズム音源のＴＯＰのＰＡＮとＶＯＬ */
#define RHYTHM_PAN_VOL3 0xD4	/* リズム音源のＨＨのＰＡＮとＶＯＬ */
#define RHYTHM_PAN_VOL4 0xD5	/* リズム音源のＴＯＭのＰＡＮとＶＯＬ */
#define RHYTHM_PAN_VOL5 0xD6	/* リズム音源のＲＩＭのＰＡＮとＶＯＬ */

#define ROLBASE		0xDD	/* ＲＯＬＡＮＤ */
#define ROLPARA		0xDE	/* ＲＯＬＡＮＤ */
#define ROLDEV		0xDF	/* ＲＯＬＡＮＤ */
 
#define BANK_PRG 	0xE2	/* ＳＴＥＰ、音色番号、ＢＡＮＫ番号 */
#define MIDI_CH		0xE6	/* ＳＴＥＰ、ＭＩＤＩ出力チャンネル（０〜１５） */
#define TEMPO2		0xE7	/* ＳＴＥＰ、テンポ、グラデーション */
#define AFTER_C		0xEA	/* ＳＴＥＰ、チャンネル・プレッシャー（０〜７ＦＨ） */
#define MIDI_CONTROL	0xEB	/* ＭＩＤＩコントロール */
#define PROGRAM		0xEC	/* ＳＴＥＰ、 */
#define AFTER_K		0xED	/* ＳＴＥＰ、ＫＥＹ、キー・プレッシャー（０〜７ＦＨ） */
#define PITCH		0xEE	/* ＳＴＥＰ、 */

#define ENDC		0xFE
#define ENDEND		0xFF
