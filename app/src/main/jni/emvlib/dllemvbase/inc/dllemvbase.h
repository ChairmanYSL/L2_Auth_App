#ifndef _DLLEMVBASE_H_
#define _DLLEMVBASE_H_

#include <string.h>
#include <stddef.h>
#include "dllemvbasetagbaselib.h"
#include "dllemvbaseavloperate.h"


//emv response
#define RLT_EMV_OK					0
#define RLT_EMV_ERR					1
#define RLT_EMV_OTHER				2
#define RLT_EMV_OFFLINE_APPROVE     3
#define RLT_EMV_OFFLINE_DECLINE     4
#define RLT_EMV_ONLINE_APPROVE      5
#define RLT_EMV_ONLINE_DECLINE      6
#define RLT_EMV_ONLINE_WAIT			7
#define RLT_EMV_TERMINATE_TRANSERR	8
#define RLT_EMV_TRANSNOTACCEPT		9
#define RLT_EMV_USERSELECT			10
#define RLT_EMV_APPSELECTTRYAGAIN	11	//select next
#define RLT_EMV_REQUESTONLINEENCPIN 12
#define RLT_ERR_EMV_IccDataFormat   13
#define RLT_ERR_EMV_APDUTIMEOUT		14
#define RLT_ERR_EMV_IccCommand      15
#define RLT_ERR_EMV_IccReturn       16
#define RLT_ERR_EMV_IccDataMissing  17
#define RLT_ERR_EMV_IccDataRedund   18
#define RLT_ERR_EMV_NoAppSel        19
#define RLT_ERR_EMV_TransRestart    20	//start B
#define RLT_ERR_EMV_CVMRFail		21
#define RLT_ERR_EMV_CVMRNoSupport	22
#define RLT_ERR_EMV_InputBYPASS     23
#define RLT_ERR_EMV_NotAccept       24
#define RLT_ERR_EMV_CancelTrans     25
#define RLT_ERR_EMV_CardBlock       26
#define RLT_ERR_EMV_APPBLOCK		27
#define RLT_ERR_INITAPP_NOTACCEPED	28
#define RLT_ERR_EMV_LASTRECORD		29
#define RLT_ERR_EMV_PSENOTFOUND		30
#define RLT_ERR_EMV_SWITCHINTERFACE	31
#define RLT_ERR_EMV_APPEFFECT		32
#define RLT_ERR_EMV_APPEXPIRED		33
#define RLT_ERR_EMV_SEEPHONE		34
#define RLT_ERR_EMV_EXCEPTION		35	//black
#define RLT_ERR_EMV_NEEDMAGCARD		36
#define RLT_ERR_EMV_TermDataMissing	37
#define RLT_EMV_PPSE_REV_6A82		38
#define RLT_EMV_EXTRACTMATCH		39
#define RLT_EMV_PARTICALMATCH		40
#define RLT_EMV_NOTMATCH 			41
#define RLT_ERR_DFMISSING 			42
#define RLT_EMV_TORN				43
#define RLT_ERR_EMV_OTHERAPCARD	    44


//emv response max to 50


//aid match mode
#define AID_PARTIAL_MATCH  0
#define AID_EXACT_MATCH    1


//apdu result
#define APDUCOMMANDOK	0
#define APDUCOMMANDERR	1
#define APDUCOMMTIMEOUT	2
#define APDUCOMMCANCEL	3

//transaction result
#define RESULT_OFFLINE_APPROVE         10
#define RESULT_OFFLINE_DECLINE         11
#define RESULT_ONLINE_APPROVE          12
#define RESULT_ONLINE_DECLINE          13
#define RESULT_ONLINE_WAIT			   14
#define RESULT_TERMINATE		15
#define RESULT_TERMINATE_NOTACCEPT 16


