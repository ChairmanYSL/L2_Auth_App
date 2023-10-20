#include "appglobal.h"

int Menu0()
{
    u8 bctime[10], astime[21];
    int key = SDK_KEY_ESC, rslt = SDK_ERR;
    u8 time[6];
    u8 POSEntryMode;
    u8 fallback = 0;
    u8 temp[128]={0}, temp1[128]={0};
	s32 readret;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "PURE L2 TEST", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1.SALE 2.CheckSum", SDK_DISP_LEFT_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE3, 0, "3.Parameters 4.AutoTest", SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "5.BCTC Param 6.TCP Set", SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "7.Outcome Delay", SDK_DISP_LEFT_DEFAULT);
	sdkDispBrushScreen();

    sdkKbKeyFlush();
    memset(gstasAmount, 0, sizeof(gstasAmount));
    memset(gbcOtherAmount,0,sizeof(gbcOtherAmount));

    while(1)
    {
		if(gstAutoTest)
		{
			rslt = BCTCStartTrade();

			if( rslt!= SDK_OK)
			{
				gstAutoTest = 0;
				key = SDK_KEY_ESC;
				goto _RETURN;
			}
			else
			{
				memset(gstasAmount, 0, sizeof(gstasAmount));
				if(gstbctcautotrade.amountexist)	//后台传9F02时用后台的金额
				{
					sdkBcdToAsc(gstasAmount, gstbctcautotrade.amount, 6);
				}
				else	//后台没传9F02，默认金额一分钱
				{
					memcpy(gstasAmount, "000000000001", 12);
				}

				if(gstbctcautotrade.otheramountexist)	//后台传9F03时用后台的金额
				{
					memcpy(gbcOtherAmount, gstbctcautotrade.otheramount, 6);
				}
				else	//后台没传9F03，默认为0
				{
					memset(gbcOtherAmount, 0, 6);
				}

				if(gstbctcautotrade.typeexist)
				{
					gTransType = gstbctcautotrade.transtype;
				}
				else
				{
					gTransType = 0x00;
				}

				if(gstbctcautotrade.currexpexist)
				{
					gTransCurrExponent = gstbctcautotrade.currexp;
				}
				else
				{
					gTransCurrExponent = 2;
				}

				rslt = DealTrade();
				if(SDK_ICC_NOCARD == rslt)
				{
					gstAutoTest = 0;
				}
                goto _RETURN;
			}
		}
//		else
//		{
//			sdkmSleep(100);
//			rslt = BCTCSingleTrade();
////			Trace("BCTC", "finish BCTCSingleTrade\r\n");
//			if(rslt == SDK_OK)
//			{
//				memset(gstasAmount, 0, sizeof(gstasAmount));
//				if(gstbctcautotrade.amountexist) //后台传9F02时用后台的金额
//				{
//					sdkBcdToAsc(gstasAmount, gstbctcautotrade.amount, 6);
//				}
//				else	//后台没传9F02，默认金额一分钱
//				{
//					memcpy(gstasAmount, "000000000001", 12);
//				}
//
//				if(gstbctcautotrade.otheramountexist)	//后台传9F03时用后台的金额
//				{
//					memcpy(gbcOtherAmount, gstbctcautotrade.otheramount, 6);
//				}
//				else	//后台没传9F03，默认为0
//				{
//					memset(gbcOtherAmount, 0, 6);
//				}
//
//				DealTrade();
//			}
//			else
//			{
//				sdkmSleep(100);
//			}
//		}

//		get_time_value(temp);
//		sdkDispFillRowRam(SDK_DISP_LINE5, 0, temp, SDK_DISP_DEFAULT);
		sdkmSleep(200);
        key = sdkKbGetKey();
        if(key == SDK_KEY_ESC)
        {
			gstAutoTest = 0;
            Trace("test", "key=%04X\r\n", key);
        }


        switch(key)
        {
            case SDK_KEY_1:
//                memset(gstasAmount, 0, sizeof(gstasAmount));
//
//				if(InputAmount("Purchase", "Pls Input Amount:",gstasAmount) == SDK_OK)
//                {
//                	TraceHex("pure-info", "after input amount gstasAmount = ",gstasAmount,13);
//                	DealTrade();
//				}

				rslt = BCTCStartTrade();
				if( rslt!= SDK_OK)
				{
					key = SDK_KEY_ESC;
					goto _RETURN;
				}

				memset(gstasAmount, 0, sizeof(gstasAmount));
				if(gstbctcautotrade.amountexist)	//后台传9F02时用后台的金额
				{
					sdkBcdToAsc(gstasAmount, gstbctcautotrade.amount, 6);
				}
				else	//后台没传9F02，默认金额一分钱
				{
					memcpy(gstasAmount, "000000000001", 12);
				}

				if(gstbctcautotrade.otheramountexist)	//后台传9F03时用后台的金额
				{
					memcpy(gbcOtherAmount, gstbctcautotrade.otheramount, 6);
				}
				else	//后台没传9F03，默认为0
				{
					memset(gbcOtherAmount, 0, 6);
				}

				if(gstbctcautotrade.currexpexist)
				{
					gTransCurrExponent = gstbctcautotrade.currexp;
				}
				else
				{
					gTransCurrExponent = 2;
				}

				DealTrade();
                goto _RETURN;

            case SDK_KEY_2:
                ShowCheckSum();
                goto _RETURN;

			 case SDK_KEY_3:
				 SetKernelParam();
				 goto _RETURN;

			 case SDK_KEY_4:
				 PostSpAutotest();
				 goto _RETURN;

             case SDK_KEY_5:
			 	 BCTCPostUpDateParam();
				 goto _RETURN;

			 case SDK_KEY_6:
			 	 PostSetTCPSetting();
				 goto _RETURN;

			 case SDK_KEY_7:
				PostSetOutcomeDelay();
				goto _RETURN;

             case SDK_KEY_FUNCTION:
			 case SDK_KEY_0:
			 	PostShowAppCompilerTime();
				key = SDK_KEY_ESC;
				goto _RETURN;

             case SDK_KEY_ENTER:
             case SDK_KEY_UP:
             case SDK_KEY_DOWN:
             case SDK_KEY_ESC:
               goto _RETURN;

             default:
               break;
        }

//        sdkmSleep(1100);//for G201
    }

_RETURN:

//    sdkIccPowerDown();
    memset(gstasAmount, 0, sizeof(gstasAmount));
    memset(gbcOtherAmount,0,sizeof(gbcOtherAmount));
    memset(gstasAmount, 0, sizeof(gstasAmount));
    memset(gbcOtherAmount,0,sizeof(gbcOtherAmount));
    return key;
}


