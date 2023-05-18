#ifndef __DEV_H__
#define __DEV_H__
/********************************ͨ�÷���ֵ�Ķ���**********************************/
#define     DDI_OK               0   //�ɹ�
#define     DDI_ERR              -1      //����
#define     DDI_ETIMEOUT         -2  //��ʱ
#define     DDI_EBUSY            -3  //�豸��æ
#define     DDI_ENODEV           -4  //�豸������
#define     DDI_EACCES           -5  //��Ȩ��
#define     DDI_EINVAL           -6  //������Ч
#define     DDI_EIO              -7      //�豸δ�򿪻��豸��������
#define     DDI_EDATA            -8  //���ݴ���
#define     DDI_EPROTOCOL        -9      //Э�����
#define     DDI_ETRANSPORT       -10     //�������
#define     DDI_READED_MAGCARD   -11     //�ǽӶ����ſ�


typedef struct
{
	int   Year; 	//!< The number of years						range 1900 to xxxx
	int   Month;	//!< The number of months,						range 1 to 12
	int   Day;		//!< The day of the month,						range 1 to 31
	int   Hour; 	//!< The number of hours past midnight, 		range 0 to 23
	int   Min;		//!< The number of minutes after the hour,		range 0 to 59
	int   Sec;		//!< The number of seconds after the minute,	range 0 to 59, (60 to allow for leap seconds)
	int   MSec; 	//!< The number of MSec after the minute,		range
	int   WeekDay;	//!< The number of days since Sunday,			range 0 to 6
	int   YearDay;	//!< The number of days since January 1,		range 0 to 365
}DDI_DATE_TIME;


typedef struct
{
	int (*ddi_apdu_exchange)(int tCardIndex,const unsigned char* pCmdBuf,unsigned int nCmdLen,unsigned char* pOutBuf,unsigned int nOutSize, unsigned int* pOutLen, unsigned int* pSW);

	int (*ddi_get_random)(unsigned char *buf, int len);

	int (*ddi_get_timerId)(DDI_DATE_TIME* pTime);

	int (*ddi_encry)(int type, const unsigned char* key, unsigned int keylen, const unsigned char* iv, const unsigned char* input, unsigned int inlen, unsigned char* output, unsigned int outsize, unsigned int* outlen, int mode);	//used for encrypt online pin

	int (*ddi_hash)(int type, const unsigned char* input, unsigned int inlen, unsigned char* output, unsigned int outsize, unsigned int* outlen, int flag);

}DDI_API;

#endif


