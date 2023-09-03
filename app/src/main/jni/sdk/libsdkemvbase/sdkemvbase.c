#include "dllemvbase.h"
#include "sdkemvbaseprivate.h"
#include "sdktypedef.h"
#include "sdkDebug.h"
#include "sdkmaths.h"
#include "sdkemvbase.h"

u8 gstemvbaseforL2TEST;		//for L2 test if anything not same as commercial
u8 gstemvbaseneedsign;
SDK_EMVBASE_CVM_RESULT gstemvbaseCVMresult;
u8 gAppListCandicateMaxNum = 8;//Ĭ��8��


#define EMVBASECAPKSHIFTNUM   	9
#define EMVBASECAPKFLAGOFFSET 	510
#define EMVBASECAPKLEN     	512
#define EMVBASECAPKENABLE  	0x55
#define EMVBASECAPKDISABLE 	0xFF

#define EMVBASEAIDSHIFTNUM    	9
#define EMVBASEAIDFLAGOFFSET  	510
#define EMVBASEAIDLEN         	512
#define EMVBASEAIDENABLE      	0x55
#define EMVBASEAIDDISABLE     	0xFF

#define EMVBASEEMVBlockLen		1024

#define SDK_EMVBASE_AID_FILENALE "emvbaseaid"
#define SDK_EMVBASE_CAPK_FILENALE "emvbasecapk"
#define SDK_EMVBASE_GMCAPK_FILENALE "emvbasegmcapk"
#define SDK_EMVBASE_AID_FILENALE_TRANSFORMER "emvaids0.par"
#define SDK_EMVBASE_CAPK_FILENALE_TRANSFORMER "emvcapk0.par"


typedef struct _CAPKDATAINDEX
{
    u8 RID[5];
    u8 CAPKI;
    u8 ifusedflag;
}EMVBASECAPKDATAINDEX;


typedef struct _AIDDATAINDEX
{
    u8 tradetypevalue;
    u8 AIDLen;
    u8 AID[16];
    u8 ASI;                  //0-partial match up to the length;1-match exactly
    u8 ifusedflag;
	u8 contactorcontectless;
}EMVBASEAIDDATAINDEX;


#define EMVBASEMAXCAPKINDEX 100
EMVBASECAPKDATAINDEX gEmvbaseCapkIndex[EMVBASEMAXCAPKINDEX];

#define EMVBASEMAXAIDINDEX  50
EMVBASEAIDDATAINDEX gEmvbaseAidIndex[EMVBASEMAXAIDINDEX];

#define EMVBASEMAXGMCAPKINDEX 5
EMVBASECAPKDATAINDEX gEmvbaseGMCapkIndex[EMVBASEMAXGMCAPKINDEX];

EMVBase_UnionStruct *gstEMVBase_UnionStruct = NULL;
EMVBase_EntryPoint *gstEMVBase_EntryPoint = NULL;
EMVBase_TermAidlistUnionStruct *gstEMVBase_TermAidlistUnion = NULL;

SDK_EMVBASE_TRADE_PARAM *gstEMVBase_TradeParam = NULL;

static int emvbase_kern_create_file(const char *filename, u32 len,u32 num, u8 initialvalue)
{
    u8 *tempbuf;
    u16 i;
    u32 add = 0;
	s32 ret;

    tempbuf = (u8 *)sdkGetMem(len);
//	Trace("lishiyao", "sdkGetMem return:%p\r\n", tempbuf);
    memset(tempbuf, initialvalue, len);
//	Trace("lishiyao", "num = %d\r\n", num);

    for(i = 0; i < num; i++)
    {
        ret = sdkInsertFile(filename, tempbuf, add, len);
        add += len;
    }
    sdkFreeMem(tempbuf);
    return ret;

}


int sdkEMVBase_CheckMatchTermAID_CL(unsigned char* aid,unsigned char aidLen,EMVBASE_TERMAPP *Applist,unsigned char* kernelid,unsigned char kernelidlen)
{
    unsigned char i;
    unsigned char matchflag = 0;
    unsigned char capkmatchflag = 1;
	u8 type = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);


	Trace("emv","contactless 9C=%02X\r\n",type);
	TraceHex("emv","contactless aid:\r\n",aid, aidLen);
	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 2) && (type == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==2 && gEmvbaseAidIndex.tradetypevalue==9C\r\n");
				return i;
			}
		}
	}


	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 0) && (type == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==0 && gEmvbaseAidIndex.tradetypevalue==9C\r\n");
				return i;
			}
		}
	}


	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 2) && (0xFF == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==2 && gEmvbaseAidIndex.tradetypevalue==FF\r\n");
				return i;
			}
		}
	}


	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 0) && (0xFF == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==0 && gEmvbaseAidIndex.tradetypevalue==FF\r\n");
				return i;
			}
		}
	}

	Trace("emv","emv error ! not match aid\r\n");
    return -1;		// -1 means not find

}


int sdkEMVBase_CheckMatchTermAID_CT(unsigned char* aid,unsigned char aidLen,EMVBASE_TERMAPP *Applist,unsigned char* kernelid,unsigned char kernelidlen)
{
    unsigned char i;
    unsigned char matchflag = 0;
    //unsigned char capkmatchflag = 1;
	u8 type = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);

	Trace("emv","contact 9C=%02X\r\n",type);
	TraceHex("emv","contact aid:\r\n",aid, aidLen);

	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 1) && (type == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==1 && gEmvbaseAidIndex.tradetypevalue==9C\r\n");
				return i;
			}
		}
	}


	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 0) && (type == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==0 && gEmvbaseAidIndex.tradetypevalue==9C\r\n");
				return i;
			}
		}
	}


	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 1) && (0xFF == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==1 && gEmvbaseAidIndex.tradetypevalue==FF\r\n");
				return i;
			}
		}
	}


	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
	{
		if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].contactorcontectless == 0) && (0xFF == gEmvbaseAidIndex[i].tradetypevalue))
		{
			if(gEmvbaseAidIndex[i].ASI == AID_PARTIAL_MATCH)
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen))
				{
					matchflag = 1;
				}
			}
			else
			{
				if(!memcmp(aid, gEmvbaseAidIndex[i].AID, aidLen) && aidLen == gEmvbaseAidIndex[i].AIDLen)
				{
					matchflag = 1;
				}
			}

			if(matchflag)
			{
				Applist->ASI = gEmvbaseAidIndex[i].ASI;
				Applist->AIDLen = gEmvbaseAidIndex[i].AIDLen;
				memcpy(&Applist->AID[0], &gEmvbaseAidIndex[i].AID[0], 16);
				Trace("emv","match aid by gEmvbaseAidIndex.contactorcontectless==0 && gEmvbaseAidIndex.tradetypevalue==FF\r\n");
				return i;
			}
		}
	}

	Trace("emv","not match aid\r\n");
    return -1;		// -1 means not find
}


s32 sdkEMVBase_DealCLCardData(unsigned char *tempsenddata, unsigned short tempdatalen, EMVBASE_APDU_RESP * apdu_r)
{
	u16 templen = 0;
	u32 sw = 0;
	u8 sw_bcd[2] = {0};

	sdkDevContactlessSendAPDU(tempsenddata, tempdatalen, apdu_r->DataOut, &templen);

	apdu_r->ReadCardDataOk = APDUCOMMANDOK;
	if(templen > 2)
	{
		apdu_r->LenOut = templen - 2;
		apdu_r->SW1 = apdu_r->DataOut[templen - 2];
		apdu_r->SW2 = apdu_r->DataOut[templen - 1];
		apdu_r->DataOut[templen - 2] = 0;
		apdu_r->DataOut[templen - 1] = 0;
	}
	else if((templen == 2) && (apdu_r->DataOut[templen - 2] > 0))
	{
		apdu_r->LenOut = 0;
		apdu_r->SW1 = apdu_r->DataOut[templen - 2];
		apdu_r->SW2 = apdu_r->DataOut[templen - 1];
		apdu_r->DataOut[templen - 2] = 0;
		apdu_r->DataOut[templen - 1] = 0;
	}
	else if(templen < 0)
	{
		apdu_r->ReadCardDataOk = APDUCOMMCANCEL;
	}
	else
	{
		apdu_r->ReadCardDataOk = APDUCOMMANDERR;
	}

	return SDK_OK;
}


s32 sdkEMVBase_DealContactCardData(unsigned char *tempsenddata, unsigned short tempdatalen, EMVBASE_APDU_RESP * apdu_r)
{
    s32 templen = 0;

//    sdkDevContactSendAPDU(tempsenddata, tempdatalen, apdu_r->DataOut, &templen);
    apdu_r->ReadCardDataOk = APDUCOMMANDOK;

    if(templen > 2)
    {
        apdu_r->LenOut = templen - 2;
        apdu_r->SW1 = apdu_r->DataOut[templen - 2];
        apdu_r->SW2 = apdu_r->DataOut[templen - 1];
        apdu_r->DataOut[templen - 2] = 0;
        apdu_r->DataOut[templen - 1] = 0;
    }
    else if((templen == 2) && (apdu_r->DataOut[templen - 2] > 0))
    {
        apdu_r->LenOut = 0;
        apdu_r->SW1 = apdu_r->DataOut[templen - 2];
        apdu_r->SW2 = apdu_r->DataOut[templen - 1];
        apdu_r->DataOut[templen - 2] = 0;
        apdu_r->DataOut[templen - 1] = 0;
    }
    else
    {
        apdu_r->ReadCardDataOk = APDUCOMMANDERR;
    }

    return SDK_OK;
}

