/*
	msdrv4の固有部分

=============================================================================
=============================================================================
	引数定義
=============================================================================
=============================================================================
*/

/*------------------------------------------------------------------------------
	driver config
------------------------------------------------------------------------------*/
#define CHANNEL			36		/* チャンネル数 */
#define MIDI_EFFECT_MAX 	8		/* midi 効果音の数 */
#define _MLOOP_DATA_MAX		8		/* loop のネスト数 */
#define MAX_SUBEOURINE		16		/* サブルーチンの最大登録数 */
#define MAX_POINTER		5		/* 何個までポインタを持つか */
#define MAX_MIDI_GATE		128		/* １２８音分のＧＡＴＥ */


/*------------------------------------------------------------------------------
	opn
------------------------------------------------------------------------------*/
/*
	timner_b = ( 3993.6 / 6 ) * ( 60000 / 16 / tempo / 3 ) / ( 192 / 48 )
	timner_b = 665.6 * ( 1250 / tempo ) / 4
	timner_b = ( 665.6 * 1250 ) / ( tempo * 4 )
	timner_b = 208000 / tempo
	
	60   :183:
*/
#define TIMER_B_HIGH		0x0003h		/* timer-b */
#define TIMER_B_LOW		0x2c80h

/*
	Ta                    = ( 1024 - Va ) * 3 / 166.4
	Ta * 166.4 / 3        =   1024 - Va
	Ta * 166.4 / 3 - 1024 =        - Va
	Va                    = - Ta * 166.4 / 3 + 1024
	
	Ta = 0.018 ms - 1.85
	Va = 0 - 1023
	
	1.67ms ( 1/60s )
	97.7                   = -16.7 * 166.4 / 3 + 1024
*/
#define LFO_SPEED		0x98
#define LFO_SPEED_H		(LFO_SPPED >> 2)
#define LFO_SPPED_L		(LFO_SPEED && 0b00000011)


/*------------------------------------------------------------------------------
	timer
------------------------------------------------------------------------------*/
#define CNT8MHZ_H		0x00724	/*  8Mhz counter 設定値の high ( 125*19968*48 ) */
#define CNT8MHZ_L		0x2000	/*  8Mhz counter 設定値の low  ( 125*19968*48 ) */
#define CNT8MHZ_16MS_H		0x00000	/*  8Mhz counter 設定値の high ( 1996.8*16.7) */
#define CNT8MHZ_16MS_L		0x08242	/*  8Mhz counter 設定値の low  ( 1996.8*16.7) */
#define CNT10MHZ_H		0x08ca	/* 10Mhz counter 設定値の high ( 125*24576*48 ) */
#define CNT10MHZ_L		0x0000	/* 10Mhz counter 設定値の low  ( 125*24576*48 ) */
#define cnt10mhz_16ms_h	equ	00000h	/* 10Mhz counter 設定値の high ( 2457.6*16.7) */
#define cnt10mhz_16ms_l	equ	0a051h	/* 10Mhz counter 設定値の low  ( 2457.6*16.7) */


/*------------------------------------------------------------------------------
	rs232c
------------------------------------------------------------------------------*/
#define RS232C_8MHZ	4	/* ８ＭｈｚのＲＳ２３２Ｃのカウンター設定値 */
#define RS232C_10MHZ	5	/* １０ＭｈｚのＲＳ２３２Ｃのカウンター設定値 */



/*------------------------------------------------------------------------------
	 ssg/opn/midi
------------------------------------------------------------------------------*/
struct _SSG_OPN_MIDI {
	WORD	CH_POINTER					/* 次の ms data への pointer */
	BYTE	DRIVER_MODE					/* driver mode
									0 = 4 byte mode keyNO/step/gate/vel
									1 = 3 byte mode keyNO/step/gate
								*/
	WORD _MLOOP_SP						/* loop の stack pointer */
	MLLOP_DATA_MAX_MLOOP_DATA  _MLOOP_DATA_MAX*MAX_POINTER	/* loop の戻り場所の pointer */

