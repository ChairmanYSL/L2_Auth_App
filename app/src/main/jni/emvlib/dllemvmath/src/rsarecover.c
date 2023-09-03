#include <stdio.h>
#include <string.h>
#include "rsa.h"
#include "sha1.h"


typedef struct {
  unsigned short bits;                     /* length in bits of modulus */
  unsigned char modulus[MAX_RSA_MODULUS_LEN];  /* modulus */
  unsigned char exponent[MAX_RSA_MODULUS_LEN]; /* public exponent */
} R_RSA_PUBLIC_KEY;


extern void *emvbase_malloc (unsigned int size);
extern void emvbase_free (void *block);

#define EMVBase_NN_ASSIGN_DIGIT(a, b, digits) {EMVBase_NN_AssignZero (a, digits); a[0] = b;}


/* Internal function prototypes */
static NN_DIGIT EMVBase_subdigitmult PROTO_LIST
	((NN_DIGIT *, NN_DIGIT *, NN_DIGIT, NN_DIGIT *, unsigned int));
static void EMVBase_dmult PROTO_LIST ((NN_DIGIT, NN_DIGIT, NN_DIGIT *, NN_DIGIT *));
static unsigned int EMVBase_NN_DigitBits PROTO_LIST ((NN_DIGIT));



/* Returns sign of a - b. */

static int EMVBase_NN_Cmp (NN_DIGIT *a, NN_DIGIT *b, unsigned int digits)
{

	if(digits) {
		do {
			digits--;
			if(*(a+digits) > *(b+digits))
				return(1);
			if(*(a+digits) < *(b+digits))
				return(-1);
		}while(digits);
	}

	return (0);
}


static void EMVBase_NN_Assign (NN_DIGIT *a, NN_DIGIT *b, unsigned int digits)
{
	if(digits) {
		do {
			*a++ = *b++;
		}while(--digits);
	}
}

/* Returns the significant length of a in digits. */

static unsigned int EMVBase_NN_Digits (NN_DIGIT *a, unsigned int digits)
{

	if(digits) {
		digits--;
		do {
			if(*(a+digits))
				break;
		}while(digits--);

		return(digits + 1);
	}

	return(digits);
}


/* Use one key from RSADSI's RSAREF(tm) RDEMO for comptiblity testing.
	 This ensures that both RSAEURO and RSAREF(tm) code is compatible.
*/

////////////////////////////////////////////////////////////////////////

/* Decodes character string b into a, where character string is ordered
	 from most to least significant.

	 Lengths: a[digits], b[len].
	 Assumes b[i] = 0 for i < len - digits * NN_DIGIT_LEN. (Otherwise most
	 significant bytes are truncated.)
 */
static void EMVBase_NN_Decode (NN_DIGIT *a, unsigned int digits, unsigned char *b,unsigned int len)
{
  NN_DIGIT t;
  int j;
  unsigned int i, u;
  
  for (i = 0, j = len - 1; i < digits && j >= 0; i++) {
    t = 0;
    for (u = 0; j >= 0 && u < NN_DIGIT_BITS; j--, u += 8)
			t |= ((NN_DIGIT)b[j]) << u;
		a[i] = t;
  }
  
  for (; i < digits; i++)
    a[i] = 0;
}

/* Encodes b into character string a, where character string is ordered
   from most to least significant.

	 Lengths: a[len], b[digits].
	 Assumes NN_Bits (b, digits) <= 8 * len. (Otherwise most significant
	 digits are truncated.)
 */
static void EMVBase_NN_Encode (unsigned char *a,unsigned int len, NN_DIGIT *b,unsigned int digits)
{
	NN_DIGIT t;
	int j;
	unsigned int i, u;

	for (i = 0, j = len - 1; i < digits && j >= 0; i++) {
		t = b[i];
		for (u = 0; j >= 0 && u < NN_DIGIT_BITS; j--, u += 8)
			a[j] = (unsigned char)(t >> u);
	}

	for (; j >= 0; j--)
		a[j] = 0;
}

/* Assigns a = 0. */

