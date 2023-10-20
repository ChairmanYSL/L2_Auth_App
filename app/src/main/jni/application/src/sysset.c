#include "appglobal.h"

#define APP_VERSION     "Vxx0000020160426100"  //wsm 20130903
#define PINPAD_VERSION "Vxx00001130903100"

_SimData gSimData;

void DispClearScreenRam(void)
{
    sdkDispClearRowRam(SDK_DISP_LINE1);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE2);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE3);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE4);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE5);              //�������?
}

void DispClearContentRam(void)
{
    sdkDispClearRowRam(SDK_DISP_LINE2);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE3);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE4);              //�������?
    sdkDispClearRowRam(SDK_DISP_LINE5);              //�������?
}

u8 *ParseDispLineData(u8 *pasOut, const u8 ucMaxOutSize, const u8 *pasIn)
{
    u8 *p = NULL, *q = NULL;
    u8 uc_len = 0, uc_size = 0;

    uc_size = ucMaxOutSize;

    if (pasIn == NULL)
    {
        return NULL;
    }
    q = (u8 *)pasIn;

    p = strstr(q, "\n");

    if (p == NULL)
    {
        strncpy(pasOut, pasIn, uc_size);
        return NULL;
    }

    if (q != NULL && p != NULL)
    {
        if (p > q)
        {
            uc_len = p - q;

            if (uc_len > uc_size)
            {
                uc_len = uc_size;
            }
            memcpy(pasOut, q, uc_len);
            return p + 1;
        }
    }
    return NULL;
}

void DispLineData(const u8 *pasIn)
{
    u8 uc_line;
    u8 uc_atr = SDK_DISP_INCOL;

    if (pasIn && strlen(pasIn) > 2)
    {
        switch (pasIn[0])
        {
             case '1':
             case '2':
             case '3':
             case '4':
             case '5':
               uc_line = (pasIn[0] - 0x31);
               break;

             default:
               uc_line = SDK_DISP_LINE2;
               break;
        }

        switch (pasIn[1])
        {
             case 'R':
             case 'r':
               uc_atr |= SDK_DISP_RDISP;
               break;

             case 'C':
             case 'c':
               uc_atr |= SDK_DISP_CDISP;
               break;

             default:
               uc_atr |= SDK_DISP_LDISP;
               break;
        }

        if (pasIn[2] != 'N' && pasIn[2] != 'n')
        {
            uc_atr |= SDK_DISP_FDISP;
        }
        else
        {
            uc_atr |= SDK_DISP_NOFDISP;
        }
        sdkDispFillRowRam(uc_line, 0, &pasIn[3], uc_atr);
    }
}

bool SetSystemTime(void)
{
    u8 tmp[64] = {0}, lcdtemp[64] = {0};
    s32 key = 0;
    u8 keybuf[64] = {0}, dispbuf[64] = {0}, timeformat[64] = {0};
    u8 i, j;

    bool flag = true;

    strcpy(timeformat, "____-__-__ __:__:__");
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Date&Time Set", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Current Date&Time:", SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "Pls Set Date&Time:", SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, timeformat, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    while (1)
    {
        memset(tmp, 0, sizeof(tmp));
        sdkGetRtc(tmp);
        memset(lcdtemp, 0, sizeof(lcdtemp));

        if (tmp[0] < 0x50) //2000��֮��
        {
            sprintf(lcdtemp, "20%02x-%02x-%02x %02x:%02x:%02x", tmp[0], //��ʾ����ϵͳʱ��
                    tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
        }
        else //2000��֮ǰ
        {
            sprintf(lcdtemp, "19%02x-%02x-%02x %02x:%02x:%02x", tmp[0], //��ʾ����ϵͳʱ��
                    tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
        }
        sdkDispRow(SDK_DISP_LINE3, 0, lcdtemp, SDK_DISP_DEFAULT);
        flag = false;
        key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);

        if (key > 0)
        {
            if (key == SDK_KEY_ENTER)
            {
                memset(tmp, 0, sizeof(tmp));

                if (strlen(keybuf))
                {
                    if (strlen(keybuf) == 14)
                    {
                        memcpy(tmp, keybuf, 4);

                        if (atoi(tmp) > 1950 && atoi(tmp) < 2049)
                        {
                            memset(tmp, 0, sizeof(tmp));
                            sdkAscToBcd(tmp, &keybuf[2], 12);

//                            if (sdkJudgeTime(tmp, 6))
//                            {
////                                sdkSetRtc(tmp);
//                                return true;
//                            }
                        }
                    }

                    if (flag)
                    {
                        sdkDispRow(SDK_DISP_LINE5, 0, "format error", SDK_DISP_DEFAULT);
                        sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);
                    }
                }
                else
                {
                    return true;
                }
            }
            else if (key == SDK_KEY_ESC)
            {
                return false;
            }
            else if ((key >= SDK_KEY_0 && key <= SDK_KEY_9) || key == SDK_KEY_BACKSPACE)
            {
                flag = true;

                if (key == SDK_KEY_BACKSPACE)
                {
                    if (strlen(keybuf) > 0)
                    {
                        keybuf[strlen(keybuf) - 1] = '\0';
                    }
                    else
                    {
                    }
                }
                else
                {
                    if (strlen(keybuf) < 14)
                    {
                        keybuf[strlen(keybuf)] = key;
                    }
                    else
                    {
                    }
                }
            }
        }

        if (flag)
        {
            strcpy(dispbuf, timeformat);

            for (i = 0, j = 0; i < strlen(keybuf); i++, j++)
            {
                if (i == 4 || i == 6 || i == 8 || i == 10 || i == 12)
                {
                    j++;
                }
                dispbuf[j] = keybuf[i];
            }

            sdkDispRow(SDK_DISP_LINE5, 0, dispbuf, SDK_DISP_DEFAULT);
        }
    }
}

void InitSimData(_SimData *SimData)
{
    memset(SimData, 0, sizeof(_SimData));                                               //���Sim������

    memset(SimData->TerminalNo, 0x30, 8);                                //Ĭ���ն˺�
    memset(SimData->MerchantNo, 0x30, 15);                       //Ĭ���̻���
    memcpy(SimData->IfnNo, "Terminal", 8);
    strcpy((char *)SimData->MerchantCName, "XGD");          //sxl arm_linux					//Ĭ���̻�����
    strcpy((char *)SimData->MerchantEName, "XGD");          //sxl arm_linux					//Ĭ���̻�Ӣ������
    SimData->BisSupportPrint = 0x00;
	SimData->ManualCashFlag = 0; //MAnual cash processing restrict flag ???AUC???
	SimData->CashBackFlag = 0;  //CashBack processing restrict flag ???AUC???

    SimData->AutoToolDispTimeMs=100;//luohuidong 2018.09.03 14:26
	SimData->JcbImplementationOption = 0xFC;//jcb initial ImplementationOption emv/legacy mode/ oda exceptfile issuer msd mode

    SimData->AppType = EMVTYPE; //TermInfo.AppType
    SimData->TransType = 0x00; //TermInfo.TransType
    SimData->bForceOnline = false; //TermInfo.bForceOnline
    SimData->bBatchCapture = false; //TermInfo.bBatchCapture
    SimData->ODAalgrthFlag = true; //TermInfo.ODAalgrthFlag
    memcpy(SimData->VocherNo, "\x00\x00\x00\x01", 4);
	SimData->TCPPort = 8182;
	memcpy(SimData->IPAddress, "192168250093", 12);
}

void SaveSimData(_SimData *SimData)
{
    unsigned char *temp;
    s32 ret;
    u8 fn[64] = {0};

    sdkSysGetCurAppDir(fn);
    strcat(fn, "SimData");

    temp = (u8 *)sdkGetMem(sizeof(_SimData));

    memset(temp, 0, sizeof(_SimData));                                                //����
    memcpy(temp, (unsigned char *)SimData, sizeof(_SimData));        //��������
    ret = sdkInsertFile(fn, temp, 0, sizeof(_SimData));                    //д����
    sdkFreeMem(temp);

    memcpy(&gSimData, SimData, sizeof(_SimData));
}

void ReadSimData(_SimData *SimData)
{
    unsigned char *temp;
    s32 len = sizeof(_SimData);
    u8 fn[64] = {0};

    sdkSysGetCurAppDir(fn);
    strcat(fn, "SimData");

    temp = (u8 *)sdkGetMem(sizeof(_SimData));

	Trace("Simdata", "want read len = %d\r\n", len);
    if(sdkReadFile(fn, temp, 0, &len) == SDK_OK)
    {
        Trace("emv","---------------ReadSimData OK\r\n");
        memcpy((u8 *)SimData, temp, sizeof(_SimData));                                    //�ָ�Sim������
        memcpy(&gSimData, SimData, sizeof(_SimData));
        sdkFreeMem(temp);
    }
    else
    {
        Trace("emv","---------------ReadSimData fail\r\n");
        sdkFreeMem(temp);
        InitSimData(SimData);                                                             //��ʼ��Sim����
        SaveSimData(SimData);                                                             //����Sim������
    }
}

void IncVoucherNo()
{
    _SimData SimData;

    memset(&SimData, 0, sizeof(SimData));
    ReadSimData(&SimData);

    sdkBcdAdd(SimData.VocherNo, SimData.VocherNo, 4, "\x00\x00\x00\x01", 4);
    SaveSimData(&SimData);

}
void InitTransData()
{
    AmtTrans = 0;
    AmtNet = 0;
    TransNum = 0;
    TransIndex = 0;
}

