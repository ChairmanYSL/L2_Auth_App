#include "dllemvbase.h"
#include "dllpure.h"
#include "sdkemvbaseprivate.h"
#include "dllpureprivate.h"
#include "sdkpureprivate.h"
#include "sdkDebug.h"
#include "sdktypedef.h"
#include "sdkped.h"
#include "sdkoutcome.h"
#include "emv_type.h"

PURETRADEPARAMETER *gstPureTradeParam = NULL;
PURETradeUnionStruct *gstPureTradeUnionStruct = NULL;
unsigned char *gstPureMemSlotUpdateTemp = NULL;
unsigned char *gstPureMemSlotReadTemp = NULL;
unsigned char *gstPureGDDOLResBuf = NULL;

u8 gPureTransStuatus = 0;

typedef struct
{
    bool bIsForceOnline;    // force online for qpboc
    bool bIsForcePIN;       //force need online pin for china,overseas not need force online pin

    s32 (*ReadCard)();
    s32 (*DispRemoveCard)(void);        //display remove card
    s32 (*DispCardExpire)(void);        //dispplay card expire
    s32 (*DispOnlineODA)(u8 Authtype, bool result);     //dispplay online oda result
    s32 (*setBeforeGPO)(void);      //set something before GPO
    s32 (*VerifyCardNo)(u8* asPAN);//Exception File Check
    s32 (*PromptSoundAndLed)(int mode);//100ms ��ʾ����/��
} SDK_PURE_TRADE_PARAM;

SDK_PURE_TRADE_PARAM *gstsdkPureTradeTable;

s32 sdkPureSetOption(u8 option)
{
	if(gstPureTradeParam == NULL)
	{
		return SDK_PARA_ERR;
	}

	gstPureTradeParam->PureImplementationOption = option;
	return SDK_OK;
}

s32 sdkPureSetForceOnline(bool bIsForceOnline)
{
	if(gstsdkPureTradeTable == NULL)
	{
		return SDK_PARA_ERR;
	}

	gstsdkPureTradeTable->bIsForceOnline = bIsForceOnline;
	if(bIsForceOnline)
	{
		emvbase_avl_settag(EMVTAG_bForceOnline, 0x01);
		emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x08);
	}
	else
	{
		emvbase_avl_settag(EMVTAG_bForceOnline, 0x00);
		emvbase_avl_setvalue_and(EMVTAG_TVR, 3, 0xF7);
	}
	return SDK_OK;
}

int BeforeGPORes = SDK_ERR;
int InputPINRes = -10;
int VerifyCardNoRes = -2;
int VerifyRevocationKeyRes = -2;
int SecondTapCardRes = SDK_ERR;
int TransAmtSumRes = SDK_ERR;

void sdkPureSetBeforeGPORes(int res)
{
	BeforeGPORes = res;
}

int sdkPureSetInputPINRes(int res, unsigned char *pinBuf, int bufLen)
{
	s32 ret;

	if(pinBuf == NULL || bufLen > 256 || bufLen < 0)
	{
		return SDK_PARA_ERR;
	}

	InputPINRes = res;
	ret = sdkEMVBaseConfigTLV(EMVTAG_PIN, pinBuf, bufLen);
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		return ret;
	}
}

void sdkPureSetVerifyCardNoRes(int res)
{
	VerifyCardNoRes = res;
}

void sdkPureSetVerifyRevocationKeyRes(int res)
{
	VerifyRevocationKeyRes = res;
}

void sdkPureSetSecondTapCardRes(int res)
{
	SecondTapCardRes = res;
}

void sdkPureSetTransAmtSumRes(int res)
{
	TransAmtSumRes = res;
}

s32 sdkPureGetBeforeGPORes()
{
	//FIXME
	BeforeGPORes = 0;
	if(BeforeGPORes == 0)
	{
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}
}

u8 sdkPureGetInputPINRes()
{
	//FIXME
//	InputPINRes = 0;
	if(InputPINRes == 0)
	{
		return RLT_EMV_OK;
	}
	else if(InputPINRes == -1)
	{
		return RLT_ERR_EMV_CancelTrans;
	}
	else if(InputPINRes == -5)
	{
		return RLT_ERR_EMV_InputBYPASS;
	}
	else
	{
		return RLT_EMV_ERR;
	}
}

s32 sdkPureGetVerifyCardNoRes()
{
	//FIXME
	return RLT_EMV_OK;
	return VerifyCardNoRes;
}

s32 sdkPureGetVerifyRevocationKeyRes()
{
	//FIXME
	return RLT_EMV_OK;
	return VerifyRevocationKeyRes;
}

s32 sdkPureGetSecondTapCardRes()
{
	//FIXME
	return SDK_EQU;
	return SecondTapCardRes;
}

s32 sdkPureGetTransAmtSumRes()
{
	//FIXME
	return SDK_EQU;
	return TransAmtSumRes;
}

s32 sdkPureSetImplementation(u8 Implementation)
{
	if(gstPureTradeParam == NULL || gstPureTradeUnionStruct == NULL)
	{
		return EMV_UNINITIALIZED;
	}

	gstPureTradeParam->PureImplementationOption = Implementation;
	return SDK_OK;
}

s32 sdkPureSetMTOL(u8      *MTOL, s32 len)
{
	if(gstPureTradeParam == NULL || gstPureTradeUnionStruct == NULL)
	{
		return EMV_UNINITIALIZED;
	}
	if(len < 50 || len > 256 || NULL == MTOL)
	{
		return SDK_PARA_ERR;
	}

	return sdkEMVBaseConfigTLV("\xDF\x83\x09", MTOL, len);
}

s32 sdkPureSetTermAIDSupport(u8 *TermAIDSup, s32 len)
{
	if(gstPureTradeParam == NULL || gstPureTradeUnionStruct == NULL)
	{
		return EMV_UNINITIALIZED;
	}
	else if(TermAIDSup == NULL || len < 0 || len >16)
	{
		return SDK_PARA_ERR;
	}

	return sdkEMVBaseConfigTLV("\xDF\x70", TermAIDSup, len);
}

s32 sdkPureGetCTPreProcessIndicator(u8 *indicator)
{
	if(gstPureTradeParam == NULL || gstPureTradeUnionStruct == NULL)
	{
		return EMV_UNINITIALIZED;
	}
	else if(indicator == NULL)
	{
		return SDK_PARA_ERR;
	}

	*indicator = gstPureTradeParam->PreprcessIndicator;
	return SDK_OK;
}

s32 sdkPureSetMemorySlotUpdateTemplate(u8 *Template, u16 len)
{
	u8 Templatelen[2]={0};

	if(Template == NULL || len < 800 || len > 65535)
	{
		return SDK_PARA_ERR;
	}

	if(Template[0] != 0xA2 || Template[0] != 0xA3 || Template[0] != 0xA4)
	{
		return SDK_PARA_ERR;
	}

	if(gstPureMemSlotUpdateTemp != NULL)	//free mem first
	{
		if(sizeof(gstPureMemSlotUpdateTemp) != len)
		{
			sdkPureFreeMemorySlotUpdateTemplate();
		}
		else
		{
			memcpy(gstPureMemSlotUpdateTemp, Template, len);
			return SDK_OK;
		}
	}

	gstPureMemSlotUpdateTemp = (unsigned char *)sdkGetMem(len);
	memset(gstPureMemSlotUpdateTemp, 0, len);
	memcpy(gstPureMemSlotUpdateTemp, Template, len);

	sdkU16ToHex(Templatelen, len, 2);

	sdkEMVBaseConfigTLV("\xBF\x70", Templatelen, 2);
	return SDK_OK;
}

void sdkPureFreeMemorySlotUpdateTemplate()
{
	if(gstPureMemSlotUpdateTemp != NULL)	//free mem first
	{
		sdkFreeMem(gstPureMemSlotUpdateTemp);
		gstPureMemSlotUpdateTemp = NULL;
	}
	sdkEMVBaseDelTLV("\xBF\x70");
}

