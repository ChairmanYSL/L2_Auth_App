#include "appglobal.h"
#include "host.h"
#include "sdkpure.h"
#include "emv_type.h"
#include "sdkoutcome.h"
#include "extern.h"
#include "extern_api.h"
//#include "dlljcbprivate.h"


#define  ONPEN_PPSE_ON_OFF
#undef   ONPEN_PPSE_ON_OFF

extern _SimData gSimData;

u8 gstDetecteCardMode;
u8 PINDesKey[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
u8 bIssecondtapcard=0;
#define SLEEP_TIME 300


#define DDI_RF_CTL_PPSE              (32)		//20200327_lhd zel fix do not send ppse cmd

void DispRemoveIcc(void)
{

}

s32 UnzipTaglist(u8 *DataIn, s32 DataLen)
{
    u16 id;
    s32 retCode = SDK_OK;
    u8 tag[4], taglen;
    u16 len;
    u16 startpos;
    u8 parseret;

    if(DataLen <= 0)
    {
        return SDK_ERR;
    }
    id = 0;

    while(id < DataLen)
    {
        if(id >= DataLen)
        {
            retCode = SDK_ERR;
            break;
        }

        if(DataIn[id] == 0x00)
        {
            id++;
            continue;
        }
        taglen = 0;
        memset(tag, 0, sizeof(tag));


        startpos = id;


        tag[taglen++] = DataIn[id];

        if((tag[0] & 0x1F) == 0x1F)
        {
            tag[taglen++] = DataIn[id + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = DataIn[id + 2];
            }
        }
        id += taglen;
        parseret = EMVBase_ParseExtLen(DataIn, &id, &len);

        if(parseret != 0)
        {
            return SDK_ERR;
        }
        sdkEMVBaseConfigTLV(tag, DataIn + id, len);

        id += len;
    }

    return retCode;
}


s32 InputAmount(u8 *title, u8 *prompt,u8 *amount)
{
	s32 retCode;
    u8 buf[64], amount_ascii[64];
    u32 i = 0;
    u8 transtype = 0;
    s32 len = 1;
	_SimData *SimData = NULL;
	u8 TransCurrExponent = 0;
	SimData = (_SimData *)sdkGetMem(sizeof(_SimData));
    memset(SimData, 0, sizeof(_SimData));
    ReadSimData(SimData);
    TransCurrExponent =  SimData->TransCurrencyExponent;
	Trace("jcb", "Trans Current Exponent(tag5F36) = %d\r\n", TransCurrExponent);
    sdkFreeMem(SimData);
	SimData = NULL;

    if(amount == NULL)
    {
        return SDK_ERR;
    }
    sdkEMVBaseReadTLV("\x9C", &transtype, &len);
    Trace("emv", "transtpye = %02X\r\n", transtype);

    memset(buf, 0, sizeof(buf));
    memset(amount_ascii, 0, sizeof(amount_ascii));
    sdkDispClearScreen();
    if(title)
    {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, title, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    }
    if(prompt)
    {
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, prompt, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    }
    sdkDispBrushScreen();


	u8 asDispBuf[64]= {"0.0"};
	sdkKbKeyFlush();
	if(1 == TransCurrExponent)//1的时候一位小�?
	{
		sdkDispClearRow(SDK_DISP_LINE3);
		sdkDispFillRowRam(SDK_DISP_LINE3, 0, asDispBuf, SDK_DISP_RIGHT_DEFAULT);
		sdkDispBrushScreen();
		while(1)
		{
			retCode= sdkKbGetKey();
			if(retCode)
			{
//				sdkSysBeep(SDK_SYS_BEEP_OK);

				if(retCode == SDK_KEY_0)
				{
					if(atol(buf) > 0 && strlen(buf) < 9) buf[strlen(buf)] = (retCode&0xFF);
				}
				else if(SDK_KEY_1 <= retCode && SDK_KEY_9 >= retCode)
				{
					if(strlen(buf)<9) buf[strlen(buf)] = (retCode&0xFF);
				}
				else if(SDK_KEY_BACKSPACE == retCode)
				{
					if(strlen(buf)) buf[strlen(buf)-1] = 0;
				}
				else if(SDK_KEY_ENTER == retCode)
				{
					memcpy(amount, buf, strlen(buf));
					break;
				}
				else if(SDK_KEY_ESC == retCode)
				{
					break;
				}
				Trace("Mai", " pheOut = %s\r\n", buf);
				sprintf(asDispBuf, "%d.%01d", atol(buf)/10, atol(buf)%10);

				sdkDispClearRow(SDK_DISP_LINE3);
				sdkDispFillRowRam(SDK_DISP_LINE3, 0, asDispBuf, SDK_DISP_RIGHT_DEFAULT);
				sdkDispBrushScreen();
		   }
	   }

		if(retCode == SDK_KEY_ENTER)
	    {
	        retCode = SDK_OK;
	    }
	    else                                         // 鍙栨秷杈撳叆閲戦杩斿洖
	    {
	        retCode = SDK_ERR;
	    }
	    return retCode;
	}

	else if(0 == TransCurrExponent)//0的时候没有小�?
	{

		retCode = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, buf, 0, 9, SDK_MMI_NUMBER, SDK_DISP_LINE3);
	}
	else //其它情况都当�?来处�?-2位小�?
	{
		retCode = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, buf, 0, 9, SDK_MMI_NUMBER | SDK_MMI_POINT, SDK_DISP_LINE3);
	}



    Trace("emv", "sdkKbGetScanf retcode %d\r\n", retCode);

    if(retCode == SDK_KEY_ENTER)
    {
        TraceHex("emv", "input amount_ASC:", &buf[1], buf[0]);
        i = buf[0];

        if(i > 9)
        {
            i = 9;
        }
        buf[1 + i] = 0;
        memcpy(amount, buf + 1, buf[0] + 1); //鏈€鍚庝竴涓瓧鑺備负缁撴潫�?\0'
    }
    TraceHex("emv", "input amount:", amount, strlen(amount));

    if(retCode == SDK_KEY_ENTER)
    {
        retCode = SDK_OK;
    }
    else                                         // 鍙栨秷杈撳叆閲戦杩斿洖
    {
        retCode = SDK_ERR;
    }
    return retCode;




}

