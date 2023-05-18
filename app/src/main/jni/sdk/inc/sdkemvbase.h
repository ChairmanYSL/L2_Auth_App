#ifndef SDKEMVBASE_H
#define SDKEMVBASE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#include "sdktypedef.h"

#ifndef SDK_EMVBASE_EMV_OK_BASE
#define SDK_EMVBASE_EMV_OK_BASE (300)
#endif

#ifndef SDK_EMVBASE_EMV_ERR_BASE
#define SDK_EMVBASE_EMV_ERR_BASE (-300)
#endif

//typedef enum
//{
//    EMV_REQ_SET_BEFOREGPO = 10,                         //!< 请求选择候选应用列表
//    EMV_REQ_SEL_CANDIDATES_AGAIN = 4,                   //!< 请求再次选择候选应用列表
//    EMV_REQ_DISPLAY_REMOVECARD = 8,                     //!< 提示用户移卡
//    EMV_REQ_ONLINE_PIN = 14,                            //!< 请求持卡人输入联机PIN
//    EMV_REQ_OFFLINE_PIN = 15,                           //!< 请求持卡人输入脱机PIN
//    EMV_REQ_OFFLINE_PIN_LAST = 16,                      //!< 请求持卡人输入脱机PIN（最后一次）
//    EMV_REQ_CONFIRM_BYPASS_PIN = 17,                    //!< 请求确认是否跳过PIN输入
//    EMV_REQ_GO_ONLINE = 22,                             //!< 请求联机
//	EMV_REQ_TORN_RESTARTTRANS = 23,						//!<发生闪卡交易,请求重走流程
//}SDK_SZZT_TRANS_STATUS;

typedef enum
{
	SDK_EMV_ERR_KERUNINIT = -100,		//内核未初始化
	SDK_EMV_ERR_APPSELECT = -101, 		//应用选择流程失败
	SDK_EMV_ERR_INITAPP = -102,			//应用初始化失败
	SDK_EMV_ERR_READRECORD = -103,		//读应用数据失败
	SDK_EMV_ERR_TERMRISKMANAGE = -104,	//终端风险管理失败
	SDK_EMV_ERR_PROCESSRESTRICT = -105,	//处理限制失败
	SDK_EMV_ERR_TERMACTANYALIZE = 106,	//终端行为分析失败
	SDK_EMV_ERR_ODA = -107,				//脱机数据认证失败
	SDK_EMV_ERR_CVM = -108,				//持卡人验证失败
}SDK_SZZT_TRANS_ERR;

typedef enum
{
	EMV_TRANSRESULT_OFFLINEACCEPT = 101, 				//!< 交易脱机接受
	EMV_TRANSRESULT_OFFLINEDECLINE = 102, 				//!< 交易脱机拒绝
	EMV_TRANSRESULT_ONLINEACCEPT = 103,					//!< 交易联机接受
	EMV_TRANSRESULT_ONLINEDECLINE = 104,				//!< 交易联机拒绝
	EMV_TRANSRESULT_TERMINATE = 105,					//!<交易终止
	EMV_SW_INTERFACE = 106, 							//!< 交易转界面
}SDK_SZZT_TRANS_RESULT;

