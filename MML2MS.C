/**
mml2mid.c

Copyright 2020 kenjishinmyou

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
/* ==========================================================================
=============================================================================
		include
=============================================================================
========================================================================== */
#define TYPEMODE	1			/* 0:WINDOWS.Hを使わないバージョン
								   1:WINDOWS.Hを使わうバージョン
								   現状、時間あわせの問題が出てきている(cコンパイラのバージョンによって違う
								   VC6++のansi-c縛りなので、読み違えてください */
#define MAX_BUFFER	16			/* _MAX_PATHに掛ける係数 */
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbstring.h>
#include <setjmp.h>

#if TYPEMODE==0
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;
#else
#include <windows.h>
#endif

#include "\obj2\make.h"
#include "\obj2\msdrv4.h"
#include "\obj2\msdrv4ssgoplmml.h"



/* ==========================================================================
=============================================================================
		define
=============================================================================
========================================================================== */
/*
	文字コード

		現在の改行コードはlinuxはlf/macはcr/winはcr+lf/os-2はcr+lf
*/
#define CR		0x0d
#define LF		0x0a
#define	EOF		0x1a
#define TAB		09h
#define MES_MARJIN	23

/*
	音色データサイズ
*/
#define SSG_DATA_SIZE	16
#define SSG_NAME_SIZE	12
#define OPN_DATA_SIZE	48
#define OPN_NAME_SIZE	12

/*
	pre processor command
*/
#define			PRE_INCLUDE		"INCLUDE\0"
#define			PRE_DEFINE		"DEFINE\0"
#define			PRE_ENDIF		"ENDIF\0"
#define			PRE_IF			"IF\0"
/* 多分、和音はプリプロセッサでパース可能なので、#cdeとかにする予定  3音以上ならば可能 */
#define			PRE_ERR_HARMONY		"[a-gA-G]\([a-gA-G]\{0,1})\\0"				/* future 正規表現の和音 (error) */
#define			PRE_HARMONY		"[a-gA-G]\([a-gA-G]\{2,})\\0"				/* future 正規表現の和音 */


/*
	mml command
*/
#define mml_dot_tr			".tr"
#define	mml_set_note			"n"
#define	mml_set_onpu_c			"c"
#define mml_set_onpu_d			"d"
#define mml_set_onpu_e			"e"
#define	mml_set_onpu_f			"f"
#define	mml_set_onpu_g			"g"
#define	mml_set_onpu_a			"a"
#define	mml_set_onpu_b			"b"
#define	mml_set_rest			"r"
#define	mml_set_off0			"off"
#define	mml_set_on0			"on"
#define	mml_set_off1			"@off"
#define	mml_set_on1			"@on"
#define	mml_set_ts2			"@ts-"
#define	mml_set_ts1			"@ts+"
#define	mml_set_ts0			"_ts"
#define	mml_set_opnch2mode		"@om"
#define	mml_set_opnch2mask0		"@m"
#define	mml_set_opnch2mask1		"m"
#define	mml_set_progssg			"@ssg"
#define	mml_set_progopn			"@opn"
#define	mml_set_progopl			"@opl"
#define	mml_set_sb4			"sb4"
#define	mml_set_sb2			"sb2"
#define	mml_set_sbc			"sbc"
#define	mml_set_oct			"o"
#define	mml_set_oct_up			">"
#define	mml_set_oct_down		"<"
#define	mml_set_len			"l"
#define	mml_set_vol_bass2		"@dvb-"
#define	mml_set_vol_bass1		"@dvb+"
#define	mml_set_vol_bass0		"@dvb"
#define	mml_set_vol_snare2		"@dvs-"
#define	mml_set_vol_snare1		"@dvs+"
#define	mml_set_vol_snare0		"@dvs"
#define	mml_set_vol_top2		"@dvt-"
#define	mml_set_vol_top1		"@dvt+"
#define	mml_set_vol_top0		"@dvt"
#define	mml_set_vol_high2		"@dvh-"
#define	mml_set_vol_high1		"@dvh+"
#define	mml_set_vol_high0		"@dvh"
#define	mml_set_vol_tom2		"@dvm-"
#define	mml_set_vol_tom1		"@dvm+"
#define	mml_set_vol_tom0		"@dvm"
#define	mml_set_vol_rim2		"@dvr-"
#define	mml_set_vol_rim1		"@dvr+"
#define	mml_set_vol_rim0		"@dvr"
#define	mml_set_drum_on			"@don"
#define	mml_set_drum_off		"@doff"
#define	mml_set_vol3			"@v-"
#define	mml_set_vol2			"@v+"
#define	mml_set_vol1			"@v"
#define	mml_set_vol0			"v"
#define	mml_set_tempo			"t"
#define	mml_set_midi_ch			"@c"
#define	mml_set_q0			"q"
#define	mml_set_q1			"@q"
#define	mml_set_pt3			"@pt-"
#define	mml_set_pt2			"@pt+"
#define	mml_set_pt0			"_pt"
#define	mml_set_pore			"pore"
#define	mml_set_porn			"porn"
#define	mml_set_porv			"porv"
#define	mml_set_por			"@p"
#define	mml_set_pan3			"pan-"
#define	mml_set_pan2			"pan+"
#define	mml_set_pan0			"p"
#define	mml_set_bendr			"@bendr"
#define	mml_set_mc			"@mc"
#define	mml_set_dt			"@dt"
#define	mml_set_lfo			"@lf"
#define	mml_set_lfo_pitch		"@lp"
#define	mml_set_lfo_amp			"@la"
#define	mml_set_music_timing		"@mt"
#define	mml_set_vc			"@"
#define	mml_set_harmony			"&"
#define	mml_set_mloops			"["
#define	mml_set_mloop			"]"
#define	mml_set_portamento		"_"
#define	mml_set_ycom			"y"


#if 0


#define N		4096					/* 環状バッファの大きさ */
#define F		18						/* 最長一致長 */
#define NIL		N						/* 木の末端 */


/* ==========================================================================
=============================================================================
		static
=============================================================================
========================================================================== */
jmp_buf main_sj;						/* error 終了の時に、mainprogへ戻るための物 */
UCHAR *lpMem;							/* エラー時に free 出来るようにグローバル化した */

FILE *infile,*outfile,*maskfile;		/* 入力ファイル, 出力ファイル */
UCHAR text[N+F-1];						/* テキスト用バッファ */
int dad[N+1], lson[N+1], rson[N+257];	/* 木 */
int matchpos, matchlen;					/* 最長一致位置, 一致長 */



/* 音長のデフォルトテーブル。L系の命令をsetp/gateに変換する為のテーブル */
BYTE len_tbl[] = { -1,192,96,64,48,-1,32,-1,24,-1,-1,-1,16,-1,-1,-1,12,-1,
					-1,-1,-1,-1,-1,-1,18,-1,-1,-1,-1,-1,-1,-1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }

/* chと、各音源のへの変換テーブル。36トラックだが、念のため256トラックに対応させておく。 */
BYTE default_ch[] = {
			80,		/* opn ch0 */
			81,		/* opn ch1 */
			82,		/* opn ch2 */
			83,		/* opn ch3 */
			84,		/* opn ch4 */
			85,		/* opn ch5 */
			240,	/* opn ch2 ( @opnmode1 用 ) */
			241,	/* opn ch2 ( @opnmode1 用 ) */
			242,	/* opn ch2 ( @opnmode1 用 ) */
			64,		/* ssg ch0 */
			65,		/* ssg ch1 */
			66,		/* ssg ch2 */
			254,	/* opna rhythm */
			255,	/* not out */
			255,	/* not out */
			255,	/* not out */
			0,		/* midi ch0 */
			1,		/* midi ch1 */
			2,		/* midi ch2 */
			3,		/* midi ch3 */
			4,		/* midi ch4 */
			5,		/* midi ch5 */
			6,		/* midi ch6 */
			7,		/* midi ch7 */
			8,		/* midi ch8 */
			9,		/* midi ch9 */
			10,		/* midi ch10 */
			11,		/* midi ch11 */
			12,		/* midi ch12 */
			13,		/* midi ch13 */
			14,		/* midi ch14 */
			15,		/* midi ch15 */
			255,	/* not out */
			255,	/* not out */
			255,	/* not out */
			255,	/* not out */
			/* 256-36 = 220は 255 で notbut */
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255 }

/* ボリュームののデフォルトテーブル。 */
BYTE vol_tbl[] = { db 0, 70, 75, 80, 84, 88, 92, 96,100,104,108,112,115,118,121,124,127 }



















DWORD nPrs;























//
//	type2 用
//
#define MAX_WRITEBUFF 1024
BYTE WriteBuff[MAX_WRITEBUFF];
BYTE *lpWrite;
DWORD nShift;
DWORD nAdd[13+1+12] ={	15,
						15,
						13,
						13,
						11,
						 9,
						 9,
						 7,
						 7,
						 7,
						 4,
						 4,
						 3,
						 1,
						 3,
						 4,
						 7,
						 7,
						 7,
						 7,
						 9,
						11,
						11,
						13,
						15,
						15 };
DWORD nVal[13+1+12] = {	0x0000,
						0x2000,
						0x1000,
						0x1800,
						0x0200,
						0x0100,
						0x0180,
						0x0020,
						0x0010,
						0x0030,
						0x0008,
						0x000e,
						0x0004,
						0x0001,
						0x0002,
						0x0006,
						0x0070,
						0x0050,
						0x0060,
						0x0040,
						0x0080,
						0x0600,
						0x0400,
						0x0800,
						0x6000,
						0x4000 };
DWORD mask0[] = {	0xffffffff,
					0xfffffffe,
					0xfffffffc,
					0xfffffff8,
					0xfffffff0,
					0xffffffe0,
					0xffffffc0,
					0xffffff80,
					0xffffff00,
					0xfffffe00,
					0xfffffc00,
					0xfffff800,
					0xfffff000,
					0xffffe000,
					0xffffc000,
					0xffff8000, };
DWORD mask1[] = {	0x00000000,
					0x00000001,
					0x00000003,
					0x00000007,
					0x0000000f,
					0x0000001f,
					0x0000003f,
					0x0000007f,
					0x000000ff,
					0x000001ff,
					0x000003ff,
					0x000007ff,
					0x00000fff,
					0x00001fff,
					0x00003fff,
					0x00007fff, };
/* ==========================================================================
=============================================================================
		program
=============================================================================
========================================================================== */
/*---------------------------------------------------------------------------
	error exit
---------------------------------------------------------------------------*/
void error(char *message)  /* メッセージを表示し終了 */
{
	fprintf(stderr, "\n%s\n", message);
	longjmp( main_sj,EXIT_FAILURE );
	// exit(EXIT_FAILURE);
}
/*---------------------------------------------------------------------------
	usage
---------------------------------------------------------------------------*/
void usage( void )  /* メッセージを表示し終了 */
{

	fprintf(stderr, "MML2MID.EXE : .mml to .mid/.ms\n");
	fprintf(stderr, "Copyright(c)KENJI 2020.6- Release 0.09 All rights reserved.\n");
	fprintf(stderr, " Programed by KENJI.\n");
	fprintf(stderr, "usage : PT1S [-options...] filename [maskfilename] [savename] [-options...]\n");
	fprintf(stderr, "	filename	.bmp file name\n");
	fprintf(stderr, "	savename	パス名が違うときに使用\n");
	fprintf(stderr, "makeモードの説明は >pt1s.exe /f /?[ret]\n");
	longjmp( main_sj,EXIT_FAILURE );
	//exit(EXIT_FAILURE);
}
/*---------------------------------------------------------------------------
	init tree
---------------------------------------------------------------------------*/
void init_tree(void)  /* 木の初期化 */
{
	int i;

	for (i = N + 1; i <= N + 256; i++) {
		 rson[i] = NIL;
	}
	for (i = 0; i < N; i++) {
		dad[i] = NIL;
	}
}

/*---------------------------------------------------------------------------
	insert node
---------------------------------------------------------------------------*/
void insert_node(int r)  /* 節 r を木に挿入 */
{
	int i, p, cmp;
	UCHAR *key;

	cmp = 1;
	key = &text[r];
	p = N + 1 + key[0];
	rson[r] = lson[r] = NIL;
	matchlen = 0;

	for ( ; ; ) {
		if (cmp >= 0) {
			if (rson[p] != NIL) {
				p = rson[p];
			} else {
				rson[p] = r;
				dad[r] = p;
				return;
			}
		} else {
			if (lson[p] != NIL) {
				p = lson[p];
			} else {
				lson[p] = r;
				dad[r] = p;
				return;
			}
		}
		for (i = 1; i < F; i++)
			if ((cmp = key[i] - text[p + i]) != 0)  {
				break;
			}
		if (i > matchlen) {
			matchpos = p;
			if ((matchlen = i) >= F) {
				break;
			}
		}
	}

	dad[r] = dad[p];
	lson[r] = lson[p];
	rson[r] = rson[p];
	dad[lson[p]] = r;
	dad[rson[p]] = r;

	if (rson[dad[p]] == p) {
		 rson[dad[p]] = r;
	} else {
		lson[dad[p]] = r;
	}
	dad[p] = NIL;  /* p を外す */
}
/*---------------------------------------------------------------------------
	delete node
---------------------------------------------------------------------------*/
void delete_node(int p)  /* 節 p を木から消す */
{
	int  q;

	if (dad[p] == NIL) return;  /* 見つからない */

	if (rson[p] == NIL) {
		q = lson[p];
	} else if (lson[p] == NIL) {
		q = rson[p];
	} else {
		q = lson[p];
		if (rson[q] != NIL) {
			do {
				q = rson[q];
			} while( rson[q] != NIL );
			rson[dad[q]] = lson[q];
			dad[lson[q]] = dad[q];
			lson[q] = lson[p];
			dad[lson[p]] = q;
		}
		rson[q] = rson[p];
		dad[rson[p]] = q;
	}
	dad[q] = dad[p];
	if( rson[dad[p]] == p ) {
		rson[dad[p]] = q;
	} else {
		lson[dad[p]] = q;
	}
	dad[p] = NIL;
}
/*---------------------------------------------------------------------------
	encode main
---------------------------------------------------------------------------*/
void encode0000( int xTop, int yTop, int cx, int cy, int xs, int ys, int fbmp32 )
{
	int i0,i1;							/* loop counter for init dictionary */
	int i, c, len, r, s, lastmatchlen, codeptr, x,y,cRead,xm;
	UCHAR code[17], mask, *lpTmp;
	unsigned long int incount = 0 ;			/* 読み込み byte */
	unsigned long int printcount = 0;		/* 1024 byte 毎に表示するため */
	unsigned long int cr;
	int cbit32;								// 32bit bmp の時のための処理


	//
	//	alloc
	//
	lpMem = malloc( cx*cy*3 );
	if( lpMem == NULL ) {
		error("err02:mempry が alloc 出来ません");
	}


	//
	//	read bmp file
	//
	cRead = cx*cy*3;
	lpTmp = lpMem+cx*(cy-1)*3;
	xm = ((xs*3+3)&0xfffffffc);
	cbit32=0;
	for( y=ys-1;y>=0;y-- ) {
		i = 0;
		for( x=0;x<xm;x++ ) {
			if( x>=xTop*3 && x<(xTop+cx)*3 && y>=yTop && y<yTop+cy ) {
				c = getc(infile);
				if( c == EOF ) {
					error("err02:bmp が読み込めません");
				}
				*lpTmp++ = (UCHAR)c;
				i = 1;
			} else {
				c = getc(infile);
				if( c == EOF ) {
					error("err02:bmp が読み込めません");
				}
			}
			// 32bit の為の処理
			if( fbmp32==1 ) {
				cbit32++;
				if(cbit32==3) {
					c = getc(infile);			// 空読み
					cbit32=0;
				}
			}
		}
		if( i == 1 ) {
			lpTmp-=(cx*3)*2;
		}
	}
	lpTmp = lpMem;


	//
	//	木を初期化
	//
	init_tree();

	code[0] = 0;
	codeptr = mask = 1;
	s = 0;
	r = N - F;

	//
	//	init dictionary ( 4096 - 18 bytes )
	//
	i = 0;
									/* 0 ～ 255 までを 13 byte づつ埋める */
	for (i0 = 0; i0 < 256; i0++) {
		for (i1 = 0; i1 < 13; i1++) {
			text[i++] = i0;
		}
	}

									/* 0 ～ 255 までで埋める */
	for (i1 = 0; i1 < 256; i1++) {
			text[i++] = i1;
	}

									/* 255 ～ 0 までで埋める */
	for (i1 = 255; i1 >= 0; i1--) {
			text[i++] = i1;
	}

									/* 0 を 128 byte 埋める */
	for (i1 = 0; i1 < 128; i1++) {
			text[i++] = 0;
	}

									/* 0x20 を 128 - 18 byte 埋める */
	for (i1 = 0; i1 < 128 - F; i1++) {
			text[i++] = 0x20;
	}

									/* 0 を 18 byte 埋める */
	for (i1 = 0; i1 < F; i1++) {
			text[i++] = 0;
	}


	//
	//	read first 18 bytes to dictionary
	//
	for (len = 0; len < F ; len++) {
		if( cRead == 0 ) {
			break;
		}
		text[r + len] = *lpTmp++;
		cRead--;
	}

	incount = len;
	if (incount == 0) {
		return;
	}
	for (i = 1; i <= F; i++) {
		 insert_node(r - i);
	}
	insert_node(r);



	do {
		if (matchlen > len) matchlen = len;
		if (matchlen < 3 ) {
			matchlen = 1;  code[0] |= mask;  code[codeptr++] = text[r];
		} else {
			code[codeptr++] = (UCHAR) matchpos;
			code[codeptr++] = (UCHAR)
				(((matchpos >> 4) & 0xf0) | (matchlen - 3 ));
		}
		if ((mask <<= 1) == 0) {
			for (i = 0; i < codeptr; i++) putc(code[i], outfile);
			nPrs += codeptr;
			code[0] = 0;  codeptr = mask = 1;
		}
		lastmatchlen = matchlen;
		for (i = 0; i < lastmatchlen; i++) {


			if( cRead == 0 ) {
				break;
			}
			delete_node(s);
			text[s] = *lpTmp;
			if (s < F - 1) {
				text[s + N] = *lpTmp;
			}
			lpTmp++;
			cRead--;
			s = (s + 1) & (N - 1);
			r = (r + 1) & (N - 1);
			insert_node(r);
		}




		//
		//	1024 byte 毎に表示
		//
		if ((incount += i) > printcount) {
			printf("%12lu\r", incount);  printcount += 1024;
		}




		while (i++ < lastmatchlen) {
			delete_node(s);
			s = (s + 1) & (N - 1);  r = (r + 1) & (N - 1);
			if (--len) insert_node(r);
		}

	} while (len > 0);




	if (codeptr > 1) {
		for (i = 0; i < codeptr; i++) putc(code[i], outfile);
		nPrs += codeptr;
	}


	//
	//	print resalt
	//
	printf("%lu byte(s) / ", nPrs);
	printf("%lu byte(s) = ", incount);
	if (incount != 0) {  /* 圧縮比を求めて報告 */
		cr = (1000 * nPrs + incount / 2) / incount;
		printf("%lu.%03lu\n", cr / 10, cr % 10);
	}

	free( lpMem );
	lpMem=NULL;
}
/*---------------------------------------------------------------------------
	encode main
---------------------------------------------------------------------------*/
void encode0001( int xTop, int yTop, int cx, int cy, int xs, int ys, int nPlane, int fbmp32 )
{
//
//	1plane mask data 用
//	rgb の 3 byte data のうち任意の plane のみを圧縮する
//
	int i0,i1;							/* loop counter for init dictionary */
	int i, c, len, r, s, lastmatchlen, codeptr, x,y,cRead,xm,mode;
	UCHAR code[17], mask, *lpTmp;
	unsigned long int incount = 0 ;			/* 読み込み byte */
	unsigned long int printcount = 0;		/* 1024 byte 毎に表示するため */
	unsigned long int cr;
	int cbit32;								// 32bit bmp の時のための処理

	mode = 0;
	if( nPlane == 3 ) {
		mode = 1;
		nPlane = 0;
	}


	//
	//	alloc
	//
	lpMem = malloc( cx*cy*3 );
	if( lpMem == NULL ) {
		error("err02:mempry が alloc 出来ません");
	}


	//
	//	read bmp file
	//
	cRead = cx*cy*3;
	lpTmp = lpMem+cx*(cy-1)*3;
	xm = ((xs*3+3)&0xfffffffc);
	cbit32=0;
	for( y=ys-1;y>=0;y-- ) {
		i = 0;
		for( x=0;x<xm;x++ ) {
			if( x>=xTop*3 && x<(xTop+cx)*3 && y>=yTop && y<yTop+cy ) {
				c = getc(infile);
				if( c == EOF ) {
					error("err02:bmp が読み込めません");
				}
				*lpTmp++ = (UCHAR)c;
				i = 1;
			} else {
				c = getc(infile);
				if( c == EOF ) {
					error("err02:bmp が読み込めません");
				}
			}
			// 32bit の為の処理
			if( fbmp32==1 ) {
				cbit32++;
				if(cbit32==3) {
					c = getc(infile);			// 空読み
					cbit32=0;
				}
			}
		}
		if( i == 1 ) {
			lpTmp-=(cx*3)*2;
		}
	}
	lpTmp = lpMem+nPlane;


	//
	//	木を初期化
	//
	init_tree();

	code[0] = 0;
	codeptr = mask = 1;
	s = 0;
	r = N - F;

	//
	//	init dictionary ( 4096 - 18 bytes )
	//
	i = 0;
									/* 0 ～ 255 までを 13 byte づつ埋める */
	for (i0 = 0; i0 < 256; i0++) {
		for (i1 = 0; i1 < 13; i1++) {
			text[i++] = i0;
		}
	}

									/* 0 ～ 255 までで埋める */
	for (i1 = 0; i1 < 256; i1++) {
			text[i++] = i1;
	}

									/* 255 ～ 0 までで埋める */
	for (i1 = 255; i1 >= 0; i1--) {
			text[i++] = i1;
	}

									/* 0 を 128 byte 埋める */
	for (i1 = 0; i1 < 128; i1++) {
			text[i++] = 0;
	}

									/* 0x20 を 128 - 18 byte 埋める */
	for (i1 = 0; i1 < 128 - F; i1++) {
			text[i++] = 0x20;
	}

									/* 0 を 18 byte 埋める */
	for (i1 = 0; i1 < F; i1++) {
			text[i++] = 0;
	}


	//
	//	read first 18 bytes to dictionary
	//
	for (len = 0; len < F ; len++) {
		if( cRead == 0 ) {
			break;
		}
		text[r + len] = *lpTmp;
		if( mode == 1 ) {
			if(( *lpTmp != *(lpTmp+1) ) || ( *lpTmp != *(lpTmp+2) )) {
				error("err02:blue/green/red plane の一致しません");
			}
		}
		lpTmp+=3;
		cRead-=3;
	}

	incount = len;
	if (incount == 0) {
		return;
	}
	for (i = 1; i <= F; i++) {
		 insert_node(r - i);
	}
	insert_node(r);



	do {
		if (matchlen > len) matchlen = len;
		if (matchlen < 3 ) {
			matchlen = 1;  code[0] |= mask;  code[codeptr++] = text[r];
		} else {
			code[codeptr++] = (UCHAR) matchpos;
			code[codeptr++] = (UCHAR)
				(((matchpos >> 4) & 0xf0) | (matchlen - 3 ));
		}
		if ((mask <<= 1) == 0) {
			for (i = 0; i < codeptr; i++) putc(code[i], outfile);
			nPrs += codeptr;
			code[0] = 0;  codeptr = mask = 1;
		}
		lastmatchlen = matchlen;
		for (i = 0; i < lastmatchlen; i++) {


			if( cRead == 0 ) {
				break;
			}
			delete_node(s);
			text[s] = *lpTmp;
			if (s < F - 1) {
				text[s + N] = *lpTmp;
			}
			if( mode == 1 ) {
				if(( *lpTmp != *(lpTmp+1) ) || ( *lpTmp != *(lpTmp+2) )) {
					error("err02:blue/green/red plane の一致しません");
				}
			}
			lpTmp+=3;
			cRead-=3;
			s = (s + 1) & (N - 1);
			r = (r + 1) & (N - 1);
			insert_node(r);
		}




		//
		//	1024 byte 毎に表示
		//
		if ((incount += i) > printcount) {
			printf("%12lu\r", incount);  printcount += 1024;
		}




		while (i++ < lastmatchlen) {
			delete_node(s);
			s = (s + 1) & (N - 1);  r = (r + 1) & (N - 1);
			if (--len) insert_node(r);
		}

	} while (len > 0);




	if (codeptr > 1) {
		for (i = 0; i < codeptr; i++) putc(code[i], outfile);
		nPrs += codeptr;
	}


	//
	//	print resalt
	//
	printf("%lu byte(s) / ", nPrs);
	printf("%lu byte(s) = ", incount);
	if (incount != 0) {  /* 圧縮比を求めて報告 */
		cr = (1000 * nPrs + incount / 2) / incount;
		printf("%lu.%03lu\n", cr / 10, cr % 10);
	}

	free( lpMem );
	lpMem=NULL;
}

/*---------------------------------------------------------------------------
	encode main
---------------------------------------------------------------------------*/
void WriteData( DWORD cmd, DWORD nWriteBit )
{
	// roll 命令がないので小細工
	(*(DWORD*)lpWrite) &= ((mask0[nWriteBit] << nShift)+mask1[nShift]);
	(*(DWORD*)lpWrite) |= (cmd<<=nShift);

	nShift+=nWriteBit;
	lpWrite+=(nShift>>3);
	nShift&=0x7;

	if( lpWrite>&WriteBuff[MAX_WRITEBUFF-16] ) {
		nPrs += lpWrite-WriteBuff;
		fwrite(WriteBuff, sizeof(char), lpWrite-WriteBuff, outfile);
		WriteBuff[0] = *lpWrite;
		lpWrite = WriteBuff;
	}
}
//---------------------------------------------------------------------------
void encode0002( int xTop, int yTop, int cx, int cy, int xs, int ys, int fbmp32 )
{
	BYTE *lpMem,*lpTmp;
	int c,x,y,i,cRead,xm;
	int incount,cr;

	char *lp0,*lpa,*lpb,*lpc;
	char b_0,r_0,g_0;
	char b_a,r_a,g_a;
	char b_b,r_b,g_b;
	char b_c,r_c,g_c;
	char b_t,r_t,g_t;
	char pixel0_b,pixel0_r,pixel0_g;
	char pixel1_b,pixel1_r,pixel1_g;
	int nBit0,nBit1;
	DWORD cmd,nWriteBit;
	int cxMem;
	int cbit32;								// 32bit bmp の時のための処理


	//
	//	alloc
	//
	lpMem = malloc( cx*cy*4 );
	if( lpMem == NULL ) {
		error("err02:memory が alloc 出来ません");
	}


	//
	//	read bmp file
	//
	cRead = cx*cy*3;
	lpTmp = lpMem+cx*(cy-1)*3;
	xm = ((xs*3+3)&0xfffffffc);
	cbit32=0;
	for( y=ys-1;y>=0;y-- ) {
		i = 0;
		for( x=0;x<xm;x++ ) {
			if( x>=xTop*3 && x<(xTop+cx)*3 && y>=yTop && y<yTop+cy ) {
				c = getc(infile);
				if( c == EOF ) {
					error("err02:bmp が読み込めません");
				}
				*lpTmp++ = (UCHAR)c;
				i = 1;
			} else {
				c = getc(infile);
				if( c == EOF ) {
					error("err02:bmp が読み込めません");
				}
			}
			// 32bit の為の処理
			if( fbmp32==1 ) {
				cbit32++;
				if(cbit32==3) {
					c = getc(infile);			// 空読み
					cbit32=0;
				}
			}
		}
		if( i == 1 ) {
			lpTmp-=(cx*3)*2;
		}
	}
	// この時点で cx*cy の画像になっている


	//
	//	prs
	//
	//
	//	最初の 1 line の処理
	//
	nShift = 0;
	lpWrite = WriteBuff;
	lp0 = (char*)lpMem;
	cxMem = cx*3;
	// 最初の 1 dot の処理
	b_0 = *lp0++;
	r_0 = *lp0++;
	g_0 = *lp0++;
	WriteData( ((DWORD)b_0)&0xff, 8 );
	WriteData( ((DWORD)r_0)&0xff, 8 );
	WriteData( ((DWORD)g_0)&0xff, 8 );

	// 最初の 1 line の処理
	lpa = lp0-3;
	for( x=1;x<cx;x++ ) {
		b_0 = *lp0++;
		r_0 = *lp0++;
		g_0 = *lp0++;
		b_a = *lpa++;
		r_a = *lpa++;
		g_a = *lpa++;
		if( b_0==b_a && r_0==r_a && g_0==g_a ) {
			WriteData( (DWORD)0x1, 1 );
		} else {
			nBit0 = 1000;
			if( b_0>=b_a-13 && b_0<=b_a+12 &&
				r_0>=r_a-13 && r_0<=r_a+12 && 
				g_0>=g_a-13 && g_0<=g_a+12 ) {
				pixel0_b = b_0-b_a+13;
				pixel0_r = r_0-r_a+13;
				pixel0_g = g_0-g_a+13;
				nBit0=2+nAdd[pixel0_b]+nAdd[pixel0_r]+nAdd[pixel0_g];
			}
			if( nBit0>(2+8+8+8) ) {
				WriteData( (DWORD)0x00, 2 );
				WriteData( ((DWORD)b_0)&0xff, 8 );
				WriteData( ((DWORD)r_0)&0xff, 8 );
				WriteData( ((DWORD)g_0)&0xff, 8 );
			} else {
				WriteData( (DWORD)0x02, 2 );
				WriteData( nVal[pixel0_b], nAdd[pixel0_b] );
				WriteData( nVal[pixel0_r], nAdd[pixel0_r] );
				WriteData( nVal[pixel0_g], nAdd[pixel0_g] );
			}
		}
	}


	for( y=1;y<cy;y++ ) {
		lp0 = (char*)lpMem+y*cxMem;
		lpc = lp0-cxMem;
		b_0 = *lp0++;
		r_0 = *lp0++;
		g_0 = *lp0++;
		b_c = *lpc++;
		r_c = *lpc++;
		g_c = *lpc++;
		// 最初の1 dot の処理
		if( b_0==b_c && r_0==r_c && g_0==g_c ) {
			WriteData( (DWORD)0x1, 1 );
		} else {
			nBit0 = 1000;
			if( b_0>=b_c-13 && b_0<=b_c+12 &&
				r_0>=r_c-13 && r_0<=r_c+12 && 
				g_0>=g_c-13 && g_0<=g_c+12 ) {
				pixel0_b = b_0-b_c+13;
				pixel0_r = r_0-r_c+13;
				pixel0_g = g_0-g_c+13;
				nBit0=2+nAdd[pixel0_b]+nAdd[pixel0_r]+nAdd[pixel0_g];
			}
			if( nBit0>(2+8+8+8) ) {
				WriteData( (DWORD)0x00, 2 );
				WriteData( ((DWORD)b_0)&0xff, 8 );
				WriteData( ((DWORD)r_0)&0xff, 8 );
				WriteData( ((DWORD)g_0)&0xff, 8 );
			} else {
				WriteData( (DWORD)0x02, 2 );
				WriteData( nVal[pixel0_b], nAdd[pixel0_b] );
				WriteData( nVal[pixel0_r], nAdd[pixel0_r] );
				WriteData( nVal[pixel0_g], nAdd[pixel0_g] );
			}
		}

		lpa = lp0-3;
		lpb = lpc-3;

		for( x=1;x<cx;x++ ) {
			b_0 = *lp0++;
			r_0 = *lp0++;
			g_0 = *lp0++;
			b_a = *lpa++;
			r_a = *lpa++;
			g_a = *lpa++;
			b_b = *lpb++;
			r_b = *lpb++;
			g_b = *lpb++;
			b_c = *lpc++;
			r_c = *lpc++;
			g_c = *lpc++;

			b_t = b_a+b_c-b_b;
			r_t = r_a+r_c-r_b;
			g_t = g_a+g_c-g_b;

			if( b_0==b_t && r_0==r_t && g_0==g_t ) {
				WriteData( (DWORD)0x02, 2 );
			} else {
				nBit0=1000;
				cmd =0xffffffff;
				if( b_0==b_a && r_0==r_a && g_0==g_a ) {
					cmd = 0x0c;
					nWriteBit = 4;
					nBit0=4;
				} else if( b_0==b_b && r_0==r_b && g_0==g_b ) {
					cmd = 0x00;
					nWriteBit = 6;
					nBit0=6;
				} else if( b_0==b_c && r_0==r_c && g_0==g_c ) {
					cmd = 0x20;
					nWriteBit = 6;
					nBit0=6;
				}
				if( b_0>=b_t-13 && b_0<=b_t+12 &&
					r_0>=r_t-13 && r_0<=r_t+12 && 
					g_0>=g_t-13 && g_0<=g_t+12 ) {
					pixel1_b = b_0-b_t+13;
					pixel1_r = r_0-r_t+13;
					pixel1_g = g_0-g_t+13;
					nBit1=1+nAdd[pixel1_b]+nAdd[pixel1_r]+nAdd[pixel1_g];
					if( nBit0>nBit1 ) {
						nBit0=nBit1;
						pixel0_b=pixel1_b;
						pixel0_r=pixel1_r;
						pixel0_g=pixel1_g;
						cmd = 0x01;
						nWriteBit = 1;
					}
				}
				if( b_0>=b_a-13 && b_0<=b_a+12 &&
					r_0>=r_a-13 && r_0<=r_a+12 && 
					g_0>=g_a-13 && g_0<=g_a+12 ) {
					pixel1_b = b_0-b_a+13;
					pixel1_r = r_0-r_a+13;
					pixel1_g = g_0-g_a+13;
					nBit1=4+nAdd[pixel1_b]+nAdd[pixel1_r]+nAdd[pixel1_g];
					if( nBit0>nBit1 ) {
						nBit0=nBit1;
						pixel0_b=pixel1_b;
						pixel0_r=pixel1_r;
						pixel0_g=pixel1_g;
						cmd = 0x04;
						nWriteBit = 4;
					}
				}
				if( b_0>=b_b-13 && b_0<=b_b+12 &&
					r_0>=r_b-13 && r_0<=r_b+12 && 
					g_0>=g_b-13 && g_0<=g_b+12 ) {
					pixel1_b = b_0-b_b+13;
					pixel1_r = r_0-r_b+13;
					pixel1_g = g_0-g_b+13;
					nBit1=6+nAdd[pixel1_b]+nAdd[pixel1_r]+nAdd[pixel1_g];
					if( nBit0>nBit1 ) {
						nBit0=nBit1;
						pixel0_b=pixel1_b;
						pixel0_r=pixel1_r;
						pixel0_g=pixel1_g;
						cmd = 0x10;
						nWriteBit = 6;
					}
				}
				if( b_0>=b_c-13 && b_0<=b_c+12 &&
					r_0>=r_c-13 && r_0<=r_c+12 && 
					g_0>=g_c-13 && g_0<=g_c+12 ) {
					pixel1_b = b_0-b_c+13;
					pixel1_r = r_0-r_c+13;
					pixel1_g = g_0-g_c+13;
					nBit1=6+nAdd[pixel1_b]+nAdd[pixel1_r]+nAdd[pixel1_g];
					if( nBit0>nBit1 ) {
						nBit0=nBit1;
						pixel0_b=pixel1_b;
						pixel0_r=pixel1_r;
						pixel0_g=pixel1_g;
						cmd = 0x30;
						nWriteBit = 6;
					}
				}
				if( cmd==0x0c || cmd==0x00 || cmd==0x20 ) {
					WriteData( cmd, nWriteBit );
				} else if( nBit0>(4+8+8+8) ) {
					WriteData( (DWORD)0x08, 4 );
					WriteData( ((DWORD)b_0)&0xff, 8 );
					WriteData( ((DWORD)r_0)&0xff, 8 );
					WriteData( ((DWORD)g_0)&0xff, 8 );
				} else {
					WriteData( cmd, nWriteBit );
					WriteData( nVal[pixel0_b], nAdd[pixel0_b] );
					WriteData( nVal[pixel0_r], nAdd[pixel0_r] );
					WriteData( nVal[pixel0_g], nAdd[pixel0_g] );
				}
			}
		}
	}

	// 最後
	fwrite(WriteBuff, sizeof(char), lpWrite-WriteBuff+1, outfile);
	nPrs += lpWrite-WriteBuff+1;

	//
	//	print resalt
	//
	incount = cx*cy*3;
	printf("%lu byte(s) / ", nPrs);
	printf("%lu byte(s) = ", incount);
	if (incount != 0) {  /* 圧縮比を求めて報告 */
		cr = (1000 * nPrs + incount / 2) / incount;
		printf("%lu.%03lu\n", cr / 10, cr % 10);
	}
	free( lpMem );
	lpMem=NULL;
}
/*---------------------------------------------------------------------------
	main
---------------------------------------------------------------------------*/
char* GetNum( int *lpData, char* lpTmp )
//
//	skip collon
//
{
	int c,f;

	// skip space
//	while( *lpTmp == ' ' || *lpTMp == 0x2c ) {
//		lpTmp++;
//	}

	c = 0;
	f = 0;
	while( *lpTmp>='0' && *lpTmp<='9' ) {
		c = c*10 + (*lpTmp)-'0';
		lpTmp++;
		f = 1;
	}

	if( f == 0 ) {
		lpTmp = NULL;
	}

	*lpData = c;
	return lpTmp;
}
/*--------------------------------------------------------------------------*/
char* GetSignedNum( int *lpData, char* lpTmp )
//
//	skip collon
//
{
	int c,f,s;

	// skip space
//	while( *lpTmp == ' ' || *lpTMp == 0x2c ) {
//		lpTmp++;
//	}

	c = 0;
	f = 0;
	s = 0;

	if( *lpTmp == '-' ) {
		s = 1;
		lpTmp++;
	} else if( *lpTmp == '+' ) {
		lpTmp++;
	}

	while( *lpTmp>='0' && *lpTmp<='9' ) {
		c = c*10 + (*lpTmp)-'0';
		lpTmp++;
		f = 1;
	}

	if( f == 0 ) {
		lpTmp = NULL;
	}


	if( s == 1 ) {
		c = -c;
	}

	*lpData = c;
	return lpTmp;
}
/*--------------------------------------------------------------------------*/
char* SkipCollon( char *lpTmp )
//
//	skip collon
//
{
	// skip space
//	while( *lpTmp == ' ' || *lpTMp == 0x2c ) {
//		lpTmp++;
//	}

	if( *lpTmp == ',' ) {
		lpTmp++;
	} else {
		lpTmp = NULL;
	}

	return lpTmp;
}
/*--------------------------------------------------------------------------*/
void SetSameTime( char *szD, char *szS)
//
// szDのタイムスタンプをszSに合わせる
//
{
#if TYPEMODE==0
	int c;
	struct _stat stat_buf;
	struct _utimbuf utime_buf;

	c = _stat( szS, &stat_buf );
	if( c == 0 ) {
		utime_buf.actime = stat_buf.st_atime;
		utime_buf.modtime = stat_buf.st_mtime;
		c = _utime( szD, &utime_buf );
	}

#else
	HANDLE hS = (HANDLE)0xffffffff;
	HANDLE hD = (HANDLE)0xffffffff;
	FILETIME CreationTime,LastAccessTime,LastWriteTime;

	hS = CreateFile( szS, GENERIC_READ, 0,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,0,NULL );
	if(hS==NULL) return;
	hD = CreateFile( szD, GENERIC_WRITE, 0,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,0,NULL );
	if(hD==NULL) return;

	GetFileTime( hS,&CreationTime,&LastAccessTime,&LastWriteTime );
	SetFileTime( hD,&CreationTime,&LastAccessTime,&LastWriteTime );

	if(hD!=NULL) CloseHandle( hD );
	if(hS!=NULL) CloseHandle( hS );
#endif
}
/*--------------------------------------------------------------------------*/
int mainprog(int argc, char *argv[])
{
	char filename0[_MAX_PATH*MAX_BUFFER];
	char filename1[_MAX_PATH*MAX_BUFFER];
	char *lpTmp;
	DWORD nVer,nVerPara0,xTop,yTop,xs,ys,xst,yst,nData,dw0,size,top;
	WORD w0;
	int nColor,c,d,e,fExp,xExp,yExp,cx,cy,fDate,fUpper;
	UCHAR opt;
	int sj;

	/* 初期化 */
	lpMem=NULL;
	infile=NULL;
	outfile=NULL;

	sj=setjmp( main_sj );
	if( sj != 0 ) {
		/* error 終了の時の処理 */
		if(lpMem!=NULL) free( lpMem );
		if(infile!=NULL) fclose(infile);
		if(outfile!=NULL) fclose(outfile);
		return sj;
	}


	d = 0;
	strcpy( filename0, "" );
	strcpy( filename1, "" );
	for( c=1;c<argc;c++) {
		if( argv[c][0] == '/' ) {
			opt = _mbctolower( argv[c][1] );
			switch( opt ) {
				default:
					error("err02:option が異常です");
					break;
			}
		} else {
			if( d == 0 ) {
				strcpy(filename0, argv[c]);
				d = 1;
			} else if( d==1 ) {
				strcpy(filename0m, argv[c]);
				d = 2;
			} else {
				strcpy(filename1, argv[c]);
				d = 3;
			}
		}
	}



	if( d == 0 ) {
		usage();
	}


	/*
		close
	*/
	infile=NULL;
	fclose(outfile);							/* close */
	outfile=NULL;


	/*
		set date
	*/
	if( fDate == 1 ) SetSameTime(filename1,filename0);

	return EXIT_SUCCESS;
}

/*----------------------------------------------------------------------------
	make.cを内蔵する為のもの(非依存型makeでasm単体版はkmake.com,c言語単体版はsmame.exe)
----------------------------------------------------------------------------*/
int makecall(int argv, char **argc )
/*
	make プログラムから呼ばれる
*/
{
	return mainprog( argv, argc );
}
//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int r;

	/* オプションの先頭に -f があるか ? */
	if( makechk( argc,argv ) ){
		// make へプログラムを登録
		makeregist( argv[0] );
		r=smake( argc,argv,"",1,1 );
		if(r!=0) printmakeerror( argv[0],r );
		return r;
	} else {
		return mainprog( argc,argv );
	}
}

#endif
