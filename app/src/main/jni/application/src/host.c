#include "appglobal.h"
#include "host.h"
#include "dllpure.h"


#define BCTC_SEND_LEN 1024
#define BCTC_RECV_LEN 512


//#ifdef BCTCHOST
unsigned short WGet(u8 *aSrc)
{
    return (unsigned short)((unsigned short)*(aSrc + 1) + (unsigned short)(*aSrc << 8));
}

unsigned int DWGet(u8 *aSrc)
{
    return (unsigned int)((unsigned int)*(aSrc + 2) + (unsigned int)(*(aSrc + 1) << 8) + (unsigned int)(*aSrc << 16));
}

unsigned short TlvTSize(u8 *aTLV) //3byte
{
    if (aTLV == NULL) { return 0; }

    if((*aTLV & 0x1F) == 0x1F)
    {
        if((*(aTLV + 1) & 0x80) == 0x80)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 1;
    }
}

unsigned short TlvLSize(u8 *aTLV)
{
    u8 *pb;

    if (aTLV == NULL) { return 0; }
    pb = aTLV + TlvTSize(aTLV);

    if (*pb & 0x80)
    {
        return (unsigned short)((*pb & 0x7F) + 1);
    }
    return 1;
}

unsigned short TlvLen(u8 *aTLV)
{
    u8 *pb;

    if (aTLV == NULL) { return 0; }
    pb = aTLV + TlvTSize(aTLV);

    if ((*pb & 0x80) == 0)
    {
        return *pb;
    }
    else if (*pb == 0x81)
    {
        return *(pb + 1);
    }
    else if (*pb == 0x82)
    {
        return WGet(pb + 1);
    }
    return 0xFFFF;
}

unsigned short TlvSizeOf(u8 *aTLV)
{
    if (aTLV == NULL) { return 0; }
    return (unsigned short)(TlvTSize(aTLV) + TlvLSize(aTLV) + TlvLen(aTLV));
}

unsigned int TlvTag(u8 *aTLV)
{
    if (aTLV == NULL) { return 0; }

    if ((*aTLV & 0x1F) == 0x1F)
    {
        if((*(aTLV + 1) & 0x80) == 0x80)
        {
            return DWGet(aTLV);
        }
        else
        {
            return WGet(aTLV);
        }
    }
    else
    {
        return *aTLV;
    }
}

u8 *TlvVPtr(u8 *aTLV)
{
    if (aTLV == NULL) { return NULL; }
    return aTLV + TlvTSize(aTLV) + TlvLSize(aTLV);
}

u8 *TlvSeek(u8 *aTlvList, unsigned short aLen, unsigned int aTag)
{
    u8 *pb;

    if (aTlvList == NULL) { return NULL; }
    pb = aTlvList;

    while (pb < aTlvList + aLen)
    {
        if (TlvTag(pb) == aTag)
        {
            return pb;
        }
        pb += TlvTSize(pb) + TlvLSize(pb) + TlvLen(pb);
    }

    return NULL;
}
//#endif

u16 TlvSeekSame(u8 *aTlvList, u16 aLen, u32 aTag, u8 *pout)
{
    u8 *pb;
    u16 offset = 0, taglen = 0, buflen = 0;

    if (aTlvList == NULL) { return 0; }
    pb = aTlvList;

    while(pb < aTlvList + aLen)
    {
        if(TlvTag(pb) == aTag)
        {
            taglen = TlvSizeOf(pb);
            memcpy(&pout[offset], pb, taglen);
            offset += taglen;
        }
        buflen += TlvSizeOf(pb);

        if(buflen == aLen)
        {
            return offset;
        }
        pb += TlvTSize(pb) + TlvLSize(pb) + TlvLen(pb);
    }

    return 0;
}

void FormTLVData(const u8 TagArray[][2],u8 *data,s32 *datalen)
{
	u16 i = 0;
	u32 valuelen = 0;
	u8 taglen = 0;
	u16 totallen = 0;
	u8 *tagvalue = sdkGetMem(256);
	u8 datain = 0;
	s32 ret = 0;

	while(1)
    {
    	if(memcmp(TagArray[i], "\x00\x00", 2) == 0)
        {
        	*datalen += totallen;
            break;
        }
		valuelen = 0;
		sdkEMVBaseReadTLV(TagArray[i], tagvalue, &valuelen);

		if(valuelen)
		{
			taglen = 1;
			if((TagArray[i][0] & 0x1F) == 0x1F)
		    {
		        taglen = 2;
		    }
			memcpy(data+totallen,TagArray[i],taglen);
			totallen += taglen;
			data[totallen ++] = valuelen;
			memcpy(data+totallen,tagvalue,valuelen);
			totallen += valuelen;
		}
		else
		{
			Trace("emv","TagArray[%d]=%02x %02x empty!! \r\n",i,TagArray[i][0],TagArray[i][1]);
		}

        i++;
    }
	sdkFreeMem(tagvalue);
}

void SaveBlackData()
{
    u8 fn[64] = {0};
//	u8 *temp=NULL;
//	int i,index;

    sdkSysGetCurAppDir(fn);
    strcat(fn, "BlackData");

//	temp = (u8 *)sdkGetMem(sizeof(gstbctcblack));
//	memset(temp, 0, sizeof(gstbctcblack));
//
//	index = 0;
//	for(i = 0; i < sizeof(gstbctcblack)/sizeof(BCTC_BLACK); i++)
//	{
//		memcpy(temp+index, gstbctcblack[i].pan, 10);
//		index += 10;
//		temp[index]  = gstbctcblack[i].panlen;
//		index++;
//	}
//
//    sdkInsertFile(fn, (const u8 *)temp, 0, sizeof(gstbctcblack));

    sdkInsertFile(fn, (const u8 *)gstbctcblack, 0, sizeof(gstbctcblack));
	Trace("BCTC", "flag1\r\n");
//	sdkFreeMem(temp);
	Trace("BCTC", "flag2\r\n");
}

void ReadBlackData()
{
    u8 fn[64] = {0};
    s32 len = sizeof(gstbctcblack);

    sdkSysGetCurAppDir(fn);
    strcat(fn, "BlackData");

    memset(gstbctcblack, 0, sizeof(gstbctcblack));
    sdkReadFile(fn, (u8 *)gstbctcblack, 0, &len);
}

void SaveRecovaData()
{
    u8 fn[64] = {0};
	s32 ret;

	Trace("BCTC", "start SaveRecovaData\r\n");
	Trace("BCTC", "flag1\r\n");
    sdkSysGetCurAppDir(fn);
    strcat(fn, "RecovaData");
	Trace("BCTC", "flag2\r\n");

    ret = sdkInsertFile(fn, (u8 *)gstbctcpkrecova, 0, sizeof(gstbctcpkrecova));
	Trace("BCTC", "sdkInsertFile ret = %d\r\n", ret);
	Trace("BCTC", "flag3\r\n");
}

void ReadRecovaData()
{
    u8 fn[64] = {0};
    s32 len = sizeof(gstbctcpkrecova);

    sdkSysGetCurAppDir(fn);
    strcat(fn, "RecovaData");

    memset(gstbctcpkrecova, 0, sizeof(gstbctcpkrecova));
    sdkReadFile(fn, (u8 *)gstbctcpkrecova, 0, &len);
}


s32 BCTCTlvToCAPKStruct(u8 *buf, u16 ilen)
{
    u8 *pb, *pbTlv, *p9F06, *pDF03;
    u16 checkLen = 0;
    u8 checksum[20];
    u8 DISP[32];
    SDK_EMVBASE_CAPK_STRUCT *msgCAPK;
    u8 checkSum[20];
    u32 uiIndex;
    u8 *checkData = NULL;
    u16 checkData_size;
	s32 ret;
	u8 checkBuf[260];

    pbTlv = TlvSeek(buf, ilen, 0x9F06);    //RID

    if(pbTlv == NULL)
    {
        return SDK_ERR;
    }
    else
    {
        msgCAPK = (SDK_EMVBASE_CAPK_STRUCT *)sdkGetMem(sizeof(SDK_EMVBASE_CAPK_STRUCT));
        memset(msgCAPK, 0, sizeof(SDK_EMVBASE_CAPK_STRUCT));

        p9F06 = pbTlv;
        pb = TlvVPtr(pbTlv);
        memcpy(msgCAPK->RID, pb, 5);
        TraceHex("emv", "msgCAPK.RID", msgCAPK->RID, 5);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F22);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        msgCAPK->CAPKI = *pb;
        Trace("emv", "msgCAPK.CAPKI=%2X\r\n", msgCAPK->CAPKI);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF07);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        msgCAPK->ArithInd = *pb;
        Trace("emv", "msgCAPK.ArithInd=%2X\r\n", msgCAPK->ArithInd);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF06);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        msgCAPK->HashInd = *pb;
        Trace("emv", "msgCAPK.HashInd=%2X\r\n", msgCAPK->HashInd);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF02);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        msgCAPK->ModulLen = TlvLen(pbTlv);
        memcpy(msgCAPK->Modul, pb, msgCAPK->ModulLen);
        TraceHex("emv", "msgCAPK.Modul", msgCAPK->Modul, msgCAPK->ModulLen);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF04);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        msgCAPK->ExponentLen = TlvLen(pbTlv);
        memcpy(msgCAPK->Exponent, pb, msgCAPK->ExponentLen);
        TraceHex("emv", "msgCAPK.Exponent", msgCAPK->Exponent, msgCAPK->ExponentLen);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF03);

    if(pbTlv != NULL)
    {
        pDF03 = pbTlv;
        pb = TlvVPtr(pbTlv);
        msgCAPK->CheckSumLen = TlvLen(pbTlv);
        memcpy(msgCAPK->CheckSum, pb, msgCAPK->CheckSumLen);
        TraceHex("emv", "msgCAPK.CheckSum", msgCAPK->CheckSum, msgCAPK->CheckSumLen);
    }
    else
    {
		msgCAPK->CheckSumLen = 0;	//2021.9.9 lishiyao DF03不存在时不用校验公钥
    }


    pbTlv = TlvSeek(buf, ilen, 0xDF05);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(msgCAPK->ExpireDate, pb, TlvLen(pbTlv));
        TraceHex("emv", "msgCAPK.ExpireDate", msgCAPK->ExpireDate, 4);
    }
    TraceHex("", "msgcapk", msgCAPK->RID, sizeof(SDK_EMVBASE_CAPK_STRUCT));

	Trace("lishiyao", "DF03 Address = %p\r\n", pDF03);
	Trace("lishiyao", "9F06 Address = %p\r\n", p9F06);

	if(pDF03 < p9F06)
	{
		if(msgCAPK->CheckSumLen == 0)	//2021.9.9 lishiyao DF03不存在时不用校验公钥
		{

		}
		else
		{
			return SDK_ERR;
		}
    }
    checkLen = pDF03 - p9F06;
    Trace("emv", "checklen=%d\r\n", checkLen);
    TraceHex("emv", "checkbuf", buf, checkLen);
    sdkSHA1(buf, checkLen, checksum);
    TraceHex("emv", "checksum_hash", checksum, 20);

    //if(msgCAPK->CheckSumLen == 0)	//2021.9.9 lishiyao DF03不存在时不用校验公钥//The hash value in the background is wrong. The hash value is not verified here
    {
		memcpy(msgCAPK->CheckSum, checksum, 20);
    }

    if(memcmp(msgCAPK->CheckSum, checksum, 20) == 0)
    {
        if(msgCAPK->ArithInd == 0x04)
        {
//            ret = sdkEMVBaseAddAnyGMCAPKList(msgCAPK, 1);
        }
        else
        {
            checkData_size = 5 + 1 + msgCAPK->ModulLen + msgCAPK->ExponentLen;
            checkData = (u8*)sdkGetMem(checkData_size);

            if(NULL == checkData)
            {
                return SDK_ERR;
            }
            memset(checkData, 0, checkData_size);

            memcpy(checkData, msgCAPK->RID, 5);
            uiIndex = 5;
            checkData[5] = msgCAPK->CAPKI;
            uiIndex += 1;

            memcpy((u8*)&checkData[uiIndex], msgCAPK->Modul, msgCAPK->ModulLen);
            uiIndex += msgCAPK->ModulLen;

            memcpy((u8*)&checkData[uiIndex], msgCAPK->Exponent, msgCAPK->ExponentLen);
            uiIndex += msgCAPK->ExponentLen;


            sdkSHA1(checkData, uiIndex, checkSum);
            memcpy(msgCAPK->CheckSum, checkSum, msgCAPK->CheckSumLen);
            sdkFreeMem(checkData);
            ret = sdkEMVBaseAddAnyCAPKList(msgCAPK, 1);
        }
		if(ret == SDK_OK)
		{
	        sdkDispClearScreen();
	        sprintf((u8 *)DISP, "RID: %02X%02X%02X%02X%02X", msgCAPK->RID[0], msgCAPK->RID[1], msgCAPK->RID[2], msgCAPK->RID[3], msgCAPK->RID[4]);
	        sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP, SDK_DISP_DEFAULT);
	        sprintf((u8 *)DISP, "Index: %02X is OK.", (u16)msgCAPK->CAPKI);
	        sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP, SDK_DISP_DEFAULT);
	        sdkDispBrushScreen();