s32 sdkTestIccInputAmount(u8 *title, u8 *prompt,u8 *amount)
{
    s32 retCode;
    u8 buf[64], amount_ascii[64];
    u32 i = 0;
    u8 transtype = 0;
    s32 len = 1;

    if(amount == NULL)
    {
        return SDK_ERR;
    }
    sdkEMVBaseReadTLV("\x9C", &transtype, &len);
    Trace("emv", "transtpye = %02X\r\n", transtype);

    memset(buf, 0, sizeof(buf));
    memset(amount_ascii, 0, sizeof(amount_ascii));
    sdkDispClearScreen();
    if(title)
    {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, title, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    }
    if(prompt)
    {
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, prompt, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    }
    sdkDispBrushScreen();
#ifdef PAYWAVEL3TEST
    u8 asDispBuf[64]= {"MYR 0.00"};

    sdkDispClearRow(SDK_DISP_LINE3);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, asDispBuf, SDK_DISP_RIGHT_DEFAULT);
    sdkDispBrushScreen();

    while(1)
    {
        retCode= sdkKbGetKey();
        if(retCode)
        {
            sdkSysBeep(0);

            if(retCode == SDK_KEY_0)
            {
                if(atol(buf) > 0 && strlen(buf)<9) buf[strlen(buf)] = (retCode&0xFF);
            }
            else if(SDK_KEY_1 <= retCode && SDK_KEY_9 >= retCode)
            {
                if(strlen(buf)<9) buf[strlen(buf)] = (retCode&0xFF);
            }
            else if(SDK_KEY_BACKSPACE == retCode)
            {
                if(strlen(buf)) buf[strlen(buf)-1] = 0;
            }
            else if(SDK_KEY_ENTER == retCode)
            {
                memcpy(amount, buf, strlen(buf));
                break;
            }
            else if(SDK_KEY_ESC == retCode)
            {
                break;
            }
            Trace("Mai", " pheOut = %s\r\n", buf);
            sprintf(asDispBuf, "MYR %d.%02d", atol(buf)/100, atol(buf)%100);

            sdkDispClearRow(SDK_DISP_LINE3);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, asDispBuf, SDK_DISP_RIGHT_DEFAULT);
            sdkDispBrushScreen();
        }
    }

#else

	retCode = sdkKbGetScanf(SDK_ICC_TIMER_AUTO, buf, 0, 9, SDK_MMI_NUMBER | SDK_MMI_POINT, SDK_DISP_LINE3);
    Trace("emv", "sdkKbGetScanf retcode %d\r\n", retCode);

    if(retCode == SDK_KEY_ENTER)
    {
        TraceHex("emv", "input amount_ASC:", &buf[1], buf[0]);
        i = buf[0];

        if(i > 9)
        {
            i = 9;
        }
        buf[1 + i] = 0;
        memcpy(amount, buf + 1, buf[0] + 1); //鏈€鍚庝竴涓瓧鑺備负缁撴潫�?\0'
    }
#endif
    TraceHex("emv", "input amount:", amount, strlen(amount));

    if(retCode == SDK_KEY_ENTER)
    {
        retCode = SDK_OK;
    }
    else                                         // 鍙栨秷杈撳叆閲戦杩斿洖
    {
        retCode = SDK_ERR;
    }
    return retCode;
}

/*******************************************************************
   �?         �? 鏂板浗閮?
   �?         �?鑻忓缓蹇?
   鍑芥暟鍚嶇�?sdkIccUserTransInit
   鍑芥暟鍔熻兘:鐢ㄦ埛鑷畾涔夊垵濮嬪寲EMV鍙傛�?
   杈撳叆鍙傛暟:
   杈撳嚭鍙傛暟:
   �?  �? �?
   淇敼澶囨敞:
   鏃ユ湡鏃堕棿:2012.05.25 18:54:25
*******************************************************************/
void IccUserTransInit()
{
    u8 *taglist;
    s32 taglen;
    u8 buf[8] = {0};
    s32 buf_len = 0;
    _SimData *SimData;
    u8 fn[64] = {0};
    s32 ret;

    SimData = (_SimData *)sdkGetMem(sizeof(_SimData));
    memset(SimData, 0, sizeof(_SimData));
    ReadSimData(SimData);

    sdkEMVBaseConfigTLV("\x9F\x1E", SimData->IfnNo, 8);
    sdkEMVBaseConfigTLV("\x9F\x41", SimData->VocherNo, 4);

    memset(buf, 0, sizeof(buf));
    buf_len = 0;
    ret = sdkEMVBaseReadTLV("\x9F\x33", buf, &buf_len);

    if(ret == SDK_OK && buf_len == 3)
    {
        buf[1] &= 0xFE;
        sdkEMVBaseConfigTLV("\x9F\x33", buf, buf_len);
    }

    if(SimData->ODAalgrthFlag == 1)
    {
        buf[0] = 1;
        sdkEMVBaseConfigTLV("\xDF\x69", buf, 1);
    }
    sdkFreeMem(SimData);
}