void DispTradeTitle(u8 const *pasTradehead)
{
    u32 displine = SDK_DISP_LINE1;  //�̶���ʾ��һ��

    if (pasTradehead == NULL)
    {
        Assert(0);
        return;
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(displine, 0, pasTradehead, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
}

u32 GetKeyMask(u8 nMin, u8 nMax)
{
    u8 i;
    u32 nMaskTmp = 0;
    u32 nMask[10] =
    {
        SDK_KEY_MASK_0,
        SDK_KEY_MASK_1,
        SDK_KEY_MASK_2,
        SDK_KEY_MASK_3,
        SDK_KEY_MASK_4,
        SDK_KEY_MASK_5,
        SDK_KEY_MASK_6,
        SDK_KEY_MASK_7,
        SDK_KEY_MASK_8,
        SDK_KEY_MASK_9
    };

    for (i = nMin; i <= nMax; i++)
    {
        nMaskTmp |= nMask[i];
    }

    return nMaskTmp;
}

s32 Inputstr(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData, u8 nMin, u8 nMax, u32 uiMask)
{
    u8 out[SDK_MAX_STR_LEN] = {0};
    s32 nRet = 0;
    u8 nLine = SDK_DISP_LINE1;

    if (pucData == NULL || nMin > nMax)
    {
        Assert(0);
        return -1;
    }
    DispClearScreenRam();

    if (pasCaption != NULL)
    {
        sdkDispFillRowRam(nLine++, 0, pasCaption, SDK_DISP_DEFAULT);
    }

    if (pasTag != NULL)
    {
        sdkDispFillRowRam(nLine++, 0, pasTag, SDK_DISP_LEFT_DEFAULT);
    }

    if (pasInfo != NULL) {sdkDispFillRowRam(SDK_DISP_LINE5, 0, pasInfo, SDK_DISP_DEFAULT); }
    sdkDispBrushScreen();

    if (nMax > SDK_MAX_STR_LEN) {nMax = SDK_MAX_STR_LEN; }
    memset(out, 0x00, sizeof(out));
    out[0] = strlen(pucData);

    if (out[0] > nMax) {out[0] = nMax; }
    memcpy(&out[1], pucData, out[0]);
    TraceHex("", "pucData", pucData, strlen(pucData));
    nRet = sdkKbGetScanf(TV_AUTORET, out, nMin, nMax, uiMask, nLine);

    if (nRet == SDK_KEY_ENTER)
    {
        memset(pucData, 0x00, nMax);
        memcpy(pucData, &out[1], out[0]);
    }
    return nRet;
}

s32 InputBcd(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData, u8 nSize, u8 nMax, bool bIsRightAlign, u32 ucMask)
{
    int i, j, nRet;
    u8 asTmp[SDK_MAX_STR_LEN] = {0};

    if (pucData == NULL || nSize == 0 || nMax > 2 * nSize)
    {
        Assert(0);
        return -1;
    }
    ucMask |= SDK_MMI_NUMBER;
    sdkBcdToAsc(asTmp, pucData, nSize);

    if (bIsRightAlign)
    {
        nRet = 2 * nSize;

        for (i = 0; nRet; i++)
        {
            if (asTmp[i] != 0x30)
            {
                break;
            }
        }

        j = 1;

        for (; i < nRet; i++)
        {
            asTmp[j++] = asTmp[i];
        }

        j--;
    }
    else{j = nMax; }
    memset(&asTmp[j], 0x00, sizeof(asTmp) - j);

    nRet = Inputstr(pasCaption, pasTag, pasInfo, asTmp, 1, nMax, ucMask);

    if (nRet == SDK_KEY_ENTER)
    {
        if (bIsRightAlign)
        {
            sdkAscToBcdR(pucData, asTmp, nSize);
        }
        else
        {
            i = strlen(asTmp);

            if (i > nMax) {i = nMax; }
            sdkAscToBcd(pucData, asTmp, i);
        }
    }
    return nRet;
}

/*****************************************************************************
** Descriptions:	����true or false
** Parameters:          u8 *pasCaption  ��ʾ̧ͷ
                               u8 *pasTag       ��ʾ��ǩ
                               u8 *pasInfo      ��ʾ��ʾ
                               u8 *pucData  Ԥ����������
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetTorF(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData)
{
    int nRet;
    u8 nLine = SDK_DISP_LINE1;
    u8 key[10] = {0};

    if (pucData == NULL)
    {
        Assert(0);
        return -1;
    }
    key[0] = *pucData;

    if (key[0] == 0 || key[0] == '0') {key[0] = '0'; }
    else{key[0] = '1'; }
    DispClearScreenRam();

    if (pasCaption != NULL) {sdkDispFillRowRam(nLine++, 0, pasCaption, SDK_DISP_DEFAULT); }

    if (pasTag != NULL) {sdkDispFillRowRam(nLine++, 0, pasTag, SDK_DISP_LEFT_DEFAULT); }

    if (pasInfo != NULL) {sdkDispFillRowRam(nLine++, 0, pasInfo, SDK_DISP_DEFAULT); }
    sdkDispFillRowRam(nLine, 0, key, SDK_DISP_RIGHT_DEFAULT);
    sdkDispBrushScreen();

    while (1)
    {
        nRet = sdkKbWaitKey(SDK_KEY_MASK_0 | SDK_KEY_MASK_1 | SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, TV_AUTORET);

        if (nRet == SDK_KEY_0 || nRet == SDK_KEY_1)
        {
            if (nRet == SDK_KEY_0)
            {
                key[0] = '0';
            }
            else{key[0] = '1'; }
            sdkDispRow(nLine, 0, key, SDK_DISP_RIGHT_DEFAULT);
        }
        else if (nRet == SDK_KEY_ENTER)
        {
            *pucData = key[0] - '0';
            break;
        }
        else{break; }
    }

    return nRet;
}

/*****************************************************************************
** Descriptions:	����һ������
** Parameters:          u8 *pasCaption  ��ʾ̧ͷ
                               u8 *pasTag       ��ʾ��ǩ
                               u8 *pasInfo  ��ʾ��ʾ
                               void *pucNum Ԥ����ʾ������
                               u8 ucNumSize ���ֵĴ�С
                               u8 nMin      ��С����
                               u8 nMax      ��󳤶�?
                               u32 uiMask   ��������
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 InputNum(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, void *pucNum, u8 ucNumSize, u32 nMin, u32 nMax, u32 ucMask)
{
    int nRet = 0;
    u8 asTmp[SDK_MAX_STR_LEN] = {0};
    u8 ucMin = 0, ucMax = 0;
    u8 ucNum8;
    u16 ucNum16;
    u32 ucNum32;

    if (nMin > nMax || (ucNumSize != sizeof(u8) && ucNumSize != sizeof(u16) && ucNumSize != sizeof(u32)))
    {
        Assert(0);
        return -1;
    }
    ucMask |= SDK_MMI_NUMBER;

    ucMin = 1;
    ucNum32 = nMin;
    ucMin = (u8)sprintf(asTmp, "%u", ucNum32);

    if (ucNum32 == 0) {ucMin = 0; }
    ucMax = 1;
    ucNum32 = nMax;
    ucMax = (u8)sprintf(asTmp, "%u", ucNum32);

    if (ucNum32 == 0) {ucMax = 0; }

    switch (ucNumSize)
    {
         case sizeof(u8):
           ucNum32 = *((u8 *)pucNum);
           break;

         case sizeof(u16):
           ucNum32 = *((u16 *)pucNum);
           break;

         default:
           ucNum32 = *((u32 *)pucNum);
           break;
    }

    memset(asTmp, 0x00, sizeof(asTmp));

    if (ucNum32 > 0) {sprintf(asTmp, "%u", ucNum32); }

    while (1)
    {
        nRet = Inputstr(pasCaption, pasTag, pasInfo, asTmp, ucMin, ucMax, ucMask);

        if (nRet == SDK_KEY_ENTER)
        {
            asTmp[ucMax] = '\0';
            ucNum32 = atoi(asTmp);

            if (ucNum32 < nMin || ucNum32 > nMax)
            {
                if (ucNum32 < nMin) {sprintf(asTmp, "����С��%u", nMin); }
                else{sprintf(asTmp, "���ܴ���%u", nMax); }
                sdkDispRow(SDK_DISP_LINE3, 0, asTmp, SDK_DISP_RIGHT_DEFAULT);
                memset(asTmp, 0x00, sizeof(asTmp));
                sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
                continue;
            }
            else
            {
                switch (ucNumSize)
                {
                     case sizeof(u8):
                       ucNum8 = ucNum32;
                       memcpy(pucNum, &ucNum8, sizeof(u8));
                       break;

                     case sizeof(u16):
                       ucNum16 = ucNum32;
                       memcpy(pucNum, &ucNum16, sizeof(u16));
                       break;

                     default:
                       memcpy(pucNum, &ucNum32, sizeof(u32));
                       break;
                }

                break;
            }
        }
        else
        {
            break;
        }
    }

    return nRet;
}

/*****************************************************************************
** Descriptions:	����TAC
** Parameters:          SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetTac(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
    int nRet;

    nRet = InputBcd("Set TAC", "TAC-Decline:", NULL, pstOutAIDList->TacDecline, 5, 10, false, SDK_MMI_HEX);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = InputBcd("Set TAC", "TAC-Online:", NULL, pstOutAIDList->TacOnline, 5, 10, false, SDK_MMI_HEX);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = InputBcd("Set TAC", "TAC-Default:", NULL, pstOutAIDList->TacDefault, 5, 10, false, SDK_MMI_HEX);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

/*****************************************************************************
** Descriptions:	����EMV���?
** Parameters:      u8 *pasCaption  ��ʾ̧ͷ
                               u8 *pasTag       ��ʾ��ǩ
                               u8 *pasInfo      ��ʾ��ʾ
                               u8 *pucData  Ԥ����������
                               u8 nSize     ������?
                               u8 nMin      ��С����
                               u8 nMax      ��󳤶�?

** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetEmvAmount(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData, u8 nSize, u32 nMin, u32 nMax)
{
    u32 nTmp = 0;
    int nRet;

    nTmp = 0;
    sdkBcdToU32(&nTmp, pucData, nSize);
    nRet = InputNum(pasCaption, pasTag, pasInfo, &nTmp, sizeof(nTmp), nMin, nMax, SDK_MMI_POINT);

    if (nRet == SDK_KEY_ENTER)
    {
        sdkU32ToBcd(pucData, nTmp, nSize);
    }
    return nRet;
}

/*****************************************************************************
** Descriptions:	����EMV���?
** Parameters:      u8 *pasCaption  ��ʾ̧ͷ
                               u8 *pasTag       ��ʾ��ǩ
                               u8 *pasInfo      ��ʾ��ʾ
                               u8 *pucData  Ԥ����������
                               u8 nSize     ������?
                               u8 nMin      ��С����
                               u8 nMax      ��󳤶�?

** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetEmvbAmount(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData, u8 nSize, u32 nMin, u32 nMax)
{
    u32 nTmp = 0;
    int nRet;

    nTmp = 0;
    sdkHexToU32(&nTmp, pucData, nSize);
    nRet = InputNum(pasCaption, pasTag, pasInfo, &nTmp, sizeof(nTmp), nMin, nMax, SDK_MMI_POINT);

    if (nRet == SDK_KEY_ENTER)
    {
        sdkU32ToHex(pucData, nTmp, nSize);
    }
    return nRet;
}

s32 SetEmvHexAmt(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData, u8 nSize, u32 nMin, u32 nMax)
{
    u32 nTmp = 0;
    int nRet;

    nTmp = 0;
    sdkHexToU32(&nTmp, pucData, nSize);
    nRet = InputNum(pasCaption, pasTag, pasInfo, &nTmp, sizeof(nTmp), nMin, nMax, SDK_MMI_POINT);

    if (nRet == SDK_KEY_ENTER)
    {
        sdkU32ToHex(pucData, nTmp, nSize);
    }
    return nRet;
}

/*****************************************************************************
** Descriptions:	����EMV�ն˲���
** Parameters:          SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetTrm(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
    int nRet;

//    TraceHex(__FUNCTION__, "pstOutAIDList->FloorLimit", pstOutAIDList->FloorLimit, 4);
//    TraceHex(__FUNCTION__, "pstOutAIDList->Threshold", pstOutAIDList->Threshold, 4);
//    TraceHex(__FUNCTION__, "pstOutAIDList->TargetPercent", &pstOutAIDList->TargetPercent, 1);
//    TraceHex(__FUNCTION__, "pstOutAIDList->MaxTargetPercent", &pstOutAIDList->MaxTargetPercent, 1);
//    TraceHex(__FUNCTION__, "pstOutAIDList->bShowRandNum", &pstOutAIDList->bShowRandNum, 1);
//    TraceHex(__FUNCTION__, "pstOutAIDList->termcvm_limit", pstOutAIDList->termcvm_limit, 6);
//    TraceHex(__FUNCTION__, "pstOutAIDList->clessofflineamt", pstOutAIDList->clessofflineamt, 6);
//    TraceHex(__FUNCTION__, "pstOutAIDList->clessofflinelimitamt", pstOutAIDList->clessofflinelimitamt, 6);
//    TraceHex(__FUNCTION__, "pstOutAIDList->vlptranslimit", pstOutAIDList->vlptranslimit, 6);

    nRet = SetEmvHexAmt("TRM", "FloorLimit:", NULL, pstOutAIDList->FloorLimit, 4, 0, 0xffffffff);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = SetEmvHexAmt("TRM", "Threshold:", NULL, pstOutAIDList->Threshold, 4, 0, 0xffffffff);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = InputNum("TRM", "TargetPercent:", NULL, &pstOutAIDList->TargetPercent, sizeof(u8), 0, 99, SDK_MMI_NUMBER);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = InputNum("TRM", "MaxTargetPercent:", NULL, &pstOutAIDList->MaxTargetPercent, sizeof(u8), 0, 99, SDK_MMI_NUMBER);

//    if (nRet != SDK_KEY_ENTER) {return nRet; }
//    nRet = SetTorF("TRM", "ShowRandNum:", "1-Yes 0-No", &pstOutAIDList->bShowRandNum);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = SetEmvAmount("Contactless Limit", "Contactless Limit:", NULL, pstOutAIDList->cl_offlinelimit, 6, 0, 0xffffffff);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = SetEmvAmount("CVM Limit", "CVM Limit:", NULL, pstOutAIDList->cl_cvmlimit, 6, 0, 0xffffffff);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = SetEmvAmount("Contactless FloorLimit", "Contactless FloorLimit:", NULL, pstOutAIDList->cl_translimit, 6, 0, 0xffffffff);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    nRet = SetEmvAmount("EC Trans Limit", "EC Trans Limit:", NULL, pstOutAIDList->ectranslimit, 6, 0, 0xffffffff);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

s32 SetTradeType(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
	return 0;
}
/*****************************************************************************
** Descriptions:	����EMVӦ�ð汾��
** Parameters:          SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetAppVer(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
    int nRet;

    nRet = InputBcd("App Ver", "App Ver:", NULL, pstOutAIDList->AppVerNum, 2, 4, false, SDK_MMI_HEX);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

/*****************************************************************************
** Descriptions:	����DDOL
** Parameters:          SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetDDOL(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
    int nRet;
    u8 asTmp[SDK_MAX_STR_LEN + 2];
    int i;

    memset(asTmp, 0x00, sizeof(asTmp));
    //lilin 2015.04.10 10:9
#if 0
    asTmp[0] = pstOutAIDList->TermDDOLLen * 2;
    sdkBcdToAsc(&asTmp[1], pstOutAIDList->TermDDOL, pstOutAIDList->TermDDOLLen);
#else
    sdkBcdToAsc(&asTmp[0], pstOutAIDList->TermDDOL, pstOutAIDList->TermDDOLLen);
#endif

    nRet = Inputstr("Set Default DDOLL", "Pls Set Default DDOLL:", NULL, asTmp, 2, 127, SDK_MMI_HEX);

    if (nRet == SDK_KEY_ENTER)
    {
        i = strlen(asTmp);

        if (i > 127) {i = 127; }
        sdkAscToBcd(pstOutAIDList->TermDDOL, asTmp, i);
        pstOutAIDList->TermDDOLLen = (i + 1) / 2;
    }
    return nRet;
}

/*****************************************************************************
** Descriptions:	����ASI����
** Parameters:          SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetAsi(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
    int nRet;

    nRet = SetTorF("AIS", "Exact Match:", "1-Exact 0-Part", &pstOutAIDList->Asi);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

/*****************************************************************************
** Descriptions:	�����Ƿ�֧����������
** Parameters:          SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 SetOnlinePin(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
    int nRet;

    nRet = SetTorF("Online Pin", "Support Online Pin:", "1-Yes 0-No", &pstOutAIDList->TermPinCap);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

/*****************************************************************************
** Descriptions:    ���ö��������ղ���
** Parameters:      SDK_EMVBASE_AID_STRUCT *pstOutAIDList
** Returned value:
** Created By:      sujianzhong 2016.10.18
** Remarks:
*****************************************************************************/
s32 SetRiskParam(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
#if 0
    int nRet;
    u8 bflag = 0;
	u8 paywavepredealparam[2];

	memset(paywavepredealparam, 0, sizeof(paywavepredealparam));
	EMVBaseGetValueFromTlv("\xDF\x03",paywavepredealparam, pstOutAIDList->reservedlen, pstOutAIDList->reserved);

    bflag = ((paywavepredealparam[0]&DEFAULTPAYWAVESTATUSCHECK)? 1 : 0);
    nRet = SetTorF("Risk Parameters", "Support Satus Check:", "1-Yes 0-No", &bflag);
    if (nRet != SDK_KEY_ENTER) {return nRet; }
    if(bflag)
    {
        paywavepredealparam[0] |= 0x80;
    }
    else
    {
        paywavepredealparam[0] &= 0x7F;
    }

    EMVBaseSetValueToTlv("\xDF\x03", 2, paywavepredealparam, pstOutAIDList->reservedlen, pstOutAIDList->reserved, sizeof(pstOutAIDList->reserved));
	TraceHex("","reserve", pstOutAIDList->reserved, pstOutAIDList->reservedlen);

    bflag = ((paywavepredealparam[0]&DEAULTPAYWAVEZEROCHECK)? 1 : 0);
    nRet = SetTorF("Risk Parameters", "Support Zero Check:", "1-Yes 0-No", &bflag);

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    if(bflag)
    {
        paywavepredealparam[0] |= 0x40;

        bflag = ((paywavepredealparam[0]&DEFAULTPAYWAVEZEROOPTION)? 1 : 0);
        nRet = SetTorF("Risk Parameters", "Zero Check Options:", "0-Online 1-App Not Allowed", &bflag);

        if (nRet != SDK_KEY_ENTER) {return nRet; }
        if(bflag)
        {
            paywavepredealparam[0] |= 0x01;
        }
        else
        {
            paywavepredealparam[0] &= 0xFE;
        }
    }
    else
    {
        paywavepredealparam[0] &= 0xBF;
    }

    EMVBaseSetValueToTlv("\xDF\x03", 2, paywavepredealparam, pstOutAIDList->reservedlen, pstOutAIDList->reserved, sizeof(pstOutAIDList->reserved));
	TraceHex("","reserve", pstOutAIDList->reserved, pstOutAIDList->reservedlen);

    bflag = ((paywavepredealparam[0]&DEFAULTPAYWAVERTCLCHECK)? 1 : 0);
    nRet = SetTorF("Risk Parameters", "Support ClessLimit:", "1-Yes 0-No", &bflag);
    if (nRet != SDK_KEY_ENTER) {return nRet; }
    if(bflag)
    {
        paywavepredealparam[0] |= 0x20;
    }
    else
    {
        paywavepredealparam[0] &= 0xDF;
    }

    EMVBaseSetValueToTlv("\xDF\x03", 2, paywavepredealparam, pstOutAIDList->reservedlen, pstOutAIDList->reserved, sizeof(pstOutAIDList->reserved));
	TraceHex("","reserve", pstOutAIDList->reserved, pstOutAIDList->reservedlen);

    bflag = ((paywavepredealparam[0]&DEFAULTPAYWAVEFLOORCHECK)? 1 : 0);
    nRet = SetTorF("Risk Parameters", "Support ClessFloorLimit:", "1-Yes 0-No", &bflag);
    if (nRet != SDK_KEY_ENTER) {return nRet; }
    if(bflag)
    {
        paywavepredealparam[0] |= 0x10;
    }
    else
    {
        paywavepredealparam[0] &= 0xEF;
    }

    EMVBaseSetValueToTlv("\xDF\x03", 2, paywavepredealparam, pstOutAIDList->reservedlen, pstOutAIDList->reserved, sizeof(pstOutAIDList->reserved));
	TraceHex("","reserve", pstOutAIDList->reserved, pstOutAIDList->reservedlen);

    bflag = ((paywavepredealparam[0]&DEFAULTPAYWAVECVMCHECK)? 1 : 0);
    nRet = SetTorF("Risk Parameters", "Support CVM Limit:", "1-Yes 0-No", &bflag);
    if (nRet != SDK_KEY_ENTER) {return nRet; }
    if(bflag)
    {
        paywavepredealparam[0] |= 0x08;
    }
    else
    {
        paywavepredealparam[0] &= 0xF7;
    }
    EMVBaseSetValueToTlv("\xDF\x03", 2, paywavepredealparam, pstOutAIDList->reservedlen, pstOutAIDList->reserved, sizeof(pstOutAIDList->reserved));
	TraceHex("","reserve", pstOutAIDList->reserved, pstOutAIDList->reservedlen);

    return nRet;
#endif
}

s32 SetTerminalType(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
//    int nRet;
//
//	APPEX_AID_STRUCT extempAid;
//	memset(&extempAid, 0, sizeof(APPEX_AID_STRUCT));
//	GetAPPEXAID(&extempAid,pstOutAIDList->Aid, pstOutAIDList->AidLen);
//
//    nRet = InputBcd("Terminal Type", "Terminal Type:", NULL, &extempAid.terminaltype, 2, 2, false, SDK_MMI_HEX);
//    if (nRet != SDK_KEY_ENTER) {return nRet; }
//
//	AddAPPEXAID(&extempAid);
//    return nRet;
}
s32 SetTerminalTTQ(SDK_EMVBASE_AID_STRUCT *pstOutAIDList)
{
//    int nRet;
//    u8 asTmp[SDK_MAX_STR_LEN + 2];
//    int i;
//
//    memset(asTmp, 0x00, sizeof(asTmp));
//    //lilin 2015.04.10 10:9
//#if 0
//    asTmp[0] = pstOutAIDList->TermDDOLLen * 2;
//    sdkBcdToAsc(&asTmp[1], pstOutAIDList->TermDDOL, pstOutAIDList->TermDDOLLen);
//#else
//	APPEX_AID_STRUCT extempAid;
//	memset(&extempAid, 0, sizeof(APPEX_AID_STRUCT));
//	GetAPPEXAID(&extempAid,pstOutAIDList->Aid, pstOutAIDList->AidLen);
//	sdkBcdToAsc(&asTmp[0], extempAid.TermTransPredicable, 4);
//	TraceHex("", "extempAid.TermTransPredicable", extempAid.TermTransPredicable, 4);
//	TraceHex("", "asTmp", asTmp, 8);
//#endif
//
//    nRet = Inputstr("Set Terminal TTQ", "Pls Set Terminal TTQ:", NULL, asTmp, 8, 8, SDK_MMI_HEX);
//
//    if (nRet == SDK_KEY_ENTER)
//    {
//        i = strlen(asTmp);
//
//        if (i > 8) {i = 8; }
//        sdkAscToBcd(extempAid.TermTransPredicable, asTmp, i);
//		AddAPPEXAID(&extempAid);
//        #ifdef EMVB_DEBUG
//            TraceHex(""," SetTerminalTTQ TermTransPredicable",pstOutAIDList->TermTransPredicable,4);
//        #endif
//    }
//    return nRet;
}
/*****************************************************************************
** Descriptions:	����EMV aid����
** Parameters:          u8 *ucAid aid����
                               u8 ucAidLen aid����
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 EmvAidParamSet(SDK_EMVBASE_AID_STRUCT *pstInAid)
{
    s32 nRet;
    u32 nMask = SDK_KEY_MASK_ESC | SDK_KEY_MASK_ENTER;
    SDK_EMVBASE_AID_STRUCT stOutAIDList;
    SDK_EMVBASE_AID_STRUCT stAIDTmp;

    TraceHex("emvaidparamset", "IC���������� AID", pstInAid->Aid, pstInAid->AidLen);

    memcpy(&stOutAIDList, pstInAid, sizeof(SDK_EMVBASE_AID_STRUCT));
    memcpy(&stAIDTmp, &stOutAIDList, sizeof(SDK_EMVBASE_AID_STRUCT));

    nMask |= GetKeyMask(0, 9);

    while (1)
    {
        DispClearScreenRam();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "1.Set TAC     ", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "2.TRM     ", SDK_DISP_RIGHT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "3.Trade Type  ", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "4.App Ver ", SDK_DISP_RIGHT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "5.Default DDOL", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "6.ASI     ", SDK_DISP_RIGHT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "7.Online Pin  ", SDK_DISP_LEFT_DEFAULT);
        //sdkDispFillRowRam(SDK_DISP_LINE5, 0, "8.��ʼ����", SDK_DISP_RIGHT_DEFAULT);
#ifdef RFD_AID
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "8.Risk Prm", SDK_DISP_RIGHT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "9.Terimal Type", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "0.TTQ     ", SDK_DISP_RIGHT_DEFAULT);//luohuidong 2018.09.01 15:38
#endif
        sdkDispBrushScreen();

        nRet = sdkKbWaitKey(nMask, TV_AUTORET);

        if (nRet > 0)
        {
            switch (nRet)
            {
                #if 1 /*Modify by luohuidong at 2018.09.01  15:37 */
                    case SDK_KEY_0:
                           nRet = SetTerminalTTQ(&stAIDTmp);
                        break;
                #endif /* if 1 */
                 case SDK_KEY_1:
                   nRet = SetTac(&stAIDTmp);
                   break;

                 case SDK_KEY_2:
                   nRet = SetTrm(&stAIDTmp);
                   break;

                 case SDK_KEY_3:
#ifdef RFD_AID
                    nRet = SetTradeType(&stAIDTmp);
#endif
                   break;

                 case SDK_KEY_4:
                   nRet = SetAppVer(&stAIDTmp);
                   break;

                 case SDK_KEY_5:
                   nRet = SetDDOL(&stAIDTmp);
                   break;

                 case SDK_KEY_6:
                   nRet = SetAsi(&stAIDTmp);
                   break;

                 case SDK_KEY_7:
                   nRet = SetOnlinePin(&stAIDTmp);
                   break;

#ifdef RFD_AID
                 case SDK_KEY_8:
                   nRet = SetRiskParam(&stAIDTmp);
                   break;

                 case SDK_KEY_9:
                   nRet = SetTerminalType(&stAIDTmp);
                    break;
#endif

                 default:
                   nRet = 0;
                   break;
            }

            if (nRet == 0)
            {
                break;
            }
        }
    }

    if (memcmp(&stAIDTmp, &stOutAIDList, sizeof(SDK_EMVBASE_AID_STRUCT)))
    {
        //sdkIccDelOneAID(ucAid, ucAidLen);
        sdkEMVBaseAddAnyAIDList(&stAIDTmp, 1);
    }
    return nRet;
}

