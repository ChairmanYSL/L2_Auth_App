#ifndef AFX_EXTERN_H
#define AFX_EXTERN_H

#include "ddi_firmware.h"
#include "ddi_icc.h"
#include "ddi_mag.h"
#include "ddi_manage.h"
#include "ddi_mifire.h"
#include "devApi.h"
#include "ddi_ped.h"
#include "ddi_prevent.h"
#include "ddi_printer.h"
#include "ddi_result.h"
#include "ddi_security.h"

#define    SDK_ICC_MANUAL       0x01                // ֧��������//support manual input
#define    SDK_ICC_MAG          0x02                // ֧��ˢ��//support swiping card
#define    SDK_ICC_ICC          0x04                // ֧�ֽӴ�ʽ//support contact
#define    SDK_ICC_RF           0x08                // ֧�ַǽӴ�//support contactless

extern s32 sdkIccDispRfLogo();

typedef enum{
	CARD_TYPE_IC1 = 0x00,
	CARD_TYPE_PSAM1 = 0x01,
	CARD_TYPE_PSAM2 = 0x02,
	CARD_TYPE_PSAM3 = 0x03,
	CARD_TYPE_CLCPU	= 0x04,
	CARD_TYPE_M1 = 0X05,
}EMV_CARD_TYPE;

#endif
