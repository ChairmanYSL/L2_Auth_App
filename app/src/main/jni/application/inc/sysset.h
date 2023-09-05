#ifndef AFX_SYSSET_H
#define AFX_SYSSET_H

#include "sdktypedef.h"

#define TV_AUTORET 60000

struct _INPUTMAP
{
    u8 asTag[128];                                                         //输入数据tag // wuzhonglin, 2012/12/22
    bool bIsJudge;                                                        //判断长度不足
    u8 ucMinLen;                                                          //最小长度限制
    u8 ucMaxLen;                                                          //最大长度限制
    u32 uiMask;                                                           //命令字
    u8 ucRowNo;                                                           //回显行号(0-4)
};

#define MAXSIMDATALEN  512

typedef struct
{
    unsigned char MerchantEName[41];                           //商户英文名称
    unsigned char MerchantCName[41];                       //商户中文名称
    unsigned char TerminalNo[9];        //终端号
    unsigned char MerchantNo[19];       //商户号
#if 0 /*Modify by luohuidong at 2018.02.28  11:58 */
    unsigned char K360Printer;
#else
    unsigned char BisSupportPrint;//打印开关
#endif /* if 0 */
    unsigned char IfnNo[9];                  //机身号
    unsigned char BisSupportES;              //机器类型

    /*=======BEGIN: sujianzhong 2015.06.03  19:8 modify===========*/
    unsigned char AppType; //TermInfo.AppType
    unsigned char TransType; //TermInfo.TransType
    unsigned char bForceOnline;      //TermInfo.bForceOnline
    unsigned char bBatchCapture;      //TermInfo.bBatchCapture
    unsigned char ODAalgrthFlag; //TermInfo.ODAalgrthFlag
    unsigned char SupportTermCheck;
    /*====================== END======================== */
	unsigned char ManualCashFlag; //Manual Cash Processing Restrict  zwb 170307
	unsigned char CashBackFlag; //CashBack Processing Restrict

    unsigned char VocherNo[4];          //流水号
    unsigned short AutoToolDispTimeMs; //luohuidong 2018.09.03 13:43
	unsigned char JcbImplementationOption;//kernel option for jcb emv mode/oda/issuer/except file
	unsigned char BisperformanceCAPK;
	unsigned char TransCurrencyExponent;//tag5F36--add by cj
	unsigned char TermCountryCode[2];
	unsigned char AcquirerID[6];
	unsigned char IFD[8];
	unsigned char CategoryCode[2];
	unsigned char MerchantID[15];
	unsigned char TermType;
	unsigned char TransCurrencyCode[2];
	int TCPPort;
	unsigned char IPAddress[16];
	unsigned char RestrictAID[16];	//for Implemention 5
	unsigned char RestrictAIDLen;
}_SimData;  //最大长度MAXSIMDATALEN,该标签用来设置IC卡tag

EXTERN unsigned char gRupayPRMissKey[16];

typedef struct
{
    unsigned char serviceid[2];
    unsigned char keyindex;
    unsigned char key[8];
}RUPAY_PRMACQKEY;

EXTERN RUPAY_PRMACQKEY gstRupayPRMacq[10];

EXTERN u8 gtesttorn;

#define Msg_EMV_StartTrade    16
#define Msg_EMV_Disp_OneKey     17
#define Msg_EMV_Disp_InputStr     18

typedef struct
{
	unsigned char AidLen;
    unsigned char Aid[16];
	unsigned char TransType;
//    unsigned char TransCurcyCode[2];             //tag'5F2A'
//    unsigned char TransCurcyExp;                 //tag'5F36'
//    unsigned char TransReferCurcyCode[2];        //tag'9F3C'
//    unsigned char TransReferCurcyExp;            //tag'9F3D'
//    unsigned char AcquireID[6];                  //tag'9F01'
//    unsigned char TermID[8];                     //tag'9F1C'
//    unsigned char MerchCateCode[2];              //tag'9F15'
//    unsigned char MerchID[15];                   //tag'9F16'
//    unsigned char MerchantNameLen;
//    unsigned char MerchantName[20];				//tag'9F4E'
//    unsigned char TermTDOLLen;
//    unsigned char TermTDOL[64];
//    unsigned char TermTransPredicable[4];		//tag'9F66'
//	unsigned char terminalcapability[3];		//tag'9F33'
//    unsigned char terminaltype;
	unsigned char RemovalTimeout[2];
	unsigned char Implementation;
	unsigned char ZeroAmtAllowFlag;		//0-:allow
	unsigned char StatusCheckFlag;		//0-:not check
	unsigned char CLAppCap[5];
	unsigned char ATOL[64];		//Additional Tag Object List
	unsigned char ATOLLen;
	unsigned char MTOL[64];		//Mandatory Tag Object List
	unsigned char MTOLLen;
//	unsigned char ATDTOL[64];	//Authentication Transaction Data Tag Object List
//	unsigned char ATDTOLLen;
//	unsigned char TransData[256];	//9F76
//	unsigned char TransDataLen;
}APPEX_AID_STRUCT;

EXTERN APPEX_AID_STRUCT appex_aid_list[10];

extern void ReadAPPEXAID();
extern void InitAPPEXAID();
extern void SaveAPPEXAID();
extern void AddAPPEXAID(APPEX_AID_STRUCT *extempAid);
extern s32 IccSetAIDEX();



#endif