s32 sdkPureSetMemorySlotReadTemplate(u8 *Template, u16 len)
{
	u8 Templatelen[2]={0};

	if(Template == NULL || len < 800 || len > 65535)
	{
		return SDK_PARA_ERR;
	}

	if(memcmp(Template, "\x9F\x70", 2))
	{
		return SDK_PARA_ERR;
	}

	if(gstPureMemSlotReadTemp != NULL)	//free mem first
	{
		if(sizeof(gstPureMemSlotReadTemp) != len)
		{
			sdkPureFreeMemorySlotUpdateTemplate();
		}
		else
		{
			memcpy(gstPureMemSlotReadTemp, Template, len);
			return SDK_OK;
		}
	}

	gstPureMemSlotReadTemp = (unsigned char *)sdkGetMem(len);
	memset(gstPureMemSlotReadTemp, 0, len);
	memcpy(gstPureMemSlotReadTemp, Template, len);

	sdkU16ToHex(Templatelen, len, 2);

	sdkEMVBaseConfigTLV("\xBF\x71", Templatelen, 2);
	return SDK_OK;
}

void sdkPureFreeMemorySlotReadTemplate()
{
	if(gstPureMemSlotReadTemp != NULL)	//free mem first
	{
		sdkFreeMem(gstPureMemSlotReadTemp);
		gstPureMemSlotReadTemp = NULL;
	}
	sdkEMVBaseDelTLV("\xBF\x71");
}

s32 sdkPureGetGDDOLResBuf(unsigned char *buf, int len)
{
	if(NULL == buf || len < 512)
	{
		return SDK_PARA_ERR;
	}

	if(NULL != gstPureGDDOLResBuf)
	{
		if(sizeof(gstPureGDDOLResBuf) > len)
		{
			return SDK_EBUSY;	//mem size open not enough
		}
		else
		{
			memcpy(buf, gstPureGDDOLResBuf, sizeof(gstPureGDDOLResBuf));
			return	SDK_OK;
		}
	}
	else
	{
		return SDK_FUN_NULL;	//kernel didnt init 9F71
	}
}

void sdkPureAidToTermInfo(unsigned char i, SDK_EMVBASE_AID_STRUCT * termaidparam, unsigned char onlinepinsup)
{
	unsigned int Threshold;
	unsigned char PureThreshold[6];

    emvbase_avl_createsettagvalue(EMVTAG_TermAID, (termaidparam + i)->Aid, (termaidparam + i)->AidLen);
    emvbase_avl_createsettagvalue(EMVTAG_AppVerNum, (termaidparam + i)->AppVerNum, 2);

	TraceHex("emv","TacDefault",(termaidparam+i)->TacDefault,5);
	if(memcmp((termaidparam+i)->TacDefault, "\xFF\xFF\xFF\xFF\xFF", 5) == 0)
	{
		emvbase_avl_deletetag(EMVTAG_TACDefault);//not configured
	}
	else
	{
	    emvbase_avl_createsettagvalue(EMVTAG_TACDefault, (termaidparam + i)->TacDefault, 5);
	}

	TraceHex("emv","TacOnline",(termaidparam+i)->TacOnline,5);
	if(memcmp((termaidparam+i)->TacOnline, "\xFF\xFF\xFF\xFF\xFF", 5) == 0)
	{
		emvbase_avl_deletetag(EMVTAG_TACOnline);//not configured
	}
	else
	{
	    emvbase_avl_createsettagvalue(EMVTAG_TACOnline, (termaidparam + i)->TacOnline, 5);
	}

	TraceHex("emv","TacDecline",(termaidparam+i)->TacDecline,5);
	if(memcmp((termaidparam+i)->TacDecline, "\xFF\xFF\xFF\xFF\xFF", 5) == 0)
	{
		emvbase_avl_deletetag(EMVTAG_TACDenial);//not configured
	}
	else
	{
	    emvbase_avl_createsettagvalue(EMVTAG_TACDenial, (termaidparam + i)->TacDecline, 5);
	}

	TraceHex("emv","cl_cvmlimit",(termaidparam+i)->cl_cvmlimit,6);
	if(memcmp((termaidparam+i)->cl_cvmlimit, "\xFF\xFF\xFF\xFF\xFF\xFF", 6) == 0)
	{
		emvbase_avl_deletetag(EMVTAG_termcvm_limit);//not configured
	}
	else
	{
	    emvbase_avl_createsettagvalue(EMVTAG_termcvm_limit, (termaidparam + i)->cl_cvmlimit, 6);
	}

	TraceHex("emv","cl_translimit",(termaidparam+i)->cl_translimit,6);
	if(memcmp((termaidparam+i)->cl_translimit, "\xFF\xFF\xFF\xFF\xFF\xFF", 6) == 0)
	{
		emvbase_avl_deletetag(EMVTAG_clessofflineamt);//not configured
	}
	else
	{
	    emvbase_avl_createsettagvalue(EMVTAG_clessofflineamt, (termaidparam + i)->cl_translimit, 6);
	}

	TraceHex("emv","cl_offlinelimit",(termaidparam+i)->cl_offlinelimit,6);
	if(memcmp((termaidparam+i)->cl_offlinelimit, "\xFF\xFF\xFF\xFF\xFF\xFF", 6) == 0)
	{
		emvbase_avl_deletetag(EMVTAG_clessofflinelimitamt);//not configured
	}
	else
	{
	    emvbase_avl_createsettagvalue(EMVTAG_clessofflinelimitamt, (termaidparam + i)->cl_offlinelimit, 6);
	}

    emvbase_avl_settag(EMVTAG_MaxTargetPercent, (termaidparam + i)->MaxTargetPercent);
    emvbase_avl_settag(EMVTAG_TargetPercent, (termaidparam + i)->TargetPercent);
    emvbase_avl_createsettagvalue(EMVTAG_Threshold, (termaidparam + i)->Threshold, 4);
	if(0x00 == (termaidparam + i)->TermDDOL[0])
	{
		emvbase_avl_deletetag(EMVTAG_TermDDOL);//not configured
	}
	else
	{
		emvbase_avl_createsettagvalue(EMVTAG_TermDDOL, (termaidparam + i)->TermDDOL, (termaidparam + i)->TermDDOLLen);
	}
	TraceHex("emv","Default DDOL", (termaidparam+i)->TermDDOL, (termaidparam + i)->TermDDOLLen);

}