static void EMVBase_NN_AssignZero (NN_DIGIT *a,unsigned int digits)
{
	if(digits) {
		do {
			*a++ = 0;
		}while(--digits);
	}
}

/* Computes a = b - c. Returns borrow.

	 Lengths: a[digits], b[digits], c[digits].
 */

static NN_DIGIT EMVBase_NN_Sub (NN_DIGIT *a, NN_DIGIT *b,NN_DIGIT * c, unsigned int digits)
{
	NN_DIGIT temp, borrow = 0;

	if(digits)
		do {
			if((temp = (*b++) - borrow) == MAX_NN_DIGIT)
				temp = MAX_NN_DIGIT - *c++;
			else
			{
			    temp -= *c;
				if((temp) > (MAX_NN_DIGIT - *c++))
					borrow = 1;
				else
					borrow = 0;
			}
			*a++ = temp;
		}while(--digits);

	return(borrow);
}

/* Computes a = b * c.

	 Lengths: a[2*digits], b[digits], c[digits].
	 Assumes digits < MAX_NN_DIGITS.
*/

static void EMVBase_NN_Mult (NN_DIGIT *a, NN_DIGIT *b, NN_DIGIT *c, unsigned int digits)
{
	NN_DIGIT *t;  //[2*MAX_NN_DIGITS]
	NN_DIGIT dhigh, dlow, carry;
	unsigned int bDigits, cDigits, i, j;

    t = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*2*MAX_NN_DIGITS);

	
	EMVBase_NN_AssignZero (t, 2 * digits);

	bDigits = EMVBase_NN_Digits (b, digits);
	cDigits = EMVBase_NN_Digits (c, digits);

	for (i = 0; i < bDigits; i++) {
		carry = 0;
		if(*(b+i) != 0) {
			for(j = 0; j < cDigits; j++) {
				EMVBase_dmult(*(b+i), *(c+j), &dhigh, &dlow);
				if((*(t+(i+j)) = *(t+(i+j)) + carry) < carry)
					carry = 1;
				else
					carry = 0;
				if((*(t+(i+j)) += dlow) < dlow)
					carry++;
				carry += dhigh;
			}
		}
		*(t+(i+cDigits)) += carry;
	}


	EMVBase_NN_Assign(a, t, 2 * digits);
    
	emvbase_free(t);
	
}

/* Computes a = b * 2^c (i.e., shifts left c bits), returning carry.

	 Requires c < NN_DIGIT_BITS. */

static NN_DIGIT EMVBase_NN_LShift (NN_DIGIT *a, NN_DIGIT *b, unsigned int c, unsigned int digits)
{
	NN_DIGIT temp, carry = 0;
	unsigned int t;

	if(c < NN_DIGIT_BITS)
		if(digits) {

			t = NN_DIGIT_BITS - c;

			do {
				temp = *b++;
				*a++ = (temp << c) | carry;
				carry = c ? (temp >> t) : 0;
			}while(--digits);
		}

	return (carry);
}

/* Computes a = c div 2^c (i.e., shifts right c bits), returning carry.

	 Requires: c < NN_DIGIT_BITS. */

static NN_DIGIT EMVBase_NN_RShift (NN_DIGIT *a,NN_DIGIT *b,unsigned int c, unsigned int digits)
{
	NN_DIGIT temp, carry = 0;
	unsigned int t;

	if(c < NN_DIGIT_BITS)
		if(digits) {

			t = NN_DIGIT_BITS - c;

			do {
				digits--;
				temp = *(b+digits);
				*(a+digits) = (temp >> c) | carry;
				carry = c ? (temp << t) : 0;
			}while(digits);
		}

	return (carry);
}

/* Computes a = c div d and b = c mod d.

	 Lengths: a[cDigits], b[dDigits], c[cDigits], d[dDigits].
	 Assumes d > 0, cDigits < 2 * MAX_NN_DIGITS,
					 dDigits < MAX_NN_DIGITS.
*/

static void EMVBase_NN_Div (NN_DIGIT *a, NN_DIGIT *b, NN_DIGIT *c,unsigned int cDigits, NN_DIGIT *d,unsigned int dDigits)
{
	NN_DIGIT ai, *cc, *dd, s;  //[2*MAX_NN_DIGITS+1]  [MAX_NN_DIGITS]
	NN_DIGIT t[2], u, v, *ccptr;
	NN_HALF_DIGIT aHigh, aLow, cHigh, cLow;
	int i;
	unsigned int ddDigits, shift;

	ddDigits = EMVBase_NN_Digits (d, dDigits);
	if(ddDigits == 0)
		return;
    
	cc = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*2*(MAX_NN_DIGITS+1));
	dd = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);

	shift = NN_DIGIT_BITS - EMVBase_NN_DigitBits (d[ddDigits-1]);
	EMVBase_NN_AssignZero (cc, ddDigits);
	cc[cDigits] = EMVBase_NN_LShift (cc, c, shift, cDigits);
	EMVBase_NN_LShift (dd, d, shift, ddDigits);
	s = dd[ddDigits-1];

	EMVBase_NN_AssignZero (a, cDigits);

	for (i = cDigits-ddDigits; i >= 0; i--) {
		if (s == MAX_NN_DIGIT)
			ai = cc[i+ddDigits];
		else {
			ccptr = &cc[i+ddDigits-1];

			s++;
			cHigh = (NN_HALF_DIGIT)HIGH_HALF (s);
			cLow = (NN_HALF_DIGIT)LOW_HALF (s);

			*t = *ccptr;
			*(t+1) = *(ccptr+1);

			if (cHigh == MAX_NN_HALF_DIGIT)
				aHigh = (NN_HALF_DIGIT)HIGH_HALF (*(t+1));
			else
				aHigh = (NN_HALF_DIGIT)(*(t+1) / (cHigh + 1));
			u = (NN_DIGIT)aHigh * (NN_DIGIT)cLow;
			v = (NN_DIGIT)aHigh * (NN_DIGIT)cHigh;
			if ((*t -= TO_HIGH_HALF (u)) > (MAX_NN_DIGIT - TO_HIGH_HALF (u)))
				t[1]--;
			*(t+1) -= HIGH_HALF (u);
			*(t+1) -= v;

			while ((*(t+1) > cHigh) ||
						 ((*(t+1) == cHigh) && (*t >= TO_HIGH_HALF (cLow)))) {
				if ((*t -= TO_HIGH_HALF (cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF (cLow))
					t[1]--;
				*(t+1) -= cHigh;
				aHigh++;
			}

			if (cHigh == MAX_NN_HALF_DIGIT)
				aLow = (NN_HALF_DIGIT)LOW_HALF (*(t+1));
			else
				aLow =
			(NN_HALF_DIGIT)((TO_HIGH_HALF (*(t+1)) + HIGH_HALF (*t)) / (cHigh + 1));
			u = (NN_DIGIT)aLow * (NN_DIGIT)cLow;
			v = (NN_DIGIT)aLow * (NN_DIGIT)cHigh;
			if ((*t -= u) > (MAX_NN_DIGIT - u))
				t[1]--;
			if ((*t -= TO_HIGH_HALF (v)) > (MAX_NN_DIGIT - TO_HIGH_HALF (v)))
				t[1]--;
			*(t+1) -= HIGH_HALF (v);

			while ((*(t+1) > 0) || ((*(t+1) == 0) && *t >= s)) {
				if ((*t -= s) > (MAX_NN_DIGIT - s))
					t[1]--;
				aLow++;
			}

			ai = TO_HIGH_HALF (aHigh) + aLow;
			s--;
		}

		cc[i+ddDigits] -= EMVBase_subdigitmult(&cc[i], &cc[i], ai, dd, ddDigits);

		while (cc[i+ddDigits] || (EMVBase_NN_Cmp (&cc[i], dd, ddDigits) >= 0)) {
			ai++;
			cc[i+ddDigits] -= EMVBase_NN_Sub (&cc[i], &cc[i], dd, ddDigits);
		}

		a[i] = ai;
	}

	EMVBase_NN_AssignZero (b, dDigits);
	EMVBase_NN_RShift (b, cc, shift, ddDigits);

    emvbase_free(cc);
	emvbase_free(dd);
	
	
}

/* Computes a = b mod c.

	 Lengths: a[cDigits], b[bDigits], c[cDigits].
	 Assumes c > 0, bDigits < 2 * MAX_NN_DIGITS, cDigits < MAX_NN_DIGITS.
*/
static void EMVBase_NN_Mod (NN_DIGIT *a, NN_DIGIT *b, unsigned int bDigits, NN_DIGIT *c,unsigned int cDigits)
{
    NN_DIGIT *t;  //[2 * MAX_NN_DIGITS]
    
	
    t = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*2*MAX_NN_DIGITS);
	
	EMVBase_NN_Div (t, a, b, bDigits, c, cDigits);
    
	emvbase_free(t);
    
	
}

/* Computes a = b * c mod d.

   Lengths: a[digits], b[digits], c[digits], d[digits].
   Assumes d > 0, digits < MAX_NN_DIGITS.
 */
static void EMVBase_NN_ModMult (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT *c,NN_DIGIT *d,unsigned int digits)
{
    NN_DIGIT *t;   //[2*MAX_NN_DIGITS]

	t = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*2*MAX_NN_DIGITS);

	EMVBase_NN_Mult (t, b, c, digits);
    EMVBase_NN_Mod (a, t, 2 * digits, d, digits);

	emvbase_free(t);
}

/* Computes a = b^c mod d.

   Lengths: a[dDigits], b[dDigits], c[cDigits], d[dDigits].
	 Assumes d > 0, cDigits > 0, dDigits < MAX_NN_DIGITS.
 */
static void EMVBase_NN_ModExp (NN_DIGIT *a, NN_DIGIT *b, NN_DIGIT *c, unsigned int cDigits, NN_DIGIT *d, unsigned int dDigits)
{
    NN_DIGIT *bPower[3], ci, *t;  //[MAX_NN_DIGITS]   [MAX_NN_DIGITS]
    int i;
	unsigned int ciBits, j, s;

	/* Store b, b^2 mod d, and b^3 mod d.
	 */
    bPower[0] = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);
	bPower[1] = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);
	bPower[2] = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);
	t = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);

	
	EMVBase_NN_Assign (bPower[0], b, dDigits);
	EMVBase_NN_ModMult (bPower[1], bPower[0], b, d, dDigits);
    EMVBase_NN_ModMult (bPower[2], bPower[1], b, d, dDigits);
  
    EMVBase_NN_ASSIGN_DIGIT (t, 1, dDigits);

	cDigits = EMVBase_NN_Digits (c, cDigits);
    for (i = cDigits - 1; i >= 0; i--) {
		ci = c[i];
		ciBits = NN_DIGIT_BITS;

		/* Scan past leading zero bits of most significant digit.
		 */
		if (i == (int)(cDigits - 1)) {
			while (! DIGIT_2MSB (ci)) {
				ci <<= 2;
				ciBits -= 2;
			}
        }

        for (j = 0; j < ciBits; j += 2, ci <<= 2) {
        /* Compute t = t^4 * b^s mod d, where s = two MSB's of ci. */
            EMVBase_NN_ModMult (t, t, t, d, dDigits);
            EMVBase_NN_ModMult (t, t, t, d, dDigits);
            if ((s = DIGIT_2MSB (ci)) != 0)
            EMVBase_NN_ModMult (t, t, bPower[s-1], d, dDigits);
        }
    }
  
	EMVBase_NN_Assign (a, t, dDigits);

	emvbase_free(bPower[0]);
	emvbase_free(bPower[1]);
	emvbase_free(bPower[2]);
	emvbase_free(t);

	
}

/* Returns the significant length of a in bits.

	 Lengths: a[digits]. */

static unsigned int EMVBase_NN_Bits (NN_DIGIT *a, unsigned int digits)
{
	if ((digits = EMVBase_NN_Digits (a, digits)) == 0)
		return (0);

	return ((digits - 1) * NN_DIGIT_BITS + EMVBase_NN_DigitBits (a[digits-1]));
}

static NN_DIGIT EMVBase_subdigitmult(NN_DIGIT *a,NN_DIGIT *b, NN_DIGIT c, NN_DIGIT *d,unsigned int digits)
{
	NN_DIGIT borrow, thigh, tlow;
	unsigned int i;

	borrow = 0;

	if(c != 0) {
		for(i = 0; i < digits; i++) {
			EMVBase_dmult(c, d[i], &thigh, &tlow);
			if((a[i] = b[i] - borrow) > (MAX_NN_DIGIT - borrow))
				borrow = 1;
			else
				borrow = 0;
			if((a[i] -= tlow) > (MAX_NN_DIGIT - tlow))
				borrow++;
			borrow += thigh;
		}
	}

	return (borrow);
}

/* Returns the significant length of a in bits, where a is a digit. */
static unsigned int EMVBase_NN_DigitBits (NN_DIGIT a)
{
	unsigned int i;

	for (i = 0; i < NN_DIGIT_BITS; i++, a >>= 1)
		if (a == 0)
			break;

	return (i);
}

/* Computes a * b, result stored in high and low. */
 
static void EMVBase_dmult( NN_DIGIT a,NN_DIGIT b, NN_DIGIT *high,NN_DIGIT *low)
{
	NN_HALF_DIGIT al, ah, bl, bh;
	NN_DIGIT m1, m2, m, ml, mh, carry = 0;

	al = (NN_HALF_DIGIT)LOW_HALF(a);
	ah = (NN_HALF_DIGIT)HIGH_HALF(a);
	bl = (NN_HALF_DIGIT)LOW_HALF(b);
	bh = (NN_HALF_DIGIT)HIGH_HALF(b);

	*low = (NN_DIGIT) al*bl;
	*high = (NN_DIGIT) ah*bh;

	m1 = (NN_DIGIT) al*bh;
	m2 = (NN_DIGIT) ah*bl;
	m = m1 + m2;

	if(m < m1)
        carry = 1L << (NN_DIGIT_BITS / 2);

	ml = (m & MAX_NN_HALF_DIGIT) << (NN_DIGIT_BITS / 2);
	mh = m >> (NN_DIGIT_BITS / 2);

	*low += ml;

	if(*low < ml)
		carry++;

	*high += carry + mh;
}


/* Raw RSA public-key operation. Output has same length as modulus.

	 Requires input < modulus.
*/
static int EMVBase_RSAPublicDecrypt(unsigned char *output, unsigned char *input,unsigned int inputLen, R_RSA_PUBLIC_KEY *publicKey)
/* output block */
/* length of output block */
/* input block */
/* length of input block */
/* RSA public key */
{
	unsigned int modulusLen;
	unsigned int eDigits, nDigits;
	NN_DIGIT *c, *e, *m,*n;  //[MAX_NN_DIGITS]
	
	modulusLen = (publicKey->bits + 7) / 8;
	if(inputLen > modulusLen)
		return 1;

	c = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);
	e = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);
	m = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);
	n = (NN_DIGIT *)emvbase_malloc(sizeof(NN_DIGIT)*MAX_NN_DIGITS);

	
	/* decode the required RSA function input data */
	EMVBase_NN_Decode(m, MAX_NN_DIGITS, input, inputLen);
	EMVBase_NN_Decode(n, MAX_NN_DIGITS, publicKey->modulus, MAX_RSA_MODULUS_LEN);
	EMVBase_NN_Decode(e, MAX_NN_DIGITS, publicKey->exponent, MAX_RSA_MODULUS_LEN);	
	nDigits = EMVBase_NN_Digits(n, MAX_NN_DIGITS);	
	eDigits = EMVBase_NN_Digits(e, MAX_NN_DIGITS);
