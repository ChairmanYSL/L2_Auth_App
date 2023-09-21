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
    unsigned char CertFormat;		//'12'֤���ʽ
    unsigned char IssuID[4];		//���ʺ��������3-8����(�Ҳ�F)�����б�ʶ
    unsigned char ExpireDate[2];	//֤��ʧЧ����MMYY
    unsigned char CertSerial[3];	//֤�����к�
    unsigned char IPKSIGNAlgoInd;	// �����й�Կǩ���㷨��ʶ  SM2-'04'
    unsigned char IPKEncrptAlgoInd;	//�����й�Կ�����㷨��ʶ, �ݲ�ʹ�ã�ȡֵ'00'
    unsigned char IPKParamInd;		//�����й�Կ������ʶ
    unsigned char IPKLen;			//�����й�Կ���ֽڳ���
    unsigned char IPK[64];			//�����й�Կ NI
    unsigned char DGTLSGNTR[64];	//����ǩ��NCA
}SM_IPK_CRTFCT;

typedef struct
{
    unsigned char CertFormat;		//'14'֤���ʽ
    unsigned char AppPAN[10];		//���ʺ�(�Ҳ�F)
    unsigned char ExpireDate[2];	//֤��ʧЧ����MMYY
    unsigned char CertSerial[3];	//֤�����к�
    unsigned char ICCPKSIGNAlgoInd;	// IC����Կǩ���㷨��ʶ  SM2-'04'
    unsigned char ICCPKEncrptAlgoInd;	//IC����Կ�����㷨��ʶ, �ݲ�ʹ�ã�ȡֵ'00'
    unsigned char ICCPKParamInd;	//IC����Կ������ʶ
    unsigned char ICCPKLen;
    unsigned char ICCPK[64];		//IC����Կ  NICC
    unsigned char DGTLSGNTR[64];	//����ǩ��NI
}SM_ICCPK_CRTFCT;


typedef struct
{
    unsigned char DataFormat;			//'13'
    unsigned char DataAuthCode[2];
    unsigned char DGTLSGNTR[64];		//����ǩ��NI
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

