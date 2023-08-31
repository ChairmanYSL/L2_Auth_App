#ifndef AFX_HOST_H
#define AFX_HOST_H

//unsigned char gstTransType;
//unsigned char gstAutoTest;
//unsigned char gstAutoTime;
//unsigned char gstHostTimeout;

EXTERN unsigned char gstAutoTest;
EXTERN unsigned char gstAutoTime;
EXTERN unsigned char gstHostTimeout;


#if 0
#define Msg_EMV_StartTrade    16
#define Msg_EMV_Disp_OneKey     17
#define Msg_EMV_Disp_InputStr     18
#define Msg_EMV_Print     19 // Note


//bctc host
#define REC_CAPKRQ            0xC2
#define REC_CAPKRP            0x82
#define REC_TERMAPPRQ         0xC3
#define REC_TERMAPPRP         0x83
#define REC_TERMINFORQ        0xC4
#define REC_TERMINFORP        0x84
#define REC_EXCEPTFILERQ      0xC5
#define REC_EXCEPTFILERP      0x85
#define REC_IPKREVOKERQ       0xC6
#define REC_IPKREVOKERP       0x86
#define REC_PRINTRQ       	  0xC7
#define REC_PRINTRP           0x87

#define REC_CONFIGRQ			0xD8	//20190605 for nexgo terminal use

#define BCTCMsg_EMV_FinaRQ			0x41
#define BCTCMsg_EMV_FinaRP          1
#define BCTCMsg_EMV_AuthRQ          0x42
#define BCTCMsg_EMV_AuthRP			2
#define BCTCMsg_EMV_ConfirmRQ		0x43
#define BCTCMsg_EMV_ConfirmRP       3
#define BCTCMsg_EMV_ReconciRQ		0x44
#define BCTCMsg_EMV_ReconciRP		4
#define BCTCMsg_EMV_ReversalRQ      0x46
#define BCTCMsg_EMV_ReversalRP      6

#define BCTCMsg_EMV_BatchOnlineApprove    	0x44
#define BCTCMsg_EMV_BatchOnlineApproveRP  	4
#define BCTCMsg_EMV_OnlineAdvice			0x45
#define BCTCMsg_EMV_OnlineAdviceRP			5
#define BCTCMsg_EMV_TransResultRQ   		0xC1
#define BCTCMsg_EMV_TransResultRP   		0x81


#define BCTCMsg_EMV_StartTrade 			0xc0
#define BCTCMsg_EMV_StartTradeRP 		0x80
#endif


//BCTC Host for JCB
//管理类报文
#define BCTC_MNG_StartTrade_SEND		0xC0
#define BCTC_MNG_TransResult_SEND		0xC1
#define BCTC_MNG_DownloadCAPK_SEND		0xC2
#define BCTC_MNG_DownloadAID_SEND		0xC3
#define BCTC_MNG_DownloadTermInfo_SEND	0xC4
#define BCTC_MNG_DownloadBlackList_SEND	0xC5
#define BCTC_MNG_DownloadRevocPK_SEND	0xC6
#define BCTC_MNG_UploadEcStrip_SEND		0xC7
#define BCTC_MNG_DownloadDRL_SEND		0xC8
#define BCTC_MNG_TermDispUI_SEND		0xC9

#define BCTC_MNG_StartTrade_RECV		0x80
#define BCTC_MNG_TransResult_RECV		0x81
#define BCTC_MNG_DownloadCAPK_RECV		0x82
#define BCTC_MNG_DownloadAID_RECV		0x83
#define BCTC_MNG_DownloadTermInfo_RECV	0x84
#define BCTC_MNG_DownloadBlackList_RECV	0x85
#define BCTC_MNG_DownloadRevocPK_RECV	0x86
#define BCTC_MNG_UploadEcStrip_RECV		0x87
#define BCTC_MNG_DownloadDRL_RECV		0x88
#define BCTC_MNG_TermDispUI_RECV		0x89

//交易类报文
#define BCTC_TRS_FinReq_SEND			0x41
#define BCTC_TRS_AuthReq_SEND			0x42
#define BCTC_TRS_FinReqConfirm_SEND		0x43
#define BCTC_TRS_BatchUp_SEND			0x44
#define BCTC_TRS_Notify_SEND			0x45
#define BCTC_TRS_Reversal_SEND			0x46
#define BCTC_TRS_UploadFailStream_SEND	0x47

#define BCTC_TRS_FinReq_RECV			0x01
#define BCTC_TRS_AuthReq_RECV			0x02
#define BCTC_TRS_FinReqConfirm_RECV		0x03
#define BCTC_TRS_BatchUp_RECV			0x04
#define BCTC_TRS_Notify_RECV			0x05
#define BCTC_TRS_Reversal_RECV			0x06
#define BCTC_TRS_UploadFailStream_RECV	0x07


typedef struct
{
    unsigned char amountexist;
    unsigned char amount[6];
    unsigned char otheramountexist;
    unsigned char otheramount[6];
    unsigned char typeexist;
    unsigned char transtype;
	unsigned char currcodeexist;
	unsigned char currencycode[2];
	unsigned char currexpexist;
	unsigned char currexp;
}BCTC_AUTOTRADE;

EXTERN BCTC_AUTOTRADE gstbctcautotrade;

typedef struct
{
    unsigned char panlen;
    unsigned char pan[10];      //BCD
}BCTC_BLACK;

EXTERN BCTC_BLACK gstbctcblack[2];


typedef struct
{
    unsigned char RID[5];
    unsigned char index;
    unsigned char sn[3];
}BCTC_PKRECOVA;

EXTERN BCTC_PKRECOVA gstbctcpkrecova[2];

EXTERN unsigned char gstbctcupdatetemplate[850];
EXTERN unsigned short gbctcupdatetemplatelen;
EXTERN unsigned char gstbctcreadtemplate[850];
EXTERN unsigned short gbctcreadtemplatelen;

extern void SaveBlackData();
extern void ReadBlackData();
extern void SaveRecovaData();
extern void ReadRecovaData();
extern signed int BCTCSendAuthorRQ();
extern signed int BCTCSendOnlineFina();
extern signed int BCTCSendConfirm(void);
extern signed int BCTCProcessAdvice();
extern signed int BCTCSendReversal();
extern signed int BCTCSendTransResult(signed int ret);
extern void BCTCSendOutCome(void);
extern void BCTCSendUIRequest(int type);
#endif

