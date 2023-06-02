#define AFX_APP_VARIABLES
#include "appglobal.h"
#undef AFX_APP_VARIABLES
#include "host.h"
#include "extern.h"

#define MENU_PAGEMAX 2

extern s32 sdkEMVBasePowerStartInit();
extern void sdkEMVBase_SetEMVLogOutput(bool enable);

static void AppSysmain(void)
{
    u8 rslt;
    s32 key, page = 0;
    bool bisBrushScrean = true;

    while(1)
    {
        switch(page)
        {
             case 1:
               key = Menu1();
               break;

             case 2:
               key = Menu2();
               break;

             default:
               key = Menu0();
               bisBrushScrean = false;
               break;
        }

        switch ( key )
        {
             case SDK_KEY_ENTER:
             case SDK_KEY_DOWN:
               bisBrushScrean = true;
               page++;

               if(page > MENU_PAGEMAX)
               {
                   page = 0;
               }
               break;

             case SDK_KEY_UP:
               bisBrushScrean = true;
               page--;

               if(page < 0)
               {
                   page = MENU_PAGEMAX;
               }
               break;

             case SDK_KEY_ESC:

               if(page != 0)
               {
                   bisBrushScrean = true;
               }
               page = 0;
               break;

             default:
               break;
        }
    }
}

s32 appmain(s32 argc, char const *argv[])
{
    int ret;
	s32 timeout = 0;
//	u8 *data_uf, *data_f;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_APPSTARTUP, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
	Trace("emv","goto sdkEMVBasePowerStartInit\r\n");
    sdkEMVBasePowerStartInit();
	ReadAPPEXAID();
	sdkEMVBase_SetEMVLogOutput(true);

#ifdef ZT_SDK_DEBUG
	sdkEMVBase_SetEMVLogOutput(true);
#else
	sdkEMVBase_SetEMVLogOutput(false);
#endif

    gstAutoTest = 0;	//2021.9.10 lishiyao  add for BCTC New Host of JCB
	gstHostTimeout = 0;		//2021.9.10 lishiyao
    memset(&gstbctcautotrade,0,sizeof(gstbctcautotrade));
    gDispSeePhone=0;//luohuidong 2018.02.02 10:20
	gJCBPaperNum = 1;
	gTransCurrExponent = 2;//默认有两位小数
	gSerialPortId = OpenComm();
	Trace("lishiyao", "return portID:%d\r\n", gSerialPortId);
	gTCPPort = 6789;
	memset(gTCPAddress, 0, sizeof(gTCPAddress));
	memcpy(gTCPAddress, "192168250093", 12);
	gHostTransType = HOST_TRANS_SERIAL;

//	data_uf = convertToCString(gTCPAddress, 16);
//	data_f = formatIPAddress(data_uf);
//	Trace("lishiyao", "before removeLeadingZeros IP:%s\r\n", data_f);
//	removeLeadingZeros(data_f);
//	sdkOpenWifi(data_f, gTCPPort);

    AppSysmain();

    return 0;
}