#define EMVB_DATAAUTH_ONLINEPIN               1
#define EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN     2
#define EMVB_DATAAUTH_OFFLINEENCPIN           3
#define EMVB_DATAAUTH_INPUTPINSTATUS          4
#define EMVB_DATAAUTH_VERIFYIDCARD            5
#define EMVB_NEEDREFER                        6




//contectless transflow mode
#define TRANSFLOW_EMVMODE	1
#define TRANSFLOW_MSDMODE	2


//init for emv malloc & free
typedef void* (*emvbase_core_malloc)(unsigned int size);
typedef void (*emvbase_core_free)(void *block);

typedef struct{
	emvbase_core_malloc EMVBallocator_malloc;
	emvbase_core_free   EMVBallocator_free;
}EMVBaseallocator;

extern void *emvbase_malloc (unsigned int size);
extern void emvbase_free (void *block);
extern void emvbase_allocator_init(EMVBaseallocator *allocator);


typedef struct
{
    unsigned char AIDLen;
    unsigned char AID[16];
    unsigned char AppLabelLen;
    unsigned char AppLabel[16];
    unsigned char PreferNameLen;
    unsigned char PreferName[16];
    unsigned char Priority;
    unsigned char LangPreferLen;
    unsigned char LangPrefer[8];
    unsigned char ICTI;		//Issuer Code Table Index.lang used for display app list according to ISO8859.but not include Chinese,Korea,etc.
    unsigned char ICTILen;
    unsigned char AidInTermLen;
    unsigned char AidInTerm[16];
    unsigned char KernelIdentifierLen;
    unsigned char KernelIdentifier[8];
    unsigned char ReqKernelID;
	unsigned char ASRPDLen;//2020.07.06 add
	unsigned char *ASRPD;//2020.07.06 add
	unsigned char rfuLen;//2021.03.09 add
	unsigned char *rfu;//2021.03.09 add
}EMVBASE_LISTAPPDATA;


typedef struct _EMVBASEEntryPoint
{
	EMVBASE_LISTAPPDATA *AppListCandidate;
	unsigned char AppListCandidatenum;
	unsigned char SelectedAppNo;

}EMVBASE_EntryPoint;


typedef struct
{
    unsigned char AppPriority[16];
    unsigned char AppSeq[16];
    unsigned char seqtype;
    unsigned char SelectedNum;
    unsigned char SelectedResult;
}EMVBASE_DISPSEQLISTDATA;


typedef struct
{
    unsigned char Command[4];
    unsigned char Lc;
    unsigned char DataIn[255];
	unsigned short Le;
    unsigned char EnableCancel;
}EMVBASE_APDU_SEND;


typedef struct
{
    unsigned short LenOut;
    unsigned char DataOut[300];
    unsigned char SW1;
    unsigned char SW2;
	unsigned char ReadCardDataOk;
}EMVBASE_APDU_RESP;


typedef struct
{
    unsigned char Type;
    unsigned char DFNameExist;
    unsigned char DFNameLen;
    unsigned char DFName[16];
    unsigned char FCIPropExist;
    unsigned char SFIExist;
    unsigned char SFI;
	unsigned char ApplicationTemplateExist;
    unsigned char LangPreferExist;
    unsigned char LangPreferLen;
    unsigned char LangPrefer[8];
    unsigned char ICTIExist;
    unsigned char ICTI;
    unsigned char AppLabelExist;
    unsigned char AppLabelLen;
    unsigned char AppLabel[16];
    unsigned char PriorityExist;
    unsigned char Priority;
    unsigned char PDOLExist;
    unsigned char PDOLLen;
    unsigned char PreferNameExist;
    unsigned char PreferNameLen;
    unsigned char PreferName[16];
    unsigned char IssuerDiscretExist;
    unsigned char IssuerDiscretLen;
    unsigned char LogEntryExist;
    unsigned char LogEntryLen;
    unsigned char LogEntry[2];
}EMVBASE_SELECT_RET;


typedef struct
{
    unsigned char Type;		//1:DDF,2:ADF
    unsigned char DFNameExist;	//0-non exist;1-exist.
    unsigned char DFNameLen;
    unsigned char DFName[16];	//5-16,ADF or DDF name according to Type.
    unsigned char AppLabelExist;
    unsigned char AppLabelLen;
    unsigned char AppLabel[16];
    unsigned char PreferNameExist;
    unsigned char PreferNameLen;
    unsigned char PreferName[16];
    unsigned char PriorityExist;
    unsigned char Priority;		//tag'87'
    unsigned char DirDiscretExist;
    unsigned char DirDiscretLen;
}EMVBASE_RECORD_PSE;


typedef struct
{
    unsigned char ASI;		//0-partial match up to the length;1-match exactly
    unsigned char AIDLen;
    unsigned char AID[16];
    unsigned char bLocalName;	//If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char AppLocalNameLen;
    unsigned char AppLocalName[16];
    unsigned char KernelID;
}EMVBASE_TERMAPP;


typedef struct
{
    unsigned char AidLen;
    unsigned char Aid[16];		//5-16
    unsigned char Asi;			//Application Selection Indicator.0-needn't match exactly(partial match up to the length);1-match exactly
    unsigned char AppVerNum[2];
    unsigned char TacDefault[5];
    unsigned char TacOnline[5];
    unsigned char TacDecline[5];
    unsigned char FloorLimit[4];
    unsigned char Threshold[4];
    unsigned char MaxTargetPercent;
    unsigned char TargetPercent;
    unsigned char TermDDOLLen;
    unsigned char TermDDOL[128];
    unsigned char TermPinCap;
    unsigned char ectranslimit[6];		//for contact PBOC use only - low value payment - electronic cash
    unsigned char cl_cvmlimit[6];
    unsigned char cl_translimit[6];
    unsigned char cl_offlinelimit[6];

	unsigned char transvaule;		//if for all transcation type set transvaule 0xFF, otherwise set transvaule with tag9C
	unsigned char contactorcontactless;	//if contactorcontactless is 0,for both contact and contactless; if is 1,for contact only; if is 2,for contactless only
}EMVBASE_AID_STRUCT;


typedef struct
{
    unsigned char RID[5];
    unsigned char CAPKI;
    unsigned char HashInd;
    unsigned char ArithInd;
    unsigned char ModulLen;
    unsigned char Modul[248];
    unsigned char ExponentLen;
    unsigned char Exponent[3];
    unsigned char CheckSumLen;
    unsigned char CheckSum[20];
    unsigned char ExpireDate[4];
}EMVBASE_CAPK_STRUCT;


typedef struct
{
    unsigned char DataHead;
    unsigned char CertFormat;
    unsigned char IssuID[4];
    unsigned char ExpireDate[2];
    unsigned char CertSerial[3];
    unsigned char HashInd;
    unsigned char IPKAlgoInd;
    unsigned char IPKLen;
    unsigned char IPKExpLen;
    unsigned char IPKLeft[212];
    unsigned char HashResult[20];
    unsigned char DataTrail;
}EMVBASE_IPK_RECOVER;


typedef struct
{
    unsigned char DataHead;    //'6A'
    unsigned char CertFormat;    //'04'
    unsigned char AppPAN[10];
    unsigned char ExpireDate[2];
    unsigned char CertSerial[3];
    unsigned char HashInd;
    unsigned char ICCPKAlgoInd;
    unsigned char ICCPKLen;
    unsigned char ICCPKExpLen;
    unsigned char ICCPKLeft[206];	//NI-42
    unsigned char HashResult[20];
    unsigned char DataTrail;    //'BC'
}EMVBASE_ICCPK_RECOVER;


typedef struct
{
    unsigned char DataHead;    //'6A'
    unsigned char DataFormat;    //'05'
    unsigned char HashInd;    //'01'
    unsigned char ICCDynDataLen;
    unsigned char ICCDynData[223];    //LDD	<NIC-25
    //unsigned char PadPattern[223];    //NIC-LDD-25,padded with 'BB'
    unsigned char HashResult[20];
    unsigned char DataTrail;    //'BC'
}EMVBASE_SIGN_DYNDATA_RECOVER;