/*****************************************************************************
** Descriptions:	��ѯAID�б�
** Parameters:          bool enSet = 1 �ܽ�ȥ�޸�AID��Ӧ�Ĳ���
                                     = 0 ֻ�ܲ鿴AID�����ܽ����޸ĺ���
** Returned value:
** Created By:		wuzhonglin2012.12.22
** Remarks:
*****************************************************************************/
s32 QueryAid(bool enSet)
{
    SDK_EMVBASE_AID_STRUCT *pstOutAIDList = NULL;
    u32 mask = SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 | SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC | SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_0;
    u32 key;  //��ֵ
    s32 iTotal = 0, iMaxLine = 0, iGetNum = 0, iClass = 0, i = 0, n = 0;
    s32 iRet = 0;
    bool bIsNewDetail = true;
    u16 uiAidStSize = sizeof(SDK_EMVBASE_AID_STRUCT);
    u8 buf[40] = {0};
    u8 aidLen;

    DispTradeTitle("Query AID");
    iRet = sdkEMVBaseGetAIDListNum(&iTotal);

    if (iRet != SDK_OK || iTotal == 0)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Get AID Fail!", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
        return SDK_ERR;
    }
    pstOutAIDList = (SDK_EMVBASE_AID_STRUCT *)sdkGetMem(uiAidStSize * 4);

    if (pstOutAIDList == NULL)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�ڴ����ʧ��?", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
        return SDK_ERR;
    }
    i = 0;
    iClass = 0;
    iGetNum = 0;
    memset(pstOutAIDList, 0x00, uiAidStSize * 4);
    iRet = sdkEMVBaseGetAnyAIDList(0, 4, pstOutAIDList, &iGetNum);

    if (iRet != SDK_OK || iGetNum == 0)
    {
        sdkFreeMem(pstOutAIDList);
        return SDK_ERR;
    }
    iMaxLine = iGetNum;

    while (1)
    {
        if (bIsNewDetail)
        {
            DispTradeTitle("Query AID");

            DispClearContentRam();

            for (n = 0; n < iMaxLine && n < 4; n++)
            {
                memset(buf, 0, sizeof(buf));
                buf[0] = 0x31 + n;
                buf[1] = '.';
                aidLen = pstOutAIDList[n].AidLen; //����Aid�ж��ٸ��ֽ�

                if (aidLen > 16) {aidLen = 16; }
                sdkBcdToAsc(&buf[2], pstOutAIDList[n].Aid, aidLen); //��AIDת����ASC����ʾ

                if (strlen(buf) > 21) //һ����ʾ����
                {
                    memcpy(&buf[18], "...", 3);  //��ʾʡ�Ժ�
                    buf[21] = 0; //�ַ�������
                }

                if (i == n)
                {
                    sdkDispFillRowRam(SDK_DISP_LINE2 + n, 0, buf, SDK_DISP_NOFDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                }
                else
                {
                    sdkDispFillRowRam(SDK_DISP_LINE2 + n, 0, buf, SDK_DISP_LEFT_DEFAULT);
                }
            }

            sdkDispBrushScreen();
        }
        key = sdkKbWaitKey(mask, TV_AUTORET);

        if (key > 0)
        {
            if (key == SDK_KEY_ESC || key == SDK_TIME_OUT)
            {
                sdkFreeMem(pstOutAIDList);
                return SDK_ESC;
            }
            else if (key == SDK_KEY_DOWN)
            {
                if ((i + 1) >= iMaxLine)
                {
                    if ((iClass + 1) * 4 < iTotal)
                    {
                        iRet = sdkEMVBaseGetAnyAIDList((iClass + 1) * 4, 4, pstOutAIDList, &iGetNum);

                        if (iRet == SDK_OK && iGetNum > 0)
                        {
                            iMaxLine = iGetNum;
                            iClass++;
                            i = 0;
                            bIsNewDetail = true;
                        }
                    }
                }
                else if ((i + 1) < iMaxLine)
                {
                    i++;
                    bIsNewDetail = true;
                }
            }
            else if (key == SDK_KEY_UP)
            {
                if (i <= 0)
                {
                    if (iClass > 0)
                    {
                        iRet = sdkEMVBaseGetAnyAIDList((iClass - 1) * 4, 4, pstOutAIDList, &iGetNum);

                        if (iRet == SDK_OK && iGetNum > 0)
                        {
                            iMaxLine = iGetNum;
                            iClass--;
                            i = 3;
                            bIsNewDetail = true;
                        }
                    }
                }
                else if (i > 0)
                {
                    i--;
                    bIsNewDetail = true;
                }
            }
            else if (key > SDK_KEY_0 && key < SDK_KEY_1 + iMaxLine)
            {
                i = key - SDK_KEY_1;
                bIsNewDetail = true;
            }
            else if (key == SDK_KEY_ENTER && enSet)
            {
                EmvAidParamSet(&pstOutAIDList[i]);
                bIsNewDetail = true;//luohuidong 2018.09.01 16:12
                //break;
            }
            else if(key == SDK_KEY_0)
            {
                bIsNewDetail = true;//luohuidong 2018.09.01 16:12
            }
        }
    }

    sdkFreeMem(pstOutAIDList);
    return SDK_OK;
}

