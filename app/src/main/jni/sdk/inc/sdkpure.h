#ifndef SDKJCB_H
#define SDKJCB_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

extern s32 sdkPureTransInit();
extern s32 sdkPureTransFlow();
extern int sdkPureSetInputPINRes(int res, unsigned char *pinBuf, int bufLen);
extern void sdkPureSetVerifyCardNoRes(int res);
extern void sdkPureSetVerifyRevocationKeyRes(int res);
extern s32 sdkPureSetForceOnline(bool bIsForceOnline);
extern s32 sdkPureImportOnlineResult(s32 ucOnlineResult, const u8 *pheRspCode);
extern s32 sdkPureSetImplementation(u8 Implementation);
extern s32 sdkPureSetMTOL(u8 *MTOL, s32 len);
extern s32 sdkPureSetTermAIDSupport(u8 *TermAIDSup, s32 len);
extern s32 sdkPureGetCTPreProcessIndicator(u8 *indicator);
extern s32 sdkPureSetMemorySlotUpdateTemplate(u8 *Template, u16 len);
extern void sdkPureFreeMemorySlotUpdateTemplate();
extern s32 sdkPureSetMemorySlotReadTemplate(u8 *Template, u16 len);
extern void sdkPureFreeMemorySlotReadTemplate();
extern s32 sdkPureGetGDDOLResBuf(unsigned char *buf, int len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif



