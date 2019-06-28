#pragma once
#include<stdio.h>

#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

#define NUMTOCENTRIES 100

/*structure to receive extracted header */
/* toc may be NULL*/
typedef struct {
	int     h_id;            /* from MPEG header, 0=MPEG2, 1=MPEG1 */
	int     samprate;        /* determined from MPEG header */
	int     flags;           /* from Vbr header data */
	int     frames;          /* total bit stream frames from Vbr header data */
	int     bytes;           /* total bit stream bytes from Vbr header data */
	int     vbr_scale;       /* encoded vbr scale from Vbr header data */
	unsigned char toc[NUMTOCENTRIES]; /* may be NULL if toc not desired */
	int     headersize;      /* size of VBR header, in bytes */
	int     enc_delay;       /* encoder delay */
	int     enc_padding;     /* encoder paddign added at end of stream */
} VBRTAGDATA;

int     GetVbrTag(VBRTAGDATA * pTagData, const unsigned char *buf);
