#ifndef _DLLJCB_H_
#define _DLLJCB_H_

#include "dllemvbase.h"

#define PURE_READAIDPARAMETERS		51
#define PURE_EXTRACTMATCH			52
#define PURE_PARTICALMATCH			53
#define PURE_NOTMATCH 				54
#define PURE_ERR_CVMFailALWAYS		55
#define PURE_ERR_CVMNoSupport		56
#define PURE_ERR_CVMFail			57
#define PURE_ERR_EMV_ScriptFormat	58
#define PURE_ERR_READCARDAGAIN		59		//communicate with card error,read card again
#define PURE_REQ_INPUTPIN			60
#define PURE_REQ_CHECKREVOKEY		61
#define PURE_REQ_LOADAIDPARAM		62
#define PURE_ERR_NO9C				63
#define PURE_REQ_SECONDTAP			64
#define PURE_REQ_FORCEONLINE		65

#define PURE_CVMMETHOD_DEFAULT  				0x3F
#define PURE_CVMMETHOD_NOCVMPERFORMED 			0x1F
#define PURE_CVMMETHOD_SIGBATURE 				0x1E
#define PURE_CVMMETHOD_ONLINEPIN 				0x02
#define PURE_CVMMETHOD_PLAINTEXTPINVERIFICATION 0x01
#define PURE_CVMMETHOD_CDCVM 					0x24

#define PURE_CVMRESULT_UNKNOWN 		0x00
#define PURE_CVMRESULT_SUCCESSFUL 	0x02

#define PURE_UIREQ_OUTCOME		0x01
#define PURE_UIREQ_RESTART		0x02

typedef struct
{
	EMVBASE_CAPK_STRUCT *CAPK;
    EMVBASE_LISTAPPDATA *SelectedApp;
	EMVBASE_LISTAPPDATA *AppListCandidate;
	unsigned char AppListCandidatenum;

	unsigned char bPrintReceipt:1;
	unsigned char bErrSDATL:1;
	unsigned char bErrAuthData:1;	//for files with SFI in the range 1 to 10, the record contents is not TLVcoded with tag equal to ‘70’
	unsigned char bCDOL1HasNoUnpredictNum:1;
	unsigned char bCDOL2HasNoUnpredictNum:1;

    unsigned char bAbleOnline:1;
	unsigned char bReversal:1;
	unsigned char bAdvice:1;
	unsigned char IfNeedRefer:1;

    unsigned char JcbIUPperformed:1;
	unsigned char startdealiup:1;
    unsigned char canceldealiup:1;

	unsigned short AuthDataMaxLen;
	unsigned short AuthDataLen;
    unsigned char *AuthData;  //SM �㷨��ʱ���������ȱ��Դ,ʹ��AuthData
    unsigned char IPKModulMaxLen;
	unsigned char IPKModulLen;
	unsigned char *IPKModul;       //zcl 2048 - > 248
	unsigned char ICCPKModulMaxLen;
    unsigned char ICCPKModulLen;
	unsigned char *ICCPKModul;

	unsigned char ICCPINPKModulLen;
	unsigned char *ICCPINPKModul;

	unsigned short RedundantDataMaxLen;
	unsigned short RedundantDataLen;
	unsigned char *RedundantData;
	unsigned char TransResult;
	unsigned char SelectedAppNo;
	unsigned char onlinetradestatus;

	unsigned char PureImplementationOption;//1111 1000 EMV/Legacy Mode 1.Offline Data Authentication 2.Exception File Check 3.Issuer Update
	unsigned char PureCandidateListEmptyFlag; //for jcb contactless terminal spec V1.3
	unsigned char PureCVMParameter;
	unsigned char CCIDExistInGPO;
	unsigned char CCIDInGPO;
	unsigned char PreprcessIndicator;
	unsigned char CurTransType;
	unsigned char CurProcessIndicator;
	unsigned char Error;
	unsigned char EchoTransIndicator;	//0:normal transaction;1:transaction for recovery of a previous torn transaction (ECHO command is used)
	unsigned char CommuProblemIndicator;	//0:No communication problem;1:communication problem during 1st GENERATE AC;2:communication problem during 2nd GENERATE AC;3:card in communication with the contactless reader after an online authorization (2nd tap) is distinct from the card used at the beginning of the transaction or the card in communication with the contactless reader after a torn transaction is not the same card.
	unsigned char ECHORespData[255];
	unsigned char ECHORespDataLen;
	unsigned char OfflineCAMSelectedIndicator;	//0:None;1:SDA selected;2:CDA selected;3: DDA selected (only possible with Application Authentication Transaction)
	unsigned char ECHOCommandSupport;	//0:Either the terminal or the contactless device are not supporting the ECHO command;1:Both the terminal and the contactless device are supporting the ECHO command
	unsigned char bForceOnline;
	unsigned char bForceAAC;
	unsigned char GenerateACRetDataLen;
	unsigned char GenerateACRetData[255];	//save GAC response for CDA
	unsigned char FCIIn1stSelect[255];	//record 1st FCI of select response for compare 2nd FCI of select when trans goto online deal or echo torn deal
	unsigned char FCIIn1stSelectLen;
	unsigned char FCIDifferFlag;		//1-: 1st select FCI and 2nd select FCI content different
	unsigned char FCIParseErrorFlag;	//0-:FCI template present and parse correct;1-:FCI parse error;2-:FCI dont be obtained
	unsigned char SecondTap;
	unsigned char RequestOnlinePIN;
}PURETRADEPARAMETER;;