/*	if(NN_Cmp(m, n, nDigits) >= 0)
		return(RE_DATA);
*/
//	*outputLen = (publicKey->bits + 7) / 8;

	/* Compute c = m^e mod n.  To perform actual RSA calc.*/

	EMVBase_NN_ModExp(c, m, e, eDigits, n, nDigits);
	/* encode output to standard form */
	EMVBase_NN_Encode(output, modulusLen, c, nDigits);

    emvbase_free(c);
	emvbase_free(e);
    emvbase_free(m);
	emvbase_free(n);
	
	return 0;
	
}



unsigned char EMVBase_RSARecover(unsigned char *m, unsigned int mLen, unsigned char *e, unsigned int eLen,unsigned char *input,unsigned char *output)
{
#if 1
	unsigned int i;
	R_RSA_PUBLIC_KEY *publicKey;

	publicKey = (R_RSA_PUBLIC_KEY *)emvbase_malloc(sizeof(R_RSA_PUBLIC_KEY));
	
	memset((unsigned char*)publicKey,0,sizeof(R_RSA_PUBLIC_KEY));
	publicKey->bits = mLen*8;

	
	if(eLen == 0x01)
		publicKey->exponent[MAX_RSA_MODULUS_LEN-1] = *e;	
	else if(eLen == 0x03)
	{
		if(memcmp(e,"\x00\x00\x03",3) == 0)
		{
			publicKey->exponent[MAX_RSA_MODULUS_LEN-1] = 0x03;
		}
		else
		{
			publicKey->exponent[MAX_RSA_MODULUS_LEN-1] = *e;
			publicKey->exponent[MAX_RSA_MODULUS_LEN-2] = *(e+1);
			publicKey->exponent[MAX_RSA_MODULUS_LEN-3] = *(e+2);
		}
	}
	else 
	{
	    emvbase_free(publicKey);
		return 1;
	}
	
	memcpy((unsigned char *)&publicKey->modulus[MAX_RSA_MODULUS_LEN-mLen],&m[0],mLen);
    
	
	i = EMVBase_RSAPublicDecrypt(output,input,mLen,publicKey);

	
	emvbase_free(publicKey);
	
	return i;
#else

    unsigned char tempe[4] = {0};
    unsigned char *pk,*data;
    unsigned int len;
    
    if(eLen == 0x01)
        tempe[3] = *e;	
    else if(eLen == 0x03)
    {
        if(memcmp(e,"\x00\x00\x03",3) == 0)
        {
        tempe[3] = 0x03;	
        }
        else
        {
            tempe[1] = *e;
            tempe[2] = *(e+1);
            tempe[3] = *(e+2);
        }
    }
    else
        return 1;

    if(mLen % 4 == 0)
    {
        ddi_rsa_pk_decrypt(m, mLen, tempe, 4,input,output);
        return 0;
    }
    else
    {
        len = (mLen /4) * 4 + 4;
        pk = emvbase_malloc(len);
        if(pk == NULL)
            return 1;
        data = emvbase_malloc(len);
        if(data == NULL)
        {
            emvbase_free(pk);
            return 1;
        }
        memset(pk,0,len);
        memcpy(pk + len - mLen,m,mLen);
        memset(data,0,len);
        memcpy(data + len - mLen,input,mLen);
        ddi_rsa_pk_decrypt(pk, len, tempe, 4,data,data);
        memcpy(output,data + len - mLen,mLen);
        emvbase_free(pk);
        emvbase_free(data);
        return 0;
    }
#endif       
}



#define SHA1HANDSOFF

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#ifndef WORDS_BIGENDIAN
#define blk0(i) (block->l[i] = (rol(block->l[i], 24) & 0xFF00FF00) |(rol(block->l[i], 8) & 0x00FF00FF))
#else
#define blk0(i) block->l[i]
#endif
#define blk(i) (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ block->l[(i + 8) & 15] ^ block->l[(i + 2) & 15] ^ block->l[i & 15], 1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z += ((w & (x ^ y)) ^ y) + blk0(i) + 0x5A827999 + rol(v, 5);w = rol(w, 30);
#define R1(v,w,x,y,z,i) z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); w = rol(w, 30);
#define R2(v,w,x,y,z,i) z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); w = rol(w, 30);
#define R3(v,w,x,y,z,i) z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5);w = rol(w, 30);
#define R4(v,w,x,y,z,i) z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5);w=rol(w, 30);