typedef enum
{
	SDK_EMV_UseOtherCard =          (SDK_EMVBASE_EMV_ERR_BASE - 31),	//使用其他卡片
	SDK_EMV_ENDAPPLICATION = 		(SDK_EMVBASE_EMV_ERR_BASE - 30),	//交易中止
    SDK_EMV_TORN =  				(SDK_EMVBASE_EMV_ERR_BASE - 29),	//检测到闪卡交易
    SDK_EMV_TransTryAgain =  		(SDK_EMVBASE_EMV_ERR_BASE - 28),	//重新发起交易
	SDK_EMV_UserSelect =            (SDK_EMVBASE_EMV_ERR_BASE - 27),	//用户手动选择应用
    SDK_EMV_ReadLogEnd =            (SDK_EMVBASE_EMV_ERR_BASE - 26),	//读取交易log，仅用于EMV接触
    SDK_EMV_SeePhone =          	(SDK_EMVBASE_EMV_ERR_BASE - 25),	//see phone，用于GPO返回6985或持卡人验证走CDCVM
    SDK_EMV_CardInException =       (SDK_EMVBASE_EMV_ERR_BASE - 24),	//卡号出现在异常文件中（黑名单）
	SDK_EMV_PPSERev6A82 = 			(SDK_EMVBASE_EMV_ERR_BASE - 23),	//PPSE返回6A82，仅用于美运amex
	SDK_EMV_PPSENotFound =       	(SDK_EMVBASE_EMV_ERR_BASE - 22),	//PPSE返回空列表
	SDK_EMV_NoLog =              	(SDK_EMVBASE_EMV_ERR_BASE - 21),	//没有交易log可读，仅用于EMV接触
    SDK_EMV_FixedAidAbsent =     	(SDK_EMVBASE_EMV_ERR_BASE - 20),	//混合AID缺失
    SDK_EMV_SwitchInterface =		(SDK_EMVBASE_EMV_ERR_BASE - 19),	//交易要求转界面
    SDK_EMV_ReadCardAgain =   		(SDK_EMVBASE_EMV_ERR_BASE - 18),	//重新读卡
    SDK_EMV_AppSelectTryAgain =  	(SDK_EMVBASE_EMV_ERR_BASE - 17),	//重新选择应用
    SDK_EMV_InitAppNotAccept =   	(SDK_EMVBASE_EMV_ERR_BASE - 16),	//一般用于GPO返回6986
    SDK_EMV_AppBlock =           	(SDK_EMVBASE_EMV_ERR_BASE - 15),	//应用被锁定
    SDK_EMV_NeedMagCard =        	(SDK_EMVBASE_EMV_ERR_BASE - 14),	//交易走磁条通道
    SDK_EMV_AppTimeOut =         	(SDK_EMVBASE_EMV_ERR_BASE - 13),	//APDU交互超时
    SDK_EMV_CardBlock =          	(SDK_EMVBASE_EMV_ERR_BASE - 12),	//卡片被锁定
    SDK_EMV_CancelTrans =        	(SDK_EMVBASE_EMV_ERR_BASE - 11),	//用户主动取消交易
    SDK_EMV_NotAccept =          	(SDK_EMVBASE_EMV_ERR_BASE - 10),	//
    SDK_EMV_IccDataRedund =      	(SDK_EMVBASE_EMV_ERR_BASE - 9),		//卡片返回数据冗余
    SDK_EMV_NoAppSel =           	(SDK_EMVBASE_EMV_ERR_BASE - 8),		//应用列表为空，无应用可选
    SDK_EMV_IccReturnErr =          (SDK_EMVBASE_EMV_ERR_BASE - 7),		//卡片返回数据错误
    SDK_EMV_IccCommandErr =         (SDK_EMVBASE_EMV_ERR_BASE - 6),		//APDU命令格式错误
    SDK_EMV_IccDataFormatErr =      (SDK_EMVBASE_EMV_ERR_BASE - 5),		//卡片返回数据格式错误
    SDK_EMV_TransNotAccept = 		(SDK_EMVBASE_EMV_ERR_BASE - 4),		//一般用于GAC返回6986
	SDK_EMV_TransTerminate =	   	(SDK_EMVBASE_EMV_ERR_BASE - 3),		//流程终止
    SDK_EMV_TransOnlineDecline =  	(SDK_EMVBASE_EMV_ERR_BASE - 2),		//联机拒绝
    SDK_EMV_TransOfflineDecline = 	(SDK_EMVBASE_EMV_ERR_BASE - 1),		//脱机拒绝


    SDK_EMV_TransOfflineApprove =   (SDK_EMVBASE_EMV_OK_BASE + 1),		//脱机批准
    SDK_EMV_TransOnlineApprove =    (SDK_EMVBASE_EMV_OK_BASE + 2),		//联机批准
    SDK_EMV_TransOnlineWait =       (SDK_EMVBASE_EMV_OK_BASE + 3),		//交易需要二次联机
    SDK_EMV_TransReadAppDataComplete = (SDK_EMVBASE_EMV_OK_BASE + 4),	//简易流程完成，仅用于客户测试用
    //max up to SDK_EMVBASE_EMV_OK_BASE + 50
}SDK_EMVBASE_RLT;



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
}SDK_EMVBASE_AID_STRUCT;

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
}SDK_EMVBASE_CAPK_STRUCT;