s32 jcbRegulateAmount(u8 *pasDest, const u8 *pbcAmount)
{
	u8 i, j = 0;
	u8 num = 0;
    u8 temp[32] = {0};


    if(pbcAmount == NULL || pasDest == NULL)
    {
        return SDK_PARA_ERR;
    }

	Trace("Trade", "gTransCurrExponent = %d\r\n", gTransCurrExponent);
	if(0 == gTransCurrExponent)//0位小数，整数部分最大位数为12
	{
		num = 11;
	}
	else if(1 == gTransCurrExponent)//1位小数，整数部分最大位数为11
	{

		num = 10;
	}
	else//都当�?位小数来处理
	{
		num = 9;
	}
    memset(temp, 0, sizeof(temp));
    sdkBcdToAsc(temp, pbcAmount, 6);
	TraceHex("chenjun", "temp", temp, 12);
    i = (u8)strspn(temp, "0");

    if(i > num) //没有整数部分
    {
    	if(gTransCurrExponent == 1)//1位小�?
    	{
    		pasDest[0] = '0';
	   	    pasDest[1] = '.';
		   	pasDest[2] = temp[11];
			pasDest[4] = 0;
    	}
		else if(gTransCurrExponent == 0)//0位小�?
		{
			pasDest[0] = '0';

			pasDest[1] = 0;

		}
		else//其它情况都是2位小�?
		{
			pasDest[0] = '0';
	   	    pasDest[1] = '.';
		   	pasDest[2] = temp[10];
		   	pasDest[3] = temp[11];
		    pasDest[4] = 0;
		}

    }
    else
    {
        for(j = 0; j < (num - i+1); j++)  //去除整数部分                                 //���ڽǷ�
        {
            pasDest[j] = temp[i + j];
        }


		if(gTransCurrExponent == 1)//1位小�?
    	{
    		 pasDest[j++] = '.';
       		 pasDest[j++] = temp[11];
       		 pasDest[j++] = 0;
    	}
		else if(gTransCurrExponent == 0)//0位小�?
		{
			pasDest[j++] = 0;

		}
		else//其它情况都是2位小�?
		{
			 pasDest[j++] = '.';
       		 pasDest[j++] = temp[10];                                                //��
       		 pasDest[j++] = temp[11];                                                //��
        	 pasDest[j++] = 0;
		}


    }
    return SDK_OK;
}

s32 ReadCardDisp()
{
    u8 temp[30], text[30];
	u8 TempotherAmount[6] = {0};
	memcpy(TempotherAmount, gbcOtherAmount, 6);

    memset(temp, 0,sizeof(temp));
    sdkAscToBcdR(temp, gstasAmount, 6);

    sdkBcdAdd(temp, temp, 6, TempotherAmount, 6);
	TraceHex("app", "Amount in BCD", temp, 6);
    memset(text, 0,sizeof(text));

	jcbRegulateAmount(text+strlen(text), temp);

    sdkDispClearScreen();
    if(gDispSeePhone == 1)
    {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "See Phone", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Present Card", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, text, SDK_DISP_DEFAULT);
    }
    else
    {
    	if(gCollisionflag)//  After multiple cards prompt to read cards
    	{
			sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Please Present One Card Only", SDK_DISP_DEFAULT);
			sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Ready to Read", SDK_DISP_DEFAULT);
			sdkSetUIRequestParam(SDK_UI_MSGID_PRESENT_ONECARD, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			sdkSendUIRequest();
		}
		else//The first prompt to read the card
		{
			Trace("app", "gDispSecondTap = %d\r\n", gDispSecondTap);
			if(gDispSecondTap)
			{
				sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Additional Tap", SDK_DISP_DEFAULT);
				sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Plz present card again", SDK_DISP_DEFAULT);
				sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Ready to Read", SDK_DISP_DEFAULT);
			}
			else
			{
				sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Present Card", SDK_DISP_DEFAULT);
				sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Ready to Read", SDK_DISP_DEFAULT);
				sdkDispFillRowRam(SDK_DISP_LINE3, 0, text, SDK_DISP_DEFAULT);
			}
		}

    }

    sdkDispBrushScreen();
    return SDK_OK;
}

s32 sdkTestInputCreditPwd(const u8 *pasTradeAmount, u8 PINTryCount, u8 IccEncryptWay, u8 *pCreditPwd)
{
    unsigned char key, KeyNum;      //sxl arm_linux
    unsigned char tempkey[13];
    unsigned char KeyBuf[13];
    u8 ucMin = 4, ucMax = 12;

    if(gstAutoTest)
    {
        KeyNum = 4;
        memcpy(KeyBuf, "1234", 4);
        pCreditPwd[0] = KeyNum;

        if(IccEncryptWay == ICCONLINEPIN)
        {
            pCreditPwd[0]  = 8;
            OnlineEncPIN(KeyBuf);
            memcpy(pCreditPwd + 1, KeyBuf, 8);
        }
        else
        {
            memcpy(pCreditPwd + 1, KeyBuf, KeyNum);
        }
        return SDK_OK;
    }
    sdkSysBeep(0);
    sdkDispClearScreen();

    Trace("", "IccEncryptWay = %d PINTryCount = %d\r\n", IccEncryptWay);

    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Pls Input Pin:", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();

    memset(tempkey, 0, sizeof(tempkey));
    KeyNum = 0;
    memset(KeyBuf, 0, sizeof(KeyBuf));

    sdkKbKeyFlush();          //娓呯┖鎸夐敭BUFFER

    while(1)
    {
        key = sdkKbGetKey();            //鑾峰彇鎸夐敭

        if(key != 0)
        {
            switch(key)
            {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':

                if(KeyNum < ucMax)
                {
                    tempkey[KeyNum] = '*';             //KeyBuf[i];
                    tempkey[KeyNum + 1] = 0;
                    KeyBuf[KeyNum++] = key;

                    sdkDispClearRowRam(SDK_DISP_LINE4);
                    sdkDispFillRowRam(SDK_DISP_LINE4, 0, tempkey, SDK_DISP_RIGHT_DEFAULT);
                    sdkDispBrushScreen();
                    sdkSysBeep(0);
                }
                else
                {
                    sdkSysBeep(1);
                }
                break;

            case SDK_KEY_BACKSPACE:

                if(KeyNum != 0)
                {
                    tempkey[KeyNum - 1] = 0;
                    KeyBuf[KeyNum--] = 0;
                    sdkDispClearRowRam(SDK_DISP_LINE4);
                    sdkDispFillRowRam(SDK_DISP_LINE4, 0, tempkey, SDK_DISP_RIGHT_DEFAULT);
                    sdkDispBrushScreen();
                    sdkSysBeep(0);
                }
                else
                {
                    sdkSysBeep(1);
                }
                break;

            case SDK_KEY_ENTER:

                if(KeyNum == 0)                //BYPASS PIN
                {
                    sdkSysBeep(1);
                }
                else if (KeyNum < ucMin)
                {
                    sdkSysBeep(1);
                }
                else
                {
                    sdkSysBeep(0);
                    pCreditPwd[0] = KeyNum;

                    if(IccEncryptWay == ICCONLINEPIN)
                    {
                        pCreditPwd[0]  = 8;
                        OnlineEncPIN(KeyBuf);
                        memcpy(pCreditPwd + 1, KeyBuf, 8);
                    }
                    else
                    {
                        memcpy(pCreditPwd + 1, KeyBuf, KeyNum);
                    }
                    return SDK_OK;
                }
                break;

            case SDK_KEY_ESC:
                sdkSysBeep(1);
                return SDK_ERR;

                break;

            default:
                sdkSysBeep(1);
                break;
            }
        }
    }
}

void sdkTestIccDispText(u8 const *Text)
{
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, Text, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 10 * 100);
}