int Menu1()
{
	Trace("app", "goto Menu1\r\n");
    int key;
	s32 rslt = 0;

    while(1)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Set parameters Menu1", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1.Date       2.Terminal", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "3.Settle     4.RF Para ", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "5.Batch mode 6.AID Param", SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "7.Sys Init   8.Printer", SDK_DISP_LEFT_DEFAULT);
        sdkDispBrushScreen();

        sdkKbKeyFlush();
        key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 | SDK_KEY_MASK_5
                           | SDK_KEY_MASK_6 | SDK_KEY_MASK_7 | SDK_KEY_MASK_8 | SDK_KEY_MASK_9 | SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC
                           | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_UP, 0);

        if(key) sdkSysBeep(0);

        switch ( key )
        {
             case SDK_KEY_1:
               SetSystemTime();
               break;

             case SDK_KEY_2:
               break;

             case SDK_KEY_3:
//               PostUpBatchdata();
               //PostSetRfSwitch();//�ǽ����ο���
               break;

             case SDK_KEY_4:
//               PostUpDateParam();
                //PostSetRfLogoPosition();
			   //emv_par_display();//���÷ǽӲ���

               break;

             case SDK_KEY_5:
               PostSetBatchCapture();
               break;

             case SDK_KEY_6:
               QueryAid(true);
               break;

             case SDK_KEY_7:

               break;

             case SDK_KEY_8:
               PostSetPrint();
               break;

             case SDK_KEY_ENTER:
             case SDK_KEY_DOWN:
             case SDK_KEY_UP:
             case SDK_KEY_ESC:
               return key;

               break;

             default:
               break;
        }

//        break;
    }

    return SDK_KEY_ESC;
}

s32 MenuTest()
{
	Trace("app", "goto MenuTest\r\n");
	int key;
	s32 rslt = 0;
	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Test Menu", SDK_DISP_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1.Version 2.Add Test AID", SDK_DISP_LEFT_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE3, 0, "3.Send TCP 4.Outcome", SDK_DISP_LEFT_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE4, 0, "5.APDU 6.Random", SDK_DISP_LEFT_DEFAULT);
	sdkDispFillRowRam(SDK_DISP_LINE5, 0, "7.SHA1 8.Display", SDK_DISP_LEFT_DEFAULT);
	sdkDispBrushScreen();

	sdkKbKeyFlush();
	key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
	Trace("app", "key = %04x\r\n", key);

	switch ( key )
	{
		case SDK_KEY_1:
			PostShowAppCompilerTime();
			break;

		case SDK_KEY_2:
			AddTestCardAID();
			AddUnionPayCTAID();
			break;

		case SDK_KEY_3:
			SendTCPTest();
			break;

		case SDK_KEY_4:
			HostOutcomeTest();
			break;

		case SDK_KEY_5:
			APDUTest();
			break;

		case SDK_KEY_6:
			RandNumTest();
			break;

		case SDK_KEY_7:
			ReadSNTest();
			break;

		case SDK_KEY_8:
			DisplayStringTest();
			break;

		case SDK_KEY_ENTER:
		case SDK_KEY_DOWN:
		case SDK_KEY_UP:
		case SDK_KEY_ESC:
			return key;
			break;

		default:
			break;
	}
	return SDK_KEY_ESC;
}