void sdkEMVBase_ContactlessIsoCommand(EMVBASE_APDU_SEND * ApduSend, EMVBASE_APDU_RESP * apdu_r)
{
    unsigned char *iccommand;
    //unsigned char retCode;
    int len = 0;

	ApduSend->Lc &= 0x00ff;

	len = ApduSend->Lc + 6;
    iccommand = (unsigned char *)emvbase_malloc(len);

	if(iccommand == NULL)
	{
		apdu_r->ReadCardDataOk = APDUCOMMANDERR;
		return;
	}

    memset(iccommand, 0, len);

	len = 0;
    memcpy(iccommand+len, ApduSend->Command, 4);
    len += 4;


    if(ApduSend->Lc > 0)
    {
        iccommand[len++] = ApduSend->Lc;

        memcpy(iccommand+len, ApduSend->DataIn, ApduSend->Lc);

        len += ApduSend->Lc;

        if(ApduSend->Le > 0)
        {
            ApduSend->Le &= 0x00ff;
            iccommand[len++] = ApduSend->Le;
        }
    }
    else
    {
        if(ApduSend->Le > 0)
        {
            ApduSend->Le &= 0x00ff;
            iccommand[len++] = ApduSend->Le;
        }
        else
        {
            iccommand[len++] = 0;
        }
    }
    memset((unsigned char *)apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
#if 0

	TraceHex("emv","contactless send apdu:", iccommand, len);

	{

		struct TESTLOG

		{

			u8 asSendCmd[4*2+1];

			u8 asSendData[255*2+1];

			u8 asRecvData[300*2+4+1];

		};



		struct TESTLOG log[]=

		{

			//PPSE
			{"00A40400", "325041592E5359532E4444463031","6F4F840E325041592E5359532E4444463031A53DBF0C3A611C4F07D9999999991010500E4170706C69636174696F6E203031870101611A4F05D777777777500E4170706C69636174696F6E2030328701029000"},
			//Select
			{"00A40400", "D9999999991010", "6F288407A0000000651010A51D500A4A4342204372656469749F380B9F52019F35019F02069C018701019000"},
			//GPO
			{"80A80000", "", "771282021D80940C1001010118010100200101009000"},
			//Select
			{"00B20114", "", "7081A25F200F4A43422043726564697420555345528C1B9F02069F03069F1A0295055F2A029A039C019F37049F35019F53038D098A02910A95059F37049F0702FFC05F280203929F420203928E100000000000000000410342035E031F005F25031301015F24034912315A0835408299994210125F3401009F0D0500000000009F0E0500000000009F0F05000000000057133540829999421012D49122015555555555552F9000"},
			//External authenticate
			{"00B2011C", "", "7081B48F01099F4A0182908180210B9821766D4AD4224D41BD4554F5B9CA2484D5FA0E1E7E07B1270AFAA16C72EE02A15C721CA6CD274890FF034FADD929C5EA9015619B03018272B486D7B0CD75C15FD17D249CBFE94719808B4FA43A62693DFFF91C938ED641189D68F614ED74716A7C9909920179CC715E47B25544E3C26D9B9A06D15BC121A9378D53DE9A9224C957310ACB7137C45110F3C85D6FBBEB1E3E7805F521B03AB89E9086EC85BC56585D4AE39F3201039000"},

			{"00B20124", "", "7081B59F46818067B74BE84242F5F62BD0D47B3263C32A2C2A9461FA8B32CE5BBA479D36A722DF33979D38F6E284985C82E66684DF95B02CF0131495FADF3EB605D8206DCAC9D26E5D62589341AFF6896B14390F7BE29387095BA7B296676A35EACF41752DC3D302049F5029E683FD5F4BAD819927784A44C2B5057445AC9365F2BCF22A375E6C9F4701039F482A58B57792CFDDAC3C0A5CC0A8D6D5F05A82D17F0E9C987038CD6B170E5BF843C9DD8983A1C88273451E279000"},

			{"00B20124", "", "7081B59F46818067B74BE84242F5F62BD0D47B3263C32A2C2A9461FA8B32CE5BBA479D36A722DF33979D38F6E284985C82E66684DF95B02CF0131495FADF3EB605D8206DCAC9D26E5D62589341AFF6896B14390F7BE29387095BA7B296676A35EACF41752DC3D302049F5029E683FD5F4BAD819927784A44C2B5057445AC9365F2BCF22A375E6C9F4701039F482A58B57792CFDDAC3C0A5CC0A8D6D5F05A82D17F0E9C987038CD6B170E5BF843C9DD8983A1C88273451E279000"},

			{"80AE9000", "", "77819C9F2701809F360200019F5001209F100807112233445566779F4B818043BBD829D8D6CD42261DB3B2F384D2E7D5342D8037A51F7B8DA9E804E91527749EE06DF3A80305B2E0FCADFFC2F79DEA825D7D98C27BFB3756953456A6C66617B8849DFD93A14D97151CE7CAEADD51010527E1A66645C46DC53BD5BA8EDDA54CC8A0F2D1F59D00B5E85F5F2EAF133739F4691BE36D59A6850B45618072477D8F9000"},


		};

		s32 k=0;

		u8 tempcmd[9]={0},tempdata[255*2+1];



		memset(apdu_r, 0, sizeof(EMVBASE_APDU_RESP));

		apdu_r->ReadCardDataOk = APDUCOMMANDERR;



		memset(tempcmd,0,sizeof(tempcmd));

		sdkBcdToAsc(tempcmd, ApduSend->Command, 4);

		memset(tempdata,0,sizeof(tempdata));

		sdkBcdToAsc(tempdata, ApduSend->DataIn, ApduSend->Lc);

		Trace("emv","cmd=%s\r\n",tempcmd);

		Trace("emv","data=%s\r\n",tempdata);

		for(k=0;k<sizeof(log)/sizeof(log[0]); k++)

		{

			if(!memcmp(tempcmd, log[k].asSendCmd, 8) && !memcmp(tempdata, log[k].asSendData, strlen(log[k].asSendData)))

			{
				Trace("lishiyao", "start Assignment\r\n");

				apdu_r->LenOut = (strlen(log[k].asRecvData)-4)/2;

				sdkAscToBcd(apdu_r->DataOut, log[k].asRecvData, strlen(log[k].asRecvData)-4);

				sdkAscToBcd(&apdu_r->SW1, log[k].asRecvData+(strlen(log[k].asRecvData)-4), 2);

				sdkAscToBcd(&apdu_r->SW2, log[k].asRecvData+(strlen(log[k].asRecvData)-4)+2, 2);

				apdu_r->ReadCardDataOk = APDUCOMMANDOK;

				break;
			}

		}

	}


	TraceHex("emv","contactless recv apdu:", apdu_r->DataOut, apdu_r->LenOut);

	Trace("emv","SWASWB=%02X%02X\r\n",apdu_r->SW1,apdu_r->SW2);

#else
	TraceHex("emv","contactless send apdu:", iccommand, len);
	sdkEMVBase_DealCLCardData(iccommand, len, apdu_r);
	TraceHex("emv","contactless recv apdu:", apdu_r->DataOut, apdu_r->LenOut);
#endif

    emvbase_free(iccommand);
}

void sdkEMVBase_ContactIsoCommand(EMVBASE_APDU_SEND * ApduSend, EMVBASE_APDU_RESP * apdu_r)
{
    unsigned char *iccommand;
    //unsigned char retCode;
    int len = 0;

    iccommand = (unsigned char *)emvbase_malloc(300);

	if(iccommand == NULL)
	{
		apdu_r->ReadCardDataOk = APDUCOMMANDERR;
		return;
	}

    memset(iccommand, 0, 300);
    memcpy(&iccommand[0], ApduSend->Command, 4);
    len = len + 4;
    ApduSend->Lc &= 0x00ff;

    if(ApduSend->Lc > 0)
    {
        iccommand[4] = ApduSend->Lc;
        len = len + 1;

        memcpy(&iccommand[5], ApduSend->DataIn, ApduSend->Lc);

        len += ApduSend->Lc;

        if(ApduSend->Le > 0)
        {
            ApduSend->Le &= 0x00ff;
            iccommand[5 + ApduSend->Lc] = ApduSend->Le;
            len = len + 1;
        }
    }
    else
    {
        if(ApduSend->Le > 0)
        {
            ApduSend->Le &= 0x00ff;
            iccommand[4 + ApduSend->Lc] = ApduSend->Le;
            len = len + 1;
        }
        else
        {
            len = len + 1;
            iccommand[4 + ApduSend->Lc] = 0;
        }
    }
    memset((unsigned char *)apdu_r, 0, sizeof(EMVBASE_APDU_RESP));
	TraceHex("emv","contact send apdu:", iccommand, len);
    sdkEMVBase_DealContactCardData(&iccommand[0], (unsigned short)(len), apdu_r);
	TraceHex("emv","contact recv apdu:", apdu_r->DataOut, apdu_r->LenOut);
    emvbase_free(iccommand);
}



s32 sdkEMVBaseEntryPointInit()
{
	s32 i=0;

	if(gstEMVBase_UnionStruct == NULL)
    {
        gstEMVBase_UnionStruct = (EMVBase_UnionStruct *)emvbase_malloc(sizeof(EMVBase_UnionStruct));
    }
	else
	{
		if(gstEMVBase_UnionStruct->rapdu)
		{
			emvbase_free(gstEMVBase_UnionStruct->rapdu);
			gstEMVBase_UnionStruct->rapdu = NULL;
		}
	}

	if(gstEMVBase_EntryPoint == NULL)
	{
		gstEMVBase_EntryPoint = (EMVBase_EntryPoint *)emvbase_malloc(sizeof(EMVBase_EntryPoint));
	}
	else
	{
		if(gstEMVBase_EntryPoint->SelectedApp)
		{
			if(gstEMVBase_EntryPoint->SelectedApp->ASRPDLen && gstEMVBase_EntryPoint->SelectedApp->ASRPD)
			{
				emvbase_free(gstEMVBase_EntryPoint->SelectedApp->ASRPD);
				gstEMVBase_EntryPoint->SelectedApp->ASRPDLen = 0;
				gstEMVBase_EntryPoint->SelectedApp->ASRPD = NULL;
			}
			emvbase_free(gstEMVBase_EntryPoint->SelectedApp);
			gstEMVBase_EntryPoint->SelectedApp = NULL;
		}
		if(gstEMVBase_EntryPoint->AppListCandidate)
		{
			for(i=0; i<gstEMVBase_EntryPoint->AppListCandidateMaxNum; i++)
			{
				if(gstEMVBase_EntryPoint->AppListCandidate[i].ASRPDLen && gstEMVBase_EntryPoint->AppListCandidate[i].ASRPD)
				{
					emvbase_free(gstEMVBase_EntryPoint->AppListCandidate[i].ASRPD);
					gstEMVBase_EntryPoint->AppListCandidate[i].ASRPDLen = 0;
					gstEMVBase_EntryPoint->AppListCandidate[i].ASRPD = NULL;
				}
			}
			emvbase_free(gstEMVBase_EntryPoint->AppListCandidate);
			gstEMVBase_EntryPoint->AppListCandidate = NULL;
		}
	}

	memset(gstEMVBase_UnionStruct,0,sizeof(EMVBase_UnionStruct));
	memset(gstEMVBase_EntryPoint,0,sizeof(EMVBase_EntryPoint));

    gstEMVBase_EntryPoint->SelectedApp = (EMVBASE_LISTAPPDATA *)emvbase_malloc(sizeof(EMVBASE_LISTAPPDATA));
    memset((unsigned char *)gstEMVBase_EntryPoint->SelectedApp, 0, sizeof(EMVBASE_LISTAPPDATA));

    gstEMVBase_EntryPoint->AppListCandidate = (EMVBASE_LISTAPPDATA *)emvbase_malloc(sizeof(EMVBASE_LISTAPPDATA) * gAppListCandicateMaxNum);
    memset((unsigned char *)gstEMVBase_EntryPoint->AppListCandidate, 0, sizeof(EMVBASE_LISTAPPDATA) * gAppListCandicateMaxNum);
	gstEMVBase_EntryPoint->AppListCandidateMaxNum = gAppListCandicateMaxNum;

    gstEMVBase_UnionStruct->EntryPoint = gstEMVBase_EntryPoint;
    gstEMVBase_UnionStruct->IsoCommand = sdkEMVBase_ContactlessIsoCommand;
    gstEMVBase_UnionStruct->CheckMatchTermAID = sdkEMVBase_CheckMatchTermAID_CL;
	gstEMVBase_UnionStruct->rapdu = NULL;

	gstEMVBase_UnionStruct->CheckTag84 = 0;
	gstEMVBase_UnionStruct->PpseRespType = 0;
	if(gstEMVBase_TradeParam)
	{
		gstEMVBase_UnionStruct->SupportExternSelect = gstEMVBase_TradeParam->SupportExternSelect;
		gstEMVBase_UnionStruct->PpseRespType = gstEMVBase_TradeParam->PpseRespType;
	}

    return SDK_OK;
}


s32 sdkEMVBase_FinalSelect(EMVBase_UnionStruct *tempEMVBase_UnionStruct,SDK_EMVBASE_CL_HIGHESTAID *tempHighestAID)
{
    unsigned char tempselectappno;
    EMVBASE_LISTAPPDATA *tempAppAppData;


    tempAppAppData = tempEMVBase_UnionStruct->EntryPoint->AppListCandidate;
    tempselectappno = tempEMVBase_UnionStruct->SelectedAppNo;


	tempHighestAID->aidlen = (tempAppAppData + tempselectappno)->AIDLen;
	memcpy(tempHighestAID->aid,(tempAppAppData + tempselectappno)->AID,tempHighestAID->aidlen);

	tempHighestAID->kernelidlen = (tempAppAppData + tempselectappno)->KernelIdentifierLen;
	memcpy(tempHighestAID->kernelid,(tempAppAppData + tempselectappno)->KernelIdentifier,tempHighestAID->kernelidlen);
	return SDK_OK;
}


s32 sdkEMVBaseEntryPoint(SDK_EMVBASE_CL_HIGHESTAID *tempHighestAID)
{
	unsigned char emvbase_rslt;
	s32 sdk_rslt;

	if(tempHighestAID == NULL)
	{
		return SDK_PARA_ERR;
	}

	tempHighestAID->aidlen = 0;
	tempHighestAID->kernelidlen = 0;

	while(1)
	{
		sdkEMVBaseEntryPointInit();

		emvbase_rslt = EMVBase_SelectPPSE(gstEMVBase_UnionStruct);
		Trace("emv", "EMVBase_SelectPPSE ret = %d\r\n",emvbase_rslt);
		if(emvbase_rslt != RLT_EMV_OK)
		{
			break;
		}

		emvbase_rslt = EMVBase_ChooseApp(gstEMVBase_UnionStruct);
		Trace("emv", "EMVBase_ChooseApp ret = %d\r\n",emvbase_rslt);
		if(emvbase_rslt != RLT_EMV_OK)
		{
			break;
		}

		sdkEMVBase_FinalSelect(gstEMVBase_UnionStruct,tempHighestAID);
		break;
	}
	sdkEMVBaseRltToSdkRlt(emvbase_rslt, &sdk_rslt);

	TraceHex("emv", "Entry Point Select Aid:", tempHighestAID->aid, tempHighestAID->aidlen);
	if(tempHighestAID->kernelidlen)
	{
		TraceHex("emv", "Entry Point Select Aid Kernel ID:", tempHighestAID->kernelid, tempHighestAID->kernelidlen);
	}

	return sdk_rslt;
}

s32 sdkEMVBaseReSelectApp(SDK_EMVBASE_CL_HIGHESTAID *tempHighestAID)
{
    u8 emvbase_rslt;
    s32 rlt = SDK_ERR;

	if(tempHighestAID == NULL)
	{
		return SDK_PARA_ERR;
	}

	tempHighestAID->aidlen = 0;
	tempHighestAID->kernelidlen = 0;

    emvbase_rslt = EMVBase_RebuildApplist(gstEMVBase_UnionStruct);
	Trace("emv", "EMVBase_RebuildApplist ret = %d\r\n", emvbase_rslt);
    if(RLT_ERR_EMV_NoAppSel == emvbase_rslt)
    {
        emvbase_rslt = RLT_ERR_EMV_NoAppSel;
    }

	else if(RLT_EMV_OK == emvbase_rslt)
	{
		emvbase_rslt = EMVBase_ChooseApp(gstEMVBase_UnionStruct);
		Trace("emv", "EMVBase_ChooseApp ret = %d\r\n", emvbase_rslt);

		if(emvbase_rslt == RLT_EMV_OK)
		{
			sdkEMVBase_FinalSelect(gstEMVBase_UnionStruct,tempHighestAID);
		}
	}
    else
    {
        emvbase_rslt = RLT_EMV_ERR;
    }
    sdkEMVBaseRltToSdkRlt(emvbase_rslt, &rlt);

	TraceHex("emv", "Entry Point ReSelect Aid:", tempHighestAID->aid, tempHighestAID->aidlen);
	if(tempHighestAID->kernelidlen)
	{
		TraceHex("emv", "Entry Point ReSelect Aid Kernel ID:", tempHighestAID->kernelid, tempHighestAID->kernelidlen);
	}

    return rlt;
}


static s32 emvbase_manage_aidread(u32 id, EMVBASE_AID_STRUCT * tmpaid)
{
    u32 addr;
    u8 *tmp;
    s32 ret;
    u8 checksum;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 readfilelen;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);


    tmp = (u8 *)emvbase_malloc(EMVBASEAIDLEN);
    addr = id << EMVBASEAIDSHIFTNUM;
//	addr = id * sizeof(EMVBASE_AID_STRUCT);
    readfilelen = EMVBASEAIDLEN;
    ret = sdkReadFile(fn, tmp, addr, &readfilelen);

    if(ret != SDK_OK)
    {
        emvbase_free(tmp);
        return 1;
    }

    if(tmp[EMVBASEAIDFLAGOFFSET] != EMVBASEAIDENABLE)
    {
        emvbase_free(tmp);
        return 1;
    }
    memcpy((u8 *)tmpaid, tmp, sizeof(EMVBASE_AID_STRUCT));

    emvbase_free(tmp);

    return 0;
}


s32 sdkEMVBaseReadAIDParameters(u8 *aid,u16 aidlen,EMVBASE_AID_STRUCT *tempaidparam,u8 transtpye,u8 contactorcontactless)
{
    u8 i;

	Trace("read aid", "read position 1\r\n");
    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if( gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE &&
			(memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen) == 0) && (aidlen == gEmvbaseAidIndex[i].AIDLen) &&
			transtpye == gEmvbaseAidIndex[i].tradetypevalue && contactorcontactless == gEmvbaseAidIndex[i].contactorcontectless
			)
        {
            {
                if(emvbase_manage_aidread(i, tempaidparam))
                {
                    return SDK_ERR;
                }
                return SDK_OK;
            }
        }
    }
	Trace("read aid", "read position 2\r\n");
    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if( gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE &&
			(memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen) == 0) && (aidlen == gEmvbaseAidIndex[i].AIDLen) &&
			transtpye == gEmvbaseAidIndex[i].tradetypevalue && 0 == gEmvbaseAidIndex[i].contactorcontectless)
        {
            {
                if(emvbase_manage_aidread(i, tempaidparam))
                {
                    return SDK_ERR;
                }
                return SDK_OK;
            }
        }
    }
	Trace("read aid", "read position 3\r\n");
	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if( gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE &&
			(memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen) == 0) && (aidlen == gEmvbaseAidIndex[i].AIDLen) &&
			0xFF == gEmvbaseAidIndex[i].tradetypevalue && contactorcontactless == gEmvbaseAidIndex[i].contactorcontectless)
        {
            {
                if(emvbase_manage_aidread(i, tempaidparam))
                {
                    return SDK_ERR;
                }
                return SDK_OK;
            }
        }
    }
	Trace("read aid", "read position 4\r\n");
	for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if( gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE && (memcmp(aid, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen) == 0) && (aidlen == gEmvbaseAidIndex[i].AIDLen) &&
			0xFF == gEmvbaseAidIndex[i].tradetypevalue && 0 == gEmvbaseAidIndex[i].contactorcontectless)
        {
            {
                if(emvbase_manage_aidread(i, tempaidparam))
                {
                    return SDK_ERR;
                }
                return SDK_OK;
            }
        }
    }
	Trace("read aid", "read position 5\r\n");
	return SDK_ERR;
}


s32 sdkEMVBaseDelOneAID(const u8 *pheInAID, s32 siAIDLen)
{
    u8 i,j = 0;
    u8 *tmp;
    u32 addr;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 ret;

    if(pheInAID == NULL || siAIDLen < 0)
    {
        return SDK_PARA_ERR;
    }
    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);

    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if(gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE && gEmvbaseAidIndex[i].AIDLen == siAIDLen)
        {
            if(!memcmp(gEmvbaseAidIndex[i].AID, pheInAID, siAIDLen))
            {
                tmp = (u8 *)emvbase_malloc(EMVBASEAIDLEN);
                memset(tmp, 0xFF, EMVBASEAIDLEN);
//                addr = i;
//                addr = addr << EMVBASEAIDSHIFTNUM;
				addr = i * sizeof(EMVBASE_AID_STRUCT);
                ret = sdkInsertFile(fn, tmp, addr, EMVBASEAIDLEN);
                emvbase_free(tmp);

                if(ret == SDK_OK)
                {
                    memset(&gEmvbaseAidIndex[i], 0xFF, sizeof(EMVBASEAIDDATAINDEX));
                    //return SDK_OK;
                    j ++;
                }
                else
                {
                    return SDK_ERR;
                }
            }
        }
    }

	if(j > 0)
	{
		return SDK_OK;
	}
	else
	{
	    return SDK_ICC_NODATA;
	}
}


