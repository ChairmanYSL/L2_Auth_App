#ifndef SDKEMVCONTACT_H
#define SDKEMVCONTACT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum
{
    SDK_EMV_ICC_WAIT,           	//提示"请稍候..."
    SDK_EMV_ICC_OFFLINE_PIN_ERR, 	//提示"PIN密码错误!"
    SDK_EMV_ICC_OFFLINE_PIN_SUC, 	//提示"密码正确"
    SDK_EMV_ICC_INPUT_PIN_AGAIN, 	//提示"PIN密码错误!\r\n请重试",读取"9F17",判断值是否为1，如果是，提示"(最后一次)"
    SDK_EMV_ICC_PERFORM_CDCVM		// "remove the card, perform CDCVM, and try again"  DPAS_CT
}SDK_EMV_ICC_PROMPT_TYPE;

typedef struct
{
    unsigned char ucAidLen;
    unsigned char asAid[32];
    unsigned char ucAppLabelLen;
    unsigned char asAppLabel[32];
    unsigned char ucPreferNameLen;
    unsigned char asPreferName[32];
    unsigned char ucPriority;
    unsigned char ucLangPreferLen;
    unsigned char asLangPrefer[16];
    unsigned char ucIcti;		//Issuer Code Table Index.lang used for display app list according to ISO8859.but not include Chinese,Korea,etc.
    unsigned char ucLocalName;	//If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char ucAppLocalNameLen;
    unsigned char asAppLocalName[32];
	unsigned char rfuLen; // for USA GP  2021.09.18
	unsigned char *rfu; // for USA GP  2021.09.18
}SDK_EMV_AIDLIST;

typedef struct
{
    unsigned int TransAmt;     			/* Transaction amount */
    unsigned short TransCount;    	 	/* Transaction Sequence Counter. inc by 1 for each trans*/
    unsigned char TransType;         	/* Transaction type BCD */
    unsigned char TSI[2];                /* Transaction Status Information */
    unsigned char PANLen;
    unsigned char PAN[10];           	/* Primary account No. */
    unsigned char PANSeq;               /* EMVTest: sequence num with same PAN */
    unsigned char TransDate[3];      	/* Trancaction Date(Form:"YY/MM/DD") */
    unsigned char TransTime[3];      	/* Trancaction Time(Form:"HH/MM/SS") */
} SDK_EMV_TRANS_LOG;


extern s32 sdkEmvContactTransInit();
extern s32 sdkEmvContactTransFlow1();
extern s32 sdkEmvContactTransFlow2();

extern s32 sdkEmvContactImportOnlineResult(s32 ucOnlineResult, u8 *pheRspCode);

extern s32 sdkEmvContactSetDispAppList(s32(*fun_dispapplist)(u8 ucCandAppNum, u8 **pheCandApp));//fun_dispapplist返回值:从0开始表示选中第一个应用
extern s32 sdkEmvContactSetDispReselectApp(s32(*fun_dispReselectApp)());//fun_dispReselectApp返回值:SDK_OK:成功; SDK_ERR:失败
extern s32 sdkEmvContactSetBeforeGPO(s32(*fun_setbeforegpo)());//fun_setbeforegpo返回值:SDK_OK:成功; SDK_ERR:失败
extern s32 sdkEmvContactSetForceOnline(bool forceonline);		//default is true






/*****************************************************************************
** Descriptions: 交易检查此发卡行公钥证书是否被回收了
** Parameters:	key: RID(5Bytes)+CAPKI(1Byte)+证书序列号(3Bytes);

** Returned value:
** Created By:luohuidong
** Remarks:  如应用不支持，则此函数不需要调用
回调函数fun_cmprevocation返回值:SDK_OK:不是回收公钥; SDK_ERR:是回收公钥
*****************************************************************************/
extern s32 sdkEmvContactSetRevocationKey(s32(*fun_cmprevocation)(u8 *key));
extern s32 sdkEmvContactSetDispPromptData(s32(*fun_dispPromptData)(unsigned char ePromptType));//fun_dispPromptData根据SDK_EMV_ICC_PROMPT_TYPE的类型进行提示
extern s32 sdkEmvContactSetDispVerifyIdCard(s32(*fun_dispVerifyIdCard)());//fun_dispVerifyIdCard返回值:SDK_OK:证件验证正确; SDK_ERR:失败
extern s32 sdkEmvContactSetCmpCardNO(s32 (*CheckCardNo)(const u8 *pasPAN));//CheckCardNo返回值:SDK_OK:不在黑名单; SDK_ERR:在黑名单
extern s32 sdkEmvContactSetDispCardNO(s32(*fun_setdispcardno)(const u8 *pasPAN));//fun_setdispcardno返回值:SDK_OK:正常; SDK_ERR:异常退出
extern s32 sdkEmvContactSetDispIssuerReferral(s32(*fun_dispIssuerReferral)());
extern s32 sdkEmvContactSetCheckDataContainer(s32(*fun_setcheckdatacontainer)(const u8 *ContainerID));
extern s32 sdkEmvContactSetStoreContainerContent(s32(*fun_setstorecontainercontent)(const u8 *ContainerID, const u8 *ContainerContent, u16 ContentLen));


extern s32 sdkEmvContactReadLog(u8 *LogFormat,SDK_EMVBASE_LOGDATA *LogData,u8 *LogNum ,u8 MaxNum,u8 tradeMode);

extern s32 sdkEmvContactTransReadAppData();
extern s32 sdkEmvContactReadEcBalance(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

