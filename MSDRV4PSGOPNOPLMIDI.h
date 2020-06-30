/*
	msdrv4 ��`
*/


#define TIME_BASE_EQU	0x80	/* set time base ( 80h, time_base_low, time_base_high ) */
#define SB16_CMD_EQU	0x81	/* sb16 command 
					81h,00h,00h,00h = 2op mode
				 	81h,00h,00h,01h = 4op mode */
#define VC		0x82	/*�m�����n�u�n�h�b�d�@�b�`�g�m�f�d	�i�O�|�e�e�g�j */
#define PRESS_DATA EQU	0x83	/* press data ( 83h, start pointer, end pointer ) */
#define PRESS_END_EQU	0x84	/* press end */
#define VOL		0x85	/* �{�����[��
					�m�u���n�{�����[��			�i�O�|�O�e�g�j
					�m���u���n�{�����[��			�i�O�|�V�e�g�j
				*/
#define TEMPO		0x8A	/*�m�s���n�e���|�Z�b�g			�i�O�|�e�O�g�j*/
#define DRIVER_MODE	0x8B	/* driver mode data0 = ( 0 = 4 byte mode / 1 = 3 byte mode ) */
#define SSG_PROGRAM	0x8c	/* ssg program, program num, data num, data
					data number ( 0-15 = data / 128-139 = name ) */
#define SSG_PACKET	0x8d	/* ssg program, program num, data num, data len, data
					data number ( 0-15 = data / 128-139 = name ) */
#define OPN_PROGRAM	0x8e	/* opn program, program num, data num, data
					data number ( 0-47 = data / 128-139 = name ) */
#define OPN_PACKET	0x8f	/* opn program, program num, data num, data len, data
					data number ( 0-47 = data / 128-139 = name ) */

#define YCOM		0x94	/*�m�x���C���n�x�R�}���h�i�x���C���j�e�l���W�X�^������ */
#define NOOP		0x96	/* �m�F�n�m�[�I�y���[�V����		�i�O�|�e�e�g�j */
#define MLOOP		0x9B	/* �c�a�@�l�k�n�n�o�A���[�v�� */
#define MLOOPS		0x9C	/* �k�n�n�o�̍ŏ� */
#define CH_DT		0x9D	/* �`�����l���f�B�`���[�� */
#define NOP1		0x9E	/* �m�n�o�̂P�o�C�g */
#define PAN		0x9F	/* �r�d�s�Q�o�`�m�@�f�[�^ */
#define PITCH_BEND 	0xA4	/* �s�b�`�x���h */
#define HARD_LFO_FREQ	0xA6	/* �n�[�h�E�G�A�k�e�n�̂n�m�^�n�e�e�Ǝ��g�� �i�O�`�W�j */
#define START_EFFECT	0xA7	/* ���ʉ����� */
#define MAX_TRC	EQU	0xA8	/* �ő�g�p�g���b�N�i�f�t�H���g�@FM=6/��ڵFM=10/MIDI=10�j */
#define CH3_MODE EQU	0xA9	/* �b�g�R�̃��[�h */
#define CH3_OP_MASK 	0xAA	/* �b�g�R�̃I�y���[�^�[�}�X�N */
#define LFO_PITCH 	0xAB	/* ���g���ϒ��i�k�e�n�@�o�h�s�b�g�j */
#define LFO_AMP		0xAC	/* ���ʕϒ��i�k�e�n�@�`�l�o�j */
#define PORTAMENTO_VAL  0xAD	/* �|���^�����g�o�����[�Z�b�g */
#define PORTAMENTO_END  0xAE	/* �|���^�����g�I���Z�b�g */
#define PORTAMENTO_NOW  0xAF	/* ���݂̃|���^�����g�Z�b�g */


#define MUSIC_TIMING	0xC1	/* ���y�h���C�o�[���瑗���Ă���M�� */

#define EX_CLEAR_SUM	0xC2	/*  exclusive clear check sum */
#define EX_PARA		0xC3	/*  exclusive put para, data */
#define EX_PUT_SUM 	0xC4	/*  exclusive clear put sum */
#define EX_PACKET 	0xC5	/*  exclusive packet, length ( word ), data .... */


#define RHYTHM_SET	0xD0	/* ���Y�������L�[�I���@�r�s�d�o�A�t���O */
#define RHYTHM_PAN_VOL0 0xD1	/* ���Y�������̂a�c�̂o�`�m�Ƃu�n�k */
#define RHYTHM_PAN_VOL1 0xD2	/* ���Y�������̂r�c�̂o�`�m�Ƃu�n�k */
#define RHYTHM_PAN_VOL2 0xD3	/* ���Y�������̂s�n�o�̂o�`�m�Ƃu�n�k */
#define RHYTHM_PAN_VOL3 0xD4	/* ���Y�������̂g�g�̂o�`�m�Ƃu�n�k */
#define RHYTHM_PAN_VOL4 0xD5	/* ���Y�������̂s�n�l�̂o�`�m�Ƃu�n�k */
#define RHYTHM_PAN_VOL5 0xD6	/* ���Y�������̂q�h�l�̂o�`�m�Ƃu�n�k */

#define ROLBASE		0xDD	/* �q�n�k�`�m�c */
#define ROLPARA		0xDE	/* �q�n�k�`�m�c */
#define ROLDEV		0xDF	/* �q�n�k�`�m�c */
 
#define BANK_PRG 	0xE2	/* �r�s�d�o�A���F�ԍ��A�a�`�m�j�ԍ� */
#define MIDI_CH		0xE6	/* �r�s�d�o�A�l�h�c�h�o�̓`�����l���i�O�`�P�T�j */
#define TEMPO2		0xE7	/* �r�s�d�o�A�e���|�A�O���f�[�V���� */
#define AFTER_C		0xEA	/* �r�s�d�o�A�`�����l���E�v���b�V���[�i�O�`�V�e�g�j */
#define MIDI_CONTROL	0xEB	/* �l�h�c�h�R���g���[�� */
#define PROGRAM		0xEC	/* �r�s�d�o�A */
#define AFTER_K		0xED	/* �r�s�d�o�A�j�d�x�A�L�[�E�v���b�V���[�i�O�`�V�e�g�j */
#define PITCH		0xEE	/* �r�s�d�o�A */

#define ENDC		0xFE
#define ENDEND		0xFF
