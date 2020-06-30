/**
	SMFの1をサポートすれば良いとする
*/

#define MIDI_CH_MAX	16

/*-----------------------------------------------------------------------------
	header
-----------------------------------------------------------------------------*/
struct _MID_HEAD {
	mid_head_chunk_type	"MThd"		/* "MThd" */
	mid_head_length		DWOED32	0	/* header length ( 通常 6 ) */
	mid_head_format		WORD16	0	/* mid file format type */
	mid_head_ntrks		WORD16	0	/* track 数 */
	mid_head_division	WORD16	0	; ???
}


/*-----------------------------------------------------------------------------
	track
-----------------------------------------------------------------------------*/
struct _MID_TRACK {
	mid_track_chunk_type	"MTrk"		/* "MTrk" */
	mid_track_length	DWORD32		/* track length */
					/* mid_track_data		db	?	; 以後、data */
}

/*-----------------------------------------------------------------------------
	曲データー形式
-----------------------------------------------------------------------------*/
MID_TBL	BYTE ={
	BYTE	note[128]		/* key */
	/* .rcp data command tbl */
	BYTE	note_off_0		/* 0x80 */
	BYTE	note_off_1
	BYTE	note_off_2
	BYTE	note_off_3
	BYTE	note_off_4
	BYTE	note_off_5
	BYTE	note_off_6
	BYTE	note_off_7
	BYTE	note_off_8
	BYTE	note_off_9
	BYTE	note_off_a
	BYTE	note_off_b
	BYTE	note_off_c
	BYTE	note_off_d
	BYTE	note_off_e
	BYTE	note_off_f
	BYTE	note_on_0		/* 0x90 */
	BYTE	note_on_1
	BYTE	note_on_2
	BYTE	note_on_3
	BYTE	note_on_4
	BYTE	note_on_5
	BYTE	note_on_6
	BYTE	note_on_7
	BYTE	note_on_8
	BYTE	note_on_9
	BYTE	note_on_a
	BYTE	note_on_b
	BYTE	note_on_c
	BYTE	note_on_d
	BYTE	note_on_e
	BYTE	note_on_f
	BYTE	after_k_0		/* 0xa0 */
	BYTE	after_k_1
	BYTE	after_k_2
	BYTE	after_k_3
	BYTE	after_k_4
	BYTE	after_k_5
	BYTE	after_k_6
	BYTE	after_k_7
	BYTE	after_k_8
	BYTE	after_k_9
	BYTE	after_k_a
	BYTE	after_k_b
	BYTE	after_k_c
	BYTE	after_k_d
	BYTE	after_k_e
	BYTE	after_k_f
	BYTE	control_0		/* 0b0h */
	BYTE	control_1
	BYTE	control_2
	BYTE	control_3
	BYTE	control_4
	BYTE	control_5
	BYTE	control_6
	BYTE	control_7
	BYTE	control_8
	BYTE	control_9
	BYTE	control_a
	BYTE	control_b
	BYTE	control_c
	BYTE	control_d
	BYTE	control_e
	BYTE	control_f
	BYTE	program_0		/* 0c0h */
	BYTE	program_1
	BYTE	program_2
	BYTE	program_3
	BYTE	program_4
	BYTE	program_5
	BYTE	program_6
	BYTE	program_7
	BYTE	program_8
	BYTE	program_9
	BYTE	program_a
	BYTE	program_b
	BYTE	program_c
	BYTE	program_d
	BYTE	program_e
	BYTE	program_f
	BYTE	after_c_0		/* 0xd0 */
	BYTE	after_c_1
	BYTE	after_c_2
	BYTE	after_c_3
	BYTE	after_c_4
	BYTE	after_c_5
	BYTE	after_c_6
	BYTE	after_c_7
	BYTE	after_c_8
	BYTE	after_c_9
	BYTE	after_c_a
	BYTE	after_c_b
	BYTE	after_c_c
	BYTE	after_c_d
	BYTE	after_c_e
	BYTE	after_c_f
	BYTE	pitch_0			/* 0xe0 */
	BYTE	pitch_1
	BYTE	pitch_2
	BYTE	pitch_3
	BYTE	pitch_4
	BYTE	pitch_5
	BYTE	pitch_6
	BYTE	pitch_7
	BYTE	pitch_8
	BYTE	pitch_9
	BYTE	pitch_a
	BYTE	pitch_b
	BYTE	pitch_c
	BYTE	pitch_d
	BYTE	pitch_e
	BYTE	pitch_f
	BYTE	exclusive_f0h		/* 0xf0 */
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	exclusive_f7h
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	next
	BYTE	meta_event
}
