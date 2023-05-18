#include "sdkDebug.h"
#include "sdktypedef.h"
#include "sha1.h"

const u16 CRCTAB[] =
{       //CRC-16 x16+x15+x2+1 (美国二进制) //国标 高位在前
    0x0000, 0x8005, 0x800f, 0x000a, 0x801b, 0x001e, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003c, 0x8039, 0x0028, 0x802d, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006c, 0x8069, 0x0078, 0x807d, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805f, 0x005a, 0x804b, 0x004e, 0x0044, 0x8041,
    0x80c3, 0x00c6, 0x00cc, 0x80c9, 0x00d8, 0x80dd, 0x80d7, 0x00d2,
    0x00f0, 0x80f5, 0x80ff, 0x00fa, 0x80eb, 0x00ee, 0x00e4, 0x80e1,
    0x00a0, 0x80a5, 0x80af, 0x00aa, 0x80bb, 0x00be, 0x00b4, 0x80b1,
    0x8093, 0x0096, 0x009c, 0x8099, 0x0088, 0x808d, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018c, 0x8189, 0x0198, 0x819d, 0x8197, 0x0192,
    0x01b0, 0x81b5, 0x81bf, 0x01ba, 0x81ab, 0x01ae, 0x01a4, 0x81a1,
    0x01e0, 0x81e5, 0x81ef, 0x01ea, 0x81fb, 0x01fe, 0x01f4, 0x81f1,
    0x81d3, 0x01d6, 0x01dc, 0x81d9, 0x01c8, 0x81cd, 0x81c7, 0x01c2,
    0x0140, 0x8145, 0x814f, 0x014a, 0x815b, 0x015e, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017c, 0x8179, 0x0168, 0x816d, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012c, 0x8129, 0x0138, 0x813d, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811f, 0x011a, 0x810b, 0x010e, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030c, 0x8309, 0x0318, 0x831d, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833f, 0x033a, 0x832b, 0x032e, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836f, 0x036a, 0x837b, 0x037e, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035c, 0x8359, 0x0348, 0x834d, 0x8347, 0x0342,
    0x03c0, 0x83c5, 0x83cf, 0x03ca, 0x83db, 0x03de, 0x03d4, 0x83d1,
    0x83f3, 0x03f6, 0x03fc, 0x83f9, 0x03e8, 0x83ed, 0x83e7, 0x03e2,
    0x83a3, 0x03a6, 0x03ac, 0x83a9, 0x03b8, 0x83bd, 0x83b7, 0x03b2,
    0x0390, 0x8395, 0x839f, 0x039a, 0x838b, 0x038e, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828f, 0x028a, 0x829b, 0x029e, 0x0294, 0x8291,
    0x82b3, 0x02b6, 0x02bc, 0x82b9, 0x02a8, 0x82ad, 0x82a7, 0x02a2,
    0x82e3, 0x02e6, 0x02ec, 0x82e9, 0x02f8, 0x82fd, 0x82f7, 0x02f2,
    0x02d0, 0x82d5, 0x82df, 0x02da, 0x82cb, 0x02ce, 0x02c4, 0x82c1,
    0x8243, 0x0246, 0x024c, 0x8249, 0x0258, 0x825d, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827f, 0x027a, 0x826b, 0x026e, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822f, 0x022a, 0x823b, 0x023e, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021c, 0x8219, 0x0208, 0x820d, 0x8207, 0x0202
};