#if 0 //del by sujianzhong 2017.12.12
/*******************************************************************
   ��    ��: ��Т��
   ��������: void PostSetEMVOrPBOC(void)
   ��������: ������PBOCӦ�ã�����EMVӦ��
   ��ڲ���?
   �� �� ֵ:
   ��ص���?
   ��    ע:
   �޸���Ϣ: huxiang090702 �����qPBOCѡ��
 ********************************************************************/
void PostSetEMVOrPBOC(void)
{
    s32 key;
    unsigned char y;      //,i=0,ch sxl arm_linux
    unsigned char str[5];      //,tempbuf[32] sxl arm_linux
    unsigned char NeedFlush;
    unsigned char TempAppType;
    unsigned int colpos = 128;
    _SimData SimData;

    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);
    memset(str, 0, 5);
    str[0] = '<';  //little triangle block,0x1b(left)
    NeedFlush = 1;
    TempAppType = SimData.AppType;

    while(1)
    {
        if(NeedFlush)
        {
            NeedFlush = 0;

            if(TempAppType == EMVTYPE)
            {
                y = SDK_DISP_LINE1;
            }
            else if(TempAppType == PBOCTYPE)
            {
                y = SDK_DISP_LINE2;
            }
            else if(TempAppType == qPBOCTYPE)
            {
                y = SDK_DISP_LINE3;
            }
            else
            {
                y = SDK_DISP_LINE1;
                TempAppType = EMVTYPE;
                SimData.AppType = TempAppType;
                SaveSimData(&SimData);
            }
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "1.EMV ", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "2.PBOC ", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "3.qPBOC ", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);        //huxiang 090702
            //sdkDispFillRowRam(y, colpos, (char *)str, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);        //sxl arm_linux
            sdkDispRowAt(y, 0, "��", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
        }
        key = sdkKbGetKey();

        if(key != 0)
        {
            if( key == SDK_KEY_1)
            {
                TempAppType = EMVTYPE;
                sdkSysBeep(0);
                NeedFlush = 1;

                if(TempAppType != SimData.AppType)
                {
                    SimData.AppType = TempAppType;
                    SaveSimData(&SimData);
                }
                return;
            }
            else if(key == SDK_KEY_2)
            {
                TempAppType = PBOCTYPE;
                sdkSysBeep(0);
                NeedFlush = 1;

                if(TempAppType != SimData.AppType)
                {
                    SimData.AppType = TempAppType;
                    SaveSimData(&SimData);
                }
                return;
            }
            else if(key == SDK_KEY_3)
            {
                TempAppType = qPBOCTYPE;
                sdkSysBeep(0);
                NeedFlush = 1;

                if(TempAppType != SimData.AppType)
                {
                    SimData.AppType = TempAppType;
                    SaveSimData(&SimData);
                }
                return;
            }
            else if ( (key == SDK_KEY_ENTER) || (key == SDK_KEY_ESC) )
            {
                sdkSysBeep(0);

                if(TempAppType != SimData.AppType)
                {
                    SimData.AppType = TempAppType;
                    SaveSimData(&SimData);
                }
                return;
            }
            else
            {
                sdkSysBeep(1);
            }
        }
    }
}
#endif

