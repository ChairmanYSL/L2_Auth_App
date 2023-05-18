#ifndef SDKJCB_H
#define SDKJCB_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

extern s32 sdkPureTransInit();
extern s32 sdkPureTransFlow();
extern s32 sdkPureTransFlow1();
extern s32 sdkPureTransFlow2();
extern void sdkPureSetBeforeGPORes(int res);
extern int sdkPureSetInputPINRes(int res, unsigned char *pinBuf, int bufLen);
extern void sdkPureSetVerifyCardNoRes(int res);
extern void sdkPureSetVerifyRevocationKeyRes(int res);
extern void sdkPureSetSecondTapCardRes(int res);
extern s32 sdkPureSetDispRemoveCard(s32(*fun_setdispremovecard)());//fun_setdispremovecard返回值:SDK_OK:成功; SDK_ERR:失败
extern s32 sdkPureSetBeforeGPO(s32(*fun_setbeforegpo)());//fun_setbeforegpo返回值:SDK_OK:成功; SDK_ERR:失败
extern s32 sdkPureSetVerifyCardNo(s32(*fun_setVerifyCardNo)(u8* asPAN));//fun_setVerifyCardNo返回值:SDK_OK:成功; SDK_ERR:失败
extern s32 sdkPureSetRevocationKey(s32(*fun_cmprevocation)(u8 *key));
extern s32 sdkPureSetForceOnline(bool bIsForceOnline);
extern s32 sdkPureImportOnlineResult(s32 ucOnlineResult, const u8 *pheRspCode);
extern s32 sdkPureSetDispTapCardAgain(s32(*fun_setDispTapCardAgain)());//fun_setDispTapCardAgain显示请读卡,然后复位: SDK_OK:成功; SDK_ERR:失败
extern s32 sdkPureSetImplementation(u8 Implementation);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif



