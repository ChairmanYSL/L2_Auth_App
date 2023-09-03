#include "dllpure.h"
#include "dllpureprivate.h"
//#include "stdlib.h"

PURETORNRECOVERYCONTEXT gPURETornRecoveryContext;

static const EMVBASETAG PureSDAmandatoryTag[] =
{
    {EMVTAG_CAPKI},
    {EMVTAG_IPKCert},
    {EMVTAG_SignStatAppData},
    {EMVTAG_IPKExp},
    {"\x00\x00\x00"}
};

static const EMVBASETAG PureDDAmandatoryTag[] =
{
    {EMVTAG_CAPKI},
    {EMVTAG_IPKCert},
    {EMVTAG_IPKExp},
    {EMVTAG_ICCPKCert},
    {EMVTAG_ICCPKExp},
    {"\x00\x00\x00"}
};



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

void pure_GetLibVersion(unsigned char *version)
{
	GetCompileTime(version, __DATE__, __TIME__);
}

unsigned char pure_Outcome_CommunicationErrors_PaymentTransactionNotCompleted(PURETradeUnionStruct *tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYAGAIN, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);

	return RLT_ERR_EMV_TransRestart;
}

unsigned char pure_Outcome_TransactionCompletedOnError(PURETradeUnionStruct *tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_SELECTNEXT, SDK_OUTCOME_START_C, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();

	return RLT_EMV_APPSELECTTRYAGAIN;
}

unsigned char pure_Outcome_CommunicationErrors_UnknownPaymentResult(PURETradeUnionStruct *tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYAGAIN, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);
	return RLT_ERR_EMV_TransRestart;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_OndeviceCVMRequested(PURETradeUnionStruct *tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION_ONDEVICECVM, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 1, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_SEEPHONE, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);
	return RLT_ERR_EMV_SEEPHONE;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_TryOtherInterface(PURETradeUnionStruct *tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYANOTHERINTERFACE, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 1, SDK_OUTCOME_AIP_CONTACTCHIP, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_ERROR_SWITCHINTERFACE, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	return RLT_ERR_EMV_SWITCHINTERFACE;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_TransactionApproved(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char CVM = tempApp_UnionStruct->EMVTradeParam->PureCVMParameter;
	unsigned char CVMRes;

	emvbase_avl_gettagvalue_spec(EMVTAG_CVMResult, &CVMRes, 2, 1);
	if(0x00 != CVMRes && 0x02 != CVMRes)
	{
		CVM = SDK_OUTCOME_CVM_NA;
	}

	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_APPROVED, SDK_OUTCOME_START_NA, CVM, 1, 0, 1, 1, SDK_OUTCOME_AIP_CONTACTCHIP, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	if(SDK_OUTCOME_CVM_OBTAINSIGNATURE == tempApp_UnionStruct->EMVTradeParam->PureCVMParameter)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_APPROVEDSIGN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	}
	else
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_APPROVED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	}

	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	return RLT_EMV_OFFLINE_APPROVE;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_TransactionDeclined(PURETradeUnionStruct *tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_DECLINED, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 1, 1, SDK_OUTCOME_AIP_CONTACTCHIP, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_NOTAUTHORISED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	return RLT_EMV_OFFLINE_DECLINE;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_OnlineRequest2ndTap(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char CVM = tempApp_UnionStruct->EMVTradeParam->PureCVMParameter;

	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ONLINE_TWOPRESENTMENTS, SDK_OUTCOME_START_B, CVM, 1, 1, 1, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_EMVDATA);
	tempApp_UnionStruct->SendOutcome();
	if(SDK_OUTCOME_CVM_OBTAINSIGNATURE == CVM || SDK_OUTCOME_CVM_NOCVMREQ == CVM)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_AUTHORISINGPLSWAIT, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, NULL, NULL);
	}
	else if(SDK_OUTCOME_CVM_ONLINEPIN == CVM)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSENTERPIN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, NULL, NULL);
	}
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);

	tempApp_UnionStruct->EMVTradeParam->SecondTap = 1;
	return RLT_EMV_ONLINE_WAIT;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_OnlineRequestLongTap(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char CVM = tempApp_UnionStruct->EMVTradeParam->PureCVMParameter;
	unsigned char RemovalTimeout[2];

	emvbase_avl_gettagvalue_spec(EMVTAG_JCBRemovalTimeout, RemovalTimeout, 0, 2);

	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ONLINE_TWOPRESENTMENTS, SDK_OUTCOME_START_B, CVM, 1, 1, 1, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, RemovalTimeout, SDK_OUTCOME_ONLINERESPDATA_ANY);
	tempApp_UnionStruct->SendOutcome();
	if(SDK_OUTCOME_CVM_OBTAINSIGNATURE == CVM || SDK_OUTCOME_CVM_NOCVMREQ == CVM)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_AUTHORISINGPLSWAIT, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, NULL, NULL);
	}
	else if(SDK_OUTCOME_CVM_ONLINEPIN == CVM)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSENTERPIN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, NULL, NULL);
	}
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PROCESSING, SDK_UI_STATUS_PROCESSING, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);

	return RLT_EMV_ONLINE_WAIT;
}

unsigned char pure_Outcome_FinancialTransactionCompleted_OnlineRequestNoAdditionalTap(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char CVM = tempApp_UnionStruct->EMVTradeParam->PureCVMParameter;
	unsigned char RemovalTimeout[2];

	emvbase_avl_gettagvalue_spec(EMVTAG_JCBRemovalTimeout, RemovalTimeout, 0, 2);

	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ONLINE_NOADDPREMENT, SDK_OUTCOME_START_B, CVM, 1, 0, 1, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	if(SDK_OUTCOME_CVM_OBTAINSIGNATURE == CVM || SDK_OUTCOME_CVM_NOCVMREQ == CVM)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_AUTHORISINGPLSWAIT, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, NULL, NULL);
	}
	else if(SDK_OUTCOME_CVM_ONLINEPIN == CVM)
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSENTERPIN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, NULL, NULL);
	}
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);

	return RLT_EMV_ONLINE_WAIT;
}

unsigned char pure_Outcome_NonFinancialCompleted_OfflineAuthentication(PURETradeUnionStruct * tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 1, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSREMOVECARD, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	return RLT_EMV_TERMINATE_TRANSERR;
}

unsigned char pure_Outcome_NonFinancialCompleted_RetrievalOfDataElements(PURETradeUnionStruct * tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSREMOVECARD, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	return RLT_EMV_TERMINATE_TRANSERR;
}

unsigned char pure_Outcome_EndApplication_EmptyCandidateList(PURETradeUnionStruct * tempApp_UnionStruct)
{
	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	tempApp_UnionStruct->SendOutcome();
	tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSREMOVECARD, SDK_UI_STATUS_PROCESSINGERR, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	return RLT_EMV_TERMINATE_TRANSERR;
}

unsigned char pure_CheckDataMissSDA(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char i;
    i = 0;

    while(1)
    {
        if(memcmp(PureSDAmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)PureSDAmandatoryTag[i].Tag) == 0)
        {
        	EMVBase_Trace("PURE-info: Tag%02x%02x missing \r\n", PureSDAmandatoryTag[i].Tag[0], PureSDAmandatoryTag[i].Tag[1]);
            return RLT_ERR_EMV_IccDataMissing;
        }
        i++;
    }

    if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL)
    {
    	EMVBase_Trace("PURE-info: SDA Tag List(Tag9F4A) error(len is not equal to 1 or value is not equal to '82')\r\n");
        return RLT_ERR_EMV_IccDataMissing;
    }

    return RLT_EMV_OK;
}

unsigned char pure_CheckDataMissDDA(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char i;
    i = 0;

    while(1)
    {
        if(memcmp(PureDDAmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)PureDDAmandatoryTag[i].Tag) == 0)
        {
        	EMVBase_Trace("PURE-info: Tag%02x%02x missing \r\n", PureDDAmandatoryTag[i].Tag[0], PureDDAmandatoryTag[i].Tag[1]);
            return RLT_ERR_EMV_IccDataMissing;
        }
        i++;
    }

    if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL)
    {
    	EMVBase_Trace("PURE-info: SDA Tag List(Tag9F4A) error(len is not equal to 1 or value is not equal to '82')\r\n");
        return RLT_ERR_EMV_IccDataMissing;
    }

    return RLT_EMV_OK;
}