int sdkEMVBaseDelAllAIDLists(void)
{
    u8 i;
    u8 haveaid = 0;
    u8 fn[SDK_EMVBASE_MAX_PATH];
	s32 ret;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);

    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if(gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE)
        {
            haveaid = 1;
            break;
        }
    }

    if(haveaid == 0)
    {
        return;
    }
    ret = sdkDelFile(fn);
	if(ret == SDK_ERR)
	{
		Trace("Delete AID", "Error Code:-7\r\n");
		Trace("Delete AID", "Info:Remove File Error\r\n");
	}
    memset(&gEmvbaseAidIndex[0], 0xFF, sizeof(EMVBASEAIDDATAINDEX) * EMVBASEMAXAIDINDEX);
    emvbase_kern_create_file(fn, (u32)EMVBASEAIDLEN,(u32)EMVBASEMAXAIDINDEX,0xff);

    return ret;
}

static void emvbase_SDK_AID_TO_AID(EMVBASE_AID_STRUCT *tempaid, const SDK_EMVBASE_AID_STRUCT *pstInAidList)
{
	tempaid->AidLen = pstInAidList->AidLen;
    memcpy(tempaid->Aid, (u8 *)(pstInAidList->Aid), 16);
	tempaid->Asi = pstInAidList->Asi;
    memcpy(tempaid->AppVerNum, (u8 *)(pstInAidList->AppVerNum), 2);
	memcpy(tempaid->TacDefault, (u8 *)(pstInAidList->TacDefault), 5);
	memcpy(tempaid->TacOnline, (u8 *)(pstInAidList->TacOnline), 5);
	memcpy(tempaid->TacDecline, (u8 *)(pstInAidList->TacDecline), 5);
	memcpy(tempaid->FloorLimit, (u8 *)(pstInAidList->FloorLimit), 4);
	memcpy(tempaid->Threshold, (u8 *)(pstInAidList->Threshold), 4);
	tempaid->MaxTargetPercent = pstInAidList->MaxTargetPercent;
	tempaid->TargetPercent = pstInAidList->TargetPercent;
	tempaid->TermDDOLLen = pstInAidList->TermDDOLLen;
	memcpy(tempaid->TermDDOL, (u8 *)(pstInAidList->TermDDOL), 128);
	tempaid->TermPinCap = pstInAidList->TermPinCap;
	memcpy(tempaid->ectranslimit, (u8 *)(pstInAidList->ectranslimit), 6);
	memcpy(tempaid->cl_cvmlimit, (u8 *)(pstInAidList->cl_cvmlimit), 6);
	memcpy(tempaid->cl_translimit, (u8 *)(pstInAidList->cl_translimit), 6);
	memcpy(tempaid->cl_offlinelimit, (u8 *)(pstInAidList->cl_offlinelimit), 6);
	tempaid->transvaule = pstInAidList->transvaule;
	tempaid->contactorcontactless = pstInAidList->contactorcontactless;
}

static void emvbase_AID_TO_SDK_AID(SDK_EMVBASE_AID_STRUCT *pstInAidList, const EMVBASE_AID_STRUCT *tempaid)
{
	pstInAidList->AidLen = tempaid->AidLen;
    memcpy(pstInAidList->Aid, (u8 *)(tempaid->Aid), 16);
	pstInAidList->Asi = tempaid->Asi;
    memcpy(pstInAidList->AppVerNum, (u8 *)(tempaid->AppVerNum), 2);
	memcpy(pstInAidList->TacDefault, (u8 *)(tempaid->TacDefault), 5);
	memcpy(pstInAidList->TacOnline, (u8 *)(tempaid->TacOnline), 5);
	memcpy(pstInAidList->TacDecline, (u8 *)(tempaid->TacDecline), 5);
	memcpy(pstInAidList->FloorLimit, (u8 *)(tempaid->FloorLimit), 4);
	memcpy(pstInAidList->Threshold, (u8 *)(tempaid->Threshold), 4);
	pstInAidList->MaxTargetPercent = tempaid->MaxTargetPercent;
	pstInAidList->TargetPercent = tempaid->TargetPercent;
	pstInAidList->TermDDOLLen = tempaid->TermDDOLLen;
	memcpy(pstInAidList->TermDDOL, (u8 *)(tempaid->TermDDOL), 128);
	pstInAidList->TermPinCap = tempaid->TermPinCap;
	memcpy(pstInAidList->ectranslimit, (u8 *)(tempaid->ectranslimit), 6);
	memcpy(pstInAidList->cl_cvmlimit, (u8 *)(tempaid->cl_cvmlimit), 6);
	memcpy(pstInAidList->cl_translimit, (u8 *)(tempaid->cl_translimit), 6);
	memcpy(pstInAidList->cl_offlinelimit, (u8 *)(tempaid->cl_offlinelimit), 6);
	pstInAidList->transvaule = tempaid->transvaule;
	pstInAidList->contactorcontactless = tempaid->contactorcontactless;
}

static bool emvbase_AID_IS_EQUAL(const EMVBASE_AID_STRUCT *tempaid, const SDK_EMVBASE_AID_STRUCT *pstInAidList)
{
    if( tempaid->AidLen == pstInAidList->AidLen &&
        tempaid->Asi == pstInAidList->Asi &&
        tempaid->MaxTargetPercent == pstInAidList->MaxTargetPercent &&
        tempaid->TargetPercent == pstInAidList->TargetPercent &&
        tempaid->TermDDOLLen == pstInAidList->TermDDOLLen &&
        tempaid->TermPinCap == pstInAidList->TermPinCap &&
        tempaid->transvaule == pstInAidList->transvaule &&
        tempaid->contactorcontactless == pstInAidList->contactorcontactless &&
        !memcmp(tempaid->Aid, pstInAidList->Aid, tempaid->AidLen) &&
        !memcmp(tempaid->AppVerNum, pstInAidList->AppVerNum, 2) &&
        !memcmp(tempaid->TacDefault, pstInAidList->TacDefault, 5) &&
        !memcmp(tempaid->TacOnline, pstInAidList->TacOnline, 5) &&
        !memcmp(tempaid->TacDecline, pstInAidList->TacDecline, 5) &&
        !memcmp(tempaid->FloorLimit, pstInAidList->FloorLimit, 4) &&
        !memcmp(tempaid->Threshold, pstInAidList->Threshold, 4) &&
        !memcmp(tempaid->TermDDOL, pstInAidList->TermDDOL, tempaid->TermDDOLLen) &&
        !memcmp(tempaid->ectranslimit, pstInAidList->ectranslimit, 6) &&
        !memcmp(tempaid->cl_cvmlimit, pstInAidList->cl_cvmlimit, 6) &&
        !memcmp(tempaid->cl_translimit, pstInAidList->cl_translimit, 6) &&
        !memcmp(tempaid->cl_offlinelimit, pstInAidList->cl_offlinelimit, 6) &&
        tempaid->transvaule == pstInAidList->transvaule &&
        tempaid->contactorcontactless == pstInAidList->contactorcontactless)
    {
        return true;
    }
    else
    {
        return false;
    }
}


s32 sdkEMVBaseAddAnyAIDList(const SDK_EMVBASE_AID_STRUCT *pstInAIDList, s32 siAIDNum)
{
    u8 i, j;
    s32 ret = SDK_OK, ret1;
    u8 iffind = 0;
    u8 *tmp;
    s32 tmplen;
    u32 addr;
    u8 fn[SDK_EMVBASE_MAX_PATH];

	Trace("emv","goto sdkEMVBaseAddAnyAIDList\r\n");

    if(pstInAIDList == NULL || siAIDNum > EMVBASEMAXAIDINDEX || siAIDNum <= 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);


    tmp = (u8 *)emvbase_malloc(EMVBASEAIDLEN);

    for(j = 0; j < siAIDNum; j++)
    {
        iffind = 0;

        if((pstInAIDList + j)->AidLen > 16)
        {
            emvbase_free(tmp);
            return SDK_PARA_ERR;
        }

        for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
        {
            if((gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE) && (gEmvbaseAidIndex[i].AIDLen == (pstInAIDList + j)->AidLen) &&
               (!memcmp(gEmvbaseAidIndex[i].AID, (pstInAIDList + j)->Aid, (pstInAIDList + j)->AidLen)) &&
               	(gEmvbaseAidIndex[i].tradetypevalue == pstInAIDList[j].transvaule) &&
            	(gEmvbaseAidIndex[i].contactorcontectless == pstInAIDList[j].contactorcontactless))
            {
                iffind = 1;
                addr = ((u32)i) << EMVBASEAIDSHIFTNUM;
                tmplen = EMVBASEAIDLEN;
//				addr = i * sizeof(SDK_EMVBASE_AID_STRUCT);
                ret1 = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret1 != SDK_OK)
                {
                    break;
                }

				if(emvbase_AID_IS_EQUAL((EMVBASE_AID_STRUCT *)tmp,(pstInAIDList + j)))
                {
                    iffind = 2;
                    break;
                }
                break;
            }
        }


        if(iffind == 0)
        {
            for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
            {
                if(gEmvbaseAidIndex[i].ifusedflag != EMVBASEAIDENABLE)
                {
                    break;
                }
            }
        }
        else if(iffind == 2)
        {
            continue;
        }

        if(i >= EMVBASEMAXAIDINDEX)
        {
            ret = SDK_ICC_OVER_LIMIT;
            break;
        }

        memset(tmp, 0xff, EMVBASEAIDLEN);
        emvbase_SDK_AID_TO_AID((EMVBASE_AID_STRUCT *)tmp,(pstInAIDList + j));
        tmp[EMVBASEAIDFLAGOFFSET] = EMVBASEAIDENABLE;
        addr = i;
        addr = addr << EMVBASEAIDSHIFTNUM;
        ret1 = sdkInsertFile(fn, tmp, addr, EMVBASEAIDLEN);
//		Trace("file", "sdkInsertFile ret = %d\r\n", ret1);
        if(ret1 == SDK_OK)
        {
            tmplen = EMVBASEAIDLEN;
            ret1 = sdkReadFile(fn, tmp, addr, &tmplen);
//			Trace("file", "sdkReadFile ret = %d\r\n", ret1);
            gEmvbaseAidIndex[i].AIDLen = pstInAIDList[j].AidLen;
            memcpy((gEmvbaseAidIndex[i].AID), (u8 *)((pstInAIDList + j)->Aid), 16);
            gEmvbaseAidIndex[i].ASI = pstInAIDList[j].Asi;
            gEmvbaseAidIndex[i].ifusedflag = EMVBASEAIDENABLE;
			gEmvbaseAidIndex[i].tradetypevalue = pstInAIDList[j].transvaule;
            gEmvbaseAidIndex[i].contactorcontectless = pstInAIDList[j].contactorcontactless;
        }
        else
        {
            ret = SDK_ERR;
            break;
        }
    }

    emvbase_free(tmp);

    return ret;
}


s32 sdkEMVBaseGetAnyAIDList(s32 siStartIndex, s32 siAIDNum, SDK_EMVBASE_AID_STRUCT *pstOutAIDList, s32 *psiOutAIDNum)
{
    u32 i;
    u8 j;
    s32 ret;
    s32 addr, tmplen;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    u8 *tmp;

	Trace("emv","goto sdkEMVBaseGetAnyAIDList\r\n");

    *psiOutAIDNum = 0;

    if(siAIDNum == 0)
    {
        return SDK_OK;
    }
    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);

    j = 0;
    tmp = (u8 *)emvbase_malloc(EMVBASEAIDLEN);

    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if(gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE)
        {
            if(j >= siStartIndex)
            {
//                addr = ((u32)i) << EMVBASEAIDSHIFTNUM;
                tmplen = EMVBASEAIDLEN;
				addr = i * sizeof(EMVBASE_AID_STRUCT);
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret != SDK_OK)
                {
                    sdkEMVBaseLoadAIDList();
                    break;
                }
                emvbase_AID_TO_SDK_AID(pstOutAIDList + j - siStartIndex, (EMVBASE_AID_STRUCT *)tmp);
                j++;

                if(siStartIndex + siAIDNum == j)
                {
                    break;
                }
            }
            else
            {
                j++;
            }
        }
    }


    if(j >= siStartIndex)
    {
        *psiOutAIDNum = j - siStartIndex;
    }
    emvbase_free(tmp);

    return SDK_OK;
}

s32 sdkEMVBaseGetAIDListNum(s32 *psiInAIDListNum)
{
    u8 i, j;


    if(psiInAIDListNum == NULL)
    {
        return SDK_PARA_ERR;
    }
    j = 0;

    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if(gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE)
        {
            j++;
        }
    }

    *psiInAIDListNum = j;

    return SDK_OK;
}


s32 sdkEMVbaseGetMatchAID(const u8 *pheInAID, s32 siAIDLen, SDK_EMVBASE_AID_STRUCT *pstOutAID,s32 MaxOutAIDNum,s32 *psiOutAIDNum)
{
    u8 i,j;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 addr;
    s32 tmplen;
    s32 ret;
    //u8 checksum;
    u8 *tmp = NULL;


    if(pheInAID == NULL || pstOutAID == NULL || siAIDLen < 0 || siAIDLen > 16 || MaxOutAIDNum < 1 || psiOutAIDNum == NULL)
    {
        return SDK_PARA_ERR;
    }
	*psiOutAIDNum = 0;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);

    for(i = 0,j = 0; i < EMVBASEMAXAIDINDEX && j < MaxOutAIDNum; i++)
    {
        if(gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE)
        {
            if((gEmvbaseAidIndex[i].AIDLen == siAIDLen) && (!memcmp(gEmvbaseAidIndex[i].AID, pheInAID, siAIDLen)))
            {
                tmp = (u8 *)emvbase_malloc(EMVBASEAIDLEN);
//                addr = ((u32)i) << EMVBASEAIDSHIFTNUM;
                tmplen = EMVBASEAIDLEN;
				addr = i * sizeof(EMVBASE_AID_STRUCT);
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret == SDK_OK)
                {
					emvbase_AID_TO_SDK_AID(pstOutAID + j,(EMVBASE_AID_STRUCT *)tmp);
					j ++;
					ret = SDK_OK;
                }
                else
                {
                    ret = SDK_ERR;
					emvbase_free(tmp);
                	return ret;
                }
            }
        }
    }

	if(tmp)
	{
		emvbase_free(tmp);
	}
	if(j > 0)
	{
		*psiOutAIDNum = j;
		return SDK_OK;
	}
    return SDK_ICC_NODATA;
}


static void emvbase_manage_aidindexread(const u8 *aidfiledir)
{
    s32 ret;
    u8 i;
    u32 tmpaddr;
    u32 readlen;
    u8 *tmp;
    u32 addr;
    u32 filelen;
    EMVBASE_AID_STRUCT *termaidparam;

    memset(&gEmvbaseAidIndex[0], 0xFF, sizeof(EMVBASEAIDDATAINDEX) * EMVBASEMAXAIDINDEX);

    if(sdkGetFileSize(aidfiledir) != (u32)EMVBASEMAXAIDINDEX * EMVBASEAIDLEN)
    {
//        sdkDelFile(aidfiledir);
        ret = emvbase_kern_create_file(aidfiledir, (u32)EMVBASEAIDLEN,(u32)EMVBASEMAXAIDINDEX, 0xff);

        if(ret != 0)
        {
            return;
        }
    }
    filelen = (u32)EMVBASEMAXAIDINDEX * EMVBASEAIDLEN;
    addr = 0;

    tmp = (u8 *)emvbase_malloc(EMVBASEEMVBlockLen);


    for(i = 0; i < EMVBASEMAXAIDINDEX; )
    {
        if((addr + EMVBASEEMVBlockLen) <= filelen)
        {
            readlen = EMVBASEEMVBlockLen;
        }
        else
        {
            readlen = filelen - addr;
        }
        ret = sdkReadFile(aidfiledir, tmp, addr, &readlen);

        if(ret != SDK_OK)
        {
            emvbase_free(tmp);
            return;
        }

        for(tmpaddr = 0; tmpaddr < readlen; )
        {
            termaidparam = (EMVBASE_AID_STRUCT *)&tmp[tmpaddr];
			gEmvbaseAidIndex[i].tradetypevalue = termaidparam->transvaule;
            gEmvbaseAidIndex[i].AIDLen = termaidparam->AidLen;
            memcpy(&(gEmvbaseAidIndex[i].AID[0]), termaidparam->Aid, 16);
            gEmvbaseAidIndex[i].ASI = termaidparam->Asi;
			gEmvbaseAidIndex[i].contactorcontectless = termaidparam->contactorcontactless;
            gEmvbaseAidIndex[i].ifusedflag = tmp[EMVBASEAIDFLAGOFFSET + tmpaddr];

            i++;

            if(i >= EMVBASEMAXAIDINDEX)
            {
                break;
            }
            tmpaddr += EMVBASEAIDLEN;
        }

        addr += readlen;
    }

    emvbase_free(tmp);
}