s32 sdkPureReadAIDParameters(PURETradeUnionStruct * temp_UnionStruct)
{
    u8 i;
    s32 ret;
    u8 type = 0;

    EMVBASE_AID_STRUCT *tempaidparam;
    EMVBASE_LISTAPPDATA *tempappdata;
    unsigned char tempselectedappno;

    tempappdata = temp_UnionStruct->EMVTradeParam->AppListCandidate;
    tempselectedappno = temp_UnionStruct->EMVTradeParam->SelectedAppNo;
    tempaidparam = (EMVBASE_AID_STRUCT *)emvbase_malloc(sizeof(EMVBASE_AID_STRUCT));

	type = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
    Trace("emv", "type=0x%x\n", type);

    ret = sdkEMVBaseReadAIDParameters((tempappdata + tempselectedappno)->AidInTerm,(tempappdata + tempselectedappno)->AidInTermLen,tempaidparam,type,2);

	Trace("emv","sdkEMVBaseReadAIDParameters = %d\r\n",ret);

    if(ret != SDK_OK)
    {
        emvbase_free(tempaidparam);
        return ret;
    }
    sdkPureAidToTermInfo(0, tempaidparam, 0);

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


static void sdkPureTradeParamDestory(void)
{
    if(gstPureTradeParam != NULL)
    {
        if(gstPureTradeParam->CAPK != NULL)
        {
            emvbase_free(gstPureTradeParam->CAPK);
            gstPureTradeParam->CAPK = NULL;
        }

        if(gstPureTradeParam->AuthData != NULL)
        {
            emvbase_free(gstPureTradeParam->AuthData);
            gstPureTradeParam->AuthData = NULL;
        }
        if(gstPureTradeParam->IPKModul != NULL)
        {
            emvbase_free(gstPureTradeParam->IPKModul);
            gstPureTradeParam->IPKModul = NULL;
        }
        if(gstPureTradeParam->ICCPKModul != NULL)
        {
            emvbase_free(gstPureTradeParam->ICCPKModul);
            gstPureTradeParam->ICCPKModul = NULL;
        }
        if(gstPureTradeParam->RedundantData != NULL)
        {
            emvbase_free(gstPureTradeParam->RedundantData);
            gstPureTradeParam->RedundantData = NULL;
        }

        emvbase_free(gstPureTradeParam);
        gstPureTradeParam = NULL;
    }

    if(gstPureTradeUnionStruct != NULL)
    {
        emvbase_free(gstPureTradeUnionStruct);
        gstPureTradeUnionStruct = NULL;
    }

    if(gstsdkPureTradeTable != NULL)
    {
        emvbase_free(gstsdkPureTradeTable);
        gstsdkPureTradeTable = NULL;
    }

}

void sdkPureMalloctradememory()
{
    s32 aidnum=0;

	sdkPureTradeParamDestory();

    if(gstPureTradeUnionStruct == NULL)
    {
        gstPureTradeUnionStruct = (PURETradeUnionStruct *)emvbase_malloc(sizeof(PURETradeUnionStruct));
    }
    memset(gstPureTradeUnionStruct,0,(sizeof(PURETradeUnionStruct)));

	if(gstsdkPureTradeTable == NULL)
	{
		gstsdkPureTradeTable = (SDK_PURE_TRADE_PARAM *)emvbase_malloc(sizeof(SDK_PURE_TRADE_PARAM));
	}
	memset(gstsdkPureTradeTable,0,(sizeof(SDK_PURE_TRADE_PARAM)));


    if(gstPureTradeParam == NULL)
    {
        gstPureTradeParam = (PURETRADEPARAMETER *)emvbase_malloc(sizeof(PURETRADEPARAMETER));
    }
    memset(gstPureTradeParam,0,sizeof(PURETRADEPARAMETER));

    sdkEMVBaseGetAIDListNum(&aidnum);

}


unsigned char sdkPureFunCB(unsigned char pintype, unsigned char *ASCCreditPwd)
{
    unsigned char rslt;
    unsigned char tCreditPwd[64], tICCEncryptWay;
    s32 rlt;
    u8 PINTryCount = 0;

    Trace("emv", "pintype=%d\r\n", pintype);
    if(gstEMVBase_TradeParam->InputPWD == NULL)
    {
        Trace("emv","ERROR!!!USER NOT SET INPUTPIN FUNCTION\r\n");
        return RLT_EMV_ERR;
    }

    if(pintype == EMVB_DATAAUTH_INPUTPINSTATUS)
    {
        return RLT_EMV_OK;
    }
    else if(pintype == EMVB_DATAAUTH_VERIFYIDCARD)
    {
        return RLT_EMV_OK;
    }
    else if(pintype == EMVB_NEEDREFER)
    {
        return RLT_EMV_OK;
    }
    memset(tCreditPwd, 0, sizeof(tCreditPwd));

    if(pintype == EMVB_DATAAUTH_OFFLINEENCPIN || pintype == EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN)
    {
        tICCEncryptWay = SDK_PED_IC_OFFLINE_PIN;
    }
    else
    {
        tICCEncryptWay = SDK_PED_IC_ONLINE_PIN;
    }

    rlt = gstEMVBase_TradeParam->InputPWD(tICCEncryptWay, PINTryCount, tCreditPwd);
    Trace("emv","InputPWD=%d\r\n",rlt);


	Trace("emv", "input pin len = %d\r\n", tCreditPwd[0]);
	TraceHex("emv", "input pin", &tCreditPwd[1], tCreditPwd[0]);

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

void sdkPureGetRandom(unsigned char* randpad, unsigned int randLen)
{
    sdkGetRandom(randpad, randLen);
}

static void sdkPureIccIsoCommand(EMVBASE_APDU_SEND * ApduSend, EMVBASE_APDU_RESP * apdu_r)
{
    sdkEMVBase_ContactlessIsoCommand(ApduSend, apdu_r);
}

static unsigned char sdkPureVerifyCardNo(unsigned char *asPAN)
{
	s32 ret = -3;
	if(gstsdkPureTradeTable->VerifyCardNo)
	{
		ret = gstsdkPureTradeTable->VerifyCardNo(asPAN);
		Trace("emv", "VerifyCardNo ret = %d\r\n", ret);
		if(ret != SDK_OK)
		{
			return RLT_EMV_ERR;
		}
	}

	return RLT_EMV_OK;
}

static unsigned char sdkPureCheckCAPK(PURETRADEPARAMETER *EMVTradeParam)
{
    EMVBASE_LISTAPPDATA *tempselectedapp;
    unsigned char i, RID[5];
    unsigned char CAPKI;
    unsigned char ret;

    Trace("emv", "sdkPureCheckCAPK\r\n");
    tempselectedapp = EMVTradeParam->SelectedApp;
	memcpy(RID, (unsigned char*)tempselectedapp->AID, 5);

    ret = emvbase_avl_gettagvalue_spec(EMVTAG_CAPKI, &CAPKI, 0, 1);

    if(ret != 0)
    {
        return RLT_EMV_ERR;
    }

	if( sdkEMVBaseCheckCAPKExit(RID,CAPKI))
	{
		return RLT_EMV_OK;
	}

    return RLT_EMV_ERR;
}

void sdkPureTransTermDataInit(void)
{
	gstPureTradeParam->bAbleOnline=1;//initial value: 1
    gstPureTradeParam->SelectedAppNo = 0xFF;
	gstPureTradeParam->PureImplementationOption = 0xFF;//default support all function
	gstPureTradeParam->PureCandidateListEmptyFlag = 0;
	gstPureTradeParam->CurProcessIndicator = 0x00;	//init to new trans
	gstPureTradeParam->Error = 0;
	gstPureTradeParam->EchoTransIndicator = 0;
	gstPureTradeParam->CommuProblemIndicator = 0;
	gstPureTradeParam->OfflineCAMSelectedIndicator = 0;
	gstPureTradeParam->ECHOCommandSupport = 1;
	gstPureTradeParam->bForceOnline = 0;
	gstPureTradeParam->bForceAAC = 0;
	gstPureTradeParam->CCIDExistInGPO = 0;

    gstPureTradeUnionStruct->EMVTradeParam = gstPureTradeParam;
    gstPureTradeUnionStruct->IsoCommand = sdkPureIccIsoCommand;
    gstPureTradeUnionStruct->EMVB_InputCreditPwd = sdkPureFunCB;
    gstPureTradeUnionStruct->EMVB_RandomNum = sdkPureGetRandom;
    gstPureTradeUnionStruct->CheckMatchTermAID = sdkEMVBase_CheckMatchTermAID_CL;
    gstPureTradeUnionStruct->ReadTermAID = sdkEMVBase_ReadTermAID_CL;
	gstPureTradeUnionStruct->VerifyCardNo = sdkPureVerifyCardNo;
//	gstPureTradeUnionStruct->CheckCapkExit = sdkPureCheckCAPK;
	gstPureTradeUnionStruct->GetInputPINRes = sdkPureGetInputPINRes;
	gstPureTradeUnionStruct->GetVerifyCardNoRes = sdkPureGetVerifyCardNoRes;
	gstPureTradeUnionStruct->GetVerifyRevocationKeyRes = sdkPureGetVerifyRevocationKeyRes;
	gstPureTradeUnionStruct->SetOutcome = sdkSetOutcomeParam;
	gstPureTradeUnionStruct->SetUIRequest = sdkSetUIRequestParam;
	gstPureTradeUnionStruct->SendOutcome = sdkSendOutcome;
	gstPureTradeUnionStruct->SendUIRequest = sdkSendUIRequest;
	gstPureTradeUnionStruct->GetTransAmtSumRes = sdkPureGetTransAmtSumRes;
}


s32 sdkPureTransInit()
{
	gPureTransStuatus = 0;
    sdkPureTradeParamDestory();
	sdkPureMalloctradememory();
    sdkPureTransTermDataInit();

	return SDK_OK;
}

s32 sdkPureFinalSelectedApp(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    retCode = pure_FinalSelectedApp(gstPureTradeUnionStruct);
    Trace("emv", "pure_FinalSelectedApp retCode = %02d\r\n", retCode);
	if(retCode == PURE_ERR_READCARDAGAIN)
	{
		return SDK_EMV_ReadCardAgain;
	}
    else if(retCode == PURE_READAIDPARAMETERS)
    {
        rlt = sdkPureReadAIDParameters(gstPureTradeUnionStruct);

        if(rlt != SDK_OK)
        {
            retCode = RLT_EMV_APPSELECTTRYAGAIN;
        }
        else
        {
			rlt = sdkPurePreprocess(pstTradeParam);
			if(rlt == SDK_EMV_ReadCardAgain || rlt == SDK_EMV_ENDAPPLICATION)
			{
				return rlt;
			}
			if(gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator & 0x08)	//section 8.2.3.8
			{
				gstPureTradeUnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYANOTHERINTERFACE, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_CONTACTCHIP, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
				gstPureTradeUnionStruct->SendOutcome;
				gstPureTradeUnionStruct->SetUIRequest(SDK_UI_MSGID_ERROR_SWITCHINTERFACE, SDK_UI_STATUS_READYTOREAD, 0, NULL, 0, NULL, NULL);
				gstPureTradeUnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
				retCode =  RLT_ERR_EMV_SWITCHINTERFACE;
			}
			else
			{
				retCode = RLT_EMV_OK;
			}

        }
    }

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureFinalSelectedApp ret = %02d\r\n", rlt);

    return rlt;
}

//section 9.2.2
s32 sdkPureTransactionInit(PURETradeUnionStruct *tempApp_UnionStruct)
{
	s32 ret, len;
	u8 retCode;
	u8 Implement = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	u8 TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	u8 ComProbIndicator = tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator;
	u8 CLPreProcessIndicator = tempApp_UnionStruct->EMVTradeParam->PreprcessIndicator;
	u8 KerCap[5]={0}, TTPI[5]={0};

	//section 9.2.2.3
	if(tempApp_UnionStruct->EMVTradeParam->Error == 0)
	{
		if(sdkEMVBaseCheckTagExit("\x8A"))
		{
			if(!sdkEMVBaseCheckTagExit("\x9F\x02") || !sdkEMVBaseCheckTagExit("\x5F\x2A") || !sdkEMVBaseCheckTagExit("\x9F\x37"))
			{
				tempApp_UnionStruct->EMVTradeParam->Error = 2;
			}
		}
		else
		{
			if(!sdkEMVBaseCheckTagExit("\x9C"))
			{
				tempApp_UnionStruct->EMVTradeParam->Error = 1;
			}
			else
			{
				if((Implement & 0x80) && (TransType == PURE_TRANS_RETRIEVE_GETDATA))
				{
					if(!sdkEMVBaseCheckTagExit("\x6F") || (gstPureMemSlotReadTemp == NULL))
					{
						tempApp_UnionStruct->EMVTradeParam->Error = 1;
					}
				}
				else if((Implement & 0x40) && (TransType == PURE_TRANS_UPDATE_PUTDATA))
				{
					if(!sdkEMVBaseCheckTagExit("\x6F") || (gstPureMemSlotUpdateTemp == NULL))
					{
						tempApp_UnionStruct->EMVTradeParam->Error = 1;
					}
				}
				else if((Implement & 0x10) && (TransType == PURE_TRANS_APPAUTHTRANS))
				{
					if(!sdkEMVBaseCheckTagExit("\x6F") || !sdkEMVBaseCheckTagExit("\x9F\x37"))
					{
						tempApp_UnionStruct->EMVTradeParam->Error = 1;
					}
				}
				else
				{
					if(!sdkEMVBaseCheckTagExit("\x9F\x02") || !sdkEMVBaseCheckTagExit("\x5F\x2A") || !sdkEMVBaseCheckTagExit("\x9F\x37") || !sdkEMVBaseCheckTagExit("\x9F\x1A") || !sdkEMVBaseCheckTagExit("\x9A"))
					{
						tempApp_UnionStruct->EMVTradeParam->Error = 2;
					}
				}
			}
		}
	}

	//section 9.2.2.4
	if((Implement & 0x40))
	{
		sdkEMVBaseConfigTLV("\x9F\x74", "\x00\x00", 2);
	}
	if((Implement & 0x80))
	{
		if(NULL == gstPureGDDOLResBuf)
		{
			gstPureGDDOLResBuf = (u8 *)sdkGetMem(512);
		}
		memset(gstPureGDDOLResBuf, 0, 512);
	}

	if(sdkEMVBaseCheckTagExit("\x9F\x75"))
	{
		if((Implement) && (ComProbIndicator != 0) && (Implement & 0x10) && (TransType != PURE_TRANS_APPAUTHTRANS))
		{
			if((sdkEMVBaseCheckTagExit("\x8A") && (ComProbIndicator == 2)) || (!sdkEMVBaseCheckTagExit("\x8A") && (ComProbIndicator == 1)))
			{
				retCode = pure_TronECHOProcess(tempApp_UnionStruct);
				if(retCode == PURE_ERR_READCARDAGAIN)
				{
					return SDK_EMV_ReadCardAgain;
				}
				else if(retCode == RLT_ERR_EMV_IccReturn)
				{

				}
				else
				{
					//FIXME:just personal think,when retcode = RLT_EMV_OK
					tempApp_UnionStruct->EMVTradeParam->CurProcessIndicator = TRANS_TORN_RECOVERGAC;
				}
			}
		}
	}

	sdkEMVBaseReadTLV("\xDF\x83\x08", KerCap, &len);
	if(!sdkEMVBaseCheckTagExit("\x8A") && (tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator == 0))
	{
		TTPI[0] = 0x3E;
		if(CLPreProcessIndicator & 0x10)
		{
			TTPI[1] |= 0x80;
		}
		if(CLPreProcessIndicator & 0x20)
		{
			TTPI[1] |= 0x40;
		}
		if(CLPreProcessIndicator & 0x80)
		{
			TTPI[1] |= 0x20;
		}
		if(CLPreProcessIndicator & 0x40)
		{
			TTPI[1] |= 0x10;
		}
		if(KerCap[3] & 0x01)
		{
			TTPI[1] |= 0x04;
		}
		if(KerCap[2] & 0x80)
		{
			TTPI[2] |= 0x80;
		}
		if(KerCap[2] & 0x40)
		{
			TTPI[2] |= 0x40;
		}
		if(KerCap[2] & 0x20)
		{
			TTPI[2] |= 0x20;
		}
		TTPI[4] = KerCap[4];

		sdkEMVBaseConfigTLV("\xC7", TTPI, 5);
		tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0;
		sdkEMVBaseConfigTLV("\x95", "\x80\x00\x00\x00\x00", 5);
		tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator = 0;

		if(sdkEMVBaseCheckTagExit("\x9F\x75") && (TTPI[2] & 0x20))
		{
			tempApp_UnionStruct->EMVTradeParam->ECHOCommandSupport = 1;
		}
		else
		{
			tempApp_UnionStruct->EMVTradeParam->ECHOCommandSupport = 0;
		}

		if(CLPreProcessIndicator & 0x10)
		{
			sdkEMVBaseConfigTLV("\x95", "\x80\x00\x00\x80\x00", 5);
		}
	}

	if(((!sdkEMVBaseCheckTagExit("\x91")) && (!sdkEMVBaseCheckTagExit("\x71")) && (!sdkEMVBaseCheckTagExit("\x72"))) || ((KerCap[2] & 0x10) == 0))
	{
		tempApp_UnionStruct->EMVTradeParam->Error = 1;
	}

	//section 9.2.2.5
	if(tempApp_UnionStruct->EMVTradeParam->Error == 0 && sdkEMVBaseCheckTagExit("\x8A"))
	{
		tempApp_UnionStruct->EMVTradeParam->CurProcessIndicator = TRANS_ONLINE_RESPONSE;
		if(((KerCap[2]&0x80) == 0) && ((KerCap[2]&0x10) == 0))
		{
			tempApp_UnionStruct->EMVTradeParam->Error = 2;
		}
		else
		{
			if(0 == tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator)
			{
				if(tempApp_UnionStruct->EMVTradeParam->FCIDifferFlag)
				{
					tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator = 3;
				}
				else
				{
					tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator = 0;
				}
			}
			return sdkPureOnlineRespProcess(tempApp_UnionStruct);
		}
	}
	else
	{
		if(tempApp_UnionStruct->EMVTradeParam->Error == 0)
		{
			if(tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator == 1)
			{
				retCode = pure_DealTornEchoResponse(gstPureTradeUnionStruct);
				sdkEMVBaseRltToSdkRlt(retCode, &ret);
				Trace("emv", "pure_DealTornEchoResponse ret = %02d\r\n", ret);
				return ret;
			}
			else
			{
				if(((Implement & 0x10) == 0) || (TransType != PURE_TRANS_APPAUTHTRANS))
				{
					//The Dynamic Transactions parameters (‘9F02’, ‘5F2A’, ‘9F1A’, ‘9A’,’9C’ and ‘9F37’) are stored in ‘Kernel Transaction Database
				}

				if(Implement & 0x80)
				{
					ret = sdkPureGetDataBeforeGPO(gstsdkPureTradeTable);
					if(ret == SDK_EMV_TransTerminate)
					{
						return ret;
					}
				}

				if(Implement & 0x40)
				{
					ret = sdkPurePutDataBeforeGPO(gstsdkPureTradeTable);
					if(ret != SDK_OK)
					{
						return ret;
					}
				}
				else
				{
					return SDK_OK;	//go to section 9.2.6,deal GPO
				}

			}
		}
	}

	//section 9.2.2.6
	if(tempApp_UnionStruct->EMVTradeParam->Error == 1)
	{
		sdkSetOutcomeParam(SDK_OUTCOME_RESULT_SELECTNEXT, SDK_OUTCOME_START_C, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
		sdkSendOutcome();
		return SDK_EMV_AppSelectTryAgain;
	}

	if(tempApp_UnionStruct->EMVTradeParam->Error == 2)
	{
		if(sdkEMVBaseCheckTagExit("\x71") || sdkEMVBaseCheckTagExit("\x72"))
		{
			sdkSetOutcomeParam(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 1, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
		}
		else
		{
			sdkSetOutcomeParam(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
		}
		sdkSendOutcome();
		return SDK_EMV_TransTerminate;
	}

	return SDK_OK;
}

s32 sdkPureInitialApp(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	rlt = sdkPureTransactionInit(gstPureTradeUnionStruct);
	if(gstPureTradeParam->EchoTransIndicator == 1)
	{
		return rlt;
	}
	if(SDK_OK != rlt)
	{
		return rlt;
	}

    retCode = pure_InitialApp(gstPureTradeUnionStruct);
    Trace("emv", "SDK-info: pure_InitialApp retCode = %02d\r\n", retCode);

    if(retCode == RLT_ERR_EMV_APDUTIMEOUT)
    {
        return SDK_EMV_ReadCardAgain;
    }
    else if(retCode == RLT_ERR_INITAPP_NOTACCEPED)
    {
        retCode = RLT_EMV_APPSELECTTRYAGAIN;
    }

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureInitialApp ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureReadAppData(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR, k;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    retCode = pure_ReadAppData(gstPureTradeUnionStruct);
	Trace("emv", "pure_ReadAppData retCode = %02d\r\n", retCode);

	if(retCode == RLT_ERR_EMV_APDUTIMEOUT)
	{
	    return SDK_EMV_ReadCardAgain;
    }

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureReadAppData ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureTermRiskManage(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
	u8 retCode = RLT_EMV_ERR, k;
	s32 rlt = SDK_ERR;

	if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
	{
		return SDK_PARA_ERR;
	}

	retCode = pure_TermRiskManage(gstPureTradeUnionStruct);
	Trace("emv", "pure_TermRiskManage retCode = %02d\r\n", retCode);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureTermRiskManage ret = %02d\r\n", rlt);
	return rlt;
}

s32 sdkPureCAMSelect(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(PURE_TRANS_APPAUTHTRANS != gstPureTradeUnionStruct->EMVTradeParam->CurTransType)
	{
		return SDK_OK;
	}

	retCode = pure_ODACheckMandotoryData(gstPureTradeUnionStruct);
	Trace("emv", "pure_ODACheckMandotoryData retCode = %02d\r\n", retCode);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureProcessRestrict ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureProcessRestrict(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR, k;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    retCode = pure_ProcessRestrict(gstPureTradeUnionStruct);
	Trace("emv", "pure_ProcessRestrict retCode = %02d\r\n", retCode);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureProcessRestrict ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureTermiAnalys(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
	u8 retCode = RLT_EMV_ERR, k;
	s32 rlt = SDK_ERR;

	if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
	{
		return SDK_PARA_ERR;
	}

	retCode = pure_TermiAnalys(gstPureTradeUnionStruct);
	Trace("emv", "pure_TermiAnalys retCode = %02d\r\n", retCode);

	retCode = pure_TransResultAnalysis(gstPureTradeUnionStruct);
	Trace("emv", "pure_TransProcess retCode = %02d\r\n", retCode);

	sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureTermiAnalys ret = %02d\r\n", rlt);
	return rlt;
}


s32 sdkPureReadCAPK(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    EMVBASE_CAPK_STRUCT *tempcapk;
    EMVBASE_LISTAPPDATA *tempselectedapp;
    unsigned char i, RID[5];
    unsigned char CAPKI;
    unsigned char ret;


	if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    Trace("emv", "sdkPureReadCAPK\r\n");
	if(gstPureTradeUnionStruct->EMVTradeParam->CAPK == NULL)
    {
        gstPureTradeUnionStruct->EMVTradeParam->CAPK = (EMVBASE_CAPK_STRUCT *)emvbase_malloc(sizeof(EMVBASE_CAPK_STRUCT));
        memset(gstPureTradeUnionStruct->EMVTradeParam->CAPK, 0, sizeof(EMVBASE_CAPK_STRUCT));
    }

    tempcapk = gstPureTradeUnionStruct->EMVTradeParam->CAPK;
    tempselectedapp = gstPureTradeUnionStruct->EMVTradeParam->SelectedApp;



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


s32 sdkPureDataAuth(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    retCode = pure_DataAuth(gstPureTradeUnionStruct);

    Trace("emv", "pure_DataAuth retCode = %02d\r\n", retCode);
    emvbase_avl_printtagallvalue("after DataAuth TVR",EMVTAG_TVR);
    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureDataAuth ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureKernelDeactivate(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    retCode = pure_TransProcess(gstPureTradeUnionStruct);

    Trace("emv", "pure_DataAuth retCode = %02d\r\n", retCode);
    emvbase_avl_printtagallvalue("after DataAuth TVR",EMVTAG_TVR);
    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureDataAuth ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureCardHolderVerf(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR, k;
    s32 rlt = SDK_ERR;
	unsigned char IUParameter;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

    retCode = pure_CardHolderVerf(gstPureTradeUnionStruct);
    Trace("emv", "pure_CardHolderVerf retCode = %02d\r\n", retCode);
	if(retCode == PURE_REQ_INPUTPIN)
	{
		return EMV_REQ_ONLINE_PIN;
	}

    emvbase_avl_printtagallvalue("Emv_CardHolderVerf TTQ", &EMVTAG_TermTransPredicable);
	if(gstPureTradeUnionStruct->EMVTradeParam->bPrintReceipt)
	{
		gstemvbaseneedsign = 1;
	}
    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureCardHolderVerf ret = %02d\r\n", rlt);
    return rlt;
}

//section 9.2.3 Online Response Processing and Kernel De-Activation
s32 sdkPureOnlineRespProcess(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	retCode = pure_OnlineRespProcess(gstPureTradeUnionStruct);
    Trace("emv", "pure_OnlineRespProcess retCode = %02d\r\n", retCode);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureOnlineRespProcess ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureDifferEMVBase()
{
	emvbase_avl_settagtype(EMVTAGTYPE_PURE);

	emvbase_avl_deletetag(EMVTAG_TSI);	//Pure not support TSI
	emvbase_avl_createsettagvalue(EMVTAG_PURETTPI, "\x3E\x02\xE0\x00\xF9", 5);
	emvbase_avl_createsettagvalue(EMVTAG_PUREKernelCap, "\x36\x00\xF0\x4B\xF9", 5);
	emvbase_avl_createsettagvalue(EMVTAG_TVR, "\x00\x00\x00\x00\x00", 5);
	emvbase_avl_deletetag(EMVTAG_PUREMTOL);
}

SDK_EMVBASE_CVM_RESULT sdkPureGetCVMresult()
{
	SDK_EMVBASE_CVM_RESULT ret=0;


	switch (gstPureTradeUnionStruct->EMVTradeParam->PureCVMParameter)
	{
	   case SDK_OUTCOME_CVM_NOCVMREQ:
			ret = SDKEMVBASE_CVM_NOCVMREQ;
	   break;
	   case SDK_OUTCOME_CVM_OBTAINSIGNATURE:
		   ret = SDKEMVBASE_CVM_OBTAINSIGNATURE;
	  break;
	   case SDK_OUTCOME_CVM_ONLINEPIN:
			ret = SDKEMVBASE_CVM_ONLINEPIN;
	   break;
	   case SDK_OUTCOME_CVM_CONFVERIFIED:
		   ret = SDKEMVBASE_CVM_CONFVERIFIED;
	  break;

	   default:
			ret = SDKEMVBASE_CVM_NA;
	   break;
	}

	return ret;
}

void sdkPureSetCandidateListEmptyFlag(bool flag)
{
	gstPureTradeUnionStruct->EMVTradeParam->PureCandidateListEmptyFlag = flag;
}

bool sdkPureNeedIssuerUpdate()
{
	if(gstPureTradeParam)
	{
		Trace("","gstPureTradeParam->secondtap = %d\r\n",gstPureTradeParam->secondtap);
	}
	if(gstPureTradeParam && gstPureTradeParam->secondtap)
	{
		return true;
	}
	else
	{
		return false;
	}
}

s32 sdkPureImportOnlineResult(s32 ucOnlineResult, const u8 *pheRspCode)
{
	if(NULL == gstPureTradeParam)
	{
		return SDK_ERR;
	}
    if(ucOnlineResult == SDK_OK)
    {
        gstPureTradeParam->onlinetradestatus = RLT_EMV_OK;
        if(pheRspCode != NULL)
        {
            emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, pheRspCode, 2);
            TraceHex("emv", "App Set AuthCode 8A:", pheRspCode, 2);
        }
    }
    else
    {
        gstPureTradeParam->onlinetradestatus = RLT_EMV_ERR;
    }

    Trace("emv", "App Set Online Result:%d,ucOnlineResult=%d\r\n", gstPureTradeParam->onlinetradestatus, ucOnlineResult);
    return SDK_OK;
}

s32 sdkPureGetDataBeforeGPO(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstPureGDDOLResBuf == NULL)
	{
		gstPureGDDOLResBuf = (unsigned char *)sdkGetMem(512);
		memset(gstPureGDDOLResBuf, 0, 512);
	}

	retCode = pure_RetrieveDataInE2PROM(gstPureTradeUnionStruct, gstPureMemSlotReadTemp, gstPureGDDOLResBuf);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureGetDataBeforeGPO ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPurePutDataBeforeGPO(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstPureMemSlotUpdateTemp == NULL || gstPureMemSlotUpdateTemp[0] == 0xA4)	//if user dont set template or template format is A4(only allow in PUT DATA after GAC),ignore it and consider it as success
	{
		return SDK_OK;
	}
	retCode = pure_UpdateDataInE2PROM(gstPureTradeUnionStruct, gstPureMemSlotUpdateTemp, 0xA2, 1);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPurePutDataBeforeGPO ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureGetDataAfterGPO(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;


    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

//	retCode = pure_RetrieveDataInE2PROM(gstPureTradeUnionStruct);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureGetDataAfterGPO ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPurePutDataAfterGPO(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstPureMemSlotUpdateTemp == NULL || gstPureMemSlotUpdateTemp[0] == 0xA4)	//if user dont set template or template format is A4(only allow in PUT DATA after GAC),ignore it and consider it as success
	{
		return SDK_OK;
	}
	retCode = pure_UpdateDataInE2PROM(gstPureTradeUnionStruct, gstPureMemSlotUpdateTemp, 0xA3, 0);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPurePutDataAfterGPO ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPurePutDataAfterGAC(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;


    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstPureMemSlotUpdateTemp == NULL)	//if user dont set template or template format is A4(only allow in PUT DATA after GAC),ignore it and consider it as success
	{
		return SDK_OK;
	}
	retCode = pure_UpdateDataInE2PROM(gstPureTradeUnionStruct, gstPureMemSlotUpdateTemp, 0xA4);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPurePutDataAfterGAC ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureReadBalanceAfterGAC(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;


    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	retCode = pure_RetrieveCardBalance(gstPureTradeUnionStruct);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureCardHolderVerf ret = %02d\r\n", rlt);
    return rlt;
}

s32 sdkPureUpdateDataAfterGAC(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
    u8 retCode = RLT_EMV_ERR;
    s32 rlt = SDK_ERR;


    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	if(gstPureMemSlotUpdateTemp == NULL || gstPureMemSlotUpdateTemp[0] == 0xA4)	//if user dont set template or template format is A4(only allow in PUT DATA after GAC),ignore it and consider it as success
	{
		return SDK_OK;
	}
	retCode = pure_UpdateDataInE2PROM(gstPureTradeUnionStruct, gstPureMemSlotUpdateTemp, 0xA3);

    sdkEMVBaseRltToSdkRlt(retCode, &rlt);
	Trace("emv", "sdkPureCardHolderVerf ret = %02d\r\n", rlt);
    return rlt;
}

void sdkPureParseTrantype(u8 transtype)
{
	switch(transtype)
	{
		case 0x00:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_GOODS;
			break;
		case 0x01:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_CASH;
			break;
		case 0x09:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_CASHBACK;
			break;
		case 0x20:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_REFUND;
			break;
		case 0x78:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_RETRIEVE_GETDATA;
			break;
		case 0x79:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_UPDATE_PUTDATA;
			break;
		default:
			gstPureTradeUnionStruct->EMVTradeParam->CurTransType = PURE_TRANS_APPAUTHTRANS;
			break;
	}
}

s32 sdkPurePreprocess(const SDK_PURE_TRADE_PARAM * pstTradeParam)
{
	u8 TransType,StatusCheckFlag;
	s32 len;
	u8 Amount[6],CLTransLmt[6], CVMLmt[6],CLFloorLmt[6],TermFloorLmt[4];
	u32 Amountnmb=0,floorlmtnmb=0;

    if((NULL == pstTradeParam) || (gstPureTradeUnionStruct == NULL) || (gstPureTradeParam == NULL))
    {
        return SDK_PARA_ERR;
    }

	//section 8.2.3 Preliminary Transaction Processing
	gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator = 0x00;	//section 8.2.3.1

	if(sdkEMVBaseReadTLV("\x9C", &TransType, &len) == SDK_OK)
	{
		sdkPureParseTrantype(TransType);		//section 8.2.3.2
		if((gstPureTradeUnionStruct->EMVTradeParam->PureImplementationOption & 0x10) && (gstPureTradeUnionStruct->EMVTradeParam->CurTransType == PURE_TRANS_APPAUTHTRANS))
		{
			return SDK_EMV_ReadCardAgain;
		}
		//section 8.2.3.3
		if(sdkEMVBaseReadTLV("\x1F\x02", &StatusCheckFlag, &len) == SDK_OK)
		{
			gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator &= 0x80;
		}
		//section 8.2.3.4
		if(sdkEMVBaseReadTLV("\x9F\x02", Amount, &len) == SDK_OK)
		{
			if((gstPureTradeUnionStruct->EMVTradeParam->CurTransType == PURE_TRANS_GOODS) && (!memcmp(Amount, "\x00\x00\x00\x00\x00\x00", 6)))
			{
				gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator &= 0x40;
			}
		}
		else
		{
			EMVBase_Trace("PURE-error:", "missing 9F02\r\n");
			return SDK_EMV_TransTerminate;
		}
		//section 8.2.3.5 ,check contactless trans limit
		if((sdkEMVBaseReadTLV("\xDF\x81\x24", CLTransLmt, &len) == SDK_OK) && sdkEMVBaseCheckTagExit("\x5F\x2A"))
		{
			if(memcmp(Amount, CLTransLmt, 6) >= 0)
			{
				gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator &= 0x08;
			}
		}
		//section 8.2.3.6,check contactless floor limit
		if((sdkEMVBaseReadTLV("\xDF\x81\x23", CLFloorLmt, &len) == SDK_OK) && sdkEMVBaseCheckTagExit("\x5F\x2A"))
		{
			if(memcmp(Amount, CLFloorLmt, 6) >= 0)
			{
				gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator &= 0x10;
			}
		}
		else if((sdkEMVBaseReadTLV("\x9F\x1B", TermFloorLmt, &len) == SDK_OK) && sdkEMVBaseCheckTagExit("\x5F\x2A"))
		{
			sdkHexToU32(&floorlmtnmb, TermFloorLmt, 4);
			sdkBcdToU32(&Amountnmb, Amount, 6);
			if(Amountnmb >= floorlmtnmb)
			{
				gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator &= 0x10;
			}
		}
		//section 8.2.3.7,check contactless cvm limit
		if((sdkEMVBaseReadTLV("\xDF\x81\x26", CVMLmt, &len) == SDK_OK) && sdkEMVBaseCheckTagExit("\x5F\x2A"))
		{
			if(memcmp(Amount, CVMLmt, 6) >= 0)
			{
				gstPureTradeUnionStruct->EMVTradeParam->PreprcessIndicator &= 0x20;
			}
		}

	}
	else
	{
		EMVBase_Trace("PURE-error:", "missing 9C\r\n");
		return SDK_EMV_TransTerminate;
	}

	//section 8.2.4 Fixed-amount transaction Processing
	//this part of specification is bullshit,ignore it


    return SDK_OK;
}

s32 sdkPureGetLibVersion(u8 *version)
{
	if(version == NULL)
	{
		return SDK_PARA_ERR;
	}

	pure_GetLibVersion(version);
	return SDK_OK;
}

s32 sdkPureTransFlow()
{
    s32 ret;
    SDK_EMVBASE_CL_HIGHESTAID tempHighestAID;
	static u8 callbackFlag = 0;

	if(gPureTransStuatus == SDK_PURE_STATUS_PPSE)
	{
		ret = sdkEMVBaseEntryPoint(&tempHighestAID);
		if(SDK_OK == ret)
		{
			#if 1
			gPureTransStuatus = SDK_PURE_STATUS_DIFFEREMVBASE;
			#else
			gPureTransStuatus = SDK_PURE_STATUS_PREPROCESS
			#endif
			return EMV_STA_CANDIDATES_BUILT;
		}
		else
		{
			return EMV_GETTERMAPPSERR;
		}
	}

	switch(gPureTransStuatus)
	{
		case SDK_PURE_STATUS_DIFFEREMVBASE:
			if((gstEMVBase_UnionStruct == NULL) || (gstEMVBase_EntryPoint == NULL) || (gstPureTradeParam == NULL)
					|| (gstsdkPureTradeTable == NULL))
			{
				return EMV_UNINITIALIZED;
			}

			if(gstEMVBase_UnionStruct->rapdu)
			{
				emvbase_free(gstEMVBase_UnionStruct->rapdu);
				gstEMVBase_UnionStruct->rapdu = NULL;
			}

			gstPureTradeParam->SelectedApp = gstEMVBase_EntryPoint->SelectedApp;
			gstPureTradeParam->SelectedAppNo = gstEMVBase_UnionStruct->SelectedAppNo;
			gstPureTradeParam->AppListCandidate = gstEMVBase_EntryPoint->AppListCandidate;
			gstPureTradeParam->AppListCandidatenum = gstEMVBase_EntryPoint->AppListCandidatenum;

			sdkPureDifferEMVBase();
			gPureTransStuatus = SDK_PURE_STATUS_SELECTAID;
			return EMV_STA_IDLE;

		case SDK_PURE_STATUS_RESELECTAID:
			sdkEMVBaseCreateUnpredictNum();
			memset(&tempHighestAID, 0, sizeof(SDK_EMVBASE_CL_HIGHESTAID));
			ret = sdkEMVBaseReSelectApp(&tempHighestAID);
			if(ret != SDK_OK)
			{
				sdkPureSetCandidateListEmptyFlag(1);
				sdkCleanOutcomeParam();
				sdkSetOutcomeParam(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
				sdkSendOutcome();
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}

		case SDK_PURE_STATUS_SELECTAID:
			ret = sdkPureFinalSelectedApp(gstsdkPureTradeTable);
			if(SDK_OK == ret)
			{
				gPureTransStuatus = SDK_PURE_STATUS_GPO;
				return EMV_STA_APP_SELECTED;
			}
			else if(ret == SDK_EMV_ReadCardAgain || SDK_EMV_TransTryAgain || SDK_EMV_AppTimeOut)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_EMV_AppSelectTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_RESELECTAID;
				return EMV_REQ_SELECT_NEXT;
			}
			else if(ret == SDK_EMV_SwitchInterface)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_SWITCH_INTERFACE;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate || ret == SDK_EMV_ENDAPPLICATION)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return AS_ERR;
			}

		case SDK_PURE_STATUS_GPO:
			ret = sdkPureInitialApp(gstsdkPureTradeTable);
			if(gstPureTradeParam->EchoTransIndicator == 1)
			{
				goto _DEALAFTERECHO;
			}
			if(SDK_OK == ret)
			{
				if(gstPureTradeUnionStruct->EMVTradeParam->PureImplementationOption & 0x40)
				{
					gPureTransStuatus = SDK_PURE_STATUS_PUTDATA_AFTERGPO;
				}
				else
				{
					gPureTransStuatus = SDK_PURE_STATUS_READRECORD;
				}
				return EMV_STA_APP_INITIALIZED;
			}
			else if(ret == SDK_EMV_ReadCardAgain || SDK_EMV_TransTryAgain == ret || SDK_EMV_TORN == ret)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_EMV_AppSelectTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_RESELECTAID;
				return EMV_REQ_SELECT_NEXT;
			}
			else if(ret == SDK_EMV_SwitchInterface)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_SWITCH_INTERFACE;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				if(SDK_EMV_TORN == ret)
				{
					gPureTransStuatus = SDK_PURE_STATUS_PPSE;
					return EMV_REQ_READCAARD_AGAIN;
				}
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return IA_ERR;
			}

		case SDK_PURE_STATUS_GETDATA:
			ret = sdkPureGetDataAfterGPO(gstsdkPureTradeTable);
			if(SDK_OK == ret)
			{
				gPureTransStuatus = SDK_PURE_STATUS_READRECORD;
				return EMV_STA_GETDATAAFTERGPO_COMPLETED;
			}

		case SDK_PURE_STATUS_PUTDATA_AFTERGPO:
			ret = sdkPurePutDataAfterGPO(gstsdkPureTradeTable);
			if(SDK_OK == ret)
			{
				gPureTransStuatus = SDK_PURE_STATUS_READRECORD;
				return EMV_STA_PUTDATAAFTERGPO_COMPLETED;
			}
			else if(ret == SDK_EMV_TransTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return CO_POSTGPOPUTDATAERR;
			}

		case SDK_PURE_STATUS_READRECORD:
			ret = sdkPureReadAppData(gstsdkPureTradeTable);
			if(SDK_OK == ret)
			{
				sdkPureReadCAPK(gstsdkPureTradeTable); // added for recovery of the IPK and ICCPK before GAC Command--20210420
				ret = sdkPureCAMSelect(gstsdkPureTradeTable);
				u8 KernelCap[5];
				s32 len;
				sdkEMVBaseReadTLV(EMVTAG_PUREKernelCap, KernelCap, &len);
				if((gstPureTradeParam->PureImplementationOption & 0x40) && (PURE_TRANS_APPAUTHTRANS == gstPureTradeParam->CurTransType) && (KernelCap[3] & 0x40))
				{
					gPureTransStuatus = SDK_PURE_STATUS_READBALANCE_AFTERGAC;
				}
				else
				{
					gPureTransStuatus = SDK_PURE_STATUS_PROCESSREST;
				}
				return EMV_REQ_GETCARDNO;
			}
			else if(ret == SDK_EMV_ReadCardAgain || SDK_EMV_TransTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_EMV_AppSelectTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_RESELECTAID;
				return EMV_REQ_SELECT_NEXT;
			}
			else if(ret == SDK_EMV_SwitchInterface)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_SWITCH_INTERFACE;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return RD_ERR;
			}

		case SDK_PURE_STATUS_PROCESSREST:
			ret = sdkPureProcessRestrict(gstsdkPureTradeTable);
			gPureTransStuatus = SDK_PURE_STATUS_CVM;
			return EMV_STA_PROCESS_RESTRICT_COMPLETED;

		case SDK_PURE_STATUS_CVM:
	        ret = sdkPureCardHolderVerf(gstsdkPureTradeTable);
			gstemvbaseCVMresult = sdkPureGetCVMresult();
			Trace("emv", "CVM Result code = %d\r\n", gstemvbaseCVMresult);

			if(ret == SDK_OK)
			{
				gPureTransStuatus = SDK_PURE_STATUS_TRM;
				return EMV_STA_CARDHOLDER_VERIFY_COMPLETED;
			}
			else if(ret == EMV_REQ_ONLINE_PIN)
			{
				return EMV_REQ_ONLINE_PIN;
			}
			else if(ret == SDK_EMV_ReadCardAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_EMV_AppSelectTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_RESELECTAID;
				return EMV_REQ_SELECT_NEXT;
			}
			else if(ret == SDK_EMV_SwitchInterface)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_SWITCH_INTERFACE;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return CV_ERR;
			}

		case SDK_PURE_STATUS_TRM:
			ret = sdkPureTermRiskManage(gstsdkPureTradeTable);
			gPureTransStuatus = SDK_PURE_STATUS_TAA;
			return EMV_STA_RISK_MANAGEMENT_COMPLETED;

		case SDK_PURE_STATUS_TAA:
			ret = sdkPureTermiAnalys(gstsdkPureTradeTable);
_DEALAFTERECHO:
			if(SDK_OK == ret)
			{
				if(gstPureTradeParam->PureImplementationOption & 0x40)
				{
					gPureTransStuatus = SDK_PURE_STATUS_PUTDATA_AFTERGAC;
				}
				else
				{
					gPureTransStuatus = SDK_PURE_STATUS_READBALANCE_AFTERGAC;
				}
				return EMV_STA_ACTION_ANALYSE_COMPLETED;
			}
			else if(ret == SDK_EMV_ReadCardAgain || ret == SDK_EMV_TORN)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_EMV_AppSelectTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_RESELECTAID;
				return EMV_REQ_SELECT_NEXT;
			}
			else if(ret == SDK_EMV_SwitchInterface)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_SWITCH_INTERFACE;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				if(ret == SDK_EMV_TransOfflineDecline)
				{
					gPureTransStuatus = SDK_PURE_STATUS_PPSE;
					return EMV_DENIALED_OFFLINE;
				}
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return AA_ERR;
			}

		case SDK_PURE_STATUS_READBALANCE_AFTERGAC:
			ret = sdkPureReadBalanceAfterGAC(gstsdkPureTradeTable);
			gPureTransStuatus = SDK_PURE_STATUS_ODA;
			return EMV_STA_POSTGACBAlANCEREAD_COMPLETED;

		case SDK_PURE_STATUS_PUTDATA_AFTERGAC:
			ret = sdkPureUpdateDataAfterGAC(gstsdkPureTradeTable);
			gPureTransStuatus = SDK_PURE_STATUS_ODA;
			return EMV_STA_POSTGACPUTDATA_COMPLETED;

		case SDK_PURE_STATUS_ODA:
			ret = sdkPureDataAuth(gstsdkPureTradeTable);
			if(SDK_OK == ret)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PROCESSREST;
				return EMV_STA_DATA_AUTH_COMPLETED;
			}
			else if(ret == SDK_EMV_ReadCardAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_REQ_READCAARD_AGAIN;
			}
			else if(ret == SDK_EMV_AppSelectTryAgain)
			{
				gPureTransStuatus = SDK_PURE_STATUS_RESELECTAID;
				return EMV_REQ_SELECT_NEXT;
			}
			else if(ret == SDK_EMV_SwitchInterface)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_SWITCH_INTERFACE;
			}
			else if(ret == SDK_PARA_ERR)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_UNINITIALIZED;
			}
			else if(ret == SDK_EMV_TransTerminate)
			{
				gPureTransStuatus = SDK_PURE_STATUS_PPSE;
				return EMV_ENDAPPLICATION;
			}
			else
			{
				return DA_ERR;
			}
		case SDK_PURE_STATUS_KERNEL_DEACTIVATE:
			ret = sdkPureKernelDeactivate(gstsdkPureTradeTable);
			if(SDK_EMV_TransOnlineWait == ret)
			{
				gPureTransStuatus = SDK_PURE_STATUS_SELECTAID;
				gstPureTradeParam->CurProcessIndicator = TRANS_ONLINE_RESPONSE;
				return EMV_REQ_GO_ONLINE;
			}
			else if(SDK_EMV_TransOfflineApprove == ret)
			{
				return EMV_ACCEPTED_OFFLINE;
			}
			else if(SDK_EMV_TransOfflineDecline == ret)
			{
				return EMV_DENIALED_OFFLINE;
			}
			else if(SDK_EMV_TransTerminate == ret)
			{
				return EMV_ENDAPPLICATION;
			}
			else if(SDK_EMV_SwitchInterface == ret)
			{
				return EMV_SWITCH_INTERFACE;
			}

		default:
			break;
	}

}
