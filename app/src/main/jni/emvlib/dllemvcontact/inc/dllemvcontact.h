#ifndef _DLLEMVCONTACT_H_
#define _DLLEMVCONTACT_H_

#include "dllemvbase.h"

//emvcontact response start form 51
#define EMVCONTACT_READAIDPARAMETERS	51
#define EMVCONTACT_EXTRACTMATCH			52
#define EMVCONTACT_PARTICALMATCH		53
#define EMVCONTACT_NOTMATCH 			54
#define EMVCONTACT_ERR_CVMFailALWAYS	55
#define EMVCONTACT_ERR_CVMNoSupport		56
#define EMVCONTACT_ERR_CVMFail			57
#define EMVCONTACT_ERR_NEEDMAGCARD		58
#define EMVCONTACT_ERR_EMV_CDAFORMAT	59
#define EMVCONTACT_ERR_EMV_CDADifferCID	60
#define EMVCONTACT_ERR_EMV_CDADifferCIDTC	61
#define EMVCONTACT_ERR_EMV_CDADifferCIDAAC	62	//20190530_lhd
#define EMVCONTACT_ERR_EMV_ScriptFormat	63
#define EMVCONTACT_EMV_NEEDREVERSAL		64
#define EMVCONTACT_ERR_PBOCLOAD			65
#define EMVCONTACT_ERR_Bypass           66


//transtype
#define TRANS_NORMAL		0
#define TRANS_LOAD			1

//9C
#define EMVBASE_TRANS_GOODS  	0x00
#define EMVBASE_TRANS_CASH   	0x01
#define EMVBASE_TRANS_CASHBACK 	0x09

typedef struct
{
    unsigned char *AppPriority;	//sjz
    unsigned char *AppSeq;	//sjz
    unsigned char seqtype;
    unsigned char SelectedNum;
    unsigned char SelectedResult;
}EMVCONTACTDISPSEQLISTDATA;


typedef struct{
    EMVBASE_CAPK_STRUCT *CAPK;
    EMVBASE_LISTAPPDATA *SelectedApp;
	EMVBASE_LISTAPPDATA *AppListCandidate;
	unsigned char *AuthData;
	unsigned char *IPKModul;
	unsigned char *ICCPKModul;
	unsigned char *ICCPINPKModul;
	unsigned char *RedundantData;
	unsigned short AuthDataMaxLen;
	unsigned short AuthDataLen;
	unsigned short RedundantDataMaxLen;
	unsigned short RedundantDataLen;
	unsigned short AuthDataFixLen;
	unsigned char IPKModulLen;
    unsigned char ICCPKModulLen;
	unsigned char ICCPINPKModulLen;
	unsigned char AppListCandidatenum;
	unsigned char SelectedAppNo;
    unsigned char bPrintReceipt;
	unsigned char bErrSDATL;
	unsigned char bErrAuthData;
	unsigned char TransResult;
	unsigned char onlinetradestatus;
	unsigned char ODASMstaus;
	unsigned char appblockflag;
	unsigned char ReadLogFlag;
	unsigned char bAbleOnline;
	unsigned char Ec_LessThanResetAmount;
	unsigned char bCDOL1HasNoUnpredictNum;
	unsigned char bCDOL2HasNoUnpredictNum;
	unsigned char GenerateACRetDataLen;
	unsigned char GenerateACRetData[255];
	unsigned char Transflow;
	unsigned char bAdvice;
	unsigned char bretforminit;
	EMVCONTACTDISPSEQLISTDATA DispSeqListData;
	unsigned char AppListCandidateMaxNum;
	unsigned short Track2OffsetInAuthData;
	unsigned char Track2DataLen;
	unsigned char bTrack2DataMasked;
	unsigned short Track2OffsetInDDOLData;
	unsigned char bTrack2InDDOLData;
	unsigned short Track2OffsetInCDOL1Data;
	unsigned char bTrack2InCDOL1Data;
    unsigned char bTrack2InCDOL2Data;
	unsigned short Track2OffsetInCDOL2Data;
	unsigned char bCDOL2DataWithTrack2;
	unsigned char bDataStorageEnable; // for DPAS_CT
	unsigned char bExtendedLoggingEnable; // for DPAS_CT
	unsigned char DSDHashValue[20]; // for DPAS_CT
	unsigned char bRequestAAC;	//for Request AAC
}EMVCONTACTTRADEPARAMETER;





#define EMVB_DATAAUTH_ONLINEPIN               1
#define EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN     2
#define EMVB_DATAAUTH_OFFLINEENCPIN           3
#define EMVB_DATAAUTH_INPUTPINSTATUS          4
#define EMVB_DATAAUTH_VERIFYIDCARD            5
#define EMVB_NEEDREFER                        6
#define EMVB_DISP_PERFORMCDCVM                7



typedef void (*EMVBASE_ContactIsoCommand)(EMVBASE_APDU_SEND *ApduSend,EMVBASE_APDU_RESP *apdu_r);
typedef void (*EMVBASE_ContactIsoCommandEx)(EMVBASE_APDU_SEND *ApduSend,EMVBASE_APDU_RESP *apdu_r, unsigned short offset);
typedef void (*EmvContact_ReadTermAID)(EMVBASE_TERMAPP *Applist,unsigned char *TermApplistNum);
typedef unsigned char (*EMVBCORE_InputCreditPwd)(unsigned char pintype,unsigned char *ASCCreditPwd);
typedef void (*EMVBCORE_RandomNum)(unsigned char *RandomNum,unsigned int RandomNumLen);
typedef void (*EMVBCORE_Printf)(char *fmt, ...);
typedef int (*EmvContact_GetHashWithMagMask)(unsigned char Track2DataLen, unsigned char mode, unsigned char offsetNum, 
	unsigned short *offset, unsigned short dataLen, unsigned char *data, unsigned char *pkData, unsigned char *hashValue);


typedef int (*EmvContact_CheckTrack2Mask)(unsigned short startpos, unsigned short endpos, unsigned char authdataflag, unsigned char* MaskFlag, unsigned char* Track2DataLen,unsigned short* Track2OffsetInAuthData);


typedef struct{
    EMVCONTACTTRADEPARAMETER *EmvTradeParam;
	EMVBASE_ContactIsoCommand IsoCommand;
	EMVBASE_CheckMatchTermAID CheckMatchTermAID;
	EMVBase_termipkrevokecheck termipkrevokecheck;
	int (*CheckCardNo)(const unsigned char *pasPAN);
	EmvContact_ReadTermAID ReadTermAID;
	EMVBCORE_InputCreditPwd EMVB_InputCreditPwd;
	EMVBCORE_RandomNum EMVB_RandomNum;
	int (*getofflineencdata)(unsigned char *encdata,unsigned int encdatalen);
	EmvContact_GetHashWithMagMask GetHashWithMagMask;
	EMVBASE_ContactIsoCommandEx IsoCommandEx;
	int (*CheckDataContainer)(const unsigned char *ContainerID);
	void (*StoreContainerContent)(const unsigned char *ContainerID, const unsigned char *ContainerContent, unsigned short ContentLen);

	EmvContact_CheckTrack2Mask CheckTrack2Mask;
}EMVCONTACTTradeUnionStruct;


extern unsigned char EmvContact_GetKernelVerInfo(int machine_code, unsigned char ver[32]);

#endif

