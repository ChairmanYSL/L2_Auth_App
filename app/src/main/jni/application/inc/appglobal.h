#ifndef AFX_GLOBAL_H
#define AFX_GLOBAL_H


#ifdef AFX_APP_VARIABLES
#define EXTERN
#else
#define EXTERN extern
#endif


#include "host.h"	//2021.9.2 lishiyao add for JCB of BCTC New Host
#include "sdkoutcome.h"

#include "sysset.h"
#include "trade.h"

#include "dllemvbasetagbaselib.h"
#include "dllemvbase.h"
#include "sdkemvbase.h"
#include "dispmessage.h"

#include "dllpure.h"
#include "withpinpad.h"
#include "sdktypedef.h"
#include "sdkkey.h"
#include "sdkDisp.h"
#include "sdkmmi.h"
#include "sdktools.h"
#include "sdkGlobal.h"
#include "sdkmaths.h"

#define     SDK_ICC_CARDNO_MIN_NUM  13          //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î»ï¿½ï¿½//the minimum number of card number
#define     SDK_ICC_TIMER_AUTO    300000         //ï¿½Ô¶ï¿½ï¿½ï¿½ï¿½Ø¶ï¿½Ê±ï¿½ï¿½30s   //automatically return timer 30s
#define     SDK_ICC_TIMER_RESET   2000          // ï¿½Ç½Ó¸ï¿½Î»Ê±ï¿½ï¿½2s  //contactless reset time 2s

#define	HOST_TRANS_SERIAL 0x01
#define HOST_TRANS_WIFI   0x02


#define MAXTRANSLOGNUM 40
EXTERN u32 TransNum;
EXTERN u8 TransIndex;

EXTERN unsigned long  AmtTrans;
EXTERN unsigned long  AmtNet;                 //total accumulative amount for reconciliation.
EXTERN unsigned long  BatchTransNum;	//number of trans stored in terminal.used for reconciliation
EXTERN BATCHRECORD TermBatchRecord[1];
EXTERN unsigned int TermBatchRecordNum;

EXTERN u8 gF1Amount[12 + 1];//f1

EXTERN u8 gstasAmount[12 + 1];
EXTERN u8 gbcOtherAmount[6];//sujianzhong 2016.09.28 10:24
EXTERN u8 TermInfobPrintReceipt;
EXTERN u8 TermInfobReversal;

EXTERN u8 gDispSeePhone;//zwb 170509
EXTERN u8 gDispSecondTap;//zwb 170509

EXTERN unsigned char gJCBPaperNum;

EXTERN u8 gCollisionflag;	//¼ì¿¨³åÍ»±êÖ¾Î»
EXTERN u8 gCollisionCounter;	//¼ì¿¨³åÍ»¼ÆÊýÆ÷
EXTERN u8 gTransCurrExponent;
EXTERN int gSerialPortId;
EXTERN int gTCPPort;
EXTERN u8 gTCPAddress[16];
EXTERN u8 gHostTransType;

#endif