//	        sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
		}
		else
		{
	        sdkDispClearScreen();
	        sprintf((u8 *)DISP, "RID: %02X%02X%02X%02X%02X", msgCAPK->RID[0], msgCAPK->RID[1], msgCAPK->RID[2], msgCAPK->RID[3], msgCAPK->RID[4]);
	        sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP, SDK_DISP_DEFAULT);
	        sprintf((u8 *)DISP, "Index: %02X is ERR.", (u16)msgCAPK->CAPKI);
	        sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP, SDK_DISP_DEFAULT);
	        sdkDispBrushScreen();
	        sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
			sdkFreeMem(msgCAPK);
			return SDK_ERR;
	    }
    }
    else
    {
        sdkDispClearScreen();
        sprintf((u8 *)DISP, "RID: %02X%02X%02X%02X%02X", msgCAPK->RID[0], msgCAPK->RID[1], msgCAPK->RID[2], msgCAPK->RID[3], msgCAPK->RID[4]);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP, SDK_DISP_DEFAULT);
        sprintf((u8 *)DISP, "Index: %02X is ERR.", (u16)msgCAPK->CAPKI);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP, SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
    }
    sdkFreeMem(msgCAPK);
    return SDK_OK;
}


void BCTCUpDataParam_CAPK()
{
    u8 MsgType, ComPackSend[16]={0};
    u16 ComPackSendLen, ComPackRecvLen, Paraindex;
    u16 PararecordNum, PararecordLen, Parai;
    u8 recordType;
    s32 retCode = SDK_ERR;
    u8 ComPackRecv[1030]={0};
    u16 TagLen;
    bool EndFlag = 0;
    u8 step = 1;
    s32 len;
    u8 disp[32];

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Download CAPK", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

    sdkEMVBaseDelAllCAPKLists();
    memset(ComPackSend, 0, sizeof(ComPackSend));
    ComPackSendLen = 0;
    ComPackRecvLen = 0;
    MsgType = BCTC_MNG_DownloadCAPK_SEND;

    while(step)
    {
        if(step == 1)
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            MsgType = BCTC_MNG_DownloadCAPK_SEND;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        else
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        step++;

        while(!EndFlag)
        {
//			sdkmSleep(500);
            len = BCTCRecvData(ComPackRecv, 1030);
			Trace("lishiyao", "BCTCRecvData len = %d\r\n", len);
            if(len <= 0)
            {
                EndFlag = 1;
                retCode = SDK_ERR;
                Trace("lishiyao", "error flag1\r\n");
                break;
            }
			TraceHex("lishiyao", "BCTCRecvData:", ComPackRecv, len-1);
            ComPackRecvLen = len;

            if((ComPackRecv[1] != BCTC_MNG_DownloadCAPK_RECV) || (ComPackRecv[0] != 0x02))
            {
                EndFlag = 1;
                retCode = SDK_ERR;
				Trace("bctc", "2Byte:%02X\r\n", ComPackRecv[1]);
				Trace("bctc", "1Byte:%02X\r\n", ComPackRecv[2]);
                Trace("lishiyao", "error flag2\r\n");
                break;
            }

            if(memcmp(&ComPackRecv[4], "\x03\x01\x00", 3) == 0)	//下载完成
            {
                retCode = SDK_OK;
                EndFlag = 1;
                break;
            }
            TagLen = ComPackRecv[2] * 256 + ComPackRecv[3];

            TraceHex("emv", "CAPK recvdata", ComPackRecv+4, TagLen);

            if(SDK_OK == BCTCTlvToCAPKStruct(&ComPackRecv[4], TagLen))
            {
                retCode = SDK_OK;
            }
            else
            {
                EndFlag = 1;
                Trace("lishiyao", "error flag3\r\n");
                retCode = SDK_ERR;
            }
            break;
        }

        if(EndFlag)
        {
            break;
        }
    }

    sdkDispClearScreen();
	if(retCode == SDK_OK)
	{
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_OK, SDK_DISP_DEFAULT);
	}
	else
	{
		sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ERR, SDK_DISP_DEFAULT);
	}
    sdkDispBrushScreen();
	if(gstAutoTest == 0)
	{
    	sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
	}
}

s32 BCTCTlvToMSTemplate(u8 *buf, u16 len)
{
    u8 *pb, *pbTlv;
	u16 vLen;

	pbTlv = TlvSeek(buf, len, 0xBF70);
	memset(gstbctcupdatetemplate, 0, 850);
	gbctcupdatetemplatelen = 0;
	if(NULL != pbTlv && 0 != TlvLen(pbTlv))
	{
		gbctcupdatetemplatelen = TlvLen(pbTlv);
		Trace("BCTC", "Download Update Template len :%d\r\n", gbctcupdatetemplatelen);

		if(gbctcupdatetemplatelen > 0 && gbctcupdatetemplatelen<= 130)
		{
//			gbctcupdatetemplatelen -= 3;
			memcpy(gstbctcupdatetemplate, pbTlv+3, gbctcupdatetemplatelen);
			TraceHex("BCTC", "Download Update Template", gstbctcupdatetemplate, gbctcupdatetemplatelen);
		}
		else if(gbctcupdatetemplatelen > 127 && gbctcupdatetemplatelen<= 259)
		{
//			gbctcupdatetemplatelen -= 4;
			memcpy(gstbctcupdatetemplate, pbTlv+4, gbctcupdatetemplatelen);
			TraceHex("BCTC", "Download Update Template", gstbctcupdatetemplate, gbctcupdatetemplatelen);
		}
		else if(gbctcupdatetemplatelen > 259)
		{
//			gbctcupdatetemplatelen -= 5;
			memcpy(gstbctcupdatetemplate, pbTlv+5, gbctcupdatetemplatelen);
			TraceHex("BCTC", "Download Update Template", gstbctcupdatetemplate, gbctcupdatetemplatelen);
		}
	}

	pbTlv = TlvSeek(buf, len, 0xBF71);
	memset(gstbctcreadtemplate, 0, 850);
	gbctcreadtemplatelen = 0;
	if(NULL != pbTlv && 0 != TlvLen(pbTlv))
	{
		gbctcreadtemplatelen = TlvLen(pbTlv);
		Trace("BCTC", "Download Read Template len :%d\r\n", gbctcreadtemplatelen);
		memcpy(gstbctcreadtemplate, pbTlv+3, gbctcreadtemplatelen);
		TraceHex("BCTC", "Download Read Template", gstbctcreadtemplate, gbctcreadtemplatelen);
	}

	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Success", SDK_DISP_DEFAULT);
	sdkDispBrushScrecen();

	return SDK_OK;
}

s32 BCTCTlvToAIDStruct(u8 *buf, u16 ilen)
{
    u8 *pb, *pbTlv;
    SDK_EMVBASE_AID_STRUCT *tempAid;
	APPEX_AID_STRUCT *extempAid;
    u32 num;
	s32 ret;
	u8 DISP[33];
	u32 index = 0;
	u32 FloorLmt_Num=0;
	_SimData SimData={0};

    pbTlv = TlvSeek(buf, ilen, 0x9F06);

    if(pbTlv == NULL)
    {
        return SDK_ERR;
    }
    else
    {
        tempAid = (SDK_EMVBASE_AID_STRUCT *)sdkGetMem(sizeof(SDK_EMVBASE_AID_STRUCT));
        memset(tempAid, 0, sizeof(SDK_EMVBASE_AID_STRUCT));

//		tempAid->transvaule = 0xFF;
		tempAid->contactorcontactless = 0;

        pb = TlvVPtr(pbTlv);
        tempAid->AidLen = TlvLen(pbTlv);
        memcpy(tempAid->Aid, pb, tempAid->AidLen);

		extempAid = (APPEX_AID_STRUCT *)sdkGetMem(sizeof(APPEX_AID_STRUCT));
        memset(extempAid, 0, sizeof(APPEX_AID_STRUCT));

		extempAid->AidLen = TlvLen(pbTlv);
        memcpy(extempAid->Aid, pb, extempAid->AidLen);
    }

	TraceHex("Download AID", "Base AID Name", tempAid->Aid, tempAid->AidLen);
	TraceHex("Download AID", "Extern AID Name", tempAid->Aid, tempAid->AidLen);

	pbTlv = TlvSeek(buf, ilen, 0xDF01); 	//Asi

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);

        if(*pb)
        {
            tempAid->Asi = 0;	//不支持部分匹配
        }
        else
        {
            tempAid->Asi = 1;	//支持部分匹配
        }
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF11);    //TAC DEFAULT

    if(pbTlv != NULL)
    {
    	if( TlvLen(pbTlv) == 0)
    	{
    		Trace("HOST", "Background did not send TAC DEFAULT\r\n");
			memset(tempAid->TacDefault, 0xFF, 5);
    	}
		else
		{
			pb = TlvVPtr(pbTlv);
        	memcpy(tempAid->TacDefault, pb, TlvLen(pbTlv));
		}

    }

    pbTlv = TlvSeek(buf, ilen, 0xDF12);    //TacOnline

    if(pbTlv != NULL)
    {
    	if( TlvLen(pbTlv) == 0)
    	{
    		Trace("HOST", "Background did not send TacOnline\r\n");
			memset(tempAid->TacOnline, 0xFF, 5);
    	}
		else
		{
			pb = TlvVPtr(pbTlv);
        	memcpy(tempAid->TacOnline, pb, TlvLen(pbTlv));
		}

    }

	pbTlv = TlvSeek(buf, ilen, 0x9C);    //Trans type

    if(pbTlv != NULL)
    {
    	if( TlvLen(pbTlv) == 0)
    	{
    		Trace("HOST", "Background did not send Trans Type\r\n");
			tempAid->transvaule = 0xFF;
    	}
		else
		{
			pb = TlvVPtr(pbTlv);
        	memcpy( &(tempAid->transvaule), pb, TlvLen(pbTlv));
			memcpy(&(extempAid->TransType), pb, TlvLen(pbTlv));
		}
    }

	Trace("Download AID", "Base TransType: %02X\r\n", tempAid->transvaule);
	Trace("Download AID", "Extern TransType: %02X\r\n", extempAid->TransType);

    pbTlv = TlvSeek(buf, ilen, 0xDF13);    //TacDecline

    if(pbTlv != NULL)
    {
    	if( TlvLen(pbTlv) == 0)
    	{
    		Trace("HOST", "Background did not send TacDecline1\r\n");
			memset(tempAid->TacDecline, 0xFF, 5);
    	}
		else
		{
			 pb = TlvVPtr(pbTlv);
       		 memcpy(tempAid->TacDecline, pb, TlvLen(pbTlv));
		}

    }
	else
	{
			Trace("HOST", "Background did not send TacDecline2\r\n");
			memset(tempAid->TacDecline, 0xFF, 5);
	}

    pbTlv = TlvSeek(buf, ilen, 0xDF14);    //DEFAULT DDOL
    if(pbTlv != NULL)
    {
    	if(0 == TlvLen(pbTlv))
    	{

    	}
		else
		{
			pb = TlvVPtr(pbTlv);
       	 	tempAid->TermDDOLLen = TlvLen(pbTlv);
        	memcpy(tempAid->TermDDOL, pb, tempAid->TermDDOLLen);
		}

    }
    pbTlv = TlvSeek(buf, ilen, 0xDF15);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        sdkBcdToU32(&num, pb, TlvLen(pbTlv));
        sdkU32ToHex(tempAid->Threshold, num, 4);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF16);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        sdkBcdToU8(&(tempAid->MaxTargetPercent), pb, 1);
    }
    pbTlv = TlvSeek(buf, ilen, 0xDF17);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        sdkBcdToU8(&(tempAid->TargetPercent), pb, 1);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F1B);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		TraceHex("BCTC", "Download 9F1B", pb, 6);
		sdkBcdToU32(&FloorLmt_Num, pb, 6);
		FloorLmt_Num /= 100;
		Trace("BCTC", "trans to u32: %d\r\n", FloorLmt_Num);
		sdkU32ToHex(tempAid->FloorLimit, FloorLmt_Num, 4);
		TraceHex("BCTC", "trans to hex", tempAid->FloorLimit, 4);
	}

    pbTlv = TlvSeek(buf, ilen, 0x9F09);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(tempAid->AppVerNum, pb, 2);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F15);    //Merchant Category Code

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->MerchCateCode, pb, 2);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F16);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->MerchID, pb, 15);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F4E);    //Merchant Name and Location

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        extempAid->MerchantNameLen = TlvLen(pbTlv);
        memcpy(extempAid->MerchantName, pb, extempAid->MerchantNameLen);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F1C);    //Terminal Identification

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->TermID, pb, TlvLen(pbTlv));
    }

    pbTlv = TlvSeek(buf, ilen, 0x5F36);    //Transaction Currency Exponent

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        extempAid->TransCurcyExp = *pb;
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F3C);    //Transaction Reference Currency Cod

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->TransReferCurcyCode, pb, 2);
    }
    pbTlv = TlvSeek(buf, ilen, 0x9F3D);    //Transaction Reference Currency Exponent (n1)

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        extempAid->TransReferCurcyExp = *pb;
    }
    pbTlv = TlvSeek(buf, ilen, 0x5F2A);    //Transaction Currency Code

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->TransCurcyCode, pb, 2);
    }

    pbTlv = TlvSeek(buf, ilen, 0x9F01);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		if(TlvLen(pbTlv) < 6)
            memcpy(&extempAid->AcquireID[6 - TlvLen(pbTlv)], pb, TlvLen(pbTlv));
		else
		    memcpy(extempAid->AcquireID, pb, TlvLen(pbTlv));
    }

    tempAid->TermPinCap = 1;

    pbTlv = TlvSeek(buf, ilen, 0xDF8102);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        extempAid->TermTDOLLen = TlvLen(pbTlv);
        memcpy(extempAid->TermTDOL, pb, extempAid->TermTDOLLen);
    }
    TraceHex("Download AID Param", "TermTDOL", extempAid->TermTDOL, extempAid->TermTDOLLen);

    pbTlv = TlvSeek(buf, ilen, 0xDF19);

    if(pbTlv != NULL)
    {
    	if(0 == TlvLen(pbTlv))
    	{
    		memset(tempAid->cl_offlinelimit, 0xFF, 6);
    	}
		else
		{
			 pb = TlvVPtr(pbTlv);
        	memcpy(tempAid->cl_offlinelimit, pb, TlvLen(pbTlv));
		}
    }
	else
	{
		memset(tempAid->cl_offlinelimit, 0xFF, 6);
	}

	TraceHex("Download AID", "Contactless Floor Limit", tempAid->cl_offlinelimit, 6);

    pbTlv = TlvSeek(buf, ilen, 0xDF20);
    if(pbTlv != NULL)
    {
    	if(0 == TlvLen(pbTlv))
    	{
    		memset(tempAid->cl_translimit, 0xFF, 6);
    	}
		else
		{
			pb = TlvVPtr(pbTlv);
        	memcpy(tempAid->cl_translimit, pb, TlvLen(pbTlv));
		}
    }
	else
	{
		memset(tempAid->cl_translimit, 0xFF, 6);
	}
	TraceHex("Download AID", "Contactless Trans Limit", tempAid->cl_translimit, 6);

     pbTlv = TlvSeek(buf, ilen, 0xDF21);

    if(pbTlv != NULL)
    {
    	if(0 == TlvLen(pbTlv))
    	{
    		memset(tempAid->cl_cvmlimit, 0xFF, 6);
    	}
		else
		{
			pb = TlvVPtr(pbTlv);
       		memcpy(tempAid->cl_cvmlimit, pb, TlvLen(pbTlv));
		}
    }
	else
	{
		memset(tempAid->cl_cvmlimit, 0xFF, 6);
	}
	TraceHex("Download AID", "CVM Limit", tempAid->cl_translimit, 6);

	pbTlv = TlvSeek(buf, ilen, 0xDF41);	//2021.9.2 lishiyao add for JCB of BCTC New Host

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->RemovalTimeout, pb, TlvLen(pbTlv));
    }
	TraceHex("Download AID", "RemovalTimeout", extempAid->RemovalTimeout, 2);


	pbTlv = TlvSeek(buf, ilen, 0xDF1A);	//Zero amount allowed flag

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		extempAid->ZeroAmtAllowFlag = *pb;
    }
    Trace("Download AID Param", "ZeroAmtAllowFlag: %d\r\n", extempAid->ZeroAmtAllowFlag);

	pbTlv = TlvSeek(buf, ilen, 0xDF32);	//Status check supported flag
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		extempAid->StatusCheckFlag = *pb;
    }
    Trace("Download AID Param", "StatusCheckFlag: %d\r\n", extempAid->StatusCheckFlag);

	pbTlv = TlvSeek(buf, ilen, 0xDF37);	//Contactless application Capabilities

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->CLAppCap, pb, TlvLen(pbTlv));
    }
	TraceHex("Download AID", "Contactless application Capabilities", extempAid->CLAppCap, TlvLen(pbTlv));

	pbTlv = TlvSeek(buf, ilen, 0xDF39);	//Additional Tag Object List (ATOL)

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->ATOL, pb, TlvLen(pbTlv));
		extempAid->ATOLLen = TlvLen(pbTlv);
    }
	TraceHex("Download AID", "Additional Tag Object List (ATOL)", extempAid->ATOL, TlvLen(pbTlv));

	pbTlv = TlvSeek(buf, ilen, 0xDF40);	//Contactless POS implementation options
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		extempAid->Implementation = *pb;
    }
    Trace("Download AID Param", "Implementation options: %d\r\n", extempAid->Implementation);

	pbTlv = TlvSeek(buf, ilen, 0xDF44);	//Mandatory Tag Object List (MTOL)

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->MTOL, pb, TlvLen(pbTlv));
		extempAid->MTOLLen = TlvLen(pbTlv);
    }
	TraceHex("Download AID", "Mandatory Tag Object List (MTOL)", extempAid->MTOL, TlvLen(pbTlv));

	pbTlv = TlvSeek(buf, ilen, 0xDF45);	//Authentication Transaction Data Tag Object List (ATDTOL)

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        memcpy(extempAid->ATDTOL, pb, TlvLen(pbTlv));
		extempAid->ATOLLen = TlvLen(pbTlv);
    }
	TraceHex("Download AID", "Authentication Transaction Data Tag Object List (ATDTOL)", extempAid->ATDTOL, TlvLen(pbTlv));

	pbTlv = TlvSeek(buf, ilen, 0x9F76);	//Authentication Transaction Data Tag Object List (ATDTOL)

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		extempAid->TransDataLen = TlvLen(pbTlv);
		if(extempAid->TransDataLen > 256)
		{
			extempAid->TransDataLen = 256;
		}
        memcpy(extempAid->TransData, pb, extempAid->TransDataLen);
		TraceHex("Download AID", "Terminal Transaction Data(9F76)", extempAid->TransData, extempAid->TransDataLen);
    }

	ReadSimData(&SimData);
	pbTlv = TlvSeek(buf, ilen, 0xDF7F);	//Authentication Transaction Data Tag Object List (ATDTOL)
	if(NULL != pbTlv)
	{
		SimData.RestrictAIDLen = TlvLen(pbTlv);
		Trace("Download AID", "Restrict AID Len = %d\r\n", SimData.RestrictAIDLen);
		pb = TlvVPtr(pbTlv);
		memcpy(SimData.RestrictAID, pb, SimData.RestrictAIDLen);
		TraceHex("Download AID", "Restrict AID(DF7F)", SimData.RestrictAID, SimData.RestrictAIDLen);
	}
	SaveSimData(&SimData);

    ret = sdkEMVBaseAddAnyAIDList(tempAid, 1);
	Trace("BCTC", "sdkEMVBaseAddAnyAIDList ret = %d\r\n", ret);

    if(ret == SDK_OK)
	{
		memset(DISP,0,sizeof(DISP));

        sdkDispClearScreen();
        sdkBcdToAsc(DISP, tempAid->Aid, tempAid->AidLen);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP, SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Download OK.", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
		AddAPPEXAID(extempAid);
        sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
	}

    sdkFreeMem(tempAid);
	sdkFreeMem(extempAid);
    return ret;
}

