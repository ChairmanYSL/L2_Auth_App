#include "dllemvbase.h"
#include "dllemvcontact.h"
#include "sdkGlobal.h"
#include "sdkemvbaseprivate.h"
#include "sdkemvcontactprivate.h"
#include "sdkemvcontact.h"

EMVCONTACTTRADEPARAMETER *gstEmvTradeParam = NULL;
EMVCONTACTTradeUnionStruct *gstEmvTradeUnionStruct = NULL;
SDK_EMVCONTACT_TRADE_PARAM *gstsdkEmvTradeTable = NULL;


void sdkEmvContactGetRandom(unsigned char* randpad, unsigned int randLen)
{
	int i = 0;
	unsigned char buf[4] = {0};

	Trace("emv", "randLen: %d\r\n", randLen);
	for(i=0; i<randLen; i+=4)
	{
		memset(buf, 0x00, 4);
		if(i+4 > randLen)
		{
			sdkGetRandom(randpad+i, randLen-i);
		}
		else
		{
    		sdkGetRandom(randpad+i, 4);
		}
	}
}

void sdkEmvctPostOfflinePinSuc(void)
{
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "PIN OK", SDK_DISP_DEFAULT);
    sdkmSleep(1000);
}

void sdkEmvctPostOfflinePinErr(void)
{
//    sdkSysBeep(SDK_SYS_BEEP_ERR);
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Incorrect PIN!", SDK_DISP_DEFAULT);

    sdkmSleep(1000);
}

s32 sdkEmvctPostInputPinAgain(u8 LastTry)
{
    s32 key;

    sdkDispClearRow(SDK_DISP_LINE2);
    sdkDispClearRow(SDK_DISP_LINE3);
    sdkDispClearRow(SDK_DISP_LINE4);
    sdkDispClearRow(SDK_DISP_LINE5);

    Verify(sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Incorrect PIN!", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    Verify(sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Pls try again", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);

    if(LastTry == 1)
    {
        Verify(sdkDispFillRowRam(SDK_DISP_LINE4, 0, "(Last Time)", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    }
    sdkDispBrushScreen();
    key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 15 * 100);

    return key;
}

void sdkEmvctPostPerformCDCVM(void)
{
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "remove card, perform", SDK_DISP_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE3, 0, "CDCVM, and try again", SDK_DISP_DEFAULT);
    sdkmSleep(1000);
}


#define ASCII      0x01
#define CFONT      0x00

static const char gcSdkIdTypeTab[][16] =
{
    "身份证", "军官证", "护照", "入境证", "临时身份证", "其它"
};

void sdkEmvContactScrDisplay(u8 mode, u8 x, u8 y, const char *Disdata)
{
    u8 strBuf[21];

    memset(strBuf, 0, sizeof(strBuf));
    strcpy((char *)strBuf, Disdata);
    Verify(sdkDispFillRowRam(x, y, strBuf, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    sdkDispBrushScreen();
}

s32 sdkEmvContactVerifyIdCard(void)
{
    u8 temp[22];
    u8 CardHolderType;
    EMVBASETAGCVLITEM *CardHolderIdNoitem;
    s32 key;

    memset(temp, 0, sizeof(temp));

    sdkDispClearScreen();

    strcpy((char *)temp, "证件:");

    CardHolderType = emvbase_avl_gettagvalue(EMVTAG_CardHoldIdType);

    strcat((char *)temp, gcSdkIdTypeTab[CardHolderType]);
    sdkEmvContactScrDisplay(CFONT, 0, 0, (char *)temp);

    memset(temp, 0, sizeof(temp));
    sdkEmvContactScrDisplay(CFONT, 1, 0, (char *)"证件�?");
    CardHolderIdNoitem = emvbase_avl_gettagitempointer(EMVTAG_CardHoldIdNo);

    if(CardHolderIdNoitem->len > 20)
    {
        memcpy(temp, CardHolderIdNoitem->data, 20);
        sdkEmvContactScrDisplay(CFONT, 2, 0, (char *)temp);
        memset(temp, 0, sizeof(temp));
        memcpy(temp, &CardHolderIdNoitem->data[20], CardHolderIdNoitem->len - 20);
        sdkEmvContactScrDisplay(CFONT, 3, 0, (char *)temp);
    }
    else
    {
        memcpy(temp, CardHolderIdNoitem->data, CardHolderIdNoitem->len);
        sdkEmvContactScrDisplay(CFONT, 2, 0, (char *)temp);
    }
    sdkEmvContactScrDisplay(CFONT, 4, 0, "1.正确  2.错误");

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
                    {
//                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        sdkDispClearScreen();
						sdkDispFillRowRam(SDK_DISP_LINE3, 0, "请稍�?..", SDK_DISP_DEFAULT);

                        if(key == '1')
                        {
                            return SDK_OK;
                        }
                        else
                        {
                            return SDK_ERR;
                        }
                    }

                 default:
                    {
//                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                    break;
            }
        }
    }
}

unsigned char sdkEmvContactFunCB(unsigned char pintype, unsigned char *ASCCreditPwd)
{
    unsigned char rslt;
    unsigned char tCreditPwd[64], tICCEncryptWay;
    s32 rlt;
    u8 PINTryCount = 0;

	if(gstEMVBase_TradeParam->InputPWD == NULL)
	{
		Trace("emv","ERROR!!!USER NOT SET INPUTPIN FUNCTION\r\n");
		return RLT_EMV_ERR;
	}

	Trace("emv","pintype = %d\r\n",pintype);

    if(pintype == EMVB_DATAAUTH_INPUTPINSTATUS)
    {
        if(ASCCreditPwd[0] == 0x00)
        {
            if(gstsdkEmvTradeTable->DispPromptData == NULL)
            {
                sdkEmvctPostOfflinePinSuc();
            }
            else
            {
                (*gstsdkEmvTradeTable->DispPromptData)(SDK_EMV_ICC_OFFLINE_PIN_SUC);
            }
        }
        else
        {
            if(ASCCreditPwd[1] == 0)
            {
                if(gstsdkEmvTradeTable->DispPromptData == NULL)
                {
                    sdkEmvctPostOfflinePinErr();
                }
                else
                {
                    (*gstsdkEmvTradeTable->DispPromptData)(SDK_EMV_ICC_OFFLINE_PIN_ERR);
                }
            }
            else
            {
                if(ASCCreditPwd[1] == 1)
                {
                    if(gstsdkEmvTradeTable->DispPromptData == NULL)
                    {
                        sdkEmvctPostInputPinAgain(1);
                    }
                    else
                    {
                        (*gstsdkEmvTradeTable->DispPromptData)(SDK_EMV_ICC_INPUT_PIN_AGAIN);
                    }
                }
                else
                {
                    if(gstsdkEmvTradeTable->DispPromptData == NULL)
                    {
                        sdkEmvctPostInputPinAgain(0);
                    }
                    else
                    {
                        (*gstsdkEmvTradeTable->DispPromptData)(SDK_EMV_ICC_INPUT_PIN_AGAIN);
                    }
                }
            }
        }
		return RLT_EMV_OK;
	}
	else if(pintype == EMVB_DATAAUTH_VERIFYIDCARD)
    {
		if(gstsdkEmvTradeTable->DispVerifyIdCard == NULL)
        {
            if( SDK_OK == sdkEmvContactVerifyIdCard())
            {
            	return RLT_EMV_OK;
			}
			else
			{
				return RLT_EMV_ERR;
			}
		}
        else
        {
			rlt = (*gstsdkEmvTradeTable->DispVerifyIdCard)();
			if( SDK_OK == rlt)
            {
            	return RLT_EMV_OK;
			}
			else
			{
				return RLT_EMV_ERR;
			}
        }

        return RLT_EMV_OK;
    }
    else if(pintype == EMVB_NEEDREFER)
    {
    	if(gstsdkEmvTradeTable->DispIssuerReferral != NULL)
    	{
    		//return RLT_EMV_OK;
			rlt = (*gstsdkEmvTradeTable->DispIssuerReferral)();
			if( SDK_OK == rlt)
            {
            	return RLT_EMV_OK;
			}
			else
			{
				return RLT_EMV_ERR;
			}
		}
		else
		{
       		return RLT_EMV_ERR;
		}
    }
	else if(pintype == EMVB_DISP_PERFORMCDCVM)
	{
		if(gstsdkEmvTradeTable->DispPromptData != NULL)
        {
            (*gstsdkEmvTradeTable->DispPromptData)(SDK_EMV_ICC_PERFORM_CDCVM);
        }

		return RLT_EMV_OK;
	}

    memset(tCreditPwd, 0, sizeof(tCreditPwd));

    if(pintype == EMVB_DATAAUTH_OFFLINEENCPIN || pintype == EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN)
    {
        tICCEncryptWay = SDK_PED_IC_OFFLINE_PIN;
		sdkEMVBaseReadTLV("\x9F\x17", &PINTryCount, &rlt);
    }
    else
    {
        tICCEncryptWay = SDK_PED_IC_ONLINE_PIN;
    }

	if(pintype == EMVB_DATAAUTH_OFFLINEENCPIN || pintype == EMVB_DATAAUTH_ONLINEPIN)
    {
//        sdkPEDSetOfflinePinMode(0); // enciphered
    }
    else
    {
//        sdkPEDSetOfflinePinMode(1); // plaintext
    }

	rlt = gstEMVBase_TradeParam->InputPWD(tICCEncryptWay, PINTryCount, tCreditPwd);

	if(rlt != SDK_OK && rlt != SDK_PED_NOPIN)
	{
		Trace("emv","WARNING!!!USER CANCEL INPUT PIN\r\n");
		return RLT_ERR_EMV_CancelTrans;
	}

	if(rlt == SDK_OK)
	{
		if(tCreditPwd[0])
		{
			memcpy(ASCCreditPwd, &tCreditPwd[1], tCreditPwd[0]);
			return RLT_EMV_OK;
		}
		else
		{
			return RLT_ERR_EMV_InputBYPASS;
		}
	}
	else if(rlt == SDK_PED_NOPIN)
	{
		return RLT_ERR_EMV_InputBYPASS;
	}
	else
	{
		return RLT_ERR_EMV_CancelTrans;
	}
}