void IccDispText(u8 const *Text)
{
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, Text, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_ERREXIT, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 200 * 100);
}

s32 IccReadCard()
{
    s32 rslt;
    int i=0;

	rslt = sdkIccOpenRfDev();
	if(rslt != 0)
	{
		sdkTestIccDispText("Open RF Dev fail!");
		return SDK_ERR;
	}

	rslt = sdkIccPowerOnAndSeek();
	Trace("read card", "sdkIccPowerOnAndSeek ret = %d\r\n", rslt);
	if(rslt != SDK_OK)
	{
		return rslt;
	}

	return SDK_OK;
}

s32 InputCreditPwd(const u8 *bcdTradeAmount, u8 PINTryCount, u8 IccEncryptWay, u8 *pCreditPwd)
{
    unsigned char key, KeyNum;
    unsigned char tempkey[13];
    unsigned char KeyBuf[13];
    u8 ucMin = 4, ucMax = 12;
    s32 len;
    u8 temp[8] = {0};
    u8 asc[16] = {0};
    u8 buf[32] = {0};
    s32 ret;

	if(gstAutoTest)
    {
        KeyNum = 4;
        memcpy(KeyBuf, "1234", 4);
        pCreditPwd[0] = KeyNum;

        if(IccEncryptWay == ICCONLINEPIN)
        {
            pCreditPwd[0]  = 8;
            OnlineEncPIN(KeyBuf);
            memcpy(pCreditPwd + 1, KeyBuf, 8);
        }
        else
        {
            memcpy(pCreditPwd + 1, KeyBuf, KeyNum);
        }
        return SDK_OK;
    }

    sdkDispClearScreen();

    len = 0;
    if(bcdTradeAmount == NULL)
    {
        sdkEMVBaseReadTLV("\x9F\x02", temp, &len);
    }
    else
    {
        memcpy(temp,bcdTradeAmount,6);
    }
    sdkRegulateAmount(asc, temp);

    strcpy(buf,DISP_AMOUNT);
    strcat(buf,":");
    strcat(buf,asc);

    sdkDispFillRowRam(SDK_DISP_LINE1, 0, buf, SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_INPUTPWD, SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, DISP_INPUTPWDBYPASS, SDK_DISP_RIGHT_DEFAULT);
    sdkDispBrushScreen();

    memset(tempkey, 0, sizeof(tempkey));
    KeyNum = 0;
    memset(KeyBuf, 0, sizeof(KeyBuf));

    sdkKbKeyFlush();

    while(1)
    {
        key = sdkKbGetKey();

        if(key != 0)
        {
            switch(key)
            {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':

                if(KeyNum < ucMax)
                {
                    tempkey[KeyNum] = '*';
                    tempkey[KeyNum + 1] = 0;
                    KeyBuf[KeyNum++] = key;

                    sdkDispClearRowRam(SDK_DISP_LINE4);
                    sdkDispFillRowRam(SDK_DISP_LINE4, 0, tempkey, SDK_DISP_RIGHT_DEFAULT);
                    sdkDispBrushScreen();
                    sdkSysBeep(0);
                }
                else
                {
                    sdkSysBeep(0);
                }
                break;

            case SDK_KEY_BACKSPACE:

                if(KeyNum != 0)
                {
                    tempkey[KeyNum - 1] = 0;
                    KeyBuf[KeyNum--] = 0;
                    sdkDispClearRowRam(SDK_DISP_LINE4);
                    sdkDispFillRowRam(SDK_DISP_LINE4, 0, tempkey, SDK_DISP_RIGHT_DEFAULT);
                    sdkDispBrushScreen();
                    sdkSysBeep(0);
                }
                else
                {
                    sdkDispClearRowRam(SDK_DISP_LINE4);
                    sdkDispFillRowRam(SDK_DISP_LINE4, 0, DISP_INPUTPWDBYPASS, SDK_DISP_RIGHT_DEFAULT);
                    sdkDispBrushScreen();
                    sdkSysBeep(0);
                }
                break;

            case SDK_KEY_ENTER:

                if(KeyNum == 0)
                {
                    sdkSysBeep(0);
                    pCreditPwd[0] = 0;
                    return SDK_OK;
                }
                else if (KeyNum < ucMin)
                {
                    sdkSysBeep(0);
                }
                else
                {
                    sdkSysBeep(0);
                    pCreditPwd[0] = KeyNum;

                    if(IccEncryptWay == ICCONLINEPIN)
                    {
                        pCreditPwd[0]  = 8;
                        OnlineEncPIN(KeyBuf);
                        memcpy(pCreditPwd + 1, KeyBuf, 8);
                    }
                    else
                    {
                        memcpy(pCreditPwd + 1, KeyBuf, KeyNum);
                    }
                    return SDK_OK;
                }
                break;

            case SDK_KEY_ESC:
                sdkSysBeep(0);
                return SDK_ERR;

                break;

            default:
                sdkSysBeep(0);
                break;
            }
        }
    }
}