/*******************************************************************
   ��    ��: ��Т��
   ��������: void PostSetForceOnLine(void)
   ��������: �Ƿ�ǿ������
   ��ڲ���?
   �� �� ֵ:
   ��ص���?
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
s32 PostSetForceOnLine(void)
{
    int nRet;
    unsigned char TempAppType;
    _SimData SimData;


    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);

    TempAppType = SimData.bForceOnline;


    nRet = SetTorF("ǿ������", "�Ƿ�ǿ������ ", "0 - ��  1 - �� ", &TempAppType);

    if(TempAppType != SimData.bForceOnline)
    {
        SimData.bForceOnline = TempAppType;
        SaveSimData(&SimData);
    }

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

s32 PostSetGM(void)
{
    int nRet;
    unsigned char TempAppType;
    _SimData SimData;

    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);

    TempAppType = SimData.ODAalgrthFlag;

    nRet = SetTorF("��������", "�Ƿ�֧�ֹ����㷨", "0 - ��  1 - �� ", &TempAppType);

    if(TempAppType != SimData.ODAalgrthFlag)
    {
        SimData.ODAalgrthFlag = TempAppType;
        SaveSimData(&SimData);
    }

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}
s32 SetAutoToolDispTimeOut(unsigned short *value)
{
    int nRet;
    u8 asTmp[SDK_MAX_STR_LEN + 2];
    int i;
    unsigned short temp=0;

    memset(asTmp, 0x00, sizeof(asTmp));

    temp = *value;

    sdkU16ToAsc(temp,&asTmp[0]);

    nRet = Inputstr("Set VACS DispTimeOut", "Pls input >0 number:", NULL, asTmp, 1, 4, SDK_MMI_HEX);

    if (nRet == SDK_KEY_ENTER)
    {
        i = strlen(asTmp);

        if (i > 4) {asTmp[4]=0x00;}

        i = atoi(asTmp);

        if(i>0xFFFD)
        {
            *value=0xFFFE;
        }
        else
        {
            *value=i;
        }

    }
    return nRet;
}
#if 1 /*Modify by luohuidong at 2018.10.19  11:21 */
s32 DispWaitTimeForVCAS(void)
{
    int nRet;

    _SimData SimData;
    unsigned short value=0;

    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);



    /*=======BEGIN: luohuidong 2018.09.03  13:44 modify===========*/
    value = SimData.AutoToolDispTimeMs;
    #ifdef EMVB_DEBUG
        printf("\r\n PostSetPrint 1 value =%d ,gSimData.AutoToolDispTimeMs%d\r\n",value,gSimData.AutoToolDispTimeMs);
    #endif

    SetAutoToolDispTimeOut(&value);

    #ifdef EMVB_DEBUG
        printf("\r\n PostSetPrint 2 value =%d \r\n",value);
    #endif

    if(value != SimData.AutoToolDispTimeMs)
    {
        SimData.AutoToolDispTimeMs = value;
        SaveSimData(&SimData);
    }
    #ifdef EMVB_DEBUG
        printf("\r\n gSimData.AutoToolDispTimeMs =%d \r\n",gSimData.AutoToolDispTimeMs);
    #endif
    /*====================== END======================== */

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}
#endif /* if 0 */
s32 PostSetPrint(void)
{
	int nRet;
	unsigned char TempAppType, PaperCount;
	_SimData SimData;
	unsigned char temp[5];

	memset(&SimData, 0, sizeof(_SimData));
	ReadSimData(&SimData);

	TempAppType = SimData.BisSupportPrint;

	//nRet = SetTorF("��ӡ����", "�Ƿ�����ӡ", "0 - ��  1 - �� ", &TempAppType);

	nRet = SetTorF("Printer Set", "Enable Printer?", "0 - NO  1 - YES ", &TempAppType);

	if(TempAppType != SimData.BisSupportPrint)
	{
		SimData.BisSupportPrint = TempAppType;
		SaveSimData(&SimData);
	}

	if (nRet != SDK_KEY_ENTER) {return nRet; }

	if(SimData.BisSupportPrint)//
	{
		memset(temp, 0x00, 0);
		temp[0] = 1;
		if(gJCBPaperNum == 0)
		{
			gJCBPaperNum += 1;
		}
		temp[1] = gJCBPaperNum + 0x30;
		sdkDispClearScreen();
		sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Print Num Set", SDK_DISP_DEFAULT);
		sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Input Paper Num(1~3):", SDK_DISP_LEFT_DEFAULT);
		if(SDK_KEY_ENTER == sdkKbGetScanf(0, temp, 1, 1, SDK_MMI_NUMBER, SDK_DISP_LINE4))
		{
			if((temp[1]-0x30) != gJCBPaperNum && (temp[1]>='1') && (temp[1]<='3'))
			{
				gJCBPaperNum = temp[1] - 0x30;
			}
		}
		else
		{
			return SDK_ERR;
		}
	}

	return nRet;
}