typedef unsigned char (*PURE_InputCreditPwd)(unsigned char pintype,unsigned char *ASCCreditPwd);
typedef void (*PURE_RandomNum)(unsigned char *RandomNum,unsigned int RandomNumLen);
typedef unsigned char (*PURE_VerifyCardNo)(unsigned char *asPAN);


typedef int (*PURE_SetOutcome)(unsigned char Result, unsigned char Start, unsigned char CVM, unsigned char UIRequestonOutcomePresent, unsigned char UIRequestonRestartPresent, unsigned char DataRecordPresent, unsigned char DiscretionaryDataPresent, unsigned char AlternateInterfacePreference, unsigned char Receipt, unsigned char FieldOffRequest, unsigned char *RemovalTimeout, unsigned char OnlineResponseData);
typedef int (*PURE_SetUIRequest)(unsigned char MessageID, unsigned char Status, unsigned char HoldTime, unsigned char *LanguagePerference, unsigned char ValueQualifier, unsigned char *Value, unsigned char *CurrencyCode);
typedef void (*PURE_SendOutcome)();
typedef void (*PURE_SendUIRequest)(int type);

typedef unsigned char (*PURE_GetInputPINRes)(void);
typedef int (*PURE_GetVerifyCardNoRes)(void);
typedef int(*PURE_GetVerifyRevocationKeyRes)(void);
typedef unsigned char(*PURE_CheckCAPKExist)(PURETRADEPARAMETER *EMVTradeParam);
typedef int(*PURE_GetTransAmtSumRes)(void);
typedef int(*PURE_Preprocess)(PURETRADEPARAMETER *EMVTradeParam);
typedef int(*PURE_CheckRestrictAID)(unsigned char *AID, int len);

typedef struct{

    PURETRADEPARAMETER *EMVTradeParam;
	EMVBASE_ContactlessIsoCommand IsoCommand;
	PURE_InputCreditPwd EMVB_InputCreditPwd;
	PURE_RandomNum EMVB_RandomNum;
	EMVBase_termipkrevokecheck termipkrevokecheck;
	EMVBASE_CheckMatchTermAID CheckMatchTermAID;
 	EMVBASE_ReadTermAID ReadTermAID;
	PURE_VerifyCardNo VerifyCardNo;
	PURE_SetOutcome SetOutcome;
	PURE_SetUIRequest SetUIRequest;
	PURE_SendOutcome SendOutcome;
	PURE_SendUIRequest SendUIRequest;
	PURE_GetInputPINRes GetInputPINRes;
	PURE_GetVerifyCardNoRes GetVerifyCardNoRes;
	PURE_GetVerifyRevocationKeyRes GetVerifyRevocationKeyRes;
	PURE_CheckCAPKExist CheckCapkExist;
	PURE_GetTransAmtSumRes GetTransAmtSumRes;
	PURE_Preprocess Preprocess;
	PURE_CheckRestrictAID CheckRestrictAID;
}PURETradeUnionStruct;

typedef struct
{
	unsigned char RecoveryTornEMVTransactionFlag;
	unsigned char TornTrack2len;
	unsigned char TornTrack2Data[19];
	unsigned char TornCDAHashDatalen;
	unsigned char TornCDAHashDataBuffer[507];
	unsigned char ReferenceControlParameter;
	unsigned char UnpredictNum[4];
	unsigned char crc;
}PURETORNRECOVERYCONTEXT;


extern unsigned char pure_FinalSelectedApp(PURETradeUnionStruct *App_UnionStruct);
extern unsigned char pure_InitialApp(PURETradeUnionStruct *tempApp_UnionStruct);
extern unsigned char pure_RebuildApplist(PURETradeUnionStruct *App_UnionStruct);
extern unsigned char pure_ChooseApp(PURETradeUnionStruct *App_UnionStruct);
extern unsigned char pure_ReadAppData(PURETradeUnionStruct *tempApp_UnionStruct);
extern unsigned char pure_ProcessRestrict(PURETradeUnionStruct * tempApp_UnionStruct);
extern unsigned char pure_DataAuth(PURETradeUnionStruct *tempApp_UnionStruct);
extern void pure_GetLibVersion(unsigned char *version);

#endif