	WORD	PITCH_BEND					/* ピッチベンド */
	WORD	PORTAMENTO					/* ポルタメントの値 */
	WORD	PORTAMENTO_VAL					/* ポルタメントの幅 */
	WORD	PORTAMENTO_END					/* ポルタメントの最終地(0=ポルタしない)  */

	BYTE	VOL_DATA					/* volume */
	BYTE	STEP						/* 残り step */
	BYTE	MIDI_CH						/* 出力チャンネル */

	BYTE	PRESS_FLAG					/* press data を使用している最中か */
	BYTE	PRESS_DATA					/* press end を書き込む前のデーター */
	BYTE	PRESS_POINTER					/* press data の終了時に戻る場所 */
}

/*------------------------------------------------------------------------------
	 ＳＳＧ　＆　ＯＰＮ
------------------------------------------------------------------------------*/
struct _SSG_OPN {
	struct SSG_OPN _SSG_OPN_DATA
	struct SSG_OPN_SOFT_LFO _SSG_OPN_DATA_SOFT_LFO

}

struct _SSG_OPN_DATA {
	BYTE	GATE				/* ゲート */
	WORD	CH_DT				/* チャンネルディチューン */
	WORD	FREQ				/* 周波数
	BYTE	VOL_DATA2			/* ＶＯＬのカウンター */
	BYTE	KEY				/* 音階 */
	BYTE	OCT				/* オクターブ */
}

struct _SSG_OPN_SOFT_LFO {
	/* ＳＳＧ　＆　ＯＰＮ　のソフトウェアーＬＦＯデーター */
	BYTE	LFO_PSP				/*ＬＦＯの一段階のスピード */
	BYTE	LFO_PSP2			/* ＬＦＯスピードカウンター  (ＬＦＯ＿ＰＳＰ～０の範囲のカウンター)  */
	BYTE	LFO_PDELAY			/* ＬＦＯの掛かり始まるまでのディレイ */
	BYTE	LFO_PDELAY2			/* ＬＦＯ＿ＰＤＥＬＡＹ～０のカウンター */
	WORD	LFO_PWAVE			/* ＬＦＯの波形（波形番号＊２が入る） */
	WORD	LFO_PWAVE2			/* ワーク */
	WORD	LFO_PUPDOWN			/* ＬＦＯの波形が上か下か */
	WORD	LFO_PCO				/* ＬＦＯを何段階変調を掛けるか */
	WORD	LFO_PCO2			/* ＬＦＯ＿ＰＣＯ～０の範囲のカウンター */
	WORD	LFO_PM				/* 変調の深さ（値が－のときは、ＬＦＯ＿ＲＲＥＱが－となり、下に変調がかかる） */
	BYTE	LFO_PFREQ			/* ＬＦＯを何回掛けるか ( 0=無限 ) */
	BYTE	LFO_PFREQ2			/* ＬＦＯを何回掛けるかのカウンター */
	WORD	LFO_FREQ			/* 実際の周波数にどれだけ＋－するか */
	BYTE	LFO_ASP				/* ＬＦＯの一段階のスピード */
	BYTE	LFO_ASP2			/* ＬＦＯスピードカウンター(ＬＦＯ＿ＡＳＰ～０の範囲のカウンター)  */
	BYTE	LFO_ADELAY			/* ＬＦＯの掛かり始まるまでのディレイ */
	BYTE	LFO_ADELAY2			/* ＿ＬＦＯ＿ＡＤＥＬＡＹ～０のカウンター */
	BYTE	LFO_AMASK			/* ＬＦＯのマスク */
	WORD	LFO_AWAVE			/* ＬＦＯの波形 */
	WORD	LFO_AWAVE2			/*　のワーク */
	WORD	LFO_AUPDOWN			/* ＬＦＯの波形が上か下か */
	WORD	LFO_ACO				/* ＬＦＯを何段階変調を掛けるか */
	WORD	LFO_ACO2			/* ＬＦＯ＿ＡＣＯ～０の範囲のカウンター */
	WORD	LFO_AM				/* 変調の深さ（値が－のときは、ＬＦＯ＿ＶＯＬが－となり、下に変調がかかる） */
	BYTE	LFO_AFREQ			/* ＬＦＯを何回掛けるか ( 0=無限 ) */
	BYTE	LFO_AFREQ2			/* ＬＦＯを何回掛けるかのカウンター */
	WORD	LFO_VOL				/*実際のＶＯＬにどれだけ＋－するか */
}