unsigned char sdkEmvContactCheckDataContainerCB(const u8 *ContainerID)
{
	s32 ret = 0;


	if(NULL == gstsdkEmvTradeTable->CheckDataContainer)
	{
		return RLT_EMV_ERR;
	}

	ret = gstsdkEmvTradeTable->CheckDataContainer(ContainerID);
	if(SDK_OK == ret)
	{
		return RLT_EMV_OK;
	}
	else
	{
		return RLT_EMV_ERR;
	}
}

void sdkEmvContactStoreContainerContentCB(const u8 *ContainerID, const u8 *ContainerContent, u16 ContentLen)
{
	s32 ret = 0;


	if(NULL == gstsdkEmvTradeTable->StoreContainerContent)
	{
		return;
	}

	ret = gstsdkEmvTradeTable->StoreContainerContent(ContainerID, ContainerContent, ContentLen);
}

s32 sdkEmvContactSetDispAppList(s32(*fun_dispapplist)(u8 ucCandAppNum, u8 **pheCandApp))
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->DispCandAppList = fun_dispapplist;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}

s32 sdkEmvContactSetDispReselectApp(s32(*fun_dispReselectApp)())
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->DispReselectApp = fun_dispReselectApp;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}

s32 sdkEmvContactSetBeforeGPO(s32(*fun_setbeforegpo)())
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->setBeforeGPO = fun_setbeforegpo;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}













s32 sdkEmvContactSetDispCardNO(s32(*fun_setdispcardno)(const u8 *pasPAN))
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->DispCardNo = fun_setdispcardno;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}

s32 sdkEmvContactSetDispPromptData(s32(*fun_dispPromptData)(unsigned char ePromptType))
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->DispPromptData = fun_dispPromptData;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}

s32 sdkEmvContactSetDispVerifyIdCard(s32(*fun_dispVerifyIdCard)())
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->DispVerifyIdCard = fun_dispVerifyIdCard;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}

s32 sdkEmvContactSetDispIssuerReferral(s32(*fun_dispIssuerReferral)())
{
	if(gstsdkEmvTradeTable)
	{
		gstsdkEmvTradeTable->DispIssuerReferral = fun_dispIssuerReferral;
		return SDK_OK;
	}
	return SDK_PARA_ERR;
}

s32 sdkEmvContactSetForceOnline(bool forceonline)
{
    if(gstsdkEmvTradeTable == NULL)
    {
        return SDK_PARA_ERR;
    }

    gstsdkEmvTradeTable->bIsForceOnline = (bool)forceonline;

    return SDK_OK;
}




s32 sdkEmvContactSetCmpCardNO(s32 (*CheckCardNo)(const u8 *pasPAN))
{
	if(gstEmvTradeUnionStruct == NULL)
	{
		return SDK_PARA_ERR;
	}

	gstEmvTradeUnionStruct->CheckCardNo = CheckCardNo;

	return SDK_OK;
}

s32 sdkEmvContactSetSupportDataStorage(bool supportDataStorage)
{
    if(gstsdkEmvTradeTable == NULL)
    {
        return SDK_PARA_ERR;
    }

    gstsdkEmvTradeTable->bIsSupportDataStorage = supportDataStorage;
	emvbase_avl_settag(EMVTAG_DPASSupportDataStorage, gstsdkEmvTradeTable->bIsSupportDataStorage);
    return SDK_OK;
}

s32 sdkEmvContactSetSupportExtendedLogging(bool supportExtendedLogging)
{
    if(gstsdkEmvTradeTable == NULL)
    {
        return SDK_PARA_ERR;
    }

    gstsdkEmvTradeTable->bIsSupportExtendedLogging = supportExtendedLogging;
	emvbase_avl_settag(EMVTAG_DPASSupportExtendedLogging, gstsdkEmvTradeTable->bIsSupportExtendedLogging);
    return SDK_OK;
}

s32 sdkEmvContactSetCheckDataContainer(s32(*fun_setcheckdatacontainer)(const u8 *ContainerID))
{
    if(gstsdkEmvTradeTable == NULL)
    {
        return SDK_PARA_ERR;
    }

    gstsdkEmvTradeTable->CheckDataContainer = fun_setcheckdatacontainer;
    return SDK_OK;
}

s32 sdkEmvContactSetStoreContainerContent(s32(*fun_setstorecontainercontent)(const u8 *ContainerID, const u8 *ContainerContent, u16 ContentLen))
{
    if(gstsdkEmvTradeTable == NULL)
    {
        return SDK_PARA_ERR;
    }

    gstsdkEmvTradeTable->StoreContainerContent = fun_setstorecontainercontent;
    return SDK_OK;
}

s32 sdkEmvContactSetGetCardTransSum(s32(*fun_setgetcardtranssum)(unsigned int *sum))
{
	if(gstsdkEmvTradeTable == NULL)
    {
        return SDK_PARA_ERR;
    }

	gstsdkEmvTradeTable->GetCardTransSum = fun_setgetcardtranssum;

	return SDK_OK;
}

static void sdkEmvContactTradeParamDestory(void)
{
	s32 i = 0;


    if(gstEmvTradeParam != NULL)
    {
        if(gstEmvTradeParam->CAPK != NULL)
        {
            emvbase_free(gstEmvTradeParam->CAPK);
            gstEmvTradeParam->CAPK = NULL;
        }

        if(gstEmvTradeParam->SelectedApp != NULL)
        {
            emvbase_free(gstEmvTradeParam->SelectedApp);
            gstEmvTradeParam->SelectedApp = NULL;
        }

        if(gstEmvTradeParam->AppListCandidate != NULL)
        {
        	for(i=0; i<gstEmvTradeParam->AppListCandidateMaxNum; i++)
			{
				if(NULL != gstEmvTradeParam->AppListCandidate[i].ASRPD)
				{
					emvbase_free(gstEmvTradeParam->AppListCandidate[i].ASRPD);
					gstEmvTradeParam->AppListCandidate[i].ASRPDLen = 0;
					gstEmvTradeParam->AppListCandidate[i].ASRPD = NULL;
				}
				if(NULL != gstEmvTradeParam->AppListCandidate[i].rfu)
				{
					emvbase_free(gstEmvTradeParam->AppListCandidate[i].rfu);
					gstEmvTradeParam->AppListCandidate[i].rfuLen = 0;
					gstEmvTradeParam->AppListCandidate[i].rfu = NULL;
				}
			}
            emvbase_free(gstEmvTradeParam->AppListCandidate);
            gstEmvTradeParam->AppListCandidate = NULL;
        }

		if(gstEmvTradeParam->AuthData != NULL)
        {
            emvbase_free(gstEmvTradeParam->AuthData);
            gstEmvTradeParam->AuthData = NULL;
        }
		if(gstEmvTradeParam->IPKModul != NULL)
        {
            emvbase_free(gstEmvTradeParam->IPKModul);
            gstEmvTradeParam->IPKModul = NULL;
        }
		if(gstEmvTradeParam->ICCPKModul != NULL)
        {
            emvbase_free(gstEmvTradeParam->ICCPKModul);
            gstEmvTradeParam->ICCPKModul = NULL;
        }
		if(gstEmvTradeParam->RedundantData != NULL)
        {
            emvbase_free(gstEmvTradeParam->RedundantData);
            gstEmvTradeParam->RedundantData = NULL;
        }

		if(gstEmvTradeParam->DispSeqListData.AppPriority != NULL)//sjz
		{
            emvbase_free(gstEmvTradeParam->DispSeqListData.AppPriority);
            gstEmvTradeParam->DispSeqListData.AppPriority = NULL;
		}
		if(gstEmvTradeParam->DispSeqListData.AppSeq != NULL)//sjz
		{
            emvbase_free(gstEmvTradeParam->DispSeqListData.AppSeq);
            gstEmvTradeParam->DispSeqListData.AppSeq = NULL;
		}
        emvbase_free(gstEmvTradeParam);
        gstEmvTradeParam = NULL;
    }

    if(gstEmvTradeUnionStruct != NULL)
    {
        emvbase_free(gstEmvTradeUnionStruct);
        gstEmvTradeUnionStruct = NULL;
    }

    if(gstsdkEmvTradeTable != NULL)
    {
        emvbase_free(gstsdkEmvTradeTable);
        gstsdkEmvTradeTable = NULL;
    }

}