s32 sdkCalcCUSUM(const u8 *pucSrc, s32 siNum)
{
    s32 i = 0;                                                                                    //临时个数
    u8 bcc = 0;                                                 //累加和

    if((NULL == pucSrc) || (siNum < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siNum; i++)
    {
        bcc += pucSrc[i];                                                                               //计算校验和
    }

    return bcc;                                                                                     //返回校验和
}

s32 sdkAscToBcd(u8 *pbcDest, const u8 *pasSrc, s32 siSrclen)
{
    s32 i = 0;
    u8 high = 0, low = 0;

    if((NULL == pasSrc) || (NULL == pbcDest) || (siSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siSrclen; i++)
    {
        if((pasSrc[i] < '0') || ((pasSrc[i] > '9') && (pasSrc[i] < 'A')) || ((pasSrc[i] > 'F') && (pasSrc[i] < 'a')) || (pasSrc[i] > 'f'))
        {
            return SDK_PARA_ERR;
        }
    }

    for(i = 0; i < siSrclen; i++)
    {
        //待转bcd码高Nibble
        if((*(pasSrc + i) >= 0x61) && (*(pasSrc + i) <= 0x66))          //range a~f
        {
            high = *(pasSrc + i) - 0x57;
        }
        else if((*(pasSrc + i) >= 0x41) && (*(pasSrc + i) <= 0x46))      //range A~F
        {
            high = *(pasSrc + i) - 0x37;
        }
        else if((*(pasSrc + i) >= 0x30) && (*(pasSrc + i) <= 0x39))      //range 0~9
        {
            high = *(pasSrc + i) - 0x30;
        }
        else
        {
            high = 0x00;                                        //其他
        }
        //待转bcd码低Nibble
        i++;

        if(i < siSrclen)
        {
            if((*(pasSrc + i) >= 0x61) && (*(pasSrc + i) <= 0x66))        //range a~f
            {
                low = *(pasSrc + i) - 0x57;
            }
            else if((*(pasSrc + i) >= 0x41) && (*(pasSrc + i) <= 0x46)) //range A~F
            {
                low = *(pasSrc + i) - 0x37;
            }
            else if((*(pasSrc + i) >= 0x30) && (*(pasSrc + i) <= 0x39))      //range 0~9
            {
                low = *(pasSrc + i) - 0x30;
            }
            else
            {
                low = 0x00;                                            //其他
            }
        }
        else
        {
            i--;                                                    //预防255个时溢出出错
            low = 0x00;                                            //如果是奇数个末尾补0x00
        }
        *(pbcDest + i / 2) = (high << 4) | low;                      //合并BCD码
    }

    return (siSrclen + 1) / 2;
}

bool sdkIsAscii(const u8 *pasSrc)
{
    u16 len, i;
    bool flag = true;

    if(NULL == pasSrc || strlen(pasSrc) == 0)
    {
        Assert(0);
        return false;
    }
    len = strlen(pasSrc);

    for(i = 0; i < len; i++)
    {
        if(pasSrc[i] > 0x80)
        {
            flag = false;
            break;
        }
    }

    return flag;
}

s32 sdkU8ToBcd(u8 *pbcDest, u8 const ucSrc, s32 siDestlen)
{
    u32 i = 0;
    u8 num = ucSrc;

    if((NULL == pbcDest) || (siDestlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = siDestlen; i > 0; i--)
    {
        *(pbcDest + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }

    return siDestlen;
}

s32 sdkBcdToAsc(u8 *pasDest, const u8 *pbcSrc, s32 siBcdSrclen)
{
    s32 i = 0;

    if((NULL == pbcSrc) || (NULL == pasDest) || (siBcdSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siBcdSrclen; i++)
    {
        //高Nibble转换
        if(((*(pbcSrc + i) & 0xF0) >> 4) <= 9)
        {
            *(pasDest + 2 * i) = ((*(pbcSrc + i) >> 4) & 0X0F) + 0x30;
        }
        else
        {
            *(pasDest + 2 * i)  = ((*(pbcSrc + i) >> 4) & 0x0F) + 0x37;   //大写A~F
        }

        //低Nibble转换
        if((*(pbcSrc + i) & 0x0F) <= 9)
        {
            *(pasDest + 2 * i + 1) = (*(pbcSrc + i) & 0x0F) + 0x30;
        }
        else
        {
            *(pasDest + 2 * i + 1) = (*(pbcSrc + i) & 0x0F) + 0x37;   //大写A~F
        }
    }

    return siBcdSrclen * 2;
}

s32 sdkU16ToAsc(u16 const usSrc, u8 *pasDest)
{
    u8 pOut[10];
    u32 len;

    if(pasDest == NULL)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len  = (u32)sprintf((char *)pOut, "%d", usSrc);
    Assert(len + 1 <= sizeof(pOut));
    memcpy(pasDest, pOut, (u32)len);
    return len;
}

s32 sdkU32ToHex(u8 *pheDest, u32 const uiSrc, s32 siHexlen)
{
    u32 i = 0;
    u32 num = uiSrc;

    if((NULL == pheDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = siHexlen; i > 0; i--)
    {
        *(pheDest + i - 1) = num % 256;
        num /= 256;
    }

    return siHexlen;
}

s32 sdkU32ToAsc(u32 const uiSrc, u8 *pasDest)
{
    u8 pOut[10];
    u32 len;

    if(pasDest == NULL)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len  = (u32)sprintf((char *)pOut, "%d", (s32)uiSrc);
    Assert(len + 1 <= sizeof(pOut));
    memcpy(pasDest, pOut, (u32)len);
    return len;
}

s32 sdkU8ToAsc(u8 const ucSrc, u8 *pasDest)
{
    u8 pOut[10];
    u32 len;

    if(pasDest == NULL)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len  = (u32)sprintf((char *)pOut, "%d", ucSrc);
    Assert(len + 1 <= sizeof(pOut));
    memcpy(pasDest, pOut, (u32)len);
    return len;
}

s32 sdkAscToBcdR(u8 *pbcDest, const u8 *pasSrc, s32 siBcdlen)
{
    u8 tmp[3];
    s32 ui_i, ui_j, ui_k;

    if((NULL == pasSrc) || (NULL == pbcDest) || (siBcdlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ui_i = (u32)strlen(pasSrc);                      //获取传入字符串长度

    for(ui_j = 0; ui_j < ui_i; ui_j++)              //非字符串报错
    {
        if((pasSrc[ui_j] < '0') || ((pasSrc[ui_j] > 'F') && (pasSrc[ui_j] < 'a')) || (pasSrc[ui_j] > 'f'))
        {
            return SDK_PARA_ERR;
        }
    }

    memset(pbcDest, 0, siBcdlen);

    if(ui_i > siBcdlen * 2)                                             //截取
    {
        for(ui_j = 0, ui_k = 0; ui_j < siBcdlen; ) //
        {
            if(ui_k >= siBcdlen * 2)
            {
                break;
            }
            Verify(sdkAscToBcd(&pbcDest[ui_j], &pasSrc[(ui_i - siBcdlen * 2) + ui_k], 2) > 0);
            ui_j = ui_j + 1;
            ui_k = ui_k + 2;
        }
    }
    else if(ui_i == siBcdlen * 2)
    {
        Verify(sdkAscToBcd(pbcDest, pasSrc, siBcdlen * 2) > 0);
    }
    else
    {
        if(ui_i % 2 == 0)                               //
        {
            for(ui_j = 0, ui_k = 0; ui_j < siBcdlen; ) //,ui_k >= ui_i
            {
                if(ui_k >= ui_i)
                {
                    break;
                }
                Verify(sdkAscToBcd(&pbcDest[(siBcdlen - ui_i / 2) + ui_j], &pasSrc[ui_k], 2) > 0);
                ui_j = ui_j + 1;
                ui_k = ui_k + 2;
            }
        }
        else                                            //奇数第一个前面补齐一个0x30
        {
            memset(tmp, 0x30, 2);
            tmp[2] = 0;
            memcpy(&tmp[1], &pasSrc[0], 1);
            Verify(sdkAscToBcd(&pbcDest[(siBcdlen - (ui_i + 1) / 2)], tmp, 2) > 0);

            for(ui_j = 1, ui_k = 1; ui_j < siBcdlen; ) //,ui_k >= ui_i
            {
                if(ui_k >= ui_i)
                {
                    break;
                }
                Verify(sdkAscToBcd(&pbcDest[(siBcdlen - (ui_i + 1) / 2) + ui_j], &pasSrc[ui_k], 2) > 0);
                ui_j = ui_j + 1;
                ui_k = ui_k + 2;
            }
        }
    }
    return siBcdlen;
}

s32 sdkBcdToU16(u16 *pusDest, const u8 *pbcSrc, s32 siSrclen)
{
    s32 i = 0;
    u32 temp = 0;

    if((NULL == pbcSrc) || (NULL == pusDest) || (siSrclen <= 0) || (siSrclen > 3))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *pusDest = 0;
    temp = 0;

    for(i = 0; i < siSrclen; i++)
    {
        temp *= 100;
        temp += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    *pusDest = (u16)temp;

    if(temp > 65535)
    {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

s32 sdkU16ToBcd(u8 *pbcDest, u16 const usSrc, s32 uiDestlen)
{
    u32 i = 0;
    u16 num = usSrc;

    if((NULL == pbcDest) || (uiDestlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = uiDestlen; i > 0; i--)
    {
        *(pbcDest + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }

    return uiDestlen;
}

s32 sdkXOR8(u8 *pheDest, const u8 *pheSrc1, const u8 *pheSrc2)
{
    s32 i = 0;

    if((NULL == pheSrc1) || (NULL == pheSrc2) || (NULL == pheDest))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = 0; i < 8; i++)
    {
        *(pheDest + i) = *(pheSrc1 + i) ^ *(pheSrc2 + i);
    }

    return SDK_OK;
}

s32 sdkCalcCrc16(const u8 *pucSrc, s32 siSrclen)
{
    u16 crcdata = 0;

    if((NULL == pucSrc) || (siSrclen <= 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    while(siSrclen > 0)
    {
        crcdata = (crcdata << 8) ^ CRCTAB[(crcdata >> 8) ^ (*pucSrc)];
        siSrclen--;
        pucSrc++;
    }

    return crcdata;
}

s32 sdkU32ToBcd(u8 *pbcDest, u32 const uiSrc, s32 siDestlen)
{
    u32 i = 0;
    u32 num = uiSrc;

    if((NULL == pbcDest) || (siDestlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = siDestlen; i > 0; i--)
    {
        *(pbcDest + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }

    return siDestlen;
}

s32 sdkBcdToU8(u8 *pucDest, const u8 *pbcSrc, s32 siSrclen)
{
    s32 i = 0;
    u32 sum = 0;

    if((NULL == pbcSrc) || (NULL == pucDest) || (siSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siSrclen; i++)
    {
        sum *= 100;
        sum += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    *pucDest = (u8)sum;

    if(sum > 255)
    {
        //  Assert(0);//fusuipu 2013.01.05 14:36
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

s32 sdkBcdToU32(u32 *puiDest, const u8 *pbcSrc, s32 siSrclen)
{
    s32 i = 0;

    if((NULL == pbcSrc) || (NULL == puiDest) || (siSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *puiDest = 0;

    for(i = 0; i < siSrclen; i++)
    {
        *puiDest *= 100;
        *puiDest += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    if(siSrclen >= 5 && pbcSrc[0] > 0x42)
    {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

s32 sdkHexToU16(u16 *pusDest, const u8 *pheSrc, s32 siHexlen)
{
    s32 i = 0;
    u32 temp = 0;

    if((NULL == pheSrc) || (NULL == pusDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siHexlen; i++)
    {
        temp *= 256;
        temp += pheSrc[i];
    }

    *pusDest = (u16)temp;

    if(temp > 65535)
    {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

s32 sdkHexToU32(u32 *puiDest, const u8 *pheSrc, s32 siHexlen)
{
    s32 i = 0;

    if((NULL == pheSrc) || (NULL == puiDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *puiDest = 0;

    for(i = 0; i < siHexlen; i++)
    {
        *puiDest *= 256;
        *puiDest += pheSrc[i];
    }

    return SDK_OK;
}

s32 sdkU16ToHex(u8 *pheDest, u16 const usSrc, s32 siHexlen)
{
    u32 i = 0;
    u16 num = usSrc;

    if((NULL == pheDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = siHexlen; i > 0; i--)
    {
        *(pheDest + i - 1) = num % 256;
        num /= 256;
    }

    return siHexlen;
}

s32 sdkBcdToFloat(double *pDest, const u8 *pbcSrc, s32 siBcdSrclen)
{
    s32 i;

    if((NULL == pbcSrc) || (NULL == pDest) || (siBcdSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *pDest = 0;

    for(i = 0; i < siBcdSrclen; i++)
    {
        *pDest *= 100;
        *pDest += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    return SDK_OK;
}

s32 sdkBcdCardNumToAsc(u8 *pasDest, const u8 *pbcSrc, s32 siBcdSrclen)
{
    s32 i;

    if((NULL == pbcSrc) || (NULL == pasDest) || (siBcdSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Verify(sdkBcdToAsc(pasDest, pbcSrc, siBcdSrclen) >= 0);

    for(i = 0; i < siBcdSrclen * 2; i++)
    {
        if((pasDest[i]) == 0X44)
        {
            *(pasDest + i) = 0x3d;
        }
    }

    return siBcdSrclen * 2;
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

static void  SHA1Transform(unsigned long state[5], unsigned char buffer[64])
{
        unsigned long a, b, c, d, e;
        unsigned long workspace[16];

        typedef union {
                unsigned char c[64];
                unsigned long l[16];
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

static void  SHA1Init(SHA1_CTX* context)
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

static void  SHA1Update(SHA1_CTX* context, unsigned char *datax, unsigned long len)
{
        unsigned long i, j;
        unsigned char *data1 = datax;

        j = (context->count[0] >> 3) & 63;
        if ((context->count[0] += len << 3) < (len << 3))
                (context->count[1])++;
        context->count[1] += (len >> 29);
        if ((j + len) > 63) {
                memcpy(&context->buffer[j], data1, (i = 64-j));
                SHA1Transform(context->state, context->buffer);
                for ( ; i + 63 < len; i += 64) {
                        SHA1Transform(context->state, &data1[i]);
                }
                j = 0;
        }
        else i = 0;
        memcpy(&context->buffer[j], &data1[i], len - i);
}


/* Add padding and return the message digest. */

static void  SHA1Final(unsigned char digest[20], SHA1_CTX* context)
{
        unsigned long i;
        unsigned char finalcount[8];

        for (i = 0; i < 8; i++) {
                finalcount[i] = (unsigned char)
                        ((context->count[(i >= 4 ? 0 : 1)] >>
                          ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
        }
        SHA1Update(context, (unsigned char *) "\200", 1);
        while ((context->count[0] & 504) != 448) {
                SHA1Update(context, (unsigned char *) "\0", 1);
        }
        SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform()
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
#define SHA1Reset			SHA1Init
#define SHA1Input			SHA1Update
#define SHA1Result(a, b)	SHA1Final(b, a)

static void Hash(unsigned char* bb,unsigned long len,unsigned char* Message_Digest)
{
		SHA1Context sha;
		SHA1Reset(&sha);
    	SHA1Input(&sha, bb, len);
    	SHA1Result(&sha, Message_Digest);
}


s32 sdkSHA1(const u8* pheSrc, s32 siLen, u8* pheDest)
{
    if(NULL == pheSrc || NULL == pheDest || siLen <=  0) //石玮松 2012.08.20 11:32
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Hash((u8*)pheSrc, siLen, (u8*)pheDest);

    return SDK_OK;
}

void BcdAdd(u8 *Dest, u8 *Src, u32 Len)
{
    u32 i;
    u8 high, low;
    u8 carry = 0;                     //运算进位标志

    for(i = Len; i > 0; i--)
    {
        //低nibble的运算
        low = (*(Dest + i - 1) & 0x0F) + (*(Src + i - 1) & 0x0F) + carry;
        carry = 0;

        if(low > 0x09)
        {
            low = low - 0x0A;
            carry = 1;
        }
        //高nibble的运算
        high  = ( (*(Dest + i - 1) & 0xF0) >> 4) + ((*(Src + i - 1) & 0xF0) >> 4) + carry;
        carry = 0;

        if(high > 0x09)
        {
            high = high - 0x0A;
            carry = 1;
        }
        *(Dest + i - 1) = (high << 4) | low;
    }
}

bool sdkIsBcdNum(u8 const *pheSrc, s32 siSrclen)
{
    if(NULL == pheSrc || siSrclen < 0)
    {
        Assert(0);
        return false;
    }

    while (siSrclen--)
    {
        if(((pheSrc[siSrclen] & 0x0F) > 0x09) || ( (pheSrc[siSrclen] & 0xF0) > 0x90))
        {
            return false;
        }
    }

    return true;
}

s32 sdkBcdAdd(u8 *pbcDest, const u8 *pbcSrc1, s32 siBcdSrc1len, const u8 *pbcSrc2, s32 siBcdSrc2len)
{
    u32 len = siBcdSrc2len > siBcdSrc1len ? siBcdSrc2len : siBcdSrc1len;

    if((NULL == pbcSrc1) || (NULL == pbcSrc2) || (NULL == pbcDest) || (siBcdSrc1len < 0) || (siBcdSrc2len < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if(!sdkIsBcdNum(pbcSrc2, siBcdSrc2len))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if(!sdkIsBcdNum(pbcSrc1, siBcdSrc1len))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    /*=======BEGIN: fusuipu 2013.05.13  14:34 modify===========*/
    if(len == 0)
    {
        return len;
    }
    /*====================== END======================== */
    {
        u8 *pTemp = sdkGetMem(len);

        if(pTemp == NULL)
        {
            Assert(0);
            return SDK_ERR;
        }
        memset(pTemp, 0, len);

        if(siBcdSrc2len > siBcdSrc1len)
        {
            memcpy(&pTemp[siBcdSrc2len - siBcdSrc1len], pbcSrc1, (u32)siBcdSrc1len);
            BcdAdd(pTemp, ( u8 *)pbcSrc2, len);
        }
        else
        {
            memcpy(&pTemp[siBcdSrc1len - siBcdSrc2len], pbcSrc2, siBcdSrc2len);
            BcdAdd(pTemp, ( u8 *)pbcSrc1, len);
        }
//   BcdAdd(pTemp, ( u8 *)pbcSrc2 , uiBcdSrc2len);
        memcpy(pbcDest, pTemp, len);

        sdkFreeMem(pTemp);
    }
    return len;
}
