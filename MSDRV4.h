/*
	msdrv4�̌ŗL����

=============================================================================
=============================================================================
	������`
=============================================================================
=============================================================================
*/

/*------------------------------------------------------------------------------
	driver config
------------------------------------------------------------------------------*/
#define CHANNEL			36		/* �`�����l���� */
#define MIDI_EFFECT_MAX 	8		/* midi ���ʉ��̐� */
#define _MLOOP_DATA_MAX		8		/* loop �̃l�X�g�� */
#define MAX_SUBEOURINE		16		/* �T�u���[�`���̍ő�o�^�� */
#define MAX_POINTER		5		/* ���܂Ń|�C���^������ */
#define MAX_MIDI_GATE		128		/* �P�Q�W�����̂f�`�s�d */


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
#define CNT8MHZ_H		0x00724	/*  8Mhz counter �ݒ�l�� high ( 125*19968*48 ) */
#define CNT8MHZ_L		0x2000	/*  8Mhz counter �ݒ�l�� low  ( 125*19968*48 ) */
#define CNT8MHZ_16MS_H		0x00000	/*  8Mhz counter �ݒ�l�� high ( 1996.8*16.7) */
#define CNT8MHZ_16MS_L		0x08242	/*  8Mhz counter �ݒ�l�� low  ( 1996.8*16.7) */
#define CNT10MHZ_H		0x08ca	/* 10Mhz counter �ݒ�l�� high ( 125*24576*48 ) */
#define CNT10MHZ_L		0x0000	/* 10Mhz counter �ݒ�l�� low  ( 125*24576*48 ) */
#define cnt10mhz_16ms_h	equ	00000h	/* 10Mhz counter �ݒ�l�� high ( 2457.6*16.7) */
#define cnt10mhz_16ms_l	equ	0a051h	/* 10Mhz counter �ݒ�l�� low  ( 2457.6*16.7) */


/*------------------------------------------------------------------------------
	rs232c
------------------------------------------------------------------------------*/
#define RS232C_8MHZ	4	/* �W�l�����̂q�r�Q�R�Q�b�̃J�E���^�[�ݒ�l */
#define RS232C_10MHZ	5	/* �P�O�l�����̂q�r�Q�R�Q�b�̃J�E���^�[�ݒ�l */



/*------------------------------------------------------------------------------
	 ssg/opn/midi
------------------------------------------------------------------------------*/
struct _SSG_OPN_MIDI {
	WORD	CH_POINTER					/* ���� ms data �ւ� pointer */
	BYTE	DRIVER_MODE					/* driver mode
									0 = 4 byte mode keyNO/step/gate/vel
									1 = 3 byte mode keyNO/step/gate
								*/
	WORD _MLOOP_SP						/* loop �� stack pointer */
	MLLOP_DATA_MAX_MLOOP_DATA  _MLOOP_DATA_MAX*MAX_POINTER	/* loop �̖߂�ꏊ�� pointer */

	WORD	PITCH_BEND					/* �s�b�`�x���h */
	WORD	PORTAMENTO					/* �|���^�����g�̒l */
	WORD	PORTAMENTO_VAL					/* �|���^�����g�̕� */
	WORD	PORTAMENTO_END					/* �|���^�����g�̍ŏI�n(0=�|���^���Ȃ�)  */

	BYTE	VOL_DATA					/* volume */
	BYTE	STEP						/* �c�� step */
	BYTE	MIDI_CH						/* �o�̓`�����l�� */

	BYTE	PRESS_FLAG					/* press data ���g�p���Ă���Œ��� */
	BYTE	PRESS_DATA					/* press end ���������ޑO�̃f�[�^�[ */
	BYTE	PRESS_POINTER					/* press data �̏I�����ɖ߂�ꏊ */
}

/*------------------------------------------------------------------------------
	 �r�r�f�@���@�n�o�m
------------------------------------------------------------------------------*/
struct _SSG_OPN {
	struct SSG_OPN _SSG_OPN_DATA
	struct SSG_OPN_SOFT_LFO _SSG_OPN_DATA_SOFT_LFO

}

