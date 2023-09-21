#ifndef _DLLEMVCONTACTPRIVATE_H_
#define _DLLEMVCONTACTPRIVATE_H_


typedef struct
{
    unsigned char DataHead;    //'6A'
    unsigned char DataFormat;    //'05'
    unsigned char HashInd;    //'01'
    unsigned char ICCDynDataLen;
    unsigned char ICCDynData[223];    //LDD	<NIC-25
    unsigned char PadPattern[223];    //NIC-LDD-25,padded with 'BB'
    unsigned char HashResult[20];
    unsigned char DataTrail;    //'BC'
}SIGN_DYN_APPDATA_RECOVER;


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


typedef struct
{
    unsigned char DataFormat;
    unsigned char ICCDynDataLen;
    unsigned char ICCDynData[190];
    unsigned char DGTLSGNTR[64];
}SM_SIGN_DYN_APPDATA;

typedef struct
{
    unsigned char ICCDynNumLen;
    unsigned char ICCDynNum[8];
    unsigned char CryptInfo;
    unsigned char AppCrypt[8];
    unsigned char HashResult[32];
}SM_ICC_DYN_DATA;


typedef struct
{
    unsigned char ICCDynNumLen;
    unsigned char ICCDynNum[8];
    unsigned char CryptInfo;
    unsigned char AppCrypt[8];
    unsigned char HashResult[20];
}ICC_DYN_DATA;


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


#define CVMR_UNKNOWN             0
#define CVMR_FAIL                1
#define CVMR_SUCCESS             2


typedef struct
{
    unsigned char DataHead;    		//'6A'
    unsigned char CertFormat;    	//'04'
    unsigned char AppPAN[10];
    unsigned char ExpireDate[2];
    unsigned char CertSerial[3];
    unsigned char HashInd;
    unsigned char ICCPIN_EPKAlgoInd;
    unsigned char ICCPIN_EPKLen;
    unsigned char ICCPIN_EPKExpLen;
    unsigned char ICCPIN_EPKLeft[206];	//NI-42
    unsigned char HashResult[20];
    unsigned char DataTrail;			//'BC'
}ICCPIN_EPK_RECOVER;


typedef struct
{
    unsigned char IACDenial[5];
    unsigned char IACOnline[5];
    unsigned char IACDefault[5];
}Card_TAC;


#define GenerateAC1  1
#define GenerateAC2  2


//#define MAXSCRIPTLEN 128
#define MAXSCRIPTLEN 256//modify by cj


typedef struct
{
    unsigned char CertFormat;		//'12'证书格式
    unsigned char IssuID[4];		//主帐号最左面的3-8个数(右补F)发卡行标识
    unsigned char ExpireDate[2];	//证书失效日期MMYY
    unsigned char CertSerial[3];	//证书序列号
    unsigned char IPKSIGNAlgoInd;	// 发卡行公钥签名算法标识  SM2-'04'
    unsigned char IPKEncrptAlgoInd;	//发卡行公钥加密算法标识, 暂不使用，取值'00'
    unsigned char IPKParamInd;		//发卡行公钥参数标识
    unsigned char IPKLen;			//发卡行公钥的字节长度
    unsigned char IPK[64];			//发卡行公钥 NI
    unsigned char DGTLSGNTR[64];	//数字签名NCA
}SM_IPK_CRTFCT;

typedef struct
{
    unsigned char CertFormat;		//'14'证书格式
    unsigned char AppPAN[10];		//主帐号(右补F)
    unsigned char ExpireDate[2];	//证书失效日期MMYY
    unsigned char CertSerial[3];	//证书序列号
    unsigned char ICCPKSIGNAlgoInd;	// IC卡公钥签名算法标识  SM2-'04'
    unsigned char ICCPKEncrptAlgoInd;	//IC卡公钥加密算法标识, 暂不使用，取值'00'
    unsigned char ICCPKParamInd;	//IC卡公钥参数标识
    unsigned char ICCPKLen;
    unsigned char ICCPK[64];		//IC卡公钥  NICC
    unsigned char DGTLSGNTR[64];	//数字签名NI
}SM_ICCPK_CRTFCT;


typedef struct
{
    unsigned char DataFormat;			//'13'
    unsigned char DataAuthCode[2];
    unsigned char DGTLSGNTR[64];		//数字签名NI
}SM_SIGN_STAT_APPDATA;


//taa result
#define TAA_OFFLINE 0x40
#define TAA_ONLINE  0x80
#define TAA_DENIAL  0x00


//dol
#define typeCDOL1       1
#define typeCDOL2       2
#define typeDDOL        3
#define typeTDOL        4


#endif

