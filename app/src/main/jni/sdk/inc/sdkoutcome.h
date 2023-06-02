#ifndef SDKOUTCOME_H
#define SDKOUTCOME_H

#include "sdktypedef.h"

//these defination come from BCTC autotest host,can refer to its manual
#define SDK_OUTCOME_ONLINERESPDATA_EMVDATA		0x10
#define SDK_OUTCOME_ONLINERESPDATA_ANY			0x20
#define SDK_OUTCOME_ONLINERESPDATA_NA			0xF0

#define SDK_OUTCOME_RESULT_APPROVED   							0x10
#define SDK_OUTCOME_RESULT_DECLINED   							0x20
#define SDK_OUTCOME_RESULT_ONLINE     							0x30
#define SDK_OUTCOME_RESULT_ENDAPPLICATION 						0x40
#define SDK_OUTCOME_RESULT_SELECTNEXT 							0x50
#define SDK_OUTCOME_RESULT_TRYANOTHERINTERFACE     				0x60
#define SDK_OUTCOME_RESULT_TRYAGAIN     						0x70
#define SDK_OUTCOME_RESULT_ENDAPPLICATION_COMMUNICATIONERROR 	0xA0
#define SDK_OUTCOME_RESULT_ENDAPPLICATION_ONDEVICECVM 			0xB0
#define SDK_OUTCOME_RESULT_ONLINE_TWOPRESENTMENTS				0xC0
#define SDK_OUTCOME_RESULT_ONLINE_PREMENTANDHOLD				0xD0
#define SDK_OUTCOME_RESULT_ONLINE_NOADDPREMENT					0xF0
#define SDK_OUTCOME_RESULT_NA 									0xFF

#define SDK_OUTCOME_START_A  0x00
#define SDK_OUTCOME_START_B  0x10
#define SDK_OUTCOME_START_C  0x20
#define SDK_OUTCOME_START_D  0x30
#define SDK_OUTCOME_START_NA 0xF0

#define SDK_OUTCOME_CVM_NOCVMREQ      	0x00
#define SDK_OUTCOME_CVM_OBTAINSIGNATURE 0x10
#define SDK_OUTCOME_CVM_ONLINEPIN       0x20
#define SDK_OUTCOME_CVM_CONFVERIFIED    0x30
#define SDK_OUTCOME_CVM_NA              0xF0

#define SDK_OUTCOME_FIELDOFFREQ_NA      	0xFF

#define SDK_OUTCOME_AIP_CONTACTCHIP      	0x10
#define SDK_OUTCOME_AIP_NA					0xF0

#define SDK_UI_MSGID_APPROVED        		0x03	//Approved
#define SDK_UI_MSGID_NOTAUTHORISED     		0x07	//Not Authorised
#define SDK_UI_MSGID_PLSENTERPIN  			0x09	//Please enter your PIN
#define SDK_UI_MSGID_PLSREMOVECARD  		0x10	//Please remove card
#define SDK_UI_MSGID_PRESENTCARD			0x15	//Present Card
#define SDK_UI_MSGID_PROCESSING        		0x16	//Processing
#define SDK_UI_MSGID_CARDREADOK      		0x17	//Card Read OK
#define SDK_UI_MSGID_PRESENT_ONECARD		0x19	//Please Present One Card Only
#define SDK_UI_MSGID_APPROVEDSIGN    		0x1A	//Approved – Please Sign
#define SDK_UI_MSGID_AUTHORISINGPLSWAIT		0x1B	//Authorising, Please Wait
#define SDK_UI_MSGID_ERROR_TRYOTHERCARD	 	0x1C	//Insert, Swipe or Try another card
#define SDK_UI_MSGID_INSERTCARD      		0x1D	//Please insert card
#define SDK_UI_MSGID_SEEPHONE        		0x20	//See Phone for Instructions
#define SDK_UI_MSGID_TRYAGAIN        		0x21	//Present Card Again
#define SDK_UI_MSGID_ERROR_SWITCHINTERFACE	0x18	//Please insert or swipe card

#define SDK_UI_STATUS_NOTREADY 				0
#define SDK_UI_STATUS_IDLE 					1
#define SDK_UI_STATUS_READYTOREAD 			2
#define SDK_UI_STATUS_PROCESSING  			3
#define SDK_UI_STATUS_CARDREADSUCCESS 		4
#define SDK_UI_STATUS_PROCESSINGERR   		5
#define SDK_UI_STATUS_COLLISION_DETECTED 	6
#define SDK_UI_STATUS_NA 					255

#define SDK_UI_VALUEQUALIFIER_NA		0x00
#define SDK_UI_VALUEQUALIFIER_BALANCE 	0x20

typedef struct
{
	unsigned char Result;
	unsigned char Start;
	unsigned char CVM;
	unsigned char UIRequestonOutcomePresent;
	unsigned char UIRequestonRestartPresent;
	unsigned char DataRecordPresent;
	unsigned char DiscretionaryDataPresent;
	unsigned char AlternateInterfacePreference;
	unsigned char Receipt;
	unsigned char FieldOffRequest;
	unsigned char RemovalTimeout[2];
	unsigned char OnlineResponseData;
}SDKOUTCOMEPARAM;

typedef struct
{
	unsigned char MessageID;
	unsigned char Status;
	unsigned char HoldTime;
	unsigned char LanguagePerference[2];
	unsigned char ValueQualifier;
	unsigned char Value[6];
	unsigned char CurrencyCode[2];
}SDKUIREQUESTPARAM;

extern s32 sdkSetOutcomeParam(u8 Result, u8 Start, u8 CVM, u8 UIRequestonOutcomePresent, u8 UIRequestonRestartPresent, u8 DataRecordPresent, u8 DiscretionaryDataPresent, u8 AlternateInterfacePreference, u8 Receipt, u8 FieldOffRequest, u8 *RemovalTimeout, u8 OnlineResponseData);
extern int sdkSendOutcome();
extern s32 sdkSetUIRequestParam(u8 MessageID, u8 Status, u8 HoldTime, u8 *LanguagePerference, u8 ValueQualifier, u8 *Value, u8 *CurrencyCode);
extern int sdkSendUIRequest();

extern SDKOUTCOMEPARAM gstOutcome;
extern SDKUIREQUESTPARAM gstUIRequest;

#endif