/* Hash a single 512-bit block. This is the core of the algorithm. */

static void  EMVBase_SHA1Transform(unsigned int state[5], unsigned char buffer[64])
{
        unsigned int a, b, c, d, e;        
        unsigned int workspace[16];
        
        typedef union {
                unsigned char c[64];
                unsigned int l[16];
        } CHAR64LONG16;
        CHAR64LONG16* block;
//#ifdef SHA1HANDSOFF
        
        block = (CHAR64LONG16 *) workspace;
        memcpy(block, buffer, 64);
/*#else
        block = (CHAR64LONG16 *) buffer;
#endif
  */      /* Copy context->state[] to working vars */
        a = state[0];
        b = state[1];
        c = state[2];
        d = state[3];
        e = state[4];
        /* 4 rounds of 20 operations each. Loop unrolled. */
        R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
        R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
        R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
        R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
        R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
        R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
        R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
        R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
        R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
        R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
        R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
        R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
        R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
        R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
        R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
        R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
        R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
        R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
        R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
        R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);
        /* Add the working vars back into context.state[] */
        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
        /* Wipe variables */
        a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
        memset(block, 0, 64);
#endif
}


/* SHA1Init - Initialize new context */

static void  EMVBase_SHA1Init(SHA1_CTX* context)
{
        /* SHA1 initialization constants */
        context->state[0] = 0x67452301;
        context->state[1] = 0xEFCDAB89;
        context->state[2] = 0x98BADCFE;
        context->state[3] = 0x10325476;
        context->state[4] = 0xC3D2E1F0;
        context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

static void  EMVBase_SHA1Update(SHA1_CTX* context, unsigned char *datax, unsigned int len)
{
        unsigned int i, j;
        unsigned char *data1 = datax;

        j = (context->count[0] >> 3) & 63;
        if ((context->count[0] += len << 3) < (len << 3))
                (context->count[1])++;
        context->count[1] += (len >> 29);
        if ((j + len) > 63) {
                memcpy(&context->buffer[j], data1, (i = 64-j));
                EMVBase_SHA1Transform(context->state, context->buffer);
                for ( ; i + 63 < len; i += 64) {
                        EMVBase_SHA1Transform(context->state, &data1[i]);
                }
                j = 0;
        }
        else i = 0;
        memcpy(&context->buffer[j], &data1[i], len - i);
}


/* Add padding and return the message digest. */

static void  EMVBase_SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
        unsigned int i;
        unsigned char finalcount[8];

        for (i = 0; i < 8; i++) {
                finalcount[i] = (unsigned char)
                        ((context->count[(i >= 4 ? 0 : 1)] >>
                          ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
        }
        EMVBase_SHA1Update(context, (unsigned char *) "\200", 1);
        while ((context->count[0] & 504) != 448) {
                EMVBase_SHA1Update(context, (unsigned char *) "\0", 1);
        }
        EMVBase_SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform()
                                              */
        for (i = 0; i < 20; i++) {
                digest[i] = (unsigned char)
                        ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) &
                         255);
        }
        /* Wipe variables */
        i = 0;
        memset(context->buffer, 0, 64);
        memset(context->state, 0, 20);
        memset(context->count, 0, 8);
        memset(finalcount, 0, 8);
}


#define	SHA1Context			SHA1_CTX
#define EMVBase_SHA1Reset			EMVBase_SHA1Init
#define EMVBase_SHA1Input			EMVBase_SHA1Update
#define EMVBase_SHA1Result(a, b)	EMVBase_SHA1Final(b, a)

void EMVBase_Hash(unsigned char* bb,unsigned int len,unsigned char* Message_Digest)
{
	SHA1Context sha;
	EMVBase_SHA1Reset(&sha);
	EMVBase_SHA1Input(&sha, bb, len);
	EMVBase_SHA1Result(&sha, Message_Digest);
}