void sdkEmvContactMalloctradememory()
{
	s32 i = 0;


    if(gstEmvTradeUnionStruct == NULL)
    {
        gstEmvTradeUnionStruct = (EMVCONTACTTradeUnionStruct *)emvbase_malloc(sizeof(EMVCONTACTTradeUnionStruct));
    }
	memset(gstEmvTradeUnionStruct,0,(sizeof(EMVCONTACTTradeUnionStruct)));

    if(gstEmvTradeParam != NULL)
    {
        if(gstEmvTradeParam->CAPK != NULL)
        {
            emvbase_free(gstEmvTradeParam->CAPK);
            gstEmvTradeParam->CAPK = NULL;
        }

        if(gstEmvTradeParam->SelectedApp != NULL)
        {
            emvbase_free(gstEmvTradeParam->SelectedApp);
            gstEmvTradeParam->SelectedApp = NULL;
        }

        if(gstEmvTradeParam->AppListCandidate != NULL)
        {
        	for(i=0; i<gstEmvTradeParam->AppListCandidateMaxNum; i++)
			{
				if(NULL != gstEmvTradeParam->AppListCandidate[i].ASRPD)
				{
					emvbase_free(gstEmvTradeParam->AppListCandidate[i].ASRPD);
					gstEmvTradeParam->AppListCandidate[i].ASRPDLen = 0;
					gstEmvTradeParam->AppListCandidate[i].ASRPD = NULL;
				}
				if(NULL != gstEmvTradeParam->AppListCandidate[i].rfu)
				{
					emvbase_free(gstEmvTradeParam->AppListCandidate[i].rfu);
					gstEmvTradeParam->AppListCandidate[i].rfuLen = 0;
					gstEmvTradeParam->AppListCandidate[i].rfu = NULL;
				}
			}
            emvbase_free(gstEmvTradeParam->AppListCandidate);
            gstEmvTradeParam->AppListCandidate = NULL;
        }

		if(gstEmvTradeParam->AuthData != NULL)
        {
            emvbase_free(gstEmvTradeParam->AuthData);
            gstEmvTradeParam->AuthData = NULL;
        }
		if(gstEmvTradeParam->IPKModul != NULL)
        {
            emvbase_free(gstEmvTradeParam->IPKModul);
            gstEmvTradeParam->IPKModul = NULL;
        }
		if(gstEmvTradeParam->ICCPKModul != NULL)
        {
            emvbase_free(gstEmvTradeParam->ICCPKModul);
            gstEmvTradeParam->ICCPKModul = NULL;
        }
		if(gstEmvTradeParam->RedundantData != NULL)
        {
            emvbase_free(gstEmvTradeParam->RedundantData);
            gstEmvTradeParam->RedundantData = NULL;
        }
		if(gstEmvTradeParam->DispSeqListData.AppPriority != NULL)//sjz
		{
            emvbase_free(gstEmvTradeParam->DispSeqListData.AppPriority);
            gstEmvTradeParam->DispSeqListData.AppPriority = NULL;
		}
		if(gstEmvTradeParam->DispSeqListData.AppSeq != NULL)//sjz
		{
            emvbase_free(gstEmvTradeParam->DispSeqListData.AppSeq);
            gstEmvTradeParam->DispSeqListData.AppSeq = NULL;
		}
        emvbase_free(gstEmvTradeParam);
        gstEmvTradeParam = NULL;
    }

    if(gstEmvTradeParam == NULL)
    {
		gstEmvTradeParam = (EMVCONTACTTRADEPARAMETER *)emvbase_malloc(sizeof(EMVCONTACTTRADEPARAMETER));
    }

	memset(gstEmvTradeParam,0,sizeof(EMVCONTACTTRADEPARAMETER));

	gstEmvTradeParam->SelectedApp = (EMVBASE_LISTAPPDATA *)emvbase_malloc(sizeof(EMVBASE_LISTAPPDATA));
	memset((unsigned char *)gstEmvTradeParam->SelectedApp, 0, sizeof(EMVBASE_LISTAPPDATA));

	gstEmvTradeParam->AppListCandidate = (EMVBASE_LISTAPPDATA *)emvbase_malloc(sizeof(EMVBASE_LISTAPPDATA) * gAppListCandicateMaxNum);
	memset((unsigned char *)gstEmvTradeParam->AppListCandidate, 0, sizeof(EMVBASE_LISTAPPDATA) * gAppListCandicateMaxNum);

	if(gstEmvTradeParam->DispSeqListData.AppPriority == NULL)//sjz
	{
		gstEmvTradeParam->DispSeqListData.AppPriority = emvbase_malloc(gAppListCandicateMaxNum);
	}
	memset(gstEmvTradeParam->DispSeqListData.AppPriority, 0, gAppListCandicateMaxNum);
	if(gstEmvTradeParam->DispSeqListData.AppSeq == NULL)//sjz
	{
		gstEmvTradeParam->DispSeqListData.AppSeq = emvbase_malloc(gAppListCandicateMaxNum);
	}
	memset(gstEmvTradeParam->DispSeqListData.AppSeq, 0, gAppListCandicateMaxNum);
	gstEmvTradeParam->AppListCandidateMaxNum = gAppListCandicateMaxNum;

	if(gstsdkEmvTradeTable == NULL)
	{
		gstsdkEmvTradeTable = (SDK_EMVCONTACT_TRADE_PARAM *)emvbase_malloc(sizeof(SDK_EMVCONTACT_TRADE_PARAM));
	}
	memset(gstsdkEmvTradeTable,0,(sizeof(SDK_EMVCONTACT_TRADE_PARAM)));
}

static void sdkEmvContactIccIsoCommand(EMVBASE_APDU_SEND * ApduSend, EMVBASE_APDU_RESP * apdu_r)
{
	sdkEMVBase_ContactIsoCommand(ApduSend, apdu_r);
}

static void sdkEmvContactIccIsoCommandEx(EMVBASE_APDU_SEND *ApduSend, EMVBASE_APDU_RESP *apdu_r, unsigned short offset)
{
    //1--icc card
//	sdkEMVBase_IsoCommandEx(ApduSend, apdu_r, offset, 1);
}


void sdkEmvContactTransTermDataInit(void)
{
    gstEmvTradeParam->SelectedAppNo = 0xff;
	gstEmvTradeParam->ReadLogFlag = 0;
	gstsdkEmvTradeTable->bIsForceOnline = 1;
	gstEmvTradeParam->bAbleOnline = 1;
	gstEmvTradeParam->AuthDataFixLen = 0xffff;



    gstEmvTradeUnionStruct->EmvTradeParam = gstEmvTradeParam;
    gstEmvTradeUnionStruct->IsoCommand = sdkEmvContactIccIsoCommand;
	gstEmvTradeUnionStruct->IsoCommandEx = sdkEmvContactIccIsoCommandEx;
//	gstEmvTradeUnionStruct->IsoCommandEx = NULL;
	gstEmvTradeUnionStruct->CheckMatchTermAID = sdkEMVBase_CheckMatchTermAID_CT;
	gstEmvTradeUnionStruct->ReadTermAID = sdkEMVBase_ReadTermAID_CT;
	gstEmvTradeUnionStruct->EMVB_InputCreditPwd = sdkEmvContactFunCB;
	gstEmvTradeUnionStruct->EMVB_RandomNum = sdkEmvContactGetRandom;
	gstEmvTradeUnionStruct->CheckDataContainer = sdkEmvContactCheckDataContainerCB;
	gstEmvTradeUnionStruct->StoreContainerContent = sdkEmvContactStoreContainerContentCB;

}


s32 sdkEmvContactTransInitPrivate()
{
    sdkEmvContactMalloctradememory();
    sdkEmvContactTransTermDataInit();

    return SDK_OK;
}

static s32 sdkEmvContactGetOfflineEncdata(unsigned char * encdata, unsigned int dataLen)
{
	return 0;
}



s32 sdkEmvContactTransInit()
{
    sdkEmvContactTradeParamDestory();
    sdkEmvContactTransInitPrivate();

	if(gstEMVBase_TradeParam && gstEMVBase_TradeParam->termipkrevokecheck)
	{
		sdkEmvContactSetRevocationKey(gstEMVBase_TradeParam->termipkrevokecheck);
	}

	if(gstEmvTradeUnionStruct)
	{
		gstEmvTradeUnionStruct->getofflineencdata = sdkEmvContactGetOfflineEncdata;//

		gstEmvTradeUnionStruct->GetHashWithMagMask =  NULL;//sdkEmvContactGetHashWithMagMask;

		gstEmvTradeUnionStruct->CheckTrack2Mask = NULL;//add by cj
	}
	return SDK_OK;
}

void sdkEmvContactIntToByteArray(unsigned long var,unsigned char* buf,unsigned char bufLen)
{
	int i;
	unsigned long temp;
	temp=var;
	for(i=0;i<bufLen;i++)
	{
		buf[bufLen-1-i]=temp%256;
		temp=temp/256;
	}
}

void sdkEmvContactImportOtherAmount(const u8 * lpAmount)
{
    u32 tmpsdkAmtAuthBin = 0;
    u8 tmpAmtAuthBin[4] = {0};

    sdkBcdToU32(&tmpsdkAmtAuthBin, lpAmount, 6);
    sdkEmvContactIntToByteArray(tmpsdkAmtAuthBin, tmpAmtAuthBin, 4);

    emvbase_avl_createsettagvalue(EMVTAG_AmtOtherBin, tmpAmtAuthBin, 4);
}

s32 sdkEmvContactDifferEMVBase()
{
	emvbase_avl_settagtype(EMVTAGTYPE_EMV);

	emvbase_avl_settag(EMVTAG_POSEntryMode, 0x05);
	emvbase_avl_settag(EMVTAG_MCHIPTransCateCode, 'R');

	//emvbase_avl_settag(EMVTAG_DPASSupportDataStorage, gstsdkEmvTradeTable->bIsSupportDataStorage);
	//emvbase_avl_settag(EMVTAG_DPASSupportExtendedLogging, gstsdkEmvTradeTable->bIsSupportExtendedLogging);

	return SDK_OK;
}

s32 sdkEmvContactGetAppCandidate()
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

	retCode = EmvContact_AppSelect(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_AppSelect ret = %d\r\n",retCode);
    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactGetAppCandidate ret = %d\r\n",rlt);
    return rlt;
}


s32 sdkEmvContactOrganizeAppList()
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

	retCode = EmvContact_ChooseApp(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_ChooseApp ret = %d\r\n",retCode);

	if(RLT_ERR_EMV_NoAppSel == retCode)
    {
        if(gstEmvTradeUnionStruct->EmvTradeParam->appblockflag == 1)
        {
           retCode = RLT_ERR_EMV_APPBLOCK;
        }
    }

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);

	Trace("emv","sdkEmvContactOrganizeAppList ret = %d\r\n",rlt);
    return rlt;
}