s32 sdkEMVBaseCreateAIDFile(void)
{
    u8 fn[SDK_EMVBASE_MAX_PATH];
	s32 ret;

    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);

	ret = emvbase_kern_create_file(fn, (u32)EMVBASEAIDLEN,(u32)EMVBASEMAXAIDINDEX, 0xff);
	return ret;
}

s32 sdkEMVBaseLoadAIDList(void)
{
    u8 fn[SDK_EMVBASE_MAX_PATH];
	s32 ret;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_AID_FILENALE);

	ret = sdkGetFileSize(fn);
	Trace("emv", "Get AID File ret = %d\r\n", ret);
	if(ret == 0 || ret == SDK_FUN_NULL)
	{
		sdkEMVBaseCreateAIDFile();
	}

    emvbase_manage_aidindexread(fn);

    return SDK_OK;
}

s32 sdkEMVBaseDelOneCAPK(const u8 *pheInRID, s32 siCAPKIndex)
{
    u8 i;
    u8 *tmp;
    s32 ret;
    s32 addr;
    u8 fn[SDK_EMVBASE_MAX_PATH];


    if(pheInRID  == NULL || siCAPKIndex < 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);


    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if(gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            if( !memcmp(gEmvbaseCapkIndex[i].RID, pheInRID, 5) && gEmvbaseCapkIndex[i].CAPKI == siCAPKIndex )
            {
                tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);
                memset(tmp, 0xFF, EMVBASECAPKLEN);
                addr = i;
                addr = addr << EMVBASECAPKSHIFTNUM;
                ret = sdkInsertFile(fn, tmp, addr, EMVBASECAPKLEN);
                emvbase_free(tmp);

                if(ret == SDK_OK)
                {
                    return SDK_OK;
                }
                else
                {
                    return SDK_ERR;
                }
            }
        }
    }
    return SDK_ICC_NODATA;
}


s32 sdkEMVBaseDelOneGMCAPK(const u8 *pheInRID, s32 siCAPKIndex)
{
    u8 i;
    u8 *tmp;
    s32 ret;
    s32 addr;
    u8 fn[SDK_EMVBASE_MAX_PATH];


    if(pheInRID  == NULL || siCAPKIndex < 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);


    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
    {
        if(gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            if( !memcmp(gEmvbaseGMCapkIndex[i].RID, pheInRID, 5) && gEmvbaseGMCapkIndex[i].CAPKI == siCAPKIndex )
            {
                tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);
                memset(tmp, 0xFF, EMVBASECAPKLEN);
                addr = i;
                addr = addr << EMVBASECAPKSHIFTNUM;
                ret = sdkInsertFile(fn, tmp, addr, EMVBASECAPKLEN);
                emvbase_free(tmp);

                if(ret == SDK_OK)
                {
                    return SDK_OK;
                }
                else
                {
                    return SDK_ERR;
                }
            }
        }
    }
    return SDK_ICC_NODATA;
}


int sdkEMVBaseDelAllCAPKLists(void)
{
    u8 caplexistflag = 0;
    u8 i;
    u8 fn[SDK_EMVBASE_MAX_PATH];
	s32 ret;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if(gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            caplexistflag = 1;
            break;
        }
    }

    if(caplexistflag == 0)
    {
        return;
    }
    ret = sdkDelFile(fn);
	if(ret == SDK_ERR)
	{
		Trace("Delete CAPK", "Error Code:-7\r\n");
		Trace("Delete CAPK", "Info:Remove File Error\r\n");
	}
    memset(&gEmvbaseCapkIndex[0], 0xFF, EMVBASEMAXCAPKINDEX * sizeof(EMVBASECAPKDATAINDEX));
    emvbase_kern_create_file(fn, (u32)EMVBASECAPKLEN,(u32)EMVBASEMAXCAPKINDEX, 0xff);
	return ret;
}



void sdkEMVBaseDelAllGMCAPKLists(void)
{
    u8 caplexistflag = 0;
    u8 i;
    u8 fn[SDK_EMVBASE_MAX_PATH];


    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);

    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
    {
        if(gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            caplexistflag = 1;
            break;
        }
    }

    if(caplexistflag == 0)
    {
        return;
    }
    sdkDelFile(fn);
    memset(&gEmvbaseGMCapkIndex[0], 0xFF, EMVBASEMAXGMCAPKINDEX * sizeof(EMVBASECAPKDATAINDEX));
    emvbase_kern_create_file(fn, (u32)EMVBASECAPKLEN,(u32)EMVBASEMAXGMCAPKINDEX, 0xff);

}


s32 sdkEMVBaseVerifyCAPK(const SDK_EMVBASE_CAPK_STRUCT* capk)
{
    u8 checkSum[20];
    u32 uiIndex;
    s32 retcode;
    u8 *checkData = NULL;

    s32 checkData_size = 5 + 1 + capk->ModulLen + capk->ExponentLen;

    checkData = (u8*)sdkGetMem(checkData_size);

    if(NULL == checkData)
    {
        return SDK_ERR;
    }
    memset(checkData, 0, checkData_size);

    memcpy(checkData, (u8 *)capk->RID, 5);
    uiIndex = 5;
    checkData[5] = capk->CAPKI;
    uiIndex += 1;

    memcpy((u8*)&checkData[uiIndex], (u8 *)(capk->Modul), capk->ModulLen);
    uiIndex += capk->ModulLen;

    memcpy((u8*)&checkData[uiIndex], (u8 *)(capk->Exponent), capk->ExponentLen);
    uiIndex += capk->ExponentLen;


    EMVBase_Hash(checkData, uiIndex, checkSum);

    if(capk->CheckSumLen && memcmp(capk->CheckSum, checkSum, 20))
    {
        retcode = SDK_ERR;
    }
    else
    {
        retcode = SDK_OK;
    }
    Trace("emv", "sdkEMVBaseVerifyCAPK retcode = %d\r\n", retcode);

    sdkFreeMem(checkData);

    return retcode;
}

static void emvbase_SDK_CAPK_TO_CAPK(EMVBASE_CAPK_STRUCT *tempcapk, const SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList)
{
    memcpy(tempcapk->RID, (u8 *)(pstInCAPKList->RID), 5);
    tempcapk->CAPKI = pstInCAPKList->CAPKI;
    tempcapk->HashInd = pstInCAPKList->HashInd;
    tempcapk->ArithInd = pstInCAPKList->ArithInd;
    tempcapk->ModulLen = pstInCAPKList->ModulLen;

    if(tempcapk->ModulLen > sizeof(tempcapk->Modul))
    {
        tempcapk->ModulLen = sizeof(tempcapk->Modul);
    }
    memset(tempcapk->Modul, 0, sizeof(tempcapk->Modul));
    memcpy(tempcapk->Modul, (u8 *)(pstInCAPKList->Modul), tempcapk->ModulLen);
    tempcapk->ExponentLen = pstInCAPKList->ExponentLen;

    if(tempcapk->ExponentLen > sizeof(tempcapk->Exponent))
    {
        tempcapk->ExponentLen = sizeof(tempcapk->Exponent);
    }
    memset(tempcapk->Exponent, 0, sizeof(tempcapk->Exponent));
    memcpy(tempcapk->Exponent, (u8 *)(pstInCAPKList->Exponent), tempcapk->ExponentLen);
    tempcapk->CheckSumLen = pstInCAPKList->CheckSumLen;

    if(tempcapk->CheckSumLen > sizeof(tempcapk->CheckSum))
    {
        tempcapk->CheckSumLen = sizeof(tempcapk->CheckSum);
    }
    memset(tempcapk->CheckSum, 0, sizeof(tempcapk->CheckSum));
    memcpy(tempcapk->CheckSum, (u8 *)(pstInCAPKList->CheckSum), tempcapk->CheckSumLen);
    memcpy(tempcapk->ExpireDate, (u8 *)(pstInCAPKList->ExpireDate), sizeof(tempcapk->ExpireDate));
}

static void emvbase_CAPK_TO_SDK_CAPK(SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList, const EMVBASE_CAPK_STRUCT *tempcapk)
{
    memcpy(pstInCAPKList->RID, (u8 *)(tempcapk->RID), 5);
    pstInCAPKList->CAPKI = tempcapk->CAPKI;
    pstInCAPKList->HashInd = tempcapk->HashInd;
    pstInCAPKList->ArithInd = tempcapk->ArithInd;
    pstInCAPKList->ModulLen = tempcapk->ModulLen;

    if(pstInCAPKList->ModulLen > sizeof(pstInCAPKList->Modul))
    {
        pstInCAPKList->ModulLen = sizeof(pstInCAPKList->Modul);
    }
    memset(pstInCAPKList->Modul, 0, sizeof(pstInCAPKList->Modul));
    memcpy(pstInCAPKList->Modul, (u8 *)(tempcapk->Modul), pstInCAPKList->ModulLen);
    pstInCAPKList->ExponentLen = tempcapk->ExponentLen;

    if(pstInCAPKList->ExponentLen > sizeof(pstInCAPKList->Exponent))
    {
        pstInCAPKList->ExponentLen = sizeof(pstInCAPKList->Exponent);
    }
    memset(pstInCAPKList->Exponent, 0, sizeof(pstInCAPKList->Exponent));
    memcpy(pstInCAPKList->Exponent, (u8 *)(tempcapk->Exponent), pstInCAPKList->ExponentLen);
    pstInCAPKList->CheckSumLen = tempcapk->CheckSumLen;

    if(pstInCAPKList->CheckSumLen > sizeof(pstInCAPKList->CheckSum))
    {
        pstInCAPKList->CheckSumLen = sizeof(pstInCAPKList->CheckSum);
    }
    memset(pstInCAPKList->CheckSum, 0, sizeof(pstInCAPKList->CheckSum));
    memcpy(pstInCAPKList->CheckSum, (u8 *)(tempcapk->CheckSum), pstInCAPKList->CheckSumLen);
    memcpy(pstInCAPKList->ExpireDate, (u8 *)(tempcapk->ExpireDate), sizeof(pstInCAPKList->ExpireDate));
}

static bool emvbase_CAPK_IS_EQUAL(const SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList, const EMVBASE_CAPK_STRUCT *tempcapk)
{
    if( !memcmp(pstInCAPKList->RID, tempcapk->RID, 5) &&
        pstInCAPKList->CAPKI == tempcapk->CAPKI &&
        pstInCAPKList->HashInd == tempcapk->HashInd &&
        pstInCAPKList->ArithInd == tempcapk->ArithInd &&
        pstInCAPKList->ModulLen == tempcapk->ModulLen &&
        !memcmp(pstInCAPKList->Modul, tempcapk->Modul, pstInCAPKList->ModulLen) &&
        pstInCAPKList->ExponentLen == tempcapk->ExponentLen &&
        !memcmp(pstInCAPKList->Exponent, tempcapk->Exponent, pstInCAPKList->ExponentLen) &&
        pstInCAPKList->CheckSumLen == tempcapk->CheckSumLen &&
        !memcmp(pstInCAPKList->CheckSum, tempcapk->CheckSum, pstInCAPKList->CheckSumLen) &&
        !memcmp(pstInCAPKList->ExpireDate, tempcapk->ExpireDate, sizeof(pstInCAPKList->ExpireDate))
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}


s32 sdkEMVBaseAddAnyCAPKList(const SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList, s32 siCAPKNum)
{
    u8 i, j;
    s32 ret = SDK_OK, ret1;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    u8 *tmp;
    u8 iffind;
    s32 addr;
    s32 tmplen;
    //u8 checksum;


    if(pstInCAPKList == NULL || siCAPKNum > EMVBASEMAXCAPKINDEX || siCAPKNum <= 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);


    tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);

    for(j = 0; j < siCAPKNum; j++)
    {
//        if(sdkEMVBaseVerifyCAPK((pstInCAPKList + j)) != SDK_OK)
//        {
//            emvbase_free(tmp);
//            return SDK_ERR;
//        }
        iffind = 0;

        for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
        {
            if((gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (gEmvbaseCapkIndex[i].CAPKI == (pstInCAPKList + j)->CAPKI) &&
               (!memcmp(gEmvbaseCapkIndex[i].RID, (pstInCAPKList + j)->RID, 5)))
            {
                iffind = 1;
                addr = ((u32)i) << EMVBASECAPKSHIFTNUM;
                tmplen = EMVBASECAPKLEN;
//				addr = i * sizeof(SDK_EMVBASE_CAPK_STRUCT);
                ret1 = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret1 != SDK_OK)
                {
                    break;
                }

                if( emvbase_CAPK_IS_EQUAL(pstInCAPKList + j, (EMVBASE_CAPK_STRUCT*)tmp) )
                {
                    iffind = 2;
                    break;
                }

                break;
            }
        }

        if(iffind == 0)
        {
            for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
            {
                if(gEmvbaseCapkIndex[i].ifusedflag != EMVBASECAPKENABLE)
                {
                    break;
                }
            }
        }
        else if(iffind == 2)
        {
            continue;
        }

        if(i >= EMVBASEMAXCAPKINDEX)
        {
            ret = SDK_ICC_OVER_LIMIT;
            break;
        }

        memset(tmp, 0xff, EMVBASECAPKLEN);
        emvbase_SDK_CAPK_TO_CAPK( (EMVBASE_CAPK_STRUCT *)tmp, (pstInCAPKList + j));
        tmp[EMVBASECAPKFLAGOFFSET] = EMVBASECAPKENABLE;
        addr = i;
        addr = addr << EMVBASECAPKSHIFTNUM;
//		addr = i * sizeof(SDK_EMVBASE_CAPK_STRUCT);
        ret1 = sdkInsertFile(fn, tmp, addr, EMVBASECAPKLEN);

        if(ret1 == SDK_OK)
        {
            memcpy(gEmvbaseCapkIndex[i].RID, (u8 *)((pstInCAPKList + j)->RID), 5);
            gEmvbaseCapkIndex[i].CAPKI = (pstInCAPKList + j)->CAPKI;
            gEmvbaseCapkIndex[i].ifusedflag = EMVBASECAPKENABLE;
        }
        else
        {
            ret = SDK_ERR;
            break;
        }
    }

    emvbase_free(tmp);

    return ret;
}


