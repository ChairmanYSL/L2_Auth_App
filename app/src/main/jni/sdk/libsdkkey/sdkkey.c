#include "sdkGlobal.h"


s32 sdkKbGetKey(void);
s32 sdkKeyOpen(void);

//extern u8 gstAutoTest;
//extern u8 gstAutoTime;

s32 sdkKbWaitKey(u32 uiMask, s32 siMs)
{
    u32 timerID;
    bool flag = false;
    s32 Key;

//    if(gstAutoTest)
//    {
//        if(gstAutoTime == 0)
//        {
//        	gstAutoTime = 8;
//        }
//
//        if(siMs > gstAutoTime * 1000 || siMs == 0)
//        {
//            siMs = gstAutoTime * 1000;
//        }
//    }
//
    sdkKbKeyFlush();

	sdkTimerStar((u32)siMs);
    while(1)
    {
        if((siMs != 0) && sdkTimerIsEnd() == 1)
        {
            return SDK_TIME_OUT;
        }
        Key = sdkKbGetKey();

        if(SDK_KEY_ERR != Key)
        {
            if(0x7FFFFFFF == uiMask)
            {
                flag = true;
            }
            else if(
                (Key == SDK_KEY_0 && (uiMask & SDK_KEY_MASK_0))
                || (Key == SDK_KEY_1 && (uiMask & SDK_KEY_MASK_1))
                || (Key == SDK_KEY_2 && (uiMask & SDK_KEY_MASK_2))
                || (Key == SDK_KEY_3 && (uiMask & SDK_KEY_MASK_3))
                || (Key == SDK_KEY_4 && (uiMask & SDK_KEY_MASK_4))
                || (Key == SDK_KEY_5 && (uiMask & SDK_KEY_MASK_5))
                || (Key == SDK_KEY_6 && (uiMask & SDK_KEY_MASK_6))
                || (Key == SDK_KEY_7 && (uiMask & SDK_KEY_MASK_7))
                || (Key == SDK_KEY_8 && (uiMask & SDK_KEY_MASK_8))
                || (Key == SDK_KEY_9 && (uiMask & SDK_KEY_MASK_9))
                || (Key == SDK_KEY_ENTER && (uiMask & SDK_KEY_MASK_ENTER))
                || (Key == SDK_KEY_ESC && (uiMask & SDK_KEY_MASK_ESC))
                || (Key == SDK_KEY_UP && (uiMask & SDK_KEY_MASK_UP))
                || (Key == SDK_KEY_DOWN && (uiMask & SDK_KEY_MASK_DOWN))
                || (Key == SDK_KEY_F1 && (uiMask & SDK_KEY_MASK_F1))
                || (Key == SDK_KEY_F2 && (uiMask & SDK_KEY_MASK_F2))
                || (Key == SDK_KEY_F3 && (uiMask & SDK_KEY_MASK_F3))
                || (Key == SDK_KEY_F4 && (uiMask & SDK_KEY_MASK_F4))
                || (Key == SDK_KEY_PRINT && (uiMask & SDK_KEY_MASK_PRINT))
                || (Key == SDK_KEY_FUNCTION && (uiMask & SDK_KEY_MASK_FUNCTION))
                || (Key == SDK_KEY_MULTITASK && (uiMask & SDK_KEY_MASK_MULTTASK))
                || (Key == SDK_KEY_ALPHA && (uiMask & SDK_KEY_MASK_ALPHA))
                || (Key == SDK_KEY_LEFT && (uiMask & SDK_KEY_MASK_LEFT))
                || (Key == SDK_KEY_DOUBLEZERO && (uiMask & SDK_KEY_MASK_00) )
                || (Key == SDK_KEY_RIGHT && (uiMask & SDK_KEY_MASK_RIGHT))
                || (Key == SDK_KEY_CLEAR && (uiMask & SDK_KEY_MASK_CLEAR))
                || (Key == SDK_KEY_BACKSPACE && (uiMask & SDK_KEY_MASK_BACKSPACE))
                || (Key == SDK_KEY_SIGN && (uiMask & SDK_KEY_MASK_LOGIN) )
                || (Key == SDK_KEY_10 && (uiMask & SDK_KEY_MASK_10) )
                || (Key == SDK_KEY_11 && (uiMask & SDK_KEY_MASK_11) )
                || (Key == SDK_KEY_F5&& (uiMask & SDK_KEY_MASK_F5))
                || (Key == SDK_KEY_SET && (uiMask & SDK_KEY_MASK_SET) ) //ֻ�������ü�
                || (Key == SDK_KEY_BILL && (uiMask & SDK_KEY_MASK_BILL) ) //ֻ�����˵���
                || (Key == SDK_KEY_MENU && (uiMask & SDK_KEY_MASK_MENU) )//ֻ�����˵���
                || (Key == SDK_KEY_PAY && (uiMask & SDK_KEY_MASK_PAY) ) //ֻ����֧����
                || (Key == SDK_KEY_FLASH && (uiMask & SDK_KEY_MASK_PHONE) )//�������ϡ����š����ᡢ�ز������硢ȥ��6����
                || (Key == SDK_KEY_SMS && (uiMask & SDK_KEY_MASK_PHONE) )//�������ϡ����š����ᡢ�ز������硢ȥ��6����
                || (Key == SDK_KEY_NOHOOK && (uiMask & SDK_KEY_MASK_PHONE) )//�������ϡ����š����ᡢ�ز������硢ȥ��6����
                || (Key == SDK_KEY_REDIAL && (uiMask & SDK_KEY_MASK_PHONE) )//�������ϡ����š����ᡢ�ز������硢ȥ��6����
                || (Key == SDK_KEY_DIALIN && (uiMask & SDK_KEY_MASK_PHONE) )//�������ϡ����š����ᡢ�ز������硢ȥ��6����
                || (Key == SDK_KEY_DIALOUT && (uiMask & SDK_KEY_MASK_PHONE) )//�������ϡ����š����ᡢ�ز������硢ȥ��6����

            )
            {
                flag = true;
            }

            if(flag)
            {
                //sdkSysBeep(SDK_SYS_BEEP_OK);
                return Key;
            }
            else
            {
                //sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
    }
}


/**********************MMI*******************************************/

#define SDK_KEY_TAB_MAX_LEN 40

static u32 Sdk_timerID = 0;
static u16 gOldPwdLen = 0;
static u8 const Sdk_SpecSymbol[] = "~!@#$%^&*()_+|\\=-`<>?\":{}[];\'.,/";

#define STR_CURSOR                      "_"
#define SYS_TIME_FORMAT         "____-__-__ __:__:__"
#define SDKMAXT9KEYNUM      9

/************************���뷽ʽ����**********************************/
#define MODE_NONE                       0x00                    //���뷨��ʼ״̬
#define MODE_CODE_D                     0x01                    //����
#define MODE_T9                         0x02                    //��д��ĸ
#define MODE_BIG_LET                    0x03                    //��д��ĸ
#define MODE_SMA_LET                    0x04                    //Сд��ĸ
#define MODE_NUMBER                     0x05                    //����
#define MODE_SYMBOL                     0x06                    //�����ַ�
#define MODE_STROKE                     0x07                    //STARTEND
#define MODE_HAND                       0x08                    //��д����ģʽ
#define MODE_FARSI                      0x09                    //��˹��
#define MODE_TEL_LET                    0x10                    //�绰������ĸ��
#define MODE_RUSSIAN                    0x11                    //����˹
#define MODE_GEORGIA                    0x12                    //��³����
/*********************T9ƴ����һЩ�ṹ�嶨��*****************************/
#ifndef F_OK
#define F_OK                    0
#endif


typedef struct                                  //������ִ�ŵĻ���.
{
    u8 *Point;                                  //������ͷ��ַ
    u8 Len;                                     //����������
}SDK_KEY_HZMEMERY;

typedef struct
{
    u32 Mask;
    u8 GetCharRslt;                             //GETCHAR�������Ϣ״̬
}SDK_KEY_GETCHARFMT;

typedef struct                                  //������ִ�ŵĻ���.
{
    u8 curr;
    u8 buf[30];                                 //����������
}SDK_KEY_STROKEHZBUF;

typedef struct                                  //ƴ���ṹ
{
    u8 Stroke[50];
    u8 StrokeNum;                               //�ʻ���
    u16 position;                               //��ǰλ��
}SDK_KEY_STROKEPOSGROUP;




typedef struct                                  //�����ṹ
{
    u16 MaxAddr;
    u16 MinAddr;
} SDK_KEY_ADDR;

typedef struct                                  //ƴ���ṹ
{
    u8 Num;                                     //���ܵ�ƴ����ϸ���
    u8 Index;                                   //��ѡ�е�ƴ��(��ƴ���ַ�������)������
    u8 String[10][10];                          //ƴ���ַ�������
    SDK_KEY_ADDR FstIndex[10];                  //һ������(���ֿ��е�,������)
}SDK_KEY_PINYINGROUP;

typedef struct                                  //��ʾ���ַ��ṹ
{
    u8 Update;                                  //�ǲ���Ҫ���¶��ֿ� 0����Ҫ,1��Ҫ
    u16 CurrentAddr;                            //��ǰҳ�Ŀ�ʼ��ַ
    u16 NextAddr;                               //��һҳ���ֵĿ�ʼ��ַ
    u16 PreAddr;                                //��һҳ���ֵĿ�ʼ��ַ
    u8 col;                                     //�����ַ�����Ҫ���Ե���
    u8 Chinese[SDK_MAX_LINE_CHAR + 1];          //��Ӧ�ĺ����ַ���
}SDK_KEY_DISPBUF;

typedef struct
{
    //Modify by huangkanghui at 2015.03.25 19:01
    //SDK_KEY_PINYINGROUP MmiPinYinGroup;         //ƴ�����뷨��ƴ���ṹ
    //SDK_KEY_DISPBUF MmiDispBuf;                 //ƴ�����뷨����ʾ�ַ��ṹ
    u8 MmiCurrentMode;                          //��ǰ�����뷽ʽ
    u8 MmiKeyBuf[SDK_KEY_BUF_SIZE + 1];         // ���ռ�������Ļ�����
    u8 MmiKeyLen;                               // �����������
    u8 MmiCurrentRow;                           //��ǰ������
    u8 MmiIsInput;                              //�Ƿ���������λ��or ƴ��
    u8 MmiIsLetterEnd;                          //������ĸ����
    u8 MmiLastKey;                              //�������һ������
    u8 MmiKeyTimes;                             //ͬһ�����ּ�����Ĵ���
}SDK_KEY_SCANMEMBER;

#define psdkMmiDisp  s32 (*sdkMmiDispCallFun)(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow, void *pVar)
#define sdkMmiDispFun(pStrDisp, uiMode, siDispRow, pVar) sdkMmiDispCallFun(pStrDisp, uiMode, siDispRow, pVar)

typedef struct
{
    u8 Contents[SDK_MAX_STR_LEN + 1 + 1];       //�������� ������ǰ���һ�����Ⱥ�������һ�� ������
    u8 RowNo;                                   //�����к�(0-4)
    u8 MaxLen;                                  //��󳤶�����
    u8 MinLen;                                  //��С��������
    u32 Mask;                                   //������
    SDK_KEY_SCANMEMBER ScanMem;
    u32 uiFistMode;                               //��ʼ�����뷨
    psdkMmiDisp;                                //�ص���ʾ����ָ��
    void *pVar;                                 //��չָ�����
}SDK_KEY_SCANFMT;                               //������Ϣ�ṹ��



void sdkMmiChangeModeToSym(SDK_KEY_SCANFMT *pScan);
void sdkMmiChangeModeToBigLet(SDK_KEY_SCANMEMBER *pScanMem);
s32 sdkMmiKeyConver (u8 keyvalue, u8 *newkey, u8 Mode, SDK_KEY_SCANFMT *pScan);


s32 Private_sdkDispRow(s32 siRow, s32 eCol, const u8 * pasStr, u32 ucAtr)
{
#if 0 /*Modify by zcl at 2016.02.16  14:37 */

    if(false == HandInputInfo.siHandInputFlag)
#endif /* if 0 */
    {
        return sdkDispRow(siRow, eCol, pasStr, ucAtr);
    }
#if 0 /*Modify by zcl at 2016.02.16  14:37 */
    else
    {
        Private_sdkDispHandinputClearRow(siRow); //sdkDispRow�Դ�������ǰ��ʾ�еĹ��ܣ�sdkDispAtû�У����Լ��ϴ˹���
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
    }
    return SDK_OK;

#endif /* if 0 */
}

void sdkMmiDelLastChar(u8 *str)
{
    s32 len, i;

    len = strlen(str);
    i = 0;

    if(len > 0)
    {
        while(1)
        {
            if(str[i] > 0x80)                   //�����ַ�
            {
                i += 2;

                if(i >= len)                    //���һ���Ǻ���
                {
                    str[len - 1] = 0;
                    str[len - 2] = 0;
                    return;
                }
            }
            else
            {
                i++;

                if(i >= len)                    //���һ�����Ǻ���
                {
                    str[len - 1] = 0;
                    return;
                }
            }
        }
    }
    else{ return; }
}

static void sdkMmiClearKey(SDK_KEY_SCANMEMBER *pScanMem)
{
    memset (pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
    pScanMem->MmiKeyLen = 0;
}

static s32 sdkMmiGetLetterKey (const u8 key, u8 *keybuf, u8 *keynum, u8 flag, u8 maxlen, u8 Mode, u8 beep, SDK_KEY_SCANFMT *pScan)
{
    s32 m = 0;
    u8 rlt = false;
    u8 str_buf[3] = {0};

    memset(str_buf, 0, sizeof(str_buf));

    if (((key < '0') || (key > '9'))
        && (key != SDK_KEY_UP) && (key != SDK_KEY_DOWN))
    {
        if (key == SDK_KEY_BACKSPACE)                                                                                           //�����
        {
            if ((*keynum == 0) && (beep == 1))
            {
                sdkSysBeep(1);
            }
            else if (*keynum > 0)
            {
                if (beep == 1)
                {
                    sdkSysBeep(0);
                }

                if (flag == 0)                                                                                  //�����������ȫ�����
                {
                    sdkMmiClearKey(&pScan->ScanMem);
                }
                else
                {
                    (*keynum)--;
                    keybuf[*keynum] = 0;
                }
                pScan->ScanMem.MmiLastKey = 0;
                pScan->ScanMem.MmiKeyTimes = 0;
                pScan->ScanMem.MmiIsLetterEnd = 0;
                //			pVTable->OpenTimerAbc(FALSE);
            }
        }
        else if ((key != SDK_KEY_ENTER) && (key != SDK_KEY_ESC) && (beep == 1))
        {
            sdkSysBeep(1);
        }
    }
    else
    {
        if ((*keynum) > maxlen)
        {
            if (beep == 1)
            {
                sdkSysBeep(1);
            }
        }
        else
        {
            if (beep == 1)
            {
                sdkSysBeep(0);
            }
            m = sdkMmiKeyConver(key, str_buf, Mode, pScan);

            if (m == 0)
            {
                keybuf[*keynum] = str_buf[0];
                (*keynum)++;
                rlt = true;
            }
            else if(*keynum != 0)
            {
                keybuf[(*keynum) - 1] = str_buf[0];
            }
        }
    }

    if (*keynum == 0)
    {
        memset(keybuf, 0, SDK_KEY_BUF_SIZE);
        pScan->ScanMem.MmiLastKey = 0;
        pScan->ScanMem.MmiKeyTimes = 0;
        pScan->ScanMem.MmiIsLetterEnd = 0;
    }
    return rlt;
}

s32 Private_sdkDispRowEx(s32 siRow, s32 eCol, const u8 * pasStr, u32 ucAtr)
{
#if 0 /*Modify by zcl at 2016.02.16  14:38 */

    if(false == HandInputInfo.siHandInputFlag)
#endif /* if 0 */
    {
        return sdkDispRow(siRow, eCol, pasStr, ucAtr);
    }
#if 0 /*Modify by zcl at 2016.02.16  14:38 */
    else
    {
        if(0 == (ucAtr & SDK_DISP_LDISP)) //�ǿ���ģʽ
        {
            ucAtr &= ~(SDK_DISP_CDISP | SDK_DISP_RDISP);
            ucAtr |= SDK_DISP_LDISP;     //����дģʽ�У��̶�Ϊ��ģʽ
        }
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
        sdkDispBrushScreen();
    }
    return SDK_OK;

#endif /* if 0 */
}

s32 Private_sdkDispLineMaxLen(void)
{
#if 0 /*Modify by zcl at 2016.02.16  14:40 */

    if(false == HandInputInfo.siHandInputFlag)
#endif /* if 0 */
    {
        return SDK_MAX_LINE_CHAR;
    }
#if 0 /*Modify by zcl at 2016.02.16  14:39 */
    else
    {
        return (HandInputInfo.siEndX - HandInputInfo.siStartX) / HandInputInfo.siByteLen;
    }
#endif /* if 0 */
}

static u8 sdkMmiCountLineChar_R(const u8 * pStr, const u8 *pEnd)
{
    u8 i = 0;

    s32 len = 0, length = pEnd - pStr;

    if(length <= Private_sdkDispLineMaxLen())                 //����ʾ�ַ���β
    {
        if(pEnd < pStr) { len = 0; }
        else
        {
            len = pEnd - pStr;                      //ȡ�����еĳ���
        }
        return (u8)len;
    }

    while(1)
    {
        if(pStr[i] > 0x80)                          //�����ַ�
        {
            i += 2;

            if(i == (length - Private_sdkDispLineMaxLen()))
            {
                len = Private_sdkDispLineMaxLen();
                break;
            }

            if(i > (length - Private_sdkDispLineMaxLen()))    //���һ���Ǻ���
            {
                len = Private_sdkDispLineMaxLen() - 1;
                break;
            }
        }
        else
        {
            i++;

            if(i == (length - Private_sdkDispLineMaxLen()))
            {
                len = Private_sdkDispLineMaxLen();
                break;
            }
        }
    }

    return (u8)len;
}

static u8 sdkMmiCountLineChar_L(const u8 * pStr)
{
    u8 i = 0, len = 0;

    while(1)                                     //ÿһ��
    {
        if(pStr[i] == 0)                         //����ʾ�ַ���β
        {
            len = i;                             //ȡ�����еĳ���
            break;
        }

        if(pStr[i] > 0x80)                       //�Ǻ���
        {
            i += 2;                              //����һ�����ֵĳ���

            if(i > Private_sdkDispLineMaxLen())            //��β
            {
                len = i - 2;                     //ȡ�����еĳ���
                break;
            }
        }
        else                                     //���Ǻ���
        {
            i++;

            if(i > Private_sdkDispLineMaxLen())            //��β
            {
                len = i - 1;                     //ȡ�����еĳ���
                break;
            }
        }
    }

    return len;
}

static u8 sdkMmiCountLine(u8 *buf, u8 *dispbuf, u8 align, SDK_KEY_SCANMEMBER ScanMem)
{
    u8 *p, *pEnd, len, MmiLineNum;

    MmiLineNum = 1;

    if((s32)strlen(buf) <= Private_sdkDispLineMaxLen())
    {
        MmiLineNum = 1;

        if(dispbuf != NULL) { memcpy(dispbuf, buf, strlen(buf)); }
    }
    else
    {
        if(align == SDK_DISP_RDISP)                     //�Ҷ��뷽ʽ:��ʾ����(�Ӻ���ǰ������)
        {
            p = buf + strlen(buf);

            while(1)
            {
                len = sdkMmiCountLineChar_R(buf, p);
                p -= len;

                if((MmiLineNum == ScanMem.MmiCurrentRow) && (dispbuf != NULL))                //������м���ǰ��ʾ��
                {
                    memcpy(dispbuf, p, len);            //����Ҫ��ʾ���ַ���д��ָ���ڴ�
                }

                if(p <= buf) { break; }
                MmiLineNum++;
            }
        }
        else                                            //��������з�ʽ:��ʾ���к�����(��ǰ���������)
        {
            p = buf;                                    //ָ��ָ�����ʾ�ַ���ͷ
            pEnd = buf + strlen(buf);

            while(1)
            {
                len = sdkMmiCountLineChar_L(p);

                if((MmiLineNum == ScanMem.MmiCurrentRow) && (dispbuf != NULL))                //������м���ǰ��ʾ��
                {
                    memcpy(dispbuf, p, len);            //����Ҫ��ʾ���ַ���д��ָ���ڴ�
                }
                p += len;

                if(p >= pEnd)
                {
                    break;                              //���������ʾ�ַ�β,����ֹѭ��
                }
                MmiLineNum++;                           //������1
            }
        }
    }
    return MmiLineNum;
}

s32 Private_sdkDispClearRowRam(s32 siRow)
{
    if(siRow < 0)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by zcl at 2016.02.16  14:42 */

    if(false == HandInputInfo.siHandInputFlag)
#endif /* if 0 */
    {
        return sdkDispClearRowRam(siRow);
    }
#if 0 /*Modify by zcl at 2016.02.16  14:42 */
    else
    {
        return Private_sdkDispHandinputClearRow(siRow);
    }
#endif /* if 0 */
}

s32 Private_sdkDispFillRowRamEx(s32 siRow, s32 eCol, const u8 * pasStr, u32 ucAtr)
{
#if 0 /*Modify by zcl at 2016.02.16  14:45 */

    if(false == HandInputInfo.siHandInputFlag)
#endif /* if 0 */
    {
        return sdkDispFillRowRam(siRow, eCol, pasStr, ucAtr);
    }
#if 0 /*Modify by zcl at 2016.02.16  14:45 */
    else
    {
        if(0 == (ucAtr & SDK_DISP_LDISP)) //�ǿ���ģʽ
        {
            ucAtr &= ~(SDK_DISP_CDISP | SDK_DISP_RDISP);
            ucAtr |= SDK_DISP_LDISP;     //����дģʽ�У��̶�Ϊ��ģʽ
        }
        sdkDispHandInput(siRow, eCol, pasStr, ucAtr);
    }
    return SDK_OK;

#endif /* if 0 */
}

u8 sdkMmiChangeLine(s16 flag, SDK_KEY_SCANFMT *pScan)
{
    u8 align, MmiLineNum;
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    if((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT)
    {
        align = SDK_DISP_LDISP;
    }
    else
    {
        align = SDK_DISP_RDISP;
    }
    MmiLineNum = sdkMmiCountLine(pScan->Contents, 0, align, *pScanMem);

    if(flag < 0)                            //���Ϸ�
    {
        if(align == SDK_DISP_RDISP)         //�Ҷ��룬���Ϸ���������
        {
            if(pScanMem->MmiCurrentRow >= MmiLineNum)
            {
                pScanMem->MmiCurrentRow = MmiLineNum;
                return false;
            }
            else
            {
                pScanMem->MmiCurrentRow++;
                return true;
            }
        }
        else                                //���������У����Ϸ���������
        {
            if( pScanMem->MmiCurrentRow <= 1)
            {
                pScanMem->MmiCurrentRow = 1;
                return false;
            }
            else
            {
                pScanMem->MmiCurrentRow--;
                return true;
            }
        }
    }
    else if (flag > 0)
    {
        if(align == SDK_DISP_RDISP)         //�Ҷ��룬���·���������
        {
            if(pScanMem->MmiCurrentRow <= 1)
            {
                pScanMem->MmiCurrentRow = 1;
                return false;
            }
            else
            {
                pScanMem->MmiCurrentRow--;
                return true;
            }
        }
        else                                //���������У����Ϸ���������
        {
            if(pScanMem->MmiCurrentRow >= MmiLineNum)
            {
                pScanMem->MmiCurrentRow = MmiLineNum;
                return false;
            }
            else
            {
                pScanMem->MmiCurrentRow++;
                return true;
            }
        }
    }
    else
    {
        sdkMmiCountLine(pScan->Contents, 0, align, *pScanMem);

        if(!(align == SDK_DISP_RDISP))      //�Ҷ��룬���·���������
        {
            pScanMem->MmiCurrentRow = MmiLineNum;
        }
        else
        {
            pScanMem->MmiCurrentRow = 1;
        }
        return true;                        //��ʱ����ֵû��ʵ������
    }
}

static void sdkMmiChangeModeToNum(SDK_KEY_SCANMEMBER *pScanMem)
{
    pScanMem->MmiCurrentMode = MODE_NUMBER;      //Ŀǰ�Ǵ�д��ĸ �л���Сд��ĸ
    Private_sdkDispRow(SDK_DISP_LINE5, 0, "123", SDK_DISP_FDISP | SDK_DISP_RDISP);    //��ʾ����T9ƴ��
}

static void sdkSysGetSysKeyPad(u8 KeyTab[MULKEYNUM][7], u8 Mode)
{
    static const u8 sdkGBigLetterKeyPad[MULKEYNUM][7] =
    {
        {"1QZ.,@"},
        {"2ABC"},
        {"3DEF"},
        {"4GHI"},
        {"5JKL"},
        {"6MNO"},
        {"7PRS"},
        {"8TUV"},
        {"9WXY"},
        {"0*#- "},
    };
    //T9����Сд��ĸ���
    static const u8 sdkGMinLetterKeyPad[MULKEYNUM][7] =
    {
        {"1qz.,@"},
        {"2abc"},
        {"3def"},
        {"4ghi"},
        {"5jkl"},
        {"6mno"},
        {"7prs"},
        {"8tuv"},
        {"9wxy"},
        {"0*#- "},
    };
    //�绰�������
    static const u8 sdkTelLetterKeyPad[MULKEYNUM][7] =
    {
        {"1,;@"},
        {"2ABC"},
        {"3D"},
        {"4"},
        {"5"},
        {"6"},
        {"7"},
        {"8"},
        {"9W"},
        {"0*-"},
    };

    u8 i;


    for (i = 0; i < MULKEYNUM; i++)
    {
        if (MODE_BIG_LET == Mode)
        {       //fusuipu 2013.01.08 14:0
            strcpy(KeyTab[i], sdkGBigLetterKeyPad[i]);
        }
        else if (MODE_SMA_LET == Mode)
        {
            //fusuipu 2013.01.08 14:1
            strcpy(KeyTab[i], sdkGMinLetterKeyPad[i]);
        }
        else if (MODE_TEL_LET == Mode)
        {
            //fusuipu 2013.01.08 14:3
            strcpy(KeyTab[i], sdkTelLetterKeyPad[i]);
        }
        else
        {
            //fusuipu 2013.01.08 14:3
            Assert(0);
        }
    }
}

s32 sdkMmiKeyConver (u8 keyvalue, u8 *newkey, u8 Mode, SDK_KEY_SCANFMT *pScan)
{
    u8 temp[3] = {0};                   //fusuipu 2013.04.11 16:37
    u8 rlt = 0, i;
    u8 offset = 0;
    u8 key = keyvalue;
    u8 keytabletter[MULKEYNUM][7]; //fusuipu 2013.08.08 17:55 ��ĸ��ÿ���������7���ַ��͹���
    u8 keytab[MULKEYNUM][SDK_KEY_TAB_MAX_LEN];
    s32 tab_len = 0;

	sdkTimerStar(800);
    memset(keytabletter, 0, sizeof(keytabletter));
    memset(keytab, 0, sizeof(keytab));  //fusuipu 2013.01.08 14:27

    sdkSysGetSysKeyPad(keytabletter, Mode);

    for(i = 0; i < MULKEYNUM; i++)
    {
        strcpy(keytab[i], keytabletter[i]);
    }


    if (key == '0')
    {
        key = 0x3a;
    }

    if (((key == pScan->ScanMem.MmiLastKey))                                                            //���ϴ��ǰ���ͬһ����
        && (sdkTimerIsEnd() != 1))
    {
        tab_len = strlen(keytab[key - 0x31]);
        if(0 == tab_len)
        {
            return SDK_ERR;
        }
        offset = (u8)(pScan->ScanMem.MmiKeyTimes % tab_len);       //ƫ����
        /*=======BEGIN: fusuipu 2013.04.11  16:37 modify===========*/
        temp[0] = keytab[key - 0x31][offset];
        //pVTable->OpenTimerAbc(TRUE);									        //���¿���ʱ��,�൱�ڶ�ʱ������
        pScan->ScanMem.MmiKeyTimes++;
        rlt = 1;
    }
    else
    {
        pScan->ScanMem.MmiLastKey = key;
        temp[0] = keytab[key - 0x31][offset];
        pScan->ScanMem.MmiKeyTimes = 1;
        //pVTable->OpenTimerAbc(TRUE);					//���¿���ʱ��,�൱�ڶ�ʱ������
        rlt = 0;
    }
    memcpy(newkey, temp, sizeof(temp));                 //fusuipu 2013.04.11 16:36
    //*newkey = temp;
    return rlt;
}

static u8 sdkMmiChangeMode(SDK_KEY_SCANFMT *pScan, const SDK_KEY_STROKEPOSGROUP *posgroup, const SDK_KEY_STROKEHZBUF *hzbuffer)
{
#if 0
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;


    sdkMmiClearKey(pScanMem);

//    if(true == SDK_MMI_IS_MULTI(pScan->Mask))
//    {
//        if(true == sdkMmiMultiChangeMode(pScan, pScanMem))
//        {
//            return true;
//        }
//    }

    if (pScanMem->MmiCurrentMode == MODE_CODE_D)                                                        //�������������
    {
        /*=======BEGIN: fusuipu 2013.06.04  16:55 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
        else
        {
            pScanMem->MmiCurrentMode = MODE_T9;                                                     //T9ƴ��
            //memset (pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
            memcpy (pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
            memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
            memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);   //��ʾ����T9ƴ��
            return true;
        }
    }
    else if(pScanMem->MmiCurrentMode == MODE_HAND)
    {
        pScanMem->MmiCurrentMode = MODE_T9;                                                     //T9ƴ��
        //memset (pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
        memcpy (pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
        memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);       //��ʾ����T9ƴ��
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_T9)
    {
        Private_sdkDispClearRow(SDK_DISP_LINE4);
#if 0               //lilin �����αʻ�
        sdkMmiChangeModeToSTROKE(pScanMem, posgroup, hzbuffer);
        return true;

#endif

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  16:59 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        else
        {
            pScanMem->MmiCurrentMode = MODE_T9;                                                     //T9ƴ��
            memcpy (pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
            memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
            memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);   //��ʾ����T9ƴ��
            return true;
        }
        /*====================== END======================== */
        //sdkMmiChangeModeToCodeD(pScanMem);//START
        //return TRUE;//END
    }
    else if(pScanMem->MmiCurrentMode == MODE_STROKE)
    {
        Private_sdkDispClearRow(SDK_DISP_LINE4);

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:0 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
        sdkMmiChangeModeToCodeD(pScanMem);        //END
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_BIG_LET)
    {
        if((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)
        {
            /*=======BEGIN: fusuipu 2013.06.05  17:17 modify===========*/
            if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
            {
                sdkMmiChangeModeToHand(pScanMem);
                return true;
            }
            /*====================== END======================== */
            sdkMmiChangeModeToNum(pScanMem);
        }

        else if ((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            pScanMem->MmiCurrentMode = MODE_SMA_LET;              //Ŀǰ�Ǵ�д��ĸ �л���Сд��ĸ
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "abc", SDK_DISP_FDISP | SDK_DISP_RDISP);     //��ʾ����T9ƴ��
        }
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_SMA_LET)     //Сд��ĸ
    {
        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ)
        {
            sdkMmiChangeModeToCodeD(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:2 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
        sdkMmiChangeModeToBigLet(pScanMem);
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_NUMBER)
    {
        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ)
        {
            sdkMmiChangeModeToCodeD(pScanMem);
            return true;
        }

        if(((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
           || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX))
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:2 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
    }
    else if(pScanMem->MmiCurrentMode == MODE_SYMBOL)     //�����ַ�
    {
        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ)
        {
            sdkMmiChangeModeToCodeD(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:2 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
    }
    else                                                                                        //���������ƴ��
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;
        return true;
    }
    return false;

#endif

#if 1
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    sdkMmiClearKey(pScanMem);

    if(pScanMem->MmiCurrentMode == MODE_BIG_LET)         //��д��ĸ
    {
        pScanMem->MmiCurrentMode = MODE_SMA_LET;                  //Ŀǰ�Ǵ�д��ĸ �л���Сд��ĸ
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "abc", SDK_DISP_FDISP | SDK_DISP_RDISP);

        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_SMA_LET)            //Сд��ĸ
    {
        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }
        sdkMmiChangeModeToBigLet(pScanMem);
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_NUMBER)            //����
    {
        if(((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
           || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX))
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }
    }
    else if(pScanMem->MmiCurrentMode == MODE_SYMBOL)         //�����ַ�
    {
        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }
    }
    else                                                                                            //���������ƴ��
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;
        return true;
    }
    return false;

#endif
}

u8 sdkMmiDelete(SDK_KEY_SCANFMT *pScan)
{
    u8 len = (u8)strlen(pScan->Contents);

    pScan->ScanMem.MmiLastKey = 0;
    pScan->ScanMem.MmiKeyTimes = 0;
    pScan->ScanMem.MmiIsLetterEnd = 0;

//    if(true == SDK_MMI_IS_MULTI(pScan->Mask))
//    {
//        return sdkMmiMultiDelete(pScan);
//    }
    if(len > 0)
    {
        if(len > 1)
        {
            sdkMmiDelLastChar(pScan->Contents);
        }
        else
        {
            pScan->Contents[0] = 0;
        }

        if(pScan->ScanMem.MmiCurrentMode == MODE_T9)
        {
            memset(pScan->ScanMem.MmiKeyBuf, 0, sizeof(pScan->ScanMem.MmiKeyBuf));
            memcpy(pScan->ScanMem.MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        }
        pScan->ScanMem.MmiIsLetterEnd = 0;
        return true;
    }
    else
    {
        return false;
    }
}

void sdkMmiDisplayScanf(SDK_KEY_SCANFMT *pScan)
{
    u8 len, i, j, align, disp;
    //u8 disbuf[128], temp[128] = {0};
    u8 disbuf[64], temp[64] = {0};
    s32 ret = 0;

    len = (u8)strlen(pScan->Contents);
    memset(disbuf, 0, sizeof(disbuf));

    if(NULL != pScan->sdkMmiDispCallFun)
    {
        ret = pScan->sdkMmiDispFun(pScan->Contents, pScan->Mask, pScan->RowNo, pScan->pVar);

        if(SDK_OK == ret)
        {
            return;
        }
    }

    if((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT)
    {
        align = SDK_DISP_LDISP;
    }
    else
    {
        align = SDK_DISP_RDISP;
    }

    if((pScan->Mask & SDK_MMI_DISP) == SDK_MMI_DISP)
    {
        disp = SDK_DISP_NOFDISP;
    }
    else
    {
        disp = SDK_DISP_FDISP;
    }

    if((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)      //���Խ��
    {
        if(strlen(pScan->Contents) == 0)            //ֻ������һλ
        {
            memcpy(temp, "0.00", 4);
        }

        if(strlen(pScan->Contents) == 1)            //ֻ������һλ
        {
            temp[0] = '0';
            temp[1] = '.';
            temp[2] = '0';
            temp[3] = pScan->Contents[0];
        }
        else if(strlen(pScan->Contents) == 2)       //ֻ��������λ
        {
            temp[0] = '0';
            temp[1] = '.';
            temp[2] = pScan->Contents[0];
            temp[3] = pScan->Contents[1];
        }
        else if(strlen(pScan->Contents) > 2)        //�������ĳ��ȴ���2
        {
            for(i = 0; i < (strlen(pScan->Contents) - 2); i++)
            {
                temp[i] = pScan->Contents[i];
            }

            temp[i] = '.';
            i++;
            temp[i] = pScan->Contents[i - 1];
            i++;
            temp[i] = pScan->Contents[i - 1];
        }                                           //������ʾ���е�λ��
        sdkMmiCountLine(temp, disbuf, align, pScan->ScanMem);
    }
    else if((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME)   //����ʱ��
    {
        memcpy(disbuf, SYS_TIME_FORMAT, 19);
        j = 0;

        for(i = 0; i < len; i++)
        {
            if(i == 4 || i == 6 || i == 8 || i == 10 || i == 12) { j++; }
            disbuf[j] = pScan->Contents[i];
            j++;
        }

        align = SDK_DISP_CDISP;
    }
    else if((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD)     //��������
    {
        if(len > 0)                                           //wanggaodeng modify
        {
            if( pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET || pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET)
            {
                if(len <= Private_sdkDispLineMaxLen())
                {
                    memset(disbuf, '*', len - 1);
                    disbuf[len - 1] = pScan->Contents[len - 1];
                }
                else
                {
                    memset(disbuf, '*', (u32)(Private_sdkDispLineMaxLen() - 1));
                    disbuf[Private_sdkDispLineMaxLen() - 1] = pScan->Contents[len - 1];
                }
            }
            else
            {
                if(len <= Private_sdkDispLineMaxLen())
                {
                    memset(disbuf, '*', len);
                }
                else
                {
                    memset(disbuf, '*', (u32)Private_sdkDispLineMaxLen());
                }
            }
        }
        else
        {
            strcpy(disbuf, STR_CURSOR);             //20090402
        }
    }
    else                                            //����
    {
        if(len == 0)                                // ��
        {
            Private_sdkDispClearRowRam(pScan->RowNo);           //�����ض����ַ�
            Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, STR_CURSOR, disp | align) == SDK_OK);
            sdkDispBrushScreen();

            return;
        }
        else
        {                                           //������ʾ���е�λ��
            sdkMmiCountLine(pScan->Contents, disbuf, align, pScan->ScanMem);
        }
    }

    Private_sdkDispClearRowRam(pScan->RowNo);                   //�����ض����ַ�
    Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, disbuf, disp | align) == SDK_OK);
    sdkDispBrushScreen();
}

void sdkMmiChangeModeToBigLet(SDK_KEY_SCANMEMBER *pScanMem)
{
    pScanMem->MmiCurrentMode = MODE_BIG_LET;      //Ŀǰ��T9 �л�����д��ĸ
    Private_sdkDispRow(SDK_DISP_LINE5, 0, "ABC", SDK_DISP_FDISP | SDK_DISP_RDISP);    //��ʾ������ĸ
}

static s32 sdkMmiScanfINIT(SDK_KEY_SCANFMT*pScan, SDK_KEY_STROKEPOSGROUP *posgroup, SDK_KEY_STROKEHZBUF *hzbuffer)
{
    u8 align, disp;
    u8 len;
    s32 ret = 0;
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;


    //��ʼ��Ĭ��ֵ
    if(pScan->Mask == 0) { pScan->Mask = SDK_MMI_NUMBER; }

    if(pScan->RowNo == 0) { pScan->RowNo = 3; }

    if(pScan->MaxLen == 0) { pScan->MaxLen = SDK_MAX_STR_LEN; }

    if(pScan->MaxLen < pScan->MinLen) { pScan->MinLen = 0; }

    if(((pScan->Mask & SDK_MMI_MULTTASK) == SDK_MMI_MULTTASK)   //�������ϵ�ǩ��״̬��ֻ����������
       || ((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)      //��С������ʾ��ֻ����������
       || ((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD)
       || ((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME))               //�������룬ֻ����������
    {
        pScan->Mask = pScan->Mask & (~SDK_MMI_HEX);

        if((pScan->Mask & SDK_MMI_PWD) != SDK_MMI_PWD) //wanggaodeng delete ���������������ĸ
        {
            pScan->Mask = pScan->Mask & (~SDK_MMI_LETTER);
        }
        pScan->Mask = pScan->Mask & (~SDK_MMI_HZ);
        pScan->Mask = pScan->Mask & (~SDK_MMI_SYMBOL);
    }

    if((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)                      //����ʮ������ʱֻ���������д��ĸ״̬
    {
        pScan->Mask = pScan->Mask | SDK_MMI_LETTER;
        pScan->Mask = pScan->Mask | SDK_MMI_NUMBER;
        pScan->Mask = pScan->Mask & (~SDK_MMI_HZ);
        pScan->Mask = pScan->Mask & (~SDK_MMI_SYMBOL);
    }

    if((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)
    {
        pScan->Mask = pScan->Mask & (~SDK_MMI_LEFT);
    }
    /*=======BEGIN: fusuipu 2013.12.13  15:16 modify Ŀǰ���뷨����֧�ֱʻ����뺺�֣�����У�һ����Ϊƴ������===========*/

    if((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT) { align = SDK_DISP_FDISP; }
    else{ align = SDK_DISP_RDISP; }

    if((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME) { align = SDK_DISP_CDISP; }

    if((pScan->Mask & SDK_MMI_DISP) == SDK_MMI_DISP) { disp = SDK_DISP_NOFDISP; }
    else{ disp = SDK_DISP_FDISP; }

//    /*=======BEGIN: fusuipu 2013.12.09  11:0 modify===========*/
//    if(true == SDK_MMI_IS_MULTI(pScan->Mask))
//    {
//        sdkMmiMultiScanfINIT(pScan, pScanMem);
//    }
//    /*====================== END======================== */
    //��ʼ�����뷽ʽ
    if(0 == pScan->uiFistMode)
    {
        pScan->uiFistMode = pScan->Mask; //fusuipu 2014.04.01 20:28 �����ѡ������û�����ã�����Ĭ�ϵĽ��д���
    }
    else if(0 == (pScan->uiFistMode & pScan->Mask)) //fusuipu 2014.04.01 20:29 �����ѡ���벻�ڵ�Ȼ���뷶Χ֮�ڣ�˵����������ֱ�ӷ���
    {
        return SDK_PARA_ERR;
    }

/*   //huangkanghui 2015.02.27 10:50
        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ &&
       (SDK_MMI_HZ & pScan->uiFistMode))                     //�������뺺��
    {
        pScanMem->MmiCurrentMode = MODE_T9;             //T9ƴ��
        memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
        memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);       //��ʾ����T9ƴ��
    }

    else if((pScan->Mask & SDK_MMI_STROKE) == SDK_MMI_STROKE &&
            (SDK_MMI_STROKE & pScan->uiFistMode))    //START
    {
        memset (posgroup, 0, sizeof(SDK_KEY_STROKEPOSGROUP));
        memset (hzbuffer, 0, sizeof(SDK_KEY_STROKEHZBUF));
        memset ((u8 *)&posgroup, 0, sizeof(STROKEPOSGROUP));
    }
 */
    //END
    //else if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER &&
    if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER &&
       (SDK_MMI_NUMBER & pScan->uiFistMode))                                    //������������
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;

        if(((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ) || ((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
           || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX) || ((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
           || ((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND))
        {
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "123", SDK_DISP_FDISP | SDK_DISP_RDISP);     //��ʾ��������
        }
    }
    else if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER &&
            (SDK_MMI_LETTER & pScan->uiFistMode))                           //����������ĸ
    {
        pScanMem->MmiCurrentMode = MODE_BIG_LET;
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "ABC", SDK_DISP_FDISP | SDK_DISP_RDISP);      //��ʾ������ĸ
    }
    else if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL &&
            (SDK_MMI_SYMBOL & pScan->uiFistMode))
    {
        pScanMem->MmiCurrentMode = MODE_SYMBOL;
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "1:~ 2:! 3:@ 4:# 5:$ ", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);     //fusuipu 2013.06.07 9:30
    }

    /*====================== END======================== */
    else                                                //��������
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;
        //    Private_sdkDispRow(SDK_DISP_LINE5, 0, "123", SDK_DISP_FDISP | SDK_DISP_RDISP);  //fusuipu 2013.06.07 9:30
    }
    sdkKbKeyFlush();                                        //������̻���
//    dev_ts_flush();//shijianglong 2012.12.18 10:49 K390������ʱ����
    sdkMmiClearKey(pScanMem);
    pScanMem->MmiIsInput = 0;                           //�������뷨״̬
    pScanMem->MmiIsLetterEnd = 0;
    pScanMem->MmiCurrentRow = 1;

    if(NULL != pScan->sdkMmiDispCallFun)    //����лص�����������ʾ���ֵ�Ȩ�����׽����ص���ʾ����
    {
        if(pScanMem->MmiCurrentMode == MODE_T9)
        {
            memcpy(pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        }
        ret = pScan->sdkMmiDispFun(pScan->Contents, pScan->Mask, pScan->RowNo, pScan->pVar);

        if(SDK_OK == ret)                   //����ص���ʾ����ֵΪSDK_OK,˵���ص���ʾ��������������ص���������ʾ��������������
        {
            return ret;
        }
    }

    if((pScan->Contents[0] & 0x80) == 0x80)             //�����г�ʼ������ʾ����
    {
        Private_sdkDispRowEx(pScan->RowNo, 0, &pScan->Contents[1], disp | align);
        memset(pScan->Contents, 0, sizeof(pScan->Contents));
    }
    else
    {
        len = pScan->Contents[0];
        memcpy(pScan->Contents, &pScan->Contents[1], len);
        pScan->Contents[len] = 0;

        if(pScanMem->MmiCurrentMode == MODE_T9)
        {
            memcpy(pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        }
        sdkMmiCountLine(pScan->Contents, 0, align, *pScanMem);
        sdkMmiChangeLine(0, pScan);

        if(strlen(pScan->Contents) == 0)
        {
            if((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME)                            //�Ƿ��������� �� С��
            {
                Private_sdkDispRowEx(pScan->RowNo, 0, SYS_TIME_FORMAT, disp | align);     //�����涨�ĸ�ʽ��ʾ��ʾ
            }
            else if((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)                     //�Ƿ��������� �� С��
            {
                Private_sdkDispRowEx(pScan->RowNo, 0, "0.00", disp | align);              //�����涨�ĸ�ʽ��ʾ��ʾ
            }
            else
            {
                Private_sdkDispRowEx(pScan->RowNo, 0, STR_CURSOR, disp | align);          //��ʾ���
            }
        }
        else                                                                    //ԭ���������ĳ��Ȳ��� 0
        {
            sdkMmiDisplayScanf(pScan);
        }
    }
    return SDK_OK;
}

static void sdkMmiDisplayPwdAgain(const SDK_KEY_SCANFMT *pScan)
{
    u8 align, disp;
    u8 disbuf[128];

    u32 len = strlen((char*)pScan->Contents);

    memset(disbuf, 0, sizeof(disbuf));

    if((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT)
    {
        align = SDK_DISP_LDISP;
    }
    else
    {
        align = SDK_DISP_RDISP;
    }

    if((pScan->Mask & SDK_MMI_DISP) == SDK_MMI_DISP)
    {
        disp = SDK_DISP_NOFDISP;
    }
    else
    {
        disp = SDK_DISP_FDISP;
    }

    if(len > 0)
    {
        if(len <= 20)
        {
            memset(disbuf, '*', len);
        }
        else
        {
            memset(disbuf, '*', 20);
        }
    }
    else
    {
        strcpy(disbuf, STR_CURSOR);               //20090402
    }
    Private_sdkDispClearRowRam(pScan->RowNo);               //�����ض����ַ�
    Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, disbuf, disp | align) == SDK_OK);
    sdkDispBrushScreen();
}

static void sdkMmiLetter(u8 key, SDK_KEY_SCANFMT *pScan)
{
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    memset(pScanMem->MmiKeyBuf, 0, SDK_KEY_BUF_SIZE);
    memcpy(pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
    pScanMem->MmiKeyLen = (u8)strlen(pScan->Contents);


    sdkMmiGetLetterKey(key, pScanMem->MmiKeyBuf, &pScanMem->MmiKeyLen, 0, pScan->MaxLen, pScanMem->MmiCurrentMode, 0, pScan);         //��˹��


    /*=======BEGIN: fusuipu 2013.04.14  19:51 modify===========*/
    if(pScanMem->MmiKeyLen <= pScan->MaxLen)
    {
        sdkSysBeep(0);
        memcpy(pScan->Contents, pScanMem->MmiKeyBuf, pScanMem->MmiKeyLen);
    }
    /*====================== END======================== */
    else
    {
        sdkSysBeep(1);
        pScanMem->MmiIsLetterEnd = 1;
    }
    sdkMmiChangeLine(0, pScan);
    sdkMmiDisplayScanf(pScan);
}

static void sdkMmiSymbol(u8 key, SDK_KEY_SCANFMT *pScan)
{
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;
    //  u8 i, dispbuf[SDK_MAX_LINE_CHAR+1] = {0}, temp[2] = {0}, temp2[5] = {0};
    u8 i, temp[2] = {0}, temp2[5] = {0};
    u8 dispbuf[128] = {0}; //shiweisong 2012.12.26 10:35  �ƸߵƷ��������д���

    for(i = 0; i < 5; i++)
    {
        if(Sdk_SpecSymbol[pScanMem->MmiKeyLen + i] == 0)
        {
            break;
        }
        sprintf(temp2, "%i:%c ", i + 1, Sdk_SpecSymbol[pScanMem->MmiKeyLen + i]);
        strcat(dispbuf, temp2);
        Trace("fusuipu", "strlen = %d \r\nline len = %d\r\n", strlen(dispbuf), Private_sdkDispLineMaxLen());
    }

    Private_sdkDispRow(SDK_DISP_LINE5, 0, dispbuf, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);    //�����ض����ַ�

    if(key)
    {
        if(key <= (i + 0x30) && key >= '1')
        {
            sdkSysBeep(0);
            temp[0] = key;
            i = (u8)atoi(temp) - 1;
            pScan->Contents[strlen(pScan->Contents)] = Sdk_SpecSymbol[pScanMem->MmiKeyLen + i];
        }
        else if(key == SDK_KEY_DOWN)
        {
            sdkSysBeep(0);
            i = pScanMem->MmiKeyLen + 5;

            if(i >= strlen(Sdk_SpecSymbol))
            {
                pScanMem->MmiKeyLen = 0;
            }
            else
            {
                pScanMem->MmiKeyLen = i;
            }
            sdkMmiSymbol(0, pScan);
        }
        else if(key == SDK_KEY_UP)
        {
            if(pScanMem->MmiKeyLen == 0)
            {
                sdkSysBeep(1);
            }
            else
            {
                sdkSysBeep(0);
                pScanMem->MmiKeyLen -= 5;
                sdkMmiSymbol(0, pScan);
            }
        }
        else
        {
            sdkSysBeep(1);
        }
        sdkMmiChangeLine(0, pScan);
        sdkMmiDisplayScanf(pScan);
    }
}

void sdkMmiChangeModeToSym(SDK_KEY_SCANFMT *pScan)
{
    pScan->ScanMem.MmiCurrentMode = MODE_SYMBOL;
    Private_sdkDispRow(SDK_DISP_LINE5, 0, " ", SDK_DISP_FDISP | SDK_DISP_RDISP);
    sdkMmiSymbol(0, pScan);
}

static s32 sdkConverScanf(const u32 overtime, SDK_KEY_SCANFMT *pScan)
{
    //static SDK_KEY_STROKEPOSGROUP posgroup;//huangkanghui 2015.02.27 10:48
    //static SDK_KEY_STROKEHZBUF hzbuffer;        //huangkanghui 2015.02.27 10:48
    u8 key, flag;
    u16 tempflag;
    u32 timerID;
    s32 ret = 0;

    //u8 i = 0;
    //u8 tmpkeytab[32] = {0};

    // ��������Ϸ����ж�
    if (NULL == pScan)
    {
        Assert(0);
        return SDK_ERR;
    }
	sdkTimerStar(800);
    //ret = sdkMmiScanfINIT(pScan, &posgroup, &hzbuffer);
    ret = sdkMmiScanfINIT(pScan, NULL, NULL);     //huangkanghui 2015.02.27 10:51

    if(SDK_OK != ret)
    {
        return ret;
    }

    while(1)
    {
        tempflag = 0;
        key = (u8)sdkKbGetKey(); //huacong 2013.04.07 9:50 ��ʯγ��Ҫ�󽫰���ֵ�޸�ΪUpkey
        //key = Private_sdkGetKeyValue();

        if((sdkTimerIsEnd() == 1) &&
           ((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD) &&
           (pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET || pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET) &&
           (NULL == pScan->sdkMmiDispCallFun)) //fusuipu 2014.04.01 20:59 ����лص���������ʾ��ʽ��ȫ�ɿ����߿���
        {
            sdkMmiDisplayPwdAgain(pScan);                           //wanggaodeng add
        }

        if(key > 0)
        {
            tempflag = 1;
#if 0 /*Modify by huacong at 2013.04.07  9:52 */
            Trace("sdkmmi", "tmpkey=%02x\r\n", key);

            for(i = 0; i < 32; i++)
            {
                if(key == tmpkeytab[i])
                {
                    tempflag = 1;
                    break;
                }
            }

#endif /* if 0 */
        }

        if( overtime != 0) //shiweisong 2012.12.21 10:35 ���ʱ��Ϊ0�Ͳ���ʱ
        {
            /*=======BEGIN: ����20121106 modify===========*/
            if( sdkTimerIsEnd())   //��ʱ���س�ʱ
            {
                sdkMmiClearKey(&pScan->ScanMem);
                memset(pScan->Contents, 0, sizeof(pScan->Contents));
                return SDK_TIME_OUT;
            }
            /*================ END================== */
        }

        if(key == SDK_KEY_ESC) //fusuipu 2013.04.15 11:2 ��K501AM����Ӧ�˳���
        {
            sdkMmiClearKey(&pScan->ScanMem);
            memset(pScan->Contents, 0, sizeof(pScan->Contents));
            return key;
        }

        if(tempflag > 0)
        {

            if(((key == SDK_KEY_UP) || (key == SDK_KEY_DOWN)) && (!pScan->ScanMem.MmiIsInput))
            {
                if(1)               //�Ƿ������¼�
                {
                    if((pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET) || (pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET))
                    {
                        if((strlen(pScan->Contents) >= pScan->MaxLen) || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX))
                        {
                            continue;
                        }
                        else
                        {
                            if(key == SDK_KEY_UP)
                            {
                                pScan->Contents[strlen(pScan->Contents)] = '*';
                            }
                            else
                            {
                                pScan->Contents[strlen(pScan->Contents)] = '#';
                            }
                            sdkMmiDisplayScanf(pScan);
                            continue;
                        }
                    }
                }

                if(pScan->ScanMem.MmiCurrentMode != MODE_SYMBOL)
                {
                    if(key == SDK_KEY_UP)
                    {
                        flag = sdkMmiChangeLine(-1, pScan);
                    }
                    else
                    {
                        flag = sdkMmiChangeLine(1, pScan);
                    }

                    sdkMmiDisplayScanf(pScan);
                    continue;
                }
            }

            if(key == SDK_KEY_FUNCTION || key == SDK_KEY_ALPHA || key == SDK_KEY_MULTITASK)
            {
                if(((pScan->Mask & SDK_MMI_MULTTASK) == SDK_MMI_MULTTASK) && ((key == SDK_KEY_FUNCTION || key == SDK_KEY_MULTITASK)))
                {
                    if(key == SDK_KEY_FUNCTION)
                    {
                        return SDK_KEY_FUNCTION;
                    }
                    else
                    {
                        return SDK_KEY_MULTITASK;
                    }
                }
                else
                {
                    if(key == SDK_KEY_FUNCTION || key == SDK_KEY_ALPHA)
                    {
                        pScan->ScanMem.MmiIsInput = 0;
                        flag = sdkMmiChangeMode(pScan, NULL, NULL);

                        sdkMmiChangeLine(0, pScan);
                        sdkMmiDisplayScanf(pScan);
                        if(MODE_HAND == pScan->ScanMem.MmiCurrentMode)
                        {
                            return MODE_HAND;
                        }
                    }
                    continue;
                }
            }

            if((key == SDK_KEY_CLEAR) && (!pScan->ScanMem.MmiIsInput))
            {
                flag = sdkMmiDelete(pScan);
                sdkMmiChangeLine(0, pScan);
                sdkMmiDisplayScanf(pScan);
                continue;
            }

            if((key == SDK_KEY_BACKSPACE) && (!pScan->ScanMem.MmiIsInput))
            {
                flag = sdkMmiDelete(pScan);
                sdkMmiChangeLine(0, pScan);
                sdkMmiDisplayScanf(pScan);
                continue;
            }

            if((key == SDK_KEY_ENTER) && (!pScan->ScanMem.MmiIsInput))
            {
                if(strlen(pScan->Contents) == 0)
                {
                    if(pScan->MinLen == 0 || ((pScan->Mask & SDK_MMI_NOINPUT_QUIT) == SDK_MMI_NOINPUT_QUIT))
                    {
                        return SDK_KEY_ENTER;
                    }
                    continue;
                }
                else if(strlen(pScan->Contents) < pScan->MinLen)
                {
                    continue;
                }
                else
                {
                    return SDK_KEY_ENTER;
                }
            }

            if(strlen(pScan->Contents) >= pScan->MaxLen)
            {
                /*=======BEGIN: ����20120727 modify===========*/
                if(pScan->MaxLen == 1 && pScan->MinLen == 1 &&
                   pScan->ScanMem.MmiCurrentMode == MODE_NUMBER)
                {
                    if(key >= '0' && key <= '9')
                    {
                        sdkMmiDelete(pScan);
                        pScan->Contents[strlen(pScan->Contents)] = key;
                        sdkMmiChangeLine(0, pScan);
                        sdkMmiDisplayScanf(pScan);
                        continue;
                    }
                }
                /*================ END================== */

                if(!(((pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET) || (pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET))
                     && (!pScan->ScanMem.MmiIsLetterEnd)))
                {
                    continue;
                }
            }

            if((strlen(pScan->Contents) + 1 >= pScan->MaxLen)
               && ((pScan->ScanMem.MmiCurrentMode == MODE_CODE_D) || (pScan->ScanMem.MmiCurrentMode == MODE_T9)))
            {
                continue;
            }

            if((strlen(pScan->Contents) == 0) && (key == '0'))
            {
                if((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)
                {
                    continue;
                }
            }

            if(pScan->ScanMem.MmiCurrentMode == MODE_SYMBOL)
            {
                sdkMmiSymbol(key, pScan);
                continue;
            }
            else if((pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET) ||
                    (pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET))
            {
                if((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)
                {
                    if(((key >= '4') && (key <= '9') ) || (key == '1') || (key == '0'))          //cwDebug 2009-06-23
                    {
                        if(strlen(pScan->Contents) >= pScan->MaxLen)
                        {
                            pScan->ScanMem.MmiIsLetterEnd = 1;
                        }
                        else
                        {
                            pScan->Contents[strlen(pScan->Contents)] = key;
                            sdkMmiChangeLine(0, pScan);
                            sdkMmiDisplayScanf(pScan);
                        }
                        continue;
                    }
                }
                sdkMmiLetter(key, pScan);
            }
            else    // if(pScan->ScanMem.MmiCurrentMode == MODE_NUMBER)
            {
                if(key >= '0' && key <= '9')
                {
                    pScan->Contents[strlen(pScan->Contents)] = key;
                    sdkMmiDisplayScanf(pScan);
                    continue;
                }
                else
                {
                    continue;
                }
            }
        }
    }
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���봦��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkKbGetScanf(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen, const u32 uiMode, const s32 siDispRow)
{
    return sdkKbGetScanfEx(siOvertime, pheOut, siMinLen, siMaxLen, uiMode, siDispRow, NULL, NULL, 0);
}

s32 sdkIsMultiLang(void)
{
    return false;
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���봦��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkKbGetScanfEx(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen, const u32 uiMode, const s32 siDispRow, s32 (*psdkCallFun)(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow, void *pVar), void *pVarible, u32 siFirstMode)
{
    SDK_KEY_SCANFMT *pScan;
    s32 rslt = 0;
    s32 lang, line;

    if(pheOut == NULL  || siMinLen > SDK_MAX_STR_LEN || siMaxLen > SDK_MAX_STR_LEN || siMinLen > siMaxLen  || (!(siDispRow  < SDK_DISP_LINE_MAX)) || 0 == siMaxLen)
    {
		Trace("test", "error flag1\r\n");
        return SDK_PARA_ERR;
    }

    if ((SDK_DISP_LINE5 == siDispRow))
    {
        if ( ((uiMode & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL) ||
             ((siFirstMode & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL) )
        {
			Trace("test", "error flag2\r\n");
            return SDK_PARA_ERR;
        }
    }
    /*====================== END======================== */

    pScan = (SDK_KEY_SCANFMT *)sdkGetMem(sizeof(SDK_KEY_SCANFMT));         //�����ڴ�

    if(pScan == NULL)
    {
		Trace("test", "error flag3\r\n");
        return SDK_ERR;
    }
    memset(pScan, 0, sizeof(SDK_KEY_SCANFMT));


    if(pheOut[0] > siMaxLen) //shijianglong 2013.02.22 10:4�������ߵƽ������Ӵ��ж�
    {
		Trace("test", "error flag4\r\n");
        sdkFreeMem(pScan);
        return SDK_PARA_ERR;
    }

    pScan->Contents[0] = pheOut[0];
    memcpy(&pScan->Contents[1], &pheOut[1], pheOut[0]);

    pScan->MinLen = (u8)siMinLen;
    pScan->MaxLen = (u8)siMaxLen;
    pScan->Mask = uiMode;
    pScan->RowNo = (u8)siDispRow;
    pScan->uiFistMode = siFirstMode;
    pScan->sdkMmiDispCallFun = psdkCallFun;
    pScan->pVar = pVarible;
    /*=======BEGIN: fusuipu 2013.12.15  17:27 modify ����ϵͳ����������Ϣ===========*/
    lang = 0;
    line = 5;
    /*====================== END======================== */

    if(true == sdkIsMultiLang())
    {
        Trace("fsp", "reay sdkMmiMultiConver");
        rslt = SDK_ERR;
    }
    else
    {
        rslt = sdkConverScanf((u32)siOvertime, pScan);
    }

    if(rslt == SDK_KEY_ENTER || rslt == SDK_KEY_FUNCTION)
    {
        memset(&pheOut[1], 0, pheOut[0]); //tjb 2014.06.04 9:29 ��pheOut�г�ֵʱ���������
        pheOut[0] = (u8)strlen(pScan->Contents);
        memcpy(&pheOut[1], pScan->Contents, pheOut[0]);
    }

    sdkFreeMem(pScan);
    return rslt;
}

