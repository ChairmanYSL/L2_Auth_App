#ifndef SDKEMVCONTACTPRIVATE_H
#define SDKEMVCONTACTPRIVATE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



typedef struct
{
	bool bIsForceOnline;			// force online for emvcontact
	bool bIsSupportDataStorage;  // for DPAS_CT
	bool bIsSupportExtendedLogging;  // for DPAS_CT

	s32 (*DispCandAppList)(u8 ucCandAppNum, u8 **pheCandApp);		//display aid list
	s32 (*DispReselectApp)(void);			//disp select app again
	s32 (*setBeforeGPO)(void);		//set something before GPO
	s32 (*DispPromptData)(unsigned char ePromptType);
	s32 (*DispVerifyIdCard)(void);
	s32 (*DispCardNo)(u8 *cardno);
	s32 (*DispIssuerReferral)(void);
	s32 (*CheckDataContainer)(const u8 *ContainerID); // for DPAS_CT
	s32 (*StoreContainerContent)(const u8 *ContainerID, const u8 *ContainerContent, u16 ContentLen); // for DPAS_CT
	s32 (*GetCardTransSum)(u32 *sum); //move trans log from SDK to Application

	
}SDK_EMVCONTACT_TRADE_PARAM;

extern bool sdkEmvContactCheckFloorLimit();



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