s32 InputIccCreditPwd(u8 ucIccEncryptWay, u8 ucPINTryCount, u8 *pheCreditPwd)
{
    return InputCreditPwd(NULL,ucPINTryCount,ucIccEncryptWay,pheCreditPwd);
}

void ImportTradeAmount()
{
    u8 Amount[6] = {0};
    u8 AmountOther[6] = {0};
    sdkAscToBcdR(Amount, gstasAmount, 6);
	TraceHex("app", "amount in BCD=", Amount,6);

	sdkEMVBaseSetTwoTransAmount(Amount, gbcOtherAmount);
	if(0 == gstbctcautotrade.amountexist)
	{
		sdkEMVBaseDelTLV("\x9F\x02");
	}
	if(0 == gstbctcautotrade.otheramountexist)
	{
		sdkEMVBaseDelTLV("\x9F\x03");
	}
	Trace("app", "typeexit = %d\r\n", gstbctcautotrade.typeexist);
	if(1 == gstbctcautotrade.typeexist)
	{
		sdkEMVBaseConfigTLV("\x9C", &(gstbctcautotrade.transtype), 1);
	}
	else
	{
		sdkEMVBaseDelTLV("\x9C");
	}
}

s32 IccDispCandAppListCb(u8 ucCandAppNum, u8 **pheCandApp)
{
    s32 key;
    u8 curPage;
    u8 totalPage;
    u8 i, j;
    u8 dispData[64];
    SDK_EMVBASE_C_AIDLIST **pAidListTemp;

    if(ucCandAppNum < 1 || pheCandApp == NULL)
    {
        return SDK_PARA_ERR;
    }
    pAidListTemp = (SDK_EMVBASE_C_AIDLIST **)pheCandApp;
    totalPage = (ucCandAppNum - 1) / 4;
    curPage = 0;
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, DISP_SELECTAID, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

    for(i = 0; i < 4; i++)
    {
        j = (curPage * 4) + i;

        if(j >= ucCandAppNum)
        {
            break;
        }
        memset(dispData, 0, sizeof(dispData));

        if(pAidListTemp[j]->ucAppLocalNameLen)
        {
            sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLocalName);
        }
        else if(pAidListTemp[j]->ucPreferNameLen && pAidListTemp[i]->ucIcti)
        {
            sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asPreferName);
        }
        else if(pAidListTemp[j]->ucAppLabelLen)
        {
            sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLabel);
        }
        else
        {
            sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAid);
        }
        sdkDispFillRowRam(i + SDK_DISP_LINE2, 0, dispData, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    }

    sdkDispBrushScreen();
    sdkKbKeyFlush();

    while(1)
    {
        if(gstAutoTest)
        {
            key = SDK_KEY_1;
        }
        else
        {
            key = sdkKbGetKey();
        }

        if(key != 0)
        {
            switch(key)
            {
            case SDK_KEY_1:
            case SDK_KEY_2:
            case SDK_KEY_3:
            case SDK_KEY_4:
            {
                if( (curPage * 4 + key - '0') > ucCandAppNum)
                {
                    sdkSysBeep(1);
                }
                else
                {
                    sdkSysBeep(0);
                    sdkDispClearScreen();
                    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_PLSWAIT, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                    sdkDispBrushScreen();
                    return (s32)(curPage * 4 + key - '0');
                }
            }
            break;

            case SDK_KEY_ESC:
            {
                sdkSysBeep(0);
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_PLSWAIT, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                return SDK_ESC;
            }

            case SDK_KEY_UP:
            {
                sdkSysBeep(0);

                if(curPage > 0)
                {
                    curPage--;
                }
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE1, 0, DISP_SELECTAID, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

                for(i = 0; i < 4; i++)
                {
                    j = (curPage * 4) + i;

                    if(j >= ucCandAppNum)
                    {
                        break;
                    }
                    memset(dispData, 0, sizeof(dispData));
                    if(pAidListTemp[j]->ucAppLocalNameLen)
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLocalName);
                    }
                    else if(pAidListTemp[j]->ucPreferNameLen && pAidListTemp[i]->ucIcti)
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asPreferName);
                    }
                    else if(pAidListTemp[j]->ucAppLabelLen)
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLabel);
                    }
                    else
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAid);
                    }
                    sdkDispFillRowRam(i + SDK_DISP_LINE2, 0, dispData, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                }

                sdkDispBrushScreen();
            }
            break;

            case SDK_KEY_DOWN:
            {
                sdkSysBeep(0);

                if(curPage < totalPage)
                {
                    curPage++;
                }
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE1, 0, DISP_SELECTAID, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

                for(i = 0; i < 4; i++)
                {
                    j = (curPage * 4) + i;

                    if(j >= ucCandAppNum)
                    {
                        break;
                    }
                    memset(dispData, 0, sizeof(dispData));
                    if(pAidListTemp[j]->ucAppLocalNameLen)
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLocalName);
                    }
                    else if(pAidListTemp[j]->ucPreferNameLen && pAidListTemp[i]->ucIcti)
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asPreferName);
                    }
                    else if(pAidListTemp[j]->ucAppLabelLen)
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAppLabel);
                    }
                    else
                    {
                        sprintf(dispData, "%d.%s", i + 1, (*(pAidListTemp + j))->asAid);
                    }
                    sdkDispFillRowRam(i + SDK_DISP_LINE2, 0, dispData, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                }

                sdkDispBrushScreen();
            }
            break;

            default:
            {
                sdkSysBeep(1);
            }
            break;
            }
        }
    }
}


