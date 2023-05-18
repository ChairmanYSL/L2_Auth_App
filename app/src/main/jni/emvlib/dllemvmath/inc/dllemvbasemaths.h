#ifndef EMVMATH_H
#define  EMVMATH_H

extern void EMVBaseMath_GetLibVersion(unsigned char *version);
extern void EMVBaseBcdToAsc(unsigned char *Dest,unsigned char *Src,unsigned short Len);
extern void EMVBaseAscToBcd(unsigned char *pbcDest, const unsigned char *pasSrc, unsigned short siSrclen);
extern void EMVBaseBcdToU32(unsigned int  *puiDest, const unsigned char *pbcSrc, unsigned short Len);
extern void EMVBaseU32ToBcd(unsigned char *Bcd, unsigned int const Src,unsigned char Len);
extern void EMVBaseU16ToBcd(unsigned char *Bcd, unsigned short const Src,unsigned char Len);
extern void EMVBaseBcdToU16(unsigned short *Dest,unsigned char *Bcd,unsigned char Len);
extern unsigned char EMVBaseStrPos(unsigned char *Src, unsigned char chr);
extern unsigned long EMVBaseByteArrayToInt(unsigned char* buf,unsigned char bufLen);
extern void EMVBaseIntToByteArray(unsigned long var,unsigned char* buf,unsigned char bufLen);
extern void EMVBaseBcdAdd(unsigned char *Dest,unsigned char *Src,unsigned char Len);
extern void EMVBaseBcdSub(unsigned char *Dest,unsigned char *Src,unsigned char Len);
extern unsigned char EMVBaseCalcBcc(unsigned char *src,unsigned int num );

extern void EMVBase_Hash(unsigned char* bb,unsigned long len,unsigned char* Message_Digest);
extern unsigned char EMVBase_RSARecover(unsigned char *m, unsigned int mLen, unsigned char *e, unsigned int eLen,unsigned char *input,unsigned char *output);

extern unsigned short EMVBaseTlvLen(unsigned char *aTLV);
extern unsigned short EMVBaseTlvTSize(unsigned char *aTLV);
extern unsigned char *EMVBaseTlvSeek(unsigned char *aTlvList, unsigned short aLen, unsigned int aTag);
extern unsigned short EMVBaseTlvSeekSame(unsigned char *aTlvList, unsigned short aLen, unsigned long aTag, unsigned char *pout);
extern unsigned short EMVBaseTlvSizeOf(unsigned char *aTLV);
extern unsigned int EMVBaseTlvTag(unsigned char *aTLV);
extern unsigned short EMVBaseTlvTSize(unsigned char *aTLV);
extern unsigned char *EMVBaseTlvVPtr(unsigned char *aTLV);

#endif

