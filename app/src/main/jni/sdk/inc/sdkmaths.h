#ifndef SDK_MATHS_H
#define SDK_MATHS_H

#include "sdktypedef.h"

typedef enum
{
    SDK_DES = 0x01,                              //DES
    SDK_3DES = 0x02                             //3DES
}SDK_ENCRYPT_MODE;                               //加密类型//encrypt mode

typedef enum
{
    SDK_DECRYPT = 0,                                                   // 解密//decrypt
    SDK_ENCRYPT = 1                                                    //加密//encrypt
}SDK_ENC_DES_MODE;

#define SDK_MAX_RSA_MODULUS_LEN  256ul
#define SDK_MAX_RSA_PRIME_LEN   128ul

/* Random structure. */
typedef struct
{
    unsigned int bytesNeeded;
    unsigned char state[16];
    unsigned int outputAvailable;
    unsigned char output[16];
} SDK_RANDOM_STRUCT;

/* RSA public and private key. */
typedef struct
{
    u32 bits;                                     /* length in bits of modulus */
    u8 modulus[SDK_MAX_RSA_MODULUS_LEN];          /* modulus */
    u8 exponent[SDK_MAX_RSA_MODULUS_LEN];         /* public exponent */
} SDK_RSA_PUBLIC_KEY;

typedef struct
{
    unsigned int bits;                                      /* length in bits of modulus */
    unsigned char modulus[SDK_MAX_RSA_MODULUS_LEN];         /* modulus */
    unsigned char publicExponent[SDK_MAX_RSA_MODULUS_LEN];  /* public exponent */
    unsigned char exponent[SDK_MAX_RSA_MODULUS_LEN];        /* private exponent */
    unsigned char prime[2][SDK_MAX_RSA_PRIME_LEN];          /* prime factors */
    unsigned char primeExponent[2][SDK_MAX_RSA_PRIME_LEN];  /* exponents for CRT */
    unsigned char coefficient[SDK_MAX_RSA_PRIME_LEN];       /* CRT coefficient */
} SDK_RSA_PRIVATE_KEY;

extern s32 sdkDesS(bool bEncrypt, u8* pheDatat, const u8* phekeyt);
extern s32 sdkDes3S(bool bEncrypt, u8* pheDatat, const u8* phekeyt);
extern s32 sdkMD5(u8 *pheDest,  const u8 *hepSrc, s32 silen);
extern s32 sdkSHA1(const u8* pheSrc, s32 siLen, u8* pheDest);
extern s32 sdkSM2InitElliptic(const u8 *pGroupP, const u8 *pGroupA, const u8 *pGroupB, const u8 *pGroupX,const u8 *pGroupY, const u8 *pGroupN);
extern s32 sdkSM3Encrypt(u8 *pSrcData, s32 siLen, u8 *pDestData);//shijianglong 2013.05.30 16:15
extern s32 sdkSM2CheckSign(const u8 *pPkeyX, const u8 *pPkeyY, const u8 *pSignR, const u8 *pSignS,const u8 *pSrcData, const u8 *pIDA);
extern s32 sdkSM2ClearAll(void);
extern s32 sdkRSAPublicEncrypt(u8 *pheDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen, const SDK_RSA_PUBLIC_KEY *pstPublicKey, const SDK_RANDOM_STRUCT*pstRandomStruct);
extern s32 sdkRSAPrivateEncrypt(u8 *pheDest, s32 *psiDestlen, const u8 *pucSrc, s32 siSrclen, const SDK_RSA_PRIVATE_KEY *pstPrivateKey);
extern s32 sdkRSAPublicDecrypt(u8 *pucDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen, const SDK_RSA_PUBLIC_KEY *pstPublicKey);
extern s32 sdkRSAPrivateDecrypt(u8 *pheDest, s32 *psiDestlen, const u8 *pucSrc, s32 siSrclen, const SDK_RSA_PRIVATE_KEY *pstPrivateKey);

extern s32 sdkAscToBcd(u8 *pbcDest, const u8 *pasSrc, s32 siSrclen);
extern bool sdkIsAscii(const u8 *pasSrc);
extern s32 sdkU8ToBcd(u8 *pbcDest, u8 const ucSrc, s32 siDestlen);
extern s32 sdkBcdToAsc(u8 *pasDest, const u8 *pbcSrc, s32 siBcdSrclen);
extern s32 sdkU16ToAsc(u16 const usSrc, u8 *pasDest);
extern s32 sdkU32ToHex(u8 *pheDest, u32 const uiSrc, s32 siHexlen);
extern s32 sdkU32ToAsc(u32 const uiSrc, u8 *pasDest);
extern s32 sdkU8ToAsc(u8 const ucSrc, u8 *pasDest);
extern s32 sdkAscToBcdR(u8 *pbcDest, const u8 *pasSrc, s32 siBcdlen);
extern s32 sdkBcdToU16(u16 *pusDest, const u8 *pbcSrc, s32 siSrclen);
extern s32 sdkU16ToBcd(u8 *pbcDest, u16 const usSrc, s32 siDestlen);
extern s32 sdkXOR8(u8 *pheDest, const u8 *pheSrc1, const u8 *pheSrc2);
extern s32 sdkEcb(u8 *pheDest, const u8 *pheSrc, s32 siLen);
extern s32 sdkU32ToBcd(u8 *pbcDest, u32 const uiSrc, s32 siDestlen);
extern s32 sdkBcdToU8(u8 *pucDest, const u8 *pbcSrc, s32 siSrclen);
extern s32 sdkBcdToU32(u32 *puiDest, const u8 *pbcSrc, s32 siSrclen);
extern s32 sdkHexToU16(u16 *pusDest, const u8 *pheSrc, s32 siHexlen);
extern s32 sdkHexToU32(u32 *puiDest, const u8 *pheSrc, s32 siHexlen);
extern s32 sdkU16ToHex(u8 *pheDest, u16 const usSrc, s32 siHexlen);
extern s32 sdkBcdToFloat(double *pDest, const u8 *pbcSrc, s32 siBcdSrclen);
extern s32 sdkBcdCardNumToAsc(u8 *pasDest, const u8 *pbcSrc, s32 siBcdSrclen);
extern s32 sdkBcdAdd(u8 *pbcDest, const u8 *pbcSrc1, s32 siBcdSrc1len, const u8 *pbcSrc2, s32 siBcdSrc2len);
extern s32 sdkBcdSub(u8 *pbcDest, const u8 *pbcSrc1, s32 siBcdSrc1len, const u8 *pbcSrc2, s32 siBcdSrc2len);
extern s32 sdkCalcCUSUM(const u8 *pucSrc, s32 siNum);
extern s32 sdkCalcCrc16(const u8 *pucSrc, s32 siSrclen);
extern s32 sdkE90S(u8 *pheDest, const u8 *pheSrc, s32 siLen, const u8 *pheWk, SDK_ENCRYPT_MODE eMode);
extern s32 sdkE98S(u8 *pheDest, const u8 *phePin, const u8 *phePan, const u8 *pheWk,  SDK_ENCRYPT_MODE eMode);
extern s32 sdkE99S(u8 *pheDest, const u8 *pheSrc, s32 siLen, const u8 *pheWk, SDK_ENCRYPT_MODE eMode);
extern s32 sdkE919S(u8 *pheDest, const u8 *pheSrc, s32 siLen, const u8 *pheWk, SDK_ENCRYPT_MODE eMode);
extern s32 sdkIDEA(SDK_ENC_DES_MODE Mode, u8* pKey, const s32 siKeyLen, const u8* pInData, s32 nLen, u8* OutData);


#endif
