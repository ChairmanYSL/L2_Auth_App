#ifndef AFX_COMM_H
#define AFX_COMM_H

//#define KEY_TEXT    0x12      //keypad text frame identifier
//#define KEY_ORDER   0x13      //keypad order frame identifier
#define KEY_LTEXT   0x15      //keypad extended text frame identifier    

#define iocomtime 500		//HHL	
#define iccovertime 500

#define COMMPROTOCOL_STX  0x02
#define COMMPROTOCOL_EXT  0x03

#define COMMDATABUFMAXLEN 1024//1024  
#define COMMDATATXMAXLEN  (1024+8+80)  //1024 
#define MAXBLOCKDATASIZE  (68+512)  //

#define COMMHEADDATALEN    8		// 4 to 8
typedef struct _COMMUNICATEDATA
{
    unsigned int serialno;
    unsigned int XGD_RFU[4];        // 20110214 hhl adds according to the new specification
    unsigned int command;
    unsigned int datalen;
    unsigned char  data[COMMHEADDATALEN+1024+32];    //reserve 1024 bytes for future use  
    unsigned char head_cnt;
    
}COMMUNICATEDATA;

EXTERN u8 Rx_Valid;
EXTERN COMMUNICATEDATA gCommData;

typedef struct _READERSTATUS
{
	u8 readerstatus;
	u8 serialno;
	u8 debugoptimizemode;
}READERSTATUS;

EXTERN READERSTATUS gReaderStatus;


//POLL
#define COMMAND_POLL                        0x07
#define COMMAND_ECHO                        0x08


//debug and optimize message mode     
#define COMMAND_SETDEBUGANDOPTIMIZEMODE     0x10
#define COMMAND_RFU                         0x11
#define COMMAND_SETPARAMETERS               0x12               

//authenticate message 
#define COMMAND_COMMINITIAL                 0x20
#define COMMAND_DUALAUTHEN                  0x21
#define COMMAND_KEYGENERATE                 0x22
#define COMMAND_DISABLEREADER               0x23
#define COMMAND_POLL_P_READER			    0x24


//pay message
#define COMMAND_PREPAREPAY                  0x30
#define COMMAND_PAYRESET                    0x31
#define COMMAND_DISPLAYSTATUS               0x32
#define COMMAND_DEALONLINE                  0x33


#define COMMAND_QUERYICCBALANCE		0x3F
#define COMMAND_SETSTATUSCHECK		0x3E


//manage message
#define COMMAND_ENTERMANAGEMODE             0x40
#define COMMAND_GETCAPABILITY               0x41
#define COMMAND_SETCAPABILITY               0x42
#define COMMAND_GETDATETIME                 0x43
#define COMMAND_SETDATETIME                 0x44
#define COMMAND_GETPARAMETERS               0x45
#define COMMAND_GETVISAPUBKEY               0x50
#define COMMAND_SETVISAPUBKEY               0x51
#define COMMAND_GETBAUDRATE                 0x52
#define COMMAND_SETBAUDRATE                 0x53
#define COMMAND_RESERACQUIRERKEY            0x54
#define COMMAND_RESUMEREADER                0x55
#define COMMAND_GETPBOCTAG                  0x56
#define COMMAND_SETPBOCTAG                  0x57
#define COMMAND_GETDISPINFO                 0x58
#define COMMAND_SETDISPINFO                 0x59
#define COMMAND_GETCVM                      0x5A
#define COMMAND_SETCVM                      0x5B
#define COMMAND_PAYWAVEGETAPID              0x5C
#define COMMAND_PAYWAVESETAPID              0x5D
#define COMMAND_SETPBOCCAPK                 0x61
#define COMMAND_QUERYPBOCCAPK               0x62
#define COMMAND_SETRETRIVECAPK              0x63
#define COMMAND_QUERYRETRIVECAPK            0x64
#define COMMAND_SETBLACKLIST                0x65
#define COMMAND_QUERYBLACKLIST              0x66
#define COMMAND_SETFIXEDPARAMETERS          0x67
#define COMMAND_SETAIDPARAMETERS            0x68


///mobile pay
#define COMMAND_MOBILEPAY                   0x6B
#define COMMAND_OFFLINEBAGCONFIRM           0x80
#define COMMAND_MPREPAREPAY                 0x84  //wsm  20130826



#define COMMAND_GETJCBPUBKEY                0xC0
#define COMMAND_SETJCBPUBKEY                0xC1

#define COMMAND_PAYPASS                     0xFA
#define COMMAND_LOOPBACK                    0xF9   
#define COMMAND_PAYWAVE                     0xF8 
#define COMMAND_DOWNLOAD                    0xF7
#define COMMAND_PAYWAVESETAPPID             0xF6
#define COMMAND_PAYPSSBATCH                 0xF5
#define COMMAND_READERKEYRESET              0xEA
#define COMMAND_PAYEAVECARDTYPE             0xF0


#define COMMAND_SETFIXEDDISPTABLE           0xFF
#define COMMAND_GETFIXEDDISPTABLE           0xFE
#define COMMAND_SETIFDSERIALNO              0xFD
#define COMMAND_GETIFDSERIALNO              0xFC
#define COMMAND_RESETSYSTEM                 0xFB