s32 PostTermCheck(void)
{
    int nRet;
    unsigned char TempAppType;
    _SimData SimData;

    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);

    TempAppType = SimData.SupportTermCheck;

    nRet = SetTorF("�ն�����", "�ն��Ƿ�֧��״̬���?  ", "0 - ��  1 - �� ", &TempAppType);

    if(TempAppType != SimData.SupportTermCheck)
    {
        SimData.SupportTermCheck = TempAppType;
        SaveSimData(&SimData);
    }

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

s32 PostSetBatchCapture(void)
{
    int nRet;
    unsigned char TempAppType;
    _SimData SimData;

    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);

    TempAppType = SimData.bBatchCapture;
    /*
       �������ݲ���:��������,����ȷ��,����
       ���������ݲ���:��Ȩ����,���ڼ�¼,֪ͨ
     */

    nRet = SetTorF("Data Capture", "0-Online  1-Batch", NULL, &TempAppType);

    if(TempAppType != SimData.bBatchCapture)
    {
        SimData.bBatchCapture = TempAppType;
        SaveSimData(&SimData);
    }

    if (nRet != SDK_KEY_ENTER) {return nRet; }
    return nRet;
}

s32 PostSpAutotest(void)
{
    int nRet;
    u8 buf[6] = {0};

    nRet = SetTorF(DISP_SETSWITCH, DISP_SETAUTOTEST, DISP_SETYESORNO, &gstAutoTest);

    if (nRet != SDK_KEY_ENTER) {return nRet; }

    if(gstAutoTime == 0)
    {
        gstAutoTime = 8;
    }
    memset(buf, 0, sizeof(buf));

    if (gstAutoTime != 0)
    {
        if(gstAutoTime >= 10)
        {
            buf[0] = 2;
            buf[1] = (gstAutoTime / 10) + 0x30;
            buf[2] = (gstAutoTime % 10) + 0x30;
        }
        else
        {
            buf[0] = 1;
            buf[1] = gstAutoTime + 0x30;
        }
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_SETOUTOTESTTIME, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    nRet = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, buf, 0, 2, SDK_MMI_NUMBER, SDK_DISP_LINE3);

    if(nRet == SDK_OK)
    {
        if (buf[0] >= 2)
        {
            gstAutoTime = (buf[1] - 0x30) * 10 + (buf[2] - 0x30);
        }
        else
        {
            gstAutoTime = buf[1] - 0x30;
        }
    }
    else
    {
        return SDK_ERR;
    }

    if(gstHostTimeout == 0)		//��̨ͨѶ��ʱʱ��
    {
        gstHostTimeout = 5;
    }

    memset(buf, 0, sizeof(buf));

    if (gstHostTimeout != 0)
    {
        if(gstHostTimeout >= 10)
        {
            buf[0] = 2;
            buf[1] = (gstHostTimeout / 10) + 0x30;
            buf[2] = (gstHostTimeout % 10) + 0x30;
        }
        else
        {
            buf[0] = 1;
            buf[1] = gstHostTimeout + 0x30;
        }
    }

    sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_SETHOSTTIMEOUT, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    nRet = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, buf, 0, 2, SDK_MMI_NUMBER, SDK_DISP_LINE3);

    if(nRet == SDK_OK)
    {
        if (buf[0] >= 2)
        {
            gstHostTimeout = (buf[1] - 0x30) * 10 + (buf[2] - 0x30);
        }
        else
        {
            gstHostTimeout = buf[1] - 0x30;
        }
    }
    else
    {
        return SDK_ERR;
    }

	gstAutoTest = 1;
    return nRet;
}

typedef struct
{
    u32 key;
    u8 keyname[8];
} STR_KEYTALE;


void ShowCheckSum(void)
{
	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Kernel:  B2BBC4C8", SDK_DISP_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Reader:  505E8FAB", SDK_DISP_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE3, 0, "App:     CA23E0B3", SDK_DISP_DEFAULT);

	sdkDispBrushScrecen();
	return sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

static int GetCompileTime(char *pasCompileTime, const char *pasDate, const char *pasTime)
{
	char temp_date[64] = {0},str_year[5] = {0}, str_month[4] = {0}, str_day[3] = {0};
	char temp_time[64] = {0},str_hour[2] = {0}, str_min[2] = {0}, str_sec[2] = {0};
	char en_month[12][4]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char tempCompileTime[20] = {0};
	int i = 0;

	if(pasCompileTime == NULL || pasDate == NULL || pasTime == NULL)
	{
		return 0;
	}

	sprintf(temp_date,"%s",pasDate);    //"Sep 7 2012"
	sscanf(temp_date,"%s %s %s",str_month, str_day, str_year);

	for(i=0; i < 12; i++)
	{
		if(strncmp(str_month,en_month[i],3)==0)
		{
		    memset(str_month, 0, sizeof(str_month));
	  		sprintf(str_month, "%02d", i + 1);
	        break;
	    }
	}

	if(strlen(str_day)==1)//������Ϊ1λ��Ҫǰ��0x30
	{
		str_day[1]=str_day[0];
		str_day[0]=0x30;
	}

	sprintf(pasCompileTime, "%s%s%s", str_year, str_month, str_day);
	sprintf(tempCompileTime, " %s", pasTime);
	strcat(pasCompileTime, tempCompileTime);

	return 0;
}

s32 PostShowAppCompilerTime(void)
{
	s32 key;
	u8 dispbuf[40] = {0};
	u8 AppVersion[40] = {0};
	u8 LibVersion_EmvBase[40] = {0};
	u8 LibVersion_Pure[40] = {0};

	strcpy(dispbuf, "App:");
	GetCompileTime(AppVersion, __DATE__, __TIME__);
	strcat(dispbuf, AppVersion);

	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, dispbuf, SDK_DISP_DEFAULT);

	memset(dispbuf, 0, 40);
	strcpy(dispbuf, "Base:");
	sdkEmvBaseGetLibVersion(LibVersion_EmvBase);
	strcat(dispbuf, LibVersion_EmvBase);
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, dispbuf, SDK_DISP_DEFAULT);

	memset(dispbuf, 0, 40);
	strcpy(dispbuf, "PURE:");
	sdkPureGetLibVersion(LibVersion_Pure);
	strcat(dispbuf, LibVersion_Pure);
	sdkDispFillRowRam(SDK_DISP_LINE4, 0, dispbuf, SDK_DISP_DEFAULT);


#ifdef ZT_SDK_DEBUG
	sdkDispFillRowRam(SDK_DISP_LINE5, 0, "Version: Debug", SDK_DISP_DEFAULT);
#else
	sdkDispFillRowRam(SDK_DISP_LINE5, 0, "Version: Release", SDK_DISP_DEFAULT);
#endif

	sdkDispBrushScreen();

	key = sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);

	return SDK_OK;
}

s32 ClearBlackDataMenu()
{
    memset(gstbctcblack, 0, sizeof(gstbctcblack));
    SaveBlackData();

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Clear Success", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);

    return SDK_OK;
}