s32 sdkEMVBaseAddAnyGMCAPKList(const SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList, s32 siCAPKNum)
{
    u8 i, j;
    s32 ret = SDK_OK, ret1;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    u8 *tmp;
    u8 iffind;
    s32 addr;
    s32 tmplen;
    u8 checksum;


    if(pstInCAPKList == NULL || siCAPKNum > EMVBASEMAXGMCAPKINDEX || siCAPKNum <= 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);


    tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);

    for(j = 0; j < siCAPKNum; j++)
    {
        iffind = 0;

        for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
        {
            if((gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (gEmvbaseGMCapkIndex[i].CAPKI == (pstInCAPKList + j)->CAPKI) &&
               (!memcmp(gEmvbaseGMCapkIndex[i].RID, (pstInCAPKList + j)->RID, 5)))
            {
                iffind = 1;
                addr = ((u32)i) << EMVBASECAPKSHIFTNUM;
//				addr = i * sizeof(EMVBASE_CAPK_STRUCT);
                tmplen = EMVBASECAPKLEN;
                ret1 = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret1 != SDK_OK)
                {
                    break;
                }
				#if 0
                checksum = sdkCalcCrc16(tmp, CAPKLEN - 1);

                if(checksum == tmp[CAPKLEN - 1])
				#endif
                {
                    if( emvbase_CAPK_IS_EQUAL(pstInCAPKList + j, (EMVBASE_CAPK_STRUCT*)tmp) )
                    {
                        iffind = 2;
                        break;
                    }
                }
                break;
            }
        }

        if(iffind == 0)
        {
            for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
            {
                if(gEmvbaseGMCapkIndex[i].ifusedflag != EMVBASECAPKENABLE)
                {
                    break;
                }
            }
        }
        else if(iffind == 2)
        {
            continue;
        }

        if(i >= EMVBASEMAXGMCAPKINDEX)
        {
            ret = SDK_ICC_OVER_LIMIT;
            break;
        }

        memset(tmp, 0xff, EMVBASECAPKLEN);
        emvbase_SDK_CAPK_TO_CAPK( (EMVBASE_CAPK_STRUCT *)tmp, (pstInCAPKList + j));
        tmp[EMVBASECAPKFLAGOFFSET] = EMVBASECAPKENABLE;
        addr = i;
        addr = addr << EMVBASECAPKSHIFTNUM;
//		addr = i * sizeof(EMVBASE_CAPK_STRUCT);
        ret1 = sdkInsertFile(fn, tmp, addr, EMVBASECAPKLEN);

        if(ret1 == SDK_OK)
        {
            memcpy(gEmvbaseGMCapkIndex[i].RID, (u8 *)((pstInCAPKList + j)->RID), 5);
            gEmvbaseGMCapkIndex[i].CAPKI = (pstInCAPKList + j)->CAPKI;
            gEmvbaseGMCapkIndex[i].ifusedflag = EMVBASECAPKENABLE;
        }
        else
        {
            ret = SDK_ERR;
            break;
        }
    }

    emvbase_free(tmp);

    return ret;
}


s32 sdkEMVBaseGetAnyCAPKList(s32 siStartIndex, s32 siCAPKNum, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPKList, s32 *psiOutCAPKNum)
{
    u32 i;
    u8 j;
    s32 ret;
    s32 addr, tmplen;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    u8 *tmp;

    if(psiOutCAPKNum == NULL || pstOutCAPKList == NULL)
    {
        return SDK_PARA_ERR;
    }
    *psiOutCAPKNum = 0;

    if(siCAPKNum == 0)
    {
        return SDK_OK;
    }
    memset(fn, 0, SDK_EMVBASE_MAX_PATH);

    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

    j = 0;
    tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);

    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if(gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            if(j >= siStartIndex)
            {
                addr = ((u32)i) << EMVBASECAPKSHIFTNUM;
//				addr = i * sizeof(EMVBASE_CAPK_STRUCT);
                tmplen = EMVBASECAPKLEN;
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret != SDK_OK)
                {
                    sdkEMVBaseLoadCAPKList();
                    break;
                }
                emvbase_CAPK_TO_SDK_CAPK((SDK_EMVBASE_CAPK_STRUCT *)&((pstOutCAPKList + j - siStartIndex)->RID), (EMVBASE_CAPK_STRUCT*)tmp);

                j++;

                if(siStartIndex + siCAPKNum == j)
                {
                    break;
                }
            }
            else
            {
                j++;
            }
        }
    }


    if(j >= siStartIndex)
    {
        *psiOutCAPKNum = j - siStartIndex;
    }
    emvbase_free(tmp);

    return SDK_OK;
}


s32 sdkEMVBaseGetAnyGMCAPKList(s32 siStartIndex, s32 siCAPKNum, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPKList, s32 *psiOutCAPKNum)
{
    u32 i;
    u8 j;
    s32 ret;
    s32 addr, tmplen;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    u8 *tmp;

    if(psiOutCAPKNum == NULL || pstOutCAPKList == NULL)
    {
        return SDK_PARA_ERR;
    }
    *psiOutCAPKNum = 0;

    if(siCAPKNum == 0)
    {
        return SDK_OK;
    }
    memset(fn, 0, SDK_EMVBASE_MAX_PATH);

    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);

    j = 0;
    tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);

    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
    {
        if(gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            if(j >= siStartIndex)
            {
                addr = ((u32)i) << EMVBASECAPKSHIFTNUM;
//				addr = i * sizeof(EMVBASE_CAPK_STRUCT);
                tmplen = EMVBASECAPKLEN;
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if(ret != SDK_OK)
                {
                    sdkEMVBaseLoadCAPKList();
                    break;
                }
                emvbase_CAPK_TO_SDK_CAPK((SDK_EMVBASE_CAPK_STRUCT *)&((pstOutCAPKList + j - siStartIndex)->RID), (EMVBASE_CAPK_STRUCT*)tmp);

                j++;

                if(siStartIndex + siCAPKNum == j)
                {
                    break;
                }
            }
            else
            {
                j++;
            }
        }
    }


    if(j >= siStartIndex)
    {
        *psiOutCAPKNum = j - siStartIndex;
    }
    emvbase_free(tmp);

    return SDK_OK;
}


s32 sdkEMVBaseGetCAPKListNum(s32 *psiInCAPKListNum)
{
    u8 i, j = 0;

    if(psiInCAPKListNum == NULL)
    {
        return SDK_PARA_ERR;
    }

    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if(gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            j++;
        }
    }

    *psiInCAPKListNum = j;

    return SDK_OK;
}

s32 sdkEMVBaseGetGMCAPKListNum(s32 *psiInCAPKListNum)
{
    u8 i, j = 0;

    if(psiInCAPKListNum == NULL)
    {
        return SDK_PARA_ERR;
    }

    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
    {
        if(gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE)
        {
            j++;
        }
    }

    *psiInCAPKListNum = j;

    return SDK_OK;
}


s32 sdkEMVBaseGetMatchCAPK(const u8 *pheInRID, s32 siCAPKIndex, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPK)
{
    u8 i;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 addr;
    s32 tmplen;
    s32 ret;
    u8 *tmp;

    if(pheInRID == NULL || pstOutCAPK == NULL || siCAPKIndex < 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if((gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (!memcmp(pheInRID, gEmvbaseCapkIndex[i].RID, 5))
           && (siCAPKIndex == gEmvbaseCapkIndex[i].CAPKI))
        {
            tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);
            addr = ((u32)i) << EMVBASECAPKSHIFTNUM;
//			addr = i * sizeof(EMVBASE_CAPK_STRUCT);
            tmplen = EMVBASECAPKLEN;
            ret = sdkReadFile(fn, tmp, addr, &tmplen);

            if(ret == SDK_OK)
            {
				emvbase_CAPK_TO_SDK_CAPK(pstOutCAPK, (EMVBASE_CAPK_STRUCT*)tmp);
                ret = SDK_OK;
            }
            else
            {
                ret = SDK_ERR;
            }
            emvbase_free(tmp);
            return ret;
        }
    }

    return SDK_ICC_NODATA;
}

s32 sdkEMVBaseGetMatchGMCAPK(const u8 *pheInRID, s32 siCAPKIndex, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPK)
{
    u8 i;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 addr;
    s32 tmplen;
    s32 ret;
    u8 *tmp;

    if(pheInRID == NULL || pstOutCAPK == NULL || siCAPKIndex < 0)
    {
        return SDK_PARA_ERR;
    }

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);


    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
    {
        if((gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (!memcmp(pheInRID, gEmvbaseGMCapkIndex[i].RID, 5))
           && (siCAPKIndex == gEmvbaseGMCapkIndex[i].CAPKI))
        {
            tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);
            addr = ((u32)i) << EMVBASECAPKSHIFTNUM;
            tmplen = EMVBASECAPKLEN;
            ret = sdkReadFile(fn, tmp, addr, &tmplen);

            if(ret == SDK_OK)
            {
				emvbase_CAPK_TO_SDK_CAPK(pstOutCAPK, (EMVBASE_CAPK_STRUCT*)tmp);
                ret = SDK_OK;
            }
            else
            {
                ret = SDK_ERR;
            }
            emvbase_free(tmp);
            return ret;
        }
    }

    return SDK_ICC_NODATA;
}

static void emvbase_manage_capkindexread(const u8 *capkfiledir)
{
    s32 ret;
    u8 i;
    u32 tmpaddr;
    u32 readlen;
    u8 *tmp;
    u32 addr;
    u32 filelen;

    memset(&gEmvbaseCapkIndex[0], 0xFF, EMVBASEMAXCAPKINDEX * sizeof(EMVBASECAPKDATAINDEX));

    if(sdkGetFileSize(capkfiledir) != (u32)EMVBASEMAXCAPKINDEX * EMVBASECAPKLEN)
    {
        ret = emvbase_kern_create_file(capkfiledir, (u32)EMVBASECAPKLEN,(u32)EMVBASEMAXCAPKINDEX, 0xff);

        if(ret != 0)
        {
            return;
        }
    }
    filelen = (u32)EMVBASEMAXCAPKINDEX * EMVBASECAPKLEN;
    addr = 0;

    tmp = (u8 *)emvbase_malloc(EMVBASEEMVBlockLen);


    for(i = 0; i < EMVBASEMAXCAPKINDEX; )
    {
        if((addr + EMVBASEEMVBlockLen) <= filelen)
        {
            readlen = EMVBASEEMVBlockLen;
        }
        else
        {
            readlen = filelen - addr;
        }
        ret = sdkReadFile(capkfiledir, tmp, addr, &readlen);

        if(ret != SDK_OK)
        {
            emvbase_free(tmp);
            return;
        }


        for(tmpaddr = 0; tmpaddr < readlen; )
        {
            memcpy(&gEmvbaseCapkIndex[i].RID, &tmp[tmpaddr], 7);
            gEmvbaseCapkIndex[i].ifusedflag = tmp[EMVBASECAPKFLAGOFFSET + tmpaddr];

            i++;

            if(i >= EMVBASEMAXCAPKINDEX)
            {
                break;
            }
            tmpaddr += EMVBASECAPKLEN;
        }

        addr += readlen;
    }

    emvbase_free(tmp);
}

s32 sdkEMVBaseCreateCAPKFile(void)
{
    u8 fn[SDK_EMVBASE_MAX_PATH];
	s32 ret;

    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

	ret = emvbase_kern_create_file(fn, (u32)EMVBASECAPKLEN,(u32)EMVBASEMAXCAPKINDEX, 0xff);
	return ret;
}

s32 sdkEMVBaseLoadCAPKList(void)
{
    u8 fn[SDK_EMVBASE_MAX_PATH];
	s32 ret;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

	ret = sdkGetFileSize(fn);
	Trace("emv", "Get CAPK File ret = %d\r\n", ret);
	if(ret == 0 || ret == SDK_FUN_NULL)
	{
		sdkEMVBaseCreateCAPKFile();
	}

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

    emvbase_manage_capkindexread(fn);

	return SDK_OK;
}

static void emvbase_manage_gmcapkindexread(const u8 *capkfiledir)
{
    s32 ret;
    u8 i;
    u32 tmpaddr;
    u32 readlen;
    u8 *tmp;
    u32 addr;
    u32 filelen;


    memset(&gEmvbaseGMCapkIndex[0], 0xFF, EMVBASEMAXGMCAPKINDEX * sizeof(EMVBASECAPKDATAINDEX));

    if(sdkGetFileSize(capkfiledir) != (u32)EMVBASEMAXGMCAPKINDEX * EMVBASECAPKLEN)
    {
        ret = emvbase_kern_create_file(capkfiledir, (u32)EMVBASECAPKLEN,(u32)EMVBASEMAXGMCAPKINDEX, 0xff);

        if(ret != 0)
        {
            return;
        }
    }

    filelen = (u32)EMVBASEMAXGMCAPKINDEX * EMVBASECAPKLEN;
    addr = 0;

    tmp = (u8 *)emvbase_malloc(EMVBASEEMVBlockLen);


    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; )
    {
        if((addr + EMVBASEEMVBlockLen) <= filelen)
        {
            readlen = EMVBASEEMVBlockLen;
        }
        else
        {
            readlen = filelen - addr;
        }
        ret = sdkReadFile(capkfiledir, tmp, addr, &readlen);

        if(ret != SDK_OK)
        {
            emvbase_free(tmp);
            return;
        }


        for(tmpaddr = 0; tmpaddr < readlen; )
        {
            memcpy(&gEmvbaseGMCapkIndex[i].RID, &tmp[tmpaddr], 7);
            gEmvbaseGMCapkIndex[i].ifusedflag = tmp[EMVBASECAPKFLAGOFFSET + tmpaddr];
            i++;

            if(i >= EMVBASEMAXGMCAPKINDEX)
            {
                break;
            }
            tmpaddr += EMVBASECAPKLEN;
        }

        addr += readlen;
    }

    emvbase_free(tmp);
}


s32 sdkEMVBaseLoadGMCAPKList(void)
{
    u8 fn[SDK_EMVBASE_MAX_PATH];

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);

    emvbase_manage_gmcapkindexread(fn);

	return SDK_OK;
}


static void *sdkEmvBaseMalloc(u32 nSize)
{
    return (void *)sdkGetMem(nSize);
}

static void sdkEmvBaseFree(void *ap)
{
    sdkFreeMem(ap);
}

static void sdkEmvBaseInitEMV()
{
    EMVBaseallocator mallocEmv;

    mallocEmv.EMVBallocator_free = (emvbase_core_free)sdkEmvBaseFree;
    mallocEmv.EMVBallocator_malloc = (emvbase_core_malloc)sdkEmvBaseMalloc;

    emvbase_allocator_init(&mallocEmv);
}

s32 sdkEMVBasePowerStartInit(void)
{
    sdkEmvBaseInitEMV();
	sdkEMVBaseLoadAIDList();
	Trace("emv","goto sdkEMVBaseLoadCAPKList\r\n");
    sdkEMVBaseLoadCAPKList();
	Trace("emv","goto sdkEMVBaseLoadGMCAPKList\r\n");
    return SDK_OK;
}

s32 sdkEMVBaseDelTLV(const u8 *pheTag)
{
    u32 tag_len;
    u8 tag[4] = {0};

    if(NULL == pheTag || pheTag[0] == 0)
    {
        return SDK_PARA_ERR;
    }
    tag_len = 1;

    if((pheTag[0] & 0x1F) == 0x1F)
    {
        tag_len = 2;
		if((pheTag[1] & 0x80))
	    {
	        tag_len = 3;
	    }
    }
    memset(tag, 0, sizeof(tag));
    memcpy(tag, (u8 *)pheTag, tag_len);

   	emvbase_avl_deletetag(tag);

    return SDK_OK;
}


s32 sdkEMVBaseConfigTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen)
{
    u32 tag_len;
    u8 tag[4] = {0};

    if(NULL == pheTag || pheTag[0] == 0 || pheInData == NULL || siInLen < 0)
    {
        return SDK_PARA_ERR;
    }

//	TraceHex("emvbase", "want config tag", pheTag, 2);
    tag_len = 1;

	if(pheTag[0] < 0x42)
	{
		if(0x1F == pheTag[0] && 0x02 == pheTag[1])
		{
			//
		}
		else
		{
			return SDK_PARA_ERR;
		}
	}

    if((pheTag[0] & 0x1F) == 0x1F)
    {
        tag_len = 2;
		if((pheTag[1] & 0x80))
	    {
	        tag_len = 3;
	    }
    }
	if(strlen(pheTag) < tag_len)
	{
		return SDK_PARA_ERR;
	}
    memset(tag, 0, sizeof(tag));
    memcpy(tag, (u8 *)pheTag, tag_len);

	if(tag[0] == 0x71 || tag[0] == 0x72)
	{
		u8 data[256]={0};
		u16 datalen=0;

		emvbase_avl_gettagvalue_all(tag, data, &datalen);

		if(tag[0] != pheInData[0])//Ӧ����config(71, 0F, 860D8424000008BFAE4615A557630E)���ýű�
		{
			data[datalen++] = tag[0];
			data[datalen++] = siInLen;
		}
		memcpy(data+datalen, pheInData, siInLen);
		datalen += siInLen;

		if(emvbase_avl_createsettagvalue(tag, (u8 *)data, datalen))
		{
			return SDK_ERR;
		}
		return SDK_OK;

	}
	if(emvbase_avl_createsettagvalue(tag, (u8 *)pheInData, siInLen))
    {
        return SDK_ERR;
    }

    return SDK_OK;
}