#define COMMAND_GETBAUD_CUSTOM		0xF1
#define COMMAND_SETBAUD_CUSTOM		0xF2
#define COMMAND_DeleteAllAID		0xF3			
#define COMMAND_DeleteAllCAPK		0xF4			


#define COMMAND_GETVERSIONNUM		0xE1			
#define COMMAND_LEDTEST				0xE2			
#define COMMAND_SIMCARD				0xE3			
#define COMMAND_OPERATESPEAKER		0xE4			
#define COMMAND_GETDATETIME_CUSTOM	0xE5			
#define COMMAND_SETDATETIME_CUSTOM	0xE6			
#define COMMAND_QUERYWAITTIME       0xE7  
#define COMMAND_PAYPASSSETPBOCTAG   0xE8


//20130117add
#define COMMAND_PAYPASS_USERREQDATA         0xEF
#define COMMAND_PAYPASS_DISCRENTDATA        0xEE
#define COMMAND_PAYPASS_OUTPARAMSET         0xED
#define COMMAND_PAYPASS_CLEANSIGNAL         0xEC
#define COMMAND_UPDATEAPPLICATION           0xEB
#define COMMAND_PAYPASS_FORCECLEANSIGNAL    0xE9  //sxl20150722
#define COMMAND_PAYPASS_LOOPTRANS           0xE0
#define COMMAND_PAYPASS_PARAMSET            0xDF
#define COMMAND_PAYPASS_CANCEL              0xDE

////////response code
#define RC_SUCCESS                          0x00
#define RC_DATA                             0x01
#define RC_POLL_A                           0x02
#define RC_POLL_P                           0x03
#define RC_SCHEME_SUPPORTED                 0x04
#define RC_SIGNATURE                        0x05
#define RC_ONLINE_PIN                       0x06
#define RC_OFFLINE_PIN                      0x07
#define RC_SECOND_APPLICATION               0x08

#define RC_PAYPASSDECLINE                   0x50
#define RC_PAYPASSENDAPPLICATION            0x51


#define RC_OTHER_INTERFACE                  0xE9
#define RC_INVALID_JCB_CA_KEY               0xEA
#define RC_NO_SIG                           0xEB
#define RC_NO_PIN                           0xEC
#define RC_US_CARDS                         0xED
#define RC_Other_AP_CARDS                   0xEE
#define RC_POLL_N                           0xEF
#define RC_NO_PARAMETER                     0xF0
#define RC_NO_PBOC_TAGS                     0xF1
#define RC_NO_CARD                          0xF2
#define RC_MORE_CARDS                       0xF3
#define RC_INVALID_VISA_CA_KEY              0xF4
#define RC_INVALID_SCHEME                   0xF5
#define RC_INVALID_KEYINDEX                 0xF6
#define RC_INVALID_PARAM                    0xF7
#define RC_INVALID_DATA                     0xF8
#define RC_INVALID_COMMAND                  0xF9
#define RC_DDA_AUTH_FAILURE                 0xFA   //sxl?这个还没有处理
#define RC_AUTH_NOT_PERFORMED               0xFB
#define RC_AUTH_FAILURE                     0xFC
#define RC_ACCESS_FAILURE                   0xFD
#define RC_ACCESS_NOT_PERFORMED             0xFE
#define RC_FAILURE                          0xFF
#define RC_DECLINE                          0x0F
#define RC_ERR_LASTRECORD                   0x0E
#define RC_ONLINEDEAL                       0x0D
#define RC_ErrTransDisp                     0x0C
#define RC_NO_PSE                           0x09
#define RC_CARDNO                           0x0B
#define RC_SMVERIFY                         0x0A


#define PAYPASSTRADE_CONSUME 0x00
#define PAYPASSTRADE_REFUND  0x20
#define PAYPASSTRADE_PURWITHCASH 0x09
#define PAYPASSTRADE_CASH    0x01


#define PAYEAVE_REFUND 0x01
#define PAYEAVE_CASH   0x02
#define PAYEAVE_CASHBACK 0x03

#define OTHERAMOUNTPOS 6
#define OTHERAMOUNTLEN 6

typedef struct _COMPAYDATA
{
    u8 selectmode;
    u8 amount[6];
    u8 amountother[6];
    u8 cardtype;
    u8 EC_IsMulAccountCard;
    SDK_ICC_PARAM *pstIccParam;
    u16 nowtask;
    u8 ifreaddatacomplete;
    u8 tradetypevalue;
    u8 SoundPlayFlag;
    u8 CheckBlackCardFlag;
    u8 flag_COMMAND_MPREPAREPAY;
    u8 ResetAmountFlag;
    u8 ContactCardStatus;
    u8 *ppaydata;
    u16 paydatalen;
}COMPAYDATA;

EXTERN u8 gbcOtherAmount[6];//sujianzhong 2016.09.28 10:24
EXTERN COMPAYDATA gPayData;
extern void transflow_Dpaspay(u8 serialno,u8 *Dpasdata,u32 Dpasdatalen);
extern s32 transflow_Dpaspaytrade(u8 serialno, u8 *paydata, u32 paydatalen, u8 *returndata, u32 *returndatalen);
extern u8 manage_readeraddserialno(void);

#endif