void sdkEmvContactDealDispAppList()
{
    SDK_EMV_AIDLIST **pAppList = NULL;
    u8 i = 0, j = 0, len = 0;
    s32 selectNO = 0;

    pAppList = (SDK_EMV_AIDLIST**)sdkGetMem(sizeof(SDK_EMV_AIDLIST*) * gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidatenum);

    if(NULL == pAppList)
    {
        return;
    }

    for(i = 0; i < gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidatenum; i++)
    {
        *(pAppList + i) = (SDK_EMV_AIDLIST *)sdkGetMem(sizeof(SDK_EMV_AIDLIST));

        if(*(pAppList + i) == NULL)
        {
            break;
        }
        memset(*(pAppList + i), 0, sizeof(SDK_EMV_AIDLIST));

        if(gstEmvTradeUnionStruct->EmvTradeParam->DispSeqListData.seqtype == 2)
        {
            j = gstEmvTradeUnionStruct->EmvTradeParam->DispSeqListData.AppSeq[i];
        }
        else
        {
            j = i;
        }
        (*(pAppList + i))->ucAidLen = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AIDLen * 2;
        len = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AIDLen;
        //sdkBcdToAsc((*(pAppList + i))->asAid, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AID, len * 2);
        sdkBcdToAsc((*(pAppList + i))->asAid, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AID, len);
		TraceHex("emv","gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AID BCD",gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AID,16);
		TraceHex("emv","(*(pAppList + i))->asAid ASC",(*(pAppList + i))->asAid,32);


        if(len % 2)
        {
            (*(pAppList + i))->asAid[len * 2] = 0;
        }
		Trace("emv", "i: %d, AppLabelLen: %d\r\n", i, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AppLabelLen);
		Trace("emv", "AppLabel: %s\r\n", gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AppLabel);
		Trace("emv", "i: %d, PreferNameLen: %d\r\n", i, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].PreferNameLen);
		Trace("emv", "PreferName: %s\r\n", gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].PreferName);
		Trace("emv", "i: %d, ICTI: %d\r\n", i, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].ICTI);
		TraceHex("emv", "aid: ", gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AID, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AIDLen);

        (*(pAppList + i))->ucAppLabelLen = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AppLabelLen;

        len = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AppLabelLen;

        memcpy((*(pAppList + i))->asAppLabel, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].AppLabel, len);

        (*(pAppList + i))->ucPreferNameLen = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].PreferNameLen;

        len = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].PreferNameLen;

        memcpy((*(pAppList + i))->asPreferName, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].PreferName, len);

        (*(pAppList + i))->ucPriority = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].Priority;

        (*(pAppList + i))->ucLangPreferLen = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].LangPreferLen;

        len = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].LangPreferLen;

        memcpy((*(pAppList + i))->asLangPrefer, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].LangPrefer, len);

        (*(pAppList + i))->ucIcti = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].ICTI;

		if(NULL != gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].rfu) // for USA GP  2021.09.18
		{
			(*(pAppList + i))->rfuLen = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].rfuLen;
			(*(pAppList + i))->rfu = (unsigned char *)sdkGetMem((*(pAppList + i))->rfuLen);
			if(NULL == (*(pAppList + i))->rfu)
			{
				break;
			}
			len = gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].rfuLen;
	        memcpy((*(pAppList + i))->rfu, gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidate[j].rfu, len);
		}
    }

    if(i != gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidatenum)
    {
        for(j = 0; j < i; j++)
        {
        	if(NULL != (*(pAppList + j))->rfu) // for USA GP  2021.09.18
        	{
				sdkFreeMem((*(pAppList + j))->rfu);
			}
            sdkFreeMem(*(pAppList + j));
        }

        sdkFreeMem(pAppList);
        return;
    }

    selectNO = (*gstsdkEmvTradeTable->DispCandAppList)(i, (u8**)pAppList);

    if(0 < selectNO && gstEmvTradeUnionStruct->EmvTradeParam->AppListCandidatenum >= selectNO)
    {
        gstEmvTradeUnionStruct->EmvTradeParam->DispSeqListData.SelectedNum = selectNO - 1;
        gstEmvTradeUnionStruct->EmvTradeParam->DispSeqListData.SelectedResult = RLT_EMV_OK;
    }
    else
    {
        gstEmvTradeUnionStruct->EmvTradeParam->DispSeqListData.SelectedResult = RLT_ERR_EMV_CancelTrans;
    }

    for(j = 0; j < i; j++)
    {
    	if(NULL != (*(pAppList + j))->rfu) // for USA GP  2021.09.18
    	{
			sdkFreeMem((*(pAppList + j))->rfu);
		}
        sdkFreeMem(*(pAppList + j));
    }

    sdkFreeMem(pAppList);
    return;
}


s32 sdkEmvContactUserSelectApp()
{
	u8 retCode;
	s32 rlt = SDK_ERR;

	if(NULL == gstsdkEmvTradeTable->DispCandAppList)
	{
		gstEmvTradeUnionStruct->EmvTradeParam->DispSeqListData.SelectedResult = RLT_EMV_ERR;
	}
	else
	{
		sdkEmvContactDealDispAppList();
	}

	retCode = EmvContact_UserSelectResult(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_UserSelectResult ret = %d\r\n",retCode);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactUserSelectApp ret = %d\r\n",rlt);

    return rlt;
}

void sdkEmvContactAidToTermInfo(unsigned char i, EMVBASE_AID_STRUCT * termaidparam, unsigned char onlinepinsup)
{
    emvbase_avl_createsettagvalue(EMVTAG_TermAID, (termaidparam + i)->Aid, (termaidparam + i)->AidLen);
    emvbase_avl_createsettagvalue(EMVTAG_AppVerNum, (termaidparam + i)->AppVerNum, 2);
    emvbase_avl_createsettagvalue(EMVTAG_TACDefault, (termaidparam + i)->TacDefault, 5);
    emvbase_avl_createsettagvalue(EMVTAG_TACOnline, (termaidparam + i)->TacOnline, 5);
	TraceHex("emv", "sdkEmvContactAidToTermInfo-TacOnline: ", (termaidparam + i)->TacOnline, 5);
    emvbase_avl_createsettagvalue(EMVTAG_TACDenial, (termaidparam + i)->TacDecline, 5);+
	emvbase_avl_createsettagvalue(EMVTAG_FloorLimit, (termaidparam + i)->FloorLimit, 4);
	emvbase_avl_createsettagvalue(EMVTAG_Threshold, (termaidparam + i)->Threshold, 4);
    emvbase_avl_settag(EMVTAG_MaxTargetPercent, (termaidparam + i)->MaxTargetPercent);
    emvbase_avl_settag(EMVTAG_TargetPercent, (termaidparam + i)->TargetPercent);
	Trace("emv", "sdkEmvContactAidToTermInfo-TermDDOLLen: %d\r\n", (termaidparam + i)->TermDDOLLen);
	TraceHex("emv", "sdkEmvContactAidToTermInfo-TermDDOL: ", (termaidparam + i)->TermDDOL, (termaidparam + i)->TermDDOLLen);
	emvbase_avl_createsettagvalue(EMVTAG_TermDDOL, (termaidparam + i)->TermDDOL, (termaidparam + i)->TermDDOLLen);
	emvbase_avl_createsettagvalue(EMVTAG_VLPTransLimit, (termaidparam + i)->ectranslimit, 6);

	if((termaidparam + i)->TermPinCap)
    {
		//not change
		Trace("emv","AID SUPPORT ONLINE PIN\r\n");
    }
    else
    {
    	Trace("emv","AID NOT SUPPORT ONLINE PIN\r\n");
        emvbase_avl_setvalue_and(EMVTAG_TermCapab, 1, 0xBF);
    }
}


s32 sdkEmvContactSetTACDefault(unsigned char *TACDefault)
{
    if(NULL == TACDefault || NULL == gstsdkEmvTradeTable)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_TACDefault, TACDefault, 5);
    return SDK_OK;

}

s32 sdkEmvContactSetTACOnline(unsigned char *TACOnline)
{
    if(NULL == TACOnline || NULL == gstsdkEmvTradeTable)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_TACOnline, TACOnline, 5);
    return SDK_OK;

}

s32 sdkEmvContactSetTACDenial(unsigned char *TACDenial)
{
    if(NULL == TACDenial || NULL == gstsdkEmvTradeTable)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_TACDenial, TACDenial, 5);
    return SDK_OK;

}

s32 sdkEmvContactSetThreshold(unsigned char *Threshold)
{
    if(NULL == Threshold || NULL == gstsdkEmvTradeTable)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_Threshold, Threshold, 4);
    return SDK_OK;

}

s32 sdkEmvContactSetMaxTargetPercent(unsigned char MaxTargetPercent)
{
    if( NULL == gstsdkEmvTradeTable)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_settag(EMVTAG_MaxTargetPercent, MaxTargetPercent);
    return SDK_OK;

}

s32 sdkEmvContactSetTargetPercent(unsigned char TargetPercent)
{
    if( NULL == gstsdkEmvTradeTable)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_settag(EMVTAG_TargetPercent, TargetPercent);
    return SDK_OK;

}

s32 sdkEmvContactSetTermDDOL(unsigned char *TermDDOL, unsigned char Len)
{
    if(NULL == TermDDOL || NULL == gstsdkEmvTradeTable || Len > 128)
    {
        return SDK_PARA_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_TermDDOL, TermDDOL, Len);
    return SDK_OK;

}



s32 sdkEmvContactReadAIDParameters(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    u8 i;
	s32 ret;
	u8 type = 0;

    EMVBASE_AID_STRUCT *tempaidparam;
    EMVBASE_LISTAPPDATA *tempappdata;
    unsigned char tempselectedappno;

    tempappdata = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;
    tempselectedappno = tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo;
    tempaidparam = (EMVBASE_AID_STRUCT *)emvbase_malloc(sizeof(EMVBASE_AID_STRUCT));

	type = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
    Trace("emv", "type=0x%x\n", type);

	ret = sdkEMVBaseReadAIDParameters((tempappdata + tempselectedappno)->AidInTerm,(tempappdata + tempselectedappno)->AidInTermLen,tempaidparam,type,2);
	Trace("emv", "sdkEmvContactReadAIDParameters-TermDDOLLen = %d\r\n", tempaidparam->TermDDOLLen);
	TraceHex("emv", "sdkEmvContactReadAIDParameters-TermDDOL: ", tempaidparam->TermDDOL, tempaidparam->TermDDOLLen);

	if(ret != SDK_OK)
	{
		emvbase_free(tempaidparam);
		return ret;
	}
	sdkEmvContactAidToTermInfo(0, tempaidparam, 0);

	emvbase_free(tempaidparam);

    if(emvbase_avl_checkiftagexist(EMVTAG_CardAID) == 0)
    {
        if((tempappdata + tempselectedappno)->AIDLen)
        {
            emvbase_avl_createsettagvalue(EMVTAG_CardAID, (tempappdata + tempselectedappno)->AID, (tempappdata + tempselectedappno)->AIDLen);
        }
    }

    if(emvbase_avl_checkiftagexist(EMVTAG_AppLabel) == 0)
    {
        if((tempappdata + tempselectedappno)->AppLabelLen)
        {
            emvbase_avl_createsettagvalue(EMVTAG_AppLabel, (tempappdata + tempselectedappno)->AppLabel, (tempappdata + tempselectedappno)->AppLabelLen);
        }
    }

    if(emvbase_avl_checkiftagexist(EMVTAG_AppPreferName) == 0)
    {
        if((tempappdata + tempselectedappno)->PreferNameLen)
        {
            emvbase_avl_createsettagvalue(EMVTAG_AppPreferName, (tempappdata + tempselectedappno)->PreferName, (tempappdata + tempselectedappno)->PreferNameLen);
        }
    }

    if(emvbase_avl_checkiftagexist(EMVTAG_LangPrefer) == 0)
    {
        if((tempappdata + tempselectedappno)->LangPreferLen)
        {
            emvbase_avl_createsettagvalue(EMVTAG_LangPrefer, (tempappdata + tempselectedappno)->LangPrefer, (tempappdata + tempselectedappno)->LangPreferLen);
        }
    }
    return SDK_OK;
}



