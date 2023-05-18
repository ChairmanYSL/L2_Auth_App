#include "sdkDebug.h"
#include "sdktypedef.h"

static unsigned char const Sfun_Tab[8][64] = { //  S-1
    {
        14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
        0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
        4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
    },
    //  S-2
    {
        15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
        3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
        0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
    },
    //  S-3
    {
        10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
        1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
    },
    //  S-4
    {
        7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
        3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
    },
    //  S-5
    {
        2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
        4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
    },
    //  S-6
    {
        12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
        9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
        4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
    },
    //  S-7
    {
        4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
        1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
        6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
    },
    //  S-8
    {
        13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
        1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
        7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
        2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
    }
};


/**************************************************************************
*                                                                        *
*                     DES                                                *
*                                                                        *
**************************************************************************/
static void DoFirstChange(unsigned char *s)
{
    unsigned char i, j;
    unsigned char t[8];

    for (i = 0; i < 8; ++i)
    {
        t[i] = 0;
    }

    for (i = 0; i < 8; ++i)
    {
        for (j = 0; j < 8; ++j)
        {
            t[7 - j] |= ((s[i] >> j) & 0x01) << i;
        }
    }

    for (i = 0; i < 4; ++i)
    {
        s[i] = t[2 * i + 1];
        s[i + 4] = t[2 * i];
    }
}

static void DoKey(unsigned char *s, unsigned char *C, unsigned char *D)
{
    unsigned char i, j;
    unsigned char t[8];

    for (i = 0; i < 8; ++i)
    {
        t[i] = 0;
    }

    for (i = 0; i < 8; ++i)
    {
        for (j = 0; j < 8; ++j)
        {
            t[7 - j] |= ((s[i] & (0x01 << j)) >> j) << i;
        }
    }

    for (i = 0; i < 4; i++)
    {
        C[i] = t[i];
        D[i] = t[6 - i];
    }

    C[3] &= 0xf0;
    D[3] = (D[3] & 0x0f) << 4;
}

static void DoLeft(unsigned char *s, short n)
{
    unsigned char L, t, i;

    L = 0xff << (8 - n);
    t = (s[0] & L) >> 4;
    s[3] |= t;

    for (i = 0; i < 3; i++)
    {
        s[i] = s[i] << n;
        t = (s[i + 1] & L) >> (8 - n);
        s[i] |= t;
    }

    s[3] = s[3] << n;
}

static void DoRight(unsigned char *s, short n)
{
    unsigned char i, l, l1;

    for (i = 0; i < n; ++i)
    {
        //
        //  rShift(s);
        l = s[0] & 1;
        s[0] >>= 1;
        l1 = s[1] & 1;
        s[1] >>= 1;
        s[1] |= (l << 7);
        l = s[2] & 1;
        s[2] >>= 1;
        s[2] |= (l1 << 7);
        s[3] >>= 1;
        s[3] |= (l << 7);

        if ((s[3] & 0xf) != 0)
        {
            s[0] |= 0x80;
            s[3] &= 0xf0;
        }
    }
}