typedef struct
{
    unsigned char DataHead;    //'6A'
    unsigned char DataFormat;    //'03'
    unsigned char HashInd;
    unsigned char DataAuthCode[2];
    //unsigned char PadPattern[222];    //NI-26
    unsigned char HashResult[20];
    unsigned char DataTrail;    //'BC'
}EMVBASE_SIGN_STATDATA_RECOVER;


typedef struct{
	unsigned char CertFormat;
	unsigned char IssuID[4];
	unsigned char ExpireDate[2];
	unsigned char CertSerial[3];
	unsigned char IPKSIGNAlgoInd;
	unsigned char IPKEncrptAlgoInd;
    unsigned char IPKParamInd;
	unsigned char IPKLen;
	unsigned char IPK[64];
	unsigned char DGTLSGNTR[64];
}EMVBASE_SM_IPK_CRTFCT;


typedef struct{
	unsigned char CertFormat;
	unsigned char AppPAN[10];
	unsigned char ExpireDate[2];
	unsigned char CertSerial[3];
	unsigned char ICCPKSIGNAlgoInd;
	unsigned char ICCPKEncrptAlgoInd;
	unsigned char ICCPKParamInd;
	unsigned char ICCPKLen;
	unsigned char ICCPK[64];
	unsigned char DGTLSGNTR[64];
}EMVBASE_SM_ICCPK_CRTFCT;


typedef struct {
	unsigned char DataFormat;
	unsigned char ICCDynDataLen;
	unsigned char ICCDynData[248];
	unsigned char DGTLSGNTR[64];
}EMVBASE_SM_SIGN_DYNDATA;


typedef struct {
	unsigned char DataFormat;
	unsigned char DataAuthCode[2];
	unsigned char DGTLSGNTR[64];
}EMVBASE_SM_SIGN_STATDATA;


typedef struct {
	unsigned char ICCDynNumLen;
	unsigned char ICCDynNum[8];
	unsigned char CryptInfo;
	unsigned char AppCrypt[8];
	unsigned char HashResult[32];
}EMVBASE_SM_ICC_DYN_DATA;


typedef struct
{
    unsigned char DataHead;    //'6A'
    unsigned char CertFormat;    //'04'
    unsigned char AppPAN[10];
    unsigned char ExpireDate[2];
    unsigned char CertSerial[3];
    unsigned char HashInd;
    unsigned char ICCPIN_EPKAlgoInd;
    unsigned char ICCPIN_EPKLen;
    unsigned char ICCPIN_EPKExpLen;
    unsigned char ICCPIN_EPKLeft[206];    //NI-42
    unsigned char HashResult[20];
    unsigned char DataTrail;    //'BC'
}EMVBASE_ICCPIN_EPK_RECOVER;


typedef struct
{
    unsigned char IACDenial[5];
    unsigned char IACOnline[5];
    unsigned char IACDefault[5];
}EMVBASE_Card_TAC;


typedef struct
{
    unsigned char ICCDynNumLen;
    unsigned char ICCDynNum[8];
    unsigned char CryptInfo;
    unsigned char AppCrypt[8];
    unsigned char HashResult[20];
}EMVBASE_ICC_DYN_DATA;


typedef void (*EMVBASE_ContactlessIsoCommand)(EMVBASE_APDU_SEND *ApduSend,EMVBASE_APDU_RESP *apdu_r);
typedef int (*EMVBase_termipkrevokecheck)(unsigned char *buf);
typedef int (*EMVBASE_CheckMatchTermAID)(unsigned char* aid,unsigned char aidLen,EMVBASE_TERMAPP *Applist,unsigned char* kernelid,unsigned char kernelidlen);
typedef void (*EMVBASE_ReadTermAID)(EMVBASE_TERMAPP *Applist,unsigned char *TermApplistNum);
typedef void (*EMVBASE_PrintfFunction)(char *fmt, ...);