s32 sdkEMVBaseReadTLV(const u8 * pheTag, u8 * pheOutData, s32 * psiOutLen)
{
    u8 tag[4];
    u8 taglen = 0;
    u16 templen;


    if((NULL == pheTag) || (NULL == pheOutData) || (NULL == psiOutLen)
       || (0 == pheTag[0]))
    {
        return SDK_PARA_ERR;
    }
    memset(tag, 0, sizeof(tag));
    taglen = 1;

    taglen = 1;

    if((pheTag[0] & 0x1F) == 0x1F)
    {
        taglen = 2;
		if((pheTag[1] & 0x80))
	    {
	        taglen = 3;
	    }
    }

	if(strlen(pheTag) < taglen)
	{
		return SDK_PARA_ERR;
	}

    memcpy(tag, (u8 *)pheTag, taglen);
    *psiOutLen = 0;

    emvbase_avl_gettagvalue_all(tag, pheOutData, &templen);

    if(templen)
    {
        *psiOutLen = templen;
        return SDK_OK;
    }
    return SDK_ERR;
}


static const s32 giSdkEmvBase2SdkRlt[][2] = {
    {RLT_EMV_OK, SDK_OK},
    {RLT_EMV_ERR, SDK_ERR},
    {RLT_EMV_OFFLINE_APPROVE, SDK_EMV_TransOfflineApprove},
    {RLT_EMV_OFFLINE_DECLINE, SDK_EMV_TransOfflineDecline},
    {RLT_EMV_ONLINE_APPROVE, SDK_EMV_TransOnlineApprove},
    {RLT_EMV_ONLINE_DECLINE, SDK_EMV_TransOnlineDecline},
    {RLT_EMV_ONLINE_WAIT, SDK_EMV_TransOnlineWait},
    {RLT_ERR_EMV_IccDataFormat, SDK_EMV_IccDataFormatErr},
    {RLT_ERR_EMV_IccCommand, SDK_EMV_IccCommandErr},
    {RLT_ERR_EMV_IccReturn, SDK_EMV_IccReturnErr},
    {RLT_ERR_EMV_NoAppSel, SDK_EMV_NoAppSel},
    {RLT_ERR_EMV_TransRestart, SDK_EMV_TransTryAgain},
    {RLT_ERR_EMV_IccDataRedund, SDK_EMV_IccDataRedund},
    {RLT_ERR_EMV_NotAccept, SDK_EMV_NotAccept},
    {RLT_ERR_EMV_CancelTrans, SDK_EMV_CancelTrans},
    {RLT_ERR_EMV_CardBlock, SDK_EMV_CardBlock},
    {RLT_ERR_EMV_APDUTIMEOUT, SDK_EMV_AppTimeOut},
    {RLT_ERR_EMV_NEEDMAGCARD, SDK_EMV_NeedMagCard},
    {RLT_ERR_EMV_APPBLOCK, SDK_EMV_AppBlock},
    {RLT_EMV_USERSELECT, SDK_EMV_UserSelect},
    {RLT_ERR_INITAPP_NOTACCEPED, SDK_EMV_InitAppNotAccept},
    {RLT_EMV_APPSELECTTRYAGAIN, SDK_EMV_AppSelectTryAgain},
	{RLT_ERR_EMV_SEEPHONE, SDK_EMV_SeePhone},
	{RLT_ERR_EMV_SWITCHINTERFACE, SDK_EMV_SwitchInterface},
	{RLT_EMV_TERMINATE_TRANSERR, SDK_EMV_TransTerminate},
	{RLT_EMV_PPSE_REV_6A82, SDK_EMV_PPSERev6A82},
	{RLT_EMV_TORN,SDK_EMV_TORN},
	{RLT_ERR_EMV_OTHERAPCARD, SDK_EMV_UseOtherCard},
};

s32 sdkEMVBaseRltToSdkRlt(u8 ucInBuf, s32 *puiOutBuf)
{
    u32 i;
    u32 num;

    if(puiOutBuf == NULL)
    {
        return SDK_PARA_ERR;
    }
    num = sizeof(giSdkEmvBase2SdkRlt) / sizeof(giSdkEmvBase2SdkRlt[0]);

    for(i = 0; i < num; i++)
    {
        if((s32)ucInBuf == giSdkEmvBase2SdkRlt[i][0])
        {
            *puiOutBuf = giSdkEmvBase2SdkRlt[i][1];
            return SDK_OK;
        }
    }

    *puiOutBuf = SDK_ERR;
    return SDK_EQU;
}

void sdkEMVBase_GetRandomNum(unsigned char* randpad, unsigned int randLen)
{
    sdkGetRandom(randpad, randLen);
}

void sdkEMVBaseReadSN(u8 * SN)
{
    u8 i;
    u8 temp[64]={0};
	u16 sn_len;

    sn_len = sdkReadPosSn(temp);

    if(sn_len < 8)
    {
        memcpy(SN, temp, i);
        memcpy(SN + i, "\x20\x20\x20\x20\x20\x20\x20\x20", 8 - i);
    }
    else
    {
        memcpy(SN, temp, 8);
    }
}


s32 sdkEMVBase_ReadCAPKFile(u32 id, EMVBASE_CAPK_STRUCT * tmpcapk)
{
    u32 addr;
    s32 ret;
    u8 *tmp;
    //u8 checksum;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 readfilelen;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_CAPK_FILENALE);

    tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);

    addr = id << EMVBASECAPKSHIFTNUM;
//	addr = id * sizeof(EMVBASE_CAPK_STRUCT);
    readfilelen = EMVBASECAPKLEN;
    ret = sdkReadFile(fn, tmp, addr, &readfilelen);
	Trace("File", "sdkReadFile ret = %d\r\n", ret);
    if(ret != SDK_OK)
    {
        emvbase_free(tmp);
        return 1;
    }
    {
        if(tmp[EMVBASECAPKFLAGOFFSET] == EMVBASECAPKENABLE)
        {
            memcpy((u8 *)tmpcapk, tmp, sizeof(EMVBASE_CAPK_STRUCT));
            emvbase_free(tmp);
            return 0;
        }
    }
    emvbase_free(tmp);
    return 1;
}

bool sdkEMVBaseCheckCAPKExit(u8 *RID, u8 capki)
{
    u16 i;

	TraceHex("emv", "sdkEMVBaseCheckCAPKExit RID:", RID, 5);
	Trace("emv", "capkI:%02X\r\n",capki);

    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if((gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (memcmp(RID, gEmvbaseCapkIndex[i].RID, 5) == 0) && (capki == gEmvbaseCapkIndex[i].CAPKI))
        {
	        return true;
        }
    }
	return false;

}

void sdkEMVBase_ReadCAPK(u8 *RID, u8 capki, EMVBASE_CAPK_STRUCT * tmpcapk)
{
    u16 i;

	TraceHex("emv", "sdkEMVBase_ReadCAPK RID:", RID, 5);
	Trace("emv", "capkI:%02X\r\n",capki);

    for(i = 0; i < EMVBASEMAXCAPKINDEX; i++)
    {
        if((gEmvbaseCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (memcmp(RID, gEmvbaseCapkIndex[i].RID, 5) == 0) && (capki == gEmvbaseCapkIndex[i].CAPKI))
        {
	        sdkEMVBase_ReadCAPKFile(i, tmpcapk);
			TraceHex("emv", "capk Modul",tmpcapk->Modul,tmpcapk->ModulLen);
	        return;
        }
    }

}

s32 sdkEMVBase_ReadGMCAPKFile(u32 index, EMVBASE_CAPK_STRUCT * tmpcapk)
{
    u32 addr;
    s32 ret;
    u8 *tmp;
    ///u8 checksum;
    u8 fn[SDK_EMVBASE_MAX_PATH];
    s32 readfilelen;

    memset(fn, 0, SDK_EMVBASE_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, SDK_EMVBASE_GMCAPK_FILENALE);

    tmp = (u8 *)emvbase_malloc(EMVBASECAPKLEN);

    addr = index << EMVBASECAPKSHIFTNUM;
    readfilelen = EMVBASECAPKLEN;


    ret = sdkReadFile(fn, tmp, addr, &readfilelen);

    if(ret != SDK_OK)
    {
        emvbase_free(tmp);
        return 1;
    }

    if(tmp[EMVBASECAPKFLAGOFFSET] == EMVBASECAPKENABLE)
    {
        memcpy((u8 *)tmpcapk, tmp, sizeof(EMVBASE_CAPK_STRUCT));
        emvbase_free(tmp);
        return 0;
    }

    emvbase_free(tmp);
    return 1;
}


void sdkEMVBase_ReadGMCAPK(u8 *RID, u8 capki, EMVBASE_CAPK_STRUCT * tmpcapk)
{
    u16 i;

	TraceHex("emv", "sdkEMVBase_ReadGMCAPK RID:", RID, 5);
	Trace("emv", "capki:%02X\r\n",capki);

    for(i = 0; i < EMVBASEMAXGMCAPKINDEX; i++)
    {
        if((gEmvbaseGMCapkIndex[i].ifusedflag == EMVBASECAPKENABLE) && (memcmp(RID, gEmvbaseGMCapkIndex[i].RID, 5) == 0) && (capki == gEmvbaseGMCapkIndex[i].CAPKI))
        {
	        sdkEMVBase_ReadGMCAPKFile(i, tmpcapk);
			TraceHex("emv", "GMcapk Modul",tmpcapk->Modul,tmpcapk->ModulLen);
	        return;
        }
    }

}

void sdkEMVBaseCreateUnpredictNum(void)
{
    u8 UnpredictNum[4];

    sdkGetRandom(UnpredictNum, 4);
    emvbase_avl_createsettagvalue(EMVTAG_UnpredictNum, UnpredictNum, 4);
}

void EMVBase_TransTermData_Init(void)
{
	u8 strBuf[8], transTime[8];

	emvbase_avl_createsettagvalue(EMVTAG_CVMResult, "\x3F\x00\x00", 3);
	sdkEMVBaseCreateUnpredictNum();

	memset(transTime, 0, sizeof(transTime));
	sdkGetRtc(transTime);
	emvbase_avl_createsettagvalue(EMVTAG_TransDate, transTime, 3);
	emvbase_avl_createsettagvalue(EMVTAG_TransTime, &transTime[3], 3);
	emvbase_avl_settag(EMVTAG_TermType, 0x22);
	emvbase_avl_settag(EMVTAG_TransTypeValue, 0);
	emvbase_avl_settag(EMVTAG_POSEntryMode, 0x07);
	emvbase_avl_createsettagvalue(EMVTAG_TVR, "\x00\x00\x00\x00\x00", 5);
	emvbase_avl_createsettagvalue(EMVTAG_TSI, "\x00\x00", 2);
	emvbase_avl_createsettagvalue(EMVTAG_AppVerNum, "\x00\x30", 2);
	emvbase_avl_createsettagvalue(EMVTAG_CountryCode, "\x01\x56", 2);
	emvbase_avl_createsettagvalue(EMVTAG_TransCurcyCode, "\x01\x56", 2);
	emvbase_avl_createsettagvalue(EMVTAG_TransReferCurcyCode, "\x01\x56", 2);
	emvbase_avl_settag(EMVTAG_TransReferCurcyExp, 0x02);
	emvbase_avl_settag(EMVTAG_TransCurcyExp, 0x02);
	emvbase_avl_createsettagvalue(EMVTAG_AcquireID, "\x00\x00\x00\x00\x00\x01", 6);
	emvbase_avl_createsettagvalue(EMVTAG_MerchCateCode, "\x00\x01", 2);
	emvbase_avl_createsettagvalue(EMVTAG_TermCapab, "\xE0\xF8\xC8", 3);
	emvbase_avl_createsettagvalue(EMVTAG_TermAddCapab, "\xFF\x80\xF0\xA0\x01", 5);
	emvbase_avl_createsettagvalue(EMVTAG_TermDDOL, "\x9F\x37\x04", 3);
	emvbase_avl_createsettagvalue(EMVTAG_TermTDOL, "\x9F\x02\x06\x5F\x2A\x02\x9A\x03\x9C\x01\x95\x05\x9F\x37\x04", 15);
	emvbase_avl_createsettagvalue(EMVTAG_TermTransPredicable, "\x36\x00\x00\x80", 4);
	emvbase_avl_createsettagvalue(EMVTAG_AmtAuthBin, "\x00\x00\x00\x00", 4);
	emvbase_avl_createsettagvalue(EMVTAG_AmtOtherBin, "\x00\x00\x00\x00", 4);
	emvbase_avl_createsettagvalue(EMVTAG_AmtAuthNum, "\x00\x00\x00\x00\x00\x00", 6);
	emvbase_avl_createsettagvalue(EMVTAG_AmtOtherNum, "\x00\x00\x00\x00\x00\x00", 6);

	memset(strBuf, 0, 8);
	sdkEMVBaseReadSN(strBuf);
	emvbase_avl_createsettagvalue(EMVTAG_IFD_SN, strBuf, 8);
}


s32 sdkEMVBaseTransInit()
{
	gstemvbaseneedsign = 0;
	gstemvbaseCVMresult=SDKEMVBASE_CVM_NA;
	emvbase_avl_createtree();
	EMVBase_TransTermData_Init();
	if(gstEMVBase_TradeParam)
	{
		emvbase_free(gstEMVBase_TradeParam);
		gstEMVBase_TradeParam = NULL;
	}
	gstEMVBase_TradeParam = emvbase_malloc(sizeof(SDK_EMVBASE_TRADE_PARAM));
	memset(gstEMVBase_TradeParam,0,(sizeof(SDK_EMVBASE_TRADE_PARAM)));
	//gstEMVBase_TradeParam->tradetype = SDKEMVBASE_TRANS_GOODSSERVICE;

	return SDK_OK;
}


s32 sdkEMVBaseSetTransAmount(u8 *bcdAmount)
{
	u8 tmpAmtAuthBin[4] = {0};
	u32 tmpsdkAmtAuthBin = 0;

	sdkEMVBaseConfigTLV("\x9F\x02", (const u8 *)bcdAmount, 6);
    sdkBcdToU32(&tmpsdkAmtAuthBin, (const u8 *)bcdAmount, 6);
	if(tmpsdkAmtAuthBin > 0xFFFFFFFF)
	{
		memcpy(tmpAmtAuthBin,"\xFF\xFF\xFF\xFF",4);
	}
	else
	{
	    sdkU32ToHex(tmpAmtAuthBin,tmpsdkAmtAuthBin, 4);
	}
	sdkEMVBaseConfigTLV("\x81", tmpAmtAuthBin, 4);
	return SDK_OK;
}


s32 sdkEMVBaseSetTwoTransAmount(u8 *bcdAmount, u8 *bcdOtherAmount)
{
	u8 tmpAmtAuthBin[4] = {0};
	u32 uisdkAmtAuthBin = 0;
	u8 tmpAmtAuthBinOther[4] = {0};
	u32 uisdkAmtAuthBinOther = 0;
	u8 lpAmount[6]={0};

	sdkEMVBaseConfigTLV("\x9F\x02", (const u8 *)bcdAmount, 6);
    sdkBcdToU32(&uisdkAmtAuthBin, (const u8 *)bcdAmount, 6);
	if(uisdkAmtAuthBin > 0xFFFFFFFF)
	{
		memcpy(tmpAmtAuthBin,"\xFF\xFF\xFF\xFF",4);
	}
	else
	{
	    sdkU32ToHex(tmpAmtAuthBin,uisdkAmtAuthBin, 4);
	}
	sdkEMVBaseConfigTLV("\x81", tmpAmtAuthBin, 4);

	if(bcdOtherAmount){

		sdkEMVBaseConfigTLV("\x9F\x03", (const u8 *)bcdOtherAmount, 6);

		sdkBcdToU32(&uisdkAmtAuthBinOther,bcdOtherAmount,6);
        sdkU32ToHex(tmpAmtAuthBinOther,uisdkAmtAuthBinOther,4);
		sdkEMVBaseConfigTLV("\x9F\x04", (const u8 *)tmpAmtAuthBinOther, 4);

		uisdkAmtAuthBin += uisdkAmtAuthBinOther;
	    sdkU32ToHex(tmpAmtAuthBin,uisdkAmtAuthBin, 4);
		sdkEMVBaseConfigTLV("\x81", tmpAmtAuthBin, 4);

		memset(lpAmount,0,sizeof(lpAmount));
		sdkBcdAdd(lpAmount,bcdAmount,6,bcdOtherAmount,6);
		sdkEMVBaseConfigTLV("\x9F\x02", (const u8 *)lpAmount, 6);
	}

	return SDK_OK;
}


s32 sdkEMVBasePreTrans()
{
    u8 i;
    u8 ifexistusedaid = 0;		//С�ڷǽ��޶�
	u8 ifexistusedaid2 = 0;		//С�ڵ��ڷǽ��޶�
    SDK_EMVBASE_AID_STRUCT *tempaidparam;
    unsigned char AmtAuthNum[6];
	s32 len=0;
	s32 num=0, aidnum=0;

	if(sdkEMVBaseReadTLV(EMVTAG_AmtAuthNum, AmtAuthNum, &len) != SDK_OK)
	{
		return SDK_ERR;
	}
	TraceHex("emv","AmtAuthNum 9F02:",AmtAuthNum,6);
    tempaidparam = (SDK_EMVBASE_AID_STRUCT *)emvbase_malloc(sizeof(SDK_EMVBASE_AID_STRUCT));

	sdkEMVBaseGetAIDListNum(&aidnum);
    for(i = 0; i < aidnum; i++)
    {
		num = 0;
		sdkEMVBaseGetAnyAIDList(i, 1, tempaidparam, &num);

        if(num == 0 || (tempaidparam->contactorcontactless != 0 && tempaidparam->contactorcontactless != 2))
        {
            continue;
        }
		Trace("emv","i = %d\r\n",i);
		TraceHex("emv","aid",tempaidparam->Aid,tempaidparam->AidLen);
		TraceHex("emv","cl_translimit",tempaidparam->cl_translimit,6);
		TraceHex("emv","cl_offlinelimit",tempaidparam->cl_offlinelimit,6);
		TraceHex("emv","cl_cvmlimit",tempaidparam->cl_cvmlimit,6);

        if(memcmp(AmtAuthNum,tempaidparam->cl_translimit,6) < 0 )
        {
            ifexistusedaid = 1;
			break;
        }
		if(memcmp(AmtAuthNum,tempaidparam->cl_translimit,6) <= 0 )
        {
            ifexistusedaid2 = 1;
        }
    }

    emvbase_free(tempaidparam);

    if(ifexistusedaid)
    {
        return SDK_OK;
    }
	else if(ifexistusedaid2)
    {
        return SDK_EQU;
    }
    else
    {
        return SDK_ERR;
    }

}

s32 sdkEMVBasePreTransGetTransType()
{
    u8 i;
    u8 ifexistusedaid = 0;		//С�ڷǽ��޶�
	u8 ifexistusedaid2 = 0;		//С�ڵ��ڷǽ��޶�
    SDK_EMVBASE_AID_STRUCT *tempaidparam;
    unsigned char AmtAuthNum[6];
	s32 len=0;
	s32 num=0, aidnum=0;
	u8 type = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);

	if(sdkEMVBaseReadTLV(EMVTAG_AmtAuthNum, AmtAuthNum, &len) != SDK_OK)
	{
		return SDK_ERR;
	}
	TraceHex("emv","AmtAuthNum 9F02:",AmtAuthNum,6);
    tempaidparam = (SDK_EMVBASE_AID_STRUCT *)emvbase_malloc(sizeof(SDK_EMVBASE_AID_STRUCT));

	sdkEMVBaseGetAIDListNum(&aidnum);
    for(i = 0; i < aidnum; i++)
    {
		num = 0;
		sdkEMVBaseGetAnyAIDList(i, 1, tempaidparam, &num);

        if((num == 0) || (tempaidparam->contactorcontactless != 0 && tempaidparam->contactorcontactless != 2) || (type != tempaidparam->transvaule && tempaidparam->transvaule != 0xFF))
        {
            continue;
        }
		Trace("emv","i = %d\r\n",i);
		TraceHex("emv","aid",tempaidparam->Aid,tempaidparam->AidLen);
		TraceHex("emv","cl_translimit",tempaidparam->cl_translimit,6);
		TraceHex("emv","cl_offlinelimit",tempaidparam->cl_offlinelimit,6);
		TraceHex("emv","cl_cvmlimit",tempaidparam->cl_cvmlimit,6);

        if(memcmp(AmtAuthNum,tempaidparam->cl_translimit,6) < 0 )
        {
            ifexistusedaid = 1;
			break;
        }
		if(memcmp(AmtAuthNum,tempaidparam->cl_translimit,6) <= 0 )
        {
            ifexistusedaid2 = 1;
        }
    }

    emvbase_free(tempaidparam);

    if(ifexistusedaid)
    {
        return SDK_OK;
    }
	else if(ifexistusedaid2)
    {
        return SDK_EQU;
    }
    else
    {
        return SDK_ERR;
    }

}