struct _SSG_OPN_DATA {
	BYTE	GATE				/* �Q�[�g */
	WORD	CH_DT				/* �`�����l���f�B�`���[�� */
	WORD	FREQ				/* ���g��
	BYTE	VOL_DATA2			/* �u�n�k�̃J�E���^�[ */
	BYTE	KEY				/* ���K */
	BYTE	OCT				/* �I�N�^�[�u */
}

struct _SSG_OPN_SOFT_LFO {
	/* �r�r�f�@���@�n�o�m�@�̃\�t�g�E�F�A�[�k�e�n�f�[�^�[ */
	BYTE	LFO_PSP				/*�k�e�n�̈�i�K�̃X�s�[�h */
	BYTE	LFO_PSP2			/* �k�e�n�X�s�[�h�J�E���^�[  (�k�e�n�Q�o�r�o�`�O�͈̔͂̃J�E���^�[)  */
	BYTE	LFO_PDELAY			/* �k�e�n�̊|����n�܂�܂ł̃f�B���C */
	BYTE	LFO_PDELAY2			/* �k�e�n�Q�o�c�d�k�`�x�`�O�̃J�E���^�[ */
	WORD	LFO_PWAVE			/* �k�e�n�̔g�`�i�g�`�ԍ����Q������j */
	WORD	LFO_PWAVE2			/* ���[�N */
	WORD	LFO_PUPDOWN			/* �k�e�n�̔g�`���ォ���� */
	WORD	LFO_PCO				/* �k�e�n�����i�K�ϒ����|���邩 */
	WORD	LFO_PCO2			/* �k�e�n�Q�o�b�n�`�O�͈̔͂̃J�E���^�[ */
	WORD	LFO_PM				/* �ϒ��̐[���i�l���|�̂Ƃ��́A�k�e�n�Q�q�q�d�p���|�ƂȂ�A���ɕϒ���������j */
	BYTE	LFO_PFREQ			/* �k�e�n������|���邩 ( 0=���� ) */
	BYTE	LFO_PFREQ2			/* �k�e�n������|���邩�̃J�E���^�[ */
	WORD	LFO_FREQ			/* ���ۂ̎��g���ɂǂꂾ���{�|���邩 */
	BYTE	LFO_ASP				/* �k�e�n�̈�i�K�̃X�s�[�h */
	BYTE	LFO_ASP2			/* �k�e�n�X�s�[�h�J�E���^�[(�k�e�n�Q�`�r�o�`�O�͈̔͂̃J�E���^�[)  */
	BYTE	LFO_ADELAY			/* �k�e�n�̊|����n�܂�܂ł̃f�B���C */
	BYTE	LFO_ADELAY2			/* �Q�k�e�n�Q�`�c�d�k�`�x�`�O�̃J�E���^�[ */
	BYTE	LFO_AMASK			/* �k�e�n�̃}�X�N */
	WORD	LFO_AWAVE			/* �k�e�n�̔g�` */
	WORD	LFO_AWAVE2			/*�@�̃��[�N */
	WORD	LFO_AUPDOWN			/* �k�e�n�̔g�`���ォ���� */
	WORD	LFO_ACO				/* �k�e�n�����i�K�ϒ����|���邩 */
	WORD	LFO_ACO2			/* �k�e�n�Q�`�b�n�`�O�͈̔͂̃J�E���^�[ */
	WORD	LFO_AM				/* �ϒ��̐[���i�l���|�̂Ƃ��́A�k�e�n�Q�u�n�k���|�ƂȂ�A���ɕϒ���������j */
	BYTE	LFO_AFREQ			/* �k�e�n������|���邩 ( 0=���� ) */
	BYTE	LFO_AFREQ2			/* �k�e�n������|���邩�̃J�E���^�[ */
	WORD	LFO_VOL				/*���ۂ̂u�n�k�ɂǂꂾ���{�|���邩 */
}