void DelAPPEXAID()
{
    u8 fn[64] = {0};
    sdkSysGetCurAppDir(fn);
    strcat(fn, "appexaid");
	Trace("APPEXAID", "DelAPPEXAID\r\n");
	sdkDelFile(fn);
}

void InitAPPEXAID()
{
    memset(appex_aid_list, 0, sizeof(appex_aid_list));
//	Trace("InitAPPEXAID", "sizeof(appex_aid_list) = %d\r\n", sizeof(appex_aid_list));
//	SaveAPPEXAID();
}

void SaveAPPEXAID()
{
    s32 ret;
    u8 fn[64] = {0};

//	Trace("file", "goto SaveAPPEXAID\r\n");
    sdkSysGetCurAppDir(fn);
    strcat(fn, "appexaid");

    ret = sdkInsertFile((const u8*)fn, (const u8*)appex_aid_list, 0, sizeof(appex_aid_list));
}

void ReadAPPEXAID()
{
    s32 len = sizeof(appex_aid_list);
    u8 fn[64] = {0};

    sdkSysGetCurAppDir(fn);
    strcat(fn, "appexaid");

    if(sdkReadFile((const u8*)fn, (u8 *)appex_aid_list, 0, &len) == SDK_FILE_OK)
    {
    }
    else
    {
        InitAPPEXAID();
        SaveAPPEXAID();
    }
}

void PostInitSysData(void)
{
	s32 key;
	_SimData *SimData;
	u8 fn[64] = {0};

	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Init all data",SDK_DISP_DEFAULT);
	sdkDispBrushScreen();

	sdkKbKeyFlush();
	key = sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0);

	switch ( key )
	{
		case SDK_KEY_ENTER:
			sdkEMVBaseDelAllAIDLists();
			sdkEMVBaseDelAllCAPKLists();
			DelAPPEXAID();
			InitAPPEXAID();
			SaveAPPEXAID();
			memset(gstbctcblack, 0, sizeof(gstbctcblack));
			SaveBlackData();
			memset(gstbctcpkrecova, 0, sizeof(gstbctcpkrecova));
			SaveRecovaData();
			memset(gstbctcreadtemplate, 0, sizeof(gstbctcreadtemplate));
			gbctcreadtemplatelen = 0;
			memset(gstbctcupdatetemplate, 0, sizeof(gstbctcupdatetemplate));
			gbctcupdatetemplatelen = 0;

			IccDispText(DISP_OK);
			break;

		default:
			break;
	}
}


void AddAPPEXAID(APPEX_AID_STRUCT *extempAid)
{
	u8 i = 0;

	if(extempAid == NULL)
	{
		return;
	}

//	Trace("APPEXAID", "conter: %d\r\n", sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT));

	for(i = 0; i < sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT); i++)
	{
        if((appex_aid_list[i].AidLen == extempAid->AidLen) && (memcmp(appex_aid_list[i].Aid,extempAid->Aid,extempAid->AidLen) == 0) && (appex_aid_list[i].TransType == extempAid->TransType))
		{
//			Trace("APPEXAID", "hit cache1\r\n");
			memcpy(&appex_aid_list[i],extempAid,sizeof(APPEX_AID_STRUCT));
			SaveAPPEXAID();
			return;
		}
	}

//	for(i = 0; i < sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT); i++)
//	{
//		if(appex_aid_list[i].AidLen == extempAid->AidLen && memcmp(appex_aid_list[i].Aid,extempAid->Aid,extempAid->AidLen) == 0)
//		{
//			Trace("APPEXAID", "hit cache2\r\n");
//			memcpy(&appex_aid_list[i],extempAid,sizeof(APPEX_AID_STRUCT));
//			SaveAPPEXAID();
//			return;
//		}
//	}

	for(i = 0; i < sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT); i++)
	{
		if(appex_aid_list[i].AidLen == 0)
		{
//			Trace("APPEXAID", "hit cache3\r\n");
			memcpy(&appex_aid_list[i],extempAid,sizeof(APPEX_AID_STRUCT));
			SaveAPPEXAID();
			return;
		}
	}

}

s32 GetAPPEXAID(APPEX_AID_STRUCT *extempAid, u8 *aid, s32 aidlen)
{
	s32 i=0;
	memset(extempAid, 0, sizeof(APPEX_AID_STRUCT));
	for(i = 0; i < sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT); i++)
	{
		if(appex_aid_list[i].AidLen == aidlen && memcmp(appex_aid_list[i].Aid,aid,aidlen) == 0)
		{
			memcpy(extempAid, &appex_aid_list[i] ,sizeof(APPEX_AID_STRUCT));
			break;
		}
	}

	if(i >= sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT))
	{
		return SDK_ERR;
	}
	return SDK_OK;
}

s32 IccSetAIDEX()
{
    APPEX_AID_STRUCT *termaidparam = appex_aid_list;
    u8 i;
    u8 aidinterm[16] = {0};
    s32 aidintermlen = 0;
    Trace("pure-info","comes to before GPO set\r\n");
	_SimData Simdata={0};
	u8 Transtype, StatusCheckFlag, ZeroAmtAllowFlag;
	s32 len;
	s32 ret;

    sdkEMVBaseReadTLV("\x9f\x06", aidinterm, &aidintermlen);

    if(aidintermlen < 5)
    {
        return SDK_OK;
    }

	if(gstbctcautotrade.typeexist)
	{
		sdkEMVBaseConfigTLV("\x9C", &gstbctcautotrade.transtype, 1);
	}

    sdkEMVBaseReadTLV("\x9C", &Transtype, &len);
	Trace("SetBeforeGPO", "TransType in Tag: %02X\r\n ", Transtype);
	Trace("SetBeforeGPO", "aidintermlen in Tag: %d\r\n ", aidintermlen);
	TraceHex("SetBeforeGPO", "aid in Tag: ", aidinterm, aidintermlen);

	Trace("SetBeforeGPO", "find %d appex aid\r\n ", sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT));
	for(i = 0; i < sizeof(appex_aid_list)/sizeof(APPEX_AID_STRUCT); i++)
    {
		Trace("SetBeforeGPO", "TransType in Extern AID: %02X\r\n ", appex_aid_list[i].TransType);
		Trace("SetBeforeGPO", "AidLen in Extern AID: %d\r\n ", appex_aid_list[i].AidLen);
		TraceHex("SetBeforeGPO", "aid in Extern AID: ", appex_aid_list[i].Aid, appex_aid_list[i].AidLen);

		if((appex_aid_list[i].AidLen == aidintermlen) && (memcmp(appex_aid_list[i].Aid,aidinterm,aidintermlen) == 0) && (appex_aid_list[i].TransType == Transtype))
        {
//			sdkEMVBaseConfigTLV("\x5F\x2A", (termaidparam+i) ->TransCurcyCode, 2);
//			TraceHex("SetBeforeGPO", "TransCurcyCode ", (termaidparam + i)->TransCurcyCode, 2);

			sdkEMVBaseConfigTLV("\xFF\x81\x79", (termaidparam + i)->RemovalTimeout, 2);
			TraceHex("SetBeforeGPO", "Removal Timeout ", (termaidparam + i)->RemovalTimeout, 2);

			StatusCheckFlag = (termaidparam + i)->StatusCheckFlag;
			ret = sdkEMVBaseConfigTLV("\x1F\x02", &StatusCheckFlag, 1);
			Trace("SetBeforeGPO", "StatusCheckFlag: %d\r\n", StatusCheckFlag);
			Trace("SetBeforeGPO", "sdkEMVBaseConfigTLV ret: %d\r\n", ret);

			ZeroAmtAllowFlag = (termaidparam + i)->ZeroAmtAllowFlag;
			ret = sdkEMVBaseConfigTLV("\xDF\x22", &ZeroAmtAllowFlag, 1);
			Trace("SetBeforeGPO", "ZeroAmtAllowFlag: %d\r\n", ZeroAmtAllowFlag);
			Trace("SetBeforeGPO", "sdkEMVBaseConfigTLV ret: %d\r\n", ret);

			sdkEMVBaseConfigTLV("\xDF\x83\x08", (termaidparam + i)->CLAppCap, 5);
			TraceHex("SetBeforeGPO", "Contactless application Capabilities ", (termaidparam + i)->CLAppCap, 5);

			sdkPureSetImplementation((termaidparam + i)->Implementation);
			Trace("SetBeforeGPO", "Implementation: %02x\r\n", (termaidparam + i)->Implementation);

			sdkPureSetMTOL((termaidparam + i)->MTOL, (termaidparam + i)->MTOLLen);
			TraceHex("SetBeforeGPO", "MTOL ", (termaidparam + i)->MTOL, (termaidparam + i)->MTOLLen);

			ret = sdkEMVBaseConfigTLV("\x9F\x76", (termaidparam + i)->TransData, (termaidparam + i)->TransDataLen);
			TraceHex("SetBeforeGPO", "Trans Data ", (termaidparam + i)->TransData, (termaidparam + i)->TransDataLen);
			Trace("SetBeforeGPO", "sdkEMVBaseConfigTLV ret = %d\r\n", ret);
			break;
        }
    }

	ReadSimData(&Simdata);
	sdkEMVBaseConfigTLV("\x9F\x1A", Simdata.TermCountryCode, 2);
	TraceHex("SetBeforeGPO", "TermCountryCode", Simdata.TermCountryCode, 2);
	sdkEMVBaseConfigTLV("\x5F\x2A", Simdata.TransCurrencyCode, 2);
	TraceHex("SetBeforeGPO", "TransCurrencyCode", Simdata.TransCurrencyCode, 2);
	if(Simdata.RestrictAIDLen)
	{
		sdkEMVBaseConfigTLV("\xDF\x7F", Simdata.RestrictAID, Simdata.RestrictAIDLen);
		TraceHex("SetBeforeGPO", "RestrictAID", Simdata.RestrictAID, Simdata.RestrictAIDLen);
	}

	if(0 != gbctcreadtemplatelen)
	{
		if(gbctcreadtemplatelen > 127)
		{
			sdkPureSetMemorySlotReadTemplate(gstbctcreadtemplate+4, gbctcreadtemplatelen-4);
		}
		else
		{
			sdkPureSetMemorySlotReadTemplate(gstbctcreadtemplate+3, gbctcreadtemplatelen-3);
		}
	}
	else
	{
		sdkPureFreeMemorySlotReadTemplate();
	}

	if(0 != gbctcupdatetemplatelen)
	{
		if(gbctcupdatetemplatelen <= 127 && gbctcupdatetemplatelen > 0)
		{
			ret = sdkPureSetMemorySlotUpdateTemplate(gstbctcupdatetemplate+3, gbctcupdatetemplatelen-3);
			TraceHex("SetBeforeGPO", "update template", gstbctcupdatetemplate+3, gbctcupdatetemplatelen-3);
		}
		else if(gbctcupdatetemplatelen > 127 && gbctcupdatetemplatelen <= 255)
		{
			ret =sdkPureSetMemorySlotUpdateTemplate(gstbctcupdatetemplate+4, gbctcupdatetemplatelen-4);
			TraceHex("SetBeforeGPO", "update template", gstbctcupdatetemplate+4, gbctcupdatetemplatelen-4);
		}
		else
		{
			ret =sdkPureSetMemorySlotUpdateTemplate(gstbctcupdatetemplate+5, gbctcupdatetemplatelen-5);
			TraceHex("SetBeforeGPO", "update template", gstbctcupdatetemplate+5, gbctcupdatetemplatelen-5);
		}
		Trace("pure-info","sdkPureSetMemorySlotUpdateTemplate ret = %d\r\n", ret);
	}
	else
	{
		sdkPureFreeMemorySlotUpdateTemplate();
	}

	Trace("pure-info","finish before GPO set\r\n");
    return SDK_OK;
}