void BCTCUpDataParam_AID()
{
    u8 MsgType, ComPackSend[16];
    u16 ComPackSendLen, ComPackRecvLen, Paraindex;
    u16 PararecordNum, PararecordLen, Parai;
    u8 recordType;
    s32 retCode = SDK_ERR;
    u8 *ComPackRecv = NULL;
    u16 TagLen;
    bool EndFlag = 0;
    u8 step = 1;
    s32 len;
    u8 disp[32] = {0};

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Download AID", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    sdkEMVBaseDelAllAIDLists();
	InitAPPEXAID();
    SaveAPPEXAID();

    memset(ComPackSend, 0, sizeof(ComPackSend));
    ComPackSendLen = 0;
    ComPackRecvLen = 0;

    MsgType = BCTC_MNG_DownloadAID_SEND;

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

    while(step)
    {
        if(ComPackRecv == NULL)
        {
            ComPackRecv = (u8 *)sdkGetMem(1024);
        }
        memset(ComPackRecv, 0, 1024);

        if(step == 1)
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            MsgType = BCTC_MNG_DownloadAID_SEND;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        else
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
//            ComPackSend[ComPackSendLen++] = 3;
//            ComPackSend[ComPackSendLen++] = 1;
//            ComPackSend[ComPackSendLen++] = 0;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        step++;

        while(!EndFlag)
        {
            len = BCTCRecvData(ComPackRecv, 1024);

            if(len <= 0)
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                EndFlag = 1;
                retCode = SDK_ERR;
                break;
            }
            ComPackRecvLen = len;

            if((ComPackRecv[1] != BCTC_MNG_DownloadAID_RECV) || (ComPackRecv[0] != 0x02))
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                EndFlag = 1;
                retCode = SDK_ERR;
                break;
            }

            if(memcmp(&ComPackRecv[4], "\x03\x01\x00", 3) == 0)	//2021.9.10 lishiyao 下载完成
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                retCode = SDK_OK;
                EndFlag = 1;
                break;
            }


            TagLen = ComPackRecv[2] * 256 + ComPackRecv[3];
            TraceHex("", "AID_recvdata ", ComPackRecv, TagLen + 4);

            if(SDK_OK == BCTCTlvToAIDStruct(&ComPackRecv[4], TagLen))
            {
                retCode = SDK_OK;
            }
            else
            {
                EndFlag = 1;
                retCode = SDK_ERR;
            }
            break;
        }

        if(EndFlag)
        {
            break;
        }
    }

    if(ComPackRecv != NULL)
    {
        sdkFreeMem(ComPackRecv);
    }
    sdkDispClearScreen();
	if(retCode == SDK_OK)
	{
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_OK, SDK_DISP_DEFAULT);
	}
	else
	{
		sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ERR, SDK_DISP_DEFAULT);
	}
    sdkDispBrushScreen();
	if(gstAutoTest == 0)
	{
		sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
	}
}

s32 TlvToTERMINFO(unsigned char *buf, int ilen)
{
    unsigned char *pb, *pbTlv;
    _SimData SimData;
    u8 time[8] = {0};
    u8 fn[64] = {0};
	s32 len;

    sdkSysGetCurAppDir(fn);
    strcat(fn, "SimData");

	ReadSimData(&SimData);

    pbTlv = TlvSeek(buf, ilen, 0xDF8104);

    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        if(*pb)
        {
            SimData.bBatchCapture = 0;
        }
        else
        {
            SimData.bBatchCapture = 1;
        }
    }

    pbTlv = TlvSeek(buf, ilen, 0x9F1A);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.TermCountryCode, pb, 2);
    }

    pbTlv = TlvSeek(buf, ilen, 0x5F36);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
        SimData.TransCurrencyExponent = *pb;
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F01);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.AcquirerID, pb, 6);
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F1E);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.IFD, pb, 8);
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F15);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.CategoryCode, pb, 2);
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F16);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.MerchantID, pb, 15);
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F4E);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.MerchantNo, pb, 23);
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F1C);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.TerminalNo, pb, 8);
    }

	pbTlv = TlvSeek(buf, ilen, 0x9F35);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		SimData.TermType = *pb;
    }

	pbTlv = TlvSeek(buf, ilen, 0x5F2A);
    if(pbTlv != NULL)
    {
        pb = TlvVPtr(pbTlv);
		memcpy(SimData.TransCurrencyCode, pb, 2);
    }
	SaveSimData(&SimData);

    return SDK_OK;
}



