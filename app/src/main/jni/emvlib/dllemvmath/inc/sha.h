#ifndef _SHA_H
#define _SHA_H

#include "dllemvglobal.h"


#ifndef BYTE 
#define BYTE unsigned char
#endif

#ifndef DDWORD 
#define DDWORD unsigned long
#endif

#ifndef uchar 
#define uchar unsigned char
#endif
#ifndef uint 
#define uint unsigned int
#endif
#ifndef ushort 
#define ushort unsigned short
#endif
#ifndef ulong 
#define ulong unsigned long
#endif

#define ASCII      0x01
#define CFONT      0x00

#define SHA_VERSION 1

//in PC or C51 it is 1234
#define SHA_BYTE_ORDER 1234
//in MC68Kvz328 it is 4321
//#define SHA_BYTE_ORDER 4321

/* NIST Secure Hash Algorithm */
/* heavily modified by Uwe Hollerbach <uh@alumni.caltech edu> */
/* from Peter C. Gutmann's implementation as found in */
/* Applied Cryptography by Bruce Schneier */

/* This code is in the public domain */

/* Useful defines & typedefs */
//typedef unsigned char BYTE;	/* 8-bit quantity */
typedef unsigned long LONG;	/* 32-or-more-bit quantity */

#define SHA_BLOCKSIZE		64
#define SHA_DIGESTSIZE		20


typedef struct {
    LONG digest[5];		/* message digest */
    LONG count_lo, count_hi;	/* 64-bit bit count */
    BYTE data[SHA_BLOCKSIZE];	/* SHA data buffer */
    int local;			/* unprocessed amount in data */
} SHA_INFO;

void sha_init(SHA_INFO *);
void sha_update(SHA_INFO *, BYTE *, int);
void sha_final(unsigned char [20], SHA_INFO *);

void EMVBase_Hash(unsigned char* bb,unsigned long len,unsigned char* Message_Digest);
unsigned char EMVBase_RSARecover(unsigned char *m, unsigned int mLen, unsigned char *e, unsigned int eLen,unsigned char *input,unsigned char *output);

#endif