typedef struct{
	EMVBASE_LISTAPPDATA *SelectedApp;
	EMVBASE_LISTAPPDATA *AppListCandidate;
	unsigned char AppListCandidatenum;
	unsigned char AppListCandidateMaxNum;
}EMVBase_EntryPoint;


typedef struct{
	EMVBase_EntryPoint *EntryPoint;
	EMVBASE_ContactlessIsoCommand IsoCommand;
	EMVBASE_CheckMatchTermAID CheckMatchTermAID;
	unsigned char SelectedAppNo;
	unsigned char *rapdu;	// r-apdu for ppse
	unsigned char CheckTag84;//bit0: tag_84 forced exist; bit1: tag_84 check length==14; bit2: tag_84 check data=="2PAY.SYS.DDF01" //sjz20200402 add
	unsigned char SupportExternSelect;//support 9f29 extern select aid; sjz20200408 add
	unsigned char PpseRespType;//0-д╛хо; 1-JCB
}EMVBase_UnionStruct;


typedef struct{
	unsigned char logoutputswitch;
	EMVBASE_PrintfFunction EmvBase_Printf;
}EMVBase_LogOutput;


typedef struct{
	EMVBASE_LISTAPPDATA *SelectedApp;
	EMVBASE_LISTAPPDATA *AppListCandidate;
	unsigned char AppListCandidatenum;
	EMVBASE_ContactlessIsoCommand IsoCommand;
	EMVBASE_CheckMatchTermAID CheckMatchTermAID;
	EMVBASE_ReadTermAID ReadTermAID;
	unsigned char AppListCandidateMaxNum;	//sjz
}EMVBase_TermAidlistUnionStruct;


extern EMVBASE_EntryPoint *gEmvBaseEntryPoint;
extern unsigned char EMVBase_ParseLen(unsigned char *ParseData,unsigned short index,unsigned short *DataLen);		//index not change
extern unsigned char EMVBase_ParseExtLen(unsigned char *ParseData,unsigned short *index,unsigned short *DataLen);	//index will be add,and ParseData max len is 300
extern unsigned char EMVBase_ParseExtLenWithMaxLen(unsigned char *ParseData,unsigned short *index,unsigned short *DataLen,unsigned short MaxLen);	//index will be add,and ParseData max len is MaxLen
extern void EMVBase_COMMAND_SELECT(unsigned char *DFFileName, unsigned char DFFileNameLen, unsigned char nextflag, EMVBASE_APDU_SEND *apdu_s);
extern unsigned char EMVBase_SelectPPSE(EMVBase_UnionStruct *tempEMVBase_UnionStruct);
extern unsigned char EMVBase_ChooseApp(EMVBase_UnionStruct *tempEMVBase_UnionStruct);
extern unsigned char EMVBase_RebuildApplist(EMVBase_UnionStruct *tempEMVBase_UnionStruct);
extern void EMVBase_GetLibVersion(unsigned char *version);
extern unsigned char EMVBase_ParseExtLen(unsigned char *ParseData,unsigned short *index,unsigned short *DataLen);
extern void EMVBase_Trace(char *fmt, ...);
extern void  EMVBase_TraceHex(unsigned char const* pasInfo, void const *pheData, int siLen);
extern unsigned char EMVBase_SelectFromTerm(EMVBase_TermAidlistUnionStruct *temp_UnionStruct);
extern unsigned char EMVBase_ChooseTermApp(EMVBase_TermAidlistUnionStruct *temp_UnionStruct);
extern void EMVBase_FillICCPK(unsigned char* ICCPKData, unsigned char tempIPKModulLen, EMVBASE_ICCPK_RECOVER* recovICCPK);
extern void EMVBase_SetLogOutput(EMVBase_LogOutput *pstLogOutput);


#endif

