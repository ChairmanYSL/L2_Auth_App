#ifndef SDKEMVBASEPRIVATE_H
#define SDKEMVBASEPRIVATE_H

#define SDK_EMVBASE_MAX_PATH 64
#include "sdkemvbase.h"

typedef int (*EMVBase_SetOutcome)(unsigned char Result, unsigned char Start, unsigned char CVM, unsigned char UIRequestonOutcomePresent, unsigned char UIRequestonRestartPresent, unsigned char DataRecordPresent, unsigned char DiscretionaryDataPresent, unsigned char AlternateInterfacePreference, unsigned char Receipt, unsigned char FieldOffRequest, unsigned char *RemovalTimeout, unsigned char OnlineResponseData);
typedef int (*EMVBase_SetUIRequest)(unsigned char MessageID, unsigned char Status, unsigned char HoldTime, unsigned char *LanguagePerference, unsigned char ValueQualifier, unsigned char *Value, unsigned char *CurrencyCode);
typedef void (*EMVBase_SendOutcome)();
typedef void (*EMVBase_SendUIRequest)(int type);

typedef struct
{
	int (*InputPWD)(unsigned char ucIccEncryptWay, unsigned char ucPINTryCount, unsigned char *CreditPwd);//SDK_OK:正常输密;SDK_PED_NOPIN:bypass; SDK_ERR:失败
	EMVBase_termipkrevokecheck termipkrevokecheck;//SDK_OK:不是回收公钥; SDK_ERR:是回收公钥
	unsigned char SupportExternSelect;//support 9f29 extern select aid; sjz20200408 add
	unsigned char PpseRespType;//0-默认; 1-JCB
	EMVBase_SetOutcome setOutcome;
	EMVBase_SetUIRequest setUIReq;
	EMVBase_SendOutcome sendOutcome;
	EMVBase_SendUIRequest sendUIReq;
}SDK_EMVBASE_TRADE_PARAM;


extern EMVBase_UnionStruct *gstEMVBase_UnionStruct;
extern EMVBase_EntryPoint *gstEMVBase_EntryPoint;
extern SDK_EMVBASE_TRADE_PARAM *gstEMVBase_TradeParam;
extern unsigned char gAppListCandicateMaxNum;//默认8个

extern unsigned char gstemvbaseforL2TEST;		//for L2 test if anything not same as commercial
extern unsigned char gstemvbaseneedsign;		//cvm need sign
extern unsigned char gstemvbaseneedonlinepin;
extern SDK_EMVBASE_CVM_RESULT gstemvbaseCVMresult;

extern int sdkEMVBaseRltToSdkRlt(unsigned char ucInBuf, int *puiOutBuf);
extern int sdkEMVBase_CheckMatchTermAID_CL(unsigned char* aid,unsigned char aidLen,EMVBASE_TERMAPP *Applist,unsigned char* kernelid,unsigned char kernelidlen);
extern int sdkEMVBase_CheckMatchTermAID_CT(unsigned char* aid,unsigned char aidLen,EMVBASE_TERMAPP *Applist,unsigned char* kernelid,unsigned char kernelidlen);
extern void sdkEMVBase_ReadTermAID_CL(EMVBASE_TERMAPP * Applist, unsigned char *TermApplistNum);
extern void sdkEMVBase_ReadTermAID_CT(EMVBASE_TERMAPP * Applist, unsigned char *TermApplistNum);
extern void sdkDev_Printf(char *fmt, ...);
extern void sdkEMVBase_ReadCAPK(unsigned char *RID, unsigned char capki, EMVBASE_CAPK_STRUCT * tmpcapk);

#endif