void BCTCUpDataParam_TERMINFO(void)
{
    u8 MsgType, *ComPackSend;
    u16 ComPackSendLen, ComPackRecvLen, Paraindex;
    u16 PararecordNum, PararecordLen, Parai;
    u8 recordType;
    s32 retCode;
    u8 *ComPackRecv;
    u16 TagLen;
    _SimData *SimData;
    u8 *tag;
    s32 taglen;
    s32 len;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Download Term Data", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    ComPackSend = (u8 *)sdkGetMem(256);
    memset(ComPackSend, 0, 256);

    ComPackSendLen = 0;
    ComPackRecvLen = 0;

    MsgType = BCTC_MNG_DownloadTermInfo_SEND;

    ComPackSend[0] = MsgType;
    ComPackSendLen++;
    SimData = (_SimData *)sdkGetMem(sizeof(_SimData));
    memset(SimData, 0, sizeof(_SimData));
    ReadSimData(SimData);

    memcpy(ComPackSend + ComPackSendLen, "\xDF\x81\x04\x01", 4);
    ComPackSendLen += 4;

    if(SimData->bBatchCapture)
    {
        ComPackSend[ComPackSendLen++] = 0;
    }
    else
    {
        ComPackSend[ComPackSendLen++] = 1;
    }
    sdkFreeMem(SimData);

    tag  = (u8 *)sdkGetMem(64);
    memset(tag, 0, 64);

	sdkEMVBaseTransInit();
 	IccUserTransInit();

    sdkEMVBaseReadTLV("\x9F\x35", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9F\x35", 2);
    ComPackSendLen += 2;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;
    sdkEMVBaseReadTLV("\x9F\x33", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9F\x33", 2);
    ComPackSendLen += 2;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;
    sdkEMVBaseReadTLV("\x9F\x40", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9F\x40", 2);
    ComPackSendLen += 2;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;

    /*
    //jcb bctc host not request 5F2A
    sdkEMVBaseReadTLV("\x5F\x2A", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x5F\x2A", 2);
    ComPackSendLen += 2;
    */

    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;
    sdkEMVBaseReadTLV("\x9F\x1A", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9F\x1A", 2);
    ComPackSendLen += 2;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;
    sdkEMVBaseReadTLV("\x9F\x1E", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9F\x1E", 2);
    ComPackSendLen += 2;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;
    sdkEMVBaseReadTLV("\x9A", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9A", 1);
    ComPackSendLen += 1;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;
    sdkEMVBaseReadTLV("\x9F\x21", tag, &taglen);
    memcpy(ComPackSend + ComPackSendLen, "\x9F\x21", 2);
    ComPackSendLen += 2;
    ComPackSend[ComPackSendLen++] = taglen;
    memcpy(ComPackSend + ComPackSendLen, tag, taglen);
    ComPackSendLen += taglen;

    sdkFreeMem(tag);
    BCTCSendData(ComPackSend, ComPackSendLen);
    sdkFreeMem(ComPackSend);
    ComPackRecv = (u8 *)sdkGetMem(256);
    memset(ComPackRecv, 0, 256);
    len = BCTCRecvData(ComPackRecv, 256);

    if(len <= 0)
    {
        sdkFreeMem(ComPackRecv);
        retCode = SDK_ERR;
        goto _EndTrans;
    }
    ComPackRecvLen = len;

    MsgType = ComPackRecv[1];

    if((MsgType != BCTC_MNG_DownloadTermInfo_RECV) || (ComPackRecv[0] != 0x02))
    {
        sdkFreeMem(ComPackRecv);
        retCode = SDK_ERR;
        goto _EndTrans;
    }
    TagLen = ComPackRecv[2] * 256 + ComPackRecv[3];

    TraceHex("TERMINFO", "TERMINFO_recvdata ", ComPackRecv, TagLen + 4);


    if(SDK_OK == TlvToTERMINFO(&ComPackRecv[4], TagLen))
    {
        retCode = SDK_OK;
    }
	else
	{
		retCode = SDK_ERR;
	}
    sdkFreeMem(ComPackRecv);

_EndTrans:
	sdkDispClearScreen();
	if(retCode == SDK_OK)
	{
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_OK, SDK_DISP_DEFAULT);
	}
	else
	{
		 sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ERR, SDK_DISP_DEFAULT);
	}
    sdkDispBrushScreen();
	if(gstAutoTest == 0)
	{

		sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
	}
}

/*
EG:
02D800069F3303E0B1C8
02D8001B9F1A0208409A031906059F2103170148DF810401009F3303E0B1C8
*/

void BCTCUpDataParam_Black()
{
    u8 MsgType, ComPackSend[16];
    u16 ComPackSendLen, ComPackRecvLen, Paraindex;
    u16 PararecordNum, PararecordLen, Parai;
    u8 recordType;
    s32 retCode = SDK_ERR;
    u8 *ComPackRecv = NULL;
    u16 TagLen;
    u8 EndFlag = 0;
    u8 step = 1;
    s32 len;
    u8 disp[32] = {0};
    u8 *pbTlv, *pb;
    u8 i = 0;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Download Black List", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

//    memset(gstbctcblack, 0, sizeof(gstbctcblack));
//    SaveBlackData();
    memset(ComPackSend, 0, sizeof(ComPackSend));
    ComPackSendLen = 0;
    ComPackRecvLen = 0;

	ComPackRecv = (u8 *)sdkGetMem(64);
	memset(ComPackRecv, 0, 64);
    MsgType = BCTC_MNG_DownloadBlackList_SEND;

    while(step)
    {
        memset(ComPackRecv, 0, 64);

        if(step == 1)
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        else
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
//            memcpy(ComPackSend + ComPackSendLen, "\x03\x04\x01\x00", 4);
//            ComPackSendLen += 4;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        step++;

        while(!EndFlag)
        {
            len = BCTCRecvData(ComPackRecv, 64);

            if(len <= 0)
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                EndFlag = 1;
                retCode = SDK_ERR;
                break;
            }
            ComPackRecvLen = len;

            if((ComPackRecv[1] != BCTC_MNG_DownloadBlackList_RECV) || (ComPackRecv[0] != 0x02))
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                EndFlag = 1;
                retCode = SDK_ERR;
                break;
            }

            if(memcmp(&ComPackRecv[4], "\x03\x01\x00", 3) == 0)
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                retCode = SDK_OK;
                EndFlag = 1;
                break;
            }
            TagLen = ComPackRecv[2] * 256 + ComPackRecv[3];
            TraceHex("emv", "BLACK_recvdata ", ComPackRecv, TagLen + 4);

            if(TagLen > 4)
            {
                for(i = 0; i < sizeof(gstbctcblack) / sizeof(BCTC_BLACK); i++)
                {
                    if(gstbctcblack[i].panlen == 0)
                    {
                        break;
                    }
                }

                if(i >= sizeof(gstbctcblack) / sizeof(BCTC_BLACK))
                {
                    i = 0;
                }
                pbTlv = TlvSeek(ComPackRecv + 4, TagLen, 0x5A);

                if(pbTlv != NULL)
                {
                    pb = TlvVPtr(pbTlv);
                    gstbctcblack[i].panlen = TlvLen(pbTlv);

                    if(gstbctcblack[i].panlen > 10)
                    {
                        gstbctcblack[i].panlen = 10;
                    }
                    memcpy(gstbctcblack[i].pan, pb, gstbctcblack[i].panlen);
                }
                else
                {
                    EndFlag = 1;
                }

                retCode = SDK_OK;
            }
            else
            {
                EndFlag = 1;
                retCode = SDK_ERR;
            }
            break;
        }

        if(EndFlag)
        {
            break;
        }
    }

    if(ComPackRecv != NULL)
    {
        sdkFreeMem(ComPackRecv);
    }

    for(i = 0; i < sizeof(gstbctcblack) / sizeof(BCTC_BLACK); i++)
    {
        if(gstbctcblack[i].panlen)
        {
            Trace("emv", "i = %d\r\n", i);
            TraceHex("emv", "pan", gstbctcblack[i].pan, gstbctcblack[i].panlen);
        }
    }

    SaveBlackData();
	Trace("BCTC", "flag3\r\n");
    sdkDispClearScreen();
	if(retCode == SDK_OK)
	{
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_OK, SDK_DISP_DEFAULT);
	}
	else
	{
		 sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ERR, SDK_DISP_DEFAULT);
	}
    sdkDispBrushScreen();
	if(gstAutoTest == 0)
	{
		sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
	}
}

void BCTCUpDataParam_MSTemplate(void)
{
	u8 MsgType, ComPackSend[16]={0};
	u16 ComPackSendLen=0, ComPackRecvLen=0, Paraindex;
	u16 PararecordNum, PararecordLen, Parai;
	u8 recordType;
	s32 retCode = SDK_ERR;
	u8 ComPackRecv[850]={0};
	u16 TagLen;
	u8 EndFlag = 0;
	u8 step = 1;
	s32 len;
	u8 disp[32] = {0};
	u8 *pbTlv, *pb;
	u8 i = 0;

	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Download MS Template", SDK_DISP_DEFAULT);
	sdkDispBrushScreen();

	MsgType = BCTC_MNG_DownloadDRL_SEND;

	memset(ComPackRecv, 0, 850);
	memset(ComPackSend, 0, sizeof(ComPackSend));
	ComPackSendLen = 0;
	ComPackRecvLen = 0;
	ComPackSend[0] = MsgType;
	ComPackSendLen++;
	BCTCSendData(ComPackSend, ComPackSendLen);

	len = BCTCRecvData(ComPackRecv, 850);

	if(len <= 0)
	{
		EndFlag = 1;
		return SDK_ERR;
	}
	ComPackRecvLen = len;

	if((ComPackRecv[1] != BCTC_MNG_DownloadDRL_RECV) || (ComPackRecv[0] != 0x02))
	{
		EndFlag = 1;
		return SDK_ERR;
	}

	if(memcmp(&ComPackRecv[4], "\x03\x01\x00", 3) == 0)
	{
		retCode = SDK_OK;
		return SDK_ERR;
	}

	TagLen = ComPackRecv[2] * 256 + ComPackRecv[3];
	TraceHex("emv", "Recv MS Template", ComPackRecv+4, TagLen);

	return BCTCTlvToMSTemplate(&ComPackRecv[4], TagLen);

}

void BCTCUpDataParam_PKRecova(void)
{
    u8 MsgType, ComPackSend[16];
    u16 ComPackSendLen, ComPackRecvLen, Paraindex;
    u16 PararecordNum, PararecordLen, Parai;
    u8 recordType;
    s32 retCode = SDK_ERR;
    u8 *ComPackRecv = NULL;
    u16 TagLen;
    u8 EndFlag = 0;
    u8 step = 1;
    s32 len;
    u8 disp[32] = {0};
    u8 *pbTlv, *pb;
    u8 i = 0;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Download Revocation PK", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    memset(gstbctcpkrecova, 0, sizeof(gstbctcpkrecova));
    SaveRecovaData();
	Trace("BCTC", "finish SaveRecovaData\r\n");
//    memset(ComPackSend, 0, sizeof(ComPackSend));
    ComPackSendLen = 0;
    ComPackRecvLen = 0;
	Trace("BCTC", "finish SaveRecovaData\r\n");
    MsgType = BCTC_MNG_DownloadRevocPK_SEND;
	ComPackRecv = (u8 *)sdkGetMem(64);
	memset(ComPackRecv, 0, 64);

    while(step)
    {
        if(step == 1)
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        else
        {
            memset(ComPackSend, 0, sizeof(ComPackSend));
            ComPackSendLen = 0;
            ComPackRecvLen = 0;
            ComPackSend[0] = MsgType;
            ComPackSendLen++;
//            memcpy(ComPackSend + ComPackSendLen, "\x03\x04\x01\x00", 4);
//            ComPackSendLen += 4;
            BCTCSendData(ComPackSend, ComPackSendLen);
        }
        step++;

        while(!EndFlag)
        {
            len = BCTCRecvData(ComPackRecv, 64);

            if(len <= 0)
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                EndFlag = 1;
                retCode = SDK_ERR;
                break;
            }
            ComPackRecvLen = len;

            if((ComPackRecv[1] != BCTC_MNG_DownloadRevocPK_RECV) || (ComPackRecv[0] != 0x02))
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                EndFlag = 1;
                retCode = SDK_ERR;
                break;
            }

            if(memcmp(&ComPackRecv[4], "\x03\x01\x00", 3) == 0)
            {
                sdkFreeMem(ComPackRecv);
                ComPackRecv = NULL;
                retCode = SDK_OK;
                EndFlag = 1;
                break;
            }
            TagLen = ComPackRecv[2] * 256 + ComPackRecv[3];
            TraceHex("emv", "PKRecova_recvdata ", ComPackRecv, TagLen + 4);

            if(TagLen > 4)
            {
                for(i = 0; i < sizeof(gstbctcpkrecova) / sizeof(BCTC_PKRECOVA); i++)
                {
                    if(memcmp(gstbctcpkrecova[i].RID, "\x00\x00\x00\x00\x00", 5) == 0)
                    {
                        break;
                    }
                }

                if(i >= sizeof(gstbctcpkrecova) / sizeof(BCTC_PKRECOVA))
                {
                    i = 0;
                }
                pbTlv = TlvSeek(ComPackRecv + 4, TagLen, 0x9F06);

                if(pbTlv != NULL)
                {
                    pb = TlvVPtr(pbTlv);
                    memcpy(gstbctcpkrecova[i].RID, pb, 5);
                }
                else
                {
                    EndFlag = 1;
                }
                pbTlv = TlvSeek(ComPackRecv + 4, TagLen, 0x8F);

                if(pbTlv != NULL)
                {
                    pb = TlvVPtr(pbTlv);
                    gstbctcpkrecova[i].index = *pb;
                }
                pbTlv = TlvSeek(ComPackRecv + 4, TagLen, 0xDF8105);

                if(pbTlv != NULL)
                {
                    pb = TlvVPtr(pbTlv);
                    memcpy(gstbctcpkrecova[i].sn, pb, 3);
                }
                retCode = SDK_OK;
            }
            else
            {
                EndFlag = 1;
                retCode = SDK_ERR;
            }
            break;
        }

        if(EndFlag)
        {
            break;
        }
    }

    if(ComPackRecv != NULL)
    {
        sdkFreeMem(ComPackRecv);
    }

    for(i = 0; i < sizeof(gstbctcpkrecova) / sizeof(BCTC_PKRECOVA); i++)
    {
        Trace("emv", "i = %d\r\n", i);
        TraceHex("emv", "RID", gstbctcpkrecova[i].RID, 5);
        Trace("emv", "index = %x\r\n", gstbctcpkrecova[i].index);
        TraceHex("emv", "SN", gstbctcpkrecova[i].sn, 3);
    }

    SaveRecovaData();
    sdkDispClearScreen();
	if(retCode == SDK_OK)
	{
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_OK, SDK_DISP_DEFAULT);
	}
	else
	{
		sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ERR, SDK_DISP_DEFAULT);
	}
    sdkDispBrushScreen();
	if(gstAutoTest == 0)
	{

		sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
	}
}


void BCTCPostUpDateParam(void)
{
    s32 key;

	sdkKbKeyFlush();
    while(1)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, DISP_BCTCHOSTMENU, SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_BCTCHOSTMENU1, SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_BCTCHOSTMENU2, SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, DISP_BCTCHOSTMENU3, SDK_DISP_LEFT_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, DISP_BCTCHOSTMENU4, SDK_DISP_LEFT_DEFAULT);

		sdkDispBrushScreen();

        key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 | SDK_KEY_MASK_5| SDK_KEY_MASK_6
                           | SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_UP | SDK_KEY_MASK_7 | SDK_KEY_MASK_8, 0);
		Trace("BCTC", "key = %02X\r\n", key);
        switch ( key )
        {
             case SDK_KEY_1:
               BCTCUpDataParam_CAPK();
               break;

             case SDK_KEY_2:
               BCTCUpDataParam_AID();
               break;

             case SDK_KEY_3:
               BCTCUpDataParam_TERMINFO();
               break;

             case SDK_KEY_4:
               BCTCUpDataParam_Black();
               break;

             case SDK_KEY_5:
               BCTCUpDataParam_PKRecova();
               break;

			case SDK_KEY_6:
				PostInitSysData();
               break;

			case SDK_KEY_7:
				BCTCUpDataParam_MSTemplate();
				break;

			case SDK_KEY_8:
				PostSetHostCommuType();
				 break;

             case SDK_KEY_ENTER:
             case SDK_KEY_DOWN:
             case SDK_KEY_UP:
             case SDK_KEY_ESC:
               break;

             default:
               break;
        }

        break;
    }
}