unsigned char pure_GetIPK(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char *IPKData, IPKHash[20];
    unsigned char buf[9], bytePAN[8], byteIssuID[8];
    EMVBASE_IPK_RECOVER recovIPK;
    EMVBASETAGCVLITEM *item;
    unsigned char IPKCertexist;
    unsigned char IPKRemexist;
    unsigned char IPKExpexist;
    unsigned char PAN[10];
    unsigned short PANLen;
    unsigned char *IPKRem = NULL;
    int i, index;
    EMVBASE_CAPK_STRUCT *tempcapk;
    unsigned char tempcapkModulLen;
    int ret;
    unsigned int ipkdatalen;


	EMVBase_Trace("PURE-info: Recover Issuer Public Key\r\n");

    tempcapk = tempApp_UnionStruct->EMVTradeParam->CAPK;

    tempcapkModulLen = tempcapk->ModulLen;

    IPKCertexist = 0;

    ipkdatalen = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_IPKRem);

    if(item != NULL)
    {
        if(item->len)
        {
            ipkdatalen += (item->len);
        }
    }


    item = emvbase_avl_gettagitempointer(EMVTAG_IPKCert);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKCertexist = 1;
        }
    }

    if(IPKCertexist == 0)
    {
    	EMVBase_Trace("PURE-info: Issuer Public Key Certificate(Tag90) missing\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(item->len != tempcapk->ModulLen)
    {
    	EMVBase_Trace("PURE-info: CA public key len: %d\r\n", tempcapk->ModulLen);
		EMVBase_Trace("PURE-info: Issuer Public Key Certificate(Tag90) value len: %d\r\n", item->len);
		EMVBase_Trace("PURE-info: different len\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    ipkdatalen += (item->len);
    ipkdatalen += 3;
    IPKData = (unsigned char *)emvbase_malloc(ipkdatalen);
    if(IPKData == NULL)
    {
    	EMVBase_Trace("PURE-info: memory error\r\n");
        return RLT_EMV_ERR;
    }

    memset(IPKData, 0, ipkdatalen);

    EMVBase_RSARecover(tempcapk->Modul, tempcapk->ModulLen, tempcapk->Exponent, tempcapk->ExponentLen, item->data, IPKData);
    EMVBase_FillIPK(IPKData, &recovIPK, tempcapk);

    if(recovIPK.DataTrail != 0xBC)
    {
    	EMVBase_Trace("PURE-info: Data Trailer != 0xBC\r\n");
        emvbase_free(IPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(recovIPK.DataHead != 0x6A)
    {
    	EMVBase_Trace("PURE-info: Data Header != 0x6A\r\n");
        emvbase_free(IPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(recovIPK.CertFormat != 0x02)
    {
    	EMVBase_Trace("PURE-info: Certificate Format != 0x02\r\n");
        emvbase_free(IPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }
    //sign all the correspoding variables to the IPK
    index = 0;

    for(i = 0; i < tempcapk->ModulLen - 22; i++)
    {
        IPKData[i] = IPKData[i + 1];
    }

    index += tempcapk->ModulLen - 22;

    if(recovIPK.IPKLen > tempcapk->ModulLen - 36)
    {
        IPKRemexist = 0;
        item = emvbase_avl_gettagitempointer(EMVTAG_IPKRem);

        if(item != NULL)
        {
            if(item->len)
            {
                IPKRemexist = 1;
            }
        }

        if(IPKRemexist == 0)
        {
        	EMVBase_Trace("PURE-info: need Issuer Public Key Remainder(Tag92) but missing\r\n");
            emvbase_free(IPKData);
//            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20); //ICC data missing
//            return RLT_ERR_EMV_IccDataMissing;
			return RLT_EMV_OTHER;
        }
        IPKRem = (unsigned char *)emvbase_malloc(item->len);
        if(IPKRem == NULL)
        {
        	EMVBase_Trace("PURE-info: memory error\r\n");
            emvbase_free(IPKData);
            return RLT_EMV_ERR;
        }
        memcpy(IPKRem, item->data, item->len);
        memcpy((unsigned char*)&IPKData[index], (unsigned char*)item->data, item->len);
        index += item->len;
    }
    IPKExpexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_IPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKExpexist = 1;
        }
    }

    if(IPKExpexist)
    {
        memcpy((unsigned char*)&IPKData[index], (unsigned char*)item->data, item->len);
        index += item->len;
    }
    memset(IPKHash, 0, 20);

    EMVBase_Hash(IPKData, index, IPKHash);

    emvbase_free(IPKData);

    if(recovIPK.HashInd == 0x01)
    {
        if(memcmp(recovIPK.HashResult, IPKHash, 20))
        {
        	EMVBase_Trace("PURE-info: Hash Result verification fails\r\n");
            if(IPKRem)
            {
                emvbase_free(IPKRem);
            }
            return RLT_ERR_EMV_IccDataFormat;
        }
    }
    else
    {
    	EMVBase_Trace("PURE-info: Hash Algorithm Indicator != 0x01\r\n");
        if(IPKRem)
        {
            emvbase_free(IPKRem);
        }
        return RLT_ERR_EMV_IccDataFormat;
    }
    memset(PAN, 0, sizeof(PAN));

	if(!emvbase_avl_checkiftagexist(EMVTAG_PAN))
	{
		pure_RecoverPanFromTrack2EQU();
	}

	emvbase_avl_gettagvalue_all(EMVTAG_PAN, PAN, &PANLen);

    //verify if leftmost 3-8 digits of PAN matches IssuID
    for(i = 0; i < 4; i++)
    {
        bytePAN[2 * i] = (PAN[i] & 0xF0) >> 4;
        bytePAN[2 * i + 1] = PAN[i] & 0x0F;
        byteIssuID[2 * i] = (recovIPK.IssuID[i] & 0xF0) >> 4;
        byteIssuID[2 * i + 1] = recovIPK.IssuID[i] & 0x0F;
    }

    for(i = 7; i >= 2; i--)
    {
        if(byteIssuID[i] != 0x0F)
        {
            if(memcmp(byteIssuID, bytePAN, i + 1))
            {
            	EMVBase_TraceHex("PURE-info: 3-8 digits of PAN: ", bytePAN, i + 1);
				EMVBase_TraceHex("PURE-info: Issuer Identifier: ", byteIssuID, i + 1);
                if(IPKRem)
                {
                	EMVBase_Trace("PURE-info: no match\r\n");
                    emvbase_free(IPKRem);
                }
                return RLT_ERR_EMV_IccDataFormat;
            }
            else
            {
                break;
            }
        }
    }

    if(i < 2)
    {
    	EMVBase_Trace("PURE-info: Issuer Identifier data error\r\n");
        if(IPKRem)
        {
            emvbase_free(IPKRem);
        }
        return RLT_ERR_EMV_IccDataFormat;
    }

    //verify if expiredate is later than current date
    if(EMVBase_ExpireDateVerify(recovIPK.ExpireDate) != RLT_EMV_OK)
    {
    	EMVBase_Trace("PURE-info: Expired\r\n");
        if(IPKRem)
        {
            emvbase_free(IPKRem);
        }
        return RLT_EMV_ERR;
    }
    //check if IPK is revoked.
    memcpy(buf, tempcapk->RID, 5);
    buf[5] = tempcapk->CAPKI;
    memcpy((unsigned char*)&buf[6], recovIPK.CertSerial, 3);

    emvbase_free(tempApp_UnionStruct->EMVTradeParam->CAPK);
    tempApp_UnionStruct->EMVTradeParam->CAPK = NULL;

    if(tempApp_UnionStruct->termipkrevokecheck != NULL)
    {
        ret = tempApp_UnionStruct->termipkrevokecheck(buf);//����SDK_OK��ʾ���ǻ��չ�Կ
        if(ret != 1)
        {
        	EMVBase_Trace("PURE-info: Issuer Public Key revoke\r\n");
            if(IPKRem)
            {
                emvbase_free(IPKRem);
            }
            return RLT_EMV_ERR;
        }
    }

    if(recovIPK.IPKAlgoInd != 0x01)  //other than '01' is not recognised.
    {
    	EMVBase_Trace("PURE-info: Issuer Public Key Algorithm Indicator != 0x01\r\n");
        if(IPKRem)
        {
            emvbase_free(IPKRem);
        }
        return RLT_ERR_EMV_IccDataFormat;
    }

	EMVBase_Trace("PURE-info: (For Debug Only)recovIPK.IPKLen = %d\r\n", recovIPK.IPKLen);
    tempApp_UnionStruct->EMVTradeParam->IPKModulLen = recovIPK.IPKLen;

    if(tempApp_UnionStruct->EMVTradeParam->IPKModul == NULL)
    {
        tempApp_UnionStruct->EMVTradeParam->IPKModul = (unsigned char *)emvbase_malloc(recovIPK.IPKLen);
        if(tempApp_UnionStruct->EMVTradeParam->IPKModul == NULL)
        {
        	EMVBase_Trace("PURE-info: memory error\r\n");
            if(IPKRem)
            {
                emvbase_free(IPKRem);
            }
            return RLT_EMV_ERR;
        }
        memset(tempApp_UnionStruct->EMVTradeParam->IPKModul,0,recovIPK.IPKLen);

    }

    if(recovIPK.IPKLen <= tempcapkModulLen - 36)
    {
        memcpy(tempApp_UnionStruct->EMVTradeParam->IPKModul, recovIPK.IPKLeft, recovIPK.IPKLen);
    }
    else
    {
        memcpy(tempApp_UnionStruct->EMVTradeParam->IPKModul, recovIPK.IPKLeft, tempcapkModulLen - 36);
        memcpy((unsigned char*)&tempApp_UnionStruct->EMVTradeParam->IPKModul[tempcapkModulLen - 36], IPKRem, (recovIPK.IPKLen + 36) - tempcapkModulLen);
    }
    if(IPKRem)
    {
        emvbase_free(IPKRem);
    }

    return RLT_EMV_OK;
}

unsigned char pure_GetICCPK(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char *ICCPKData, ICCPKHash[20];
    unsigned short index;
    EMVBASE_ICCPK_RECOVER recovICCPK;
    EMVBASETAGCVLITEM *item, *item1;
    unsigned char *ICCPKToSign;
    unsigned char testlen;
    unsigned char tmppan1[20], tmppan1len, tmppan2[20], tmppan2len;
    unsigned char IPKExpexist, ICCPKCertexist, ICCPKRemexist, PANexist;
    unsigned char *ICCPKRem = NULL;
    unsigned char CardIdentifyInfo;
    unsigned int ICCPKToSignLen = 0;


	EMVBase_Trace("PURE-info: Recover ICC Public Key\r\n");

	if(tempApp_UnionStruct->EMVTradeParam->AuthDataLen)
	{
		if(tempApp_UnionStruct->EMVTradeParam->AuthData == NULL)
		{
			EMVBase_Trace("EmvTradeParam->AuthData is freed\r\n");
			return RLT_EMV_ERR;
		}
	}

    ICCPKCertexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_ICCPKCert);

    if(item != NULL)
    {
        if(item->len)
        {
            ICCPKCertexist = 1;
        }
    }

    if(ICCPKCertexist == 0)
    {
    	EMVBase_Trace("PURE-info: ICC Public Key Certificate(Tag9F46) missing\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(item->len != tempApp_UnionStruct->EMVTradeParam->IPKModulLen)
    {
    	EMVBase_Trace("PURE-info: Issuer public key len: %d\r\n", tempApp_UnionStruct->EMVTradeParam->IPKModulLen);
		EMVBase_Trace("PURE-info: ICC Public Key Certificate(Tag9F46) value len: %d\r\n", item->len);
		EMVBase_Trace("PURE-info: different len\r\n");
        return RLT_EMV_ERR;
    }

    IPKExpexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_IPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKExpexist = 1;
        }
    }

    if(IPKExpexist == 0)
    {
    	EMVBase_Trace("PURE-info: Issuer Public Key Exponent(Tag9F32) missing\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }
    ICCPKCertexist = 0;
    item1 = emvbase_avl_gettagitempointer(EMVTAG_ICCPKCert);

    if(item1 != NULL)
    {
        if(item1->len)
        {
            ICCPKCertexist = 1;
        }
    }

    if(ICCPKCertexist == 0)
    {
    	EMVBase_Trace("PURE-info: ICC Public Key Certificate(Tag9F46) missing\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    ICCPKData = (unsigned char *)emvbase_malloc(tempApp_UnionStruct->EMVTradeParam->IPKModulLen);
    if(ICCPKData == NULL)
    {
    	EMVBase_Trace("PURE-info: memory error\r\n");
        return RLT_EMV_ERR;
    }
    memset(ICCPKData,0,tempApp_UnionStruct->EMVTradeParam->IPKModulLen);

    EMVBase_RSARecover(tempApp_UnionStruct->EMVTradeParam->IPKModul, tempApp_UnionStruct->EMVTradeParam->IPKModulLen, item->data, item->len, item1->data, ICCPKData);

    emvbase_free(tempApp_UnionStruct->EMVTradeParam->IPKModul);
    tempApp_UnionStruct->EMVTradeParam->IPKModul = NULL;

    EMVBase_FillICCPK(ICCPKData, tempApp_UnionStruct->EMVTradeParam->IPKModulLen, &recovICCPK);

    if(recovICCPK.DataTrail != 0xBC)
    {
    	EMVBase_Trace("PURE-info: Data Trailer != 0xBC\r\n");
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    if(recovICCPK.DataHead != 0x6A)
    {
    	EMVBase_Trace("PURE-info: Data Header != 0x6A\r\n");
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    if(recovICCPK.CertFormat != 0x04)
    {
    	EMVBase_Trace("PURE-info: Certificate Format != 0x04\r\n");
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }
    index = 0;


    ICCPKToSignLen = tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22;
    ICCPKToSignLen += 3;        //EMVTAG_ICCPKExp
    ICCPKToSignLen += tempApp_UnionStruct->EMVTradeParam->AuthDataLen;

	EMVBase_Trace("PURE-info: (For Debug Only)recovICCPK.ICCPKLen = %d\r\n", recovICCPK.ICCPKLen);
	EMVBase_Trace("PURE-info: (For Debug Only)IPKModulLen = %d\r\n", tempApp_UnionStruct->EMVTradeParam->IPKModulLen);
    if(recovICCPK.ICCPKLen > (tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 42))
    {
        ICCPKRemexist = 0;
        item1 = emvbase_avl_gettagitempointer(EMVTAG_ICCPKRem);

        if(item1 != NULL)
        {
            if(item1->len)
            {
                ICCPKRemexist = 1;
            }
        }

        if(ICCPKRemexist == 0)
        {
        	EMVBase_Trace("PURE-info: need ICC Public Key Remainder(Tag9F48) but missing\r\n");
            emvbase_free(ICCPKData);
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);           //ICC data missing
            return RLT_ERR_EMV_IccDataMissing;
        }

        ICCPKRem = (unsigned char *)emvbase_malloc(item1->len);
        if(ICCPKRem == NULL)
        {
        	EMVBase_Trace("PURE-info: memory error\r\n");
            emvbase_free(ICCPKData);
            return RLT_EMV_ERR;
        }

        ICCPKToSignLen += item1->len;
        ICCPKToSign = (unsigned char *)emvbase_malloc(ICCPKToSignLen);

        if(ICCPKToSign == NULL)
        {
        	EMVBase_Trace("PURE-info: memory error\r\n");
            emvbase_free(ICCPKData);
            emvbase_free(ICCPKRem);
            return RLT_EMV_ERR;
        }
        memcpy(ICCPKToSign, (unsigned char*)&ICCPKData[1], tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22);
        index += tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22;

        memcpy(ICCPKRem, item1->data, item1->len);
        memcpy((unsigned char*)&ICCPKToSign[index], (unsigned char*)item1->data, item1->len);
        index += item1->len;
    }
    else
    {
        ICCPKToSign = (unsigned char *)emvbase_malloc(ICCPKToSignLen);
        if(ICCPKToSign == NULL)
        {
        	EMVBase_Trace("PURE-info: memory error\r\n");
            emvbase_free(ICCPKData);
            return RLT_EMV_ERR;
        }

        memcpy(ICCPKToSign, (unsigned char*)&ICCPKData[1], tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22);
        index += tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22;
    }

    emvbase_free(ICCPKData);

    ICCPKCertexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_ICCPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            ICCPKCertexist = 1;
        }
    }

    if(ICCPKCertexist == 0)
    {
    	EMVBase_Trace("PURE-info: ICC Public Key Exponent(Tag9F47) missing\r\n");
        emvbase_free(ICCPKToSign);
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_ERR_EMV_IccDataFormat;
    }
    memcpy((unsigned char*)&ICCPKToSign[index], (unsigned char*)item->data, item->len);
    index += item->len;

	if(tempApp_UnionStruct->EMVTradeParam->AuthData)
    {
        memcpy((unsigned char*)&ICCPKToSign[index], tempApp_UnionStruct->EMVTradeParam->AuthData, tempApp_UnionStruct->EMVTradeParam->AuthDataLen);
	    index += tempApp_UnionStruct->EMVTradeParam->AuthDataLen;

	    emvbase_free(tempApp_UnionStruct->EMVTradeParam->AuthData);
	    tempApp_UnionStruct->EMVTradeParam->AuthData = NULL;
    }


    EMVBase_Hash(ICCPKToSign, index, ICCPKHash);

    emvbase_free(ICCPKToSign);


    if(recovICCPK.HashInd == 0x01)  //SHA-1 algorithm
    {
        if(memcmp(recovICCPK.HashResult, ICCPKHash, 20))
        {
        	EMVBase_Trace("PURE-info: Hash Result verification fails\r\n");
            if(ICCPKRem)
            {
                emvbase_free(ICCPKRem);
            }
            return RLT_EMV_ERR;
        }
    }
    else
    {
    	EMVBase_Trace("PURE-info: Hash Algorithm Indicator != 0x01\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_EMV_ERR;
    }
    PANexist = 0;
    item1 = emvbase_avl_gettagitempointer(EMVTAG_PAN);

    if(item1 != NULL)
    {
        if(item1->len)
        {
            PANexist = 1;
        }
    }

    if(PANexist == 0)
    {
    	EMVBase_Trace("PURE-info: Track2(Tag57) missing\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_ERR_EMV_IccDataFormat;
    }

    memset(tmppan1, 0, sizeof(tmppan1));
    EMVBaseBcdToAsc(tmppan1, recovICCPK.AppPAN, 10);
    tmppan1len = 20;

    for(testlen = 0; testlen < 20; testlen++)
    {
        if((tmppan1[testlen] == 'F') || (tmppan1[testlen] == 'f'))
        {
            tmppan1len = testlen;
            break;
        }
    }

    memset(tmppan2, 0, sizeof(tmppan2));
    EMVBaseBcdToAsc(tmppan2, item1->data, item1->len);
    tmppan2len = (item1->len) << 1;

    for(testlen = 0; testlen < tmppan2len; testlen++)
    {
        if((tmppan1[testlen] == 'F') || (tmppan2[testlen] == 'f'))
        {
            tmppan2len = testlen;
            break;
        }
    }

	EMVBase_Trace("PURE-info: PAN in Track2(Tag57): %s\r\n", tmppan1);
	EMVBase_Trace("PURE-info: APP PAN of ICC Public Key: %s\r\n", tmppan2);
    if(tmppan1len != tmppan2len)
    {
    	EMVBase_Trace("PURE-info: PAN different\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_EMV_ERR;
    }

    if(memcmp(tmppan1, tmppan2, tmppan1len))
    {
    	EMVBase_Trace("PURE-info: PAN different\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_EMV_ERR;
    }

    //verify expiredate is later than current date
    if(EMVBase_ExpireDateVerify(recovICCPK.ExpireDate) == RLT_EMV_ERR)
    {
    	EMVBase_Trace("PURE-info: Expired\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_EMV_ERR;
    }

    if(recovICCPK.ICCPKAlgoInd != 0x01)  //other than '01' is not recognised.
    {
    	EMVBase_Trace("PURE-info: ICC Public Key Algorithm Indicator != 0x01\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_EMV_ERR;
    }
    tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen = recovICCPK.ICCPKLen;

    if(tempApp_UnionStruct->EMVTradeParam->ICCPKModul == NULL)
    {
        tempApp_UnionStruct->EMVTradeParam->ICCPKModul = (unsigned char  *)emvbase_malloc(recovICCPK.ICCPKLen);

    }

    if(tempApp_UnionStruct->EMVTradeParam->ICCPKModul == NULL)
    {
    	EMVBase_Trace("PURE-info: memory error\r\n");
        if(ICCPKRem)
        {
            emvbase_free(ICCPKRem);
        }
        return RLT_EMV_ERR;
    }

    memset(tempApp_UnionStruct->EMVTradeParam->ICCPKModul,0,recovICCPK.ICCPKLen);

    if(recovICCPK.ICCPKLen <= (tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 42))
    {
        memcpy(tempApp_UnionStruct->EMVTradeParam->ICCPKModul, recovICCPK.ICCPKLeft, recovICCPK.ICCPKLen);
    }
    else
    {
        memcpy(tempApp_UnionStruct->EMVTradeParam->ICCPKModul, recovICCPK.ICCPKLeft, tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 42);
        memcpy((unsigned char*)&tempApp_UnionStruct->EMVTradeParam->ICCPKModul[tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 42], ICCPKRem, recovICCPK.ICCPKLen - tempApp_UnionStruct->EMVTradeParam->IPKModulLen + 42);
    }

    if(ICCPKRem)
    {
        emvbase_free(ICCPKRem);
    }
    return RLT_EMV_OK;
}

unsigned char pure_CompareIACTAC(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char i, k;
    unsigned char bFitIAC, bFitTAC;
    Card_TAC CardTAC;

    unsigned char TVR[5];
    unsigned char TACDenial[5];
    unsigned char IACDenial[5], IACDenialbExist;
    unsigned char TermType;
    unsigned char TACOnline[5], IACOnline[5], IACOnlinebExist;
    unsigned char TACDefault[5], IACDefault[5], IACDefaultbExist;
    unsigned char bExist;
	unsigned char TTPI[5];


//	if(tempApp_UnionStruct->EMVTradeParam->bForceAAC)
//	{
//		return TAA_DENIAL;
//	}
//
//	if(tempApp_UnionStruct->EMVTradeParam->bForceOnline)
//	{
//		return TAA_ONLINE;
//	}

    IACDenialbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACDenial, IACDenial, 0, 5));
    if(IACDenialbExist == 0)   //IAC-denial not exist
    {
        memset(IACDenial, 0, 5);
    }
    memcpy(CardTAC.IACDenial, IACDenial, 5);


    IACOnlinebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACOnline, IACOnline, 0, 5));
    if(IACOnlinebExist == 0)   //IAC-online not exist
    {
        memset(CardTAC.IACOnline, 0xFF, 5);
        memset(IACOnline, 0, 5);
    }
    else
	{
		memcpy(CardTAC.IACOnline, IACOnline, 5);
	}

    IACDefaultbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACDefault, IACDefault, 0, 5));
    if(IACDefaultbExist == 0)   //IAC-default not exist
    {
        memset(CardTAC.IACDefault, 0xFF, 5);
        memset(IACDefault, 0, 5);
    }
    else
    {
        memcpy(CardTAC.IACDefault, IACDefault, 5);   //lechan 20070207
    }
    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACDenial, TACDenial, 0, 5));
    if(bExist == 0)
    {
        memset(TACDenial, 0, 5);
    }

    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACOnline, TACOnline, 0, 5));
    if(bExist == 0)
    {
        memset(TACOnline, 0, 5);
        TACOnline[0] |= 0xC8;      //EMVapp,p35
    }

    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACDefault, TACDefault, 0, 5));
    if(bExist == 0)
    {
        memset(TACDefault, 0, 5);
        TACDefault[0] |= 0xC8;
    }
    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

	EMVBase_TraceHex("TVR",TVR,5);
	EMVBase_TraceHex("TACDefault",TACDefault,5);
	EMVBase_TraceHex("TACOnline",TACOnline,5);
	EMVBase_TraceHex("TACDenial",TACDenial,5);
	EMVBase_TraceHex("IACDefault",IACDefault,5);
	EMVBase_TraceHex("IACOnline",IACOnline,5);
	EMVBase_TraceHex("IACDenial",IACDenial,5);

    if(!memcmp(TVR, "\x00\x00\x00\x00\x00", 5))
	{
		return TAA_OFFLINE;
	}

    bFitIAC = 0;
    bFitTAC = 0;
	EMVBase_Trace("Terminal ActAnaly Denial Flag\r\n");
    for(i = 0; i < 5; i++)
    {
        k = TVR[i];

        if((k & CardTAC.IACDenial[i]) != 0)
		{
			bFitIAC = 1;
		}

        if((k & TACDenial[i]) != 0)
		{
			bFitTAC = 1;
		}
    }

    if(bFitIAC || bFitTAC)
	{
		return TAA_DENIAL;
	}

    TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);
	EMVBase_Trace("TermType: %02x\r\n", TermType);
    TermType = TermType & 0x0F;
	EMVBase_Trace("TermType bit4-1: %d\r\n", TermType);

	emvbase_avl_gettagvalue_spec(EMVTAG_PURETTPI, TTPI, 0, 5);
	EMVBase_TraceHex("pure-info: TTPI: ", TTPI, 5);
	EMVBase_Trace("pure-info: TTPI show Term whether has online cap: %d\r\n", TTPI[0] & 0x08);
	if((TermType == 1 || TermType == 2 || TermType == 4 || TermType == 5) && (TTPI[0] & 0x08)) //FIXME: Terminal has Online capability
    {
		EMVBase_Trace("Terminal ActAnaly Online Flag\r\n");
        bFitIAC = 0;
        bFitTAC = 0;

        for(i = 0; i < 5; i++)
        {
            k = TVR[i];

            if((k & CardTAC.IACOnline[i]) != 0)
			{
				bFitIAC = 1;
			}

            if((k & TACOnline[i]) != 0)
			{
				bFitTAC = 1;
			}
        }

        if(bFitIAC || bFitTAC)
		{
			return TAA_ONLINE;
		}
        else
		{
			return TAA_OFFLINE;
		}
    }

	EMVBase_Trace("Unable Online/Offline Only Terminal ActAnaly\r\n");

    bFitIAC = 0;
    bFitTAC = 0;

    for(i = 0; i < 5; i++)
    {
        k = TVR[i];

        if((k & CardTAC.IACDefault[i]) != 0)
		{
			bFitIAC = 1;
		}

        if((k & TACDefault[i]) != 0)
		{
			bFitTAC = 1;
		}
    }


    if(bFitIAC || bFitTAC)
	{
		return TAA_DENIAL;
	}
    else
	{
		return TAA_OFFLINE;
	}
}

//unsigned char pure_ReadSelectRetData(EMVBASE_SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, PURETradeUnionStruct *tempAppUnionStruct)
//{
//	  int matchnum;
//    EMVBASE_LISTAPPDATA *tempAppAppData;
//    EMVBASE_TERMAPP tempAppTermApp;
//    unsigned char *tempappnum;
//    //unsigned char *temptermlistnum;
//    unsigned char j, k;     //i,
//    unsigned short index, indexFCI, indexFCIProp, len, lenFCI, lenFCIProp;
//    int indexIssuerDiscret, lenIssuerDiscret;
//    int indexAppApp, lenAppApp;
//    unsigned char priorityexist;
//	unsigned char templen, ExtendedLen;
//	unsigned char tempindex;
//	unsigned char errindex = 0,i;
//	unsigned char errindex2 = 0;
//	unsigned short tempBF0Clen;
//
//
//    tempappnum = &(tempAppUnionStruct->EMVTradeParam->AppListCandidatenum);
//    tempAppAppData = tempAppUnionStruct->EMVTradeParam->AppListCandidate;
//    *tempappnum = 0;
//
//
//    index = 0;
//
//    if(DataOut[index] != 0x6F)          //FCIģ���ʶ
//    {
//        return RLT_ERR_EMV_IccDataFormat;         //FCI template
//    }
//    index++;
//
//    if(EMVBase_ParseExtLen(DataOut, &index, &lenFCI))
//    {
//        return RLT_ERR_EMV_IccDataFormat;
//    }
//    indexFCI = index;
//
//    while(index < indexFCI + lenFCI)
//    {
//        if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }
//
//        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
//        else if(DataOut[index] == 0x84)         //DF��
//        {
//            if(selectRet->DFNameExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
//            index++;
//            selectRet->DFNameLen = DataOut[index];
//
//            if(selectRet->DFNameLen > 16) { return RLT_ERR_EMV_IccDataFormat; }
//            memcpy(selectRet->DFName, DataOut + index + 1, selectRet->DFNameLen);
//            index += selectRet->DFNameLen + 1;
//            selectRet->DFNameExist = 1;
//        }
//        else if(DataOut[index] == 0xA5)         //FCI����ר��ģ��
//        {
//            selectRet->FCIPropExist = 1;
//            index++;
//
//            if(EMVBase_ParseExtLen(DataOut, &index, &lenFCIProp))
//            {
//                return RLT_ERR_EMV_IccDataFormat;
//            }
//            indexFCIProp = index;
//
//			EMVBase_Trace( "indexFCIProp=%d lenFCIProp=%d\r\n",indexFCIProp, lenFCIProp);
//            while(index < indexFCIProp + lenFCIProp)
//            {
//                if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }
//
//                if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
//                else if(!memcmp(DataOut + index, "\xBF\x0C", 2))             //FCI �������Զ�������
//                {
//                    if(selectRet->IssuerDiscretExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
//                    index += 2;
//
//                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
//                    {
//                    	EMVBase_Trace( "bf0c error!\r\n");
//                        return RLT_ERR_EMV_IccDataFormat;
//                    }
//
//                    if(len > 253) { return RLT_ERR_EMV_IccDataFormat; }
//                    selectRet->IssuerDiscretLen = len;                     //sxl 090211
//                    EMVBase_Trace( "IssuerDiscretLen=%d index=%d\r\n", len, index);
//                    //memcpy(selectRet->IssuerDiscret,DataOut+index,len);
//                    selectRet->IssuerDiscretExist = 1;
//
//                    indexIssuerDiscret = index;
//                    lenIssuerDiscret = len;
//
//                    while(index < indexIssuerDiscret + lenIssuerDiscret)
//                    {
//                    	EMVBase_Trace( "index=%d indexIssuerDiscret=%d lenIssuerDiscret=%d",index, indexIssuerDiscret,lenIssuerDiscret);
//                        if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }
//
//                        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
//                        else if(DataOut[index] == 0x61)
//                        {
//                            memset((unsigned char *)&tempAppAppData[*tempappnum], 0, sizeof(EMVBASE_LISTAPPDATA));                           //��Ӧ��
//                            priorityexist = 0;
//                            index += 1;
//
//                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
//                            {
//                            	EMVBase_Trace( "tag 61 error\r\n");
//                                return RLT_ERR_EMV_IccDataFormat;
//                            }
//                            indexAppApp = index;
//                            lenAppApp = len;
//
//							errindex = index;
//							if(len == 0)
//							{
//								EMVBase_Trace("tag 61 length zero!\r\n");
//								return RLT_ERR_EMV_IccDataFormat;
//							}
//
//                            while(index < indexAppApp + lenAppApp)
//                            {
//                            	EMVBase_Trace( "index=%d LenOut=%d\r\n", index, LenOut);
//                                if(index >= LenOut)
//								{
//									EMVBase_Trace( "error11111");
//									//return ERR_EMV_IccDataFormat;
//									break;
//								}
//
//                                if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
//                                else if(DataOut[index] == 0x4F)                                 //AID
//                                {
//                                    index += 1;
//                                    (tempAppAppData + (*tempappnum))->AIDLen = DataOut[index];
//#if 1
//									//if((tempAppAppData + (*tempappnum))->AIDLen > 16) { (tempAppAppData + (*tempappnum))->AIDLen = 16; }
//									if((tempAppAppData + (*tempappnum))->AIDLen > 16)
//									{
//										(tempAppAppData + (*tempappnum))->AIDLen = 0;
//									}
//
//#else
//									if((tempAppAppData + (*tempappnum))->AIDLen > 16) //zwb  for pure 180312
//									{
//										//return ERR_EMV_IccDataFormat;
//										index = indexIssuerDiscret + lenIssuerDiscret;
//										break;
//									}
//#endif
//
//
//									index++;
//                                    j = (tempAppAppData + (*tempappnum))->AIDLen;
//                                    memcpy((tempAppAppData + (*tempappnum))->AID, DataOut + index, j);
//                                    EMVBase_TraceHex( "4F aid &len", DataOut + index, j);
//                                    index += j;
//                                }
//                                else if(DataOut[index] == 0x50)                                 //AppLabel
//                                {//500B 4A434220437265646974870101
//                                	EMVBase_Trace("goto deal tag 50!\r\n");
//                                    index += 1;
//                                    (tempAppAppData + (*tempappnum))->AppLabelLen = DataOut[index];
//
//                                    if((tempAppAppData + (*tempappnum))->AppLabelLen > 16) { (tempAppAppData + (*tempappnum))->AppLabelLen = 16; }
//                                    index++;
//                                    j = (tempAppAppData + (*tempappnum))->AppLabelLen;
//                                    memcpy((tempAppAppData + (*tempappnum))->AppLabel, DataOut + index, j);
//                                    index += j;
//                                }
//                                else if(DataOut[index] == 0x87)    //AppPriority
//                                {
//									EMVBase_Trace("goto deal tag 87!\r\n");
//									index ++;
//									templen = DataOut[index];
//									if(templen == 1)//870201   870001	87020001   870101
//									{
//										index++;
//										(tempAppAppData + (*tempappnum))->Priority = DataOut[index];
//										index++;
//										priorityexist = 1;
//
//									}
//									else
//									{
//										index++;
//										templen = 0;
//										index +=templen;
//									}
//
//                                }
//								else if(memcmp(DataOut + index, "\x9F\x2A", 2) == 0)//kernel ID
//								{//9F2A0105  9F2A04050001  9F2A0305000001  9F2A0005
//
//									index += 2;
//									templen = DataOut[index];
//									EMVBase_Trace( "tag9F2A templen=%d\r\n", templen);
//									if(templen == 1)
//									{
//										index++;
//										EMVBase_Trace( "tag9F2A 1byte=%02x\r\n", DataOut[index]);
//										if(DataOut[index] != 0x05)
//										{
//											templen = 0;
//											(tempAppAppData + (*tempappnum))->AIDLen = 0;
//											//index--;
//										}
//										index += templen;
//									}
//									else
//									{
//										index++;
//										templen = 0;
//										index += templen;
//									}
//								}
//								#if 1
//								else if(memcmp(DataOut + index, "\x9F\x29", 2) == 0)//Extened Selection zwb 180202
//								{  //9F29021122  9F29031122  9F29011122  9F29001122
//
//									EMVBase_Trace( "goto deal tag 9F29!\r\n");
//									index += 2;
//									templen = DataOut[index];
//									if(templen == 2)
//									{
//										index++;
//										index += templen;
//									}
//									else
//									{
//										index++;
//										templen =  0;
//										index += templen;
//									}
//								}
//								#endif
//                                else
//                                {
//									errindex = index;
//
//                                    k = DataOut[index];
//									EMVBase_Trace( "xxx k=%02x\r\n", k);
//
//                                    if((k & 0x1F) == 0x1F)
//                                    {
//                                        index++;
//                                    }
//                                    index++;
//
//
//									EMVBase_Trace( "index111=%d indexAppApp + lenAppApp=%d\r\n", index, indexAppApp + lenAppApp);
//									EMVBase_Trace( "DataOut[index]=%02x\r\n", DataOut[index]);
//									if(index < indexAppApp + lenAppApp)
//									{
//										if(EMVBase_ParseExtLen(DataOut, &index, &len))
//										{
//											break;
//										}
//										EMVBase_Trace( "test111111");
//									}
//									else
//									{
//										break;
//									}
//
//                                    index += len;
//                                }
//
//                            }
//
//							EMVBase_Trace("00000index=%d LenOut=%d",index,LenOut);
//
//							EMVBase_Trace("00000index=%d indexAppApp + lenAppApp=%d\r\n", index, indexAppApp + lenAppApp);
//							if(index >  indexAppApp + lenAppApp /*&& index < LenOut*/)	// parase err
//							{
//								EMVBase_Trace("errindex=%d\r\n", errindex);
//								index = errindex;
//								for(i = errindex ; i < indexAppApp + lenAppApp ;  i++)
//								{
//									if(DataOut[i] == 0x61)
//									{
//										EMVBase_Trace("another tag 61");
//										break;
//									}
//								}
//								index = i;
//
//							}
//
//                            if((tempAppAppData + (*tempappnum))->AIDLen != 0)                            //ȷ��ÿ��Ŀ¼��ڶ���AID
//                            {
//                                //paywave��û��˵���
//                                if(priorityexist == 0)
//                                {
//                                    (tempAppAppData + (*tempappnum))->Priority = 0;
//                                }
//                                //sxl paywave�Ĳ��ж�priority���λ
//                                //if(((tempAppAppData + (*tempappnum))->Priority & 0x80) == 0 )
//                                {
//                                    //matchnum = tempAppUnionStruct->check_MatchTermAID((tempAppAppData + (*tempappnum))->ReqKernelID, (tempAppAppData + (*tempappnum))->AID, (tempAppAppData + (*tempappnum))->AIDLen, &tempAppTermApp, PAYWAVEAPP);
//								#if 1
//									//matchnum = tempAppUnionStruct->check_MatchTermAID((tempAppAppData + (*tempappnum))->ReqKernelID, (tempAppAppData + (*tempappnum))->AID, (tempAppAppData + (*tempappnum))->AIDLen, &tempAppTermApp, EMVAPP);
//
//									matchnum = tempAppUnionStruct->CheckMatchTermAID((tempAppAppData + (*tempappnum))->AID, (tempAppAppData + (*tempappnum))->AIDLen, &tempAppTermApp,NULL,0);
//									EMVBase_Trace( "matchnum=%d tempappnum=%d\r\n", matchnum, *tempappnum);
//
//                                    if(matchnum >= 0)
//                                    {
//                                        (tempAppAppData + (*tempappnum))->AidInTermLen = tempAppTermApp.AIDLen;
//                                        memcpy((tempAppAppData + (*tempappnum))->AidInTerm, tempAppTermApp.AID, (tempAppAppData + (*tempappnum))->AidInTermLen);
//                                        (*tempappnum)++;
//                                    }
//								#endif
//                                }
//
//                                //}
//                            }
//                        }
//                        else if(!memcmp(DataOut + index, "\x9F\x4D", 2))                    //Log Entry��־���
//                        {
//                            if(selectRet->LogEntryExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
//                            index += 2;
//
//                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
//                            {
//                                return RLT_ERR_EMV_IccDataFormat;
//                            }
//                            selectRet->LogEntryLen = len;
//                            //memcpy(selectRet->LogEntry,DataOut+index,selectRet->LogEntryLen);
//                            index += selectRet->LogEntryLen;
//                            selectRet->LogEntryExist = 1;
//                        }
//                        else
//                        {
//                            if(errindex2 == 0)
//                            {
//                            	errindex2 = index;
//                            }
//                            k = DataOut[index];
//							EMVBase_Trace("errindex2 k=%02x\r\n", k);
//
//                            if((k & 0x1F) == 0x1F)
//                            {
//                                index++;
//                            }
//                            index++;
//
//							EMVBase_Trace("index2222=%d indexIssuerDiscret + lenIssuerDiscret=%d\r\n", index, indexIssuerDiscret + lenIssuerDiscret);
//
//							if(index < indexIssuerDiscret + lenIssuerDiscret)
//							{
//								EMVBase_Trace("DataOut[index]=%02x\r\n", DataOut[index]);
//								if(EMVBase_ParseExtLen(DataOut, &index, &len))
//								{
//									break;
//								}
//							}
//							else
//							{
//								break;
//							}
//
//                            index += len;
//                        }
//                    }
//
//					EMVBase_Trace("index3333=%d indexIssuerDiscret + lenIssuerDiscret=%d\r\n", index, indexIssuerDiscret + lenIssuerDiscret);
//					if(index >	indexIssuerDiscret + lenIssuerDiscret)	// parase err
//					{
//						EMVBase_Trace("errindex2=%d\r\n", errindex2);
//						index = errindex2;
//						for(i = errindex2 ; i < indexIssuerDiscret + lenIssuerDiscret ; i++)
//						{
//							if(DataOut[i] == 0x61)
//							{
//								break;
//							}
//						}
//						index = i;
//
//					}
//				}
//                else                //other unknown TLV data
//                {
//                    k = DataOut[index];
//					EMVBase_Trace( "2222k=%d\r\n",k);
//
//                    if((k & 0x1F) == 0x1F)
//                    {
//                        index++;
//                    }
//                    index++;
//
//                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
//                    {
//						EMVBase_Trace( "yyyyyyyyyyyy\r\n");
//                        return RLT_ERR_EMV_IccDataFormat;
//                    }
//                    index += len;
//                }
//            }
//
//			EMVBase_Trace("IssuerDiscretLen=%d index=%d indexIssuerDiscret=%d", selectRet->IssuerDiscretLen, index, indexIssuerDiscret);
//			if( selectRet->IssuerDiscretLen != (index - indexIssuerDiscret))// BF0C length value
//			{// the length  must correspond to the length of value
//				EMVBase_Trace("BF0C length value error!\r\n");
//				return RLT_ERR_EMV_IccDataFormat;
//			}
//
//            if(index != indexFCIProp + lenFCIProp)
//            {
//				EMVBase_Trace( "zzzzzzzzzzzzz\r\n");
//                return RLT_ERR_EMV_IccDataFormat;
//            }
//        }
//        else
//        {
//            k = DataOut[index];
//
//            if((k & 0x1F) == 0x1F)
//            {
//                index++;
//            }
//            index++;
//
//            if(EMVBase_ParseExtLen(DataOut, &index, &len))
//            {
//				EMVBase_Trace( "aaaaaaaaaa\r\n");
//                return RLT_ERR_EMV_IccDataFormat;
//            }
//            index += len;
//        }
//    }
//
//    if(index != indexFCI + lenFCI)
//    {
//		EMVBase_Trace( "bbbbbbbbbbb\r\n");
//        return RLT_ERR_EMV_IccDataFormat;
//    }
//    return RLT_EMV_OK;
//}

unsigned char pure_checkifwrongtag(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char *tag, unsigned char taglen, EMVBASETAGCVLITEM *retrunitem, unsigned char Redundantflag)
{
	int i;
	unsigned char present = 0;
	unsigned char updateRA = 0;
	unsigned char temptag[3];
	unsigned char temptaglen;
	EMVBASETAGCVLITEM *item;
	unsigned char ret = 2;
	int ret1;
	unsigned int rundatabuflen;

	memset(temptag, 0, sizeof(temptag));
	temptaglen = (taglen > 3) ? 3 : (taglen);
	memcpy(temptag, tag, temptaglen);

	item = emvbase_avl_gettagitempointer(temptag);
	if(item != NULL)
	{
		present = 1;
		updateRA = item->updatecondition & EMVTAGUPDATECONDITION_RA;
	}

	if(present == 0)
	{
		ret1 = emvbase_TagBaseLib_read(tag, taglen, retrunitem,EMVTAGTYPE_PURE);
		if(ret1 == 0)
		{
			updateRA = retrunitem->updatecondition & EMVTAGUPDATECONDITION_RA;
		}
	}
	else
	{
		memcpy(retrunitem, item, sizeof(EMVBASETAGCVLITEM));
	}

	if(updateRA)
	{
		ret = 1;
	}

	if(Redundantflag)
	{
		rundatabuflen = tempApp_UnionStruct->EMVTradeParam->RedundantDataLen;

		if(tempApp_UnionStruct->EMVTradeParam->RedundantData == NULL)
		{
			tempApp_UnionStruct->EMVTradeParam->RedundantDataMaxLen = 256;
			tempApp_UnionStruct->EMVTradeParam->RedundantData = (unsigned char *)emvbase_malloc(256);
			if(tempApp_UnionStruct->EMVTradeParam->RedundantData)
			{
				memset(tempApp_UnionStruct->EMVTradeParam->RedundantData,0,256);
			}
		}

		if(rundatabuflen && tempApp_UnionStruct->EMVTradeParam->RedundantData)
		{
			if(EMVBase_CheckifRedundantData(temptag, tempApp_UnionStruct->EMVTradeParam->RedundantData, rundatabuflen))
			{
				ret = 0;
			}
		}
		if(tempApp_UnionStruct->EMVTradeParam->RedundantData)
		{
			if((temptaglen + rundatabuflen) < tempApp_UnionStruct->EMVTradeParam->RedundantDataMaxLen)
			{
				memcpy(&tempApp_UnionStruct->EMVTradeParam->RedundantData[rundatabuflen], temptag, temptaglen);
				tempApp_UnionStruct->EMVTradeParam->RedundantDataLen = (rundatabuflen + temptaglen);
			}
			else
			{
				if(tempApp_UnionStruct->EMVTradeParam->RedundantDataMaxLen < 512)
				{
					unsigned char *p = (unsigned char *)emvbase_malloc(512);
					if(p)
					{
						memset(p,0,512);
						memcpy(p,tempApp_UnionStruct->EMVTradeParam->RedundantData,rundatabuflen);
						if(tempApp_UnionStruct->EMVTradeParam->RedundantData)
						{
							emvbase_free(tempApp_UnionStruct->EMVTradeParam->RedundantData);
						}
						tempApp_UnionStruct->EMVTradeParam->RedundantDataMaxLen = 512;
						tempApp_UnionStruct->EMVTradeParam->RedundantData = p;
						memcpy(&tempApp_UnionStruct->EMVTradeParam->RedundantData[rundatabuflen], temptag, temptaglen);
						tempApp_UnionStruct->EMVTradeParam->RedundantDataLen = (rundatabuflen + temptaglen);
					}
				}
			}
		}
	}

	return ret;
}


unsigned char pure_ParseAndStoreCardResponse(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char *DataOut, unsigned short *DataOutIndex, unsigned char *parsebInTable, unsigned char readstep)
{
    //unsigned char k;
    unsigned short index, len, tagdatalen;
    //unsigned char ifemvtag;  //tmpdata,tagindex,tmpval,addvalue,
    unsigned char bInTable;
    unsigned char tag[4], taglen;
    unsigned char ret;
    //unsigned char needdonextstep = 1;
    EMVBASETAGCVLITEM tagitem;

#ifdef EMVB_DEBUG
    EMVBASETAGCVLITEM *item;
    unsigned char i;
#endif

    index = *DataOutIndex;
    bInTable = 0;


    if(DataOut[index] == 0xFF || DataOut[index] == 0x00)
    {
        index++;
        *DataOutIndex = index;
        *parsebInTable = bInTable;

        return RLT_EMV_OK;
    }
    memset(tag, 0, sizeof(tag));
    taglen = 0;
    tag[taglen++] = DataOut[index];

    if((tag[0] & 0x1f) == 0x1f)
    {
        tag[taglen++] = DataOut[index + 1];

        if(tag[1] & 0x80)
        {
            tag[taglen++] = DataOut[index + 2];
        }
    }

	EMVBase_Trace("PURE-info: Tag%02X%02X parsing\r\n", tag[0], tag[1]);
    ret = pure_checkifwrongtag(tempApp_UnionStruct, tag, taglen, &tagitem, readstep);
	EMVBase_Trace("PURE-info: pure_checkifwrongtag ret = %d\r\n", ret);

    if(ret == 0)
    {
    	EMVBase_Trace("PURE-error: Tag already existed\r\n");
        return RLT_ERR_EMV_IccDataRedund;
    }
    else if(ret == 2)
    {
       	if(memcmp(tag, "\x9F\x02", 2)== 0)
    	{
			if(taglen != 6)
			{
				EMVBase_Trace("tag 9F02 length error!\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}
		}
    }
    else
    {
        index += taglen;

        if(EMVBase_ParseExtLen(DataOut, &index, &len))
        {
            EMVBase_Trace("PURE-error: Tag parsing error\r\n");
            return RLT_ERR_EMV_IccDataFormat;
        }
        tagdatalen = len;

        if(len)
        {
            if(len > tagitem.maxlen || len < tagitem.minlen)
            {
            	EMVBase_Trace("PURE-info: Tag value len range: min(%d)-max(%d)\r\n", tagitem.minlen, tagitem.maxlen);
				EMVBase_Trace("PURE-info: Tag value len in card: %d\r\n", len);

                if(readstep == 0)
                {
                    if(memcmp(tag, EMVTAG_AppLabel, taglen) == 0 || memcmp(tag, EMVTAG_AppPreferName, taglen) == 0 ||
                       memcmp(tag, EMVTAG_ICTI, taglen) == 0 || memcmp(tag, EMVTAG_LangPrefer, taglen) == 0)
                    {
                        if(len > tagitem.maxlen)
                        {
                            tagdatalen = tagitem.maxlen;
                        }
                    }
                    else
                    {
                    	EMVBase_Trace("PURE-error: Tag value len error\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                }
                else
                {
                	EMVBase_Trace("PURE-error: Tag value len error\r\n");
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }
        }

		EMVBase_TraceHex("PURE-info: create Tag:", tag, 3);
		EMVBase_TraceHex("PURE-info: Tag Value:", &DataOut[index], tagdatalen);
        emvbase_avl_createsettagvalue(tag, &DataOut[index], tagdatalen);

        index += len;
        bInTable = 1;
    }

    if(bInTable == 0)
    {
        index += taglen;

        if(EMVBase_ParseExtLen(DataOut, &index, &len))
        {
			EMVBase_Trace("PURE-error: Tag parsing error\r\n");
            return RLT_ERR_EMV_IccDataFormat;
        }
        index += len;

        bInTable = 1;
    }

    *DataOutIndex = index;
    *parsebInTable = bInTable;

    return RLT_EMV_OK;
}




/*******************************************************************
   ��    ��: sxl
   ��������: ����SELECT����صĿ�����
   ��ڲ���: selectRet - ���ļ�ѡ��Ӧ��ṹ��
          DataOut -  �����ص�����
          LenOut -   �����ص����ݳ���
   �� �� ֵ: EMV_OK��0��-�ɹ�
          ERR_EMV_IccDataFormat��34�� - IC�����ݸ�ʽ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:    sxl0221
 ********************************************************************/
unsigned char pure_FinalReadSelectRetData(EMVBASE_SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, PURETradeUnionStruct *tempAppUnionStruct)
{
    unsigned char k;
    unsigned short index, indexFCI, indexFCIProp, len, lenFCI, lenFCIProp, templen;
    unsigned short indexIssuerDiscret;
    unsigned char ret;
    unsigned char bIntable;
    EMVBASETAGCVLITEM *emvitem,*emvitem1;
    unsigned char *temp;
	unsigned char CurProcessIndicator = tempAppUnionStruct->EMVTradeParam->CurProcessIndicator;


    index = 0;
    if(DataOut[index] != 0x6F)
    {
    	EMVBase_Trace("PURE-error: FCI template error(r-apdu first byte != 6F)\r\n");
        return RLT_ERR_EMV_IccDataMissing;         //FCI template miss
    }
    index++;

    if(EMVBase_ParseExtLen(DataOut, &index, &lenFCI))
    {
    	EMVBase_Trace("PURE-error: FCI template(Tag6F) parsing error\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

	if(TRANS_NEW == CurProcessIndicator)
    {
    	EMVBase_Trace("PURE-info: This is a new trans\r\n");
        memset(tempAppUnionStruct->EMVTradeParam->FCIIn1stSelect, 0, sizeof(tempAppUnionStruct->EMVTradeParam->FCIIn1stSelect));
		tempAppUnionStruct->EMVTradeParam->FCIIn1stSelectLen = lenFCI;
    	EMVBase_Trace("PURE-info: store FCI template(Tag6F) len = %d\r\n", lenFCI);
		memcpy(tempAppUnionStruct->EMVTradeParam->FCIIn1stSelect, &DataOut[index], tempAppUnionStruct->EMVTradeParam->FCIIn1stSelectLen);
		EMVBase_TraceHex("PURE-info: store FCI template(Tag6F)", tempAppUnionStruct->EMVTradeParam->FCIIn1stSelect, tempAppUnionStruct->EMVTradeParam->FCIIn1stSelectLen);
	}
	else if(TRANS_ONLINE_RESPONSE == CurProcessIndicator || TRANS_TORN_RECOVERGAC == CurProcessIndicator)
	{
		if(!memcmp(tempAppUnionStruct->EMVTradeParam->FCIIn1stSelect, &DataOut[index], tempAppUnionStruct->EMVTradeParam->FCIIn1stSelectLen) || (lenFCI != tempAppUnionStruct->EMVTradeParam->FCIIn1stSelectLen))
		{
			tempAppUnionStruct->EMVTradeParam->FCIDifferFlag = 1;
		}
	}

    indexFCI = index;

    while(index < indexFCI + lenFCI)
    {
    	EMVBase_Trace("PURE-info: sub Tags in FCI template(Tag6F) parsing\r\n");

        if(index >= LenOut)
		{
        	EMVBase_Trace("PURE-error: r-apdu parsing error\r\n");
			EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
        else if(DataOut[index] == 0x84)         //DF��
        {
			EMVBase_Trace("PURE-info: DF Name(Tag84) parsing\r\n");

            if(selectRet->DFNameExist == 1)
			{
            	EMVBase_Trace("PURE-error: DF Name(Tag84) already existed\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &templen))
            {
            	EMVBase_Trace("PURE-error: DF Name(Tag84) parsing error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }

            selectRet->DFNameLen = templen;

			if(selectRet->DFNameLen == 0)
			{
				EMVBase_Trace("PURE-error: DF Name(Tag84)'s length is zero!\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}


            if(selectRet->DFNameLen > 16)
			{
            	EMVBase_Trace("PURE-error: DF Name(Tag84)'s len(%d) > 16\r\n", selectRet->DFNameLen);
                return RLT_ERR_EMV_IccDataFormat;
            }
            memcpy(selectRet->DFName, DataOut + index, selectRet->DFNameLen);
            selectRet->DFNameExist = 1;

            index += templen;
            emvbase_avl_createsettagvalue(EMVTAG_DFName, selectRet->DFName, selectRet->DFNameLen);
        }
        else if(DataOut[index] == 0xA5)         //FCI����ר��ģ��
        {
        	EMVBase_Trace("PURE-info: FCI Proprietary Template(TagA5) parsing\r\n");

            selectRet->FCIPropExist = 1;
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &lenFCIProp))
            {
            	EMVBase_Trace("PURE-error: FCI Proprietary Template(TagA5) parsing error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexFCIProp = index;

			emvbase_avl_deletetag(EMVTAG_AppLabel);//SELECT DF must contain 50
			emvbase_avl_deletetag(EMVTAG_PDOL);

            while(index < indexFCIProp + lenFCIProp)
            {
            	EMVBase_Trace("PURE-info: sub Tags in FCI Proprietary Template(TagA5) parsing\r\n");

                ret = pure_ParseAndStoreCardResponse(tempAppUnionStruct, DataOut, &index, &bIntable, 0);

                if(ret != RLT_EMV_OK)
                {
					if(ret == RLT_ERR_EMV_IccDataRedund)
					{
						tempAppUnionStruct->SetOutcome(SDK_OUTCOME_RESULT_SELECTNEXT, SDK_OUTCOME_START_C, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
						tempAppUnionStruct->SendOutcome();
						return RLT_EMV_APPSELECTTRYAGAIN;
					}
					EMVBase_Trace("PURE-info: sub Tags in FCI Proprietary Template(TagA5) parsing error\r\n");
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }

            if(index != indexFCIProp + lenFCIProp)
            {
				EMVBase_Trace("PURE-error: FCI Proprietary Template(TagA5)'s value parsing error\r\n");
				EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }


			emvitem = emvbase_avl_gettagitempointer(EMVTAG_AppLabel);
			if(emvitem != NULL)
			{
				emvitem1 = emvbase_avl_gettagitempointer(EMVTAG_PDOL);
				if(emvitem1 == NULL)
				{
					EMVBase_Trace("PURE-error: PDOL(Tag9F38) missing\r\n");
					return RLT_ERR_EMV_IccDataFormat;
				}
			}
			else
			{
				EMVBase_Trace("PURE-error: App Label(Tag50) missing\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}

			emvitem = emvbase_avl_gettagitempointer(EMVTAG_PDOL);//
			if(emvitem != NULL)
			{
				if(emvitem->len == 0)
				{
					EMVBase_Trace("PURE-error: PDOL(Tag9F38) missing\r\n");
					return RLT_ERR_EMV_IccDataFormat;
				}
			}

            //BF0C
            indexIssuerDiscret = 0;
            emvitem = emvbase_avl_gettagitempointer(EMVTAG_FCIDisData);

            if(emvitem != NULL)
            {
            	EMVBase_Trace("PURE-info: sub Tags in FCI Issuer Discretionary Data(TagBF0C) parsing\r\n");

				if(!emvitem->len)
				{
					EMVBase_Trace("PURE-error: FCI Issuer Discretionary Data(TagBF0C)'s len is 0\r\n");
					return RLT_ERR_EMV_IccDataFormat;
				}
                temp = (unsigned char *)emvbase_malloc(300);
                memset(temp, 0, 300);
                memcpy(temp, emvitem->data, emvitem->len);

                while(indexIssuerDiscret < emvitem->len)
                {
                    ret = pure_ParseAndStoreCardResponse(tempAppUnionStruct, temp, &indexIssuerDiscret, &bIntable, 0);

                    if(ret != RLT_EMV_OK)
                    {
                    	EMVBase_Trace("PURE-error: sub Tags in FCI Issuer Discretionary Data(TagBF0C) parsing error\r\n");
                        emvbase_free(temp);
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                }

                emvbase_free(temp);

                if(indexIssuerDiscret != emvitem->len)
                {
                	EMVBase_Trace("PURE-error: FCI Issuer Discretionary Data(TagBF0C)'s value parsing error\r\n");
					EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
                    return RLT_ERR_EMV_IccDataFormat;
                }
                emvbase_avl_deletetag(EMVTAG_FCIDisData);
            }
        }
        else
        {
            k = DataOut[index];
			EMVBase_Trace( "k=%02x\r\n", k);
            if((k & 0x1F) == 0x1F)
            {
            	EMVBase_Trace("PURE-info: Tag%02x%02x parsing\r\n", DataOut[index], DataOut[index+1]);
                index++;
            }
			else
			{
				EMVBase_Trace("PURE-info: Tag%02x parsing\r\n", DataOut[index]);
			}
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &len))
            {
            	EMVBase_Trace("PURE-error: Tag parsing error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
            index += len;
        }
    }

    if(index != indexFCI + lenFCI)
    {
    	EMVBase_Trace("PURE-error: FCI template(Tag6F)'s value parsing error\r\n");
		EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }
    return RLT_EMV_OK;
}

/*******************************************************************
   ��    ��: sxl
   ��������: unsigned char Paywave_FinalSelectRetData(AppUnionStruct *tempApp_UnionStruct)
   ��������: ����Paywaveѡ��Ӧ�÷�������
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:         sxl0221
 ********************************************************************/
unsigned char  pure_FinalSelectRetData(EMVBASE_APDU_RESP *apdu_r, PURETradeUnionStruct *tempApp_UnionStruct)
{
    EMVBASE_SELECT_RET selectRet;
    EMVBASE_LISTAPPDATA *tempselectedapp;
    EMVBASE_LISTAPPDATA *tempappdata;
    unsigned char tempselectedappno;
    unsigned char *tempappnum;
    unsigned char tempaidlen;
	unsigned char retCode;
	unsigned char CurProcessIndicator = tempApp_UnionStruct->EMVTradeParam->CurProcessIndicator;

    tempappnum = &(tempApp_UnionStruct->EMVTradeParam->AppListCandidatenum);
    tempselectedapp = tempApp_UnionStruct->EMVTradeParam->SelectedApp;
    tempappdata = tempApp_UnionStruct->EMVTradeParam->AppListCandidate;
    tempselectedappno = tempApp_UnionStruct->EMVTradeParam->SelectedAppNo;

    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00) //Select OK
    {
        memset((unsigned char*)&selectRet, 0, sizeof(EMVBASE_SELECT_RET));

		retCode = pure_FinalReadSelectRetData(&selectRet, apdu_r->DataOut, apdu_r->LenOut, tempApp_UnionStruct);
		if(retCode != RLT_EMV_OK)
		{
			if(RLT_ERR_EMV_IccDataFormat == retCode)
			{
				tempApp_UnionStruct->EMVTradeParam->FCIParseErrorFlag = 1;
			}
			else if(RLT_ERR_EMV_IccDataMissing == retCode)
			{
				tempApp_UnionStruct->EMVTradeParam->FCIParseErrorFlag = 2;
			}
			else if(RLT_EMV_APPSELECTTRYAGAIN == retCode)
			{
            	EMVBase_Trace("PURE-info: select next aid\r\n");
				return retCode;
			}
		}

        if(selectRet.DFNameExist == 0 || selectRet.FCIPropExist == 0)
        {
        	EMVBase_Trace("PURE-info: DF Name(Tag84) or FCI Proprietary Template(TagA5) missing\r\n");

            if(*tempappnum > 1)
            {
            	EMVBase_Trace("PURE-info: select next aid\r\n");
                return RLT_EMV_APPSELECTTRYAGAIN;
            }
            else
            {
				EMVBase_Trace("PURE-error: no more aid\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
        }

		if(CurProcessIndicator == TRANS_NEW)
        {
            tempaidlen = (tempappdata + tempselectedappno)->AIDLen;

            if(tempaidlen > selectRet.DFNameLen)
            {
                tempaidlen = selectRet.DFNameLen;
            }
            memcpy(tempselectedapp->AID, (tempappdata + tempselectedappno)->AID, (tempappdata + tempselectedappno)->AIDLen);
            tempselectedapp->AIDLen = (tempappdata + tempselectedappno)->AIDLen;
        }
    }
    else    //current app selected fail,delete it from app list and select again.
    {
		if(TRANS_ONLINE_RESPONSE == CurProcessIndicator)
		{
			EMVBase_Trace("PURE-error: In online response Select SW != 9000\r\n");
			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			return RLT_EMV_TERMINATE_TRANSERR;
		}
		else if(TRANS_NEW == CurProcessIndicator)
		{
			if(*tempappnum > 1)
			{
				EMVBase_Trace("PURE-info: select next aid\r\n");
				return RLT_EMV_APPSELECTTRYAGAIN;
			}
		}
	}
    return PURE_READAIDPARAMETERS;
}

unsigned char pure_RestrictListAID(PURETradeUnionStruct *tempApp_UnionStruct)
{
    EMVBASE_LISTAPPDATA *tempAppAppData;
    unsigned char tempselectappno;

    tempAppAppData = tempApp_UnionStruct->EMVTradeParam->AppListCandidate;
    tempselectappno = tempApp_UnionStruct->EMVTradeParam->SelectedAppNo;



}

unsigned char pure_FinalSelect(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char tempselectappno;
    EMVBASE_LISTAPPDATA *tempAppAppData;
	EMVBASETAGCVLITEM *TermAIDSupItem;
	int ret,i;
	static int ReadAIDParamFlag=0;
	unsigned char stopAfCmdFlag = 0;
	int MatchTermFlag=0,MatchCardFlag=0;

    tempAppAppData = tempApp_UnionStruct->EMVTradeParam->AppListCandidate;
    tempselectappno = tempApp_UnionStruct->EMVTradeParam->SelectedAppNo;

	EMVBase_Trace("pure-info: PureCandidateListEmptyFlag = %d\r\n", tempApp_UnionStruct->EMVTradeParam->PureCandidateListEmptyFlag);

	if(tempApp_UnionStruct->EMVTradeParam->PureCandidateListEmptyFlag)
	{
		tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
		tempApp_UnionStruct->SendOutcome();
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_ERROR_TRYOTHERCARD, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
		tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
		return RLT_EMV_TERMINATE_TRANSERR;
	}

	EMVBase_Trace("pure-info: Candidate List Num= %d\r\n", tempApp_UnionStruct->EMVTradeParam->AppListCandidatenum);
	EMVBase_Trace("pure-info: Kernel Support Impletion 5 = %d\r\n", tempApp_UnionStruct->EMVTradeParam->PureImplementationOption & 0x08);

	if(tempApp_UnionStruct->EMVTradeParam->PureImplementationOption & 0x08)
	{
		TermAIDSupItem = emvbase_avl_gettagitempointer(EMVTAG_PURETAID);	//if kernel support Implementation 5 and terminal AID support value is not lowest priority in candidate list,then replace terminal AID support  of highest AID in candidate list
		if(TermAIDSupItem)
		{
			MatchCardFlag = 0;
			MatchTermFlag = tempApp_UnionStruct->CheckRestrictAID(TermAIDSupItem->data, TermAIDSupItem->len);
			for(i = 0; i < tempApp_UnionStruct->EMVTradeParam->AppListCandidatenum; i++)
			{
				if(!memcmp((tempAppAppData+i)->AID, TermAIDSupItem->data, TermAIDSupItem->len))
				{
					MatchCardFlag = 1;
					break;
				}
			}
			if(MatchTermFlag && MatchCardFlag)
			{
				tempselectappno = i;
			}
			else
			{

			}
		}

		EMVBase_Trace("pure-info: after restrict AID tempselectappno = %d\r\n", tempselectappno);
	}

	EMVBase_Trace("pure-info: start preprocess in kernel\r\n");
	ret = tempApp_UnionStruct->Preprocess(tempApp_UnionStruct->EMVTradeParam);
	EMVBase_Trace("pure-info: Preprocess ret = %d\r\n", ret);
	if(-1 == ret)
	{
		stopAfCmdFlag = 1;
	}
	else if(PURE_REQ_LOADAIDPARAM == ret)
	{
		return PURE_REQ_LOADAIDPARAM;
	}
//	else if(PURE_ERR_NO9C == ret)
//	{
//		return
//	}
	else
	{
		if(tempApp_UnionStruct->EMVTradeParam->PreprcessIndicator & 0x08)	//Amount >= Contactless Trans limit
		{
			//FIXME: need to add Outcome dealing
			if(tempApp_UnionStruct->EMVTradeParam->AppListCandidatenum > 1)
			{
				tempselectappno++;
			}
//			return RLT_EMV_APPSELECTTRYAGAIN;
		}
		if(tempApp_UnionStruct->EMVTradeParam->PreprcessIndicator & 0x10)
		{
			EMVBase_Trace("PURE-info: set TVR byte4 bit8 = 1");
			emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80);
		}
	}

	EMVBase_TraceHex("pure-info: AID in Command: ", (tempAppAppData + tempselectappno)->AID, (tempAppAppData + tempselectappno)->AIDLen);
    EMVBase_COMMAND_SELECT((tempAppAppData + tempselectappno)->AID, (tempAppAppData + tempselectappno)->AIDLen, 0, &apdu_s);
    tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
		EMVBase_Trace("PURE-error: apdu timeout or other error\r\n");
		EMVBase_Trace("PURE-info: Redo PPSE\r\n");
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

	if(stopAfCmdFlag)
	{
		return RLT_EMV_TERMINATE_TRANSERR;
	}
    retCode = pure_FinalSelectRetData(&apdu_r, tempApp_UnionStruct);
	EMVBase_Trace("PURE-info: pure_FinalSelectRetData ret = %d\r\n", retCode);

    return retCode;
}

unsigned char pure_FindTagFromPDOL(unsigned char *pheTag)
{
	int tag_len;
	unsigned int uiIndex;
	unsigned char i, k, bInTable;
	EMVBASETAGCVLITEM *PDOLdataitem = NULL;
	unsigned short PDOLLen;
	unsigned char *PDOLData;

	uiIndex = 0;

	PDOLdataitem = emvbase_avl_gettagitempointer(EMVTAG_PDOL);

	if(NULL == pheTag || PDOLdataitem == NULL)
	{
		return 0;
	}
	tag_len = 1;

	if((0x1F & pheTag[0]) == 0x1F)
	{
		tag_len = 2;
	}
	PDOLLen = PDOLdataitem->len;
	PDOLLen = (PDOLLen>255) ? 255 :(PDOLLen);

	PDOLData = (unsigned char *)emvbase_malloc(255);
	memcpy(PDOLData, PDOLdataitem->data, PDOLLen);

	while(uiIndex < PDOLLen)	 //Process PDOL
	{
		if(PDOLData[uiIndex] == 0xFF || PDOLData[uiIndex] == 0x00)
		{
			uiIndex++; continue;
		}
		//bInTable = 0;
		k = PDOLData[uiIndex];
		if(memcmp(&PDOLData[uiIndex], pheTag, tag_len) == 0)
		{
			emvbase_free(PDOLData);
			return 1;
		}

		if((k & 0x1F) == 0x1F)
		{
			uiIndex++;
		}
		uiIndex += 2;
	}

	emvbase_free(PDOLData);
	return 0;
}

unsigned char pure_CheckGACRespMandatoryDataMiss(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char CID,CCID;
	unsigned char CCIDExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);


	CID = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
	if(CCIDExist)
	{
		CCID = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
	}

	if(0x00 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator)	//CDA not requested
	{
		if(!emvbase_avl_checkiftagexist(EMVTAG_AppCrypt))
		{
			EMVBase_Trace("pure-error: miss 9F26\r\n");
			return 1;
		}
	}

	if(CCIDExist)	//AAC card decision with CDA requested
	{
		if((0x00 == (CID & 0xC0)) && (0x00 == (CCID & 0xC0)) && (0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator))
		{
			if(!emvbase_avl_checkiftagexist(EMVTAG_AppCrypt) || !emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData))
			{
				EMVBase_Trace("pure-error: miss 9F26 or 9F4B\r\n");
				return 1;	//miss 9F10 or 9F4B
			}
		}
	}
	else
	{
		if((0x00 == (CID & 0xC0)) && (0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator))
		{
			if(!emvbase_avl_checkiftagexist(EMVTAG_AppCrypt) || !emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData))
			{
				EMVBase_Trace("pure-error: miss 9F26 or 9F4B\r\n");
				return 1;	//miss 9F10 or 9F4B
			}
		}
	}

	if(CCIDExist)	//TC card decision or ARQC card decision with CDA requested
	{
		if((((0x40 == (CID & 0xC0)) && (0x40 == (CCID & 0xC0))) || ((0x80 == (CID & 0xC0)) && (0x80 == (CCID & 0xC0)))) && (0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator))
		{
			if(!emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData))
			{
				EMVBase_Trace("pure-error: miss 9F4B\r\n");
				return 1;	//miss 9F4B
			}
		}
	}
	else
	{
		if(((0x40 == (CID & 0xC0)) || (0x80 == (CID & 0xC0))) && (0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator))
		{
			if(!emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData))
			{
				EMVBase_Trace("pure-error: miss 9F4B\r\n");
				return 1;	//miss 9F4B
			}
		}
	}
	EMVBase_Trace("pure-info: Check Over\r\n");
	return 0;
}

unsigned char pure_DealTornEchoResponse(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char *EchoData = tempApp_UnionStruct->EMVTradeParam->ECHORespData;
	unsigned char AppCryptExist, CryptInfoExist, ATCExist,SDADExist,IssuAuthDataExist,CCIDExist, CCIDInGPOExist;
	unsigned char ret;
	unsigned short index,indexTemp,lenTemp,orgindex,indexRet;
	unsigned char bIntable;
    EMVBASETAGCVLITEM *item;
	unsigned short LenOut = tempApp_UnionStruct->EMVTradeParam->ECHORespDataLen;
	unsigned char GenerateACRetDataflag;
	unsigned char TermAnaResult;
	unsigned char CCIDInECHO,CID;
	unsigned char CVMRes[3];

	//K14.3
	if(0x77 != EchoData[0])
	{
		EMVBase_Trace("PURE-error: r-apdu template error (first byte != 0x77)\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

    index=0;

	EMVBase_Trace("PURE-info: GAC template 77\r\n");
	index++;
	if(EMVBase_ParseExtLen(EchoData,&index,&lenTemp))
	{
		EMVBase_Trace("PURE-info: Tag77 parsing error\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}
	indexTemp=index;
	indexRet=0;
	while(index<indexTemp+lenTemp)
	{
		if(index>= LenOut)
		{
			EMVBase_Trace("PURE-info: r-apdu parsing error\r\n");
			EMVBase_Trace("PURE-info: sub Tags' len error\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}
		if(EchoData[index]==0xFF || EchoData[index]==0x00)
		{
			index++;
			continue;
		}

        orgindex = index;
		bIntable = 0;
		GenerateACRetDataflag = 1;

		if(!memcmp(&EchoData[index],"\x9F\x4B",2))
		{
			GenerateACRetDataflag = 0;
		}
		if(0xC5 == EchoData[index])
		{
			CCIDInECHO = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
			emvbase_avl_deletetag(EMVTAG_PURECCID);
		}

		ret = pure_ParseAndStoreCardResponse(tempApp_UnionStruct,EchoData,&index,&bIntable,0);
		if(ret != RLT_EMV_OK)
		{
			EMVBase_Trace("PURE-info: sub Tags in Tag77 parsing error\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}

		if(GenerateACRetDataflag&&(indexRet+index-orgindex <=255))
		{
			memcpy(tempApp_UnionStruct->EMVTradeParam->GenerateACRetData+indexRet,(unsigned char*)&EchoData[orgindex],index-orgindex);
			indexRet+=index-orgindex;
		}

	}	//endwhile
	if(index!=indexTemp+lenTemp)
	{
		EMVBase_Trace("PURE-info: Tag77's value parsing error\r\n");
		EMVBase_Trace("PURE-info: sub Tags' len error\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	IssuAuthDataExist =	emvbase_avl_checkiftagexist(EMVTAG_IssuAppData);//9F10
	AppCryptExist = emvbase_avl_checkiftagexist(EMVTAG_AppCrypt);//9F26
	CryptInfoExist = emvbase_avl_checkiftagexist(EMVTAG_CryptInfo);//9F27
	ATCExist = emvbase_avl_checkiftagexist(EMVTAG_ATC);//9F36
	SDADExist = emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData);//9F4B
	CCIDExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);//C5

	if((CryptInfoExist == 0)  || (ATCExist == 0) || (IssuAuthDataExist == 0))
	{
		if(0 == CryptInfoExist)
		{
			EMVBase_Trace("PURE-info: CID(Tag9F27) missing\r\n");
		}
		if(0 == ATCExist)
		{
			EMVBase_Trace("PURE-info: ATC(Tag9F36) missing\r\n");
		}
		if(0 == IssuAuthDataExist)
		{
			EMVBase_Trace("PURE-info: Issuer Auth Data(Tag9F10) missing\r\n");
		}
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	ret = pure_CheckGACRespMandatoryDataMiss(tempApp_UnionStruct);
	if(ret)
	{
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	if((1 == tempApp_UnionStruct->EMVTradeParam->CCIDExistInGPO) && (!emvbase_avl_checkiftagexist(EMVTAG_PURECCID)))
	{
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	TermAnaResult = pure_CompareIACTAC(tempApp_UnionStruct);

	//K14.5
	emvbase_avl_gettagvalue_spec(EMVTAG_CVMResult, CVMRes, 0, 3);
	CID = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
	CCIDInECHO = emvbase_avl_gettagvalue(EMVTAG_PURECCID);

	if(((CVMRes[0] == 0x24 || CVMRes[0] == 0x64)) && (0x00 == CVMRes[2]) && (0x00 == CID & 0xC0) && (0 == CCIDInGPOExist))
	{
		return pure_Outcome_FinancialTransactionCompleted_OndeviceCVMRequested(tempApp_UnionStruct);
	}
	if(emvbase_avl_checkiftagexist(EMVTAG_PURECCID) && (0x04 == (CCIDInECHO & 0x07)))
	{
		return pure_Outcome_FinancialTransactionCompleted_OndeviceCVMRequested(tempApp_UnionStruct);
	}

	if((0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator) && (0x00 == (CID & 0xC0)) && (0 == SDADExist))
	{
		tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x00;
	}

	if(CCIDInGPOExist && (CCIDInECHO & 0x20))	//CCID indicate Switch to contact interface
	{
		return pure_Outcome_FinancialTransactionCompleted_TryOtherInterface(tempApp_UnionStruct);
	}
	else
	{
		if((TAA_OFFLINE == TermAnaResult) && (0x00 == (CID & 0xC0)))
		{
			return pure_Outcome_FinancialTransactionCompleted_TryOtherInterface(tempApp_UnionStruct);
		}
	}

	return RLT_EMV_OK;
}

unsigned char pure_DealTornECHOCommand(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
    tempApp_UnionStruct->EMVTradeParam->ECHORespDataLen = 0;
	memset(tempApp_UnionStruct->EMVTradeParam->ECHORespData,0,255);

	if(!(apdu_r->SW1==0x90 && apdu_r->SW2==0x00))
	{
		EMVBase_Trace("PURE-error: ECHO SW != 9000\r\n");
		tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator = 0;
		tempApp_UnionStruct->EMVTradeParam->Error = tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator;
		return RLT_ERR_EMV_IccReturn;	//FIXME:just personal thinking,maybe not invalid
	}
	else
	{
		tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator = 1;
		tempApp_UnionStruct->EMVTradeParam->ECHORespDataLen = apdu_r->LenOut;
		memcpy(tempApp_UnionStruct->EMVTradeParam->ECHORespData, apdu_r->DataOut, apdu_r->LenOut);
		EMVBase_TraceHex("PURE-info: ECHO response", apdu_r->DataOut, apdu_r->LenOut);
	}

	return RLT_EMV_OK;
}

unsigned char pure_TronECHOProcess(PURETradeUnionStruct *tempApp_UnionStruct)
{
	EMVBASE_APDU_RESP apdu_r;
	EMVBASE_APDU_SEND apdu_s;
	unsigned char retCode;


	memcpy(apdu_s.Command, "\x80\xDF", 2);
	apdu_s.Command[2] = tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator;	//P1
	EMVBase_Trace("PURE-info: ECHO command P1:%02X\r\n", apdu_s.Command[2]);
	apdu_s.Command[3] = 0x00;	//P1

	if(apdu_s.Command[2] == 0x01)
	{
		unsigned char UN[4]={0};
		emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UN, 0, 4);
		memcpy(apdu_s.DataIn, UN, 4);
	}
	else if(apdu_s.Command[2] == 0x02)
	{
		EMVBASETAGCVLITEM *item = NULL;
		item = emvbase_avl_gettagitempointer(EMVTAG_IssuerAuthenData);
		if(item)
		{
			memcpy(apdu_s.DataIn, item->data, item->len);
		}
	}

	apdu_s.Lc = 0x00;
	apdu_s.Le = 256;
	apdu_s.EnableCancel = 0x01;

	tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

	if(apdu_r.ReadCardDataOk == RLT_EMV_OK)
	{
		retCode = pure_DealTornECHOCommand(tempApp_UnionStruct, &apdu_r);
		EMVBase_Trace( "Pure_DealTornECHOCommand retcode=%d\r\n", retCode);
		return retCode;
	}
	else
	{
		tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION_COMMUNICATIONERROR, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
		tempApp_UnionStruct->SendOutcome();
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
		tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
		tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);
		return PURE_ERR_READCARDAGAIN;
	}

}


//process after select command before gpo
unsigned char pure_FinalSelectedApp(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;

    retCode = pure_FinalSelect(tempApp_UnionStruct);
	EMVBase_Trace("PURE-info: pure_FinalSelect ret = %d\r\n", retCode);
	return retCode;
}

unsigned char pure_PDOLProcess(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char *pdolData;
    unsigned short index, indexOut;
    unsigned char i, k, m, bInTable;
    unsigned char len;
    EMVBASETAGCVLITEM *item;
    unsigned char *PDOL;
    unsigned char tag[3], taglen;
    EMVBASETAGCVLITEM *pdoldataitem = NULL;
    unsigned char tempTermTransPredicable;
    unsigned short templen, tempPDOLlen;
    unsigned char PDOLbexist = 0;
    unsigned char *pdata, *buf;
	unsigned short DOLDataMaxLen = 255;//20201217 lsy
	unsigned char errflag = 0;

    item = emvbase_avl_gettagitempointer(EMVTAG_PDOL);

    if(item != NULL)
    {
        if(item->len)
        {
            PDOLbexist = 1;
        }
    }

    if(PDOLbexist == 0)
    {
        memcpy(apdu_s->Command, "\x80\xA8\x00\x00", 4);
        apdu_s->Lc = 2;
        memcpy(apdu_s->DataIn, "\x83\x00", 2);
        apdu_s->Le = 256;
        apdu_s->EnableCancel = 1;

        return RLT_EMV_OK;
    }

    pdata = (unsigned char *)emvbase_malloc(1024);
    pdolData = &pdata[0];
    PDOL = &pdata[300];
    buf = &pdata[700];

    memset(pdolData, 0, 255);

    index = 0;
    indexOut = 0;

    memset(PDOL, 0, 300);

    if(item->len > 255)
    {
        tempPDOLlen = 255;
    }
    else
    {
        tempPDOLlen = item->len;
    }
    memcpy(PDOL, item->data, tempPDOLlen);

	errflag = 0;//20201217 lsy
    while(index < tempPDOLlen)  		//Process PDOL
    {
        if(PDOL[index] == 0xFF || PDOL[index] == 0x00) {index++; continue; }
        memset(buf, 0, 255);
        bInTable = 0;

        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = PDOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = PDOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = PDOL[index + 2];
            }
        }

		if((memcmp(tag, "\x9F\x50", 2) < 0) || (memcmp(tag, "\x9F\x7F", 2) > 0) || (!memcmp(tag, "\x9F\x7A", 2)))
		{
        	pdoldataitem = emvbase_avl_gettagitempointer(tag);
        	if(pdoldataitem != NULL)
	        {
				EMVBase_TraceHex("pure-info: PDOL item:", pdoldataitem->Tag, 3);
				EMVBase_TraceHex("pure-info: Item data:", pdoldataitem->data, pdoldataitem->len);
				index += taglen;

	            if(EMVBase_ParseExtLen(PDOL, &index, &templen))
	            {
	                emvbase_free(pdata);
	                return RLT_ERR_EMV_IccDataFormat;
	            }
	            k = templen;
	            m = pdoldataitem->len;
				if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
				{
					errflag = 1;
					break;
				}
	            if(pdoldataitem->datafomat & EMVTAGFORMAT_N)	//numeric
	            {
	                if(k >= m)
	                {
	                    if(m)
	                    {
	                        memcpy(&buf[k - m], pdoldataitem->data, m);
	                    }
	                    memcpy(&pdolData[indexOut], buf, k);
	                }
	                else
	                {
	                    if(m)
	                    {
	                        memcpy(buf, pdoldataitem->data, m);
	                    }
	                    memcpy(&pdolData[indexOut], &buf[m - k], k);
	                }
	            }
	            else if(pdoldataitem->datafomat & EMVTAGFORMAT_CN)	//compact numeric
	            {
	                if(m)
	                {
	                    memset(buf, 0xFF, 255);
	                    memcpy(buf, pdoldataitem->data, m);
	                    memcpy(&pdolData[indexOut], buf, k);
	                }
	                else
	                {
	                    memset(buf, 0x00, 255);
	                    memcpy(&pdolData[indexOut], buf, k);
	                }
	            }
	            else	//other formats
	            {
	                if(m)
	                {
	                    memcpy(buf, pdoldataitem->data, m);
	                }
	                memcpy(&pdolData[indexOut], buf, k);
	            }
	            indexOut += k;
	            bInTable = 1;
	        }
		}

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(PDOL, &index, &templen))
            {
                emvbase_free(pdata);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
			if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
			{
				errflag = 1;
				break;
			}
            memcpy(&pdolData[indexOut], buf, k);
            indexOut += k;
        }
    }

	if (errflag)//20201217 lsy
	{
		emvbase_free(pdata);
		EMVBase_Trace("EmvContact_PDOLProcess doldata over maxdollen error !!!!!!\r\n");
		return RLT_EMV_ERR;
	}

    emvbase_avl_createsettagvalue(EMVTAG_PDOLData, pdolData, indexOut);

    memcpy(apdu_s->Command, "\x80\xA8\x00\x00", 4);
    apdu_s->Lc = indexOut + 2;
    len = 0;
    apdu_s->DataIn[len++] = 0x83;

    if((indexOut & 0x80) > 0)
    {
        (apdu_s->Lc)++;
        apdu_s->DataIn[len++] = 0x81;
        apdu_s->DataIn[len++] = indexOut;
    }
    else
    {
        apdu_s->DataIn[len++] = indexOut;
    }
    memcpy((unsigned char*)&apdu_s->DataIn[len], pdolData, indexOut);
	len += indexOut;

    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    emvbase_free(pdata);
    return RLT_EMV_OK;
}

unsigned char pure_DelGPORetData(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
	unsigned char AFL_num = 0;
	EMVBASETAGCVLITEM *AFLitem;
	unsigned char AIPTagExist = 1, AIP[2];
	unsigned char retCode;
	unsigned char ImpleOption;
	unsigned char CCID = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
	unsigned char CCIDExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);
	unsigned char KernelCap[5]={0};
	unsigned char KernelCapExist = 1;

	EMVBase_Trace( "PURE-info: start pure_DelGPORetData\r\n");
	memset(AIP, 0, sizeof(AIP));

    if(emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2))//82
    {
        AIPTagExist = 0;
    }

	if(AIPTagExist == 0)
	{
		EMVBase_Trace("PURE-error: AIP(Tag82) missing\r\n");
		return RLT_ERR_EMV_IccDataMissing;
	}

	EMVBase_Trace( "PURE-info: AIP(Tag82): %02x %02x\r\n", AIP[0], AIP[1]);

	AFLitem = emvbase_avl_gettagitempointer(EMVTAG_AFL);

	if(AFLitem != NULL)
	{
		if(AFLitem->len)
		{
			if(AFLitem->len < 4 || (AFLitem->len % 4 != 0))
			{
				EMVBase_Trace("PURE-error: len of AFL(Tag94) error\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}
		}
		else
		{
			EMVBase_Trace("PURE-error: len of AFL(Tag94) is 0\r\n");
			return RLT_ERR_EMV_IccDataFormat;
		}
	}
	else
	{
		EMVBase_Trace( "PURE-error:AFL(Tag94) missing\r\n");
		return RLT_ERR_EMV_IccDataMissing;
	}

	ImpleOption = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;

	KernelCapExist = emvbase_avl_checkiftagexist(EMVTAG_PUREKernelCap);
	if(KernelCapExist)
	{
		emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	}
	else
	{
		EMVBase_Trace( "PURE-warning:Contactless Application/Kernel Capability(TagDF8308) missing\r\n");
	}

	if(CCIDExist)
	{
		EMVBase_Trace("pure-info: CCID in GPO: %02X\r\n", CCID);
		tempApp_UnionStruct->EMVTradeParam->CCIDExistInGPO = 1;	//if CCID exist in GPO response,then it must exist in 1st GAC response too,otherwise its invalid
		if(CCID & 0x20)	//CCID indicate switch to contact interface
		{
			EMVBase_Trace( "PURE-info: CCID in GPO indicate switch to contact interface\r\n");
			return RLT_ERR_EMV_SWITCHINTERFACE;
		}
		else if((CCID & 0x07) == 0x04)	//CCID indicate Confirmation Code required
		{
			EMVBase_Trace( "PURE-info: CCID in GPO indicate Confirmation Code required\r\n");
			return RLT_ERR_EMV_SEEPHONE;
		}
		else if(((CCID & 0xC0) == 0x00) && (KernelCapExist == 1) && (KernelCap[3] & 0x02))	//transaction declined
		{
			EMVBase_Trace( "PURE-info: CCID in GPO indicate transaction decline\r\n");
			return RLT_EMV_OFFLINE_DECLINE;
		}
	}
	else
	{
		EMVBase_Trace( "PURE-info: CCID dont obtain in GPO Response\r\n");
	}

	return RLT_EMV_OK;
}

unsigned char pure_GPORetData(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
    unsigned short index, indexTemp, len, lenTemp, cplen, j;
    unsigned char k;  //i,
    unsigned char bInTable;
    //unsigned int unknowlen;
    //unsigned char tag;
    unsigned char ret;
	unsigned char TransanctionMode;
	unsigned char TornFlag;

	EMVBase_Trace("PURE-info: GPO response parsing\r\n");


    if((apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00))   //Get Processing Options OK
    {
        index = 0;

        if(apdu_r->DataOut[index] == 0x80)      //primitive data
        {
			EMVBase_Trace("PURE-error: GPO template 80\r\n");	//Pure only support fotmat 2
			return RLT_ERR_EMV_IccDataFormat;
        }
        else if(apdu_r->DataOut[index] == 0x77)      //TLV coded data
        {
			EMVBase_Trace("PURE-info: GPO template 77\r\n");

            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenTemp))
            {
            	EMVBase_Trace("PURE-error: Tag77 parsing error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexTemp = index;

            while(index < indexTemp + lenTemp)
            {
                if(index >= apdu_r->LenOut)
                {
                	EMVBase_Trace("PURE-error: r-apdu parsing error\r\n");
					EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
                    return RLT_ERR_EMV_IccDataFormat;
                }

                if(0xFF == apdu_r->DataOut[index] || 0x00 == apdu_r->DataOut[index])
				{
					index++;
					continue;
				}
                ret = pure_ParseAndStoreCardResponse(tempApp_UnionStruct, apdu_r->DataOut, &index, &bInTable, 1);

                if(ret != RLT_EMV_OK)
                {
					if(ret == RLT_ERR_EMV_IccDataRedund)
					{
						EMVBase_Trace("PURE-error: data element is already present in the Kernel Transaction Database!Section K6.5\r\n");
						return ret;
					}
                	EMVBase_Trace("PURE-error: sub Tags in Tag77 parsing error\r\n");
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }

            if(index != indexTemp + lenTemp)
            {
            	EMVBase_Trace("PURE-error: Tag77's value parsing error\r\n");
				EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
        else
		{
        	EMVBase_Trace("PURE-error: GPO template error (first byte != 77 or 80)\r\n");
            return RLT_ERR_EMV_IccDataFormat;
        }
    }
    else if(apdu_r->SW1 == 0x69 && apdu_r->SW2 == 0x85)
    {
    	EMVBase_Trace("PURE-error: GPO SW == 6985\r\n");
        return RLT_EMV_APPSELECTTRYAGAIN;
    }
    else
	{
		EMVBase_Trace("PURE-error: SW != 9000\r\n");
		if(tempApp_UnionStruct->EMVTradeParam->AppListCandidatenum >1)	//2021.9.22 lishiyao modify for 1.5a
		{
			return RLT_EMV_APPSELECTTRYAGAIN;
		}
		else
		{
			return RLT_EMV_TERMINATE_TRANSERR;
		}
	}
    return RLT_EMV_OK;
}

unsigned char pure_InitialApp(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char tempTermTransPredicable;
    unsigned char AIP[2];
    unsigned char AIPTagExist = 1;
	unsigned char AFLTagExist = 1;
	unsigned char ImpleOption, CombOptions[2];

	retCode = pure_PDOLProcess(tempApp_UnionStruct, &apdu_s);
	EMVBase_Trace("PURE-info: pure_PDOLProcess ret = %d\r\n", retCode);

	emvbase_avl_printtagallvalue("after pure_PDOLProcess 9F66:", EMVTAG_TermTransPredicable);
	emvbase_avl_printtagallvalue("after pure_PDOLProcess 9F1A:", EMVTAG_CountryCode);

	EMVBase_Trace("PURE-info: (For Debug only)before send GPO CCID Exist = %d\r\n", emvbase_avl_checkiftagexist(EMVTAG_PURECCID));

	if(retCode != RLT_EMV_OK)
	{
		if(tempApp_UnionStruct->EMVTradeParam->AppListCandidatenum <= 1)
		{
			tempApp_UnionStruct->EMVTradeParam->PureCandidateListEmptyFlag = 1;
			return pure_Outcome_EndApplication_EmptyCandidateList(tempApp_UnionStruct);
		}
		else
		{
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}
	}

	ImpleOption = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	EMVBase_Trace( "PURE-info: Implementation Option: %02x\r\n", ImpleOption);

	tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

	if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
	{
		EMVBase_Trace("PURE-error: communication problem during the GPO command processing\r\n");
		return pure_Outcome_CommunicationErrors_PaymentTransactionNotCompleted(tempApp_UnionStruct);
	}

	retCode = pure_GPORetData(tempApp_UnionStruct, &apdu_r);

	if(RLT_ERR_EMV_IccDataFormat == retCode || RLT_ERR_EMV_IccReturn == retCode || RLT_ERR_EMV_IccDataRedund == retCode)
	{
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	if(retCode != RLT_EMV_OK && retCode != RLT_ERR_EMV_SEEPHONE)
	{
		return retCode;
	}

	retCode = pure_DelGPORetData(tempApp_UnionStruct, &apdu_r);
	if(retCode != RLT_EMV_OK)
	{
		if(RLT_ERR_EMV_SWITCHINTERFACE == retCode)
		{
			return pure_Outcome_FinancialTransactionCompleted_TryOtherInterface(tempApp_UnionStruct);
		}
		else if(RLT_ERR_EMV_SEEPHONE == retCode)
		{
			return pure_Outcome_FinancialTransactionCompleted_OndeviceCVMRequested(tempApp_UnionStruct);
		}
		else if(RLT_EMV_OFFLINE_DECLINE == retCode)
		{
			return pure_Outcome_FinancialTransactionCompleted_TransactionDeclined(tempApp_UnionStruct);
		}
		else if(RLT_ERR_EMV_IccDataMissing == retCode)
		{
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}
		else
		{
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}
	}

	return retCode;
}

unsigned char pure_DealAFLData(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char temp[50];
    EMVBASETAGCVLITEM *item, *item1, *item2;
    unsigned char TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	unsigned char CDOL1Exist=0,PanExist=0, MTOLExist=0;
	unsigned char Impletion = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	unsigned char KernelCap[5]={0},Tag[3]={0};
	int i;
	unsigned char CVMListExist = emvbase_avl_checkiftagexist(EMVTAG_CVMList);

	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	if((Impletion & 0x10) && (KernelCap[3] & 0x40))
	{
		return RLT_EMV_OK;
	}

	CDOL1Exist = emvbase_avl_checkiftagexist(EMVTAG_CDOL1);//8C
	PanExist = emvbase_avl_checkiftagexist(EMVTAG_Track2Equ);//57

	if(0 == CDOL1Exist || 0 == PanExist)
	{
		if(0 == CDOL1Exist)
		{
			EMVBase_Trace("PURE-error: CDOL1(Tag8C) missing\r\n");
		}
		if(0 == PanExist)
		{
			EMVBase_Trace("PURE-error: Track2(Tag57) missing\r\n");
		}
		//TODO: add dealing of MTOL
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	item2 = emvbase_avl_gettagitempointer(EMVTAG_PUREMTOL);
	if(item2 != NULL)
	{
		for(i = 0; i < item2->len; i+=3)
		{
			if(0x00 == item2->data[i])
			{
				break;
			}
			memcpy(Tag, &(item2->data[i]), 3);
			MTOLExist = emvbase_avl_checkiftagexist(Tag);
			if(0 == MTOLExist)
			{
				EMVBase_Trace("PURE-error: MTOL Item missing\r\n");
				EMVBase_TraceHex("PURE-info: MTOL Item = ", Tag, 3);
				return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
			}
		}
	}

//	if(CVMListExist)
//	{
//		item = emvbase_avl_gettagitempointer(EMVTAG_CVMList);
//        if((item->len % 2 != 0) || (item->len < 10))
//        {
//			EMVBase_Trace("pure-info: CVM List len is not correct. Len is %d\r\n",CVMListitem->len);
//			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);	//ICC Data Missing
//            return RLT_ERR_EMV_IccDataFormat;
//        }
//	}

    return RLT_EMV_OK;
}

unsigned char pure_ReadAFLRetData(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r, unsigned char tempt, unsigned char authdataflag)
{
    unsigned char t;
    unsigned char bInTable;      //i,k,
    unsigned short index, indexAEF, lenAEF;    //len,
    //unsigned char tempreadaflnum;
    //unsigned char tag;
    unsigned char ret;
	unsigned short indexlen, lenTemp, indexTemp, len;
	unsigned char k = 0;


	EMVBase_Trace("PURE-info: apdu response parsing\r\n");

    if(!(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)) // ICC parameter Records found fail.
    {
		EMVBase_Trace("PURE-error: SW != 9000\r\n");
        return RLT_ERR_EMV_IccReturn;
    }
    index = 0;

    t = tempt;
    t >>= 3;

    if((t >= 11) && (t <= 30)) //mofified according to SU Bullitin No.12
    {
        if(authdataflag)
        {
            if(apdu_r->DataOut[index] == 0x70)           //����SFI��11��30�ļ�¼TAG==70�ͼ�¼���������ѻ���֤
            {
				if(tempApp_UnionStruct->EMVTradeParam->AuthData == NULL)
                {
                    tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 0;
                    tempApp_UnionStruct->EMVTradeParam->AuthData = (unsigned char *)emvbase_malloc(512);

                    if(tempApp_UnionStruct->EMVTradeParam->AuthData == NULL)
                    {
                    	EMVBase_Trace("PURE-error: memory error\r\n");
                        return RLT_EMV_ERR;
                    }

                    tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 512;
                    memset(tempApp_UnionStruct->EMVTradeParam->AuthData,0,512);
                }

                if((tempApp_UnionStruct->EMVTradeParam->AuthDataLen + apdu_r->LenOut) > tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen)
                {
                    if(tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen < 1024)
                    {
                        unsigned char *p = (unsigned char *)emvbase_malloc(1024);
                        if(p == NULL)
                        {
                        	EMVBase_Trace("PURE-error: memory error\r\n");
                            return RLT_EMV_ERR;
                        }
                        memcpy(p,tempApp_UnionStruct->EMVTradeParam->AuthData,tempApp_UnionStruct->EMVTradeParam->AuthDataLen);
                        emvbase_free(tempApp_UnionStruct->EMVTradeParam->AuthData);
                        tempApp_UnionStruct->EMVTradeParam->AuthData = p;
                        tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 1024;
                    }
                    else if(tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen < 2048)
                    {
                        unsigned char *p = (unsigned char *)emvbase_malloc(2048);
                        if(p == NULL)
                        {
                        	EMVBase_Trace("PURE-error: memory error\r\n");
                            return RLT_EMV_ERR;
                        }
                        memcpy(p,tempApp_UnionStruct->EMVTradeParam->AuthData,tempApp_UnionStruct->EMVTradeParam->AuthDataLen);
                        emvbase_free(tempApp_UnionStruct->EMVTradeParam->AuthData);
                        tempApp_UnionStruct->EMVTradeParam->AuthData = p;
                        tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 2048;
                    }
                    else
                    {
                    	EMVBase_Trace("PURE-error: data len of Offline data authentication error\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                }
                memcpy((unsigned char*)&tempApp_UnionStruct->EMVTradeParam->AuthData[tempApp_UnionStruct->EMVTradeParam->AuthDataLen], apdu_r->DataOut, apdu_r->LenOut);
                tempApp_UnionStruct->EMVTradeParam->AuthDataLen += apdu_r->LenOut;

            }
            else            //������70����Ϊ�ѻ�������֤�Ѿ�ִ�в�ʧ��
            {
                tempApp_UnionStruct->EMVTradeParam->bErrAuthData = 1;
            }
        }

		// for tag9F24 9F19 9F6E 9F7C  20200707
		indexlen = 0;
		if(apdu_r->DataOut[indexlen] == 0x70)
		{
            indexlen++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &lenTemp))
            {
            	EMVBase_Trace("PURE-error: Tag70 parsing error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexTemp = indexlen;

            while(indexlen < indexTemp + lenTemp)
            {
                if(indexlen >= apdu_r->LenOut)
				{
		        	EMVBase_Trace("PURE-error: r-apdu parsing error\r\n");
					EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
		            return RLT_ERR_EMV_IccDataFormat;
		        }

                if(apdu_r->DataOut[indexlen] == 0xFF || apdu_r->DataOut[indexlen] == 0x00) { indexlen++; continue; }

                else if(!memcmp(&apdu_r->DataOut[indexlen], "\x9F\x24", 2))
                {
                    indexlen += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &len))
                    {
                    	EMVBase_Trace("PURE-error: Tag9F24 parsing error\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)
                    {
                    	EMVBase_Trace("PURE-error: Tag9F24's len is 0\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_PayAccountRefer, &apdu_r->DataOut[indexlen], len);
                    indexlen += len;
                }
                else		//unknown data,out of this command.
                {
                    k = apdu_r->DataOut[indexlen];

                    if((k & 0x1F) == 0x1F)
                    {
                    	EMVBase_Trace("PURE-info: Tag%02x%02x parsing\r\n", apdu_r->DataOut[index], apdu_r->DataOut[index+1]);
                        indexlen++;
                    }
					else
					{
						EMVBase_Trace("PURE-info: Tag%02x parsing\r\n", apdu_r->DataOut[index]);
					}
                    indexlen++;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &len))
                    {
                    	EMVBase_Trace("PURE-error: Tag parsing error\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    indexlen += len;
                }
            }

            if(indexlen != indexTemp + lenTemp)
            {
            	EMVBase_Trace("PURE-error: Tag70's value parsing error\r\n");
				EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
        }

        return RLT_EMV_OK;
    }

    while(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
    {
        index++;
    }

    if(apdu_r->DataOut[index] != 0x70)
    {
    	EMVBase_Trace("PURE-error: r-apdu template error (first byte != 0x70)\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }
    index++;

    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenAEF))
    {
    	EMVBase_Trace("PURE-error: Tag70 parsing error\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(authdataflag)     //add record to authentication data string if it is required.
    {
		if(tempApp_UnionStruct->EMVTradeParam->AuthData == NULL)
        {
            tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 0;
            tempApp_UnionStruct->EMVTradeParam->AuthData = (unsigned char *)emvbase_malloc(512);

            if(tempApp_UnionStruct->EMVTradeParam->AuthData == NULL)
            {
            	EMVBase_Trace("PURE-error: memory error\r\n");
                return RLT_EMV_ERR;
            }

            tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 512;
            memset(tempApp_UnionStruct->EMVTradeParam->AuthData,0,512);
        }

        if((tempApp_UnionStruct->EMVTradeParam->AuthDataLen + apdu_r->LenOut) > tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen)
        {
            if(tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen < 1024)
            {
                unsigned char *p = (unsigned char *)emvbase_malloc(1024);
                if(p == NULL)
                {
                	EMVBase_Trace("PURE-error: memory error\r\n");
                    return RLT_EMV_ERR;
                }
                memcpy(p,tempApp_UnionStruct->EMVTradeParam->AuthData,tempApp_UnionStruct->EMVTradeParam->AuthDataLen);
                emvbase_free(tempApp_UnionStruct->EMVTradeParam->AuthData);
                tempApp_UnionStruct->EMVTradeParam->AuthData = p;
                tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 1024;
            }
            else if(tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen < 2048)
            {
                unsigned char *p = (unsigned char *)emvbase_malloc(2048);
                if(p == NULL)
                {
                	EMVBase_Trace("PURE-error: memory error\r\n");
                    return RLT_EMV_ERR;
                }
                memcpy(p,tempApp_UnionStruct->EMVTradeParam->AuthData,tempApp_UnionStruct->EMVTradeParam->AuthDataLen);
                emvbase_free(tempApp_UnionStruct->EMVTradeParam->AuthData);
                tempApp_UnionStruct->EMVTradeParam->AuthData = p;
                tempApp_UnionStruct->EMVTradeParam->AuthDataMaxLen = 2048;
            }
            else
            {
            	EMVBase_Trace("PURE-error: data len of Offline data authentication error\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
        }

        memcpy((unsigned char*)&tempApp_UnionStruct->EMVTradeParam->AuthData[tempApp_UnionStruct->EMVTradeParam->AuthDataLen], (unsigned char*)&apdu_r->DataOut[index], lenAEF);
        tempApp_UnionStruct->EMVTradeParam->AuthDataLen += lenAEF;

    }
    indexAEF = index;


    while(index < indexAEF + lenAEF)
    {
        if(index >= apdu_r->LenOut)
        {
        	EMVBase_Trace("PURE-error: r-apdu parsing error\r\n");
			EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
        {
            index++;
            continue;
        }
        ret = pure_ParseAndStoreCardResponse(tempApp_UnionStruct, apdu_r->DataOut, &index, &bInTable, 1);

        if(ret != RLT_EMV_OK)
        {
        	EMVBase_Trace("PURE-error: sub Tags in Tag70 parsing error\r\n");
            return RLT_ERR_EMV_IccDataFormat;
        }
    }

    if(index != indexAEF + lenAEF)
    {
    	EMVBase_Trace("PURE-error: Tag70's value parsing error\r\n");
		EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    //sxl20110325
    if(index < apdu_r->LenOut)
    {
    	EMVBase_Trace("PURE-error: r-apdu parsing error\r\n");
		EMVBase_Trace("PURE-error: sub Tags' len error\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }

    return RLT_EMV_OK;
}

unsigned char pure_ReadAppData(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char retCode;
	EMVBASETAGCVLITEM *item;
	unsigned char aflexist = 0;
	unsigned char Read_AFL_Num = 0;
	unsigned char AFL_RecordNum = 0;
	unsigned char AFL_Num;
	EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned char authdataflag;
	unsigned char TornCompareFlag = 0;
	unsigned char Track2Data[30];
	unsigned short tmpTrack2Len, Track2Exist;


	tempApp_UnionStruct->EMVTradeParam->AuthDataLen = 0;
	tempApp_UnionStruct->EMVTradeParam->bErrAuthData = 0;
	tempApp_UnionStruct->EMVTradeParam->bErrSDATL = 0;

	item = emvbase_avl_gettagitempointer(EMVTAG_AFL);

	if(item != NULL)
	{
		if(item->len >> 2)
		{
			aflexist = 1;
		}
	}
	else
	{
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
//		return RLT_ERR_EMV_IccDataMissing;
	}

	retCode = EMVBase_CheckReadAFL(item);

	if(retCode != RLT_EMV_OK)
	{
		EMVBase_Trace("PURE-error: data of AFL(Tag94) error\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	AFL_Num = item->len >> 2;
	tempApp_UnionStruct->EMVTradeParam->AuthDataLen = 0;
	tempApp_UnionStruct->EMVTradeParam->bErrAuthData = 0;

	for(Read_AFL_Num = 0; Read_AFL_Num < AFL_Num; Read_AFL_Num++)
	{
		AFL_RecordNum = item->data[(Read_AFL_Num << 2) + 1];

		while(AFL_RecordNum <= item->data[(Read_AFL_Num << 2) + 2])
		{
			EMVBase_FormReadAflData(&apdu_s, AFL_RecordNum, item->data[(Read_AFL_Num << 2) + 0]);

			if(Read_AFL_Num == AFL_Num - 1 && AFL_RecordNum == item->data[(Read_AFL_Num << 2) + 2])
			{
                //apdu_s.EnableCancel = 0;
                apdu_s.EnableCancel = 1;
			}
			tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
			if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
			{
				if(apdu_r.ReadCardDataOk == APDUCOMMCANCEL)
            	{
            		EMVBase_Trace("PURE-error: trans cancel\r\n");
					return RLT_ERR_EMV_CancelTrans;
				}
				EMVBase_Trace("PURE-error: apdu timeout or other error\r\n");
				return pure_Outcome_CommunicationErrors_PaymentTransactionNotCompleted(tempApp_UnionStruct);
			}

			if(AFL_RecordNum < item->data[(Read_AFL_Num << 2) + 1] + item->data[(Read_AFL_Num << 2) + 3])
			{
				authdataflag = 1;
			}
			else
			{
				authdataflag = 0;
			}
			retCode = pure_ReadAFLRetData(tempApp_UnionStruct, &apdu_r, item->data[(Read_AFL_Num << 2) + 0], authdataflag);

			if(RLT_EMV_OK != retCode)
			{
				return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
			}

			AFL_RecordNum++;
		}
	}

	retCode = pure_DealAFLData(tempApp_UnionStruct);
	return retCode;
}

//unsigned char pure_GenerateDDADynamicSignature(PURETradeUnionStruct *tempApp_UnionStruct)
//{
//	EMVBASE_APDU_SEND apdu_s;
//	EMVBASE_APDU_RESP apdu_r;
//
//	memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
//	memcpy(apdu_s.Command, "\x00\x88\x00\x00");
//}

unsigned char pure_TDOLProcess(unsigned char* TDOL, unsigned short TDOLLen, unsigned char* DOLData, unsigned short* DOLDataLen)
{
    unsigned short index = 0;
    unsigned char *buf;
    unsigned char bInTable;
    unsigned char tag[3], taglen;
    EMVBASETAGCVLITEM *DOLdataitem;
    unsigned short templen, indexOut = 0, DOLLen;

    unsigned char i, k, m;
    unsigned char *DOL;


    buf = (unsigned char *)emvbase_malloc(255);
    DOL = (unsigned char *)emvbase_malloc(300);


    memset(DOL, 0, 300);

    DOLLen = (TDOLLen > 255) ? 255 : (TDOLLen);
    memcpy(DOL, TDOL, DOLLen);

    while(index < DOLLen)  //Process PDOL
    {
        if(DOL[index] == 0xFF || DOL[index] == 0x00) {index++; continue; }
        memset(buf, 0, 255);
        bInTable = 0;


        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = DOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = DOL[index + 2];
            }
        }
        DOLdataitem = emvbase_avl_gettagitempointer(tag);


        if(DOLdataitem != NULL)
        {
            index += taglen;



            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
            	EMVBase_Trace("PURE-error: TDOL's value error\r\n");
                emvbase_free(buf);
                emvbase_free(DOL);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
            m = DOLdataitem->len;


            if(DOLdataitem->datafomat & EMVTAGFORMAT_N)          //numeric
            {
                if(k >= m)
                {
                    if(m)
                    {
                        memcpy(&buf[k - m], DOLdataitem->data, m);
                    }
                    memcpy(&DOLData[indexOut], buf, k);
                }
                else
                {
                    if(m)
                    {
                        memcpy(buf, DOLdataitem->data, m);
                    }
                    memcpy(&DOLData[indexOut], &buf[m - k], k);
                }
            }
            else if(DOLdataitem->datafomat & EMVTAGFORMAT_CN)          //compact numeric
            {
                if(m)
                {
                    memset(buf, 0xFF, 255);
                    memcpy(buf, DOLdataitem->data, m);
                    memcpy(&DOLData[indexOut], buf, k);
                }
                else
                {
                    memset(buf, 0x00, 255);
                    memcpy(&DOLData[indexOut], buf, k);
                }
            }
            else            //other formats
            {
                if(m)
                {
                    memcpy(buf, DOLdataitem->data, m);
                }
                memcpy(&DOLData[indexOut], buf, k);
            }
            indexOut += k;
            bInTable = 1;
        }

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
            	EMVBase_Trace("PURE-error: TDOL's value error\r\n");
                emvbase_free(buf);
                emvbase_free(DOL);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
            memcpy(&DOLData[indexOut], buf, k);
            indexOut += k;
        }
    }

    *DOLDataLen = indexOut;


    emvbase_free(buf);
    emvbase_free(DOL);
    return RLT_EMV_OK;
}

unsigned char pure_DOLProcessFunc(unsigned char type, unsigned char* CDDOL, unsigned short CDDOLLen, unsigned char* DOLData, unsigned short* DOLDataLen, unsigned short DOLDataMaxLen, PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned short index = 0;
    unsigned char *buf;
    unsigned char bInTable;
    unsigned char tag[3], taglen;
    EMVBASETAGCVLITEM *DOLdataitem;
    unsigned short templen, indexOut = 0;
    unsigned char bHasUnpredictNum = 0;
    unsigned char AIP[2], TermCapab[3];
    unsigned char *TDOLData;
    unsigned short TDOLDataLen;
    EMVBASETAGCVLITEM * item;
    unsigned char CardTDOLDatabExist, TermTDOLDatabExist;
    unsigned char TCHashValue[20];
    unsigned char *DOL;
    unsigned short DOLLen;
    unsigned char i, k, m;
    unsigned char CAPKI = 0;

	unsigned short TrackOffsetInTDOLDataOffset = 0;
	unsigned char Track2DataLenInTDOL = 0;

	unsigned char errflag = 0;  //20201217 lsy

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
    memset(TermCapab, 0, sizeof(TermCapab));
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

    DOL = (unsigned char *)emvbase_malloc(300);
    buf = (unsigned char *)emvbase_malloc(255);

    memset(DOL, 0, 300);

    DOLLen = (CDDOLLen > 255) ? 255 : (CDDOLLen);
    memcpy(DOL, CDDOL, DOLLen);

	errflag = 0; //20201217 lsy
    while(index < DOLLen)
    {
        if(DOL[index] == 0xFF || DOL[index] == 0x00)
		{
			index++;
			continue;
		}
        memset(buf, 0, 255);
        bInTable = 0;

        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = DOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = DOL[index + 2];
            }
        }

        if(!memcmp(tag, "\x9F\x37", 2))
        {
            bHasUnpredictNum = 1;
        }

        //4.3e add 20160330
        if(!memcmp((unsigned char*)&DOL[index], "\x9F\x22", 2))
        {
            index += 3;
            emvbase_avl_gettagvalue_spec(EMVTAG_CAPKI, &CAPKI, 0, 1);
            DOLData[indexOut] = CAPKI;
            indexOut += 1;
            continue;
        }
        //end

        if(type == typeCDOL1 || type == typeCDOL2)
        {
            if(tag[0] == 0x98)
            {
                TDOLData = (unsigned char *)emvbase_malloc(512);
                memset(TDOLData, 0, 512);
                TDOLDataLen = 0;

                TermTDOLDatabExist = 0;
                item = emvbase_avl_gettagitemandstatus(EMVTAG_TDOL, &CardTDOLDatabExist);

                if(CardTDOLDatabExist == 0)
                {
                    item = emvbase_avl_gettagitemandstatus(EMVTAG_TermTDOL, &TermTDOLDatabExist);
                }

                if(item != NULL)
                {
                    if(item->len)
                    {
                        pure_TDOLProcess(item->data, item->len, TDOLData, &TDOLDataLen);
                    }
                }

			    EMVBase_Hash(TDOLData, TDOLDataLen, TCHashValue);

                emvbase_free(TDOLData);

                if(TermTDOLDatabExist)
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x80);
                }
                emvbase_avl_createsettagvalue(EMVTAG_TCHashValue, TCHashValue, 20);
            }
        }

		if((memcmp(tag, "\x9F\x50", 2) < 0) || (memcmp(tag, "\x9F\x7F", 2) > 0))
		{
        	DOLdataitem = emvbase_avl_gettagitempointer(tag);

        	if(DOLdataitem != NULL)
	        {
	            index += taglen;

	            if(EMVBase_ParseExtLen(DOL, &index, &templen))
	            {
	                emvbase_free(DOL);
	                emvbase_free(buf);
	                return RLT_ERR_EMV_IccDataFormat;
	            }
	            k = templen;
	            m = DOLdataitem->len;

				if ((indexOut + k) > DOLDataMaxLen) //20201217 lsy
				{
					errflag = 1;
					break;
				}

	            if(DOLdataitem->datafomat & EMVTAGFORMAT_N)		//numeric
	            {
	                if(k >= m)
	                {
	                    if(m)
	                    {
	                        memcpy(&buf[k - m], DOLdataitem->data, m);
	                    }
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	                else
	                {
	                    if(m)
	                    {
	                        memcpy(buf, DOLdataitem->data, m);
	                    }
	                    memcpy(&DOLData[indexOut], &buf[m - k], k);
	                }
	            }
	            else if(DOLdataitem->datafomat & EMVTAGFORMAT_CN)	//compact numeric
	            {
	                if(m)
	                {
	                    memset(buf, 0xFF, 255);
	                    memcpy(buf, DOLdataitem->data, m);
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	                else
	                {
	                    memset(buf, 0x00, 255);
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	            }
	            else	//other formats
	            {
	                if(m)
	                {
	                    memcpy(buf, DOLdataitem->data, m);
	                }
	                memcpy(&DOLData[indexOut], buf, k);
	            }

	            indexOut += k;
	            bInTable = 1;
	        }
		}

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
                emvbase_free(DOL);
                emvbase_free(buf);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
			if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
			{
				errflag = 1;
				break;
			}
            memcpy(&DOLData[indexOut], buf, k);
            indexOut += k;
        }
    }


    emvbase_free(DOL);
    emvbase_free(buf);

	if (errflag)//20201217 lsy
	{
		EMVBase_Trace("pure-error: pure_DOLProcessFunc doldata over maxdollen error !!!!!!\r\n");
		return RLT_EMV_ERR;
	}
    *DOLDataLen = indexOut;

    if(bHasUnpredictNum == 0)
    {
        if(type == typeDDOL)
        {
			EMVBase_Trace("pure-error: DDOL dont contain Unpredict Number(9F37)\r\n");
            return RLT_EMV_ERR;
        }
        else if(type == typeCDOL1)
        {
            tempApp_UnionStruct->EMVTradeParam->bCDOL1HasNoUnpredictNum = 1;

            if((AIP[0] & 0x01) && (TermCapab[2] & 0x08))
            {
                return RLT_EMV_OK;
            }
        }
        else if(type == typeCDOL2)
        {
            tempApp_UnionStruct->EMVTradeParam->bCDOL2HasNoUnpredictNum = 1;

            if((AIP[0] & 0x01) && (TermCapab[2] & 0x08))
            {
                return RLT_EMV_OK;
            }
        }
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}

unsigned char pure_DOLProcess(unsigned char type, unsigned char * DOL, unsigned short DOLLen, unsigned char* DOLData, unsigned short* DOLDataLen, unsigned short DOLDataMaxLen, PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;
    unsigned char TVR[5];

    retCode = pure_DOLProcessFunc(type, DOL, DOLLen, DOLData, DOLDataLen,DOLDataMaxLen, tempApp_UnionStruct);

    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

    if(TVR[4] & 0x80)		//if 'Default TDOL used' bit is 1
    {
        pure_DOLProcessFunc(type, DOL, DOLLen, DOLData, DOLDataLen,DOLDataMaxLen, tempApp_UnionStruct);
    }
    return retCode;
}

void pure_RecoverPanFromTrack2EQU(PURETradeUnionStruct *tempApp_UnionStruct)
{
	int i,j;
	unsigned char pbuf[100] = {0};
	unsigned short Track2EquLen;
	unsigned char Track2Equ[20];
	unsigned char pan[10], panlen=0;

    memset(Track2Equ, 0, 20);
	emvbase_avl_gettagvalue_all(EMVTAG_Track2Equ, Track2Equ, &Track2EquLen);

    if(Track2EquLen < 2)
    {
        return;
    }

    for(i = Track2EquLen-1; i > 1; i--)
    {
        if(Track2Equ[i] != 0xff)
		{
			break;
		}
    }

    j = i+1;
    if((Track2Equ[j-1]&0x0f) == 0x0f)
    {
        i = j*2 -1;
    }
    else
	{
		i = j*2;
	}

    EMVBaseBcdToAsc(pbuf,Track2Equ,(i+1)/2);
    for(i=0;i<24;i++)
    {
        if((pbuf[i]=='D') || (pbuf[i]=='F'))
        {
            pbuf[i]=0x00;
            break;
        }
    }

	memset(pan, 0 , 10);
	EMVBaseAscToBcd(pan, pbuf, i);
	if(i % 2)
	{
		pan[(i-1)/2] |= 0x0F;
	}
	panlen = (i+1) / 2;

	emvbase_avl_createsettagvalue(EMVTAG_PAN, pan, panlen);
}

unsigned char pure_initDynamicAuth(PURETradeUnionStruct *tempApp_UnionStruct)
{
	EMVBASE_CAPK_STRUCT *tempcapk;

	tempcapk = tempApp_UnionStruct->EMVTradeParam->CAPK;

	if(tempcapk == NULL)
	{
		return RLT_EMV_ERR;
	}

    if(pure_CheckDataMissDDA(tempApp_UnionStruct) != RLT_EMV_OK)
    {

        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);       //ICC data missing

        return RLT_ERR_EMV_IccDataMissing;
    }

    if(tempcapk->ModulLen == 0)
    {
            return RLT_EMV_ERR;
	}

    if(tempApp_UnionStruct->EMVTradeParam->IPKModulLen == 0)
    {
        if(pure_GetIPK(tempApp_UnionStruct) != RLT_EMV_OK)                  //retrieval of the issuer public key modulus
        {
            return RLT_EMV_ERR;
        }

    }
    EMVBase_Trace("chenjun test1 for Segmentation fault\r\n");
    if(tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen == 0)
    {
        if(pure_GetICCPK(tempApp_UnionStruct) != RLT_EMV_OK)                //retrieval of the ICC public key
        {

            return RLT_EMV_ERR;
        }
    }

    return RLT_EMV_OK;
}

unsigned char pure_FormDynSignGenerate(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char *TempDDOLData;
    unsigned short TempDDOLDataLen;
    unsigned char bTermDDOL, bDDOL;
    EMVBASETAGCVLITEM *ddolitem = NULL;
    unsigned char *DDOL;
    unsigned short DDOLLen;


    ddolitem = emvbase_avl_gettagitemandstatus(EMVTAG_DDOL, &bDDOL);

    if(bDDOL == 0)
    {
        ddolitem = emvbase_avl_gettagitemandstatus(EMVTAG_TermDDOL, &bTermDDOL);

        if(bTermDDOL == 0)
        {
			EMVBase_Trace("pure-error: Card don't return DDOL and Terminal dont contacin Default DDOL,DDA failed!!!\r\n");
            return RLT_ERR_EMV_TermDataMissing;
        }
    }
    DDOL = (unsigned char *)emvbase_malloc(300);
    TempDDOLData = (unsigned char *)emvbase_malloc(300);
    memset(DDOL, 0, 300);
    DDOLLen = (ddolitem->len > 255) ? 255 : (ddolitem->len);
    memcpy(DDOL, ddolitem->data, DDOLLen);


    if(pure_DOLProcess(typeDDOL, DDOL, DDOLLen, TempDDOLData, &TempDDOLDataLen, 255, tempApp_UnionStruct) != RLT_EMV_OK)//20121221 lsy
    {
    	EMVBase_Trace("pure-error: DDOL data format error\r\n");
        emvbase_free(DDOL);
        emvbase_free(TempDDOLData);
        return RLT_EMV_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_DDOLRelatedData, TempDDOLData, TempDDOLDataLen);

    memcpy(apdu_s->Command, "\x00\x88\x00\x00", 4);
    apdu_s->Lc = TempDDOLDataLen;
    memcpy(apdu_s->DataIn, TempDDOLData, TempDDOLDataLen);
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;

    emvbase_free(DDOL);
    emvbase_free(TempDDOLData);
    return RLT_EMV_OK;
}

unsigned char pure_DynSignGenerate(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
    unsigned char k;
    unsigned short index, indexTemp, len, lenTemp;


    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)   //Internal authenticaton OK
    {
        if(apdu_r->LenOut == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;		//added on 2002.09.29. for JCB Card AP 37.
        }
        index = 0;

        if(apdu_r->DataOut[index] == 0x80)			//primitive data
        {
            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            //2CL.035.00,if data format parse error,should terminate transaction.
            if(index + len != apdu_r->LenOut)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(len == 0)               //sxl EMV4.1e
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            emvbase_avl_createsettagvalue(EMVTAG_SignDynAppData, &apdu_r->DataOut[index], len);
        }
        else if(apdu_r->DataOut[index] == 0x77)      //TLV coded data,assume that SDAD is at first.
        {
            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenTemp))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexTemp = index;

            while(index < indexTemp + lenTemp)
            {
                if(index >= apdu_r->LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00) { index++; continue; }

                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x4B", 2))
                {                 //tag of Signed Dyn App Data
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)                       //sxl emv4.1e
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_SignDynAppData, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //20170509 add for 4.3f
                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x24", 2))
                {                 //tag of Payment Account Reference
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)                       //sxl emv4.1e
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_PayAccountRefer, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //end
                //20190603 lhd add for 4.3h
                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x19", 2))
                {
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)                       //sxl emv4.1e
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_TOKENREQUESTID, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //end
                //20200526  for 4.3i
                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x25", 2))
                {
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_PANLast4Digits, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //end
                else                //unknown data,out of this command.
                {
                    k = apdu_r->DataOut[index];

                    if((k & 0x1F) == 0x1F)
                    {
                        index++;
                    }
                    index++;
                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    index += len;
                }
            }

            if(index != indexTemp + lenTemp)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
        else
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData) == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        return RLT_EMV_OK;
    }
    else
	{
		EMVBase_Trace("pure-error:INTERNAL AUTHENTICATE command return SW != 9000\r\n");
		return RLT_ERR_EMV_IccReturn;
	}
}

unsigned char pure_CheckIPKReminderExist(void)
{
	unsigned char ipkcertlen = 0,iccpkcertlen = 0;

	EMVBASETAGCVLITEM *item, *item1;

	item = emvbase_avl_gettagitempointer(EMVTAG_IPKCert);

	if(item != NULL)
	{
		if(item->len)
		{
			ipkcertlen = item->len;
		}
	}

	item = emvbase_avl_gettagitempointer(EMVTAG_ICCPKCert);

	if(item != NULL)
	{
		if(item->len)
		{
			iccpkcertlen = item->len;
		}
	}

	item = emvbase_avl_gettagitempointer(EMVTAG_IPKRem);

	if(item != NULL)
	{
		if(item->len)
		{
		}
	}

	if((iccpkcertlen + 36) > ipkcertlen)
	{
		if(!emvbase_avl_checkiftagexist(EMVTAG_IPKRem))
		{
			return 1;
		}
	}

	return 0;
}

unsigned char pure_ODACheckMandotoryData(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char AIP[2];
	unsigned char ODAFail = 0;
	unsigned char ImpleOption = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	unsigned char KernelCap[5];
	unsigned char TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	unsigned char retCode;
	EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned char CID;

	EMVBase_Trace("PURE-info: (for Debug only)In pure_ODACheckMandotoryData Implementation Option: %02x\r\n", ImpleOption);

	memset(AIP, 0, sizeof(AIP));

	emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
	EMVBase_Trace("PURE-info: AIP(Tag82): %02x %02x\r\n", AIP[0], AIP[1]);
	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	EMVBase_TraceHex("PURE-info: Contactless kernel Capabilities", KernelCap, 5);

	if(((AIP[0] & 0x01) && (0 == (ImpleOption & 0x10)) && (KernelCap[4] & 0x01)) || (PURE_TRANS_APPAUTHTRANS != TransType))//ICC and terminal support Combined DDA/AC
	{
		EMVBase_Trace( "PURE-info: CDA supported by terminal and card\r\n");
		emvbase_avl_setvalue_and(EMVTAG_TVR,0,0x7F); //Offline CAM was not performed

		if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData)		   //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
		{
			EMVBase_Trace("PURE-info: SDA Tag List(Tag9F4A) error(len is not equal to 1 or value is not equal to '82')\r\n");
			EMVBase_Trace("or data need to be authenticated returned by read app data is not template '70'\r\n");
			ODAFail = 1;
		}
		else
		{
			if(RLT_EMV_OK != pure_CheckDataMissDDA(tempApp_UnionStruct))
		    {
		    	ODAFail = 1;
				emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20); //ICC data missing
		        //retCode = RLT_ERR_EMV_IccDataMissing;
		    }
			else
			{
				tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x02; //CDA
				EMVBase_Trace( "PURE-info: Goto CDA\r\n");
				if(pure_CheckIPKReminderExist())
				{
//					ODAFail = 1;
//					emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20); //ICC data missing
				}
			}
			if(emvbase_avl_checkiftagexist(EMVTAG_CAPKI))
			{

				if(tempApp_UnionStruct->CheckCapkExist)
				{
					if(RLT_EMV_OK != tempApp_UnionStruct->CheckCapkExist(tempApp_UnionStruct->EMVTradeParam))
					{
						ODAFail = 1;
					}
				}
			}
		}

		if(ODAFail)
		{
			EMVBase_Trace("PURE-info: set TVR byte1 bit3 = 1\r\n");
			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);		//CDA failed
		}
	}
	else if((AIP[0] & 0x20) && (ImpleOption & 0x10) && (PURE_TRANS_APPAUTHTRANS == TransType) && (KernelCap[4] & 0x20))
	{
		EMVBase_Trace( "PURE-info: DDA supported by terminal and card\r\n");
		emvbase_avl_setvalue_and(EMVTAG_TVR,0,0x7F); //Offline CAM was not performed

		if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData)		   //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
		{
			EMVBase_Trace("PURE-info: SDA Tag List(Tag9F4A) error(len is not equal to 1 or value is not equal to '82')\r\n");
			EMVBase_Trace("or data need to be authenticated returned by read app data is not template '70'\r\n");
			ODAFail = 1;
		}
		else
		{
			if(RLT_EMV_OK != pure_CheckDataMissDDA(tempApp_UnionStruct))
			{
				ODAFail = 1;
				emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20); //ICC data missing
				//retCode = RLT_ERR_EMV_IccDataMissing;
			}
			else
			{
				tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x03; //DDA
				EMVBase_Trace( "PURE-info: Goto DDA\r\n");
				if(pure_CheckIPKReminderExist())
				{
//					ODAFail = 1;
//					emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20); //ICC data missing
				}

			}
			if(emvbase_avl_checkiftagexist(EMVTAG_CAPKI))
			{

				if(tempApp_UnionStruct->CheckCapkExist)
				{
					if(tempApp_UnionStruct->CheckCapkExist(tempApp_UnionStruct->EMVTradeParam)!=RLT_EMV_OK)
					{
						ODAFail = 1;
					}
				}
			}

			memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
			retCode = pure_FormDynSignGenerate(tempApp_UnionStruct, &apdu_s);
			if(RLT_EMV_OK != retCode)
			{
				ODAFail = 1;
			}
			else
			{
				memset(&apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
				tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
				if(RLT_EMV_OK != apdu_r.ReadCardDataOk)
				{
					ODAFail = 1;
				}
				else
				{
					retCode = pure_DynSignGenerate(tempApp_UnionStruct, &apdu_r);
					if(RLT_EMV_OK != retCode)
					{
						ODAFail = 1;
					}
				}
			}

		}
		if(ODAFail)
		{
			EMVBase_Trace("PURE-info: set TVR byte1 bit4 = 1\r\n");
			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x08);		//DDA failed
		}
	}
	else if((AIP[0] & 0x40) && (KernelCap[4] & 0x40))
	{
		EMVBase_Trace( "PURE-info: SDA supported by terminal and card\r\n");
		emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x02); //SDA was performed
		emvbase_avl_setvalue_and(EMVTAG_TVR,0,0x7F); //Offline CAM was not performed
		if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData)		   //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
		{
			EMVBase_Trace("PURE-info: SDA Tag List(Tag9F4A) error(len is not equal to 1 or value is not equal to '82')\r\n");
			EMVBase_Trace("or data need to be authenticated returned by read app data is not template '70'\r\n");
			ODAFail = 1;
		}
		else
		{
			if(RLT_EMV_OK != pure_CheckDataMissSDA(tempApp_UnionStruct))
			{
				ODAFail = 1;
				emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20); //ICC data missing
				//retCode = RLT_ERR_EMV_IccDataMissing;
			}
			else
			{
				tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x01; //SDA
				EMVBase_Trace( "PURE-info: Goto SDA\r\n");
				if(pure_CheckIPKReminderExist())
				{
//					ODAFail = 1;
//					emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20); //ICC data missing
				}
			}
			if(emvbase_avl_checkiftagexist(EMVTAG_CAPKI))
			{

				if(tempApp_UnionStruct->CheckCapkExist)
				{
					if(tempApp_UnionStruct->CheckCapkExist(tempApp_UnionStruct->EMVTradeParam)!=RLT_EMV_OK)
					{
						ODAFail = 1;
					}
				}
			}
		}
		if(ODAFail)
		{
			EMVBase_Trace("PURE-info: set TVR byte1 bit4 = 1\r\n");
			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x40);		//SDA failed
		}
	}

	if(0x00 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator)
	{
		EMVBase_Trace("PURE-info: ODA not perform\r\n");
		emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x80);		//SDA failed
	}

	return RLT_EMV_OK;
}



void pure_RandTransSelect(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned int MinLimit,ThresholdMoney,TermAmtAuth;
	unsigned char TransTargetPercent;
	unsigned char bRandSelected;

	unsigned char FloorLimit[6];
	unsigned char Threshold[6];
	unsigned char RandomNum;
	unsigned char bShowRandNum;
	unsigned char AmtAuthNum[6];
	unsigned char MaxTargetPercent,TargetPercent;
	unsigned char  TVR[5];
	unsigned char TermFloorLmtHex[4];
	unsigned short tmp;


    memset(FloorLimit,0,sizeof(FloorLimit));
	if(emvbase_avl_checkiftagexist(EMVTAG_clessofflinelimitamt))
	{
		emvbase_avl_gettagvalue_spec(EMVTAG_clessofflinelimitamt, FloorLimit, 0, 6);
	}
	else
	{
		if(emvbase_avl_checkiftagexist(EMVTAG_FloorLimit))
		{
			emvbase_avl_gettagvalue_spec(EMVTAG_FloorLimit, TermFloorLmtHex, 0, 4);
			EMVBaseHex2U16(&tmp, TermFloorLmtHex, 4);
			EMVBaseU16ToBcd(FloorLimit, tmp, 6);
		}
		else
		{
			memcpy(FloorLimit, "\x00\x00\x00\x05\x00\x00", 6);
		}
	}

	memset(Threshold,0,sizeof(Threshold));
    emvbase_avl_gettagvalue_spec(EMVTAG_Threshold, Threshold,0,6);

	memset(AmtAuthNum,0,sizeof(AmtAuthNum));
    emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum,AmtAuthNum,0,6);

	EMVBaseBcdToU32(&MinLimit, FloorLimit, 6);
	EMVBaseBcdToU32(&ThresholdMoney, Threshold, 6);
	EMVBaseBcdToU32(&TermAmtAuth, AmtAuthNum, 6);

	if(TermAmtAuth > MinLimit)
	{
		return;
	}

	//zcl add
	memset(TVR, 0, sizeof(TVR));
	emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);
	if(TVR[3] & 0x80)
	{
		return;
	}

	bRandSelected=0;
	if(tempApp_UnionStruct->EMVB_RandomNum == NULL)
	{
		return;
	}

	tempApp_UnionStruct->EMVB_RandomNum(&RandomNum,1);

	RandomNum = (unsigned char)(RandomNum%99+1);
    EMVBase_Trace("pure-info: RandomNum=%d\r\n", RandomNum);
	if(emvbase_avl_gettagvalue(EMVTAG_bShowRandNum))
	{
		emvbase_avl_settag(EMVTAG_bShowRandNum,RandomNum);
	}

	MaxTargetPercent = emvbase_avl_gettagvalue(EMVTAG_MaxTargetPercent);
	TargetPercent = emvbase_avl_gettagvalue(EMVTAG_TargetPercent);

    EMVBase_Trace("pure-info: pure_RandTransSelect Precent max=%d target=%d \r\n",MaxTargetPercent,TargetPercent);
	EMVBase_Trace("pure-info: Threshold=%d TermAmtAuth=%d MinLimit=%d\r\n",ThresholdMoney,TermAmtAuth, MinLimit);


	if(TermAmtAuth < ThresholdMoney)
	{
		if(RandomNum<=TargetPercent) bRandSelected=1;
	}
	else
	{
		if(MinLimit < ThresholdMoney + 100)
		{
			EMVBase_Trace("pure-info:(for Debug only) MinLimit < ThresholdMoney + 100\r\n");
			return;
		}
		TransTargetPercent=(unsigned char)((MaxTargetPercent - TargetPercent)*(TermAmtAuth - ThresholdMoney)/(MinLimit-ThresholdMoney)+TargetPercent);
		EMVBase_Trace("pure-info: TransTargetPercent=%d\r\n", TransTargetPercent);
		if(RandomNum<=TransTargetPercent)
		{
			bRandSelected=1;
		}
	}

	if(bRandSelected==1)
	{
		EMVBase_Trace("Random Trans online Selected!!!\r\n");
	    emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x10); //set 'Transaction selected randomly for online processing' bit 1.
	}

}


int pure_IccGetPAN(unsigned char *pasOutPAN)
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

unsigned char pure_CheckFloorLimit(PURETradeUnionStruct *tempApp_UnionStruct)
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
	EMVBase_Trace("emv","pure_CheckFloorLimit amt = %d\r\n", amt);
	EMVBase_Trace("emv","pure_CheckFloorLimit MinLimit = %d\r\n", MinLimit);

	if(amt >= MinLimit)
    {
		emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80); //transaction exceeds floor limit
        return 1;
    }

	if(tempApp_UnionStruct->GetTransAmtSumRes != NULL)
	{
		ret = tempApp_UnionStruct->GetTransAmtSumRes();
		if(ret == 0)	//user dont set this func,default value
		{
			return 0;
		}
		else if(ret == 1)	//user think amount sum dont overflow floor limit
		{
			return 0;
		}
		else if(ret == -1)	//user think amount sum overflow floor limit
		{
			return 1;
		}
	}

    if(amt >= MinLimit)
    {
		emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80); //set 'transaction exceeds floor limit' bit 1.
        return 1;
    }
    return 0;
}

unsigned char pure_CommandReadATC(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command,"\x80\xCA\x9F\x36",4);
	apdu_s->Lc=0;
	apdu_s->Le=256;

	apdu_s->EnableCancel = 1;
	return RLT_EMV_OK;
}

unsigned char pure_CommandReadLOATC(EMVBASE_APDU_SEND *apdu_s)
{
	#if Algeria_Request
	emvbase_avl_deletetag(EMVTAG_PURECOTC);
    memcpy(apdu_s->Command,"\x80\xCA\xDF\x13",4);
	#else
    memcpy(apdu_s->Command,"\x80\xCA\x9F\x13",4);
	#endif
	apdu_s->Lc=0;
	apdu_s->Le=256;

	apdu_s->EnableCancel = 1;
	return RLT_EMV_OK;
}

unsigned char pure_CheckVelocity_Algeria(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char COTCExist)
{
	EMVBASE_APDU_SEND apdu_s={0};
	EMVBASE_APDU_RESP apdu_r={0};
	unsigned char LCOLExist,UCOLExist,LCOL,UCOL;
	unsigned char COTC[2];
	unsigned char nCOTC=0;

	LCOLExist = emvbase_avl_checkiftagexist(EMVTAG_PURELCOL);
	if(!LCOLExist)
	{
		memcpy(apdu_s.Command,"\x80\xCA\xDF\x1B",4);
		apdu_s.Lc=0;
		apdu_s.Le=256;
		apdu_s.EnableCancel = 1;

		tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

		if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00)
		{
			if(!memcmp(apdu_r.DataOut,"\xDF\x1B",2))// ATC found
			{
				LCOLExist = 1;
				emvbase_avl_createsettagvalue(EMVTAG_PURELCOL,(unsigned char*)&apdu_r.DataOut[3],1);
				EMVBase_Trace("pure-info: (Only for Algeria)LCOL(DF1B)", apdu_r.DataOut[3]);
			}
		}
	}

	memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
	memset(&apdu_r, 0, sizeof(EMVBASE_APDU_RESP));

	UCOLExist = emvbase_avl_checkiftagexist(EMVTAG_PUREUCOL);
	if(!UCOLExist)
	{
		memcpy(apdu_s.Command,"\x80\xCA\xDF\x1C",4);
		apdu_s.Lc=0;
		apdu_s.Le=256;
		apdu_s.EnableCancel = 1;

		tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

		if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00)
		{
			if(!memcmp(apdu_r.DataOut,"\xDF\x1C",2))// ATC found
			{
				UCOLExist = 1;
				emvbase_avl_createsettagvalue(EMVTAG_PUREUCOL,(unsigned char*)&apdu_r.DataOut[3],1);
				EMVBase_Trace("pure-info: (Only for Algeria)UCOL(DF1C)", apdu_r.DataOut[3]);
			}
		}
	}

	if(COTCExist)
	{
		nCOTC = emvbase_avl_gettagvalue(EMVTAG_PURECOTC);
		if(LCOLExist)	// 1.Check LCOL(DF1B)，if COTC(DF13) is equal or greater than LCOL,set TVR related bit
		{
			LCOL = emvbase_avl_gettagvalue(EMVTAG_PURELCOL);
			if(nCOTC >= LCOL);
			{
				emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
			}
		}

		if(UCOLExist)
		{
			UCOL = emvbase_avl_gettagvalue(EMVTAG_PUREUCOL);
			if(nCOTC >= UCOL)	// 2.Check UCOL(DF1C)，if COTC(DF13) is equal or greater than LCOL,set TVR related bit
			{
				emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
			}
		}
	}

    return RLT_EMV_OK;
}

unsigned char pure_CheckVelocity(unsigned char bATCExist,unsigned char bLOATCExist)
{
	unsigned short counts=0,nAtc = 0,nLoatc = 0;
	unsigned char LOATC[2],ATC[2];
    unsigned char LCOL=0,UCOL=0;

	if(bATCExist==0 || bLOATCExist==0)	//ATC or LOATC is missing
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
		emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20);//set 'ICC data missing' bit 1.
	}

    memset(LOATC,0,sizeof(LOATC));
	emvbase_avl_gettagvalue_spec(EMVTAG_LOATC,LOATC,0,2);


	if(bLOATCExist==1 && LOATC[0]==0 && LOATC[1]==0)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x08);	//set 'New Card' bit 1
	}

    memset(ATC,0,sizeof(ATC));
	emvbase_avl_gettagvalue_spec(EMVTAG_ATC,ATC,0,2);
	// ATC and LOATC are high byte ahead,low byte behind.
	nAtc = nAtc + ATC[0]*256+ATC[1];
	nLoatc = nLoatc + LOATC[0]*256+LOATC[1];

	if(nAtc<=nLoatc)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
	}
	else
	{
		counts=nAtc-nLoatc;
		LCOL = emvbase_avl_gettagvalue(EMVTAG_LCOL);
		UCOL = emvbase_avl_gettagvalue(EMVTAG_UCOL);//20190531_lhd


		if(counts > LCOL)
			emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
		if(counts > UCOL)
			emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
	}

    return RLT_EMV_OK;
}

unsigned char pure_TermRiskManage(PURETradeUnionStruct *tempApp_UnionStruct)
{
	int retCode;
	unsigned char AIP[2], KernelCap[5];
	unsigned char bLOATCExist = 0,bATCExist = 0;
    EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned char COTCExist;

	emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);

	EMVBase_TraceHex("pure-info: AIP(82):", AIP, 2);
	EMVBase_TraceHex("pure-info: Contactless Kernel Cap:", KernelCap, 5);

	if((KernelCap[4] & 0x08) && (AIP[0] & 0x08))
	{
		EMVBase_Trace("pure-info: Contactless Kernel Cap B5b4 and AIP B1b4 is 1\r\n");
//		//EMV Book 3 section 10.6.1
//		pure_CheckFloorLimit(tempApp_UnionStruct);
//
//		//EMV Book 3 section 10.6.2
//		pure_RandTransSelect(tempApp_UnionStruct);
//
//
//		if(((TransCurExp == 0) && (memcmp(AmtAuthNum, "\x00\x00\x00\x00\x00\x01", 6) == 0)) ||
//			((TransCurExp == 1) &&	(memcmp(AmtAuthNum, "\x00\x00\x00\x00\x00\x10", 6) == 0)) ||
//			((TransCurExp == 2) &&	(memcmp(AmtAuthNum, "\x00\x00\x00\x00\x01\x00", 6) == 0)))
//			{
//				emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80);
//			}
		//Velocity Checking
		EMVBase_Trace("pure-info: LCOL(9F14) exist = %d\r\n", emvbase_avl_checkiftagexist(EMVTAG_LCOL));
		EMVBase_Trace("pure-info: UCOL(9F23) exist = %d\r\n", emvbase_avl_checkiftagexist(EMVTAG_UCOL));
		if((0 == emvbase_avl_checkiftagexist(EMVTAG_LCOL))|| (0 == emvbase_avl_checkiftagexist(EMVTAG_UCOL)))
		{
			return RLT_EMV_OK;
		}
		EMVBase_Trace("pure-info: need to Get ATC(9F36)\r\n");
		pure_CommandReadATC(&apdu_s);
		tempApp_UnionStruct->IsoCommand(&apdu_s,&apdu_r);
		if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
		{
//			return RLT_ERR_EMV_APDUTIMEOUT;
		}
		if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00)
		{
			if(!memcmp(apdu_r.DataOut,"\x9F\x36",2))		// ATC found
			{
				bATCExist = 1;
				emvbase_avl_createsettagvalue(EMVTAG_ATC,(unsigned char*)&apdu_r.DataOut[3],2);
			}
		}

		pure_CommandReadLOATC(&apdu_s);
		 tempApp_UnionStruct->IsoCommand(&apdu_s,&apdu_r);
		if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
		{
//			return RLT_ERR_EMV_APDUTIMEOUT;
		}
		if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00)
		{
			#if Algeria_Request
			if(!memcmp(apdu_r.DataOut,"\xDF\x13",2))// ATC found
			{
				COTCExist = 1;
				emvbase_avl_createsettagvalue(EMVTAG_PURECOTC,(unsigned char*)&apdu_r.DataOut[3],1);
				EMVBase_TraceHex("pure-info: (Only for Algeria)COTC(DF13)", apdu_r.DataOut[3], 1);
			}
			#else
			if(!memcmp(apdu_r.DataOut,"\x9F\x13",2))// ATC found
			{
				bLOATCExist = 1;
				emvbase_avl_createsettagvalue(EMVTAG_LOATC,(unsigned char*)&apdu_r.DataOut[3],2);
			}
			#endif
		}

		#if Algeria_Request
		retCode = pure_CheckVelocity_Algeria(tempApp_UnionStruct, COTCExist);
		#else
		retCode = pure_CheckVelocity(bATCExist,bLOATCExist);
		#endif

	}

	return RLT_EMV_OK;


	//3.Exception File Check
	//FIXME:reserve for use
//	if(tempApp_UnionStruct->VerifyCardNo != NULL)
//	{
//    	if(emvbase_avl_checkiftagexist(EMVTAG_PAN))
//    	{
//        	if(pure_IccGetPAN(asPAN) == RLT_EMV_OK)
//            {
//     			EMVBase_Trace( "begin to compare!\r\n");
//                retCode = (*tempApp_UnionStruct->VerifyCardNo)(asPAN);
//				EMVBase_Trace( "retcode=%d\r\n", retCode);
//                if(retCode != RLT_EMV_OK)
//                {
//                	EMVBase_Trace( "card is in exception file!\r\n");
//                    emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x10);
//                }
//            }
//    	}
//	}

	return RLT_EMV_OK;
}


/*******************************************************************
   ��    ��: sxl
   ��������: unsigned char Paywave_CheckExpDate(AppUnionStruct *tempApp_UnionStruct)
   ��������: �жϿ�Ƭ��Ч����
   ��ڲ���: ��
   �� �� ֵ: OK ��Ч.
                  ERR ��Ч.
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char pure_CheckExpDate(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char currentDate[4], EffectDate[4], ExpireDate[4];   //buf[10],
    unsigned char AppEffectDateexist, AppExpireDateexist;
    EMVBASETAGCVLITEM *item;

    emvbase_avl_gettagvalue_spec(EMVTAG_TransDate, &currentDate[1], 0, 3);
	EMVBase_TraceHex("PURE-info: Transaction Date(Tag9A): ", &currentDate[1], 3);

    if(currentDate[1] > 0x49) { currentDate[0] = 0x19; }
    else{ currentDate[0] = 0x20; }
    AppEffectDateexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_AppEffectDate);

    if(item != NULL)
    {
        if(item->len >= 3)
        {
            AppEffectDateexist = 1;
        }
    }

    if(AppEffectDateexist)    //App effective date exist
    {
    	EMVBase_TraceHex("PURE-info: Effective Date(Tag5F25): ", item->data, 3);
        if(EMVBase_CheckDateFormat(item->data) == RLT_EMV_ERR)
		{
        	EMVBase_Trace("PURE-error: data of Effective Date(Tag5F25) error\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
        }

        if(item->data[0] > 0x49) {EffectDate[0] = 0x19; }
        else{ EffectDate[0] = 0x20; }
        memcpy((unsigned char*)&EffectDate[1], item->data, 3);

        if(memcmp(currentDate, EffectDate, 4) < 0)
        {
        	EMVBase_Trace("PURE-info: Application not yet effective \r\n");
            emvbase_avl_setvalue_or(EMVTAG_TVR, 1, 0x20);
        }
    }
    AppExpireDateexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_AppExpireDate);

    if(item != NULL)
    {
        if(item->len >= 3)
        {
            AppExpireDateexist = 1;
        }
    }

    if(AppExpireDateexist)    //App expiration date exist
    {
    	EMVBase_TraceHex("PURE-info: Expiry Date(Tag5F24):", item->data, 3);
        if(EMVBase_CheckDateFormat(item->data) == RLT_EMV_ERR)
		{
        	EMVBase_Trace("PURE-error: data of Expiry Date(Tag5F24) error\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
        }

        if(item->data[0] > 0x49) { ExpireDate[0] = 0x19; }
        else{ ExpireDate[0] = 0x20; }
        memcpy((unsigned char*)&ExpireDate[1], item->data, 3);

        if(memcmp(currentDate, ExpireDate, 4) > 0)
        {
        	EMVBase_Trace("PURE-info: Expired\r\n");
            emvbase_avl_setvalue_or(EMVTAG_TVR, 1, 0x40);
        }
    }
    return RLT_EMV_OK;
}

void pure_CheckAVN(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char AVNCardExist,AVNTermExist;
	unsigned char AVNTermULExist,AVNTermLLExist;
	unsigned char AVNCard[2],AVNTerm[2];
	unsigned char AVNTermUL[2]={0},AVNTermLL[2]={0};

	AVNCardExist = emvbase_avl_checkiftagexist(EMVTAG_AppVer);
	AVNTermExist = emvbase_avl_checkiftagexist(EMVTAG_AppVerNum);
	AVNTermULExist = emvbase_avl_checkiftagexist(EMVTAG_PUREAppVerUL);
	AVNTermLLExist = emvbase_avl_checkiftagexist(EMVTAG_PUREAppVerLL);

	if(AVNCardExist && AVNTermExist)
	{
		emvbase_avl_gettagvalue_spec(EMVTAG_AppVer, AVNCard, 0, 2);
		emvbase_avl_gettagvalue_spec(EMVTAG_AppVerNum, AVNTerm, 0, 2);
		if(AVNTermULExist)
		{
			emvbase_avl_gettagvalue_spec(EMVTAG_PUREAppVerUL, AVNTermUL, 0, 2);
			EMVBase_TraceHex("pure-info: App Ver Upper Limit", AVNTermUL, 2);
		}
		if(AVNTermLLExist)
		{
			emvbase_avl_gettagvalue_spec(EMVTAG_PUREAppVerLL, AVNTermLL, 0, 2);
			EMVBase_TraceHex("pure-info: App Ver Lower Limit", AVNTermLL, 2);
		}

		if(0 != memcmp(AVNCard, AVNTerm, 2))
		{
			#if Algeria_Request
			//every AVN (9F09) of SATIM card's AID is not unique,its a range,if 9F08 is in range of 0x10 to 0x16,no need to set TVR related bit
			if((memcmp(AVNCard, AVNTermUL, 2) > 0) || (memcmp(AVNCard, AVNTermLL, 2) < 0))
			{
				EMVBase_Trace("pure-info: App Ver different");
				emvbase_avl_setvalue_or(EMVTAG_TVR, 1, 0x80); //ICC and terminal have different application versions
			}
			#else
			EMVBase_Trace("pure-info: App Ver different");
			emvbase_avl_setvalue_or(EMVTAG_TVR, 1, 0x80); //ICC and terminal have different application versions
			#endif
		}
	}
}

void pure_CheckAUC(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char bTestFail;
    unsigned char TermType,AUC[2],AUCbExist;
    unsigned char IssuCountryCode[2],CountryCode[2],IssuCountryCodebExist;
    unsigned char TermAmtOther[6];
    unsigned char TermAddCapab[5];
    unsigned char TermAmtOtherBin = 0;
    unsigned char TransType;

    bTestFail=0;
    memset(AUC,0,sizeof(AUC));
    AUCbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_AUC,AUC,0,2));
	EMVBase_TraceHex("pure-info: AUC(9F07):", AUC, 2);

	TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);
//  TransType = tempApp_UnionStruct->EMVTradeParam->TransType;
    TransType = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
	EMVBase_Trace("pure-info: TransType(9C) = %02X\r\n", TransType);
	TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	EMVBase_Trace("pure-info: CurTransType = %02X\r\n", TransType);

	memset(TermAddCapab,0,sizeof(TermAddCapab));
    emvbase_avl_gettagvalue_spec(EMVTAG_TermAddCapab,TermAddCapab,0,5);

    memset(IssuCountryCode,0,sizeof(IssuCountryCode));
    IssuCountryCodebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IssuCountryCode,IssuCountryCode,0,2));
    memset(CountryCode,0,sizeof(CountryCode));
    emvbase_avl_gettagvalue_spec(EMVTAG_CountryCode,CountryCode,0,2);

    memset(TermAmtOther,0,sizeof(TermAmtOther));
    emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum,TermAmtOther,0,6);
    if(memcmp(TermAmtOther,"\x00\x00\x00\x00\x00\x00",6))
    {
        TermAmtOtherBin = 1;
    }


	EMVBase_Trace("pure-info: AUCbExist = %d\r\n", AUCbExist);
	EMVBase_Trace("pure-info: IssuCountryCodebExist = %d\r\n", IssuCountryCodebExist);

	EMVBase_TraceHex("pure-info: IssuCountryCode(5F28):", IssuCountryCode, 2);
	EMVBase_TraceHex("pure-info: CountryCode(9F1A):", CountryCode, 2);

	if(AUCbExist)   //AUC exist.
    {
        if((TermType==0x14 || TermType==0x15 || TermType==0x16 )
           && (TermAddCapab[0]&0x80))  //The termianl is ATM
        {
            if(!(AUC[0]&0x02))
                bTestFail=1;
        }
        else        //The terminal is not ATM
        {
            if(!(AUC[0]&0x01))
                bTestFail=1;
        }
        if(IssuCountryCodebExist)       //Issuer country code exist
        {
            if(!memcmp(IssuCountryCode,CountryCode,2))      //domestic
            {
                if(TransType == PURE_TRANS_CASH)
                {
                    if(!(AUC[0]&0x80))
                        bTestFail=1;
                }

                if(TransType == PURE_TRANS_GOODS)
                {
                    if( !(AUC[0]&0x28))
                        bTestFail=1;
                }
                if(TransType == PURE_TRANS_CASHBACK)
                {
                    if(!(AUC[1]&0x80))
                        bTestFail=1;
                }
            }
            else                            //international,terminal country code differ from issuer country code
            {
                if(TransType == PURE_TRANS_CASH)
                {
                    if(!(AUC[0]&0x40))
                        bTestFail=1;
                }

                if(TransType == PURE_TRANS_GOODS )
                {
                    if( !(AUC[0]&0x14))
                        bTestFail=1;
                }

                if(TransType == PURE_TRANS_CASHBACK)
                {
                    if(!(AUC[1]&0x40))
                        bTestFail=1;
                }
            }
        }
    }


    if(bTestFail)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x10);
    }

}

unsigned char pure_ProcessRestrict(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char AIP[2];
	unsigned char KernelCap[5];
	unsigned char retCode = RLT_EMV_OK;
	unsigned char SZZTICS[7];

	emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	emvbase_avl_gettagvalue_spec(EMVTAG_SZZTICS, SZZTICS, 0, 7);

	if((KernelCap[4] & 0x08) && (AIP[0] & 0x08))
	{
		pure_CheckAVN(tempApp_UnionStruct);
		pure_CheckAUC(tempApp_UnionStruct);
		retCode = pure_CheckExpDate(tempApp_UnionStruct);
	}

	return retCode;
}

unsigned char pure_TermActAnaly(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char i, k;   //TermAnaResult,CardAnaResult;//0-Denial,1-Online,2-Offline
    unsigned char bFitIAC, bFitTAC;
    EMVBASE_Card_TAC CardTAC;

    unsigned char TVR[5];
    unsigned char TACDenial[5];
    unsigned char IACDenial[5], IACDenialbExist;
    unsigned char TermType;
    unsigned char TACOnline[5], IACOnline[5], IACOnlinebExist;
    unsigned char TACDefault[5], IACDefault[5], IACDefaultbExist;
    unsigned char bExist;



#if 1
    emvbase_avl_printtagallvalue("PURE-info: TVR: ", EMVTAG_TVR);
    emvbase_avl_printtagallvalue("PURE-info: TAC-online: ", EMVTAG_TACOnline);
    emvbase_avl_printtagallvalue("PURE-info: TAC-default: ", EMVTAG_TACDefault);
    emvbase_avl_printtagallvalue("PURE-info: TAC-Denial: ", EMVTAG_TACDenial);
    emvbase_avl_printtagallvalue("PURE-info: IAC-online: ", EMVTAG_IACOnline);
    emvbase_avl_printtagallvalue("PURE-info: IAC-default: ", EMVTAG_IACDefault);
    emvbase_avl_printtagallvalue("PURE-info: IAC-Denial: ", EMVTAG_IACDenial);
#endif

    IACDenialbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACDenial, IACDenial, 0, 5));
    if(IACDenialbExist == 0)   //IAC-denial not exist
    {
        memset(IACDenial, 0, 5);
    }
    memcpy(CardTAC.IACDenial, IACDenial, 5);

    IACOnlinebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACOnline, IACOnline, 0, 5));
    if(IACOnlinebExist == 0)   //IAC-online not exist
    {
        memset(CardTAC.IACOnline, 0xFF, 5);
        memset(IACOnline, 0, 5);
    }
    else
	{
		memcpy(CardTAC.IACOnline, IACOnline, 5);
	}

    IACDefaultbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACDefault, IACDefault, 0, 5));
    if(IACDefaultbExist == 0)   //IAC-default not exist
    {
        memset(CardTAC.IACDefault, 0xFF, 5);
        memset(IACDefault, 0, 5);
    }
    else
    {
        memcpy(CardTAC.IACDefault, IACDefault, 5);   //lechan 20070207
    }

	//PURE Default TAC
	//Denial	0410000000
	//Online	9060009000
	//Default 9040008000
    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACDenial, TACDenial, 0, 5));
    if(bExist == 0)
    {
    	EMVBase_Trace("PURE-info: TAC-Denial missing-->use '04 10 00 00 00'\r\n");
        memset(TACDenial, 0, 5);
		memcpy(TACDenial, "\x04\x10\x00\x00\x00", 5);
    }

    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACOnline, TACOnline, 0, 5));
    if(bExist == 0)
    {
    	EMVBase_Trace("PURE-info: TAC-Online missing-->use '90 60 00 90 00'\r\n");
        memset(TACOnline, 0, 5);
        memcpy(TACOnline, "\x90\x60\x00\x90\x00", 5);
    }

    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACDefault, TACDefault, 0, 5));
    if(bExist == 0)
    {
    	EMVBase_Trace("PURE-info: TAC-Default missing-->use '90 40 00 80 00'\r\n");
        memset(TACDefault, 0, 5);
        memcpy(TACDefault, "\x90\x40\x00\x80\x00", 5);
    }

    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);
    if(!memcmp(TVR, "\x00\x00\x00\x00\x00", 5))
	{
		//return TAA_OFFLINE;
	}

#if 0
    emvbase_avl_printtagallvalue("Term online", EMVTAG_TACOnline);
    emvbase_avl_printtagallvalue("Term default", EMVTAG_TACDefault);
    emvbase_avl_printtagallvalue("Term declined", EMVTAG_TACDenial);
    emvbase_avl_printtagallvalue("ICC online", EMVTAG_IACOnline);
    emvbase_avl_printtagallvalue("ICC default", EMVTAG_IACDefault);
    emvbase_avl_printtagallvalue("ICC declined", EMVTAG_IACDenial);
#endif

	bFitIAC = 0;
    bFitTAC = 0;
    //比较IAC和TVR,TAC和TVR的DENIAL�?如果有相同的位则交易DENIAL.
    for(i = 0; i < 5; i++)
    {
        k = TVR[i];

        if((k & CardTAC.IACDenial[i]) != 0)
        {
        	bFitIAC = 1;
        }

        if((k & TACDenial[i]) != 0)
        {
        	bFitTAC = 1;
        }
    }
    if(bFitIAC || bFitTAC)
	{
		EMVBase_Trace("PURE-info: AAC required\r\n");
		return TAA_DENIAL;
	}

    //k=tempterminfo->TermType&0x0F;
    TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);
    TermType = TermType & 0x0F;

    //sxl20131024  bAbleOnline还需要重新处�?
    if((TermType == 1 || TermType == 2 || TermType == 4 || TermType == 5) &&
       tempApp_UnionStruct->EMVTradeParam->bAbleOnline)          //Terminal has Online capability
    {           //比较是有联机能力的终端类�?
                //比较IAC和TVR,TAC和TVR的Online�?如果有相同的位则交易Online.否则OFFLINE
                //当交易联机失败时,bAbleOnline被清�?.只判断执行ACC或TC
        bFitIAC = 0;
        bFitTAC = 0;

        for(i = 0; i < 5; i++)
        {
            k = TVR[i];

            if((k & CardTAC.IACOnline[i]) != 0) { bFitIAC = 1; }

            if((k & TACOnline[i]) != 0) { bFitTAC = 1; }
        }

        if(bFitIAC || bFitTAC)
		{
			EMVBase_Trace("PURE-info: ARQC required\r\n");
			return TAA_ONLINE;
		}
        else
		{
			EMVBase_Trace("PURE-info: TC required\r\n");
			return TAA_OFFLINE;
		}
    }

    bFitIAC = 0;
    bFitTAC = 0;
    for(i = 0; i < 5; i++)
    {
        k = TVR[i];

        if((k & CardTAC.IACDefault[i]) != 0) { bFitIAC = 1; }

        if((k & TACDefault[i]) != 0) { bFitTAC = 1; }
    }

    if(bFitIAC || bFitTAC)
	{
		EMVBase_Trace("PURE-info: AAC required\r\n");
		return TAA_DENIAL;
	}
    else
	{
		EMVBase_Trace("PURE-info: TC required\r\n");
		return TAA_OFFLINE;
	}
}

unsigned char pure_CheckIfCardSupportCDA(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char AIP[2];

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);

    if((AIP[0] & 0x01))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char pure_CheckIfdoCDA(PURETradeUnionStruct *tempApp_UnionStruct)
{
	if(0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char pure_GACDOLProcessFunc(unsigned char type, unsigned char* CDDOL, unsigned short CDDOLLen, unsigned char* DOLData, unsigned short* DOLDataLen, PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned short index = 0;
    unsigned char *buf;   //[255]
    unsigned char bInTable;
    unsigned char tag[4], taglen;
    EMVBASETAGCVLITEM *DOLdataitem;
    unsigned short templen, indexOut = 0;
    unsigned char bHasUnpredictNum = 0;
    unsigned char AIP[2], KernelCap[5];
    unsigned char *TDOLData;       //[512]
    unsigned short TDOLDataLen;
    EMVBASETAGCVLITEM * item;
    unsigned char CardTDOLDatabExist, TermTDOLDatabExist;
    unsigned char TCHashValue[20];
    unsigned char *DOL;         //[300]
    unsigned short DOLLen;
    unsigned char i, k, m;
    unsigned char CAPKI = 0;

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
    memset(KernelCap, 0, sizeof(KernelCap));
    emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	EMVBase_TraceHex("pure-info: KernelCap", KernelCap, 5);


    DOL = (unsigned char *)emvbase_malloc(300);
    buf = (unsigned char *)emvbase_malloc(255);



    memset(DOL, 0, 300);

    DOLLen = (CDDOLLen > 255) ? 255 : (CDDOLLen);
    memcpy(DOL, CDDOL, DOLLen);


    while(index < DOLLen)  //Process PDOL
    {
        if(DOL[index] == 0xFF || DOL[index] == 0x00) {index++; continue; }
        memset(buf, 0, 255);
        bInTable = 0;


        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = DOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = DOL[index + 2];
            }
        }

        if(!memcmp(tag, "\x9F\x37", 2))
        {
            bHasUnpredictNum = 1;
        }

        //4.3e add 20160330
        if(!memcmp((unsigned char*)&DOL[index], "\x9F\x22", 2))      //终端CAPKI
        {
            index += 3;
            emvbase_avl_gettagvalue_spec(EMVTAG_CAPKI, &CAPKI, 0, 1);
            DOLData[indexOut] = CAPKI;
            indexOut += 1;
            continue;
        }
        //end

		EMVBase_Trace( "bHasUnpredictNum=%d tag[0]=%02x\r\n",bHasUnpredictNum, tag[0]);
        if(type == typeCDOL1 || type == typeCDOL2)
        {
            if(tag[0] == 0x98)
            {
                TDOLData = (unsigned char *)emvbase_malloc(512);
                memset(TDOLData, 0, 512);
                TDOLDataLen = 0;

                TermTDOLDatabExist = 0;
                item = emvbase_avl_gettagitemandstatus(EMVTAG_TDOL, &CardTDOLDatabExist);

                if(CardTDOLDatabExist == 0)
                {
                    item = emvbase_avl_gettagitemandstatus(EMVTAG_TermTDOL, &TermTDOLDatabExist);
                }

                if(item != NULL)
                {
                    if(item->len)
                    {
                        pure_TDOLProcess(item->data, item->len, TDOLData, &TDOLDataLen);
                    }
                }
                EMVBase_Hash(TDOLData, TDOLDataLen, TCHashValue);

                emvbase_free(TDOLData);

                if(TermTDOLDatabExist)
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x80);
                }
                emvbase_avl_createsettagvalue(EMVTAG_TCHashValue, TCHashValue, 20);
            }
        }
        DOLdataitem = emvbase_avl_gettagitempointer(tag);


        if(DOLdataitem != NULL)
        {
            index += taglen;



            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
            	EMVBase_Trace("PURE-error: DOL's value error\r\n");
                emvbase_free(DOL);
                emvbase_free(buf);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;               // 1
            m = DOLdataitem->len;           // 4



            if(DOLdataitem->datafomat & EMVTAGFORMAT_N)          //numeric
            {
                if(k >= m)
                {
                    if(m)
                    {
                        memcpy(&buf[k - m], DOLdataitem->data, m);
                    }
                    memcpy(&DOLData[indexOut], buf, k);
                }
                else
                {
                    if(m)
                    {
                        memcpy(buf, DOLdataitem->data, m);
                    }
                    memcpy(&DOLData[indexOut], &buf[m - k], k);
                }
            }
            else if(DOLdataitem->datafomat & EMVTAGFORMAT_CN)          //compact numeric
            {
                if(m)
                {
                    memset(buf, 0xFF, 255);
                    memcpy(buf, DOLdataitem->data, m);
                    memcpy(&DOLData[indexOut], buf, k);
                }
                else
                {
                    memset(buf, 0x00, 255);
                    memcpy(&DOLData[indexOut], buf, k);
                }
            }
            else            //other formats
            {
                if(m)
                {
                    memcpy(buf, DOLdataitem->data, m);
                }
                memcpy(&DOLData[indexOut], buf, k);
            }
            indexOut += k;
            bInTable = 1;
        }

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
            	EMVBase_Trace("PURE-error: DOL's value error\r\n");
                emvbase_free(DOL);
                emvbase_free(buf);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
            memcpy(&DOLData[indexOut], buf, k);
            indexOut += k;
        }
		EMVBase_TraceHex("DOL tag", tag, taglen);
    }


    emvbase_free(DOL);
    emvbase_free(buf);


    *DOLDataLen = indexOut;

    if(bHasUnpredictNum == 0)
    {
        if(type == typeDDOL)
        {
			EMVBase_Trace("pure-error: DDOL dont contacin 9F37\r\n");
            return RLT_EMV_ERR;
        }
        else if(type == typeCDOL1)
        {
            tempApp_UnionStruct->EMVTradeParam->bCDOL1HasNoUnpredictNum = 1;
			EMVBase_Trace("pure-info: CDOL1 dont contacin 9F37\r\n");
			if(pure_CheckIfdoCDA(tempApp_UnionStruct))
			{
				return RLT_EMV_OK;
			}

            if((AIP[0] & 0x01) && (KernelCap[4] & 0x01))
            {
				EMVBase_Trace("pure-info: card and term support CDA\r\n");
                return RLT_EMV_OK;
            }
        }
        else if(type == typeCDOL2)
        {
            tempApp_UnionStruct->EMVTradeParam->bCDOL2HasNoUnpredictNum = 1;
			EMVBase_Trace("pure-info: CDOL2 dont contacin 9F37\r\n");
			if(pure_CheckIfdoCDA(tempApp_UnionStruct))
			{
				return RLT_EMV_OK;
			}

            if((AIP[0] & 0x01) && (KernelCap[4] & 0x01))
            {
				EMVBase_Trace("pure-info: card and term support CDA\r\n");
                return RLT_EMV_OK;
            }
        }
		EMVBase_Trace("pure-error: unrecognize DOL type\r\n");
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}


unsigned char pure_GACDOLProcess(unsigned char type, unsigned char * DOL, unsigned short DOLLen, unsigned char* DOLData, unsigned short* DOLDataLen, PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;
    unsigned char TVR[5];

    retCode = pure_GACDOLProcessFunc(type, DOL, DOLLen, DOLData, DOLDataLen, tempApp_UnionStruct);
	EMVBase_Trace("Emvb_DOLProcessFunc retcode=%d\r\n", retCode);

    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

    if(TVR[4] & 0x80)  //if 'Default TDOL used' bit is 1
    {
        pure_GACDOLProcessFunc(type, DOL, DOLLen, DOLData, DOLDataLen, tempApp_UnionStruct);
    }
    return retCode;
}

unsigned char pure_CommandGAC1(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char TermAnaResult, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char retCode;    //TermAnaResult,CardAnaResult;//0-Denial,1-Online,2-Offline
    unsigned short len;
    unsigned char SubAmount[6];
    unsigned char bCombineDDA = 0;
    unsigned char CDOL1bExist;
    EMVBASETAGCVLITEM *item;
	unsigned char TTPI[5];
	unsigned char TVR[5];

    memcpy(apdu_s->Command, "\x80\xAE", 2);
	if(TermAnaResult == TAA_ONLINE)
    {
        memcpy(&apdu_s->Command[2], "\x80\x00", 2);      //get ARQC
    }
    else if(TermAnaResult == TAA_OFFLINE)
    {
    	memcpy(&apdu_s->Command[2], "\x40\x00", 2);          //get TC
    }
    else if(TermAnaResult == TAA_DENIAL)
    {
    	memcpy(&apdu_s->Command[2], "\x00\x00", 2);          //get AAC
    }

	if(0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator)
	{
		emvbase_avl_gettagvalue_spec(EMVTAG_PURETTPI, TTPI, 0, 5);
		emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);
		if(((TermAnaResult == TAA_OFFLINE) || (TTPI[1] & 0x04)) && (0 == TVR[0] & 0x04))	//add for FIME Tool test case CDA_PROC_02
		{
            apdu_s->Command[2] |= 0x10;          //Combined DDA/AC requested.
		}
		else
		{
			tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x00;
		}
	}

    emvbase_avl_settag(EMVTAG_ReferenceControlParameter, apdu_s->Command[2]);


    item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL1, &CDOL1bExist);

    if(CDOL1bExist == 0)
    {
    	EMVBase_Trace("PURE-error: CDOL1(Tag8C) missing\r\n");
        return RLT_EMV_ERR;
    }
    retCode = pure_GACDOLProcess(typeCDOL1, item->data, item->len, apdu_s->DataIn, &len, tempApp_UnionStruct);


    emvbase_avl_createsettagvalue(EMVTAG_CDOL1RelatedData, apdu_s->DataIn, len);

    apdu_s->Lc = len;
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 0;

    return retCode;
}

unsigned char pure_DealGAC1Rsp(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r, unsigned char CCIDInGPO, unsigned char TermAnaResult)
{
    unsigned char ret;
	unsigned short index,indexTemp,lenTemp,orgindex,indexRet;
	unsigned char bIntable;
    EMVBASETAGCVLITEM *item;
	unsigned char AppCryptExist, CryptInfoExist, ATCExist,SDADExist,IssuAuthDataExist,CCIDInGPOExist;
	unsigned char CCIDInGAC,CID;
	unsigned char CVMRes[3];
	unsigned char GenerateACRetDataflag;
	unsigned char CCIDInGACExist;

	if(0x90 == apdu_r->SW1 && 0x00 == apdu_r->SW2)
	{

	}
	else
	{
		EMVBase_Trace("pure-error: GAC SW!=9000\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

    tempApp_UnionStruct->EMVTradeParam->GenerateACRetDataLen = 0;
	memset(tempApp_UnionStruct->EMVTradeParam->GenerateACRetData,0,255);

	emvbase_avl_deletetag(EMVTAG_SignDynAppData);
	emvbase_avl_deletetag(EMVTAG_CryptInfo);
	emvbase_avl_deletetag(EMVTAG_ATC);
	emvbase_avl_deletetag(EMVTAG_AppCrypt);
	if(CCIDInGPOExist)
	{
		EMVBase_Trace("pure-info: CCID Exist in GPO, delete for deal GAC resp\r\n");
		emvbase_avl_deletetag(EMVTAG_PURECCID); //if CCID present in GPO,delete tag in avl to ensure check CCID whether present in GAC response correctly
	}

	if(apdu_r->DataOut[0] != 0x77)
	{
		EMVBase_Trace("PURE-error: r-apdu template error (first byte != 0x77)\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

    index=0;
	if(apdu_r->DataOut[index]==0x77)//TLV coded data
	{
		EMVBase_Trace("PURE-info: GAC template 77\r\n");
		index++;
		if(EMVBase_ParseExtLen(apdu_r->DataOut,&index,&lenTemp))
		{
			EMVBase_Trace("PURE-info: Tag77 parsing error\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}
		indexTemp=index;
		indexRet=0;
		while(index<indexTemp+lenTemp)
		{
			if(index>=apdu_r->LenOut)
			{
				EMVBase_Trace("PURE-info: r-apdu parsing error\r\n");
				EMVBase_Trace("PURE-info: sub Tags' len error\r\n");
				return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
			}
			if(apdu_r->DataOut[index]==0xFF || apdu_r->DataOut[index]==0x00)
			{
				index++;
				continue;
			}

            orgindex = index;
			bIntable = 0;
			GenerateACRetDataflag = 1;

			if(!memcmp((unsigned char*)&apdu_r->DataOut[index],"\x9F\x4B",2))
			{
				GenerateACRetDataflag = 0;
			}

			ret = pure_ParseAndStoreCardResponse(tempApp_UnionStruct,apdu_r->DataOut,&index,&bIntable,0);
			if(ret != RLT_EMV_OK)
			{
				EMVBase_Trace("PURE-info: sub Tags in Tag77 parsing error\r\n");
				return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
			}

			if(GenerateACRetDataflag&&(indexRet+index-orgindex <=255))
			{
				memcpy(tempApp_UnionStruct->EMVTradeParam->GenerateACRetData+indexRet,(unsigned char*)&apdu_r->DataOut[orgindex],index-orgindex);
				indexRet+=index-orgindex;
			}

		}	//endwhile
		if(index!=indexTemp+lenTemp)
		{
			EMVBase_Trace("PURE-info: Tag77's value parsing error\r\n");
			EMVBase_Trace("PURE-info: sub Tags' len error\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
		}
	}

	IssuAuthDataExist =	emvbase_avl_checkiftagexist(EMVTAG_IssuAppData);//9F10
	AppCryptExist = emvbase_avl_checkiftagexist(EMVTAG_AppCrypt);//9F26
	CryptInfoExist = emvbase_avl_checkiftagexist(EMVTAG_CryptInfo);//9F27
	ATCExist = emvbase_avl_checkiftagexist(EMVTAG_ATC);//9F36
	SDADExist = emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData);//9F4B
	CCIDInGPOExist = tempApp_UnionStruct->EMVTradeParam->CCIDExistInGPO;//C5
	CCIDInGACExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);
	EMVBase_Trace("PURE-info: AppCryptExist: %d\r\n", AppCryptExist);

	if((CryptInfoExist == 0)  || (ATCExist == 0) || (IssuAuthDataExist == 0))
	{
		if(0 == CryptInfoExist)
		{
			EMVBase_Trace("PURE-info: CID(Tag9F27) missing\r\n");
		}
		if(0 == ATCExist)
		{
			EMVBase_Trace("PURE-info: ATC(Tag9F36) missing\r\n");
		}
		if(0 == IssuAuthDataExist)
		{
			EMVBase_Trace("PURE-info: Issuer Auth Data(Tag9F10) missing\r\n");
		}
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	ret = pure_CheckGACRespMandatoryDataMiss(tempApp_UnionStruct);
	if(ret)
	{
		EMVBase_Trace("pure-error: GAC Response miss mandatory data\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}

	EMVBase_Trace("PURE-info: CCIDInGPOExist: %d\r\n", CCIDInGPOExist);
	EMVBase_Trace("PURE-info: CCIDInGACExist: %d\r\n", CCIDInGACExist);

	if((1 == CCIDInGPOExist) && (0 == CCIDInGACExist))
	{
		EMVBase_Trace("pure-error: CCID return in GPO but not in GAC\r\n");
		return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
	}
	else
	{
		if(1 == CCIDInGPOExist && 1 == CCIDInGACExist)
		{
			CCIDInGAC = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
			if((CCIDInGPO & 0x07) != (CCIDInGAC & 0x07))	//CVM Information (3 least significant bits) has a distinct value in both CCID values
			{
				EMVBase_Trace("pure-error: CVM Information (3 least significant bits) has a distinct value in both CCID values\r\n");
				return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);
			}
		}
	}

	//K14.5
	emvbase_avl_gettagvalue_spec(EMVTAG_CVMResult, CVMRes, 0, 3);
	CID = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
	CCIDInGAC = emvbase_avl_gettagvalue(EMVTAG_PURECCID);

	EMVBase_TraceHex("PURE-info: CVM Result:", CVMRes, 3);
	EMVBase_Trace("PURE-info: CID indicate: %02X\r\n", (CID&0xC0));
	EMVBase_Trace("PURE-info: CCIDInGACExist: %d\r\n", CCIDInGACExist);


	if(((CVMRes[0] == 0x24 || CVMRes[0] == 0x64) && (0x00 == CVMRes[2])) && (0x00 == (CID & 0xC0)) && (0 == CCIDInGACExist))
	{
		EMVBase_Trace("PURE-info: see phone,Request CDCVM\r\n", CID);
		return pure_Outcome_FinancialTransactionCompleted_OndeviceCVMRequested(tempApp_UnionStruct);
	}
	if(CCIDInGACExist && (0x04 == (CCIDInGAC & 0x07)))
	{
		EMVBase_Trace("PURE-info: see phone,Request CDCVM\r\n", CID);
		return pure_Outcome_FinancialTransactionCompleted_OndeviceCVMRequested(tempApp_UnionStruct);
	}

	if((0x02 == tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator) && (0x00 == (CID & 0xC0)) && (0 == SDADExist))
	{
		tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x00;
	}

	if(CCIDInGPOExist && (CCIDInGAC & 0x20))	//CCID indicate Switch to contact interface
	{
		return pure_Outcome_FinancialTransactionCompleted_TryOtherInterface(tempApp_UnionStruct);
	}
	else
	{
		if((TAA_OFFLINE == TermAnaResult) && (0x00 == (CID & 0xC0)))
		{
			return pure_Outcome_FinancialTransactionCompleted_TryOtherInterface(tempApp_UnionStruct);
		}
	}

	return RLT_EMV_OK;
}

unsigned char pure_DynSignVerify(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char *SDAToSign, SDAHash[20], *SDAData;    //[512]   [248]
    unsigned int index;
    EMVBASE_SIGN_DYNDATA_RECOVER *recovSDA;
    EMVBASETAGCVLITEM *item;
    unsigned char SignDynAppDataLen;
    unsigned char *SignDynAppData;
    unsigned char ICCPKExpexist, qPBOCUndfinedParamexist;
    unsigned char UnpredictNum[4];
    unsigned char AmtAuthNum[6];
    unsigned char TransCurcyCode[2];
    unsigned char DDAVersionType = 0;
    unsigned char CardUnpredictNum[20];

    SignDynAppDataLen = 0;

    item = emvbase_avl_gettagitempointer(EMVTAG_SignDynAppData);

    if(item != NULL)
    {
        SignDynAppDataLen = item->len;
        SignDynAppData = item->data;

        if(SignDynAppData == NULL)
        {
            return RLT_EMV_ERR;
        }
    }
    else
    {
        return RLT_EMV_ERR;
    }

    if(SignDynAppDataLen != tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen)
    {
        return RLT_EMV_ERR;
    }
    ICCPKExpexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_ICCPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            ICCPKExpexist = 1;
        }
    }

    if(ICCPKExpexist == 0)
    {
        return RLT_EMV_ERR;
    }
	EMVBase_TraceHex("ICCPKModul", tempApp_UnionStruct->EMVTradeParam->ICCPKModul, tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen);
	EMVBase_TraceHex("CardInfo.ICCPKExp",item->data, item->len);
    emvbase_avl_printtagallvalue("SignDynAppData: ", EMVTAG_SignDynAppData);

    SDAData = (unsigned char *)emvbase_malloc(248);


    EMVBase_RSARecover(tempApp_UnionStruct->EMVTradeParam->ICCPKModul, tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen, item->data, item->len, SignDynAppData, SDAData);

	EMVBase_TraceHex("Recovered SDAD:", SDAData, tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen);


    recovSDA = (EMVBASE_SIGN_DYNDATA_RECOVER *)emvbase_malloc(sizeof(EMVBASE_SIGN_DYNDATA_RECOVER));

    if(EMVBase_FillSDA(SDAData, recovSDA, tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen) != RLT_EMV_OK)
    {
        EMVBase_Trace("\r\npaywave_DynSignVerify emv_fillsda fail\r\n");
        emvbase_free(SDAData);
        emvbase_free(recovSDA);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataTrail != 0xBC)             //If it is not ��BC��, dynamic data authentication has failed.
    {
        EMVBase_Trace("\r\npaywave_DynSignVerify BC fail\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataHead != 0x6A)              //If it is not ��6A��, dynamic data authentication has failed.
    {
        EMVBase_Trace("\r\npaywave_DynSignVerify 6A fail\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    //�������ص���95
    if(recovSDA->DataFormat != 0x05)    //If it is not ��05��, dynamic data authentication has failed.
    {
        EMVBase_Trace("\r\npaywave_DynSignVerify 05 fail\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }
    index = 0;
    SDAToSign = (unsigned char *)emvbase_malloc(512);
    memcpy(SDAToSign, (unsigned char*)&SDAData[1], tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen - 22);
    index += tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen - 22;

    emvbase_free(SDAData);

    memset(CardUnpredictNum, 0, sizeof(CardUnpredictNum));

    //tag 9F 69 exist
    if(emvbase_avl_gettagvalue_spec(EMVTAG_CardAuthData, CardUnpredictNum, 0, 5) == 0)
    {
        if(CardUnpredictNum[0] == 0x01)
        {
            DDAVersionType = 0x01;
        }
    }
    EMVBase_Trace("pure-info: DDA Version = %d\r\n", DDAVersionType);

    if(DDAVersionType == 0)
    {
        memset(UnpredictNum, 0, sizeof(UnpredictNum));
        emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UnpredictNum, 0, 4);
        memcpy((unsigned char*)&SDAToSign[index], UnpredictNum, 4);
        index += 4;
    }
    else
    {
        memset(UnpredictNum, 0, sizeof(UnpredictNum));
        emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UnpredictNum, 0, 4);
        memcpy((unsigned char*)&SDAToSign[index], UnpredictNum, 4);
        index += 4;


		EMVBase_TraceHex("UnpredictNum:", UnpredictNum, 4);

        memset(AmtAuthNum, 0, sizeof(AmtAuthNum));
        emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum, AmtAuthNum, 0, 6);
        memcpy((unsigned char*)&SDAToSign[index], AmtAuthNum, 6);
        index += 6;

		EMVBase_TraceHex("AmtAuthNum:", AmtAuthNum, 6);

        memset(TransCurcyCode, 0, sizeof(TransCurcyCode));
        emvbase_avl_gettagvalue_spec(EMVTAG_TransCurcyCode, TransCurcyCode, 0, 2);
        memcpy((unsigned char*)&SDAToSign[index], TransCurcyCode, 2);
        index += 2;

		EMVBase_TraceHex("TransCurcyCode:", TransCurcyCode, 2);

    }

    qPBOCUndfinedParamexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_CardAuthData);

    if(item != NULL)
    {
        if(item->len)
        {
            qPBOCUndfinedParamexist = 1;
        }
    }

    if(qPBOCUndfinedParamexist == 0)
    {
        EMVBase_Trace("\r\npaywave_DynSignVerify MV_UNPREDICTUDOL fail\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAToSign);
        return RLT_EMV_ERR;
    }
    EMVBase_Trace("\r\ntempcardinfo->qPBOCUndfinedParam[0] = %d %d\r\n", item->data[0], item->len);

    if((item->data[0] != 0x01) || (item->len < 5) || (item->len > 16))
    {
        EMVBase_Trace("\r\npaywave_DynSignVerify qPBOCUndfinedParam 01 fail\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAToSign);
        return RLT_EMV_ERR;
    }

    memcpy((unsigned char*)&SDAToSign[index], item->data, item->len);
    index += item->len;

	EMVBase_TraceHex("AmtAuthNum:", item->data, item->len);
	EMVBase_TraceHex("SDAToSign:", SDAToSign, index);

    EMVBase_Hash(SDAToSign, index, SDAHash);

    emvbase_free(SDAToSign);

    if(recovSDA->HashInd == 0x01)  //SHA-1 algorithm
    {
		EMVBase_TraceHex("hash result", recovSDA->HashResult, 20);
		EMVBase_TraceHex("SDAHash", SDAHash, 20);

        if(memcmp(recovSDA->HashResult, SDAHash, 20))
        {
            EMVBase_Trace("pure-error: hash result from Signed dynamic App Data is different from 9F4B\r\n");
            emvbase_free(recovSDA);
            return RLT_EMV_ERR;
        }
    }
    else
    {
        EMVBase_Trace("pure-error:Hash Indicator error,only support SHA-1\r\n");
        emvbase_free(recovSDA);
        return RLT_EMV_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_ICCDynNum, &recovSDA->ICCDynData[1], recovSDA->ICCDynData[0]);

    emvbase_free(recovSDA);
    return RLT_EMV_OK;
}
unsigned char pure_GetCAPK(PURETradeUnionStruct *tempApp_UnionStruct)
{
    return RLT_EMV_ERR;
}

unsigned char pure_RecoverCheckPK(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char ret;

    EMVBASE_CAPK_STRUCT *tempcapk;

    tempcapk = tempApp_UnionStruct->EMVTradeParam->CAPK;
    if(tempcapk->ModulLen==0)
    {
    	EMVBase_Trace("PURE-info: CAPK missing\r\n");
        if(pure_GetCAPK(tempApp_UnionStruct)!=RLT_EMV_OK)       //retrieval of the certificate authentication public key
        {
            return RLT_EMV_ERR;
        }
    }

    if(tempApp_UnionStruct->EMVTradeParam->IPKModulLen==0)
    {
    	ret = pure_GetIPK(tempApp_UnionStruct);
		EMVBase_Trace("PURE-info: pure_GetIPK ret = %d\r\n", ret);
        if(ret !=RLT_EMV_OK)        //retrieval of the issuer public key modulus
        {
            return ret;
        }
    }

    if(tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen==0)
    {
    	ret = pure_GetICCPK(tempApp_UnionStruct);
		EMVBase_Trace("PURE-info: pure_GetICCPK ret = %d\r\n", ret);
        if(ret !=RLT_EMV_OK)          //retrieval of the ICC public key
        {
            return ret;
        }
    }
    return RLT_EMV_OK;
}


unsigned char pure_AacRefuse(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char respCrpt;
    unsigned char CryptInfo = 0;

    respCrpt = emvbase_avl_gettagvalue(EMVTAG_ReferenceControlParameter);

    tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_OFFLINE_DECLINE;

    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if((CryptInfo & 0x08) == 0x08)		//Advice required
    {
        tempApp_UnionStruct->EMVTradeParam->bAdvice = 1;
    }

    if((respCrpt & 0x10) != 0)
    {
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);
    }
    return RLT_EMV_OK;
}


void pure_FillICCDynData(unsigned char* DynData, EMVBASE_ICC_DYN_DATA* ICCDynData)
{
    unsigned char Ldn;

    Ldn = DynData[0];
    ICCDynData->ICCDynNumLen = Ldn;
    memcpy(ICCDynData->ICCDynNum, DynData + 1, Ldn);
    ICCDynData->CryptInfo = DynData[Ldn + 1];
    memcpy(ICCDynData->AppCrypt, DynData + Ldn + 2, 8);
    memcpy(ICCDynData->HashResult, DynData + Ldn + 10, 20);
}

unsigned char pure_CombineDDASignVerify(unsigned char type, PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char *SDAToSign, SDAHash[20], *SDAData, i = 0;
    unsigned int index;
    EMVBASE_SIGN_DYNDATA_RECOVER *recovSDA;
    EMVBASE_ICC_DYN_DATA ICCDynDataSign;
    EMVBASETAGCVLITEM *SignDynAppDataitem, *ICCPKExpitem, *DOLDataitem;
    unsigned char CryptInfo;
    unsigned char UnpredictNum[4];


	EMVBase_Trace("PURE-info: Signed Dynamic Application Data(Tag9F4B) verification\r\n");

    if((emvbase_avl_checkiftagexist(EMVTAG_CryptInfo) == 0) || (emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData) == 0)
       || (emvbase_avl_checkiftagexist(EMVTAG_ATC) == 0))
    {
    	EMVBase_Trace("PURE-info: Signed Dynamic Application Data(Tag9F4B) or CID(Tag9F27) or ATC(Tag9F36) missing\r\n");
        return RLT_EMV_ERR;
    }
    memset((unsigned char*)&ICCDynDataSign, 0, sizeof(EMVBASE_ICC_DYN_DATA));

    memset(SDAHash, 0, 20);

    SignDynAppDataitem = emvbase_avl_gettagitempointer(EMVTAG_SignDynAppData);

    if(SignDynAppDataitem->len != tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen)
    {
    	EMVBase_Trace("PURE-info: ICC public key len: %d\r\n", tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen);
		EMVBase_Trace("PURE-info: Signed Dynamic Application Data(Tag9F4B) value len: %d\r\n", SignDynAppDataitem->len);
		EMVBase_Trace("PURE-info: different len\r\n");
        return RLT_EMV_ERR;
    }
    ICCPKExpitem = emvbase_avl_gettagitempointer(EMVTAG_ICCPKExp);

    SDAData = (unsigned char *)emvbase_malloc(248);
    EMVBase_RSARecover(tempApp_UnionStruct->EMVTradeParam->ICCPKModul, tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen, ICCPKExpitem->data, ICCPKExpitem->len, SignDynAppDataitem->data, SDAData);

    recovSDA = (EMVBASE_SIGN_DYNDATA_RECOVER *)emvbase_malloc(sizeof(EMVBASE_SIGN_DYNDATA_RECOVER));


    if(EMVBase_FillSDA(SDAData, recovSDA, tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen) != RLT_EMV_OK)
    {
    	EMVBase_Trace("PURE-info: Format of Data Recovered error\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataTrail != 0xBC)
    {
    	EMVBase_Trace("PURE-info: Data Trailer != 0xBC\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataHead != 0x6A)
    {
    	EMVBase_Trace("PURE-info: Data Header != 0x6A\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataFormat != 0x05)
    {
    	EMVBase_Trace("PURE-info: Certificate Format != 0x05\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    pure_FillICCDynData(recovSDA->ICCDynData, &ICCDynDataSign);

    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if(ICCDynDataSign.CryptInfo != CryptInfo)
    {
    	EMVBase_Trace("PURE-info: CID(Tag9F27) != CID in ICC Dynamic Data(recover data)\r\n");

        if((ICCDynDataSign.CryptInfo & 0xC0) == 0x80) // ??
        {
        	EMVBase_Trace("Emvb_CombineDDASignVerify 7\r\n");
            emvbase_free(recovSDA);
            emvbase_free(SDAData);
            return RLT_EMV_ERR;
        }
        else if((ICCDynDataSign.CryptInfo & 0xC0) == 0x40) // ??
        {
        	EMVBase_Trace("Emvb_CombineDDASignVerify 8\r\n");
            emvbase_free(recovSDA);
            emvbase_free(SDAData);
            return RLT_EMV_ERR;
        }
        else
        {
        	EMVBase_Trace("Emvb_CombineDDASignVerify 9\r\n");
            emvbase_free(recovSDA);
            emvbase_free(SDAData);
            return RLT_EMV_ERR;
        }
    }

    SDAToSign = (unsigned char *)emvbase_malloc(512);
    index = 0;
    memset(SDAToSign, 0, 512);
    memcpy(SDAToSign, (unsigned char*)&SDAData[1], tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen - 22);
    index += tempApp_UnionStruct->EMVTradeParam->ICCPKModulLen - 22;

    emvbase_free(SDAData);

    if(GenerateAC2 == type)
    {
        i = tempApp_UnionStruct->EMVTradeParam->bCDOL2HasNoUnpredictNum;
    }

    if(GenerateAC1 == type)
    {
        i = tempApp_UnionStruct->EMVTradeParam->bCDOL1HasNoUnpredictNum;
    }

    if(i == 0)
    {
        emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UnpredictNum, 0, 4);
        memcpy((unsigned char*)&SDAToSign[index], UnpredictNum, 4);
        index += 4;
    }
    else
    {
    }

    if(recovSDA->HashInd != 0x01)		//SHA-1 algorithm
    {
    	EMVBase_Trace("PURE-info: Hash Algorithm Indicator != 0x01\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAToSign);
        return RLT_EMV_ERR;
    }
    EMVBase_Hash(SDAToSign, index, SDAHash);

    if(memcmp(recovSDA->HashResult, SDAHash, 20))
    {
    	EMVBase_Trace("PURE-info: Hash Result verification fails\r\n");
        emvbase_free(recovSDA);
        emvbase_free(SDAToSign);
        return RLT_EMV_ERR;
    }
    emvbase_free(recovSDA);

    memset(SDAToSign, 0, 512);
    memset(SDAHash, 0, 20);
    index = 0;


    DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_PDOLData);

    if(DOLDataitem != NULL)
    {
        if(DOLDataitem->len)
        {
            memcpy(SDAToSign, DOLDataitem->data, DOLDataitem->len);
            index += DOLDataitem->len;
        }
    }

    DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_CDOL1RelatedData);

    if(DOLDataitem != NULL)
    {
        if(DOLDataitem->len)
        {
            memcpy(SDAToSign + index, DOLDataitem->data, DOLDataitem->len);
            index += DOLDataitem->len;
        }
    }

	if(type == GenerateAC2)
    {
        DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_CDOL2RelatedData);

        if(DOLDataitem != NULL)
        {
            if(DOLDataitem->len)
            {
                memcpy(SDAToSign + index, DOLDataitem->data, DOLDataitem->len);
                index += DOLDataitem->len;
            }
        }
    }
    memcpy(SDAToSign + index, tempApp_UnionStruct->EMVTradeParam->GenerateACRetData, tempApp_UnionStruct->EMVTradeParam->GenerateACRetDataLen);
    index += tempApp_UnionStruct->EMVTradeParam->GenerateACRetDataLen;


    EMVBase_Hash(SDAToSign, index, SDAHash);

    emvbase_free(SDAToSign);

    if(memcmp(ICCDynDataSign.HashResult, SDAHash, 20))
    {
    	EMVBase_Trace("PURE-info: Transaction Data Hash Code in ICC Dynamic Data(recover data) verification fails\r\n");
        return RLT_EMV_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_ICCDynNum, ICCDynDataSign.ICCDynNum, ICCDynDataSign.ICCDynNumLen);
    emvbase_avl_createsettagvalue(EMVTAG_AppCrypt, ICCDynDataSign.AppCrypt, 8);


    return RLT_EMV_OK;
}

unsigned char pure_CombineDDA(unsigned char type, PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;

    retCode = pure_CombineDDASignVerify(type, tempApp_UnionStruct);

    if(retCode != RLT_EMV_OK)
    {
        return retCode;
    }
    return RLT_EMV_OK;
}

unsigned char pure_GAC1AndAnalys(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char TermAnaResult)
{
	unsigned char retCode;
	unsigned char TransactionMode;
	EMVBASE_APDU_RESP apdu_r;
	EMVBASE_APDU_SEND apdu_s;
	unsigned char AIP[2] = {0};
	unsigned char ODAFail = 0;
	unsigned char ImpleOption = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	unsigned char CCIDInGPO;

	// shall recover IPK and ICCPK before GAC--20210420
	if(0x00 != tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator)//ODA Select CDA/DDA/SDA,goto recover and check CAPK,IPK,ICCPK
	{
		EMVBase_Trace( "PURE-info: Recover and Check CAPK,IPK,ICCPK\r\n");

		if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData) //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
		{
			EMVBase_Trace("PURE-info: Tag9F4A error(len is not equal to 1 or value is not equal to '82')\r\n");
			EMVBase_Trace("or data need to be authenticated returned by read app data is not format '70'\r\n");
			ODAFail = 1;
		}
		else
		{
			retCode = pure_RecoverCheckPK(tempApp_UnionStruct); // Get CAPK,IPK,ICCPK
			if(retCode != RLT_EMV_OK)
			{
				if(RLT_EMV_OTHER == retCode)	//for FIME Tool test case CDA_PROC_04,if 92 need but not present,consider it as CDA not fail
				{
					ODAFail = 0;
				}
				else
				{
					ODAFail = 1;
				}
			}
		}

		if(ODAFail)
		{
			EMVBase_Trace("PURE-info: set TVR byte1 bit3 = 1\r\n");
			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);		//Offline Combined DDA/AC Generation failed
		}
	}

	retCode = pure_CommandGAC1(tempApp_UnionStruct, TermAnaResult, &apdu_s);

	if(retCode != RLT_EMV_OK)
	{
		return retCode;
	}

//	if(tempApp_UnionStruct->EMVTradeParam->CCIDExistInGPO)
//	{
//		CCIDInGPO = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
//		emvbase_avl_deletetag(EMVTAG_PURECCID); //if CCID present in GPO,delete tag in avl to ensure check CCID whether present in GAC response correctly
//		EMVBase_Trace("PURE-info: apdu timeout or other error\r\n");
//	}

	tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

	//K14.4
	if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
	{
		EMVBase_Trace("PURE-error: apdu timeout or other error\r\n");
		if(0x00 == tempApp_UnionStruct->EMVTradeParam->ECHOCommandSupport)
		{
			return pure_Outcome_CommunicationErrors_PaymentTransactionNotCompleted(tempApp_UnionStruct);
		}
		else
		{
			tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator = 0x01;
			pure_Outcome_CommunicationErrors_UnknownPaymentResult(tempApp_UnionStruct);
			return RLT_EMV_TORN;
		}
	}

	retCode = pure_DealGAC1Rsp(tempApp_UnionStruct, &apdu_r, CCIDInGPO, TermAnaResult);
	return retCode;
}

unsigned char pure_TermiAnalys(PURETradeUnionStruct *tempApp_UnionStruct)//no TSI
{
	unsigned char transtype, refundflag = 0;
	unsigned char IACDecline[5], IACOnline[5],IACDefault[5];
	unsigned char retCode = RLT_EMV_OK;
	unsigned char TermAnaResult;
	unsigned char DTIP[3],tvr[5];
    unsigned short len;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char CDOL1bExist,ForceOnlineFlag;
    EMVBASETAGCVLITEM *item = NULL;
	unsigned char SZZTICS[7];
	static unsigned char callbackFlag=0;

	transtype = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
	if(transtype == 0x20)//refund
	{
		TermAnaResult = TAA_DENIAL;
		tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator = 0x00;
	}
	else
	{
		item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL1, &CDOL1bExist);

		if(CDOL1bExist)
		{
			if(pure_DOLProcess(typeCDOL1, item->data, item->len, apdu_s.DataIn, &len, 255,tempApp_UnionStruct) != RLT_EMV_OK)//20201217 lsy
			{
				if(pure_CheckIfdoCDA(tempApp_UnionStruct))
				{
					emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);
					pure_DOLProcess(typeCDOL1, item->data, item->len, apdu_s.DataIn, &len, 255,tempApp_UnionStruct);//20201217 lsy
				}
			}
		}
		else
		{
			EMVBase_Trace("pure-warning: CDOL1(8C) missing,select next AID\r\n");
			return pure_Outcome_TransactionCompletedOnError(tempApp_UnionStruct);	//if 8C miss, select next
		}
		TermAnaResult = pure_CompareIACTAC(tempApp_UnionStruct);
	}

	if(0 == callbackFlag)
	{
		callbackFlag++;
		return PURE_REQ_FORCEONLINE;
	}
	else
	{
		ForceOnlineFlag = emvbase_avl_gettagvalue(EMVTAG_bForceOnline);
		if(ForceOnlineFlag)
		{
			TermAnaResult = TAA_ONLINE;
			emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x08);
		}
		callbackFlag = 0;
	}

	EMVBase_Trace("pure-info: Term Decide Trans Result: %02X\r\n", TermAnaResult);

	retCode = pure_GAC1AndAnalys(tempApp_UnionStruct, TermAnaResult);
	EMVBase_Trace("pure-info: pure_GAC1AndAnalys ret = %d\r\n", retCode);
	return retCode;
}

unsigned char pure_TransResultAnalysis(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char retCode;
	unsigned char CryptInfo;
	unsigned char TransactionMode;
	unsigned char respCrpt = 0;

	CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
	EMVBase_Trace( "pure-info: CID=%02x\r\n", CryptInfo);

	if((CryptInfo&0xC0)==0x00)
	{
		tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
//		return RLT_EMV_OFFLINE_DECLINE;
	}
    else if((CryptInfo&0xC0)==0x40)
    {
		tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_OFFLINE_APPROVE;
    }
	else if((CryptInfo&0xC0)==0x80)
	{
		tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_ONLINE_WAIT;
	}
	else
    {
		tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_TERMINATE;
    }

	return RLT_EMV_OK;
}

unsigned char pure_TransProcess(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	unsigned char Impletion = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	unsigned char KernelCap[5];
	unsigned char CID = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
	unsigned char CCIDExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);
	unsigned char CCID = emvbase_avl_gettagvalue(EMVTAG_PURECCID);

	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	//K18.3
	if((PURE_TRANS_APPAUTHTRANS == TransType) && (Impletion & 0x10) && (KernelCap[3] & 0x40))
	{
		return pure_Outcome_NonFinancialCompleted_OfflineAuthentication(tempApp_UnionStruct);
	}


	//K18.4
	if(0x40 == (CID & 0xC0))	//TC
	{
		return pure_Outcome_FinancialTransactionCompleted_TransactionApproved(tempApp_UnionStruct);
	}
	else if(0x00 == (CID & 0xC0))	//AAC
	{
		return pure_Outcome_FinancialTransactionCompleted_TransactionDeclined(tempApp_UnionStruct);
	}
	else if(0x80 == (CID & 0xC0))	//ARQC
	{
		if(CCIDExist && (CCID & 0x08) && (KernelCap[2] & 0x80))
		{
			return pure_Outcome_FinancialTransactionCompleted_OnlineRequest2ndTap(tempApp_UnionStruct);
		}
		else
		{
			if(KernelCap[2] & 0x10)
			{
				return pure_Outcome_FinancialTransactionCompleted_OnlineRequestLongTap(tempApp_UnionStruct);
			}
			else
			{
				return pure_Outcome_FinancialTransactionCompleted_OnlineRequestNoAdditionalTap(tempApp_UnionStruct);
			}
		}
	}

}

void pure_FillSSA(unsigned char* SSAData, SIGN_STAT_APPDATA_RECOVER* recovSSA, unsigned char tempIPKModulLen)
{
    unsigned char i;

    recovSSA->DataHead = SSAData[0];
    recovSSA->DataFormat = SSAData[1];
    recovSSA->HashInd = SSAData[2];
    memcpy(recovSSA->DataAuthCode, (unsigned char*)&SSAData[3], 2);
    memcpy(recovSSA->PadPattern, (unsigned char*)&SSAData[5], tempIPKModulLen - 26);

    for(i = 0; i < 20; i++)
    {
        recovSSA->HashResult[i] = SSAData[tempIPKModulLen - 21 + i];
    }

    recovSSA->DataTrail = SSAData[tempIPKModulLen - 1];
}

unsigned char pure_SDVerify(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char SSAHash[20];
    unsigned short index = 0;
    unsigned char *SSAData;
    SIGN_STAT_APPDATA_RECOVER *recovSSA;
    unsigned char SignStatAppDatabExist, IPKExpbExist;
    EMVBASETAGCVLITEM *SignStatAppDataitem = NULL, *IPKExpitem = NULL;
	EMVBASETAGCVLITEM *item = NULL;
	EMVBASETAGCVLITEM *item2 = NULL;

    unsigned char *SSAToSign;
	unsigned short fixlen = 0;
	unsigned short offset = 0;


    SignStatAppDataitem = emvbase_avl_gettagitemandstatus(EMVTAG_SignStatAppData, &SignStatAppDatabExist);
    IPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_IPKExp, &IPKExpbExist);

    if(SignStatAppDatabExist == 0 || IPKExpbExist == 0)
    {
        return RLT_EMV_ERR;
    }

    if(SignStatAppDataitem->len != tempApp_UnionStruct->EMVTradeParam->IPKModulLen)
    {
        return RLT_EMV_ERR;
    }

    SSAData = (unsigned char *)emvbase_malloc(248);
    memset(SSAData, 0, 248);
    EMVBase_RSARecover(tempApp_UnionStruct->EMVTradeParam->IPKModul, tempApp_UnionStruct->EMVTradeParam->IPKModulLen, IPKExpitem->data, IPKExpitem->len, SignStatAppDataitem->data, SSAData);

    recovSSA = (SIGN_STAT_APPDATA_RECOVER *)emvbase_malloc(sizeof(SIGN_STAT_APPDATA_RECOVER));
    pure_FillSSA(SSAData, recovSSA, tempApp_UnionStruct->EMVTradeParam->IPKModulLen);


    if(recovSSA->DataTrail != 0xBC)
    {
        emvbase_free(SSAData);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

    if(recovSSA->DataHead != 0x6A)
    {
        emvbase_free(SSAData);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

    if(recovSSA->DataFormat != 0x03)
    {
        emvbase_free(SSAData);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }
    SSAToSign = (unsigned char *)emvbase_malloc(2048 + 512);


    index = 0;
    memcpy(SSAToSign, (unsigned char*)&SSAData[1], tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22);
    index += tempApp_UnionStruct->EMVTradeParam->IPKModulLen - 22;

    emvbase_free(SSAData);

    if((index + tempApp_UnionStruct->EMVTradeParam->AuthDataLen) > (2048 + 512))
    {
        emvbase_free(SSAToSign);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

	fixlen = index;

	EMVBase_TraceHex("EmvContact_SDVerify-AuthData: ", tempApp_UnionStruct->EMVTradeParam->AuthData, tempApp_UnionStruct->EMVTradeParam->AuthDataLen);

	memcpy((unsigned char*)&SSAToSign[index], tempApp_UnionStruct->EMVTradeParam->AuthData, tempApp_UnionStruct->EMVTradeParam->AuthDataLen);
    index += tempApp_UnionStruct->EMVTradeParam->AuthDataLen;

	EMVBase_Hash(SSAToSign, index, SSAHash);

    if(recovSSA->HashInd == 0x01)  //SHA-1 algorithm
    {
        if(memcmp(recovSSA->HashResult, SSAHash, 20))
        {
            emvbase_free(SSAToSign);
            emvbase_free(recovSSA);
            return RLT_EMV_ERR;
        }
    }
    else
    {
        emvbase_free(SSAToSign);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

    emvbase_avl_createsettagvalue(EMVTAG_DataAuthCode, recovSSA->DataAuthCode, 2);

    emvbase_free(SSAToSign);
    emvbase_free(recovSSA);
    return RLT_EMV_OK;
}

unsigned char pure_DataAuth_SDA(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char ODAFail=0;
	EMVBASE_CAPK_STRUCT *tempcapk = tempApp_UnionStruct->EMVTradeParam->CAPK;

	emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x02);
	if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData) 		 //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object
	{
		ODAFail = 1;
	}
	else
	{
		if(tempcapk == NULL || tempcapk->ModulLen == 0)
		{
			ODAFail = 1;
		}

		if(RLT_EMV_OK != pure_CheckDataMissSDA(tempApp_UnionStruct))
		{
			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);		//ICC data missing
			ODAFail = 1;
		}

		if(tempApp_UnionStruct->EMVTradeParam->IPKModulLen == 0)
		{
			if(RLT_EMV_OK != pure_GetIPK(tempApp_UnionStruct)) 	//retrieval of the issuer public key modulus
			{
				ODAFail = 1;
			}
		}

		if(RLT_EMV_OK != pure_SDVerify(tempApp_UnionStruct))				//Verification of the Signed Static Application Data by the terminal
		{
			ODAFail = 1;
		}
	}

	if(ODAFail)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x40);
		return RLT_EMV_ERR;
	}
	else
	{
		emvbase_avl_setvalue_and(EMVTAG_TVR, 0, 0xBF);
		return RLT_EMV_OK;
	}
}

unsigned char pure_DataAuth_CDA(PURETradeUnionStruct * tempApp_UnionStruct)
{
    unsigned char ODAFail = 0;
	if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData) 		 //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
	{
		ODAFail = 1;
	}
	else
	{
		if(pure_initDynamicAuth(tempApp_UnionStruct) != RLT_EMV_OK)
		{
			ODAFail = 1;
		}
	}

	if(ODAFail)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);	   //Offline Combined DDA/AC Generation failed
		emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);	   //set bit 'Offline Data Authentication was performed' bit 1
		return RLT_EMV_ERR;
	}

	return pure_CombineDDASignVerify(GenerateAC1 ,tempApp_UnionStruct);
}

unsigned char pure_DataAuth_DDA(PURETradeUnionStruct * tempApp_UnionStruct)
{
    unsigned char ODAFail = 0;
	unsigned char retCode;
	EMVBASE_APDU_SEND apdu_s;

	if(tempApp_UnionStruct->EMVTradeParam->bErrSDATL || tempApp_UnionStruct->EMVTradeParam->bErrAuthData) 		 //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
	{
		ODAFail = 1;
	}
	else
	{
		if(pure_initDynamicAuth(tempApp_UnionStruct) != RLT_EMV_OK)
		{
			ODAFail = 1;
		}
	}

	if(ODAFail)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);	   //Offline Combined DDA/AC Generation failed
		emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);	   //set bit 'Offline Data Authentication was performed' bit 1
		return RLT_EMV_ERR;
	}

	return pure_DynSignVerify(tempApp_UnionStruct);
}

unsigned char pure_DataAuth(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char retCode;
	unsigned char ODAIndicator = tempApp_UnionStruct->EMVTradeParam->OfflineCAMSelectedIndicator;
	unsigned char KernelCap[5];
	unsigned char TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	unsigned char Impletion = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;

	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);

	//K17.3
	if(0x01 == ODAIndicator)	//SDA
	{
		EMVBase_Trace("pure-info: start perform SDA\r\n");
		if((RESULT_OFFLINE_APPROVE == tempApp_UnionStruct->EMVTradeParam->TransResult || RESULT_OFFLINE_DECLINE == tempApp_UnionStruct->EMVTradeParam->TransResult)
			|| (KernelCap[1] & 0x04) || ((PURE_TRANS_APPAUTHTRANS == TransType) && (Impletion & 0x10) && (KernelCap[3] & 0x40)))
		{
			retCode = pure_DataAuth_SDA(tempApp_UnionStruct);
			if((RLT_EMV_OK != retCode) && (0x00 == (Impletion & 0x10) || PURE_TRANS_APPAUTHTRANS != TransType))
			{
				emvbase_avl_setvalue_and(EMVTAG_CryptInfo, 0, 0x3F);	//CID set to ‘00’ – AAC
			}
		}
	}
	else if(0x02 == ODAIndicator)	//CDA
	{
		EMVBase_Trace("pure-info: start perform CDA\r\n");
		retCode = pure_DataAuth_CDA(tempApp_UnionStruct);
		if(RLT_EMV_OK != retCode)
		{
			emvbase_avl_setvalue_and(EMVTAG_CryptInfo, 0, 0x3F);	//CID set to ‘00’ – AAC
		}

	}
	else if(0x03 == ODAIndicator)	//DDA
	{
		EMVBase_Trace("pure-info: start perform CDA\r\n");
		retCode = pure_DataAuth_DDA(tempApp_UnionStruct);
	}

	return RLT_EMV_OK;
}

unsigned char pure_CardHolderVerf_onlinepin(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char tempCreditPwd[20];
	unsigned char retCode=RLT_EMV_ERR;
	static unsigned char callbackFlag = 0;

	memset(tempCreditPwd,0,sizeof(tempCreditPwd));

	if(callbackFlag == 0)
	{
		callbackFlag++;
		return PURE_REQ_INPUTPIN;
	}
	else
	{
		 retCode = tempApp_UnionStruct->GetInputPINRes();
		 callbackFlag = 0;
	}

	if(retCode == RLT_ERR_EMV_InputBYPASS)
    {
        //emvbase_avl_setvalue_or(EMVTAG_TVR,2,0x08);
		//emvbase_avl_setvalue_and(EMVTAG_TVR,2,0xDF);
		EMVBase_Trace("PURE-info: Input PIN bypass\r\n");
		return PURE_ERR_CVMFail;
    }
	else if(retCode == RLT_ERR_EMV_CancelTrans)
    {
    	return RLT_ERR_EMV_CancelTrans;
    }
    else if(retCode == RLT_EMV_OK)
    {

    }
    else// if(retCode == RLT_EMV_ERR)
    {
    	//emvbase_avl_setvalue_or(EMVTAG_TVR,2,0x10);
		return PURE_ERR_CVMFail;
    }

	return retCode;
}

unsigned char pure_InitCardHolderVerify(PURETradeUnionStruct *tempApp_UnionStruct, CVMSTRCUT *CVMdata)
{
    unsigned char i;
    unsigned char AIP[2];
    unsigned char CVMResult[3];
    unsigned char CVMListbExist;
    EMVBASETAGCVLITEM *CVMListitem;

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);

	EMVBase_TraceHex("pure-info: AIP",AIP,2);

    if(!(AIP[0] & 0x10))
    {
        CVMResult[0] = PURE_CVMMETHOD_DEFAULT;
        CVMResult[1] = 0;
        CVMResult[2] = CVMR_UNKNOWN;
        emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);
        return RLT_EMV_OK;
    }

    //get cvm list in the card, if not available, return
    CVMListitem = emvbase_avl_gettagitemandstatus(EMVTAG_CVMList, &CVMListbExist);

    if(CVMListitem->len < 10 || (CVMListitem->len % 2))
    {
    	EMVBase_Trace("pure-info: CVM List len is not correct. Len is %d\r\n",CVMListitem->len);
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);	//ICC Data Missing
        CVMResult[0] = PURE_CVMMETHOD_DEFAULT;
        CVMResult[1] = 0;
        CVMResult[2] = CVMR_UNKNOWN;
        emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);
        return RLT_ERR_EMV_IccDataFormat;
    }

	EMVBase_TraceHex("CVM List 8E",CVMListitem->data,CVMListitem->len);

    memcpy(&CVMdata->CVM_X[2], CVMListitem->data, 4);
    memcpy(&CVMdata->CVM_Y[2], &CVMListitem->data[4], 4);

    CVMdata->CVRListLen = (CVMListitem->len - 8) >> 1;

    for(i = 0; i < CVMdata->CVRListLen; i++)
    {
        CVMdata->CVRList[i].method = CVMListitem->data[i * 2 + 8];
        CVMdata->CVRList[i].condition = CVMListitem->data[i * 2 + 9];
    }


    CVMResult[0] = PURE_CVMMETHOD_DEFAULT;
    CVMResult[1] = 0;
    CVMResult[2] = CVMR_FAIL;
    emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);

    return RLT_EMV_OTHER;
}

unsigned char pure_CheckIfSupportCVM(unsigned char method)
{
	unsigned char temp;
	unsigned char TTPI[5];

	temp = method & 0x3F;

	memset(TTPI, 0, sizeof(TTPI));
	emvbase_avl_gettagvalue_spec(EMVTAG_PURETTPI, TTPI, 0, 5);

    switch(temp)
    {
		case 0x00:	//Fail CVM processing
			return 1;

		case 0x01: //Plaintext PIN verification performed by ICC
			return 0;	//Offline PIN is always considered as not supported by the terminal

		case 0x02: //enciphered PIN verification online
			if(TTPI[0] & 0x04)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		case 0x03: //Plaintext PIN verification performed by ICC and signature(paper)
			return 0;

		case 0x04: //enciphered PIN verification performed by ICC
			return 0;

		case 0x05: //enciphered PIN verification performed by ICC and signature(paper)
			return 0;

		case 0x1E: //signature (paper)
			if(TTPI[0] & 0x02)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		case 0x1F: //no CVM required
			if(TTPI[1] & 0x40)
			{
				return 0;
			}
			else
			{
				return 1;
			}

		case 0x20: //PBOC ID
			return 0;

		case 0x24: //Confirmation Code checked using the Customer contactless device
			if(TTPI[2] & 0x40)
			{
				return 1;
			}
			else
			{
				return 0;
			}

		default:
		    return 0;
    }

    return 0;
}

unsigned char pure_AnalyCvmlist_cmpamount(unsigned char *CVMamount,unsigned char type)
{
    unsigned char TransCurcyCodebExist,TransCurcyCode[2];
    unsigned char TermAmtAuth[6],TermAmtCVM[6];
	unsigned char AppCurcyCodebExist,AppCurcyCode[2];
	unsigned int TermAmt;

	memset(TransCurcyCode,0,sizeof(TransCurcyCode));
    TransCurcyCodebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TransCurcyCode,TransCurcyCode,0,2));

	memset(AppCurcyCode,0,sizeof(AppCurcyCode));
	AppCurcyCodebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_AppCurcyCode,AppCurcyCode,0,2));

    memset(TermAmtAuth,0,sizeof(TermAmtAuth));
	emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum,TermAmtAuth,0,6);

	if(TransCurcyCodebExist==0x00 || AppCurcyCodebExist == 0)		//Application Currency Code is not present
	{
		return 0;
	}


	if( !memcmp(TransCurcyCode,AppCurcyCode,2))			//under x shouldn't include case of equal x.(EMV2000 2CJ.077.02)
	{
		TermAmt = EMVBaseByteArrayToInt(&CVMamount[2],4);
		memset(TermAmtCVM,0,sizeof(TermAmtCVM));
		EMVBaseU32ToBcd(TermAmtCVM,TermAmt,6);
		EMVBase_TraceHex("pure-info: CVM Amount in BCD 6Byte:", TermAmtCVM, 6);
		if(type == 0)
		{
			if(memcmp(TermAmtAuth,TermAmtCVM,6) < 0)
			{
				return 1;
			}
		}
		else
		{
			if(memcmp(TermAmtAuth,TermAmtCVM,6) > 0)
			{
				return 1;
			}
		}
	}

	return 0;

}

unsigned char pure_AnalyCvmlist(unsigned char method, unsigned char condition, unsigned char TransType, unsigned char TermType, unsigned char *CVM_X, unsigned char *CVM_Y)
{
	unsigned char needcvm = 0, nextcvm = 0;
	unsigned char returnvalue;
	unsigned char TermAmtOther[6];
	unsigned char TermAmtOthernotzero = 0;
	unsigned char ret;

	EMVBase_Trace("pure-info: pure_AnalyCvmlist condition = %d\r\n", condition);
    switch(condition)
    {
         case 0x00:       //always
			needcvm = 1;
            break;

         case 0x01:       //if cash or cashback
			if(TransType == PURE_TRANS_CASH && (TermType & 0x0F) > 3 )
			{
				needcvm = 1;
			}
			else
			{
				nextcvm = 1;
			}
            break;

         case 0x02:       //if not cash or cashback
			memset(TermAmtOther, 0, sizeof(TermAmtOther));
			emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum, TermAmtOther, 0, 6);

			if(memcmp(TermAmtOther, "\x00\x00\x00\x00\x00\x00", 6))
			{
				TermAmtOthernotzero = 1;
			}

			if(TransType != PURE_TRANS_CASH  && TransType != PURE_TRANS_CASHBACK && TermAmtOthernotzero == 0)
			{
				needcvm = 1;
			}
			else
			{
				nextcvm = 1;
			}
			break;

         case 0x03:       //if terminal support CVM
			if(pure_CheckIfSupportCVM(method))
			{
				 needcvm = 1;
			}
			else
			{
				 nextcvm = 1;
			}
			break;

         case 0x04:       //if manual cash (added in EMV4.1,SU16)
            if(TransType == PURE_TRANS_CASH && (TermType & 0x0F) <= 3 )
             {
                 needcvm = 1;
             }
             else
			 {
			 	nextcvm = 1;
			 }
             break;

         case 0x05:       //if purchase with cashback (added in EMV4.1,SU16)
            {memset(TermAmtOther, 0, sizeof(TermAmtOther));
             emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum, TermAmtOther, 0, 6);

             if(memcmp(TermAmtOther, "\x00\x00\x00\x00\x00\x00", 6))
             {
                 TermAmtOthernotzero = 1;
             }

             if(TransType == PURE_TRANS_CASHBACK|| TermAmtOthernotzero )
             {
                 needcvm = 1;
             }
             else{ nextcvm = 1; }}
                                 break;

		case 0x06:       //if trans is in App currency and under X value
			ret  = pure_AnalyCvmlist_cmpamount(CVM_X, 0);
			EMVBase_Trace("pure_AnalyCvmlist_cmpamount ret = %d\r\n", ret);
			if(ret == 0)
			{
				nextcvm = 1;
			}
			else
			{
				needcvm = 1;
			}
			break;

		case 0x07:       //if trans is in App currency and over X value
			ret  = pure_AnalyCvmlist_cmpamount(CVM_X, 1);
			EMVBase_Trace("pure_AnalyCvmlist_cmpamount ret = %d\r\n", ret);
			if(ret == 0)
			{
				nextcvm = 1;
			}
			else
			{
				needcvm = 1;
			}
			break;

         case 0x08:       //if trans is in App currency and under Y value
            {ret  = pure_AnalyCvmlist_cmpamount(CVM_Y, 0);

             if(ret == 0)
             {
                 nextcvm = 1;
             }
             else
             {
                 needcvm = 1;
             }}
              break;

         case 0x09:       //if trans is in App currency and over Y value
            {ret  = pure_AnalyCvmlist_cmpamount(CVM_Y, 1);

             if(ret == 0)
             {
                 nextcvm = 1;
             }
             else
             {
                 needcvm = 1;
             }}
              break;

         default:
            {
                nextcvm = 1;
            }
            break;
    }

	EMVBase_Trace("pure-info: pure_AnalyCvmlist nextcvm = %d\r\n", nextcvm);
	EMVBase_Trace("pure-info: pure_AnalyCvmlist needcvm = %d\r\n", needcvm);

	returnvalue = 0;
	if(nextcvm)
	{
		returnvalue |= 0x0F;
	}
	if(needcvm)
	{
		returnvalue |= 0xF0;
	}
//	returnvalue = nextcvm;
//    returnvalue = (returnvalue << 8) | needcvm;

	EMVBase_Trace("pure-info: pure_AnalyCvmlist returnvalue = %02X\r\n", returnvalue);
    return returnvalue;
}

unsigned char pure_PerformCVM(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char method, unsigned char condition, unsigned char *TTPI,unsigned char BypassFlag)
{
    unsigned char retCode = 0;
    unsigned char temp=0;
    unsigned char CVMResult[3]={0};
    temp = method & 0x3F;

	EMVBase_Trace("pure-info: pure_PerformCVM temp = %02X\r\n", temp);
	switch(temp)
    {
		case 0x00:	//Fail CVM processing
			CVMResult[2] = CVMR_FAIL;
			retCode = PURE_ERR_CVMFailALWAYS;
			break;

		case 0x01:           //Plaintext PIN verification performed by ICC
//			emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
			return PURE_ERR_CVMNoSupport;

		case 0x02:          //enciphered PIN verification online
			if(0 == (TTPI[1] & 0x40))
			{
				emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
				return PURE_ERR_CVMNoSupport;
			}
			emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x04);
			if(BypassFlag)
			{
				retCode = RLT_ERR_EMV_InputBYPASS;
				break;
			}

			CVMResult[2] = CVMR_UNKNOWN;
			tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_ONLINEPIN;
			retCode = pure_CardHolderVerf_onlinepin(tempApp_UnionStruct);
			break;

		case 0x03:         //Plaintext PIN verification performed by ICC and signature(paper)
//			emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
			return PURE_ERR_CVMNoSupport;

		case 0x04:         //enciphered PIN verification performed by ICC
//			emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
			return PURE_ERR_CVMNoSupport;

		case 0x05:        //enciphered PIN verification performed by ICC and signature(paper)
//			emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
			return PURE_ERR_CVMNoSupport;

		case 0x1E:       //signature (paper)
			if(0 == (TTPI[0] & 0x02))
			{
				return PURE_ERR_CVMNoSupport;
			}
			tempApp_UnionStruct->EMVTradeParam->bPrintReceipt = 1;
			CVMResult[2] = CVMR_UNKNOWN;
			retCode = RLT_EMV_OK;
			tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_OBTAINSIGNATURE;
			break;

		case 0x1F:       //no CVM required
			if(TTPI[1] & 0x40)
			{
				return PURE_ERR_CVMNoSupport;
			}
			emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0x7F);
			CVMResult[2] = CVMR_SUCCESS;
			retCode = RLT_EMV_OK;
			tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_NOCVMREQ;
			break;

		case 0x20:
			return PURE_ERR_CVMNoSupport;

		case 0x24:
			CVMResult[2] = CVMR_UNKNOWN;
			tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_CONFVERIFIED;
			break;

		default:
			emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x40);
			CVMResult[2] = CVMR_FAIL;
			return PURE_ERR_CVMFail;

    }

    CVMResult[0] = method;
    CVMResult[1] = condition;

    emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);

    return retCode;
}

unsigned char pure_CardHolderVerfUseCVMList(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode;
    CVMSTRCUT CVMdata;
    unsigned char NowCVRListLen = 0;
    unsigned char needcvm, nextcvm;
    unsigned char needcvmnextcvm;
    unsigned char TransType, TermType;
    unsigned char TermCapab[3];
    unsigned char BypassFlag = 0;
	unsigned char TTPI[5];

    memset(&CVMdata, 0, sizeof(CVMSTRCUT));
    retCode = pure_InitCardHolderVerify(tempApp_UnionStruct, &CVMdata);

    if(retCode == RLT_EMV_OK)
    {
        return RLT_EMV_OK;
    }
	else if(RLT_ERR_EMV_IccDataFormat == retCode)
	{
		return RLT_EMV_TERMINATE_TRANSERR;
	}

    TransType = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
    TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);

    memset(TTPI, 0, sizeof(TTPI));
    emvbase_avl_gettagvalue_spec(EMVTAG_PURETTPI, TTPI, 0, 5);

	EMVBase_TraceHex("pure-info: TTPI C7",TTPI,5);
	EMVBase_Trace("pure-info: TransType 9C:%02X\r\n",TransType);
	EMVBase_Trace("pure-info: TermType 9F35:%02X\r\n",TermType);

	EMVBase_Trace("pure-info: CVRListLen:%d\r\n",CVMdata.CVRListLen);

    while(NowCVRListLen < CVMdata.CVRListLen)
    {

		EMVBase_Trace("pure-info: NowCVRListLen:%d\r\n",NowCVRListLen);

        needcvmnextcvm = 0;
        needcvmnextcvm = pure_AnalyCvmlist(CVMdata.CVRList[NowCVRListLen].method, CVMdata.CVRList[NowCVRListLen].condition, TransType, TermType, CVMdata.CVM_X, CVMdata.CVM_Y);


        needcvm = needcvmnextcvm & 0xF0;
        nextcvm = needcvmnextcvm & 0x0F;

		EMVBase_Trace("pure-info: needcvm:%d\r\n",needcvm);
		EMVBase_Trace("pure-info: nextcvm:%d\r\n",nextcvm);
//		if(needcvm == 1)
		if(needcvm)
        {
            retCode = pure_PerformCVM(tempApp_UnionStruct, CVMdata.CVRList[NowCVRListLen].method, CVMdata.CVRList[NowCVRListLen].condition, TTPI, BypassFlag);

            if(RLT_EMV_OK == retCode || PURE_REQ_INPUTPIN == retCode)
            {
                return retCode;
            }
            else if(RLT_ERR_EMV_InputBYPASS == retCode)//add for subsequent bypass
            {
                BypassFlag = 1;
            }
            else if(retCode == RLT_ERR_EMV_IccReturn || retCode == RLT_ERR_EMV_CancelTrans
				|| retCode == RLT_ERR_EMV_APDUTIMEOUT || retCode == RLT_EMV_TERMINATE_TRANSERR)
            {
                return retCode;
            }
            else if(PURE_ERR_CVMFailALWAYS == retCode)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
                return RLT_EMV_OK;
            }

            if((CVMdata.CVRList[NowCVRListLen].method & 0x40) == 0x00)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
                return RLT_EMV_OK;
            }
            else{ nextcvm = 1; }}
//        if(nextcvm == 1)
        if(nextcvm)
        {
            NowCVRListLen++;
        }
    }

    emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);	//set "Cardholder verification was not successful"
    return RLT_EMV_OK;
}


unsigned char pure_CardHolderVerf(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char retCode = RLT_EMV_OK;
	unsigned char CVMRes[3];
	unsigned char TTPI[5];
	unsigned char CCID;
	unsigned char KernelCap[5];
	unsigned char AIP[2];

	emvbase_avl_createsettagvalue(EMVTAG_CVMResult, "\x3F\x00\x00", 3);
	memset(CVMRes, 0, 3);

	emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
	EMVBase_TraceHex("pure-info: AIP", AIP, 2);

	emvbase_avl_gettagvalue_spec(EMVTAG_PURETTPI, TTPI, 0, 5);
	EMVBase_TraceHex("pure-info: TTPI", TTPI, 5);

	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	EMVBase_TraceHex("pure-info: Kernel Cap", KernelCap, 5);

	tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_NA;

	//K11.3
	EMVBase_Trace("pure-info: CCIDExistInGPO = %d\r\n", tempApp_UnionStruct->EMVTradeParam->CCIDExistInGPO);
	if(tempApp_UnionStruct->EMVTradeParam->CCIDExistInGPO)
	{
		emvbase_avl_gettagvalue_spec(EMVTAG_PURECCID, &CCID, 0, 1);

		EMVBase_Trace("pure-info: CCID = %02X\r\n", CCID);

		if((TTPI[1] & 0x40) || (0x00 !=(CCID & 0x07)))	//ICC or Terminal request CVM
		{
			//Depending on CCID (CVM Information – bits 3-1) value
			if(0x00 == (CCID & 0x07))	//No CVM Required
			{
				CVMRes[0] = PURE_CVMMETHOD_NOCVMPERFORMED;
				CVMRes[1] = 0x00;
				CVMRes[2] = CVMR_UNKNOWN;
				emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMRes, 3);
				emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
				tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_NOCVMREQ;
			}
			else if(0x02 == (CCID & 0x07))	//Online PIN required
			{
				CVMRes[0] = PURE_CVMMETHOD_ONLINEPIN;
				CVMRes[1] = 0x00;
				if(TTPI[0] & 0x04)
				{
					CVMRes[2] = CVMR_UNKNOWN;
					emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMRes, 3);
					emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x04);
					return pure_CardHolderVerf_onlinepin(tempApp_UnionStruct);
				}
				else
				{
					CVMRes[2] =CVMR_FAIL;
					emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMRes, 3);
					emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
				}
				tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_ONLINEPIN;
			}
			else if(0x01 == (CCID & 0x07))	//Signature
			{
				CVMRes[0] = PURE_CVMMETHOD_SIGBATURE;
				CVMRes[1] = 0x00;
				if(TTPI[0] & 0x02)
				{
					CVMRes[2] = CVMR_UNKNOWN;
				}
				else
				{
					CVMRes[2] =CVMR_FAIL;
					emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
				}
				emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMRes, 3);
				tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_OBTAINSIGNATURE;
			}
			else if(0x05 == (CCID & 0x07))	//Confirmation Code already checked using Customer device
			{
				CVMRes[0] = PURE_CVMMETHOD_CDCVM;
				CVMRes[1] = 0x00;
				CVMRes[2] = CVMR_SUCCESS;
				emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMRes, 3);
				tempApp_UnionStruct->EMVTradeParam->PureCVMParameter = SDK_OUTCOME_CVM_CONFVERIFIED;
			}
			else if(0x03 == (CCID & 0x07))	//using CVM List(8E)
			{
				if((0 == (KernelCap[4] & 0x10)) || (0 == emvbase_avl_checkiftagexist(EMVTAG_CVMList)))
				{
					emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
				}
				else
				{
					retCode = pure_CardHolderVerfUseCVMList(tempApp_UnionStruct);
					if(PURE_REQ_INPUTPIN == retCode)
					{
						return retCode;
					}
				}

			}
		}

	}
	else	//K11.4
	{
		if(TTPI[1] & 0x40)
		{
			if(AIP[0] & 0x10)
			{
				if((0 == (KernelCap[4] & 0x10)) || (0 == emvbase_avl_checkiftagexist(EMVTAG_CVMList)))
				{
					EMVBase_Trace("pure-info: CCID and 8E miss,CVM Failed\r\n");
					EMVBase_Trace("pure-info: Set TVR byte 3 bit 8 = 1\r\n");
					emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
				}
				else
				{
					retCode = pure_CardHolderVerfUseCVMList(tempApp_UnionStruct);
					if(PURE_REQ_INPUTPIN == retCode)
					{
						return retCode;
					}
				}
			}
			else
			{
				emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
			}
		}
	}

	emvbase_avl_printtagallvalue("after CVM 9F34", EMVTAG_CVMResult);
	emvbase_avl_printtagallvalue("after CVM 95", EMVTAG_TVR);
	return retCode;
}

unsigned char pure_UnableGoOnlineDeal(PURETradeUnionStruct *tempApp_UnionStruct)
{

    if(tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator == 0)
    {
        tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_OFFLINE_APPROVE;
    }
    else
    {
        tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_OFFLINE_DECLINE;

    }
    return RLT_EMV_OK;
}

unsigned char pure_CommandIssuerAuthen(EMVBASE_APDU_SEND *apdu_s, unsigned char *IssuAuthenData, unsigned char IssuAuthenDataLen)
{
    memcpy(apdu_s->Command, "\x00\x82\x00\x00", 4);

    memcpy(apdu_s->DataIn, IssuAuthenData, IssuAuthenDataLen);
    apdu_s->Lc = IssuAuthenDataLen;
    apdu_s->Le = 0;  //no Le


    apdu_s->EnableCancel = 0;
    return RLT_EMV_OK;
}

unsigned char pure_IssuerAuthen(EMVBASE_APDU_RESP *apdu_r)
{
    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)
    {
        return RLT_EMV_OK;
    }
    else
    {
        return RLT_ERR_EMV_IccReturn;
    }
}

unsigned char pure_DealIssuerAuthenData(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned char retCode = RLT_EMV_OK;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    EMVBASETAGCVLITEM *item;

	EMVBase_Trace("Emvb_DealpaywaveIssuerAuthenData begin");

    item = emvbase_avl_gettagitempointer(EMVTAG_IssuerAuthenData);

    if(item == NULL)
    {
        return RLT_EMV_OK;
    }

    if(item->len == 0)
    {
        return RLT_EMV_OK;
    }
   // emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x10);  //Issuer authentication was performed
    pure_CommandIssuerAuthen(&apdu_s, item->data, item->len);

    tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk == RLT_EMV_OK)
    {
        retCode = pure_IssuerAuthen(&apdu_r);
    }
    else
    {
        retCode = RLT_ERR_EMV_APDUTIMEOUT;
		emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x40);
		return retCode;
    }

    if(retCode != RLT_EMV_OK)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x40);
    }

	EMVBase_Trace("Emvb_DealpaywaveIssuerAuthenData end");
    return RLT_EMV_OK;
}

unsigned char pure_AnalysisEchoResp(PURETradeUnionStruct * tempApp_UnionStruct)
{
	unsigned char ret;
	unsigned char indexRet = 0;
	unsigned short index,indexTemp,len,lenTemp,orgindex,tmplen,startpos;
	unsigned char Primitivetype = 0;
	unsigned char bIntable;
	unsigned char GenerateACRetDataflag;
	EMVBASETAGCVLITEM *itemARC = emvbase_avl_gettagitempointer(EMVTAG_AuthRespCode);
	unsigned char CID,CCID;
	unsigned char CIDExist = emvbase_avl_checkiftagexist(EMVTAG_CryptInfo);
	unsigned char CCIDExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);

	//K3.5
	if(tempApp_UnionStruct->EMVTradeParam->ECHORespData[0] != 0x77)
	{
		return RLT_ERR_EMV_IccDataFormat;
	}
	emvbase_avl_deletetag(EMVTAG_CryptInfo);
	emvbase_avl_deletetag(EMVTAG_ATC);
	emvbase_avl_deletetag(EMVTAG_SignDynAppData);
	emvbase_avl_deletetag(EMVTAG_IssuAppData);
	emvbase_avl_deletetag(EMVTAG_AppCrypt);

	index=0;
	if(tempApp_UnionStruct->EMVTradeParam->ECHORespData[index]==0x77)//TLV coded data
	{
		Primitivetype = 1;
		index++;
		if(EMVBase_ParseExtLen(tempApp_UnionStruct->EMVTradeParam->ECHORespData,&index,&lenTemp))
		{
			return RLT_ERR_EMV_IccDataFormat;
		}
		indexTemp=index;
		indexRet=0;
		while(index<indexTemp+lenTemp)
		{
			if(index>=tempApp_UnionStruct->EMVTradeParam->ECHORespDataLen)
			{
				EMVBase_Trace("pure-error: parse 77 template len > receive APDU len\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}
			if(tempApp_UnionStruct->EMVTradeParam->ECHORespData[index]==0xFF || tempApp_UnionStruct->EMVTradeParam->ECHORespData[index]==0x00)
			{
				index++;
				continue;
			}

			orgindex = index;

			bIntable = 0;


			ret = pure_ParseAndStoreCardResponse(tempApp_UnionStruct,tempApp_UnionStruct->EMVTradeParam->ECHORespData,&index,&bIntable,0);
			if(ret != RLT_EMV_OK)
			{
				return ret;
			}
		}
		if(index!=indexTemp+lenTemp)
		{
			EMVBase_Trace("pure-error: parse 77 template len error\r\n");
			return RLT_ERR_EMV_IccDataFormat;
		}
	}

	// GENERATE AC Response Data Field, CDA not requested or AAC card decision with CDA requested
	if(1)
	{
		if((emvbase_avl_checkiftagexist(EMVTAG_CryptInfo)==0)||(emvbase_avl_checkiftagexist(EMVTAG_ATC)==0) || (emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData)==0) || (emvbase_avl_checkiftagexist(EMVTAG_IssuAppData)==0))
		{
			//Completion_ID = 1 (‘error’)
			return RLT_ERR_EMV_IccDataMissing;
		}
	}
	else	//GENERATE AC Response Data Field for TC card decision or ARQC card decision or AAC card decision with CDA requested
	{
		if((emvbase_avl_checkiftagexist(EMVTAG_CryptInfo)==0)||(emvbase_avl_checkiftagexist(EMVTAG_ATC)==0) || (emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData)==0) || (emvbase_avl_checkiftagexist(EMVTAG_IssuAppData)==0))
		{
			//Completion_ID = 1 (‘error’)
			return RLT_ERR_EMV_IccDataMissing;
		}
	}

	if(CIDExist)
	{
		CID = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
		if(CCIDExist)
		{
			CCID = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
			if(itemARC != NULL)
			{
				if(((CID & 0xC0) == 0x40) && ((CCID & 0xC0) == 0x40) && (memcmp(itemARC->data, "\x30\x30", 2) != 0))
				{
					//Completion_ID = 1 (‘error’)
					return RLT_ERR_EMV_IccDataFormat;
				}
			}
		}
		else
		{
			if(itemARC != NULL)
			{
				if(((CID & 0xC0) == 0x40) && (memcmp(itemARC->data, "\x30\x30", 2) != 0))
				{
					return RLT_ERR_EMV_IccDataFormat;
				}
			}
		}

		if((CID & 0xC0) == 0x40)
		{
			tempApp_UnionStruct->EMVTradeParam->TransResult == RESULT_ONLINE_APPROVE;
//					return RLT_EMV_ONLINE_APPROVE;
		}
		else
		{
			tempApp_UnionStruct->EMVTradeParam->TransResult == RESULT_ONLINE_DECLINE;
//					return RLT_EMV_ONLINE_DECLINE;
		}
	}

	return RLT_EMV_OK;
}

unsigned char pure_AnalysisGacResp(PURETradeUnionStruct * tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
	unsigned char ret;
	unsigned char indexRet = 0;
	unsigned short index,indexTemp,len,lenTemp,orgindex,tmplen,startpos;
	unsigned char Primitivetype = 0;
	unsigned char bIntable;
	unsigned char GenerateACRetDataflag;
	EMVBASETAGCVLITEM *item1, *item2;
	EMVBASETAGCVLITEM *itemARC = emvbase_avl_gettagitempointer(EMVTAG_AuthRespCode);
	unsigned char CIDExist = emvbase_avl_checkiftagexist(EMVTAG_CryptInfo);
	unsigned char CCIDExist = emvbase_avl_checkiftagexist(EMVTAG_PURECCID);
	unsigned char CID,CCID;

	//K3.5
	if(apdu_r->DataOut[0] != 0x77)
	{
		return RLT_ERR_EMV_IccDataFormat;
	}
	emvbase_avl_deletetag(EMVTAG_CryptInfo);
	emvbase_avl_deletetag(EMVTAG_ATC);
	emvbase_avl_deletetag(EMVTAG_SignDynAppData);
	emvbase_avl_deletetag(EMVTAG_IssuAppData);
	emvbase_avl_deletetag(EMVTAG_AppCrypt);

	index=0;
	if(apdu_r->DataOut[index]==0x77)//TLV coded data
	{
		Primitivetype = 1;
		index++;
		if(EMVBase_ParseExtLen(apdu_r->DataOut,&index,&lenTemp))
		{
			return RLT_ERR_EMV_IccDataFormat;
		}
		indexTemp=index;
		indexRet=0;
		while(index<indexTemp+lenTemp)
		{
			if(index>=apdu_r->LenOut)
			{
				EMVBase_Trace("pure-error: parse 77 template len > receive APDU len\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}
			if(apdu_r->DataOut[index]==0xFF || apdu_r->DataOut[index]==0x00)
			{
				index++;
				continue;
			}

			orgindex = index;

			bIntable = 0;


			ret = pure_ParseAndStoreCardResponse(tempApp_UnionStruct,apdu_r->DataOut,&index,&bIntable,0);
			if(ret != RLT_EMV_OK)
			{
				return ret;
			}
		}
		if(index!=indexTemp+lenTemp)
		{
			EMVBase_Trace("pure-error: parse 77 template len error\r\n");
			return RLT_ERR_EMV_IccDataFormat;
		}
	}

	ret = pure_CheckGACRespMandatoryDataMiss(tempApp_UnionStruct);
	if(ret)
	{
		EMVBase_Trace("pure-error: GAC Response miss mandatory data\r\n");
		return RLT_ERR_EMV_IccDataMissing;
	}

	if(CIDExist)
	{
		CID = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
		if(CCIDExist)
		{
			CCID = emvbase_avl_gettagvalue(EMVTAG_PURECCID);
			if(itemARC != NULL)
			{
				if(((CID & 0xC0) == 0x40) && ((CCID & 0xC0) == 0x40) && (memcmp(itemARC->data, "\x30\x30", 2) != 0))
				{
					//Completion_ID = 1 (‘error’)
					return RLT_ERR_EMV_IccDataFormat;
				}
			}
		}
		else
		{
			if(itemARC != NULL)
			{
				if(((CID & 0xC0) == 0x40) && (memcmp(itemARC->data, "\x30\x30", 2) != 0))
				{
					return RLT_ERR_EMV_IccDataFormat;
				}
			}
		}

		if((CID & 0xC0) == 0x40)
		{
			tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_ONLINE_APPROVE;
//					return RLT_EMV_ONLINE_APPROVE;
		}
		else
		{
			tempApp_UnionStruct->EMVTradeParam->TransResult = RESULT_ONLINE_DECLINE;
//					return RLT_EMV_ONLINE_DECLINE;
		}
	}

	return RLT_EMV_OK;
}

unsigned char pure_DealGAC2Rsp(PURETradeUnionStruct * tempApp_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
	unsigned char ret;

	if((apdu_r->SW1==0x90 && apdu_r->SW2==0x00))
	{
		ret = pure_AnalysisGacResp(tempApp_UnionStruct, apdu_r);
		EMVBase_Trace("pure-info: pure_AnalysisEchoOrGacResp ret = %d\r\n", ret);
		return ret;
	}
	else
	{
	}

	return RLT_EMV_OK;
}



unsigned char pure_ReadScriptNum(PURETradeUnionStruct *tempApp_UnionStruct)
{
    unsigned short index;       // 1106
    unsigned short len;
    unsigned char j, k, retCode = RLT_EMV_OK;
    unsigned char IssuScriptNum;
    unsigned char i;
    unsigned char tag[4];
    unsigned char IssuScript[300];
    unsigned short IssuScriptLen;



    IssuScriptNum = 0;
    memset(tag, 0, sizeof(tag));

    for(i = 0; i < 2; i++)
    {
        if(i == 0)
        {
            tag[0] = 0x71;

            if(emvbase_avl_checkiftagexist(tag) == 0)
            {
                continue;
            }
        }
        else
        {
            tag[0] = 0x72;

            if(emvbase_avl_checkiftagexist(tag) == 0)
            {
                continue;
            }
        }
        retCode = RLT_EMV_OK;
        memset(IssuScript, 0, sizeof(IssuScript));
        emvbase_avl_gettagvalue_all(tag, IssuScript, &IssuScriptLen);
        index = 0;

        while(index < IssuScriptLen)
        {
            if(IssuScript[index] == 0x71 || IssuScript[index] == 0x72)
            {
                index++;

                if(IssuScript[index] <= 127)
                {
                    len = IssuScript[index];
                    len += 2;
                    k = 0;
                }
                else
                {
                    len = 0;
                    k = IssuScript[index] & 0x7F;

                    for(j = 1; j <= k; j++)
                    {
                        len = len * 256 + IssuScript[index + j];
                    }

                    len += k + 2;
                }
                index--;

                if(index + len > IssuScriptLen)
                {
                    //EMVBase_Trace("index = %d,len = %d,IssuScriptLen =%d \r\n",tempterminfo->IssuScriptLen);
                    retCode = PURE_ERR_EMV_ScriptFormat;
                    break;
                }
                IssuScriptNum++;
                index += len;
            }
            else
            {
                retCode = PURE_ERR_EMV_ScriptFormat;
                break;
            }
        }


        if(retCode != RLT_EMV_OK)
        {
            IssuScriptNum++;
        }
    }



    return IssuScriptNum;
}

unsigned char pure_SendScriptCommand(unsigned char* Script, unsigned char ScriptLen, EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, Script, 4);
    memcpy(apdu_s->DataIn, (unsigned char*)&Script[5], Script[4]);
    apdu_s->Lc = Script[4];
    apdu_s->Le = 0;  //no Le
    apdu_s->EnableCancel = 0;

    return RLT_EMV_OK;
}


#define MAXSCRIPTLEN 128
/*******************************************************************
   ��    ��: ��Т��
   ��������: unsigned char Paywave_ScriptProcess(unsigned char tag,AppUnionStruct *tempApp_UnionStruct)
   ��������: Paywave �ű�����
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:  20130731  �ŵ�com
 ********************************************************************/
unsigned char pure_ScriptProcess(unsigned char tag, PURETradeUnionStruct *tempApp_UnionStruct)
{
    //unsigned char m;   //j,
    unsigned short lenScript, index, Scriptindex;
    //unsigned char script_amount[6];
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    EMVBASETAGCVLITEM *item;
    unsigned char *IssuScript;       //[300]
    unsigned short lenScriptPerformed, IssuScriptlen;
    unsigned char *ScriptResult;    //[300]
    unsigned char ScriptResultlen;
    unsigned char curScriptID[4];
    unsigned char ScriptK;
    unsigned char ifscriptlenoverfloor;
    unsigned char numScriptCom;
    unsigned char retCode = RLT_EMV_OK;
    unsigned char dealscripterr;
    unsigned char script_amount[6];
    unsigned char VLPAvailableFund[6] = {0};
    unsigned char VLPTransLimit[6] = {0};
	unsigned short templen86, tempindex86;

	EMVBase_Trace("start pure_ScriptProcess\r\n");

    if(tag == 0x71)
    {
        item = emvbase_avl_gettagitempointer(EMVTAG_SCRIPT1);
    }
    else if(tag == 0x72)
    {
        item = emvbase_avl_gettagitempointer(EMVTAG_SCRIPT2);
    }
    else
    {
        return RLT_EMV_OK;
    }

    if(item == NULL)
    {
        return RLT_EMV_OK;
    }

    if(item->len == 0)
    {
        return RLT_EMV_OK;
    }
    IssuScript = (unsigned char *)emvbase_malloc(512);
    ScriptResult = (unsigned char *)emvbase_malloc(300);

    index = 0;
    memset(IssuScript, 0, 512);

    if(item->len > 512)
    {
        IssuScriptlen = 512;
    }
    else
    {
        IssuScriptlen = item->len;
    }
    memcpy(IssuScript, item->data, IssuScriptlen);
    EMVBase_TraceHex("pure-info: Script content", IssuScript, IssuScriptlen);

    ScriptResultlen = 0;
    memset(ScriptResult, 0, 300);


    lenScriptPerformed = 0; //zcl add 20160513
    while(index < IssuScriptlen)
    {
        EMVBase_Trace( "pure-info: (for debug only)Line 8766 index = %d IssuScriptlen = %d\r\n", index, IssuScriptlen);

        if(IssuScript[index] == tag)
        {
            index++;

            if(EMVBase_ParseExtLen(IssuScript, &index, &lenScript))
            {
                retCode = PURE_ERR_EMV_ScriptFormat;
                break;
            }
            Scriptindex = index;

            memset(curScriptID, 0, sizeof(curScriptID));

            ifscriptlenoverfloor = 1;
            numScriptCom = 0;
            dealscripterr = 0;
            //lenScriptPerformed = 0;   //zcl add 20160513

            while(index < (Scriptindex + lenScript))
            {
		       EMVBase_Trace("pure-info: (for debug only)Line 8788 index = %d %02x\r\n", index, IssuScript[index]);

                if(ifscriptlenoverfloor)
                {
                    ifscriptlenoverfloor = 0;
                    lenScriptPerformed += (lenScript + 2);

					EMVBase_Trace( "pure-info: (for debug only)Line 8795 Script len = %d\r\n", lenScriptPerformed);

                    if(lenScriptPerformed > MAXSCRIPTLEN)
                    {
                        dealscripterr = 1;
                        break;
                    }
                }

                if(!memcmp(&IssuScript[index], "\x9F\x18\x04", 3))
                {
                    memcpy(curScriptID, &IssuScript[index + 3], 4);
                    index += 7;
                }
                else if(!memcmp(&IssuScript[index], "\x9F\x18\x00", 3))
                {
                    index += 3;
                }
                else if(IssuScript[index] == 0x86)
                {
					tempindex86 = index + 1;	//2021.8.18 lishiyao should parse len rather than tag
					templen86 = 0;
					if(EMVBase_ParseExtLen(IssuScript, &tempindex86, &templen86))
					{
						EMVBase_Trace("PURE-info:Parse Tag 86 data len error\r\n");
						retCode = PURE_ERR_EMV_ScriptFormat;
						dealscripterr = 1;
                		break;
					}
                    ScriptK = IssuScript[index + 1];                    // script len //  0D
					if((ScriptK < 5) || (IssuScript[index + 3] & 0x01) == 0x01
                       || (IssuScript[index + 3] & 0xF0) == 0x60 || (IssuScript[index + 3] & 0xF0) == 0x90 ||
                       (((ScriptK != IssuScript[index + 6]+5) && (ScriptK != IssuScript[index + 6]+6)) || ( (index+ScriptK+2) > (Scriptindex + lenScript))))
                    {
						EMVBase_Trace("PURE-error: IssuScript len invalid\r\n");
                        dealscripterr = 1;
                        break;
                    }

					EMVBase_Trace("pure-info: Start pure_SendScriptCommand\r\n");
                    pure_SendScriptCommand((unsigned char*)&IssuScript[index + 2], ScriptK, &apdu_s);

                    tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

					EMVBase_Trace("pure-info: apdu_r.ReadCardDataOk=%d apdu_r.SW1=%02x apdu_r.SW2=%02x\r\n", apdu_r.ReadCardDataOk, apdu_r.SW1, apdu_r.SW2);
					if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
					{
						if(tag == 0x71)//ISSUER_SCRIPT_080	72 fail don't send end application when timeout happens
						{
							tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
							tempApp_UnionStruct->SendOutcome();
						}
						//break;
						return RLT_EMV_TERMINATE_TRANSERR;
					}

                    if((apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00) || apdu_r.SW1 == 0x62 || apdu_r.SW1 == 0x63)
                    {
                        index += (ScriptK + 2);
                        numScriptCom++;
                    }
                    else
                    {
                    	EMVBase_Trace("pure-info: script command sw is not 9000,62xx,63xx! quit deal\r\n");
                        numScriptCom++;
                        dealscripterr = 1;
                        break;
                    }
                }
                else
                {
					EMVBase_Trace("pure-error: unknow script tag\r\n");
                    dealscripterr = 1;
                    break;
                }
            }

            EMVBase_Trace("pure-info: (for debug only)deal script err = %d index = %d data = %02x\r\n", dealscripterr, index, IssuScript[index]);

            if(dealscripterr)
            {
            	EMVBase_Trace("pure-info: dealscript error! tag=%02x\r\n", tag);
                //emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x04);

                if(tag == 0x71)
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x20);                  //Script processing failed before final GENERATE AC
                }
                else                //tag=0x72
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x10);                  //Script processing failed after final GENERATE AC
                }

                if(numScriptCom)
                {
                    ScriptResult[ScriptResultlen] = 0x10;

                    if(numScriptCom < 15)
                    {
                        ScriptResult[ScriptResultlen] |= numScriptCom;
                    }
                    else
                    {
                        ScriptResult[ScriptResultlen] |= 0x0F;
                    }
                }
                memcpy(&ScriptResult[ScriptResultlen + 1], curScriptID, 4);
                ScriptResultlen += 5;

                index = Scriptindex + lenScript;
                retCode = PURE_ERR_EMV_ScriptFormat;
            }
            else
            {
                if(numScriptCom)
                {
                    ScriptResult[ScriptResultlen] = 0x20;

                    memcpy(&ScriptResult[ScriptResultlen + 1], curScriptID, 4);
                    ScriptResultlen += 5;
                }
            }
        }
        else
        {
        	EMVBase_Trace("pure-error: script is not equal to tag,user may set invalid data\r\n");
            retCode = PURE_ERR_EMV_ScriptFormat;
			break;
        }
    }

	if(tempApp_UnionStruct->EMVTradeParam->canceldealiup)
	{
		ScriptResult[0] = 0x00;
		tempApp_UnionStruct->EMVTradeParam->canceldealiup = 0;
	}

    if(ScriptResultlen)
    {
        if(tag == 0x71)
        {
            emvbase_avl_createsettagvalue(EMVTAG_SCRIPT71RESULT, ScriptResult, ScriptResultlen);
        }
        else
        {
            emvbase_avl_createsettagvalue(EMVTAG_SCRIPT72RESULT, ScriptResult, ScriptResultlen);
        }
		emvbase_avl_createsettagvalue(EMVTAG_PURESCRIPTResult, ScriptResult, ScriptResultlen);
    }
    emvbase_free(IssuScript);
    emvbase_free(ScriptResult);
	EMVBase_Trace("pure-info: pure_ScriptProcess finish\r\n");

    return retCode;
}

unsigned char pure_OnlineRespProcess(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char EchoTransIndicator = tempApp_UnionStruct->EMVTradeParam->EchoTransIndicator;
	unsigned char CommProbIndicator = tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator;
	unsigned char AuthRespCode[2];
	unsigned short tmplen;
    EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	EMVBASETAGCVLITEM *item;
	unsigned short len;
	unsigned char retCode;
	unsigned char Completion_ID = 0, Script_Result_Available = 0, Outcome_CVM;
	unsigned char Balance[6],CVMRes[3];
	unsigned char Decision;

	EMVBase_Trace("PURE-info: EchoTransIndicator = %d\r\n", EchoTransIndicator);
	//K3.3
	if(1 == EchoTransIndicator)
	{
		//K3.5
		retCode = pure_AnalysisEchoResp(tempApp_UnionStruct);
		if(retCode != RLT_EMV_OK)
		{
			Completion_ID = 1; // (‘error’)
//			return retCode;
		}
	}
	else
	{
		if(3 == CommProbIndicator)
		{
			Completion_ID = 1; //(‘Terminate on error’)
			//section K3.7
		}
		else
		{
			Completion_ID = 0; //(‘no information from card’)
			Script_Result_Available = 0;
			if(emvbase_avl_checkiftagexist(EMVTAG_SCRIPT1))
			{
				pure_ScriptProcess(0x71, tempApp_UnionStruct);
			}
		}
	}

	//K3.4
	if(emvbase_avl_checkiftagexist(EMVTAG_IssuerAuthenData))
	{
		if(emvbase_avl_checkiftagexist(EMVTAG_CDOL2))
		{
			emvbase_avl_gettagvalue_spec(EMVTAG_AuthRespCode, AuthRespCode, 0, 2);
			if(!memcmp(AuthRespCode, "\x30\x30", 2))
			{
				//Decision = ‘Approve’
				Decision = TAA_OFFLINE;
			}
			else
			{
				//Decision = ‘Decline’
				Decision = TAA_DENIAL;
			}

			EMVBase_Trace("PURE-info: Decision = %02X\r\n", Decision);

			memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
			memcpy(apdu_s.Command, "\x80\xAE", 2);

//			if(!memcmp(AuthRespCode, "\x30\x30", 2))
//			{
//				memcpy(&apdu_s.Command[2], "\x40\x00", 2);
//			}
//			else
//			{
//				memcpy(&apdu_s.Command[2], "\x00\x00", 2);
//			}

			if(TAA_OFFLINE == Decision)
			{
				memcpy(&apdu_s.Command[2], "\x40\x00", 2);
			}
			else
			{
				memcpy(&apdu_s.Command[2], "\x00\x00", 2);
			}

			EMVBase_Trace("pure-info: send 2nd GAC\r\n");

			item = emvbase_avl_gettagitempointer(EMVTAG_CDOL2);
			EMVBase_TraceHex("pure-info:CDOL2",item->data,item->len);

			if(pure_GACDOLProcess(typeCDOL2, item->data, item->len, apdu_s.DataIn, &len, tempApp_UnionStruct) != RLT_EMV_OK)
			{
				EMVBase_Trace("pure-error: CDOL2 process error!\r\n");
				return RLT_EMV_TERMINATE_TRANSERR;
			}
			apdu_s.Lc = len;

			emvbase_avl_createsettagvalue(EMVTAG_CDOL2RelatedData, apdu_s.DataIn, apdu_s.Lc);
			apdu_s.Le = 256;
			apdu_s.EnableCancel = 1;

			tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

			if(RLT_EMV_OK != apdu_r.ReadCardDataOk)
			{
				if(tempApp_UnionStruct->EMVTradeParam->ECHOCommandSupport == 1)
				{
					tempApp_UnionStruct->EMVTradeParam->CommuProblemIndicator = 2;
					tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYAGAIN, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
					tempApp_UnionStruct->SendOutcome();
					tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
					tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
					tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
					tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);
					return RLT_EMV_TORN;
				}
			}
			else
			{
				//get in here to find K3.5
				retCode = pure_DealGAC2Rsp(tempApp_UnionStruct, &apdu_r);
				if(retCode != RLT_EMV_OK)
				{
					Completion_ID = 1; // (‘error’)
//					return retCode;
				}

				if(RESULT_ONLINE_APPROVE == tempApp_UnionStruct->EMVTradeParam->TransResult)
				{
					Completion_ID = 2;	//Approved
				}
				else if(RESULT_ONLINE_DECLINE == tempApp_UnionStruct->EMVTradeParam->TransResult)
				{
					Completion_ID = 3;	//Declined
				}

				//K3.6
				if(emvbase_avl_checkiftagexist(EMVTAG_SCRIPT2))
				{
					retCode = pure_ScriptProcess(0x72, tempApp_UnionStruct);
					EMVBase_Trace("pure-info: pure_ScriptProcess ret = %d\r\n", retCode);
					Script_Result_Available =1;
				}
			}

		}
	}

	EMVBase_Trace("pure-info: Completion_ID = %d\r\n", Completion_ID);
	EMVBase_Trace("pure-info: Script_Result_Available = %d\r\n", Script_Result_Available);
	//K3.7
	switch (Completion_ID)
	{
		case 0:	//Value 0 : ‘no information from card, let the terminal to decide’
			if(1 == Script_Result_Available)
			{
				tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 1, 0, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
				tempApp_UnionStruct->SendOutcome();
			}
			else
			{
				tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 0, 0, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
				tempApp_UnionStruct->SendOutcome();
			}
			return RLT_EMV_TERMINATE_TRANSERR;
			break;
		case 1:	//Value 1 : terminate on error
			if(1 == Script_Result_Available)
			{
				tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_DECLINED, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 1, 1, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
				tempApp_UnionStruct->SendOutcome();
				emvbase_avl_gettagvalue_spec(EMVTAG_OfflineAccumulatorBalance, Balance, 0, 6);
				tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_NOTAUTHORISED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, Balance, NULL);
				tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			}
			else
			{
				tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_DECLINED, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 1, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
				tempApp_UnionStruct->SendOutcome();
				emvbase_avl_gettagvalue_spec(EMVTAG_OfflineAccumulatorBalance, Balance, 0, 6);
				tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_NOTAUTHORISED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, Balance, NULL);
				tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			}
			return RLT_EMV_ONLINE_DECLINE;
			break;
		case 2:	//Value 2 : transaction approved
			emvbase_avl_gettagvalue_spec(EMVTAG_CVMResult, CVMRes, 0, 3);

			if(PURE_CVMRESULT_UNKNOWN == CVMRes[2] || PURE_CVMRESULT_SUCCESSFUL == CVMRes[2])
			{
				if(PURE_CVMMETHOD_ONLINEPIN == CVMRes[0])
				{
					Outcome_CVM = SDK_OUTCOME_CVM_ONLINEPIN;
				}
				else if(PURE_CVMMETHOD_SIGBATURE == CVMRes[0])
				{
					Outcome_CVM = SDK_OUTCOME_CVM_OBTAINSIGNATURE;
				}
				else if(PURE_CVMMETHOD_NOCVMPERFORMED == CVMRes[0])
				{
					Outcome_CVM = SDK_OUTCOME_CVM_NOCVMREQ;
				}
				else if(PURE_CVMMETHOD_CDCVM == CVMRes[0])
				{
					Outcome_CVM = SDK_OUTCOME_CVM_CONFVERIFIED;
				}
				else
				{
					Outcome_CVM = SDK_OUTCOME_CVM_NA;
				}
			}

			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_APPROVED, SDK_OUTCOME_START_NA, Outcome_CVM, 1, 0, 1, 1, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			emvbase_avl_gettagvalue_spec(EMVTAG_OfflineAccumulatorBalance, Balance, 0, 6);
			if(SDK_OUTCOME_CVM_OBTAINSIGNATURE != Outcome_CVM)
			{
				tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_APPROVED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, Balance, NULL);
			}
			else
			{
				tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_APPROVEDSIGN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, Balance, NULL);
			}
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			return RLT_EMV_ONLINE_APPROVE;
			break;
		case 3:	//Value 3 : transaction declined
			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_DECLINED, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 1, 1, SDK_OUTCOME_AIP_NA, 0, 0, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			emvbase_avl_gettagvalue_spec(EMVTAG_OfflineAccumulatorBalance, Balance, 0, 6);
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_NOTAUTHORISED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_BALANCE, Balance, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			return RLT_EMV_ONLINE_DECLINE;
			break;
		default:
			break;
	}

	return RLT_EMV_OK;
}

unsigned char pure_GDDOLProcess(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char *GDDOL, unsigned short DOLLen, unsigned char *ResBuf)
{
	unsigned char i,taglen=0,valuelen=0;
	EMVBASETAGCVLITEM *dolitem=NULL;
	unsigned char Tag[3];
    EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned char *pb;
	unsigned short ResBufLen = 0;

	i = 0;
	ResBufLen = 0;

	EMVBase_Trace("pure-info: DOLLen ret = %d\r\n", DOLLen);
	EMVBase_TraceHex("pure-info: GDDOL", GDDOL, DOLLen);

	while (i < DOLLen)
	{
		if(0xFF == GDDOL[i] || 0x00 == GDDOL[i])
		{
			i++;
			continue;
		}
		if(0x1F == (GDDOL[i] & 0x1F))
		{
			if(GDDOL[i+1] & 0x80)
			{
				taglen = 3;
			}
			else
			{
				taglen = 2;
			}
		}
		else
		{
			taglen = 1;
		}
		memcpy(Tag, GDDOL+i, taglen);

		memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
		memcpy(apdu_s.Command, "\x80\xCA", 2);
		if(2 == taglen)
		{
			memcpy(&apdu_s.Command[2], Tag, 2);
		}
		else if(1 == taglen)
		{
			apdu_s.Command[3] = Tag[0];
		}
		apdu_s.Lc = 0;
		apdu_s.Le = 0;
		memset(&apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
		tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

		if(RLT_EMV_OK == apdu_r.ReadCardDataOk)
		{
			if((0x90 == apdu_r.SW1) && (0x00 == apdu_r.SW2))
			{
				if(EMVBaseIsTLV(apdu_r.DataOut, apdu_r.LenOut))
				{
					valuelen = EMVBaseTlvLen(apdu_r.DataOut);
					pb = EMVBaseTlvVPtr(apdu_r.DataOut);
					emvbase_avl_createsettagvalue(Tag, pb, valuelen);
					memcpy(ResBuf+ResBufLen, apdu_r.DataOut, apdu_r.LenOut);
					ResBufLen += apdu_r.LenOut;
				}
			}
		}
		else
		{
			return RLT_EMV_ERR;
		}

		i += taglen;
		if(0 != (GDDOL[i] & 0x80))	//ignore length,because APDU return lenght may not equal to len in GDDOL
		{
			i += 2;	//FIXME: only allow tag has at most 2 byte len
		}
		else
		{
			i++;
		}
	}

	return RLT_EMV_OK;
}

unsigned char pure_RetrieveDataInE2PROM(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char *Template, unsigned char *ResBuf)
{
	unsigned char retCode;
	unsigned char TemplatelenHex[2];
	unsigned short Templatelen,GDDOLLen;

	if(0 == (tempApp_UnionStruct->EMVTradeParam->PureImplementationOption & 0x80))	//if implementation option dont support,quit flow
	{
		EMVBase_Trace("pure-info: Implementation Option Dont support GET DATA\r\n");
		return RLT_EMV_OK;
	}
	//K4.3
	EMVBase_Trace("pure-info: BF71 Exist = %d\r\n", emvbase_avl_checkiftagexist(EMVTAG_PUREMSReadTemp));
	if(emvbase_avl_checkiftagexist(EMVTAG_PUREMSReadTemp))
	{
		emvbase_avl_gettagvalue_spec(EMVTAG_PUREMSReadTemp, TemplatelenHex, 0, 2);
		EMVBaseHex2U16(&Templatelen, TemplatelenHex, 2);
		GDDOLLen = Templatelen - 3;
		if(GDDOLLen != Template[2])
		{
			EMVBase_Trace("pure-error: Parse GDDOL len error,DOL len is not equal to TLV's Value field\r\n");
			return RLT_ERR_EMV_IccDataFormat;
		}

		retCode = pure_GDDOLProcess(tempApp_UnionStruct, Template+3, GDDOLLen, ResBuf);
		EMVBase_Trace("pure-info: pure_GDDOLProcess ret = %d\r\n", retCode);
		if(retCode != RLT_EMV_OK)
		{
			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYAGAIN, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);
			return RLT_EMV_TERMINATE_TRANSERR;
		}
	}

	if(PURE_TRANS_RETRIEVE_GETDATA == tempApp_UnionStruct->EMVTradeParam->CurTransType)
	{
		return pure_Outcome_NonFinancialCompleted_RetrievalOfDataElements(tempApp_UnionStruct);
	}

	return RLT_EMV_OK;
}

unsigned char pure_DealPutDataCommand(PURETradeUnionStruct *tempApp_UnionStruct, EMVBASE_APDU_SEND *apdu_s, EMVBASE_APDU_RESP *apdu_r, unsigned char *SendTLV, unsigned short TLVLLen)
{
	unsigned char Tag[3];
	unsigned short taglen;
	int i = 0;
	unsigned char DataUpdateRes[2]={0};
	static unsigned char counter = 0;

	memcpy(apdu_s->Command, "\x00\xDA", 2);

	if(0x1F == (*SendTLV & 0x1F))
	{
		taglen = 2;
	}
	else
	{
		taglen = 1;
	}
	if(2 == taglen)
	{
		memcpy(&(apdu_s->Command[2]), SendTLV, 2);
		apdu_s->Lc = *(SendTLV+2);
		i+=3;
	}
	else if(1 == taglen)
	{
		apdu_s->Command[3] = *SendTLV;
		apdu_s->Lc = *(SendTLV+1);
		i+=2;
	}
	apdu_s->Le = 0;
	memcpy(apdu_s->DataIn, SendTLV+i, apdu_s->Lc);

	tempApp_UnionStruct->IsoCommand(apdu_s, apdu_r);
	if(RLT_EMV_OK == apdu_r->ReadCardDataOk)
	{
		if((0x90 == apdu_r->SW1) && (0x00 == apdu_r->SW2))
		{
			DataUpdateRes[0] = 0x02;
			counter+=1;
			DataUpdateRes[1] = counter;
			emvbase_avl_createsettagvalue(EMVTAG_PUREDEUResult, DataUpdateRes, 2);
			return RLT_EMV_OK;
		}
	}

	DataUpdateRes[0] = 0x01;
	DataUpdateRes[1] = counter-1;
	emvbase_avl_createsettagvalue(EMVTAG_PUREDEUResult, DataUpdateRes, 2);
	return RLT_EMV_ERR;
}

unsigned char pure_CheckEntryTypeExistInTemplate(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char *Template, unsigned char EntryType)
{
	unsigned char TmplateLenHex[2]={0};
	unsigned short TmplateLen,i;

	emvbase_avl_gettagvalue_spec(EMVTAG_PUREMSUTemp, TmplateLenHex, 0, 2);
	EMVBaseHex2U16(&TmplateLen, TmplateLenHex, 2);

	while (i < TmplateLen)
	{
		if(Template[i] == EntryType)
		{
			return 1;
		}
		i++;
	}
	return 0;
}

unsigned char pure_UpdateDataInE2PROM(PURETradeUnionStruct *tempApp_UnionStruct, unsigned char *Template, unsigned char EntryType, unsigned char bCheckNextEntry)
{
	unsigned char Update, Terminate, retCode, EntryLen, ErrorFlag, UpdateRes;
	unsigned char TmplateLenHex[2]={0};
	unsigned short TmplateLen,i;
    EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned char Tag[3]={0};

	Update = 0;
	Terminate = 1;
	ErrorFlag = 0;

	if(0 == (tempApp_UnionStruct->EMVTradeParam->PureImplementationOption & 0x40))	//if implementation option dont support,quit flow
	{
		return RLT_EMV_OK;
	}

	if(emvbase_avl_checkiftagexist(EMVTAG_PUREMSUTemp))
	{
		Update = 1;
		emvbase_avl_gettagvalue_spec(EMVTAG_PUREMSUTemp, TmplateLenHex, 0, 2);
		EMVBaseHex2U16(&TmplateLen, TmplateLenHex, 2);
		i = 0;

		emvbase_avl_createsettagvalue(EMVTAG_PUREDEUResult, "\x00\x00", 2);	//initialize
		while(i < TmplateLen)
		{
			if(Template[i] == EntryType)
			{
				EntryLen = Template[i+1];
				i+=2;
				memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
				memset(&apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
				retCode = pure_DealPutDataCommand(tempApp_UnionStruct ,&apdu_s, &apdu_r, &Template[i], EntryLen-3);
				if(RLT_EMV_OK != retCode)
				{
					ErrorFlag = 1;
					emvbase_avl_createsettagvalue(EMVTAG_PUREMSUES, &Template[TmplateLen-1], 1); //check tag 85,store it for later use
					break;
				}
			}
			else
			{
				i++;
			}
		}
	}

	if(!ErrorFlag)
	{
		Terminate = 0;
		if(Update)
		{
			emvbase_avl_gettagvalue_spec(EMVTAG_PUREDEUResult, &UpdateRes, 0, 1);
			if(bCheckNextEntry)
			{
				if(((0x01 == UpdateRes) && (Template[TmplateLen-1] & 0x01)) || 0 == pure_CheckEntryTypeExistInTemplate(tempApp_UnionStruct, Template, EntryType+1))//TODO: Or The kernel has found no “Memory Slot Update Entry Type 2 – ‘A3’” in the “Memory Slot Update template” parameter.(no additional PUT DATA)
				{
					Terminate = 1;
				}
			}
			else
			{
				if((0x01 == UpdateRes) && (Template[TmplateLen-1] & 0x01)) //TODO: Or The kernel has found no “Memory Slot Update Entry Type 2 – ‘A3’” in the “Memory Slot Update template” parameter.(no additional PUT DATA)
				{
					Terminate = 1;
				}
			}
		}
		else
		{
			if(PURE_TRANS_UPDATE_PUTDATA == tempApp_UnionStruct->EMVTradeParam->CurTransType)
			{
				Terminate = 1;
			}
		}
		if(Terminate)
		{
			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSREMOVECARD, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			return RLT_EMV_TERMINATE_TRANSERR;
		}
	}
	else
	{
		if(PURE_TRANS_UPDATE_PUTDATA == tempApp_UnionStruct->EMVTradeParam->CurTransType)
		{
			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_ENDAPPLICATION, SDK_OUTCOME_START_NA, SDK_OUTCOME_CVM_NA, 1, 0, 0, 1, SDK_OUTCOME_AIP_NA, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_PLSREMOVECARD, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			return RLT_EMV_TERMINATE_TRANSERR;
		}
		else
		{
			tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_TRYAGAIN, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
			tempApp_UnionStruct->SendOutcome();
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
			tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
			tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_RESTART);
			return RLT_ERR_EMV_TransRestart;
		}
	}


	return RLT_EMV_OK;
}

unsigned char pure_RetrieveCardBalance(PURETradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char KernelCap[5];
	EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned char TransType = tempApp_UnionStruct->EMVTradeParam->CurTransType;
	unsigned char Impletion = tempApp_UnionStruct->EMVTradeParam->PureImplementationOption;
	unsigned char CID;

	emvbase_avl_gettagvalue_spec(EMVTAG_PUREKernelCap, KernelCap, 0, 5);
	EMVBase_TraceHex("pure-info: Kernel Contactless Cap: ", KernelCap, 5);
	EMVBase_Trace("pure-info: and result: %d\r\n", (KernelCap[3] & 0x08));
	if(0x00 == (KernelCap[3] & 0x08))
	{
		return RLT_EMV_OK;
	}

	memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
	memset(&apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
	memcpy(apdu_s.Command, "\x80\xCA\x9F\x50", 4);

	tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

	if((RLT_EMV_OK == apdu_r.ReadCardDataOk) && (0x90 == apdu_r.SW1) && (0x00 == apdu_r.SW2))
	{
		emvbase_avl_createsettagvalue(EMVTAG_OfflineAccumulatorBalance, &apdu_r.DataOut[3], 6);
	}
	else
	{
		return RLT_EMV_OK;
	}

	memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
	memset(&apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
	memcpy(apdu_s.Command, "\x80\xCA\x00\xCD", 4);

	tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
	if((RLT_EMV_OK == apdu_r.ReadCardDataOk) && (0x90 == apdu_r.SW1) && (0x00 == apdu_r.SW2))
	{
		emvbase_avl_createsettagvalue(EMVTAG_OfflineAccumulatorBalance, &apdu_r.DataOut[3], 6);
	}
	else
	{
		return RLT_EMV_OK;
	}

	emvbase_avl_gettagvalue_spec(EMVTAG_CryptInfo, &CID, 0, 1);
	if(((PURE_TRANS_APPAUTHTRANS == TransType) && (Impletion & 0x10)) || (0x10 != (CID & 0xC0)) || (0 == (KernelCap[2] & 0x10)))
	{
		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_CARDREADOK, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
		tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
	}
	return RLT_EMV_OK;
}