s32 sdkEmvContactFinalSelectedApp()
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 AID[5];
    u8 TermCapab[3];

	retCode = EmvContact_FinalSelectedApp(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_FinalSelectedApp ret = %d\r\n",retCode);

	if(retCode == EMVCONTACT_READAIDPARAMETERS)
	{
		rlt = sdkEmvContactReadAIDParameters(gstEmvTradeUnionStruct);
		Trace("emv","sdkEmvContactReadAIDParameters ret = %d\r\n",rlt);
		retCode = RLT_EMV_OK;
		if(rlt != SDK_OK)
		{
			retCode = RLT_EMV_APPSELECTTRYAGAIN;
		}
	}

	if(retCode == RLT_EMV_OK)
	{
		#if 0
        memset(AID, 0, sizeof(AID));
        emvbase_avl_gettagvalue_spec(EMVTAG_CardAID, AID, 0, 5);
		memset(TermCapab, 0, sizeof(TermCapab));
        emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

		if(memcmp(AID, "\xA0\x00\x00\x03\x33", 5) == 0)		//pboc
        {
            TermCapab[1] &= 0xEF;
        }
        else												//emv
        {
            TermCapab[1] &= 0xFE;
        }
		emvbase_avl_createsettagvalue(EMVTAG_TermCapab, TermCapab, 3);
		TraceHex("emv", "9F33", TermCapab, 3);
		#endif
	}

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactFinalSelectedApp ret = %d\r\n",rlt);

    return rlt;
}


s32 sdkEmvContactGoToSelectApp()
{
    s32 rlt;

    rlt = sdkEmvContactOrganizeAppList();

	if(rlt == SDK_EMV_UserSelect)
    {
        rlt = sdkEmvContactUserSelectApp();
    }

    if(rlt != SDK_OK)
    {
        return rlt;
    }
    return sdkEmvContactFinalSelectedApp();
}

s32 sdkEmvContactReadDataStorage()
{
	u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;


	retCode = EmvContact_ReadDataStorage(gstEmvTradeUnionStruct);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
    return rlt;
}

s32 sdkEmvContactInitialApp()
{
    s32 rlt = SDK_ERR;
	u8 retCode;
	//u8 type=0;
	//u8 bcAmount[6] = {0};
	//u8 bcOtherAmount[6] = {0};

	if(gstsdkEmvTradeTable->setBeforeGPO != NULL)
	{
		(gstsdkEmvTradeTable->setBeforeGPO)();
	}

	//sdk不在维护交易金额和其他金�?由应用调用sdkemvbase的接�?
	#if 0
	//20190529
	type = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
	if (0x09 == type)//cashback
	{
		memset(bcAmount, 0, sizeof(bcAmount));
		emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum,bcAmount,0,6);

		memset(bcOtherAmount, 0, sizeof(bcOtherAmount));
		emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum,bcOtherAmount,0,6);

		sdkEmvContactImportOtherAmount(bcOtherAmount);

		sdkBcdAdd(bcAmount, bcAmount, 6, bcOtherAmount, 6);
		emvbase_avl_createsettagvalue(EMVTAG_AmtAuthNum, bcAmount, 6);
	}
	#endif

	if(gstsdkEmvTradeTable->bIsForceOnline)
    {
        emvbase_avl_settag(EMVTAG_bForceOnline, 0x01);
    }
    else
    {
        emvbase_avl_settag(EMVTAG_bForceOnline, 0x00);
    }



	//end

    retCode = EmvContact_InitialApp(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_InitialApp ret = %d\r\n",retCode);
//	gstemvbaseTransFlowModeStatus = SDKEMVBASE_TRANSFLOW_EMV_MODE;

	if(retCode == RLT_EMV_TRANSNOTACCEPT || retCode == RLT_ERR_EMV_NotAccept)
	{
		retCode =  RLT_EMV_APPSELECTTRYAGAIN;
	}
    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactInitialApp ret = %d\r\n",rlt);
    return rlt;
}





s32 sdkEmvContactGoToInitialApp()
{
    return sdkEmvContactInitialApp();
}


s32 sdkEmvContactReadAppData()
{
    u8 retCode;
    s32 rlt = SDK_ERR;
    u8 AmtAuthNum[6] = {0};
    u8 VLPAvailableFund[6] = {0};
    u8 CardTransPredicable;

    retCode = EmvContact_ReadAppData(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_ReadAppData ret = %d\r\n",retCode);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactReadAppData ret = %d\r\n",rlt);
    return rlt;

}

s32 sdkEmvContactReadCAPK(EMVCONTACTTradeUnionStruct * pstEmvContactTradeParam)
{
    EMVBASE_CAPK_STRUCT *tempcapk;
    EMVBASE_LISTAPPDATA *tempselectedapp;
    unsigned char i, RID[5];
    unsigned char CAPKI;
    unsigned char ret;

    tempcapk = pstEmvContactTradeParam->EmvTradeParam->CAPK;
    tempselectedapp = pstEmvContactTradeParam->EmvTradeParam->SelectedApp;

    tempcapk->ModulLen = 0;

    ret = emvbase_avl_gettagvalue_spec(EMVTAG_CAPKI, &CAPKI, 0, 1);

    if(ret != 0)
    {
        return -1;
    }

    if(tempcapk->ModulLen == 0)
    {
        memcpy(RID, (unsigned char*)tempselectedapp->AID, 5);

       sdkEMVBase_ReadCAPK(RID,CAPKI,tempcapk);
    }
	return SDK_OK;
}


s32 sdkEmvContactReadGMCAPK(EMVCONTACTTradeUnionStruct * pstEmvContactTradeParam)
{
    EMVBASE_CAPK_STRUCT *tempcapk;
    EMVBASE_LISTAPPDATA *tempselectedapp;
    unsigned char i, RID[5];
    unsigned char CAPKI;
    unsigned char ret;

    tempcapk = pstEmvContactTradeParam->EmvTradeParam->CAPK;
    tempselectedapp = pstEmvContactTradeParam->EmvTradeParam->SelectedApp;

    tempcapk->ModulLen = 0;

    ret = emvbase_avl_gettagvalue_spec(EMVTAG_CAPKI, &CAPKI, 0, 1);

    if(ret != 0)
    {
        return -1;
    }

    if(tempcapk->ModulLen == 0)
    {
		memcpy(RID, (unsigned char*)tempselectedapp->AID, 5);
		sdkEMVBase_ReadGMCAPK(RID,CAPKI,tempcapk);
    }
	return SDK_OK;
}


s32 sdkEmvContactDataAuth()
{
    u8 retCode = RLT_EMV_ERR, checkcapkexpire = true;
    s32 rlt = SDK_ERR;
	u8 tvr[5];

    if((gstEmvTradeUnionStruct == NULL) || (gstEmvTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstEmvTradeUnionStruct->EmvTradeParam->CAPK == NULL)
	{
		gstEmvTradeUnionStruct->EmvTradeParam->CAPK = (EMVBASE_CAPK_STRUCT *)emvbase_malloc(sizeof(EMVBASE_CAPK_STRUCT));
		memset(gstEmvTradeUnionStruct->EmvTradeParam->CAPK,0,sizeof(EMVBASE_CAPK_STRUCT));

    }

    if(gstEmvTradeUnionStruct->EmvTradeParam->ODASMstaus)
    {
        sdkEmvContactReadGMCAPK(gstEmvTradeUnionStruct);
		retCode = EmvContact_SM_DataAuth(gstEmvTradeUnionStruct);
		Trace("emv","EmvContact_SM_DataAuth ret = %d\r\n",retCode);
    }
    else
    {
        sdkEmvContactReadCAPK(gstEmvTradeUnionStruct);
		retCode = EmvContact_DataAuth(gstEmvTradeUnionStruct);
		Trace("emv","EmvContact_DataAuth ret = %d\r\n",retCode);
    }

	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, tvr, 0, 5);
	TraceHex("emv", "after DataAuth TVR", tvr,5);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactDataAuth ret = %d\r\n",rlt);
    return rlt;
}


s32 sdkEmvContactProcessRestrict()
{
	u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;
	u8 tvr[5];

	retCode = EmvContact_ProcessRestrict(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_ProcessRestrict ret = %d\r\n",retCode);

	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, tvr, 0, 5);
	TraceHex("emv", "after ProcessRestrict TVR", tvr,5);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactProcessRestrict ret = %d\r\n",rlt);
    return rlt;

}