s32 IccReadCardCb()
{
	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Present Card Again", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
	sdkDispBrushScreen();
	if(sdkSysIsNeedDispRfLogo())
  	{

	  if(!gstAutoTest)
	  {
		  sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
	  }
	  sdkIccDispRfLogo();
   }

	sdkSysBeep(0);
    u8 heUID[30] = {0};
    u8 temp[40] = {0};
    u8 handLen = 0, handData[40] = {0};
    s32 key;
    u32 timerID;
    s32 rslt;
    u32 rftimerID;
    u8 rfstate = 0;
    u8 i;

	sdkIccPowerDown();
	sdkIccCloseRfDev();

	sdkKbWaitKey(SDK_KEY_MASK_ALL, 3000);
    sdkIccOpenRfDev();

	sdkTimerStar(SDK_ICC_TIMER_AUTO * 2);
    while(1)
    {
        rslt = sdkIccResetIcc();

        if(rslt == SDK_OK)
        {
            return rslt;
        }
        else if(rslt == 2)
        {
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Multiple Cards", SDK_DISP_DEFAULT);
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Pls Use Single Card", SDK_DISP_DEFAULT);
            sdkDispBrushScreen();
			sdkIccCloseRfDev();
			sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);

			sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Present Card Again", SDK_DISP_DEFAULT);
			sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Issuer Update", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
            sdkIccOpenRfDev();
            continue;
        }
        else if(rslt != 3 && rslt != 2)
        {
			sdkDispClearScreen();
			sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Read Card Fail", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
            return SDK_ERR;
        }
        key = sdkKbGetKey();

        if(key == SDK_KEY_ESC)
        {
            return SDK_ESC;
        }

        if(1 == sdkTimerIsEnd())
        {
            return SDK_TIME_OUT;
        }
    }
}

s32 Badvice(void)
{
     s32 CID_len = 0;
     u8 CryptInfo = 0;
     u8 Bisadvice = 0;

     sdkEMVBaseReadTLV("\x9F\x27", &CryptInfo, &CID_len);
     Bisadvice  = CryptInfo & 0x08;
     return Bisadvice;
}