s32 sdkEMVBaseFreeTradeMemory()
{
	if(gstEMVBase_EntryPoint)
	{
		if(gstEMVBase_EntryPoint->SelectedApp)
		{
			emvbase_free(gstEMVBase_EntryPoint->SelectedApp);
			gstEMVBase_EntryPoint->SelectedApp = NULL;
		}
		if(gstEMVBase_EntryPoint->AppListCandidate)
		{
			emvbase_free(gstEMVBase_EntryPoint->AppListCandidate);
			gstEMVBase_EntryPoint->AppListCandidate = NULL;
		}
		emvbase_free(gstEMVBase_EntryPoint);
		gstEMVBase_EntryPoint = NULL;
	}

	if(gstEMVBase_UnionStruct)
	{
		if(gstEMVBase_UnionStruct->rapdu)
		{
			emvbase_free(gstEMVBase_UnionStruct->rapdu);
			gstEMVBase_UnionStruct->rapdu = NULL;
		}
		emvbase_free(gstEMVBase_UnionStruct);
		gstEMVBase_UnionStruct = NULL;
	}

	if(gstEMVBase_TermAidlistUnion)
	{
		if(gstEMVBase_TermAidlistUnion->SelectedApp)
		{
			emvbase_free(gstEMVBase_TermAidlistUnion->SelectedApp);
			gstEMVBase_TermAidlistUnion->SelectedApp = NULL;
		}
		if(gstEMVBase_TermAidlistUnion->AppListCandidate)
		{
			emvbase_free(gstEMVBase_TermAidlistUnion->AppListCandidate);
			gstEMVBase_TermAidlistUnion->AppListCandidate = NULL;
		}
		emvbase_free(gstEMVBase_TermAidlistUnion);
		gstEMVBase_TermAidlistUnion = NULL;
	}
	return SDK_OK;
}


s32 sdkEMVBaseCountTransRecord(u8 * TransRecordSFI, u8 * TransRecordNum, u8 RecordType)
{
    unsigned short k, j, len, t;
    u32 uiIndex;
    u8 tag[2] = {0};
    EMVBASETAGCVLITEM *tagitem;
    u8 *tempIssuerDiscret;
    u8 tempIssuerDiscretLen = 0;

    uiIndex = 0;

    if(RecordType == 0)
    {
        memcpy(tag, "\x9F\x4D", 2);
    }
    else if(RecordType == 1)
    {
        memcpy(tag, "\xDF\x4D", 2);
    }
    else
    {
        return SDK_ERR;
    }
    tempIssuerDiscret = (u8 *)sdkGetMem(256);
    memset(tempIssuerDiscret, 0, 256);

    tagitem = emvbase_avl_gettagitempointer(EMVTAG_FCIDisData);

    if(tagitem != NULL)
    {
        if(tagitem->len)
        {
            tempIssuerDiscretLen = tagitem->len;

            if(tempIssuerDiscretLen > 250)
            {
                tempIssuerDiscretLen = 250;
            }
            memcpy(tempIssuerDiscret, tagitem->data, tempIssuerDiscretLen);
        }
    }

    while(uiIndex < tempIssuerDiscretLen)
    {
        if(uiIndex >= tempIssuerDiscretLen)
        {
            sdkFreeMem(tempIssuerDiscret);
            return SDK_ERR;
        }

        if(tempIssuerDiscret[uiIndex] == 0xFF || tempIssuerDiscret[uiIndex] == 0x00)
        {
            uiIndex++; continue;
        }
        else if(!memcmp(tempIssuerDiscret + uiIndex, tag, 2))
        {
            uiIndex += 2;

            if(tempIssuerDiscret[uiIndex] != 2)
            {
                sdkFreeMem(tempIssuerDiscret);
                return SDK_ERR;
            }
            uiIndex++;
            (*TransRecordSFI) = tempIssuerDiscret[uiIndex];
            uiIndex++;
            (*TransRecordNum) = tempIssuerDiscret[uiIndex];

            if((*TransRecordNum) > 10 )
            {
                sdkFreeMem(tempIssuerDiscret);
                return SDK_ERR;
            }
            sdkFreeMem(tempIssuerDiscret);
            return SDK_OK;
        }
        else        //other unknown TLV data
        {
            k = tempIssuerDiscret[uiIndex];

            if((k & 0x1F) == 0x1F)
            {
                uiIndex++;
            }
            uiIndex++;

            if(tempIssuerDiscret[uiIndex] <= 127)
            {
                len = tempIssuerDiscret[uiIndex];
                uiIndex++;
            }
            else
            {
                len = 0;
                t = tempIssuerDiscret[uiIndex] & 0x7F;

                for(j = 1; j <= t; j++)
                {
                    len = (u16)(len * 256 + tempIssuerDiscret[uiIndex + j]);
                }

                uiIndex += t + 1;
            }
            uiIndex += len;
        }
    }

    sdkFreeMem(tempIssuerDiscret);
    return SDK_EQU;
}

u8 sdkEMVBasePostReadLogCommand(EMVBASE_APDU_SEND * apdu_s, u8 RecordType)
{
    memcpy(apdu_s->Command, "\x80\xCA", 2);

    if(RecordType == 0)
    {
        memcpy((u8*)&apdu_s->Command[2], "\x9F\x4F", 2);
    }
    else
    {
        memcpy((u8*)&apdu_s->Command[2], "\xDF\x4F", 2);
    }
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;
    return 0;
}

s32 sdkEMVBaseDealICCParamRecord(const EMVBASE_APDU_RESP * apdu_r, u8 * aucDOL, s32 siAucLen, u8 RecordType)
{
    u8 ucDOLLen;
    u8 tag[2] = {0};

    if(RecordType == 0)
    {
        memcpy(tag, "\x9F\x4F", 2);
    }
    else if(RecordType == 1)
    {
        memcpy(tag, "\xDF\x4F", 2);
    }
    else
    {
        return SDK_ERR;
    }

    if(!(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00) || siAucLen < 0)	// ICC parameter Records found fail.
    {
        return SDK_ERR;
    }
    else
    {
        if(memcmp(apdu_r->DataOut, tag, 2))
        {
            return SDK_ERR;
        }
        else
        {
            if(apdu_r->DataOut[2] <= siAucLen)
            {
                ucDOLLen = apdu_r->DataOut[2];
            }
            else
            {
                ucDOLLen = siAucLen;
            }
            memset(aucDOL, 0x00, siAucLen);
            memcpy(aucDOL, (u8 *)(&apdu_r->DataOut[3]), ucDOLLen);
            return SDK_OK;
        }
    }
}

s32 sdkEMVBaseSetInputPINFun(s32(*fun_inputpin)(u8 ucIccEncryptWay, u8 ucPINTryCount, u8 *pheCreditPwd))
{
	if(gstEMVBase_TradeParam)
	{
		gstEMVBase_TradeParam->InputPWD = fun_inputpin;
		return SDK_OK;
	}
	return SDK_ERR;
}

s32 sdkEMVBaseSetSpTermCheck(bool enable)
{
    u8 SupportTermCheck = enable;

    emvbase_avl_createsettagvalue(EMVTAG_SupportTermCheck, &SupportTermCheck, 1);

	return SDK_OK;
}

s32 sdkEMVBaseGetScriptResult(s32 * psiScriptResultLen, u8 * pheScriptResult)
{
    EMVBASETAGCVLITEM *item;

    if((pheScriptResult == NULL) || (psiScriptResultLen == NULL))
    {
        return SDK_PARA_ERR;
    }
    *psiScriptResultLen  = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_SCRIPT71RESULT);

    if(item != NULL)
    {
        if(item->len)
        {
            memcpy(&pheScriptResult[*psiScriptResultLen], item->data, item->len);
            *psiScriptResultLen += item->len;
        }
    }
    item = emvbase_avl_gettagitempointer(EMVTAG_SCRIPT72RESULT);

    if(item != NULL)
    {
        if(item->len)
        {
            memcpy(&pheScriptResult[*psiScriptResultLen], item->data, item->len);
            *psiScriptResultLen += item->len;
        }
    }
    return SDK_OK;
}

bool sdkEMVBaseNeedSignature()
{
	return gstemvbaseneedsign;
}

SDK_EMVBASE_CVM_RESULT sdkEMVBaseGetCVMresult(void)
{
    return gstemvbaseCVMresult;
}


s32 sdkEMVBaseGetEntryPointAIDList(SDK_EMVBASE_CL_AIDLIST *entrypointlist ,s32 silistnum ,s32 *psiOutlistnum)
{
	u8 i;
	EMVBASE_LISTAPPDATA *tempAppAppData;
    unsigned char *tempappnum;


	if(silistnum < 1 || entrypointlist == NULL || psiOutlistnum == NULL)
	{
        return SDK_PARA_ERR;
    }

	*psiOutlistnum = 0;

	if(gstEMVBase_EntryPoint == NULL)
	{
		return SDK_ICC_NODATA;
	}

	tempappnum = &(gstEMVBase_EntryPoint->AppListCandidatenum);
    tempAppAppData = gstEMVBase_EntryPoint->AppListCandidate;

	Trace("emv","aid list num form entrypoint:%d\r\n",*tempappnum);

	for(i = 0 ; i < silistnum && i < gstEMVBase_EntryPoint->AppListCandidateMaxNum && i < *tempappnum; i++)
	{
		memset((entrypointlist+i),0,sizeof(SDK_EMVBASE_CL_AIDLIST));

		(entrypointlist+i)->AIDLen = (tempAppAppData + i)->AIDLen;
		(entrypointlist+i)->AppLabelLen = (tempAppAppData + i)->AppLabelLen;
		(entrypointlist+i)->PreferNameLen = (tempAppAppData + i)->PreferNameLen;
		(entrypointlist+i)->Priority = (tempAppAppData + i)->Priority;
		(entrypointlist+i)->LangPreferLen = (tempAppAppData + i)->LangPreferLen;
		(entrypointlist+i)->AidInTermLen = (tempAppAppData + i)->AidInTermLen;
		(entrypointlist+i)->KernelIdentifierLen = (tempAppAppData + i)->KernelIdentifierLen;

		memcpy((entrypointlist+i)->AID,(tempAppAppData + i)->AID,(entrypointlist+i)->AIDLen);
		memcpy((entrypointlist+i)->AppLabel,(tempAppAppData + i)->AppLabel,(entrypointlist+i)->AppLabelLen);
		memcpy((entrypointlist+i)->PreferName,(tempAppAppData + i)->PreferName,(entrypointlist+i)->PreferNameLen);
		memcpy((entrypointlist+i)->LangPrefer,(tempAppAppData + i)->LangPrefer,(entrypointlist+i)->LangPreferLen);
		memcpy((entrypointlist+i)->AidInTerm,(tempAppAppData + i)->AidInTerm,(entrypointlist+i)->AidInTermLen);
		memcpy((entrypointlist+i)->KernelIdentifier,(tempAppAppData + i)->KernelIdentifier,(entrypointlist+i)->KernelIdentifierLen);

		(*psiOutlistnum) ++;
	}
	return SDK_OK;
}