static void SetKey(unsigned char encrypt, unsigned char *C, unsigned char *D, short n, unsigned char *K)
{
    unsigned char i;

    for (i = 0; i < 6; ++i)
    {
        K[i] = 0;
    }

    if (encrypt)
    {
        DoLeft(C, n);
        DoLeft(D, n);
    }
    K[0] = ((C[1] & 4) << 5) |
           ((C[2] & 0X80) >> 1) |
           (C[1] & 0X20) |
           ((C[2] & 1) << 4) |
           ((C[0] & 0X80) >> 4) |
           ((C[0] & 8) >> 1) |
           ((C[0] & 0X20) >> 4) |
           ((C[3] & 0X10) >> 4);
    K[1] = ((C[1] & 2) << 6) |
           ((C[0] & 4) << 4) |
           ((C[2] & 8) << 2) |
           ((C[1] & 0X40) >> 2) |
           ((C[2] & 2) << 2) |
           ((C[2] & 0x20) >> 3) |
           ((C[1] & 0X10) >> 3) |
           ((C[0] & 0X10) >> 4);
    K[2] = ((C[3] & 0x40) << 1) |
           ((C[0] & 0X01) << 6) |
           ((C[1] & 0X01) << 5) |
           ((C[0] & 2) << 3) |
           ((C[3] & 0X20) >> 2) |
           ((C[2] & 0x10) >> 2) |
           ((C[1] & 0X08) >> 2) |
           ((C[0] & 0X40) >> 6);
    K[3] = ((D[1] & 0x08) << 4) |
           ((D[2] & 0X01) << 6) |
           (D[0] & 0X20) |
           ((D[1] & 0x80) >> 3) |
           ((D[2] & 0X20) >> 2) |
           ((D[3] & 0x20) >> 3) |
           ((D[0] & 0X40) >> 5) |
           ((D[1] & 0X10) >> 4);
    K[4] = ((D[2] & 0x02) << 6) |
           ((D[2] & 0X80) >> 1) |
           ((D[0] & 0X08) << 2) |
           (D[2] & 0x10) |
           ((D[1] & 0X01) << 3) |
           ((D[2] & 0x08) >> 1) |
           ((D[1] & 0X20) >> 4) |
           ((D[3] & 0X10) >> 4);
    K[5] = ((D[0] & 0x04) << 5) |
           ((D[3] & 0X80) >> 1) |
           ((D[2] & 0X40) >> 1) |
           ((D[1] & 0x04) << 2) |
           ((D[2] & 0X04) << 1) |
           ((D[0] & 0x01) << 2) |
           ((D[0] & 0X80) >> 6) |
           ((D[0] & 0X10) >> 4);

    if(!encrypt)
    {
        DoRight(C, n);
        DoRight(D, n);
    }
}

static void E_expand(unsigned char * s, unsigned char * r)
{
    r[0] = ((s[3] & 0x01) << 7) | ((s[0] & 0xf8) >> 1) | ((s[0] & 0x18) >> 3);
    r[1] = ((s[0] & 0x07) << 5) |
           ((s[0] & 0x01) << 3) |
           ((s[1] & 0x80) >> 3) |
           ((s[1] & 0xe0) >> 5);
    r[2] = ((s[1] & 0x18) << 3) | ((s[1] & 0x1f) << 1) | ((s[2] & 0x80) >> 7);
    r[3] = ((s[1] & 0x01) << 7) | ((s[2] & 0xf8) >> 1) | ((s[2] & 0x18) >> 3);
    r[4] = ((s[2] & 0x07) << 5) |
           ((s[2] & 0x01) << 3) |
           ((s[3] & 0x80) >> 3) |
           ((s[3] & 0xe0) >> 5);
    r[5] = ((s[3] & 0x18) << 3) | ((s[3] & 0x1f) << 1) | ((s[0] & 0x80) >> 7);
}

static void P_chang(unsigned char * s)
{
    unsigned char i;
    unsigned char t[4];

    t[0] = ((s[1] & 0x01) << 7) |
           ((s[0] & 0x02) << 5) |
           ((s[2] & 0x10) << 1) |
           ((s[2] & 0x08) << 1) |
           (s[3] & 0x08) |
           ((s[1] & 0x10) >> 2) |
           ((s[3] & 0x10) >> 3) |
           ((s[2] & 0x80) >> 7);
    t[1] = (s[0] & 0x80) |
           ((s[1] & 0x02) << 5) |
           ((s[2] & 0x02) << 4) |
           ((s[3] & 0x40) >> 2) |
           (s[0] & 0x08) |
           ((s[2] & 0x40) >> 4) |
           (s[3] & 0x02) |
           ((s[1] & 0x40) >> 6);
    t[2] = ((s[0] & 0x40) << 1) |
           ((s[0] & 0x01) << 6) |
           ((s[2] & 0x01) << 5) |
           ((s[1] & 0x04) << 2) |
           ((s[3] & 0x01) << 3) |
           ((s[3] & 0x20) >> 3) |
           ((s[0] & 0x20) >> 4) |
           ((s[1] & 0x80) >> 7);
    t[3] = ((s[2] & 0x20) << 2) |
           ((s[1] & 0x08) << 3) |
           ((s[3] & 0x04) << 3) |
           ((s[0] & 0x04) << 2) |
           ((s[2] & 0x04) << 1) |
           ((s[1] & 0x20) >> 3) |
           ((s[0] & 0x10) >> 3) |
           ((s[3] & 0x80) >> 7);

    for (i = 0; i < 4; ++i)
    {
        s[i] = t[i];
    }
}