typedef struct{
	unsigned char aidlen;
	unsigned char aid[16];
	unsigned char kernelidlen;
	unsigned char kernelid[8];
}SDK_EMVBASE_CL_HIGHESTAID;


typedef struct
{
    unsigned short LenOut;
    unsigned char DataOut[256];
    unsigned char SW1;
    unsigned char SW2;
}SDK_EMVBASE_LOGDATA;


typedef struct
{
    unsigned char ucAidLen;
    unsigned char asAid[32];    //5-16
    unsigned char ucAppLabelLen;
    unsigned char asAppLabel[32];
    unsigned char ucPreferNameLen;
    unsigned char asPreferName[32];
    unsigned char ucPriority;     //tag'87'
    unsigned char ucLangPreferLen;
    unsigned char asLangPrefer[16];    //2-8
    unsigned char ucIcti;    	 //Issuer Code Table Index.lang used for display app list according to ISO8859.but not include Chinese,Korea,etc.
    unsigned char ucLocalName;   //If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char ucAppLocalNameLen;
    unsigned char asAppLocalName[32];
}SDK_EMVBASE_C_AIDLIST;


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
    unsigned char AidInTermLen;
    unsigned char AidInTerm[16];
    unsigned char KernelIdentifierLen;
    unsigned char KernelIdentifier[8];
}SDK_EMVBASE_CL_AIDLIST;


typedef enum
{
	SDKEMVBASE_CVM_NA=0,
	SDKEMVBASE_CVM_NOCVMREQ,
	SDKEMVBASE_CVM_OBTAINSIGNATURE,
	SDKEMVBASE_CVM_ONLINEPIN,
	SDKEMVBASE_CVM_CONFVERIFIED,
	SDKEMVBASE_CVM_MOBILE,
	SDKEMVBASE_CVM_PLAINTEXTPIN,
	SDKEMVBASE_CVM_ENCIPHEREDPIN,
 	SDKEMVBASE_CVM_PLAINTEXTPIN_AND_SIGN,
	SDKEMVBASE_CVM_ENCIPHEREDPIN_AND_SIGN,
	SDKEMVBASE_CVM_SKIPCVM,
}SDK_EMVBASE_CVM_RESULT;

extern s32 sdkemvbaseInitDDI(void);
extern void sdkEMVBase_SetEMVLogOutput(bool enable);

extern s32 sdkEMVBasePowerStartInit(void);
extern s32 sdkEMVBaseDelOneAID(const u8 *pheInAID, s32 siAIDLen);
extern s32 sdkEMVBaseDelAllAIDLists(void);
extern s32 sdkEMVBaseAddAnyAIDList(const SDK_EMVBASE_AID_STRUCT *pstInAIDList, s32 siAIDNum);
extern s32 sdkEMVBaseGetAnyAIDList(s32 siStartIndex, s32 siAIDNum, SDK_EMVBASE_AID_STRUCT *pstOutAIDList, s32 *psiOutAIDNum);
extern s32 sdkEMVBaseGetAIDListNum(s32 *psiInAIDListNum);
extern s32 sdkEMVbaseGetMatchAID(const u8 *pheInAID, s32 siAIDLen, SDK_EMVBASE_AID_STRUCT *pstOutAID,s32 MaxOutAIDNum,s32 *psiOutAIDNum);