s32 SetKernelParam(void)
{
	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, "This feature temp unavailable", SDK_DISP_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Press Any Key Exit", SDK_DISP_DEFAULT);
	sdkDispBrushScrecen();
	return sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

bool sdkSupportPrint()
{
  return false;
}

#define PRINTDATA_LINE_MAX 20
#define PRINTDATA_LINE_CHAR 21//K300??26,N3??30???,G101??16
static u8 gPrintData[PRINTDATA_LINE_MAX][PRINTDATA_LINE_CHAR+1];
static u8 gPrintDataLine=0;

void sdkClearDispPrintData(void)
{
  memset(gPrintData, 0, sizeof(gPrintData));
  gPrintDataLine = 0;
}

s32 sdkSetDispPrintData(const u8 *pasStr)
{
  u8 *p = (u8*)pasStr;
  u8 tmp[PRINTDATA_LINE_CHAR];
  if(p)
  {
    memset(tmp, ' ', sizeof(tmp));
    while(gPrintDataLine < PRINTDATA_LINE_MAX && strlen(p)>0)
    {
      if(strlen(p) < PRINTDATA_LINE_CHAR){
        if(memcmp(p, tmp, strlen(p)))
        {
          strcat((u8*)&gPrintData[gPrintDataLine++], p);
        }
        return SDK_OK;
      }

      memcpy(&gPrintData[gPrintDataLine++], p, PRINTDATA_LINE_CHAR);
      p += PRINTDATA_LINE_CHAR;
    }
  }
  return SDK_ERR;
}
s32 sdkDispPrintDataOnePage(s32 pos)
{
  if(pos < gPrintDataLine)
  {
    s32 line=SDK_DISP_LINE1;

    sdkDispClearScreen();
    while(pos<gPrintDataLine && line<SDK_DISP_LINE_MAX)
    {
      sdkDispFillRowRam(line++, 0, gPrintData[pos++], SDK_DISP_LEFT_DEFAULT);
    }
    sdkDispBrushScreen();
  }
  else
  {
    pos = 0;
  }

  return pos;
}

s32 sdkDispPrintData(void)
{
  s32 key=0;
  s32 pageline=0,nextpageline=0;

  sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);//???1s??????????????????????????
  while(1)
  {
    nextpageline=sdkDispPrintDataOnePage(pageline);
    Trace("emv","%d, %d, %d\r\n", gPrintDataLine, pageline, nextpageline);
    key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
    switch(key)
    {
      case SDK_KEY_DOWN:
      case SDK_KEY_ENTER:
        if(nextpageline < gPrintDataLine)
        {
          pageline = nextpageline;
        }
		else
        {
          pageline = 0;
        }
        break;
      case SDK_KEY_ESC:
        return SDK_OK;
        break;
      default:
        break;
    }
  }
}

void PostSetTCPSetting(void)
{
	u8 IPAddress[13], port[6];
	s32 ret,i,j,len;
	u8 *data_uf, *data_f;
	_SimData SimData = {0};

	gHostTransType = HOST_TRANS_WIFI;

	ReadSimData(&SimData);

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "IP Address", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

	IPAddress[0] = 12;
//	memcpy(IPAddress+1, gTCPAddress, 12);
	memcpy(IPAddress+1, SimData.IPAddress, 12);

	sdkKbKeyFlush();
	ret = sdkKbGetScanf(0, IPAddress, 12, 12, SDK_MMI_NUMBER, SDK_DISP_LINE3);
//    Trace("emv", "sdkKbGetScanf retcode %d\r\n", ret);

    if(SDK_KEY_ENTER == ret)
    {
        len = IPAddress[0];
//        TraceHex("emv", "input IPAddress:", &IPAddress[1], len);

        if(len > 12)
        {
            len = 12;
        }
//		memset(gTCPAddress, 0, 16);
//        memcpy(gTCPAddress, IPAddress + 1, len);
        memset(SimData.IPAddress, 0, 16);
        memcpy(SimData.IPAddress, IPAddress + 1, len);
    }
	else if(SDK_KEY_ESC == ret)
	{
		return ;
	}
//    TraceHex("emv", "input IPAddress:", SimData.IPAddress, len);

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Port", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

	port[0] = IntBitMapLen(SimData.TCPPort);
	sdkU32ToAsc((u32)SimData.TCPPort, port+1);
	sdkKbKeyFlush();
	ret = sdkKbGetScanf(0, port, 4, 5, SDK_MMI_NUMBER, SDK_DISP_LINE3);
//    Trace("emv", "sdkKbGetScanf retcode %d\r\n", ret);

    if(SDK_KEY_ENTER == ret)
    {
        len = port[0];
//        TraceHex("emv", "input port:", &port[1], len);

        if(len > 5)
        {
            len = 5;
        }
		SimData.TCPPort = convertToUnsignedInt(port + 1, len);
//		SimData.TCPPort = gTCPPort;
    }
//    Trace("emv", "input port: %d", SimData.TCPPort);

	data_uf = convertToCString(SimData.IPAddress, 16);
	data_f = formatIPAddress(data_uf);
//	Trace("lishiyao", "before removeLeadingZeros IP:%s\r\n", data_f);
	removeLeadingZeros(data_f);
	sdkOpenWifi(data_f, SimData.TCPPort);
	SaveSimData(&SimData);
}

void PostSetOutcomeDelay(void)
{
	u8 delay[4], delay_asc[8+1];
	s32 ret,i,j,len;
	u8 *data_uf, *data_f;

	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Outcome Delay", SDK_DISP_DEFAULT);
	sdkDispBrushScreen();

	delay_asc[0] = 8;

	Trace("emv", "gOutcomeDelay: %d\r\n", gOutcomeDelay);
	sdkU32ToBcd(delay, (u32)gOutcomeDelay, 4);
	TraceHex("emv", "after switch delay:", delay, 4);
	sdkBcdToAsc(&delay_asc[1], delay, 4);
	Trace("emv", "after switch delay_asc: %s\r\n", &delay_asc[1]);

	sdkKbKeyFlush();
	ret = sdkKbGetScanf(0, delay_asc, 8, 8, SDK_MMI_NUMBER, SDK_DISP_LINE3);
	Trace("emv", "sdkKbGetScanf retcode %d\r\n", ret);

	if(SDK_KEY_ENTER == ret)
	{
		len = delay_asc[0];
		TraceHex("emv", "input delay:", &delay_asc[1], len);
		Trace("emv", "out len: %d\r\n", len);
		if(len > 8)
		{
			len = 8;
		}
		sdkAscToBcd(delay, &delay_asc[1], len);
		TraceHex("emv", "after switch delay:", delay, 4);
		sdkBcdToU32((u32 *)(&gOutcomeDelay), delay, 4);
		Trace("emv", "after input delay: %d\r\n", gOutcomeDelay);
	}
	else if(SDK_KEY_ESC == ret)
	{
		return ;
	}
}

void PostSetHostCommuType(void)
{
	s32 key;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Communicate Type", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1-Serial  2-Wifi", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

	key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_ENTER | SDK_KEY_ESC, SDK_ICC_TIMER_AUTO);
	switch (key)
	{
		case SDK_KEY_1:
			gHostTransType = HOST_TRANS_SERIAL;
			break;

		case SDK_KEY_2:
			gHostTransType = HOST_TRANS_WIFI;
			break;

		default:
			break;
	}

}