s32 DealTrade(void)
{
	s32 ret = SDK_OK;
	u8 rspcode[2];
	_SimData SimData;
	SDK_EMVBASE_CL_HIGHESTAID tempaid = {0};
	u8 FlowContinueFlag = 1;
	u8 AmountBCD[6], Passwd[64];
	s32 len;
	u8 trendit_dir[]="/sdcard/pure/";

	gCollisionflag = 0;
	gCollisionCounter = 0;
	gDispSecondTap = 0;

	memset(&SimData, 0, sizeof(_SimData));
	ReadSimData(&SimData);
	gTransCurrExponent = SimData.TransCurrencyExponent;
	Trace("Trade", "SimData.TransCurrencyExponent = %d\r\n");
_RETRY:

	gstDetecteCardMode = 0;
	bIssecondtapcard = 0;

	memset(gstResponseCode,0,sizeof(gstResponseCode));

	sdkSysSetCurAppDir(trendit_dir, strlen(trendit_dir));
	sdkEMVBaseTransInit();
	sdkPureTransInit();
	sdkPureSetSendOutcome(BCTCSendOutCome);
	sdkPureSetSendUIRequest(BCTCSendUIRequest);

_SECONDTAP:
	ReadCardDisp();
	sdkmSleep(500);
	ret = IccReadCard();
	Trace("app","IccReadCard ret= %d\r\n",ret);
	if(SDK_OK == ret)
	{

	}
	else if(SDK_ICC_MUTICARD == ret)
	{
		sdkTestIccDispText("Multi Card Collision");
		sdkIccPowerDown();
		gCollisionflag = 1;
		gCollisionCounter++;
		sdkmSleep(1000);
		goto _SECONDTAP;
	}
	else if(SDK_ERR == ret)
	{
		sdkTestIccDispText("Read Card error,Tx Stop");
		return ret;
	}
	else if(SDK_ICC_NOCARD == ret)
	{
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}


	ImportTradeAmount();

	ret = sdkIccResetIcc();
	if(ret != SDK_OK)
	{
		return SDK_ERR;
	}

	while(FlowContinueFlag)
	{
		ret = sdkPureTransFlow();
		Trace("app", "sdkPureTransFlow ret = %d\r\n", ret);
		switch (ret)
		{
			case EMV_REQ_PREPROCESS_LOADAIDPARAM:
				IccSetAIDEX();
				break;

			case EMV_REQ_ONLINE_PIN:
				sdkEMVBaseReadTLV("\x9F\x02", AmountBCD, &len);
				if(SDK_OK == InputIccCreditPwd(ICCONLINEPIN, 3, Passwd))
				{
					sdkPureSetInputPINRes(0, Passwd+1, Passwd[0]);
				}
				else
				{
					sdkPureSetInputPINRes(-1, Passwd+1, Passwd[0]);
				}
				break;

			case EMV_REQ_GO_ONLINE:
				ret = SendOnlineBag();
				if(ret != SDK_OK)
				{
					sdkPureImportOnlineResult(SDK_ERR, NULL);
				}
				else
				{
					sdkPureImportOnlineResult(SDK_OK, gstResponseCode);
				}

				memset(rspcode, 0, sizeof(rspcode));
				sdkEMVBaseReadTLV("\x8A", rspcode, &ret);
				TraceHex("app", "after sdkPureImportOnlineResult 8A", rspcode, 2);
				break;
			case EMV_ACCEPTED_ONLINE:
				BCTCSendTransResult(ret);
				sdkTestIccDispText("Online Approve");
				FlowContinueFlag = 0;
				break;

			case EMV_DENIALED_ONLINE:
				BCTCSendTransResult(ret);
				sdkTestIccDispText("Online Decline");
				FlowContinueFlag = 0;
				break;

			case EMV_ACCEPTED_OFFLINE:
				BCTCSendTransResult(ret);
				sdkTestIccDispText("Offline Approve");
				if(SDK_OUTCOME_CVM_OBTAINSIGNATURE == gstOutcome.CVM)
				{
					sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Please Sign", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
					sdkDispBrushScreen();
				}
				FlowContinueFlag = 0;
				break;

			case EMV_DENIALED_OFFLINE:
				sdkTestIccDispText("Offline Decline");
				FlowContinueFlag = 0;
				break;

			case EMV_SWITCH_INTERFACE:
				sdkIccPowerDown();
				sdkIccCloseRfDev();
				sdkTestIccDispText("Switch Interface");
				FlowContinueFlag = 0;
				break;

			case EMV_SEEPHONE:
				sdkIccPowerDown();
				sdkIccCloseRfDev();
				sdkTestIccDispText("See Phone");
				FlowContinueFlag = 0;
				break;

			case EMV_STA_TORNRECOVER:
			case EMV_REQ_READCAARD_AGAIN:
				sdkIccPowerDown();
				sdkmSleep(1000);
				goto _RETRY;

			case EMV_REQ_SECONDTAP:
				sdkDispClearScreen();
				sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Additional Tap", SDK_DISP_DEFAULT);
				sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Plz present card again", SDK_DISP_DEFAULT);
				sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Ready to Read", SDK_DISP_DEFAULT);
				sdkDispBrushScrecen();
				sdkIccPowerDown();
				sdkmSleep(1000);

				ret = SendOnlineBag();
				if(ret != SDK_OK)
				{
					sdkPureImportOnlineResult(SDK_ERR, NULL);
				}
				else
				{
					sdkPureImportOnlineResult(SDK_OK, gstResponseCode);
				}

				memset(rspcode, 0, sizeof(rspcode));
				sdkEMVBaseReadTLV("\x8A", rspcode, &ret);
				TraceHex("app", "after sdkPureImportOnlineResult 8A", rspcode, 2);

				gDispSecondTap = 1;
				goto _SECONDTAP;

			case EMV_ENDAPPLICATION:
				sdkIccPowerDown();
				sdkIccCloseRfDev();
				sdkTestIccDispText("End Application");
				FlowContinueFlag = 0;
				break;

			default:
				if(ret < 0)
				{
					sdkIccPowerDown();
					sdkIccCloseRfDev();
					sdkTestIccDispText("End Application");
					FlowContinueFlag = 0;
				}
				break;
		}
	}

	sdkmSleep(500);
	sdkIccPowerDown();
	memset(gstasAmount, 0, sizeof(gstasAmount));
	memset(gbcOtherAmount,0,sizeof(gbcOtherAmount));
	return ret;
}


void PostTransApproved(unsigned char CardType)
{
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Trans Approved", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
}

void PostTransDeclined(unsigned char CardType)
{
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Trans Declined", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
}

u8 GetBatchCapture()
{
	return 0;
    u8 bBatchCapture = 0;
    _SimData SimData;

    memset(&SimData, 0, sizeof(_SimData));
    ReadSimData(&SimData);
    bBatchCapture = SimData.bBatchCapture;
    return bBatchCapture;
}


int Emvb_IccGetPAN(unsigned char *pasOutPAN)
{
    unsigned char temp[128] = {0};
    unsigned char bcPAN[128] = {0};
    unsigned int i, j, len;
    EMVBASETAGCVLITEM *item = NULL, *panitem = NULL;
    unsigned char tagexistflag, pantagexistflag;
    unsigned char Track2Len, Track2Data[20];


    if(NULL == pasOutPAN)
    {
        return RLT_EMV_ERR;
    }
    panitem = emvbase_avl_gettagitemandstatus(EMVTAG_PAN, &pantagexistflag);

   if(pantagexistflag)
    {
        memset(temp, 0, sizeof(temp));
        EMVBaseBcdToAsc(temp, panitem->data, panitem->len);

        if((panitem->data[panitem->len - 1] & 0x0F) == 0x0F)
        {
            temp[panitem->len * 2 - 1] = '\0';
        }
        memcpy(pasOutPAN, temp, strlen((char *)temp));
        return RLT_EMV_OK;
    }
    return RLT_EMV_ERR;
}

