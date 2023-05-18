#include "appglobal.h"

#include "extern.h"
//#include <stdarg.h>
#include <sys/stat.h>
//#include <unistd.h>
#include <stdio.h>
//#include <stdlib.h>

#include <android/log.h>
#include "sdkmaths.h"
#include "devApi.h"
#include <fcntl.h>
#include <dlfcn.h>



#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern DDI_API gstddiapi;


extern bool sdkIsBcdNum(u8 const *pheSrc, s32 siSrclen);

int sdkGetRtc(unsigned char *pbcDest)
{
    if(NULL == pbcDest)
    {
        return SDK_PARA_ERR;
    }
    ddi_sys_get_time(pbcDest);

    if(sdkIsBcdNum(pbcDest, 6))
    {
        return SDK_OK;
    }
    else
    {
        return SDK_PARA_ERR;
    }
}

void sdkmSleep(const int siMs)
{
	usleep(siMs);
}

int sdkGetRandom(unsigned char *pheRdm, int siNum)
{
	gstddiapi.ddi_get_random(pheRdm, siNum);
	TraceHex("ddi", "random:", pheRdm, siNum);
    return SDK_OK;
}

int sdkReadPosSn(unsigned char *pasDest)
{
    if(pasDest == NULL)
    {
        return SDK_PARA_ERR;
    }
    return 0;
}

void *sdkGetMem(unsigned int size)
{
	return ddi_k_malloc(size);
}

int sdkFreeMem(void *ap )
{
    ddi_k_free(ap);
    return 1;
}

int sdkSysGetCurAppDir(unsigned char *pasData)
{
    if(NULL == pasData) //shijianglong 2013.01.30 15:58
    {
        return SDK_PARA_ERR;
    }
    strcpy(pasData, "/data/local/config/app/");
    return SDK_OK;
}

int sdkKbGetKey(void)
{
    int tmpkey = sdk_dev_get_key();

	if(tmpkey)
	{
		//sdkSysBeep(SDK_SYS_BEEP_OK);
	}

    return tmpkey;
}

int sdkIccCloseRfDev(void)
{
	return 0;
}

int sdkIccDispRfLogo()
{
	return 0;
}

s32 sdkemvbaseInitDDI(void)
{
	void* pHandler;

	pHandler = dlopen("libAsdkClient_8623.so", RTLD_LAZY);
	if(pHandler == NULL)
	{
		Trace("lishiyao", "ddi-fatal:open libAsdkClient_8623.so fail\n");
		pHandler = dlopen("/system/lib/libAsdkClient.so", RTLD_LAZY);
		if(pHandler == NULL)
		{
			Trace("lishiyao", "ddi-fatal:open /system/lib/libAsdkClient.so fail\n");
			return -1;
		}
	}

	gstddiapi.ddi_apdu_exchange = dlsym(pHandler,"API_ICC_Apdu");
	gstddiapi.ddi_get_random= dlsym(pHandler,"API_Rng");
	gstddiapi.ddi_get_timerId= dlsym(pHandler,"API_TimeGet");
	gstddiapi.ddi_encry = dlsym(pHandler,"API_Crypt");
	gstddiapi.ddi_hash = dlsym(pHandler,"API_Hash");
	return 0;
}



s32 sdkDevContactlessSendAPDU(const u8 *pheInBuf, s32 siInLen, u8 *pheOutBuf, s32 *psiOutLen, u32 *sw)
{
    s32 ret = 0;
	u32 size = 512;
//	u32 sw = 0;

    if((NULL == pheInBuf) || (NULL == pheOutBuf) || (NULL == psiOutLen) || siInLen < 0)
    {
        return SDK_PARA_ERR;
    }

	TraceHex("apdu", "pheInBuf:", pheInBuf, siInLen);

	ret = gstddiapi.ddi_apdu_exchange(2, pheInBuf, (u32)siInLen,pheOutBuf, size, (u32 *)psiOutLen, sw);

	Trace("extern", "ddi_apdu_exchange ret = %d\r\n", ret);

    if(ret == DDI_OK)
    {
		Trace("apdu", "psiOutLen = %d\r\n", *psiOutLen);
    	TraceHex("emv", "contactless r-apdu:", pheOutBuf, *psiOutLen);
        return SDK_OK;
    }

    return SDK_ERR;
}


#define DDI_OK 0

#define SDK_MAX_PATH                40

s32 sdkInsertFile(const u8 *pasFile, const u8 *pheSrc, s32 siStart, s32 siSrclen)
{
	s32 fp,i,ret;

    if (NULL == pasFile || NULL == pheSrc || siStart < 0 || siSrclen < 0) {
		Trace("file", "flag1\r\n");
		return SDK_PARA_ERR;
    }

    if (siStart != 0)
	{
        i = sdkGetFileSize(pasFile);
        if (siStart > i && i > 0)
        {
			Trace("file", "siStart = %d\r\n", siStart);
			Trace("file", "i = %d\r\n", i);
			Trace("file", "flag2\r\n");
            return SDK_PARA_ERR;
   		}
    }
	Trace("ddi", "want open file:%s", pasFile);
	fp = fopen(pasFile, "rb+");
	if((NULL == fp) && (sdkGetFileSize(pasFile) <= 0))//file doesn't exist
	{
		fp = fopen(pasFile, "wb+");
		if(fp == NULL)
		{
			Trace("ddi", "open file error\r\n");
			return SDK_ERR;
		}
		else
		{
			fclose(fp);
			fp = fopen(pasFile, "rb+");
		}
	}
	Trace("ddi", "siStart = %d\r\n", siStart);
	fseek(fp, 0, SEEK_END);
	i = ftell(fp);
	if (siStart > i)
	{
		fclose(fp);
		Trace("file", "flag3\r\n");
		return SDK_PARA_ERR;
	}
	fseek(fp, siStart, SEEK_SET);

	ret = fwrite(pheSrc, sizeof(u8), siSrclen, fp);
	Trace("ddi", "fwrite ret = %d\r\n", ret);
	Trace("ddi", "siSrclen = %d\r\n");
//	if(siSrclen <= 1024)
//	{
//		TraceHex("ddi", "write content:", pheSrc, siSrclen);
//	}
	Trace("ddi", "flow flag1\r\n");
	if(ret == siSrclen)
	{
		Trace("ddi", "flow flag2\r\n");
		fclose(fp);
		return SDK_OK;
	}
	else
	{
		fclose(fp);
		return SDK_ERR;
	}
}

s32 sdkReadFile(const u8 *pasFile, u8 *pheDest, s32 siOffset, s32 *psiDestlen)
{
	s32 fp;
	u32 i;

    if (NULL == pasFile || NULL == pheDest || NULL == psiDestlen || siOffset < 0)
	{
        return SDK_PARA_ERR;
    }

	fp = fopen(pasFile, "rb+");
	if(fp == NULL)
	{
		Trace("lishiyao", "open file error!\r\n");
		return SDK_FUN_NULL;																						//�ļ���ʧ��
	}

    if( 0 != fseek(fp, siOffset, SEEK_SET) )
    {
        fclose(fp);
        return SDK_FILE_EOF;
    }

	i = *psiDestlen;
	*psiDestlen = fread(pheDest, sizeof(u8), i, fp);
	Trace("lishiyao", "act read len = %d\r\n", *psiDestlen);
	if(*psiDestlen != i) //consider it as get end of file
	{
		fclose(fp);
		return SDK_ERR;
	}
	else
	{
		fclose(fp);
		return SDK_OK;
	}
}

s32 sdkGetFileSize(const u8 *pasFile)
{
	s32 fileSize;
	s32 fp;

	if (NULL == pasFile)
	{
		return SDK_PARA_ERR;
	}

	fp = fopen(pasFile, "rb+");
	if(NULL == fp)
	{
		return SDK_FUN_NULL;																						//�ļ���ʧ��
	}

	fseek(fp, 0, SEEK_END);

	fileSize = ftell(fp);

	fclose(fp);

//	Trace("lishiyao", "sdkGetFileSize file size:%d\r\n", fileSize);
	return fileSize;
}

s32 sdkDelFile(const u8 *pasFile)
{
	s32 ret;

	if(pasFile == NULL)
	{
		return SDK_PARA_ERR;
	}

	ret = remove(pasFile);
	Trace("lishiyao", "want del file:%s\r\n", pasFile);
	Trace("lishiyao", "remove ret = %d\r\n", ret);
	if(ret == 0)
	{
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}
}



int beeper(int timer){
	return 0;
	return getBeeper(timer);
}

bool  sdkSysBeep(int eType)
{
	int rtn = DDI_OK;

	if(eType == 0)
	{
		rtn = beeper(3000);
		return SDK_OK;
	}
	else
	{
		rtn = beeper(1500);
	}
	return true;
}

void sdkDispClearScreen(){
    clearLcdLine(0, 4);
}
int sdkDispFillRowRam(int siRow, int siColid, const unsigned char *pasStr, unsigned int ucAtr){

    dispLcdLine(siRow, siColid, pasStr, ucAtr);
    return SDK_OK;
}
void sdkDispBrushScreen(void)
{

}

int sdkDispClearRowRam(int siRow){
    clearLcdLine(siRow, siRow);
    return SDK_OK;
}

int sdkDispRowAt(int siRow, int sicol, const unsigned char *pasStr, unsigned int ucAtr){
    dispLcdLine(siRow, sicol, pasStr, ucAtr);
    return SDK_OK;
}

int sdkDispClearRow(int siRow){
    clearLcdLine(siRow, siRow);
    return SDK_OK;
}

int sdkDispRow(int siRow, int eCol, const unsigned char * pasStr, unsigned int ucAtr){
     dispLcdLine(siRow, eCol, pasStr, ucAtr);
     return SDK_OK;
}

int sdk_dev_get_key(void){

    return getKey();
}

typedef struct
{
    unsigned int key;
    unsigned char keyname[8];
} STR_KEYTALE;

bool sdkSysIsNeedDispRfLogo()
{
	return false;//是否需要显示非接logo,因为如果需要显示,请挥卡的提示语不能挡到非接logo
}

s32 sdkSysGetColorValue(SDK_COLOR_ID eColorId)
{
    u32 color_value[2][2] = {{SDK_BLACK_COLOR_ID, 1},
                             {SDK_WHITE_COLOR_ID, 0}};
    u8 i = 0;

    for(i = 0; i < 2; i++)
    {
        if(eColorId == color_value[i][0])
        {
            return color_value[i][1];
        }
    }

    return SDK_ERR;
}

void sdkDev_Printf(char *fmt, ...)
{
	va_list args;
	unsigned char printbuffer[256] = {0};

	va_start ( args, fmt );
	vsprintf ( printbuffer, fmt, args );
	va_end ( args );
	LOGD("%s",printbuffer);
}

void sdkDispBrushScrecen()
{

}

void AddTestCardAID(void)
{
	SDK_EMVBASE_AID_STRUCT tempAid;
	APPEX_AID_STRUCT extempAid;

	memset(&tempAid, 0, sizeof(SDK_EMVBASE_AID_STRUCT));
	memcpy(tempAid.Aid, "\xA0\x00\x00\x02\x80\x20\x10", 7);
	tempAid.AidLen = 7;
	tempAid.Asi = 0;
	memcpy(tempAid.AppVerNum, "\x00\x01", 2);
	memcpy(tempAid.FloorLimit, "\x00\x00\x00\x3C", 4);
	memcpy(tempAid.TermDDOL, "\x9F\x37\x04", 3);
	tempAid.TermDDOLLen = 3;
	tempAid.TermPinCap = 1;
	tempAid.transvaule = 0x00;
	tempAid.contactorcontactless = 2;
	memcpy(tempAid.cl_cvmlimit, "\x00\x00\x00\x00\x25\x00", 6);
	memcpy(tempAid.cl_translimit, "\x00\x00\x00\x01\x00\x00", 6);
	memcpy(tempAid.cl_offlinelimit, "\x00\x00\x00\x00\x50\x00", 6);

	sdkEMVBaseAddAnyAIDList(&tempAid, 1);

	memset(&extempAid, 0, sizeof(APPEX_AID_STRUCT));
	memcpy(extempAid.Aid, "\xA0\x00\x00\x02\x80\x20\x10", 7);
	extempAid.AidLen = 7;
	memcpy(extempAid.TransCurcyCode, appex_aid_list[0].TransCurcyCode, 2);
	extempAid.TransCurcyExp = appex_aid_list[0].TransCurcyExp;
	memcpy(extempAid.TransReferCurcyCode, appex_aid_list[0].TransReferCurcyCode, 2);
	extempAid.TransReferCurcyExp = appex_aid_list[0].TransReferCurcyExp;
	memcpy(extempAid.AcquireID, appex_aid_list[0].AcquireID, 6);
	memcpy(extempAid.TermID, appex_aid_list[0].TermID, 8);
	memcpy(extempAid.MerchCateCode, appex_aid_list[0].MerchCateCode, 2);
	memcpy(extempAid.MerchID, appex_aid_list[0].MerchID, 15);
	extempAid.MerchantNameLen = appex_aid_list[0].MerchantNameLen;
	memcpy(extempAid.MerchantName, appex_aid_list[0].MerchantName, 20);
	extempAid.TermTDOLLen = appex_aid_list[0].TermTDOLLen;
	memcpy(extempAid.TermTDOL, appex_aid_list[0].TermTDOL, 64);
	memcpy(extempAid.TermTransPredicable, appex_aid_list[0].TermTransPredicable, 4);
	memcpy(extempAid.terminalcapability, appex_aid_list[0].terminalcapability, 3);
	extempAid.terminaltype = appex_aid_list[0].terminaltype;
	memcpy(extempAid.RemovalTimeout, appex_aid_list[0].RemovalTimeout, 2);
	extempAid.Implementation = appex_aid_list[0].Implementation;
	extempAid.ZeroAmtAllowFlag = appex_aid_list[0].ZeroAmtAllowFlag;
	extempAid.StatusCheckFlag = appex_aid_list[0].StatusCheckFlag;
	memcpy(extempAid.CLAppCap, appex_aid_list[0].CLAppCap, 5);
	memcpy(extempAid.ATOL, appex_aid_list[0].ATOL, 64);
	extempAid.ATOLLen = appex_aid_list[0].ATOLLen;
	memcpy(extempAid.MTOL, appex_aid_list[0].MTOL, 64);
	extempAid.MTOLLen = appex_aid_list[0].MTOLLen;
	memcpy(extempAid.ATDTOL, appex_aid_list[0].ATDTOL, 64);
	extempAid.ATDTOLLen = appex_aid_list[0].ATDTOLLen;

	AddAPPEXAID(&extempAid);
}