/*------------------------------------------------------------------------------
	SSG �`�����l���f�[�^�[
------------------------------------------------------------------------------*/
struct _SSG_VOICE {
	/* �r�r�f�̉��F�Ɋւ���f�[�^�[ */
	BYTE SSG_VOL_AR			/* �r�r�f�̂`�q�i�n�o�m�Ƃ͋t�j*/
	BYTE SSG_VOL_DR			/*�r�r�f�̂c�q�i�n�o�m�Ƃ͋t�j*/
	BYTE SSG_VOL_SR			/*�r�r�f�̂r�q�i�n�o�m�Ƃ͋t�j*/
	BYTE SSG_VOL_SL			/*�r�r�f�̂r�k�i�n�o�m�Ƃ͋t�j*/
	BYTE SSG_VOL_RR			/*�r�r�f�̂q�q�i�n�o�m�Ƃ͋t�j*/
	BYTE SSG_VOL_EG			/*�r�r�f�̃G���x���[�v�̃J�E���^�[*/
	WORD SSG_VOL_EG_FLAG		/*�r�r�f�̃G���x���[�v�̃t���O�i���Q�j*/
}

struct _CH_DATA_SSG {
	struct _SSG_OPN_MIDI SSG_OPN_MIDI
	struct _SSG_OPN SSG_OPN
	struct _SSG_VOICE SSG_VOICE
};


/*------------------------------------------------------------------------------
	OPN/OPL �`�����l���f�[�^�[
-----------------------------------------------------------------------------*/
struct _OPNOPL_DATA {
	/* OPN/OPL�Ɋւ���f�[�^�[ */
	BYTE	OPN_OP0_TL		/* �n�o�O�̂s�k */
	BYTE	OPN_OP1_TL		/* �n�o�P�̂s�k */
	BYTE	OPN_OP2_TL		/* �n�o�Q�̂s�k */
	BYTE	OPN_OP3_TL		/*�n�o�R�̂s�k */
	BYTE	OPN_ALG	DB		/* �e�a�^�`�k�f */
	BYTE	OPN_LR_AMS_PMS		/* �k�^�q�^�`�l�r�^�o�l�r */
	BYTE	OPN_OP_MASK		/*�n�o�m�̃I�y���[�^�[�}�X�N */
	/* opl */
	BYTE	OPL_KEY_ON		/* opl key on */
}
struct _CH_DATA_OPNOPL {
	struct _SSG_OPN_MIDI SSG_OPN_MIDI
	struct _SSG_OPN SSG_OPN
	struct _OPNOPLDATA OPNOPL_DATA
}


/*-----------------------------------------------------------------------------
	MIDI �`�����l���f�[�^�[
----------------------------------------------------------------------------*/
struct _MIDI_DATA
	/* MIDI �ŗL�� */
	BYTE	_ATE_MIDI[MAX_MIDI_GATE]
	BYTE	VOLUME_MIDI		/* �{�����[���f�[�^�[ */
	BYTE	EX_DEVICE_ID		/* �f�B�o�C�X�h�c */
	BYTE	EX_MODEL_ID		/* ���f���h�c */
	BYTE	EX_ADDRESS_MSB		/* �A�h���X�l�r�a */
	BYTE	EX_ADDRESS		/* �A�h���X */
	BYTE	SMPU_EX_DATA		/* super mpu �� exclusive data */
	BYTE	SMPU_EX_FLAG		/* super mpu �� exclusive flag
					   0b = 0:f700 / ef00 ��]�����Ă��Ȃ�
						1:f700 / ef00 ��]���ς�
					   1b = 0:exclusive 1 byte ��
					        1:exclusive 2 byte ��
					*/
}

struct _CH_DATA_OPNOPL {
	struct _SSG_OPN_MIDI SSG_OPN_MIDI
	struct _MIDI_DATA MIDI_DATA
}