s32 sdkEmvContactCardHolderVerf()
{
	u8 retCode = RLT_EMV_ERR;
	u8 iccEncryptWay, creditPwd[64];
	s32 rlt = SDK_ERR;
	u8 tvr[5];
	u8 PINTryCount;

	if(!gstEmvTradeUnionStruct->EmvTradeParam->ODASMstaus)
	{
		if(gstEmvTradeUnionStruct->EmvTradeParam->CAPK == NULL && gstEmvTradeUnionStruct->EmvTradeParam->IPKModul == NULL)
		{
			gstEmvTradeUnionStruct->EmvTradeParam->CAPK = (EMVBASE_CAPK_STRUCT *)emvbase_malloc(sizeof(EMVBASE_CAPK_STRUCT));
			memset(gstEmvTradeUnionStruct->EmvTradeParam->CAPK,0,sizeof(EMVBASE_CAPK_STRUCT));
			sdkEmvContactReadCAPK(gstEmvTradeUnionStruct);

        }
	}

	retCode = EmvContact_CardHolderVerf(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_CardHolderVerf ret = %d\r\n",retCode);

	if (gstEmvTradeUnionStruct->EmvTradeParam->bPrintReceipt)//20190531_lhd
	{
		gstemvbaseneedsign = 1;
	}

	if(gstEmvTradeUnionStruct->EmvTradeParam->CAPK)
	{
		emvbase_free(gstEmvTradeUnionStruct->EmvTradeParam->CAPK);
		gstEmvTradeUnionStruct->EmvTradeParam->CAPK = NULL;

}

	if(gstEmvTradeUnionStruct->EmvTradeParam->IPKModul)
	{
		emvbase_free(gstEmvTradeUnionStruct->EmvTradeParam->IPKModul);
		gstEmvTradeUnionStruct->EmvTradeParam->IPKModul = NULL;
	}

	if(gstEmvTradeUnionStruct->EmvTradeParam->ICCPINPKModul)
	{
		emvbase_free(gstEmvTradeUnionStruct->EmvTradeParam->ICCPINPKModul);
		gstEmvTradeUnionStruct->EmvTradeParam->ICCPINPKModul = NULL;
	}

	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, tvr, 0, 5);
	TraceHex("emv", "after CardHolderVerf TVR", tvr,5);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactCardHolderVerf ret = %d\r\n",rlt);
	return rlt;
}


s32 sdkEmvContactTermRiskManage()
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 AIP[2];
	u8 tvr[5];

    if(emvbase_avl_checkiftagexist(EMVTAG_VLPIssuAuthorCode))
    {
        return SDK_OK;
    }
    memset(AIP, 0, sizeof(AIP));

    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);

    //if(AIP[0] & 0x08)
    {
        sdkEmvContactCheckFloorLimit();

    }

    retCode = EmvContact_TermRiskManage(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_TermRiskManage ret = %d\r\n",retCode);

	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, tvr, 0, 5);
	TraceHex("emv", "after TermRiskManage TVR", tvr,5);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactTermRiskManage ret = %d\r\n",rlt);
	return rlt;
}


s32 sdkEmvContactTermiAnalys()
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;
	u8 tvr[5];
    u8 TSI[2] = {0};

    retCode = EmvContact_TermiAnalys(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_TermiAnalys ret = %d\r\n",retCode);

	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, tvr, 0, 5);
	TraceHex("emv", "after TermiAnalys TVR", tvr,5);

    emvbase_avl_gettagvalue_spec(EMVTAG_TSI, TSI, 0, 2);
	TraceHex("emv", "after TermiAnalys TSI", TSI,2);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactTermiAnalys ret = %d\r\n",rlt);
	return rlt;
}


s32 sdkEmvContactTransAnalys()
{
	u8 retCode = RLT_EMV_ERR;
	s32 rlt = SDK_ERR;
	u8 tvr[5];

	retCode = EmvContact_TransProcess(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_TransProcess ret = %d\r\n",retCode);

	if(gstEmvTradeParam->TransResult == RESULT_ONLINE_WAIT)
	{
		retCode = RLT_EMV_ONLINE_WAIT;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_OFFLINE_APPROVE)
	{
		Trace("emv","TransResult == RESULT_OFFLINE_APPROVE\r\n");
		EmvContact_WriteDataStorage(gstEmvTradeUnionStruct);
		retCode = RLT_EMV_OFFLINE_APPROVE;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_OFFLINE_DECLINE)
	{

		Trace("emv","TransResult == RESULT_OFFLINE_DECLINE\r\n");
		retCode = RLT_EMV_OFFLINE_DECLINE;
		EmvContact_WriteDataStorage(gstEmvTradeUnionStruct);

	}
	else if(gstEmvTradeParam->TransResult == RESULT_TERMINATE_NOTACCEPT)
	{
		retCode = RLT_ERR_EMV_NotAccept;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_TERMINATE)
	{
		retCode = RLT_EMV_TERMINATE_TRANSERR;
	}

	if(gstEmvTradeParam->Ec_LessThanResetAmount && (RESULT_ONLINE_WAIT == gstEmvTradeParam->TransResult))
	{
		//EmvContact_CVMOnlineEncPin(gstEmvTradeUnionStruct);
		unsigned char tempCreditPwd[20];
	    u8 retCode1 = RLT_EMV_ERR;

	    memset(tempCreditPwd, 0, sizeof(tempCreditPwd));
	    retCode1 = gstEmvTradeUnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_ONLINEPIN, tempCreditPwd);
		if(RLT_EMV_OK == retCode1)
	    {
	        emvbase_avl_createsettagvalue(EMVTAG_PIN, tempCreditPwd, 8);
	    }
	}

	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, tvr, 0, 5);
	TraceHex("emv", "after TransProcess TVR", tvr,5);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactTransAnalys ret = %d\r\n",rlt);
    return rlt;

}



s32 sdkEmvContactSelectNextInit()
{
	emvbase_avl_createtree();
	EMVBase_TransTermData_Init();
	sdkEmvContactDifferEMVBase();

}

s32 sdkEmvContactReSelectApp()
{
    u8 retCode;
    s32 rlt = SDK_ERR;

    retCode = EmvContact_RebuildApplist(gstEmvTradeUnionStruct);
	Trace("emv","EmvContact_RebuildApplist ret = %d\r\n",retCode);

    if(RLT_ERR_EMV_NoAppSel == retCode)
    {
        if(gstEmvTradeUnionStruct->EmvTradeParam->appblockflag)
        {
            return SDK_EMV_AppBlock;
        }
    }

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactReSelectApp ret = %d\r\n",rlt);

    return rlt;
}

SDK_EMVBASE_CVM_RESULT sdkContactGetCVMresult(void)
{
	u8 cvmresult[3]={0},method=0;
	SDK_EMVBASE_CVM_RESULT ret;

	emvbase_avl_gettagvalue_spec(EMVTAG_CVMResult,cvmresult,0,3);
	method = cvmresult[0] & 0x3F;

	switch (method)
	{
		case 0x01:
		ret = SDKEMVBASE_CVM_PLAINTEXTPIN;
		break;
		case 0x02:
		ret = SDKEMVBASE_CVM_ONLINEPIN;
		break;
		case 0x03:
		ret = SDKEMVBASE_CVM_PLAINTEXTPIN_AND_SIGN;
		break;
		case 0x04:
		ret = SDKEMVBASE_CVM_ENCIPHEREDPIN;
		break;
		case 0x05:
		ret = SDKEMVBASE_CVM_ENCIPHEREDPIN_AND_SIGN;
		break;
		case 0x1E: //signature (paper):
		ret = SDKEMVBASE_CVM_OBTAINSIGNATURE;
		break;
		case 0x1F: //no CVM required:
		ret = SDKEMVBASE_CVM_NOCVMREQ;
		break;
		case 0x20:
		ret = SDKEMVBASE_CVM_CONFVERIFIED;
		break;
		default:
		ret = SDKEMVBASE_CVM_NA;
		break;
	}

	return ret;
}