void UpDateParam()
{
	BCTCPostUpDateParam();
}

s32 BCTCSendReversal()
{
    s32 ComPackSendLen, ComPackRecvLen, VarReadLen, i;
    u8 MsgType;
    s32 len = 0;
    u8 *ComPackSend, *ComPackRecv;
    u16 Tlvlength = 0;
    u8 termtype;

	const u8 tagarray[][2] =
	{
		{0x82},
		{0x9F,0x36},
		{0x9F,0x1E},
		{0x9F,0x10},
		{0x9F,0x33},
		{0x95},
		{0x9B},
		{0x9F,0x02},
		{0x5F,0x24},
		{0x5A},
		{0x5F,0x34},
		{0x8A},
		{0x9F,0x39},
		{0x9F,0x1A},
		{0x9F,0x1C},
		{0x57},
		{0x81},
		{0x5F,0x2A},
		{0x9A},
		{0x9F,0x21},
		{0x9C},
		{0x9F,0x41},
		{0x9F,0x24},
		{0x9F,0x0A},
		{0x9F,0x35},
		{0x9F,0x74},
		{0x9F,0x79},
		{0x5F,0x20},
		{0x9F,0x5D},
		{0x9F,0x19},//20190529_lhd 4.3h
		{0},
	};

	const u8 tagarray1[][2] =
	{
		{0x9F,0x01},
		{0},
	};

	const u8 tagarray2[][2] =
	{
		{0x9F,0x15},
		{0x9F,0x16},
		{0},
	};


//    sdkDispClearScreen();
//    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_COMMUTOHOST, SDK_DISP_DEFAULT);
//    sdkDispBrushScreen();

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

    ComPackSendLen = 0;
    ComPackRecvLen = 0;

    MsgType = BCTC_TRS_Reversal_SEND;

    ComPackSend = (u8*)sdkGetMem(512);
    memset(ComPackSend, 0, 512);

    ComPackSend[0] = MsgType;
	ComPackSendLen = 1;
	FormTLVData(tagarray,ComPackSend+ComPackSendLen,&ComPackSendLen);

	if(SDK_OK == sdkEMVBaseReadTLV("\x9F\x35", &termtype, &len))
    {
        if(((termtype & 0xF0) == 0x10) || ((termtype & 0xF0) == 0x20))
        {
            FormTLVData(tagarray1,ComPackSend+ComPackSendLen,&ComPackSendLen);
        }

        if((termtype & 0xF0) == 0x20)
        {
            FormTLVData(tagarray2,ComPackSend+ComPackSendLen,&ComPackSendLen);
        }
    }

    TraceHex("emv", "Tlvdata", ComPackSend, ComPackSendLen);

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        sdkFreeMem(ComPackSend);
        return SDK_ERR;
    }
    sdkFreeMem(ComPackSend);

    ComPackRecv = (u8*)sdkGetMem(512);
    memset(ComPackRecv, 0, 512);

    len = BCTCRecvData(ComPackRecv, 512);

    if(len <= 0)
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }
    ComPackRecvLen = len;

    if((ComPackRecv[0] != 0x02) || (ComPackRecv[1] != BCTC_TRS_Reversal_RECV))
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }
    Tlvlength = ComPackRecv[2] * 256 + ComPackRecv[3];

    sdkFreeMem(ComPackRecv);
    return SDK_OK;
}


void SendReversal()
{
	BCTCSendReversal();
}

s32 BCTCSendConfirm(void)
{
    u8 *ComPackSend, *ComPackRecv;
    u16 ComPackSendLen;
    s32 ComPackRecvLen, VarReadLen;
    u8 MsgType;
    s32 len = 0;
    u16 Tlvlength = 0;

    const u8 tagarray[][2] =
	{
		{0x82},
		{0x9F,0x36},
		{0x9F,0x26},
		{0x9F,0x27},
		{0x9F,0x34},
		{0x9F,0x1E},
		{0x9F,0x10},
		{0x9F,0x33},
		{0x95},
		{0x9B},
		{0x9F,0x37},
		{0x9F,0x02},
		{0x9F,0x03},
		{0x81},
		{0x5F,0x24},
		{0x5F,0x25},
		{0x5A},
		{0x5F,0x34},
		{0x8A},
		{0x99},
		{0x9F,0x74},
		{0x9F,0x79},
		{0x9F,0x39},
		{0x9F,0x1A},
		{0x9F,0x1C},
		{0x57},
		{0x5F,0x2A},
		{0x9A},
		{0x9F,0x21},
		{0x9C},
		{0x9F,0x41},
		{0x9F,0x24},
		{0x9F,0x0A},
		{0x9F,0x07},
		{0x9F,0x6E},
		{0x9F,0x7C},
		{0x9F,0x63},
		{0x5F,0x20},
		{0x9F,0x5D},
		{0x9F,0x19},//20190529_lhd 4.3h
		{0x9F,0x35}, // 20200525 zzq
		{0x9F,0x25}, // 20200525 zzq
		{0x50},      // 20200602 for ca.143.00
		{0x9F,0x12}, // 20200602 for ca.143.00
		{0x9F,0x09}, // 20200602 for ca.143.00
		{0x9F,0x0B}, // 20200602 for ca.143.00
		{0x8E},      // 20200602 for ca.143.00
		{0x84},      // 20200602 for ca.143.00
		{0x9F,0x0D}, // 20200602 for ca.143.00
		{0x9F,0x0E}, // 20200602 for ca.143.00
		{0x9F,0x0F}, // 20200602 for ca.143.00
		{0x9F,0x11}, // 20200602 for ca.143.00
		{0x5F,0x28}, // 20200602 for ca.143.00
		{0x5F,0x2D}, // 20200602 for ca.143.00
		{0x9F,0x15}, // 20200602 for ca.143.00
		{0x9F,0x16}, // 20200602 for ca.143.00
		{0},
	};

    MsgType = BCTC_TRS_FinReqConfirm_SEND;

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }


	ComPackSend = (u8*)sdkGetMem(BCTC_SEND_LEN);
    memset(ComPackSend, 0, BCTC_SEND_LEN);

    ComPackSend[0] = MsgType;
	ComPackSendLen = 1;

    len = 0;
    sdkEMVBaseGetScriptResult(&len, ComPackSend+ComPackSendLen+3);
    if(len != 0)
    {
		memcpy(ComPackSend+ComPackSendLen,"\xDF\x31",2);
		ComPackSendLen += 2;
		ComPackSend[ComPackSendLen ++] = len;
		ComPackSendLen += len;
    }

	FormTLVData(tagarray,ComPackSend+ComPackSendLen,&ComPackSendLen);

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        sdkFreeMem(ComPackSend);
        return SDK_ERR;
    }
    sdkFreeMem(ComPackSend);

    ComPackRecv = (u8*)sdkGetMem(BCTC_RECV_LEN);
    memset(ComPackRecv, 0, BCTC_RECV_LEN);

    len = BCTCRecvData(ComPackRecv, BCTC_RECV_LEN);
    Trace("app","recv data len %d\r\n",len);

    if(len <= 0)
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }
    ComPackRecvLen = len;

    if((ComPackRecv[0] != 0x02) || (ComPackRecv[1] != BCTC_TRS_FinReqConfirm_RECV))
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }
    Tlvlength = ComPackRecv[2] * 256 + ComPackRecv[3];

    sdkFreeMem(ComPackRecv);
    CloseComm();

	return SDK_OK;
}

s32 BCTCSendMAGBase(u8 MsgType)
{
    u8 *ComPackSend, *ComPackRecv;
    u16 ComPackSendLen = 0, ComPackRecvLen = 0, VarReadLen = 0;
    s32 len = 0;
    u8 *pb, *pbTlv;
    u16 TlvLength = 0;
    u16 rsplen = 0;
	_SimData *SimData;
	u8 Amount[6] = {0};
	u8 tmpAmtAuthBin[4] = {0};
	u32 tmpsdkAmtAuthBin = 0;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_COMMUTOHOST, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

    ComPackSendLen = 0;
    ComPackRecvLen = 0;

	sdkAscToBcdR(Amount, gstasAmount, 6);
	sdkBcdToU32(&tmpsdkAmtAuthBin, Amount, 6);
	if(tmpsdkAmtAuthBin > 0xFFFFFFFF)
	{
		memcpy(tmpAmtAuthBin,"\xFF\xFF\xFF\xFF",4);
	}
	else
	{
	    sdkU32ToHex(tmpAmtAuthBin,tmpsdkAmtAuthBin, 4);
	}

	ComPackSend = (u8*)sdkGetMem(BCTC_SEND_LEN);
    memset(ComPackSend, 0, BCTC_SEND_LEN);
	ComPackSend[0] = MsgType;
	ComPackSendLen = 1;

	memcpy(ComPackSend+ComPackSendLen,"\x9F\x02\x06",3);
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,Amount,6);
	ComPackSendLen += 6;
	memcpy(ComPackSend+ComPackSendLen,"\x81\x04",2);
	ComPackSendLen += 2;
	memcpy(ComPackSend+ComPackSendLen,tmpAmtAuthBin,4);
	ComPackSendLen += 4;
	memcpy(ComPackSend+ComPackSendLen,"\x9F\x03\x06",3);
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,"\x00\x00\x00\x00\x00\x00",6);
	ComPackSendLen += 6;
	memcpy(ComPackSend+ComPackSendLen,"\x9F\x39\x01",3);//20190530_lhd
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,&gstmagcarddata.POSEntryMode,1);
	ComPackSendLen += 1;
	memcpy(ComPackSend+ComPackSendLen,"\x9F\x1C\x08",3);
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,"88888888", 8);
	ComPackSendLen += 8;
	memcpy(ComPackSend+ComPackSendLen,"\x9A\x03",2);
	ComPackSendLen += 2;
	memcpy(ComPackSend+ComPackSendLen,gstmagcarddata.TradeTime, 3);
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,"\x9F\x21\x03",3);
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,gstmagcarddata.TradeTime+3, 3);
	ComPackSendLen += 3;

	SimData = (_SimData *)sdkGetMem(sizeof(_SimData));
	memset(SimData, 0, sizeof(_SimData));
	ReadSimData(SimData);

	memcpy(ComPackSend+ComPackSendLen,"\x9F\x41\x04",3);
	ComPackSendLen += 3;
	memcpy(ComPackSend+ComPackSendLen,SimData->VocherNo, 4);
	ComPackSendLen += 4;

	sdkFreeMem(SimData);

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        sdkFreeMem(ComPackSend);
        return SDK_ERR;
    }
    sdkFreeMem(ComPackSend);

    ComPackRecv = (u8*)sdkGetMem(BCTC_RECV_LEN);

    if(ComPackRecv == NULL)
    {
        return SDK_ERR;
    }
    memset(ComPackRecv, 0, BCTC_RECV_LEN);

    len = BCTCRecvData(ComPackRecv, BCTC_RECV_LEN);

    if(len <= 0)
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }
    ComPackRecvLen = len;


	if((ComPackRecv[0] != 0x02))
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }

    TlvLength = ComPackRecv[2] * 256 + ComPackRecv[3];

    if(TlvLength >= 3)
    {
        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x8A);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 2)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            memcpy(gstResponseCode, pb, 2);
        }
        else
        {
            sdkFreeMem(ComPackRecv);
            return SDK_ERR;
        }
	}

    sdkFreeMem(ComPackRecv);
    return SDK_OK;
}


s32 BCTCSendICCBase(u8 MsgType)
{
    u8 *ComPackSend, ComPackRecv[512];
    s32 ComPackSendLen = 0, ComPackRecvLen = 0, VarReadLen = 0;
    s32 len = 0, heIssuerAuthDatalen = 0;
    u8 POSEntryMode = MAG_NO_IC;
    u8 *VarRead;
    u8 *heScript;
    u8 *pb, *pbTlv;
    u16 TlvLength = 0;
    u16 rsplen = 0;
    u16 Script71Len = 0, Script72Len = 0;
    u8 *pTempScript71, *pTempScript72;
    u8 termtype;
	u8 Amount[6] = {0};
	u8 tmpAmtAuthBin[4] = {0};
	u32 tmpsdkAmtAuthBin = 0;
	_SimData *SimData;

    const u8 icctagarray[][2] =
	{
		{0x82},
		{0x9F,0x36},
		{0x9F,0x26},
		{0x9F,0x27},
		{0x9F,0x34},
		{0x9F,0x1E},
		{0x9F,0x10},
		{0x9F,0x33},
		{0x95},
		{0x9B},
		{0x9F,0x37},
		{0x9F,0x02},
		{0x9F,0x03},
		{0x81},
		{0x5F,0x24},
		{0x5F,0x25},
		{0x5A},
		{0x5F,0x34},
		{0x8A},
		{0x99},
		{0x9F,0x74},
		{0x9F,0x79},
		{0x9F,0x39},
		{0x9F,0x1A},
		{0x9F,0x1C},
		{0x57},
		{0x5F,0x2A},
		{0x9A},
		{0x9F,0x21},
		{0x9C},
		{0x9F,0x41},
		{0x9F,0x24},
		{0x9F,0x0A},
		{0x9F,0x07},
		{0x9F,0x6E},
		{0x9F,0x7C},
		{0x9F,0x63},
		{0x5F,0x20},
		{0x9F,0x5D},
		{0x9F,0x19},//20190529_lhd 4.3h
		{0x9F,0x35}, // 20200525 zzq
		{0x9F,0x25}, // 20200525 zzq
		{0x50},      // 20200602 for ca.143.00
		{0x9F,0x12}, // 20200602 for ca.143.00
		{0x9F,0x09}, // 20200602 for ca.143.00
		{0x9F,0x0B}, // 20200602 for ca.143.00
		{0x8E},      // 20200602 for ca.143.00
		{0x84},      // 20200602 for ca.143.00
		{0x9F,0x0D}, // 20200602 for ca.143.00
		{0x9F,0x0E}, // 20200602 for ca.143.00
		{0x9F,0x0F}, // 20200602 for ca.143.00
		{0x9F,0x11}, // 20200602 for ca.143.00
		{0x5F,0x28}, // 20200602 for ca.143.00
		{0x5F,0x2D}, // 20200602 for ca.143.00
		{0},
	};

	const u8 tagarray1[][2] =
	{
		{0x9F,0x01},
		{0},
	};

	const u8 tagarray2[][2] =
	{
		{0x9F,0x15},
		{0x9F,0x16},
		{0},
	};

    ComPackSendLen = 0;
    ComPackRecvLen = 0;

	ComPackSend = (u8*)sdkGetMem(BCTC_SEND_LEN);
	memset(ComPackSend, 0, BCTC_SEND_LEN);
	ComPackSend[0] = MsgType;
	ComPackSendLen = 1;

    sdkEMVBaseReadTLV("\x9F\x39", &POSEntryMode, &len);

    if(POSEntryMode == MAG_LAST_FAIL_IC || POSEntryMode == MAG_NO_IC)
    {
    	sdkAscToBcdR(Amount, gstasAmount, 6);
		sdkBcdToU32(&tmpsdkAmtAuthBin, Amount, 6);
		if(tmpsdkAmtAuthBin > 0xFFFFFFFF)
		{
			memcpy(tmpAmtAuthBin,"\xFF\xFF\xFF\xFF",4);
		}
		else
		{
		    sdkU32ToHex(tmpAmtAuthBin,tmpsdkAmtAuthBin, 4);
		}

		memcpy(ComPackSend+ComPackSendLen,"\x9F\x02\x06",3);
		ComPackSendLen += 3;
		memcpy(ComPackSend+ComPackSendLen,Amount,6);
		ComPackSendLen += 6;
		memcpy(ComPackSend+ComPackSendLen,"\x81\x04",2);
		ComPackSendLen += 2;
		memcpy(ComPackSend+ComPackSendLen,tmpAmtAuthBin,4);
		ComPackSendLen += 4;
		memcpy(ComPackSend+ComPackSendLen,"\x9F\x03\x06",3);
		ComPackSendLen += 3;
		memcpy(ComPackSend+ComPackSendLen,"\x00\x00\x00\x00\x00\x00",6);
		ComPackSendLen += 6;
		memcpy(ComPackSend+ComPackSendLen,"\x9F\x39\x01",3);//20190530_lhd 35- 39
		ComPackSendLen += 3;
		memcpy(ComPackSend+ComPackSendLen,&gstmagcarddata.POSEntryMode,1);
		ComPackSendLen += 1;
		memcpy(ComPackSend+ComPackSendLen,"\x9F\x1C\x08",3);
		ComPackSendLen += 3;
		memcpy(ComPackSend+ComPackSendLen,"88888888", 8);
		ComPackSendLen += 8;
		memcpy(ComPackSend+ComPackSendLen,"\x9A\x03",2);
		ComPackSendLen += 2;
		memcpy(ComPackSend+ComPackSendLen,gstmagcarddata.TradeTime, 3);
		ComPackSendLen += 3;
		memcpy(ComPackSend+ComPackSendLen,"\x9F\x21\x03",3);
		ComPackSendLen += 3;
		memcpy(ComPackSend+ComPackSendLen,gstmagcarddata.TradeTime+3, 3);
		ComPackSendLen += 3;

		SimData = (_SimData *)sdkGetMem(sizeof(_SimData));
		memset(SimData, 0, sizeof(_SimData));
		ReadSimData(SimData);

		sdkFreeMem(SimData);

    }
    else
    {
    	FormTLVData(icctagarray,ComPackSend+ComPackSendLen,&ComPackSendLen);

		if(SDK_OK == sdkEMVBaseReadTLV("\x9F\x35", &termtype, &len))
		{
			if(((termtype & 0xF0) == 0x10) || ((termtype & 0xF0) == 0x20))
			{
				FormTLVData(tagarray1,ComPackSend+ComPackSendLen,&ComPackSendLen);
			}

			if((termtype & 0xF0) == 0x20)
			{
				FormTLVData(tagarray2,ComPackSend+ComPackSendLen,&ComPackSendLen);
			}
    	}
    }

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        sdkFreeMem(ComPackSend);
        return SDK_ERR;
    }
    sdkFreeMem(ComPackSend);

    memset(ComPackRecv, 0, sizeof(ComPackRecv));

    ComPackRecvLen = BCTCRecvData(ComPackRecv, sizeof(ComPackRecv));
	Trace("BCTC", "after BCTCRecvData return %d\r\n",ComPackRecvLen);

    if(len <= 0)
    {
        return SDK_ERR;
	}

	if(ComPackRecv[0] != 0x02)
    {
        return SDK_ERR;
    }
	else if(ComPackRecv[1] != BCTC_TRS_AuthReq_RECV)
	{
        return SDK_ERR;
	}
	else if(ComPackRecv[1] != BCTC_TRS_FinReq_RECV)
	{
        return SDK_ERR;
	}

    TlvLength = ComPackRecv[2] * 256 + ComPackRecv[3];
	TraceHex("BCTC", "Recv Pack", ComPackRecv, TlvLength+4);

    if(TlvLength >= 3)
    {
        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x8A);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 2)
            {
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            memcpy(gstResponseCode, pb, 2);
            TraceHex("lishiyao", "Recv Host 8A", pb, 2);
        }
        else
        {
            return SDK_ERR;
        }

        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x89);
        if(pbTlv != NULL)
        {
            pb = TlvVPtr(pbTlv);
            sdkEMVBaseConfigTLV("\x89", pb, 6);
        }

        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x91);
        if(pbTlv != NULL)
        {
            pb = TlvVPtr(pbTlv);
            heIssuerAuthDatalen = TlvLen(pbTlv);
			if(heIssuerAuthDatalen > 16)
			{
				heIssuerAuthDatalen = 16;
			}
			gstheIssuerAuthDataLen = heIssuerAuthDatalen;
            memcpy(gstheIssuerAuthData, pb, gstheIssuerAuthDataLen);
			sdkEMVBaseConfigTLV("\x91", gstheIssuerAuthData, gstheIssuerAuthDataLen);
			TraceHex("BCTC", "Issuer Auth Data", gstheIssuerAuthData, gstheIssuerAuthDataLen);
		}

        pTempScript71 = (u8*)sdkGetMem(256);

        if(pTempScript71 == NULL)
        {
            return SDK_ERR;
        }
        memset(pTempScript71, 0, 256);
        Script71Len = TlvSeekSame(&ComPackRecv[4], TlvLength, 0x71, pTempScript71);
		Trace("BCTC", "Script71Len = %d\r\n", Script71Len);

        TraceHex("", "pTempScript71", pTempScript71, Script71Len);

        pTempScript72 = (u8*)sdkGetMem(256);

        if(pTempScript72 == NULL)
        {
            sdkFreeMem(pTempScript71);
            return SDK_ERR;
        }
        memset(pTempScript72, 0, 256);
        Script72Len = TlvSeekSame(&ComPackRecv[4], TlvLength, 0x72, pTempScript72);

        TraceHex("", "pTempScript72", pTempScript72, Script72Len);

		gstheIssuerScriptLen = 0;
        if(Script71Len > 0)
        {
	        memcpy(gstheIssuerScriptData+gstheIssuerScriptLen, pTempScript71, Script71Len);
			gstheIssuerScriptLen += Script71Len;
			sdkEMVBaseConfigTLV("\x71", pTempScript71, Script71Len);
        }

        if(Script72Len > 0)
        {
			memcpy(gstheIssuerScriptData+gstheIssuerScriptLen, pTempScript72, Script72Len);
			gstheIssuerScriptLen += Script72Len;
			sdkEMVBaseConfigTLV("\x72", pTempScript72, Script72Len);
        }
	}

	if(pTempScript71)
	{
	    sdkFreeMem(pTempScript71);
	}
	if(pTempScript72)
	{
	    sdkFreeMem(pTempScript72);
	}

//    TraceHex("Auth", "gstheIssuerScriptData", gstheIssuerScriptData, gstheIssuerScriptLen);

    return SDK_OK;
}

s32 BCTCSendMAGAuthorRQ()
{
	return BCTCSendMAGBase(BCTC_TRS_AuthReq_SEND);
}

s32 BCTCSendAuthorRQ()
{
	return BCTCSendICCBase(BCTC_TRS_AuthReq_SEND);
}

s32 BCTCSendMagOnlineFina()
{
	return BCTCSendMAGBase(BCTC_TRS_FinReq_SEND);
}

s32 BCTCSendOnlineFina()
{
	return BCTCSendICCBase(BCTC_TRS_FinReq_SEND);
}

s32 BCTCProcessAdvice(void)
{
    s32 ComPackSendLen, ComPackRecvLen, VarReadLen, i;
    u8 MsgType;
    s32 len = 0;
    u8 *ComPackSend, *ComPackRecv;
    u8 termtype;

	const u8 icctagarray[][2] =
	{
		{0x82},
		{0x9F,0x36},
		{0x9F,0x26},
		{0x9F,0x27},
		{0x9F,0x34},
		{0x9F,0x1E},
		{0x9F,0x10},
		{0x9F,0x33},
		{0x95},
		{0x9B},
		{0x9F,0x37},
		{0x9F,0x02},
		{0x9F,0x03},
		{0x81},
		{0x5F,0x24},
		{0x5F,0x25},
		{0x5A},
		{0x5F,0x34},
		{0x8A},
		{0x99},
		{0x9F,0x74},
		{0x9F,0x79},
		{0x9F,0x39},
		{0x9F,0x1A},
		{0x9F,0x1C},
		{0x57},
		{0x5F,0x2A},
		{0x9A},
		{0x9F,0x21},
		{0x9C},
		{0x9F,0x41},
		{0x9F,0x24},
		{0x9F,0x0A},
		{0x9F,0x07},
		{0x9F,0x6E},
		{0x9F,0x7C},
		{0x9F,0x63},
		{0x5F,0x20},
		{0x9F,0x5D},
		{0x9F,0x19},//20190529_lhd 4.3h
		{0x9F,0x35}, // 20200525 zzq
		{0x9F,0x25}, // 20200525 zzq
		{0x50},      // 20200602 for ca.143.00
		{0x9F,0x12}, // 20200602 for ca.143.00
		{0x9F,0x09}, // 20200602 for ca.143.00
		{0x9F,0x0B}, // 20200602 for ca.143.00
		{0x8E},      // 20200602 for ca.143.00
		{0x84},      // 20200602 for ca.143.00
		{0x9F,0x0D}, // 20200602 for ca.143.00
		{0x9F,0x0E}, // 20200602 for ca.143.00
		{0x9F,0x0F}, // 20200602 for ca.143.00
		{0x9F,0x11}, // 20200602 for ca.143.00
		{0x5F,0x28}, // 20200602 for ca.143.00
		{0x5F,0x2D}, // 20200602 for ca.143.00
		{0},
	};

	const u8 tagarray1[][2] =
	{
		{0x9F,0x01},
		{0},
	};

	const u8 tagarray2[][2] =
	{
		{0x9F,0x15},
		{0x9F,0x16},
		{0},
	};

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

    MsgType = BCTC_TRS_Notify_SEND;

	ComPackSend = (u8*)sdkGetMem(BCTC_SEND_LEN);
	memset(ComPackSend, 0, BCTC_SEND_LEN);

	ComPackSend[0] = MsgType;
	ComPackSendLen = 1;

	FormTLVData(icctagarray,ComPackSend+ComPackSendLen,&ComPackSendLen);

	if(SDK_OK == sdkEMVBaseReadTLV("\x9F\x35", &termtype, &len))
    {
        if(((termtype & 0xF0) == 0x10) || ((termtype & 0xF0) == 0x20))
        {
            FormTLVData(tagarray1,ComPackSend+ComPackSendLen,&ComPackSendLen);
        }

        if((termtype & 0xF0) == 0x20)
        {
            FormTLVData(tagarray2,ComPackSend+ComPackSendLen,&ComPackSendLen);
        }
    }

	len = 0;

	sdkEMVBaseGetScriptResult(&len, ComPackSend+ComPackSendLen+3);
	if(len != 0)
	{
		memcpy(ComPackSend+ComPackSendLen,"\xDF\x31",2);
		ComPackSendLen += 2;
		ComPackSend[ComPackSendLen ++] = len;
		ComPackSendLen += len;
	}

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        sdkFreeMem(ComPackSend);
        CloseComm();
        return SDK_ERR;
    }
    sdkFreeMem(ComPackSend);

    ComPackRecv = (u8*)sdkGetMem(BCTC_RECV_LEN);
    memset(ComPackRecv, 0, BCTC_RECV_LEN);

    ComPackRecvLen = BCTCRecvData(ComPackRecv, BCTC_RECV_LEN);

    if(ComPackRecvLen <= 0)
    {
        sdkFreeMem(ComPackRecv);
        CloseComm();
        return SDK_ERR;
    }

    if((ComPackRecv[0] != 0x02) || (ComPackRecv[1] != BCTC_TRS_Notify_RECV))
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }

    sdkFreeMem(ComPackRecv);
    CloseComm();
    return SDK_OK;
}

s32 BCTCSendTransResult(s32 ret)
{
    u8 *ComPackSend, *ComPackRecv;
    s32 ComPackSendLen, ComPackRecvLen, VarReadLen;
    u8 MsgType;
    u8 *tmp;
    u8 TVR[5] = {0};
    u8 OdaResult = 0;
    s32 len = 0;
    u16 Tlvlength = 0;
    s32 tmp_len = 0;

	const u8 icctagarray[][2] =
	{
		{0x95},
		{0x9B},
		{0},
	};

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }


    MsgType = BCTC_MNG_TransResult_SEND;

    ComPackSend = (u8*)sdkGetMem(BCTC_SEND_LEN);
    memset(ComPackSend, 0, BCTC_SEND_LEN);

    ComPackSend[0] = MsgType;
	ComPackSendLen = 1;		//JCB不用上送03交易结果

	len = 0;

	sdkEMVBaseGetScriptResult(&len, ComPackSend+ComPackSendLen+3);
	if(len != 0)
	{
		memcpy(ComPackSend+ComPackSendLen,"\xDF\x31",2);	//脚本结果
		ComPackSendLen += 2;
		ComPackSend[ComPackSendLen ++] = len;
		ComPackSendLen += len;
	}

	memcpy(ComPackSend+ComPackSendLen,"\xDF\xC1\x0B",3); //ODA 结果
	ComPackSendLen += 3;
	ComPackSend[ComPackSendLen ++] = 1;
    //jcb OdaResult = ODA not perfor /CDA fail /CDA succes
	sdkEMVBaseReadTLV("\x95", TVR, &len);
	if(TVR[0] & 0x80)
	{
		OdaResult = 0x00;	//ODA未执行
	}
    /*
	else if(TVR[0] & 0x40)
	{
		OdaResult = 0x04;	//SDA失败
	}
	else if(TVR[0] & 0x20)
	{
		OdaResult = 0x04;
	}
	else if(TVR[0] & 0x08)
	{
		OdaResult = 0x06;	//DDA失败
	}
	*/
	else if(TVR[0] & 0x04)
	{
		OdaResult = 0x08;	//CDA失败
	}
	else
	{
		OdaResult = 0x07;//CDA success
	}
	ComPackSend[ComPackSendLen ++] = OdaResult;

	FormTLVData(icctagarray,ComPackSend+ComPackSendLen,&ComPackSendLen);

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        sdkFreeMem(ComPackSend);
//        sdkDispClearRowRam(SDK_DISP_LINE5);
//        sdkDispFillRowRam(SDK_DISP_LINE5, 0, DISP_ERR, SDK_DISP_DEFAULT);
//        sdkDispBrushScreen();
//        sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
        return SDK_ERR;
    }
    sdkFreeMem(ComPackSend);

    ComPackRecv = (u8*)sdkGetMem(BCTC_RECV_LEN);
    memset(ComPackRecv, 0, BCTC_RECV_LEN);

    len = BCTCRecvData(ComPackRecv, BCTC_RECV_LEN);

    if(len <= 0)
    {
        sdkFreeMem(ComPackRecv);
//        sdkDispClearRowRam(SDK_DISP_LINE5);
//        sdkDispFillRowRam(SDK_DISP_LINE5, 0, DISP_ERR, SDK_DISP_DEFAULT);
//        sdkDispBrushScreen();
//        sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
        return SDK_ERR;
    }
    len = ComPackRecvLen;

    if((ComPackRecv[0] != 0x02) || (ComPackRecv[1] != BCTC_MNG_TransResult_RECV))
    {
        sdkFreeMem(ComPackRecv);
//        sdkDispClearRowRam(SDK_DISP_LINE5);
//        sdkDispFillRowRam(SDK_DISP_LINE5, 0, DISP_ERR, SDK_DISP_DEFAULT);
//        sdkDispBrushScreen();
//        sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
        return SDK_ERR;
    }
    Tlvlength = ComPackRecv[2] * 256 + ComPackRecv[3];
    TraceHex("", "SendConfirm RecvData", ComPackRecv, Tlvlength + 4);

    sdkFreeMem(ComPackRecv);
    return SDK_OK;
}


s32 BCTCStartTrade(void)
{
    u8 ComPackSend[8], *ComPackRecv;
    u16 ComPackSendLen, ComPackRecvLen, VarReadLen;
    u8 MsgType;
    u8 *tmp;
    s32 len = 0;
    u16 Tlvlength = 0;
    u16 TlvLength = 0;
    u8 *pbTlv = NULL, *pb = NULL;
    u16 rsplen;
    u8 fn[64] = {0};

	Trace("Host", "Start BCTCStartTrade\r\n");

    memset(&gstbctcautotrade, 0, sizeof(gstbctcautotrade));

    MsgType = BCTC_MNG_StartTrade_SEND;

    memset(ComPackSend, 0, sizeof(ComPackSend));

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispClearScreen();
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispClearScreen();
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			return SDK_ERR;
		}
    }

    ComPackSend[0] = MsgType;
    ComPackSendLen = 1;

    if(0 >= BCTCSendData(ComPackSend, ComPackSendLen))
    {
        CloseComm();
        return SDK_ERR;
    }

    ComPackRecv = (u8*)sdkGetMem(BCTC_RECV_LEN);
    memset(ComPackRecv, 0, BCTC_RECV_LEN);

	Trace("Host", "Start BCTCRecvData\r\n");
    len = BCTCRecvData(ComPackRecv, BCTC_RECV_LEN);
	Trace("Host", "BCTCRecvData ret = %d\r\n", len);

    if(len <= 0)
    {
        sdkFreeMem(ComPackRecv);
        CloseComm();
        return SDK_ERR;
    }
    ComPackRecvLen = len;

    TraceHex("", "ComPackRecv", ComPackRecv, len);

    if((ComPackRecv[0] != 0x02) || (ComPackRecv[1] != BCTC_MNG_StartTrade_RECV))
    {
        sdkFreeMem(ComPackRecv);
        return SDK_ERR;
    }
    TlvLength = ComPackRecv[2] * 256 + ComPackRecv[3];
    Trace("", "tlvlength=%d\n", TlvLength);
    TraceHex("", "RecvData ", ComPackRecv, TlvLength + 4);

    if(TlvLength >= 3)
    {
        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x9F02);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 6)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.amountexist = 1;
            memcpy(gstbctcautotrade.amount, pb, 6);
            TraceHex("", "amount ", pb, 6);
        }
        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x9F03);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 6)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.otheramountexist = 1;
            memcpy(gstbctcautotrade.otheramount, pb, 6);
            TraceHex("", "otheramount ", pb, 6);
        }
        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x9C);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 1)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.typeexist = 1;
            memcpy(&gstbctcautotrade.transtype, pb, 1);
            Trace("BCTC", "trans type = %02X\r\n", gstbctcautotrade.transtype);
        }

        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x5F2A);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 2)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.currcodeexist = 1;
            memcpy(&gstbctcautotrade.currencycode, pb, 2);
            TraceHex("BCTC", "trans currency code", gstbctcautotrade.currencycode, 2);
        }

        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0x5F36);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 1)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.currexpexist = 1;
            memcpy(&gstbctcautotrade.currexp, pb, 1);
            Trace("BCTC", "trans currency exponent = %02X\r\n", gstbctcautotrade.currexp);
        }


        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0xDF8106);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 1)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            Trace("", "DF8106 = %d\r\n", *pb, 1);

            if((*pb) == 0)
            {
            }
            else if((*pb) == 0xFF)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            else
            {
                sdkFreeMem(ComPackRecv);
                sdkmSleep((*pb) * 1000);
                return SDK_ERR;
            }
        }
        pbTlv = TlvSeek(&ComPackRecv[4], TlvLength, 0xDF8103);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 1)
            {
                sdkFreeMem(ComPackRecv);
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            Trace("", "DF8103 = %d", *pb, 1);	//下载参数指示位

            if(((*pb) & 0x10) == 0x10)	//删除所有交易日志
            {
                InitTransData();
            }
            if(((*pb) & 0x01) == 0x01)	//下载CAPK
            {
				BCTCUpDataParam_CAPK();
            }
            if(((*pb) & 0x02) == 0x02) //下载AID
            {
				BCTCUpDataParam_AID();
            }
			if(((*pb) & 0x04) == 0x04)	//下载黑名单
			{
				BCTCUpDataParam_Black();
			}
			if(((*pb) & 0x08) == 0x08)	//下载回收公钥
			{
				BCTCUpDataParam_PKRecova();
			}
			if(((*pb) & 0x40) == 0x40)	//下载终端参数
			{
				BCTCUpDataParam_TERMINFO();
			}
        }
    }
    sdkFreeMem(ComPackRecv);
    return SDK_OK;
}

#if 0
s32 BCTCSingleTrade(void)
{
    u8 BCDComPackRecv[64]={0};
    s32 len = 0;
    u16 TlvLength = 0;
    u8 *pbTlv = NULL, *pb = NULL;
    u16 rsplen;

//	Trace("BCTC", "start BCTCSingleTrade\r\n");
    memset(&gstbctcautotrade, 0, sizeof(gstbctcautotrade));

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

    len = BCTCRecvData(BCDComPackRecv, 64);
    if(len <= 0)
    {
        return SDK_ERR;
    }

    TraceHex("", "ComPackRecv in BCD", BCDComPackRecv, len);

    if((BCDComPackRecv[0] != 0x02) || (BCDComPackRecv[1] != 0X80))
    {
        return SDK_ERR;
    }
    TlvLength = BCDComPackRecv[2] * 256 + BCDComPackRecv[3];
    Trace("", "tlvlength=%d\n", TlvLength);
//    TraceHex("", "RecvData ", BCDComPackRecv, TlvLength + 4);

    if(TlvLength >= 3)
    {
        pbTlv = TlvSeek(&BCDComPackRecv[4], TlvLength, 0x9F02);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 6)
            {
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.amountexit = 1;
            memcpy(gstbctcautotrade.amount, pb, 6);
            TraceHex("", "amount ", pb, 6);
        }
		else
		{
            gstbctcautotrade.amountexit = 0;
		}
        pbTlv = TlvSeek(&BCDComPackRecv[4], TlvLength, 0x9F03);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 6)
            {
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.otheramountexit = 1;
            memcpy(gstbctcautotrade.otheramount, pb, 6);
            TraceHex("", "otheramount ", pb, 6);
        }
		else
		{
            gstbctcautotrade.otheramountexit = 0;
		}
        pbTlv = TlvSeek(&BCDComPackRecv[4], TlvLength, 0x9C);

        if(pbTlv != NULL)
        {
            rsplen = TlvLen(pbTlv);

            if(rsplen != 1)
            {
                return SDK_ERR;
            }
            pb = TlvVPtr(pbTlv);
            gstbctcautotrade.typeexit = 1;
            memcpy(&gstbctcautotrade.transtype, pb, 1);
            Trace("emv", "transtype = %02X", gstbctcautotrade.transtype);
        }
		else
		{
			gstbctcautotrade.typeexit = 0;
		}
    }
    return SDK_OK;
}
#endif

void BCTCSendDataRecord(void)
{
    u8 ComPackSend[256],data[256];
    u16 ComPackSendLen, ComPackRecvLen;
    u8 MsgType,firstByte;
	u8 *tag;
	s32 dataLen=0,i,tagLen,protolLen,index;
	u8 tagList[][3] = {
		{0x9F,0x02,0x00},
		{0x9F,0x03,0x00},
		{0x9F,0x26,0x00},
		{0x82,0x00,0x00},
		{0x9F,0x36,0x00},
		{0x9F,0x27,0x00},
		{0x9F,0x10,0x00},
		{0x9F,0x1A,0x00},
		{0x95,0x00,0x00},
		{0x5F,0x2A,0x00},
		{0x9A,0x00,0x00},
		{0x9C,0x00,0x00},
		{0x9F,0x37,0x00},
		{0x9F,0x35,0x00},
		{0x57,0x00,0x00},
		{0x9F,0x34,0x00},
		{0x84,0x00,0x00},
		{0x5F,0x34,0x00},
		{0x5A,0x00,0x00},
		{0x9F,0x1F,0x00},
		{0x5F,0x20,0x00},
		{0x9F,0x77,0x00},
	};

    MsgType = BCTC_MNG_TermDispUI_SEND;

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

	ComPackSendLen = 0;
	ComPackSend[ComPackSendLen++] = MsgType;
	Trace("BCTC", "Msgtype: %02X\r\n", MsgType);
	Trace("BCTC", "ComPackSend[0]: %02X\r\n", ComPackSend[0]);
	Trace("BCTC", "ComPackSend[ComPackSendLen]: %02X\r\n", ComPackSend[ComPackSendLen]);

	memcpy(ComPackSend+ComPackSendLen, "\xFF\x81\x05", 4);
	ComPackSendLen += 4;
	index = ComPackSendLen - 1;

	protolLen = 0;
	for(i = 0; i < sizeof(tagList)/sizeof(tagList[0]); i++)
	{
		memset(data, 0, sizeof(data));
		firstByte = tagList[i][0];
		if ((firstByte & 0x1F) == 0x1F)
		{
			tagLen = 2;
		}
		else
		{
			tagLen = 1;
		}

		tag = (u8 *)sdkGetMem(tagLen);
		if(tag == NULL)
		{
			return;
		}
//		memset(tag, 0, tagLen);
		memcpy(tag, tagList[i], tagLen);

		if(sdkEMVBaseReadTLV(tag, data, &dataLen) == SDK_OK)
		{
			memcpy(ComPackSend+ComPackSendLen, tag, tagLen);
			ComPackSendLen += tagLen;
			protolLen += tagLen;
			ComPackSend[ComPackSendLen++] = dataLen;
			protolLen++;
			memcpy(ComPackSend+ComPackSendLen, data, dataLen);
			ComPackSendLen += dataLen;
			protolLen += dataLen;
		}

		sdkFreeMem(tag);
	}

	ComPackSend[index] = protolLen;

	BCTCSendData(ComPackSend, ComPackSendLen);
}

