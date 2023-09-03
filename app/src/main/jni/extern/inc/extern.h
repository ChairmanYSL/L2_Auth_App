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
