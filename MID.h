/**
	アセンブラのEQUを持ってきただけなので、cでは未実装
*/


midi_ch_max	equ	16
;------------------------------------------------------------------------------
;	header
;------------------------------------------------------------------------------
mid_head	struc
_mid_head_chunk_type	db 4h dup(?)	; "MThd"
_mid_head_length	dd	?	; header length ( 通常 6 )
_mid_head_format	dw	?	; mid file format type
_mid_head_ntrks		dw	?	; track 数
_mid_head_division	dw	?	; ???
mid_head	ends


;------------------------------------------------------------------------------
;	track
;------------------------------------------------------------------------------
mid_track	struc
_mid_track_chunk_type	db 4h dup(?)	; "MTrk"
_mid_track_length	dd	?	; track length
_mid_track_data		db	?	; 以後、data
mid_track	ends



;------------------------------------------------------------------------------
;	曲データー形式
;------------------------------------------------------------------------------
mid_tbl		macro
;
;	.rcp data command tbl macro
;
				dw	#note_off_0		;080h
				dw	#note_off_1
				dw	#note_off_2
				dw	#note_off_3
				dw	#note_off_4
				dw	#note_off_5
				dw	#note_off_6
				dw	#note_off_7
				dw	#note_off_8
				dw	#note_off_9
				dw	#note_off_a
				dw	#note_off_b
				dw	#note_off_c
				dw	#note_off_d
				dw	#note_off_e
				dw	#note_off_f
				dw	#note_on_0		;090h
				dw	#note_on_1
				dw	#note_on_2
				dw	#note_on_3
				dw	#note_on_4
				dw	#note_on_5
				dw	#note_on_6
				dw	#note_on_7
				dw	#note_on_8
				dw	#note_on_9
				dw	#note_on_a
				dw	#note_on_b
				dw	#note_on_c
				dw	#note_on_d
				dw	#note_on_e
				dw	#note_on_f
				dw	#after_k_0		;0a0h
				dw	#after_k_1
				dw	#after_k_2
				dw	#after_k_3
				dw	#after_k_4
				dw	#after_k_5
				dw	#after_k_6
				dw	#after_k_7
				dw	#after_k_8
				dw	#after_k_9
				dw	#after_k_a
				dw	#after_k_b
				dw	#after_k_c
				dw	#after_k_d
				dw	#after_k_e
				dw	#after_k_f
				dw	#control_0		;0b0h
				dw	#control_1
				dw	#control_2
				dw	#control_3
				dw	#control_4
				dw	#control_5
				dw	#control_6
				dw	#control_7
				dw	#control_8
				dw	#control_9
				dw	#control_a
				dw	#control_b
				dw	#control_c
				dw	#control_d
				dw	#control_e
				dw	#control_f
				dw	#program_0		;0c0h
				dw	#program_1
				dw	#program_2
				dw	#program_3
				dw	#program_4
				dw	#program_5
				dw	#program_6
				dw	#program_7
				dw	#program_8
				dw	#program_9
				dw	#program_a
				dw	#program_b
				dw	#program_c
				dw	#program_d
				dw	#program_e
				dw	#program_f
				dw	#after_c_0		;0d0h
				dw	#after_c_1
				dw	#after_c_2
				dw	#after_c_3
				dw	#after_c_4
				dw	#after_c_5
				dw	#after_c_6
				dw	#after_c_7
				dw	#after_c_8
				dw	#after_c_9
				dw	#after_c_a
				dw	#after_c_b
				dw	#after_c_c
				dw	#after_c_d
				dw	#after_c_e
				dw	#after_c_f
				dw	#pitch_0		;0e0h
				dw	#pitch_1
				dw	#pitch_2
				dw	#pitch_3
				dw	#pitch_4
				dw	#pitch_5
				dw	#pitch_6
				dw	#pitch_7
				dw	#pitch_8
				dw	#pitch_9
				dw	#pitch_a
				dw	#pitch_b
				dw	#pitch_c
				dw	#pitch_d
				dw	#pitch_e
				dw	#pitch_f
				dw	#exclusive_f0h		;0f0h
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#exclusive_f7h
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#next
				dw	#meta_event
	endm