static unsigned char Find_S(unsigned char * s, short ns)
{
    unsigned char col, num, uIndex = 0;

    if (ns == 1 || ns == 5)
    {
        col = ((s[0] & 0x80) >> 6) | ((s[0] & 0x04) >> 2);
        num = (s[0] & 0x78) >> 3;
        uIndex = col * 16 + num;
    }

    if (ns == 2 || ns == 6)
    {
        col = (s[0] & 0x02) | ((s[1] & 0x10) >> 4);
        num = ((s[0] & 0x01) << 3) | ((s[1] & 0xe0) >> 5);
        uIndex = col * 16 + num;
    }

    if (ns == 3 || ns == 7)
    {
        col = ((s[1] & 0x08) >> 2) | ((s[2] & 0x40) >> 6);
        num = ((s[1] & 0x07) << 1) | ((s[2] & 0x80) >> 7);
        uIndex = col * 16 + num;
    }

    if (ns == 4 || ns == 8)
    {
        col = ((s[2] & 0x20) >> 4) | (s[2] & 0x01);
        num = ((s[2] & 0x1e) >> 1);
        uIndex = col * 16 + num;
    }
    return Sfun_Tab[ns - 1][uIndex];
    //   ReadCodeFlash( (uint)(ns-1)*64+index +DESCODEADD-CODEHINTADD,&num,1);
    //  return num;
}

/*void  DoSfun(unsigned char xdata *s,unsigned char xdata * r)
   {
     r[0] = (Find_S(s,1) << 4) | Find_S(s,2);
     r[1] = (Find_S(s,3) << 4) | Find_S(s,4);
     r[2] = (Find_S(s+3,5) << 4) | Find_S(s+3,6);
     r[3] = (Find_S(s+3,7) << 4) | Find_S(s+3,8);
   }
 */
static void f_fun(unsigned char *R, unsigned char *K, unsigned char *m)
{
    unsigned char i;
    unsigned char t[6];

    E_expand(R, t);

    for (i = 0; i < 6; ++i)
    {
        t[i] ^= K[i];
    }

    //  DoSfun(t, m);

    m[0] = (Find_S(t, 1) << 4) | Find_S(t, 2);
    m[1] = (Find_S(t, 3) << 4) | Find_S(t, 4);
    m[2] = (Find_S(t + 3, 5) << 4) | Find_S(t + 3, 6);
    m[3] = (Find_S(t + 3, 7) << 4) | Find_S(t + 3, 8);

    P_chang(m);
}

/*void  DoMut(unsigned char xdata * L, unsigned char xdata * R, unsigned char xdata * K)
   {
    uchar   i;
    unsigned char xdata t[4],t2[6];

    f_fun(R, K, t);

    E_expand(R, t2);
    for(i = 0; i < 6; ++i)  t2[i] ^= K[i];

     t[0] = (Find_S(t2,1) << 4) | Find_S(t2,2);
     t[1] = (Find_S(t2,3) << 4) | Find_S(t2,4);
     t[2] = (Find_S(t2+3,5) << 4) | Find_S(t2+3,6);
     t[3] = (Find_S(t2+3,7) << 4) | Find_S(t2+3,8);
     P_chang(t);

    for(i = 0; i < 4; ++i)
    {
       t[i] ^= L[i];
       L[i] = R[i];
       R[i] = t[i];
    }
   }
 */


