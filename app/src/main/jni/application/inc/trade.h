#ifndef AFX_TRADE_H
#define AFX_TRADE_H

#include "sdktypedef.h"

#define SALE         GOODS

#define EMVTYPE   0
#define PBOCTYPE  1
#define qPBOCTYPE 2
#define MSDTYPE   3
#define CLPBOCTYPE 4            //hhl 20111204 for CL_PBOC
#define PAYWAVETYPE 5
#define RUPAYTYPE 6 //印度rupay//sujianzhong 2017.12.12 14:2


#define ICCONLINEPIN                            0                                       //IC卡请求联机PIN
#define ICCOFFLINEPIN                           1                                       //IC卡请求离线PIN
#define APPNUM                                  10                                                              //最大应用个数(受Daemon及系统限制,最大不能超过30)
#define TPK_STORENO                                     0x00                                                                    //存储TPK在密码键盘的编号

#define MV_CardIdentifyInfo     70  //2013-04-10
#define MV_FormFactIndicator    172
#define MV_CustomerExclusiveData 173

#define MAG_LAST_FAIL_IC        0x92
#define MAG_NO_IC                               0x90
#define MAG_LAST_SUCCESS_IC             0x91

#define APP_OPS_STATUS_APPROVED   1
#define APP_OPS_STATUS_ONLINE     2
#define APP_OPS_STATUS_ONLINE_TWOPRESENTMENTS		3
#define APP_OPS_STATUS_ONLINE_PREMENTANDHOLD		4
#define APP_OPS_STATUS_DECLINED   5
#define APP_OPS_STATUS_TRYANOTHERINTERFACE     6
#define APP_OPS_STATUS_ENDAPPLICATION 	7
#define APP_OPS_STATUS_ENDAPPLICATION_COMMUNICATIONERROR 	8
#define APP_OPS_STATUS_ENDAPPLICATION_ONDEVICECVM 	9
#define APP_OPS_STATUS_SELECTNEXT 		0x0A
#define APP_OPS_STATUS_NA 		0x0F

typedef struct
{
    unsigned short RecordLen;
    unsigned char RecordData[1022];
}BATCHRECORD;

u8 GetBatchCapture();
bool IsEcTrade();
void sdkTestIccDispText(u8        *Text);

typedef struct
{
    unsigned long TransAmt;             /* Transaction amount */
    unsigned short TransCount;     /* Transaction Sequence Counter. inc by 1 for each trans*/
    unsigned char TransType;         /* Transaction type BCD */
    unsigned char TSI[2];                /* Transaction Status Information */
    unsigned char PANLen;
    unsigned char PAN[10];           /* Primary account No. */
    unsigned char PANSeq;                /* EMVTest: sequence num with same PAN */
    unsigned char TransDate[3];      /* Trancaction Date(Form:"YY/MM/DD") */
    unsigned char TransTime[3];      /* Trancaction Time(Form:"HH/MM/SS") */
} L2_TRANS_LOG;

typedef struct
{

    u8 POSEntryMode;
    u8 ucCardLen;
    u8 HexCardNO[10];
    u8 HexExpiredDate[3];
	u8 TradeTime[6];
}MAGTRADECARDDATA;			//2021.9.1 lishiyao add for New Bctc Host

EXTERN MAGTRADECARDDATA gstmagcarddata;		//2021.9.1 lishiyao add for New Bctc Host
//2021.9.1 lishiyao add for New Bctc Host
EXTERN u8 gstResponseCode[2];		// tag 8A
EXTERN u16 gstheIssuerAuthDataLen;
EXTERN u8 gstheIssuerAuthData[16];	// tag 91
EXTERN u16 gstheIssuerScriptLen;
EXTERN u8 gstheIssuerScriptData[256];	// tag 91
EXTERN u8 gstheIssuerAuthCode[6];	// tag 89
//
extern void sdkTestIccUserTransInitCb();
extern s32 sdkTestReadCardDisp();
extern s32 ReadCardDisp();
extern s32 IccReadCard();
extern s32 AppqPBOCPreTrans();
extern u8 OnlineEncPIN(u8 *pCreditPwd);
void IccDispText(u8     *Text);

extern u8 	EMVB_ZipPagData(u8 TradeType,u8 *DOLData,u32 *DataLen,u8 TradeOkButDecline,u8 SrciptResult,u8 usedtype);
extern void sdkSendUIRequestWhenMoreCard(u8 MassafeId, u8 stauts);

#endif