u8 OnlineEncPIN(u8 *pCreditPwd)
{
    u8 i, k, tempPIN[8], PINLen, retCode;
    u8 buf[40], PanBlockAsc[16], PanBlockBcd[8], PanLen;
    u8 tetminfopan[20] = {0};
    s32 terminfopanlen = 0;
    u8 tetminfoenpin[8] = {0};

    PINLen = strlen(pCreditPwd);
    tempPIN[0] = PINLen;

//    Trace("", "pCreditPwd = %s\r\n", pCreditPwd);

    for(i = 1; i < 8; i++)
    {
        tempPIN[i] = 0xFF;
    }

    sdkAscToBcd(&tempPIN[1], pCreditPwd, PINLen);
//    TraceHex("", "tempPIN", tempPIN, 8);

    if(PINLen %  2)
    {
        tempPIN[PINLen / 2 + 1] |= 0x0F;
    }
//    TraceHex("", "tempPIN", tempPIN, 8);

    memset(buf, 0, sizeof(buf));
    k = 0;

    sdkEMVBaseReadTLV("\x5A", tetminfopan, &terminfopanlen);

    if(terminfopanlen == 0)
    {
        Emvb_IccGetPAN(buf);
        k = strlen(buf);
    }

    else
    {
//    TraceHex("", "tetminfopan", tetminfopan, terminfopanlen);

        if(terminfopanlen > 10)
        {
            terminfopanlen = 10;
        }

        for(i = 0; i < terminfopanlen; i++)
        {
            if((tetminfopan[i] & 0xF0) == 0xF0)
            {
                break;
            }
            k++;

            if((tetminfopan[i] & 0x0F) == 0x0F)
            {
                break;
            }
            k++;
        }

        sdkBcdToAsc(buf, tetminfopan, k);
    }
    memset(PanBlockAsc, '0', sizeof(PanBlockAsc));

    if(k > 0)
    {
        PanLen = k - 1;

        if(PanLen > 12)
        {
            PanLen = 12;
        }

        for(i = 0; i < PanLen; i++)
        {
            PanBlockAsc[15 - i] = buf[k - i - 2];
        }
    }
//    TraceHex("", "PanBlockAsc", PanBlockAsc, 16);
    sdkAscToBcd(PanBlockBcd, PanBlockAsc, 16);
//    TraceHex("", "PanBlockBcd", PanBlockBcd, 8);

    for(i = 0; i < 8; i++)
    {
        tempPIN[i] ^= PanBlockBcd[i];
    }

//    TraceHex("", "tempPIN", tempPIN, 8);
//    TraceHex("", "PINDesKey", PINDesKey, 8);
    sdkDesS(1, tempPIN, PINDesKey);

    memcpy(pCreditPwd, tempPIN, 8);
    return SDK_OK;
}

s32 DetecteOther()
{
#if 0
    s32 rslt;
    SDK_ICC_PARAM *newpstIccParam = NULL;

    if((newpstIccParam = sdkGetMem(sizeof(SDK_ICC_PARAM))) == NULL)
    {
        return SDK_OK;
    }

    if(gstCardData == NULL)
    {
        gstCardData = sdkGetMem(sizeof(SDK_ICC_CARDDATA));
    }
    memset(newpstIccParam, 0, sizeof(SDK_ICC_PARAM));
    memset(gstCardData, 0, sizeof(SDK_ICC_CARDDATA));

#if 0 /*Modify by luohuidong at 2018.07.17  16:52 */

    rslt = sdkIccGetMagData(newpstIccParam, gstCardData);

    if(rslt == SDK_OK)         //鎴愬姛杩斿洖
    {
        Trace("emv", "DetecteOther MAG\r\n");
        gstDetecteCardMode = SDK_ICC_MAG;
        sdkFreeMem(newpstIccParam);
        return SDK_EQU;
    }
    else if(rslt != SDK_ICC_NOCARD)         //澶辫触閫€鍑?
    {
//            gstDetecteCardMode = SDK_ICC_MAG;
//            sdkFreeMem(newpstIccParam);
//            return SDK_EQU;
    }

#endif /* if 0 */

    rslt = sdkIccGetCardStatus(newpstIccParam, 1);

    if(rslt == SDK_OK)         //鎴愬姛杩斿洖
    {
        Trace("emv", "DetecteOther ICC\r\n");
        gstDetecteCardMode = SDK_ICC_ICC;
        sdkFreeMem(newpstIccParam);
        return SDK_EQU;
    }
    else if(rslt != SDK_ICC_NOCARD)         //澶辫触閫€鍑?
    {
//            gstDetecteCardMode = SDK_ICC_ICC;
//            sdkFreeMem(newpstIccParam);
//            return SDK_EQU;
    }
    sdkFreeMem(newpstIccParam);
    return SDK_OK;

#else
	s32 key = 0;
	u32 count;
	for(count = 0; count < 500; count++)
	{
		sdkmSleep(1);
		key = sdkKbGetKey();
		if(key != 0)
		{
			break;
		}
	}
	Trace("lishiyao", "DetecteOther Get Key = %02x\r\n", key);
	if(key == SDK_KEY_ESC)
	{
		return SDK_EQU;
	}

	return SDK_OK;
#endif

}

s32 AppqPBOCPreTrans()
{
    s32 iRet, iTotal, iGetNum;
    SDK_EMVBASE_AID_STRUCT tempAid2;
    u8 i;
    u8 bcdamt[6] = {0}, tradeamt[6] = {0};
    u8 buf[16] = {0};

    iRet = sdkEMVBaseGetAIDListNum(&iTotal);

    if (iRet != SDK_OK || iTotal == 0)
    {
        return SDK_OK;
    }

    for(i = 0; i < iTotal; i++)
    {
        iRet = sdkEMVBaseGetAnyAIDList(i, 1, &tempAid2, &iGetNum);

        if (iRet != SDK_OK || iGetNum == 0)
        {
            return SDK_OK;
        }

        if(memcmp(tempAid2.Aid, "\xA0\x00\x00\x03\x33", 5) == 0)
        {
            memcpy(bcdamt, tempAid2.cl_offlinelimit, 6);
            break;
        }
    }

    TraceHex("", "bcdamt", bcdamt, 6);

    sdkAscToBcdR(tradeamt, gstasAmount, 6);

    if(memcmp(tradeamt, bcdamt, 6) >= 0)
    {
        return SDK_ERR;
    }
    return SDK_OK;
}