void BCTCSendOutCome(void)
{
    u8 ComPackSend[128], ComPackRecv[16];
    u16 ComPackSendLen, ComPackRecvLen;
    u8 MsgType;
	u8 flag=0x00;
	s32 ret;

    MsgType = BCTC_MNG_TermDispUI_SEND;

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

	ComPackSendLen = 0;
	ComPackSend[ComPackSendLen] = MsgType;
	Trace("BCTC", "Msgtype: %02X\r\n", MsgType);
	Trace("BCTC", "ComPackSend[0]: %02X\r\n", ComPackSend[0]);
	Trace("BCTC", "ComPackSend[ComPackSendLen]: %02X\r\n", ComPackSend[ComPackSendLen]);

    ComPackSendLen++;
	memcpy(ComPackSend+ComPackSendLen, "\xDF\x81\x29\x09", 4);
	ComPackSendLen += 4;
	ComPackSend[ComPackSendLen++] =	gstOutcome.Result;
	ComPackSend[ComPackSendLen++] =	gstOutcome.Start;
	ComPackSend[ComPackSendLen++] =	gstOutcome.OnlineResponseData;
	ComPackSend[ComPackSendLen++] =	gstOutcome.CVM;
	if(gstOutcome.UIRequestonOutcomePresent)
	{
		flag |= 0x80;
	}
	if(gstOutcome.UIRequestonRestartPresent)
	{
		flag |= 0x40;
	}
	if(gstOutcome.DataRecordPresent)
	{
		flag |= 0x20;
	}
	if(gstOutcome.DiscretionaryDataPresent)
	{
		flag |= 0x10;
	}
	if(gstOutcome.Receipt)
	{
		flag |= 0x08;
	}
	ComPackSend[ComPackSendLen++] =	flag;
	ComPackSend[ComPackSendLen++] =	gstOutcome.AlternateInterfacePreference;
	ComPackSend[ComPackSendLen++] =	gstOutcome.FieldOffRequest;
	memcpy(ComPackSend+ComPackSendLen, gstOutcome.RemovalTimeout, 2);
	ComPackSendLen+=2;

	ret = BCTCSendData(ComPackSend, ComPackSendLen);

	if(flag & 0x20)
	{
		BCTCSendDataRecord();
	}
//	CloseComm();
}

void BCTCSendUIRequest(int type)
{
    u8 ComPackSend[128], ComPackRecv[16];
    u16 ComPackSendLen, ComPackRecvLen;
    u8 MsgType;
	u8 flag=0x00;
	s32 ret;

    MsgType = BCTC_MNG_TermDispUI_SEND;

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }


	ComPackSendLen = 0;
    ComPackSend[ComPackSendLen++] = MsgType;
	if(PURE_UIREQ_OUTCOME == type)
	{
		memcpy(ComPackSend+ComPackSendLen, "\xDF\x81\x16\x0E", 4);
	}
	else if(PURE_UIREQ_RESTART == type)
	{
		memcpy(ComPackSend+ComPackSendLen, "\xDF\x81\x17\x0E", 4);
	}
	ComPackSendLen += 4;
	ComPackSend[ComPackSendLen++] =	gstUIRequest.MessageID;
	ComPackSend[ComPackSendLen++] =	gstUIRequest.Status;
	ComPackSend[ComPackSendLen++] =	gstUIRequest.HoldTime;
	memcpy(ComPackSend+ComPackSendLen, gstUIRequest.LanguagePerference, 2);
	ComPackSendLen += 2;
	ComPackSend[ComPackSendLen++] = gstUIRequest.ValueQualifier;
	memcpy(ComPackSend+ComPackSendLen, gstUIRequest.Value, 6);
	ComPackSendLen += 6;
	memcpy(ComPackSend+ComPackSendLen, gstUIRequest.CurrencyCode, 2);
	ComPackSendLen += 2;

	ret = BCTCSendData(ComPackSend, ComPackSendLen);
//	CloseComm();
}

s32 ProcessMagAuthorRQ()
{
    s32 retCode;

    retCode =  BCTCSendMAGAuthorRQ();

    if(retCode != SDK_OK)
    {
	    sdkDispClearScreen();
	    sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_ONLINERECEIVEERR, SDK_DISP_DEFAULT);
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_EXITERR, SDK_DISP_DEFAULT);
	    sdkDispBrushScreen();
	    sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);
	    return SDK_ERR;
    }
    return SDK_OK;
}


s32 ProcessAuthorRQ(u8 Authtype)
{
    s32 retCode;

    retCode =  BCTCSendAuthorRQ();

    if(retCode != SDK_OK)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_ONLINERECEIVEERR, SDK_DISP_DEFAULT);

        if(Authtype)
        {
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ONLINESWITCHOFFLINE, SDK_DISP_DEFAULT);
        }
        else
        {
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_EXITERR, SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);

        return SDK_ERR;
    }
    return SDK_OK;
}

s32 ProcessMagOnlineFina()
{
	s32 retCode;

	retCode = BCTCSendMagOnlineFina();

	if(retCode != SDK_OK)
	{
		sdkDispClearScreen();
		sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_ONLINERECEIVEERR, SDK_DISP_DEFAULT);
		sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_EXITERR, SDK_DISP_DEFAULT);
		sdkDispBrushScreen();
		sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);
		return SDK_ERR;
	}
	return SDK_OK;
}


s32 ProcessOnlineFina(u8 Authtype)
{
    s32 retCode;

    retCode = BCTCSendOnlineFina();

    if(retCode != SDK_OK)
	{
//		sdkDispClearScreen();
//		sdkDispFillRowRam(SDK_DISP_LINE2, 0, DISP_ONLINERECEIVEERR, SDK_DISP_DEFAULT);
//
//		if(Authtype)
//		{
//			sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ONLINESWITCHOFFLINE, SDK_DISP_DEFAULT);
//		}
//		else
//		{
//			sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_EXITERR, SDK_DISP_DEFAULT);
//		}
//		sdkDispBrushScreen();
//		sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);

		return SDK_ERR;
	}
    return SDK_OK;
}

s32 SendOnlineBag()
{
    s32 ret;

    ret = ProcessOnlineFina(1);
	return ret;
}


s32 PrintInit_My()
{
    return SDK_OK;
}

s32 PrintString_My(const u8 *pasStr)
{
    u8 *ComPackSend = NULL;
    u16 ComPackSendLen = 0;
    u8 oncestr[30] = {0};
    u16 i = 0;

    ComPackSend = (u8 *)sdkGetMem(64);

    if(ComPackSend == NULL)
    {
        return SDK_ERR;
    }

    if(strlen(pasStr) > 30 )
    {
        memset(oncestr, 0, sizeof(oncestr));

        for(i = 0; (i + 30) < strlen(pasStr); i += 30)
        {
            memcpy(oncestr, (u8 *)pasStr + i, 30);
            memset(ComPackSend, 0, 64);
            ComPackSendLen = 0;
            {
                ComPackSend[0] = 0xFF;
                ComPackSendLen++;
                memcpy(ComPackSend + ComPackSendLen, oncestr, 30);
                ComPackSendLen += 30;
                BCTCSendData(ComPackSend, ComPackSendLen);
                sdkmSleep(50);
            }
        }

        if(strlen(pasStr) > i)
        {
            memcpy(oncestr, (u8 *)pasStr + i, strlen(pasStr) - i);
            memset(ComPackSend, 0, 64);
            ComPackSendLen = 0;
            {
                ComPackSend[0] = 0xFF;
                ComPackSendLen++;
                memcpy(ComPackSend + ComPackSendLen, oncestr, strlen(pasStr) - i);
                ComPackSendLen += strlen(pasStr) - i;
                BCTCSendData(ComPackSend, ComPackSendLen);
                sdkmSleep(50);
            }
        }
    }
    else
    {
        {
            ComPackSend[0] = 0xFF;
            ComPackSendLen++;
            memcpy(ComPackSend + ComPackSendLen, (u8 *)pasStr, strlen(pasStr));
            ComPackSendLen += strlen(pasStr);
            BCTCSendData(ComPackSend, ComPackSendLen);
            sdkmSleep(50);
        }
    }
    sdkFreeMem(ComPackSend);
}

s32 PrintClose_My()
{
    return SDK_OK;
}

u8 *gstbctcprintdata = NULL;
u16 gstbctcprintdatalen;

s32 PrintInit_BCTC()
{
	if(gstbctcprintdata)
	{
		sdkFreeMem(gstbctcprintdata);
		gstbctcprintdata = NULL;
	}
	gstbctcprintdata = (u8 *)sdkGetMem(1024);
	memset(gstbctcprintdata,0,1024);
	gstbctcprintdatalen = 0;
    return SDK_OK;
}

s32 PrintString_BCTC(const u8 *pasStr)
{
	memcpy(gstbctcprintdata+gstbctcprintdatalen,pasStr,strlen(pasStr));
	gstbctcprintdatalen += strlen(pasStr);
	memcpy(gstbctcprintdata+gstbctcprintdatalen,"\r\n",2);
	gstbctcprintdatalen += 2;

	return SDK_OK;
}

s32 PrintClose_BCTC()
{
    u8 MsgType;
    u16 ComPackSendLen, ComPackRecvLen;
    s32 retCode = SDK_ERR;
    u8 *ComPackRecv = NULL,*ComPackSend = NULL;
    s32 len;

    MsgType = BCTC_MNG_UploadEcStrip_SEND;

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
		if(OpenComm() < 0)
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Serial Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
	}
    else if(HOST_TRANS_WIFI == gHostTransType)
    {
		if(!sdkGetWifiEnable())
		{
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Open Wifi Error", SDK_DISP_DEFAULT);
			return SDK_ERR;
		}
    }

	ComPackSendLen = 0;
	ComPackRecvLen = 0;

	ComPackSend = (u8 *)sdkGetMem(gstbctcprintdatalen + 1 + 3 + 3);
	memset(ComPackSend,0,(gstbctcprintdatalen + 1 + 3 + 3));

    ComPackSend[ComPackSendLen++] = MsgType;

	memcpy(ComPackSend+ComPackSendLen,"\xDF\xC1\x09",3);
	ComPackSendLen += 3;

	if(gstbctcprintdatalen > 255)
	{
		ComPackSend[ComPackSendLen++] = 0x82;
		sdkU16ToHex(ComPackSend+ComPackSendLen,gstbctcprintdatalen,2);
		ComPackSendLen += 2;
	}
	else if(gstbctcprintdatalen > 127)
	{
		ComPackSend[ComPackSendLen++] = 0x81;
		sdkU16ToHex(ComPackSend+ComPackSendLen,gstbctcprintdatalen,1);
		ComPackSendLen ++;
	}
	else
	{
		sdkU16ToHex(ComPackSend+ComPackSendLen,gstbctcprintdatalen,1);
		ComPackSendLen ++;
	}
	memcpy(ComPackSend+ComPackSendLen,gstbctcprintdata,gstbctcprintdatalen);
	ComPackSendLen += gstbctcprintdatalen;
	sdkFreeMem(gstbctcprintdata);
	gstbctcprintdata = NULL;

    BCTCSendData(ComPackSend, ComPackSendLen);

	ComPackRecv = (u8 *)sdkGetMem(1024);
	memset(ComPackRecv, 0, 1024);

    len = BCTCRecvData(ComPackRecv, 1024);

    if(len)
	{
        ComPackRecvLen = len;
        MsgType = ComPackRecv[1];

        if((MsgType == BCTC_MNG_UploadEcStrip_RECV) && (ComPackRecv[0] == 0x02))
		{
			retCode = SDK_OK;
		}
	}
	sdkFreeMem(ComPackSend);
	sdkFreeMem(ComPackRecv);

	#if 0
    sdkDispClearScreen();
	if(retCode == SDK_OK)
	{
	    sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_OK, SDK_DISP_DEFAULT);
	}
	else
	{
		 sdkDispFillRowRam(SDK_DISP_LINE3, 0, DISP_ERR, SDK_DISP_DEFAULT);
	}
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, TV_AUTORET);
	#endif

	return SDK_OK;
}