/*************************************************
   Function:       // (bool bEncrypt, u8* pheDatat,const u8* phekeyt)
   Description:    // DES加解密函数
   Input:			 bEncrypt = false; 解密
                                         bEncrypt = true; 加密
                                pheDatat,原数据 8BYTE
                                         phekeyt,密钥 8BYTE
   Output:          datat加解密后数据
   Others:
*************************************************/
s32 sdkDesS(bool bEncrypt, u8* pheDatat, const u8* phekeyt)
{
    unsigned char i, j;

    unsigned char en_loop[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    unsigned char de_loop[16] = {1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 1};
    unsigned char C0[4], D0[4], K[6];
    unsigned char  * L, * R, c;
    unsigned char s1[9], s2[9], t[8], tx1[4], t2[6];

    if(NULL == pheDatat || NULL == phekeyt)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = 0; i < 8; ++i)
    {
        s1[i] = pheDatat[i];
        s2[i] = phekeyt[i];
    }

    //WATCHDOG;
    s1[8] = 0;
    s2[8] = 0;
    DoFirstChange(s1);
    L = s1; R = s1 + 4;
    DoKey(s2, C0, D0);

    for (i = 0; i < 16; ++i)
    {
        //WATCHDOG;
        if (bEncrypt)
        {
            SetKey(bEncrypt, C0, D0, en_loop[i], K);
        }
        else
        {
            SetKey(bEncrypt, C0, D0, de_loop[i], K);
        }
        f_fun(R, K, t);

        E_expand(R, t2);

        for (j = 0; j < 6; ++j)
        {
            t2[j] ^= K[j];
        }

        tx1[0] = (Find_S(t2, 1) << 4) | Find_S(t2, 2);
        tx1[1] = (Find_S(t2, 3) << 4) | Find_S(t2, 4);
        tx1[2] = (Find_S(t2 + 3, 5) << 4) | Find_S(t2 + 3, 6);
        tx1[3] = (Find_S(t2 + 3, 7) << 4) | Find_S(t2 + 3, 8);
        P_chang(tx1);

        for (j = 0; j < 4; ++j)
        {
            tx1[j] ^= L[j];
            L[j] = R[j];
            R[j] = tx1[j];
        }
    }

    for (i = 0; i < 4; ++i)
    {
        c = R[i];
        s1[i + 4] = L[i];
        s1[i] = c;
    }

    for (i = 0; i < 8; ++i)
    {
        t[i] = 0;
    }

    for (i = 0; i < 8; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            t[i] |= ((s1[j] << (7 - i)) & 0x80) >> (2 * j + 1);
            t[i] |= ((s1[j + 4] << (7 - i)) & 0x80) >> (2 * j);
        }
    }

    for (i = 0; i < 8; ++i)  //s1[i] = t[i];
    {
        pheDatat[i] = t[i];
    }

    //WATCHDOG;
    return SDK_OK;
}

/*************************************************
   Function:       // (bool bEncrypt, u8* pheDatat,const u8* phekeyt)
   Description:    // 3DES加解密函数
   Input:			 bEncrypt = false; 解密
                                         bEncrypt = true; 加密
                                pheDatat,原数据 8BYTE
                                         phekeyt,密钥 16BYTE
   Output:          pheDatat加解密后数据
   Others:
*************************************************/
s32 sdkDes3S(bool bEncrypt, u8* pheDatat, const u8* phekeyt)
{
    if(bEncrypt > 1 || NULL == pheDatat || NULL == phekeyt)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Verify(sdkDesS(bEncrypt, pheDatat, &phekeyt[0]) == SDK_OK);
    Verify(sdkDesS(!bEncrypt, pheDatat, &phekeyt[8]) == SDK_OK);
    Verify(sdkDesS(bEncrypt, pheDatat, &phekeyt[0]) == SDK_OK);
    return SDK_OK;
}