/*------------------------------------------------------------------------------
	SSG チャンネルデーター
------------------------------------------------------------------------------*/
struct _SSG_VOICE {
	/* ＳＳＧの音色に関するデーター */
	BYTE SSG_VOL_AR			/* ＳＳＧのＡＲ（ＯＰＮとは逆）*/
	BYTE SSG_VOL_DR			/*ＳＳＧのＤＲ（ＯＰＮとは逆）*/
	BYTE SSG_VOL_SR			/*ＳＳＧのＳＲ（ＯＰＮとは逆）*/
	BYTE SSG_VOL_SL			/*ＳＳＧのＳＬ（ＯＰＮとは逆）*/
	BYTE SSG_VOL_RR			/*ＳＳＧのＲＲ（ＯＰＮとは逆）*/
	BYTE SSG_VOL_EG			/*ＳＳＧのエンベロープのカウンター*/
	WORD SSG_VOL_EG_FLAG		/*ＳＳＧのエンベロープのフラグ（＊２）*/
}

struct _CH_DATA_SSG {
	struct _SSG_OPN_MIDI SSG_OPN_MIDI
	struct _SSG_OPN SSG_OPN
	struct _SSG_VOICE SSG_VOICE
};


/*------------------------------------------------------------------------------
	OPN/OPL チャンネルデーター
-----------------------------------------------------------------------------*/
struct _OPNOPL_DATA {
	/* OPN/OPLに関するデーター */
	BYTE	OPN_OP0_TL		/* ＯＰ０のＴＬ */
	BYTE	OPN_OP1_TL		/* ＯＰ１のＴＬ */
	BYTE	OPN_OP2_TL		/* ＯＰ２のＴＬ */
	BYTE	OPN_OP3_TL		/*ＯＰ３のＴＬ */
	BYTE	OPN_ALG	DB		/* ＦＢ／ＡＬＧ */
	BYTE	OPN_LR_AMS_PMS		/* Ｌ／Ｒ／ＡＭＳ／ＰＭＳ */
	BYTE	OPN_OP_MASK		/*ＯＰＮのオペレーターマスク */
	/* opl */
	BYTE	OPL_KEY_ON		/* opl key on */
}
struct _CH_DATA_OPNOPL {
	struct _SSG_OPN_MIDI SSG_OPN_MIDI
	struct _SSG_OPN SSG_OPN
	struct _OPNOPLDATA OPNOPL_DATA
}


/*-----------------------------------------------------------------------------
	MIDI チャンネルデーター
----------------------------------------------------------------------------*/
struct _MIDI_DATA
	/* MIDI 固有部 */
	BYTE	_ATE_MIDI[MAX_MIDI_GATE]
	BYTE	VOLUME_MIDI		/* ボリュームデーター */
	BYTE	EX_DEVICE_ID		/* ディバイスＩＤ */
	BYTE	EX_MODEL_ID		/* モデルＩＤ */
	BYTE	EX_ADDRESS_MSB		/* アドレスＭＳＢ */
	BYTE	EX_ADDRESS		/* アドレス */
	BYTE	SMPU_EX_DATA		/* super mpu の exclusive data */
	BYTE	SMPU_EX_FLAG		/* super mpu の exclusive flag
					   0b = 0:f700 / ef00 を転送していない
						1:f700 / ef00 を転送済み
					   1b = 0:exclusive 1 byte 目
					        1:exclusive 2 byte 目
					*/
}

struct _CH_DATA_OPNOPL {
	struct _SSG_OPN_MIDI SSG_OPN_MIDI
	struct _MIDI_DATA MIDI_DATA
}