s32 sdkEmvContactTransFlow1()
{
    s32 ret;
	u8 selectnextflag = 0;
	u8 asPAN[20] = {0};

	if((gstsdkEmvTradeTable == NULL) || (gstEmvTradeParam == NULL) || (gstEmvTradeUnionStruct == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstEmvTradeUnionStruct->EmvTradeParam == NULL)
    {
        return SDK_PARA_ERR;
    }

	if(gstEMVBase_UnionStruct != NULL)
	{
		if(gstEMVBase_UnionStruct->rapdu != NULL)
		{
			emvbase_free(gstEMVBase_UnionStruct->rapdu);
			gstEMVBase_UnionStruct->rapdu = NULL;
		}
	}

	ret = sdkEmvContactDifferEMVBase();

	ret = sdkEmvContactGetAppCandidate();

	if(ret != SDK_OK)
	{
		return ret;
	}

	while(1)
    {
    	if(selectnextflag)
        {

            sdkEmvContactSelectNextInit();
        }
        ret = sdkEmvContactGoToSelectApp();
		selectnextflag = 1;

		if(SDK_EMV_NoAppSel == ret)
        {
			return SDK_EMV_NeedMagCard;
        }
		else if(ret == SDK_OK)
        {
        	if(gstEmvTradeParam->bDataStorageEnable)
			{
				Trace("emv", "Read Data Storage\r\n");
				ret = sdkEmvContactReadDataStorage();
			}

			ret = sdkEmvContactGoToInitialApp();
		}

		if(ret == SDK_EMV_AppSelectTryAgain)
        {
        	Trace("9f02 test2","comes to deal app\r\n");
        	ret = sdkEmvContactReSelectApp();
			if(ret != SDK_OK)
	        {
	            return ret;
	        }
			//need DispReselectApp
			if(NULL != gstsdkEmvTradeTable->DispReselectApp)
			{
				ret = (*gstsdkEmvTradeTable->DispReselectApp)();
                if(ret != SDK_OK)
                {
                //    return ret;
                }
			}
			continue;
		}
		else if(ret != SDK_OK)
        {
            return ret;
        }

		ret =sdkEmvContactReadAppData();
		if(ret != SDK_OK)
        {
            return ret;
        }

		if(gstsdkEmvTradeTable->DispCardNo)
		{
			//if(emvbase_avl_checkiftagexist(EMVTAG_PAN) || emvbase_avl_checkiftagexist(EMVTAG_Track2Equ))
			if(emvbase_avl_checkiftagexist(EMVTAG_PAN))
	        {
	            if(EmvContact_IccGetPAN(asPAN) == RLT_EMV_OK)
	            {
					ret = gstsdkEmvTradeTable->DispCardNo(asPAN);
					if(ret != SDK_OK)
					{
						return ret;
					}
            	}
			}
		}

		ret =sdkEmvContactDataAuth();
		if(ret != SDK_OK)
        {
            return ret;
        }
		ret =sdkEmvContactProcessRestrict();
		if(ret != SDK_OK)
        {
            return ret;
        }

		ret =sdkEmvContactCardHolderVerf();
		gstemvbaseCVMresult = sdkContactGetCVMresult();

		if(ret != SDK_OK)
        {
            return ret;
        }
		ret =sdkEmvContactTermRiskManage();
		if(ret != SDK_OK)
        {
            return ret;
        }
		ret =sdkEmvContactTermiAnalys();
		if(ret != SDK_OK)
        {
            return ret;
        }
		ret = sdkEmvContactTransAnalys();

		break;
    }
	Trace("emv","sdkEmvContactTransFlow1 return %d\r\n",ret);
	return ret;
}


s32 sdkEmvContactTransProcess()
{
	u8 retCode;
    s32 rlt = SDK_ERR;

	if(gstEmvTradeParam->onlinetradestatus == RLT_EMV_OK)
	{
		retCode = EmvContact_OnLineDeal(gstEmvTradeUnionStruct);
		Trace("emv","EmvContact_OnLineDeal ret = %d\r\n",retCode);
	}
	else
	{
		retCode = EmvContact_UnableGoOnlineDeal(gstEmvTradeUnionStruct);
		Trace("emv","EmvContact_UnableGoOnlineDeal ret = %d\r\n",retCode);
	}

	if(retCode == RLT_EMV_OK)
	{
		retCode = EmvContact_PreTransComplete(gstEmvTradeUnionStruct);
		Trace("emv","EmvContact_PreTransComplete ret = %d\r\n",retCode);
	}

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv","sdkEmvContactTransProcess ret = %d\r\n",rlt);
    return rlt;
}


s32 sdkEmvContactTransComplete(void)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

	if(gstEmvTradeParam->TransResult == RESULT_ONLINE_WAIT)
	{
		retCode = RLT_EMV_ONLINE_WAIT;
	}

	else if(gstEmvTradeParam->TransResult == RESULT_ONLINE_APPROVE)
	{
		retCode = RLT_EMV_ONLINE_APPROVE;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_ONLINE_DECLINE)
	{

		retCode = RLT_EMV_ONLINE_DECLINE;


	}
	else if(gstEmvTradeParam->TransResult == RESULT_OFFLINE_APPROVE)
	{
		retCode = RLT_EMV_OFFLINE_APPROVE;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_OFFLINE_DECLINE)
	{
		retCode = RLT_EMV_OFFLINE_DECLINE;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_TERMINATE_NOTACCEPT)
	{
		//retCode = RLT_EMV_TRANSNOTACCEPT;
		retCode = RLT_ERR_EMV_NotAccept;
	}
	else if(gstEmvTradeParam->TransResult == RESULT_TERMINATE)
	{
		retCode = RLT_EMV_TERMINATE_TRANSERR;
	}

   	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
    return rlt;
}


s32 sdkEmvContactTransFlow2()
{
	s32 ret;

	ret = sdkEmvContactTransProcess();
	if(ret != SDK_OK)
    {
        return ret;
    }
	ret = sdkEmvContactTransComplete();
	Trace("emv","sdkEmvContactTransFlow2 result %d\r\n",ret);
	return ret;
}


s32 sdkEmvContactSetRevocationKey(s32(*fun_cmprevocation)(u8 *key))
{
	if(gstEmvTradeUnionStruct == NULL)
	{
		return SDK_PARA_ERR;
	}

	gstEmvTradeUnionStruct->termipkrevokecheck = fun_cmprevocation;
	return SDK_OK;
}


s32 sdkEmvContactImportOnlineResult(s32 ucOnlineResult, u8 *pheRspCode)
{
	if(NULL == gstEmvTradeParam)
	{
		return SDK_ERR;
	}

    if(ucOnlineResult == SDK_OK)
    {
        gstEmvTradeParam->onlinetradestatus = RLT_EMV_OK;
		if(pheRspCode != NULL)
		{
			emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, pheRspCode, 2);
			TraceHex("emv","App Set AuthCode 8A:",pheRspCode,2);
		}
    }
    else
    {
        gstEmvTradeParam->onlinetradestatus = RLT_EMV_ERR;
    }

	Trace("emv","App Set Online Result:%d\r\n",gstEmvTradeParam->onlinetradestatus);
    return SDK_OK;
}


bool sdkEmvContactCheckFloorLimit()
{
    unsigned int amt = 0, i, MinLimit=0;
	unsigned char FloorLimit[4];
	unsigned char AmtAuthNum[6];
	unsigned char PAN[10] = {0};
    unsigned short PANLen=0;
	unsigned char PANSeq=0;
	int ret;

	memset(FloorLimit, 0, sizeof(FloorLimit));
	emvbase_avl_gettagvalue_spec(EMVTAG_FloorLimit,FloorLimit,0,4);
	memset(AmtAuthNum, 0, sizeof(AmtAuthNum));
	emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum,AmtAuthNum,0,6);

	sdkHexToU32(&MinLimit, FloorLimit,4);
	sdkBcdToU32(&amt, AmtAuthNum, 6);

	emvbase_avl_gettagvalue_spec(EMVTAG_PANSeq, &PANSeq, 0, 1);

	memset(PAN, 0, sizeof(PAN));
	emvbase_avl_gettagvalue_all(EMVTAG_PAN, PAN, &PANLen);
	Trace("emv","sdkEmvContactCheckFloorLimit amt = %d\r\n", amt);
	Trace("emv","sdkEmvContactCheckFloorLimit MinLimit = %d\r\n", MinLimit);

	if(amt >= MinLimit)//假如超过了就不需要再执行后面的循�?可能会有300�?
    {
		emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80); //set 'transaction exceeds floor limit' bit 1.
        return true;
    }

	if(gstsdkEmvTradeTable->GetCardTransSum != NULL)
	{
		ret = gstsdkEmvTradeTable->GetCardTransSum(&amt);
		if(ret != SDK_OK)
		{
			return false;
		}
	}

    if(amt >= MinLimit)
    {
		emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80); //set 'transaction exceeds floor limit' bit 1.
        return true;
    }
    return false;
}

