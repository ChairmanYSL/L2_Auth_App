#ifndef _DLLJCBPRIVATE_H_
#define _DLLJCBPRIVATE_H_

#include "sdkoutcome.h"

typedef struct{
	unsigned char method;
	unsigned char condition;
}CVMR;

typedef struct{
	CVMR CVRList[120];
	unsigned char CVRListLen;
	unsigned char CVM_X[6];
	unsigned char CVM_Y[6];
}CVMSTRCUT;

typedef struct
{
    unsigned char IACDenial[5];
    unsigned char IACOnline[5];
    unsigned char IACDefault[5];
}Card_TAC;

typedef struct
{
    unsigned char DataHead;    //'6A'
    unsigned char DataFormat;    //'03'
    unsigned char HashInd;
    unsigned char DataAuthCode[2];
    unsigned char PadPattern[222];    //NI-26
    unsigned char HashResult[20];
    unsigned char DataTrail;    //'BC'
}SIGN_STAT_APPDATA_RECOVER;


#define Algeria_Request 0

#define CVMR_UNKNOWN             0
#define CVMR_FAIL                1
#define CVMR_SUCCESS             2


#define GenerateAC1  1
#define GenerateAC2  2


#define MAXSCRIPTLEN 128


//taa result
#define TAA_OFFLINE 0x40
#define TAA_ONLINE  0x80
#define TAA_DENIAL  0x00


//dol
#define typeCDOL1       1
#define typeCDOL2       2
#define typeDDOL        3
#define typeTDOL        4


unsigned char pure_CheckDataMissDDA(PURETradeUnionStruct *tempApp_UnionStruct);
unsigned char pure_CombineDDA_FirstHalf(PURETradeUnionStruct *tempApp_UnionStruct);

//9C
#define PURE_TRANS_GOODS  			0x00
#define PURE_TRANS_CASH   			0x01
#define PURE_TRANS_CASHBACK 		0x09
#define PURE_TRANS_REFUND			0x20
#define PURE_TRANS_RETRIEVE_GETDATA	0x78
#define PURE_TRANS_UPDATE_PUTDATA	0x79
#define PURE_TRANS_APPAUTHTRANS		0x90	//FIXME:accroding to PURE L2 Test Config001

#define TRANS_NEW					0x00
#define TRANS_ONLINE_RESPONSE		0x01
#define TRANS_TORN_RECOVERGAC		0x02

#endif