s32 sdkEMVBaseSetUserSelcetFixedAid(u8 *aidincard, u8 aidlen)
{
	u8 i;
	EMVBASE_LISTAPPDATA *tempAppAppData;
    unsigned char *tempappnum;
    unsigned char *tempselectappno;


	if(aidlen < 5 || aidincard == NULL)
	{
        return SDK_PARA_ERR;
    }

	if(gstEMVBase_EntryPoint == NULL || gstEMVBase_UnionStruct == NULL)
	{
		return SDK_ICC_NODATA;
	}

	tempappnum = &(gstEMVBase_EntryPoint->AppListCandidatenum);
    tempAppAppData = gstEMVBase_EntryPoint->AppListCandidate;
	tempselectappno = &(gstEMVBase_UnionStruct->SelectedAppNo);

	//Trace("emv","default select aid seqnum:%d\r\n",*tempselectappno);
	//Trace("emv","AppListCandidatenum:%d\r\n",gstEMVBase_EntryPoint->AppListCandidatenum);

	TraceHex("emv","aidincard",aidincard,aidlen);

	for(i = 0 ; i < *tempappnum; i++)
	{
		//TraceHex("emv","aidlist",(tempAppAppData + i)->AID,(tempAppAppData + i)->AIDLen);
		if(aidlen == (tempAppAppData + i)->AIDLen && memcmp(aidincard,(tempAppAppData + i)->AID,aidlen) == 0)
		{
			*tempselectappno = i;
			Trace("emv","user select aid seqnum:%d\r\n",*tempselectappno);
			return SDK_OK;
		}
	}
	return SDK_ICC_NODATA;
}

s32 sdkEMVBaseSetRevocationKey(s32(*fun_cmprevocation)(u8 *key))
{
	if(gstEMVBase_TradeParam)
	{
		gstEMVBase_TradeParam->termipkrevokecheck = fun_cmprevocation;
		return SDK_OK;
	}
	return SDK_ERR;
}


bool sdkEMVBaseCheckTagExit(u8 *tag)
{
	u8 exit = 0;

	if(tag == NULL)
	{
        return false;
    }

	exit = emvbase_avl_checkiftagexist(tag);
	if(exit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void sdkEMVBase_ReadTermAID_CL(EMVBASE_TERMAPP * Applist, unsigned char *TermApplistNum)
{
    unsigned char i, j;
    unsigned char ApplistNum = 0;
	unsigned char transtpye;

	transtpye = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);

    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if( gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE &&
			((transtpye == gEmvbaseAidIndex[i].tradetypevalue && 2 == gEmvbaseAidIndex[i].contactorcontectless)
			|| (transtpye == gEmvbaseAidIndex[i].tradetypevalue && 0 == gEmvbaseAidIndex[i].contactorcontectless)
			|| (0xFF == gEmvbaseAidIndex[i].tradetypevalue && 2 == gEmvbaseAidIndex[i].contactorcontectless)
			|| (0xFF == gEmvbaseAidIndex[i].tradetypevalue && 0 == gEmvbaseAidIndex[i].contactorcontectless)
				)
			)
        {//ȡ������tradetypevalue��contactorcontectless��aid,Ȼ��ȥ��
			for(j=0; j<ApplistNum; j++)//ȥ��
			{
				if((Applist + j)->AIDLen == gEmvbaseAidIndex[i].AIDLen
					&& memcmp((Applist + j)->AID, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen)==0)
				{
					break;
				}
			}
			if(j < ApplistNum)
			{
				continue;
			}

            (Applist + ApplistNum)->ASI = gEmvbaseAidIndex[i].ASI;
            (Applist + ApplistNum)->AIDLen = gEmvbaseAidIndex[i].AIDLen;
            memcpy((Applist + ApplistNum)->AID, &gEmvbaseAidIndex[i].AID[0], 16);
            ApplistNum++;
            if(ApplistNum >= 40)
            {
                break;
            }
        }
    }

	*TermApplistNum = ApplistNum;

}


void sdkEMVBase_ReadTermAID_CT(EMVBASE_TERMAPP * Applist, unsigned char *TermApplistNum)
{
    unsigned char i, j;
    unsigned char ApplistNum = 0;
	unsigned char transtpye;

	transtpye = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);

    for(i = 0; i < EMVBASEMAXAIDINDEX; i++)
    {
        if( gEmvbaseAidIndex[i].ifusedflag == EMVBASEAIDENABLE &&
			((transtpye == gEmvbaseAidIndex[i].tradetypevalue && 1 == gEmvbaseAidIndex[i].contactorcontectless)
			|| (transtpye == gEmvbaseAidIndex[i].tradetypevalue && 0 == gEmvbaseAidIndex[i].contactorcontectless)
			|| (0xFF == gEmvbaseAidIndex[i].tradetypevalue && 1 == gEmvbaseAidIndex[i].contactorcontectless)
			|| (0xFF == gEmvbaseAidIndex[i].tradetypevalue && 0 == gEmvbaseAidIndex[i].contactorcontectless)
				)
			)
        {//ȡ������tradetypevalue��contactorcontectless��aid,Ȼ��ȥ��
			for(j=0; j<ApplistNum; j++)//ȥ��
			{
				if((Applist + j)->AIDLen == gEmvbaseAidIndex[i].AIDLen
					&& memcmp((Applist + j)->AID, gEmvbaseAidIndex[i].AID, gEmvbaseAidIndex[i].AIDLen)==0)
				{
					break;
				}
			}
			if(j < ApplistNum)
			{
				continue;
			}

            (Applist + ApplistNum)->ASI = gEmvbaseAidIndex[i].ASI;
            (Applist + ApplistNum)->AIDLen = gEmvbaseAidIndex[i].AIDLen;
            memcpy((Applist + ApplistNum)->AID, &gEmvbaseAidIndex[i].AID[0], 16);
            ApplistNum++;
            if(ApplistNum >= 40)
            {
                break;
            }
        }
    }

	*TermApplistNum = ApplistNum;

}

EMVBase_LogOutput *gstsdkemvbaselogoutput = NULL;
extern void EMVBase_SetLogOutput(EMVBase_LogOutput *pstLogOutput);

void sdkEMVBase_SetEMVLogOutput(bool enable)
{
	if(enable)
	{
		if(gstsdkemvbaselogoutput == NULL)
		{
			gstsdkemvbaselogoutput = sdkGetMem(sizeof(EMVBase_LogOutput));
		}
		gstsdkemvbaselogoutput->EmvBase_Printf = sdkDev_Printf;
		gstsdkemvbaselogoutput->logoutputswitch = 1;
		EMVBase_SetLogOutput(gstsdkemvbaselogoutput);
	}
	else
	{
		EMVBase_SetLogOutput(NULL);
	}
}

s32 sdkEMVBaseSelectFromTermSelect(u8 *aidnum)
{
    u8 retCode = RLT_EMV_ERR;
	s32 i=0;

	if(gstEMVBase_TermAidlistUnion == NULL)
	{
		gstEMVBase_TermAidlistUnion = (EMVBase_TermAidlistUnionStruct *)emvbase_malloc(sizeof(EMVBase_TermAidlistUnionStruct));
	}
	else
	{
		if(gstEMVBase_TermAidlistUnion->SelectedApp)
		{
			if(gstEMVBase_TermAidlistUnion->SelectedApp->ASRPDLen && gstEMVBase_TermAidlistUnion->SelectedApp->ASRPD)
			{
				emvbase_free(gstEMVBase_TermAidlistUnion->SelectedApp->ASRPD);
				gstEMVBase_TermAidlistUnion->SelectedApp->ASRPDLen = 0;
				gstEMVBase_TermAidlistUnion->SelectedApp->ASRPD = NULL;
			}
			emvbase_free(gstEMVBase_TermAidlistUnion->SelectedApp);
			gstEMVBase_TermAidlistUnion->SelectedApp = NULL;
		}

		if(gstEMVBase_TermAidlistUnion->AppListCandidate)
		{
			for(i=0; i<gstEMVBase_TermAidlistUnion->AppListCandidateMaxNum; i++)
			{
				if(gstEMVBase_TermAidlistUnion->AppListCandidate[i].ASRPDLen && gstEMVBase_TermAidlistUnion->AppListCandidate[i].ASRPD)
				{
					emvbase_free(gstEMVBase_TermAidlistUnion->AppListCandidate[i].ASRPD);
					gstEMVBase_TermAidlistUnion->AppListCandidate[i].ASRPDLen = 0;
					gstEMVBase_TermAidlistUnion->AppListCandidate[i].ASRPD = NULL;
				}
			}
			emvbase_free(gstEMVBase_TermAidlistUnion->AppListCandidate);
			gstEMVBase_TermAidlistUnion->AppListCandidate = NULL;
		}
	}

	memset(gstEMVBase_TermAidlistUnion,0,sizeof(EMVBase_TermAidlistUnionStruct));
	gstEMVBase_TermAidlistUnion->SelectedApp = (EMVBASE_LISTAPPDATA *)emvbase_malloc(sizeof(EMVBASE_LISTAPPDATA));
    memset((unsigned char *)gstEMVBase_TermAidlistUnion->SelectedApp, 0, sizeof(EMVBASE_LISTAPPDATA));

    gstEMVBase_TermAidlistUnion->AppListCandidate = (EMVBASE_LISTAPPDATA *)emvbase_malloc(sizeof(EMVBASE_LISTAPPDATA) * gAppListCandicateMaxNum);
    memset((unsigned char *)gstEMVBase_TermAidlistUnion->AppListCandidate, 0, sizeof(EMVBASE_LISTAPPDATA) * gAppListCandicateMaxNum);
	gstEMVBase_TermAidlistUnion->AppListCandidateMaxNum = gAppListCandicateMaxNum;

    gstEMVBase_TermAidlistUnion->IsoCommand = sdkEMVBase_ContactlessIsoCommand;
    gstEMVBase_TermAidlistUnion->CheckMatchTermAID = sdkEMVBase_CheckMatchTermAID_CL;
	gstEMVBase_TermAidlistUnion->ReadTermAID = sdkEMVBase_ReadTermAID_CL;

	retCode = EMVBase_SelectFromTerm(gstEMVBase_TermAidlistUnion);
	Trace("emv", "EMVBase_SelectFromTerm ret = %d\r\n", retCode);

	if(retCode == RLT_EMV_OK)
	{
		retCode = EMVBase_ChooseTermApp(gstEMVBase_TermAidlistUnion);
		Trace("emv", "EMVBase_ChooseApp ret = %d\r\n", retCode);
	}
	if(retCode == RLT_EMV_OK)
	{
		*aidnum = gstEMVBase_TermAidlistUnion->AppListCandidatenum;
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}
}


s32 sdkEMVBaseGetFromTermSelectAIDList(SDK_EMVBASE_CL_AIDLIST *entrypointlist ,s32 silistnum ,s32 *psiOutlistnum)
{
	u8 i;
	EMVBASE_LISTAPPDATA *tempAppAppData;
    unsigned char *tempappnum;


	if(silistnum < 1 || entrypointlist == NULL || psiOutlistnum == NULL)
	{
        return SDK_PARA_ERR;
    }

	*psiOutlistnum = 0;

	if(gstEMVBase_TermAidlistUnion == NULL)
	{
		return SDK_ICC_NODATA;
	}

	if(gstEMVBase_EntryPoint == NULL || gstEMVBase_EntryPoint->AppListCandidate == NULL)
	{
		return SDK_ICC_NODATA;
	}

	tempappnum = &(gstEMVBase_TermAidlistUnion->AppListCandidatenum);
    tempAppAppData = gstEMVBase_TermAidlistUnion->AppListCandidate;

	Trace("emv","aid list num form term select:%d\r\n",*tempappnum);

	gstEMVBase_EntryPoint->AppListCandidatenum = gstEMVBase_TermAidlistUnion->AppListCandidatenum;
	memcpy(gstEMVBase_EntryPoint->AppListCandidate,gstEMVBase_TermAidlistUnion->AppListCandidate,sizeof(EMVBASE_LISTAPPDATA) * gstEMVBase_TermAidlistUnion->AppListCandidateMaxNum);

	for(i = 0 ; i < silistnum && i < gstEMVBase_TermAidlistUnion->AppListCandidateMaxNum && i < *tempappnum; i++)
	{
		memset((entrypointlist+i),0,sizeof(SDK_EMVBASE_CL_AIDLIST));

		(entrypointlist+i)->AIDLen = (tempAppAppData + i)->AIDLen;
		(entrypointlist+i)->AppLabelLen = (tempAppAppData + i)->AppLabelLen;
		(entrypointlist+i)->PreferNameLen = (tempAppAppData + i)->PreferNameLen;
		(entrypointlist+i)->Priority = (tempAppAppData + i)->Priority;
		(entrypointlist+i)->LangPreferLen = (tempAppAppData + i)->LangPreferLen;
		(entrypointlist+i)->AidInTermLen = (tempAppAppData + i)->AidInTermLen;
		(entrypointlist+i)->KernelIdentifierLen = (tempAppAppData + i)->KernelIdentifierLen;

		memcpy((entrypointlist+i)->AID,(tempAppAppData + i)->AID,(entrypointlist+i)->AIDLen);
		memcpy((entrypointlist+i)->AppLabel,(tempAppAppData + i)->AppLabel,(entrypointlist+i)->AppLabelLen);
		memcpy((entrypointlist+i)->PreferName,(tempAppAppData + i)->PreferName,(entrypointlist+i)->PreferNameLen);
		memcpy((entrypointlist+i)->LangPrefer,(tempAppAppData + i)->LangPrefer,(entrypointlist+i)->LangPreferLen);
		memcpy((entrypointlist+i)->AidInTerm,(tempAppAppData + i)->AidInTerm,(entrypointlist+i)->AidInTermLen);
		memcpy((entrypointlist+i)->KernelIdentifier,(tempAppAppData + i)->KernelIdentifier,(entrypointlist+i)->KernelIdentifierLen);

		(*psiOutlistnum) ++;
	}
	return SDK_OK;
}

s32 sdkEMVBaseSetSupportExternSelect(u8 flag)
{
	if(gstEMVBase_TradeParam == NULL)
	{
		return SDK_PARA_ERR;
	}

	gstEMVBase_TradeParam->SupportExternSelect = flag;

	return SDK_OK;
}

s32 sdkEMVBaseSetAppListCandicateMaxNum(u8 maxnum)
{
	if(maxnum<1) return SDK_PARA_ERR;

	gAppListCandicateMaxNum = maxnum;
	return SDK_OK;
}

s32 sdkEMVBaseSetPpseRespType(u8 flag)
{
	if(gstEMVBase_TradeParam == NULL)
	{
		return SDK_ERR;
	}

	gstEMVBase_TradeParam->PpseRespType = flag;

	return SDK_OK;
}

s32 sdkEmvBaseGetLibVersion(u8 *version)
{
	if(version == NULL)
	{
		return SDK_PARA_ERR;
	}

	EMVBase_GetLibVersion(version);
	return SDK_OK;
}