s32 sdkEmvContactReadLogDetail(u8 ucLogNumIn, u8 *pheLogFormat, SDK_EMVBASE_LOGDATA *LogData, u8 *pheLogNumOut,u8 tradeMode)
{
    u8 retCode;
    u8 TransRecordSFI = 0, TransRecordNum = 0, aucDOL[100],  ucNowNum;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    u8 logtype = 0;

    u8 *pDOL = aucDOL;
    u8 ucTagLen;
    u16 recorddatalen = 0, LogLen = 0;

    if((ucLogNumIn > 10) || (ucLogNumIn == 0) || (pheLogFormat == NULL)  || (LogData == NULL)  || (pheLogNumOut == NULL) )
    {
        return SDK_PARA_ERR;
    }

	if (tradeMode != 0)
	{
		logtype =1;
	}

    retCode = sdkEMVBaseCountTransRecord(&TransRecordSFI, &TransRecordNum, logtype);

    if(retCode == SDK_EQU && gstemvbaseforL2TEST == 0)
    {
        TransRecordSFI = 0x0B;
        TransRecordNum = 0x0A;
    }
	else if(retCode == SDK_EQU && tradeMode!=0 && gstemvbaseforL2TEST == 0)//20190604_LHD
	{
		TransRecordSFI = 0x0C;
        TransRecordNum = 0x0A;
    }
    else if(retCode != SDK_OK)
    {
        return SDK_ERR;
    }

    if(TransRecordNum == 0)
    {
        return SDK_EMV_NoLog;
    }
    sdkEMVBasePostReadLogCommand(&apdu_s, logtype);

    gstEmvTradeUnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != APDUCOMMANDOK)
    {
        return SDK_ERR;
    }

    retCode = sdkEMVBaseDealICCParamRecord(&apdu_r, aucDOL, 100, logtype);

    if(retCode != SDK_OK)
    {
        return SDK_ERR;
    }

    if(ucLogNumIn > TransRecordNum)
    {
        ucLogNumIn = TransRecordNum;
    }
    ucNowNum = 1;
    (*pheLogNumOut) = 0;
    memset(pheLogFormat, 0, 100);
    memcpy(pheLogFormat, aucDOL, sizeof(aucDOL));

    while((*pheLogNumOut) < ucLogNumIn)
    {
        memset(&apdu_s, 0, sizeof(apdu_s));
        memset(&apdu_r, 0, sizeof(apdu_r));

        EMVBase_FormReadAflData((EMVBASE_APDU_SEND*)&apdu_s, ucNowNum, (u8)(TransRecordSFI << 3));

        gstEmvTradeUnionStruct->IsoCommand(&apdu_s, &apdu_r);

	    if(apdu_r.ReadCardDataOk != APDUCOMMANDOK)
	    {
	        return SDK_ERR;
	    }

        if(!(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00))
        {
            if((*pheLogNumOut) == 0)
            {
                return SDK_ERR;
            }
            else
            {
                return SDK_EMV_ReadLogEnd;
            }
        }
		#if 1//20190604_lhd for 圈存 会死循环
			if (tradeMode != 0)
			{
			//	Trace("","recorddatalen=%d,strlen(aucDOL)=%d",recorddatalen,strlen(aucDOL));
				while(recorddatalen < strlen(aucDOL))           //?????????
		        {
			//		Trace("","(*pDOL)=%02x",(*pDOL));
		            if(((*pDOL) & 0x1F) == 0x1F) { ucTagLen = 2; }
		            else{ ucTagLen = 1; }
		            LogLen += (*(pDOL + ucTagLen));
		            recorddatalen += ucTagLen + 1;
		            pDOL += ucTagLen + 1;

			//		Trace("","recorddatalen=%d LogLen=%d",recorddatalen ,LogLen);

		        }
			//	Trace("","LogLen + 14=%d,apdu_r.LenOut=%d",LogLen + 14,apdu_r.LenOut);
		        if(LogLen + 14 != apdu_r.LenOut) { return SDK_EMV_ReadLogEnd; }
			}
		#endif

		LogData[*pheLogNumOut].LenOut = apdu_r.LenOut;
		memcpy(LogData[*pheLogNumOut].DataOut,apdu_r.DataOut,apdu_r.LenOut);
		LogData[*pheLogNumOut].SW1 = apdu_r.SW1;
        LogData[*pheLogNumOut].SW2 = apdu_r.SW2;
        (*pheLogNumOut)++;
        ucNowNum++;
    }

    return SDK_EMV_ReadLogEnd;
}

 s32 sdkEmvContactReadLog(u8 *LogFormat,SDK_EMVBASE_LOGDATA *LogData,u8 *LogNum ,u8 MaxNum,u8 tradeMode)
{
	s32 ret;
	u8 selectnextflag = 0;

	if((gstsdkEmvTradeTable == NULL) || (gstEmvTradeParam == NULL) || (gstEmvTradeUnionStruct == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstEmvTradeUnionStruct->EmvTradeParam == NULL)
    {
        return SDK_PARA_ERR;
    }

	if(gstEMVBase_UnionStruct != NULL)
	{
		if(gstEMVBase_UnionStruct->rapdu != NULL)
		{
			emvbase_free(gstEMVBase_UnionStruct->rapdu);
			gstEMVBase_UnionStruct->rapdu = NULL;
		}
	}
	gstEmvTradeParam->ReadLogFlag = 1;//20190604_lhd

	ret = sdkEmvContactGetAppCandidate();

	if(ret != SDK_OK)
	{
		return ret;
	}

	while(1)
    {
    	if(selectnextflag)
        {
            sdkEmvContactSelectNextInit();
        }
        ret = sdkEmvContactGoToSelectApp();
		selectnextflag = 1;

		if(SDK_EMV_NoAppSel == ret)
        {
			return SDK_EMV_NeedMagCard;
        }
		else if(ret == SDK_OK)
        {
			return sdkEmvContactReadLogDetail(MaxNum,LogFormat,LogData,LogNum,tradeMode);

}

		if(ret == SDK_EMV_AppSelectTryAgain)
        {
        	ret = sdkEmvContactReSelectApp();
			if(ret != SDK_OK)
	        {
	            return ret;
	        }
			continue;
		}
		break;
    }
	return ret;
}

s32 sdkEmvContactTransReadAppData()
{
    s32 ret;
	u8 selectnextflag = 0;
	u8 asPAN[20] = {0};

	if((gstsdkEmvTradeTable == NULL) || (gstEmvTradeParam == NULL) || (gstEmvTradeUnionStruct == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstEmvTradeUnionStruct->EmvTradeParam == NULL)
    {
        return SDK_PARA_ERR;
    }

	if(gstEMVBase_UnionStruct != NULL)
	{
		if(gstEMVBase_UnionStruct->rapdu != NULL)
		{
			emvbase_free(gstEMVBase_UnionStruct->rapdu);
			gstEMVBase_UnionStruct->rapdu = NULL;
		}
	}

	sdkEmvContactDifferEMVBase();

	ret = sdkEmvContactGetAppCandidate();

	if(ret != SDK_OK)
	{
		return ret;
	}

	while(1)
    {
    	if(selectnextflag)
        {
            sdkEmvContactSelectNextInit();
        }
        ret = sdkEmvContactGoToSelectApp();
		selectnextflag = 1;

		if(SDK_EMV_NoAppSel == ret)
        {
			return SDK_EMV_NeedMagCard;
        }
		else if(ret == SDK_OK)
        {
			ret = sdkEmvContactGoToInitialApp();

}

		if(ret == SDK_EMV_AppSelectTryAgain)
        {
        	ret = sdkEmvContactReSelectApp();
			if(ret != SDK_OK)
	        {
	            return ret;
	        }
			//need DispReselectApp
			if(NULL != gstsdkEmvTradeTable->DispReselectApp)
			{
				ret = (*gstsdkEmvTradeTable->DispReselectApp)();
                if(ret != SDK_OK)
                {
                //    return ret;
                }
			}
			continue;
		}
		else if(ret != SDK_OK)
        {
            return ret;
        }

		ret =sdkEmvContactReadAppData();
		if(ret != SDK_OK)
        {
            return ret;
        }

		if(gstsdkEmvTradeTable->DispCardNo)
		{
			//if(emvbase_avl_checkiftagexist(EMVTAG_PAN) || emvbase_avl_checkiftagexist(EMVTAG_Track2Equ))
			if(emvbase_avl_checkiftagexist(EMVTAG_PAN))
	        {
	            if(EmvContact_IccGetPAN(asPAN) == RLT_EMV_OK)
	            {
					ret = gstsdkEmvTradeTable->DispCardNo(asPAN);
					if(ret != SDK_OK)
					{
						return ret;
					}
            	}
			}
		}

		return SDK_EMV_TransReadAppDataComplete;

    }
}

s32 sdkEmvContactReadEcBalance(void)
{
	s32 ret;
	u8 selectnextflag = 0;
	unsigned char retCode;

	if((gstsdkEmvTradeTable == NULL) || (gstEmvTradeParam == NULL) || (gstEmvTradeUnionStruct == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstEmvTradeUnionStruct->EmvTradeParam == NULL)
    {
        return SDK_PARA_ERR;
    }

	if(gstEMVBase_UnionStruct != NULL)
	{
		if(gstEMVBase_UnionStruct->rapdu != NULL)
		{
			emvbase_free(gstEMVBase_UnionStruct->rapdu);
			gstEMVBase_UnionStruct->rapdu = NULL;
		}
	}

	ret = sdkEmvContactGetAppCandidate();
	if(ret != SDK_OK)
	{
		return ret;
	}

	while(1)
    {
    	if(selectnextflag)
        {
            sdkEmvContactSelectNextInit();
        }
        ret = sdkEmvContactGoToSelectApp();
		selectnextflag = 1;

		if(SDK_EMV_NoAppSel == ret)
        {
			return SDK_EMV_NeedMagCard;
        }
		else if(ret == SDK_OK)
        {
			retCode = EmvContact_ReadEcData(gstEmvTradeUnionStruct);
			sdkEMVBaseRltToSdkRlt(retCode, &ret);
			return ret;
		}

		if(ret == SDK_EMV_AppSelectTryAgain)
        {
        	ret = sdkEmvContactReSelectApp();
			if(ret != SDK_OK)
	        {
	            return ret;
	        }
			continue;
		}
		break;
    }
	return ret;
}

s32 sdkEmvContactTransForceAAC(void)
{
	s32 ret;
	u8 selectnextflag = 0;
	u8 asPAN[20] = {0};

	if((gstsdkEmvTradeTable == NULL) || (gstEmvTradeParam == NULL) || (gstEmvTradeUnionStruct == NULL))
	{
	    return SDK_PARA_ERR;
	}

	if(gstEmvTradeUnionStruct->EmvTradeParam == NULL)
	{
	    return SDK_PARA_ERR;
	}

	if(gstEMVBase_UnionStruct != NULL)
	{
	if(gstEMVBase_UnionStruct->rapdu != NULL)
	{
	  emvbase_free(gstEMVBase_UnionStruct->rapdu);
	  gstEMVBase_UnionStruct->rapdu = NULL;
	}
	}

	sdkEmvContactDifferEMVBase();

	ret = sdkEmvContactGetAppCandidate();

	if(ret != SDK_OK)
	{
		return ret;
	}

	while(1)
	{
		if(selectnextflag)
		{
		    sdkEmvContactSelectNextInit();
		}
	    ret = sdkEmvContactGoToSelectApp();
		selectnextflag = 1;

		if(SDK_EMV_NoAppSel == ret)
	    {
			return SDK_EMV_NeedMagCard;
	    }
		else if(ret == SDK_OK)
		{
			ret = sdkEmvContactGoToInitialApp();
		}

		if(ret == SDK_EMV_AppSelectTryAgain)
		{
			ret = sdkEmvContactReSelectApp();
			if(ret != SDK_OK)
			{
				return ret;
			}
		  //need DispReselectApp
			if(NULL != gstsdkEmvTradeTable->DispReselectApp)
			{
				ret = (*gstsdkEmvTradeTable->DispReselectApp)();
			    if(ret != SDK_OK)
			    {
			    //    return ret;
			    }
			}
			continue;
		}
		else if(ret != SDK_OK)
	    {
	        return ret;
	    }

		ret =sdkEmvContactReadAppData();
		if(ret != SDK_OK)
	    {
	        return ret;
	    }

		if(gstsdkEmvTradeTable->DispCardNo)
		{
		  //if(emvbase_avl_checkiftagexist(EMVTAG_PAN) || emvbase_avl_checkiftagexist(EMVTAG_Track2Equ))
			if(emvbase_avl_checkiftagexist(EMVTAG_PAN))
			{
				if(EmvContact_IccGetPAN(asPAN) == RLT_EMV_OK)
				{
					ret = gstsdkEmvTradeTable->DispCardNo(asPAN);
					if(ret != SDK_OK)
					{
						return ret;
					}
				}
			}
		}

		gstEmvTradeUnionStruct->EmvTradeParam->bRequestAAC = 1;
		ret =sdkEmvContactTermiAnalys();
		if(ret != SDK_OK)
		{
		    return ret;
		}

		return SDK_EMV_TransReadAppDataComplete;
	}
}

//2021.7.16 lishiyao sdk灞傝幏鍙杁ll搴撶増鏈?
s32 sdkEmvContactGetLibVersion(u8 *version)
{
	if(version == NULL)
	{
		return SDK_PARA_ERR;
	}

	EmvContact_GetLibVersion(version);
	return SDK_OK;
}


s32 sdkEmvContactGetKernelVersion(s32 machine_code, u8 *version)
{
	if(version == NULL)
	{
		return SDK_PARA_ERR;
	}

	EmvContact_GetKernelVerInfo(machine_code,version);
	return SDK_OK;
}