extern s32 sdkEMVBaseDelOneCAPK(const u8 *pheInRID, s32 siCAPKIndex);
extern s32 sdkEMVBaseDelAllCAPKLists(void);
extern s32 sdkEMVBaseAddAnyCAPKList(const SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList, s32 siCAPKNum);
extern s32 sdkEMVBaseGetAnyCAPKList(s32 siStartIndex, s32 siCAPKNum, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPKList, s32 *psiOutCAPKNum);
extern s32 sdkEMVBaseGetCAPKListNum(s32 *psiInCAPKListNum);
extern s32 sdkEMVBaseGetMatchCAPK(const u8 *pheInRID, s32 uiCAPKIndex, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPK);

extern s32 sdkEMVBaseDelOneGMCAPK(const u8 *pheInRID, s32 siCAPKIndex);
extern void sdkEMVBaseDelAllGMCAPKLists(void);
extern s32 sdkEMVBaseAddAnyGMCAPKList(const SDK_EMVBASE_CAPK_STRUCT *pstInCAPKList, s32 siCAPKNum);
extern s32 sdkEMVBaseGetAnyGMCAPKList(s32 siStartIndex, s32 siCAPKNum, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPKList, s32 *psiOutCAPKNum);
extern s32 sdkEMVBaseGetGMCAPKListNum(s32 *psiInCAPKListNum);
extern s32 sdkEMVBaseGetMatchGMCAPK(const u8 *pheInRID, s32 uiCAPKIndex, SDK_EMVBASE_CAPK_STRUCT *pstOutCAPK);
extern bool sdkEMVBaseCheckCAPKExit(u8 *RID, u8 capki);

extern s32 sdkEMVBaseTransInit();
extern s32 sdkEMVBaseEntryPoint(SDK_EMVBASE_CL_HIGHESTAID *tempHighestAID);

extern s32 sdkEMVBaseConfigTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen);
extern s32 sdkEMVBaseReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen);
extern bool sdkEMVBaseCheckTagExit(u8 *tag);
extern s32 sdkEMVBaseDelTLV(const u8 *pheTag);

extern s32 sdkEMVBaseSetInputPINFun(s32(*fun_inputpin)(u8 ucIccEncryptWay, u8 ucPINTryCount, u8 *pheCreditPwd));
extern s32 sdkEMVBaseSetSpTermCheck(bool enable);		//default is false
extern s32 sdkEMVBaseGetScriptResult(s32 * psiScriptResultLen, u8 * pheScriptResult);
extern bool sdkEMVBaseNeedSignature();
extern s32 sdkEMVBaseGetEntryPointAIDList(SDK_EMVBASE_CL_AIDLIST *entrypointlist ,s32 silistnum ,s32 *psiOutlistnum);		//must call after sdkEMVBaseEntryPoint
extern s32 sdkEMVBaseSetUserSelcetFixedAid(u8 *aidincard, u8 aidlen);	//must call after sdkEMVBaseEntryPoint

extern s32 sdkEMVBaseSetRevocationKey(s32(*fun_cmprevocation)(u8 *key));

extern s32 sdkEMVBaseReSelectApp(SDK_EMVBASE_CL_HIGHESTAID *tempHighestAID);

extern s32 sdkEMVBaseSetTransAmount(u8 *bcdAmount);
extern s32 sdkEMVBaseSetTwoTransAmount(u8 *bcdAmount, u8 *bcdOtherAmount);
extern s32 sdkEMVBasePreTrans();
extern s32 sdkEMVBasePreTransGetTransType();

extern s32 sdkEMVBaseSelectFromTermSelect(u8 *aidnum);
extern s32 sdkEMVBaseGetFromTermSelectAIDList(SDK_EMVBASE_CL_AIDLIST *entrypointlist ,s32 silistnum ,s32 *psiOutlistnum);

extern SDK_EMVBASE_CVM_RESULT sdkEMVBaseGetCVMresult(void);

extern s32 sdkEMVBaseSetSupportExternSelect(u8 flag);


extern s32 sdkEMVBaseSetAppListCandicateMaxNum(u8 maxnum);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

