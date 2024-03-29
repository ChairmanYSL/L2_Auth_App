#include "appglobal.h"

#include "extern.h"
//#include <stdarg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
//#include <stdlib.h>

#include <android/log.h>
#include "sdkmaths.h"
#include "devApi.h"
#include <fcntl.h>
#include <dlfcn.h>
#include <string.h>
#include "ddi_icc.h"
#include "ddi_result.h"
#include "ddi_manage.h"
#include <signal.h>
#include <time.h>
#include "extern.h"

#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern bool sdkIsBcdNum(u8 const *pheSrc, s32 siSrclen);
bool sdkTimerIsEnd(long uiId, u32 uiMs);
long sdkTimerGetId();



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

static volatile int timer_expired = 0;
static timer_t timerID;
u8 config_dir[64] = {0};


void timer_handler(int signum)
{
    timer_expired = 1;
	 timer_delete(timerID);
}

#if 0
int set_timer(int milliseconds)
{
    struct sigaction sa;
    struct itimerspec timerSpec;

    // 清除之前的定时器状态
    timer_expired = 0;

    // 设置信号处理函数
    memset(&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

    // 创建定时器
    if(timer_create(CLOCK_REALTIME, NULL, &timerID) == -1)
	{
        Trace("ddi", "timer_create");
        return -1;
    }

    // 设置定时器
    timerSpec.it_value.tv_sec = milliseconds / 1000;
    timerSpec.it_value.tv_nsec = (milliseconds % 1000) * 1000000;
    timerSpec.it_interval.tv_sec = 0;
    timerSpec.it_interval.tv_nsec = 0;

    if(timer_settime(timerID, 0, &timerSpec, NULL) == -1)
	{
        Trace("ddi", "timer_settime");
        return -1;
    }

    return 0;
}
#endif

//u32 sdkTimerGetId(void)
//{
//    u32 sys_tick;
//
//    if( DDI_OK == ddi_sys_get_tick(&sys_tick) )
//    {
//        return sys_tick;
//    }
//    else
//    {
//        Assert(0);
//        return 0;
//    }
//}


// 查询定时器是否过时
int is_timer_expired()
{
	if(timer_expired)
	{

	}
    return timer_expired;
}

int get_time_value(u8 *time)
{
	struct timeval tv;
	struct tm *tm;

    if (gettimeofday(&tv, NULL) == -1)
	{
        Trace("ddi", "gettimeofday error");
        return SDK_ERR;
    }

    tm = localtime(&tv.tv_sec);
    if (tm == NULL)
	{
        Trace("ddi", "localtime error");
        return SDK_ERR;
    }

	strftime(time, 128, "%Y-%m-%d- %H:%M:%S", tm);

	return SDK_OK;
}


int sdkIccDispRfLogo()
{
	return 0;
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

void sdkDispClearScreen()
{
	clearLcdLine(0, 4);
}
int sdkDispFillRowRam(int siRow, int siColid, unsigned char *pasStr, unsigned int ucAtr){

    dispLcdLine(siRow, siColid, (char *)pasStr, ucAtr);
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

int sdk_dev_get_key(void)
{
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

//u32 sdkTimerGetId(void)
//{
//    u32 sys_tick;
//
//    if( DDI_OK == ddi_sys_get_tick(&sys_tick) )
//    {
//        return sys_tick;
//    }
//    else
//    {
//        Assert(0);
//        return 0;
//    }
//}


void sdkmSleep(const s32 siMs)
{
//    u32 timeid1, timeid2;
//
////    ddi_sys_msleep(siMs);
//    timeid1 = sdkTimerGetId();
//    while(1){
//        timeid2 = sdkTimerGetId();
//        if((timeid2 - timeid1) > siMs){
//            break;
//        }
//    }
	usleep(siMs*1000);
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

void AddUnionPayCTAID(void)
{
	SDK_EMVBASE_AID_STRUCT tempAid;
	APPEX_AID_STRUCT extempAid;

	memset(&tempAid, 0, sizeof(SDK_EMVBASE_AID_STRUCT));
	memcpy(tempAid.Aid, "\xA0\x00\x00\x03\x33\x01\x01\x01", 8);
	tempAid.AidLen = 8;
	tempAid.Asi = 0;
	memcpy(tempAid.AppVerNum, "\x00\x01", 2);
	memcpy(tempAid.FloorLimit, "\x00\x00\x00\x3C", 4);
	memcpy(tempAid.TermDDOL, "\x9F\x37\x04", 3);
	tempAid.TermDDOLLen = 3;
	tempAid.TermPinCap = 1;
	tempAid.transvaule = 0x00;
	tempAid.contactorcontactless = 1;
	memcpy(tempAid.cl_cvmlimit, "\x00\x00\x00\x00\x25\x00", 6);
	memcpy(tempAid.cl_translimit, "\x00\x00\x00\x01\x00\x00", 6);
	memcpy(tempAid.cl_offlinelimit, "\x00\x00\x00\x00\x50\x00", 6);

	sdkEMVBaseAddAnyAIDList(&tempAid, 1);

	memset(&extempAid, 0, sizeof(APPEX_AID_STRUCT));
	memcpy(extempAid.Aid, "\xA0\x00\x00\x03\x33\x01\x01\x01", 8);
	extempAid.AidLen = 8;
//	memcpy(extempAid.TransCurcyCode, "\x09\x78", 2);
//	extempAid.TransCurcyExp = appex_aid_list[0].TransCurcyExp;
//	memcpy(extempAid.TransReferCurcyCode, appex_aid_list[0].TransReferCurcyCode, 2);
//	extempAid.TransReferCurcyExp = appex_aid_list[0].TransReferCurcyExp;
//	memcpy(extempAid.AcquireID, appex_aid_list[0].AcquireID, 6);
//	memcpy(extempAid.TermID, appex_aid_list[0].TermID, 8);
//	memcpy(extempAid.MerchCateCode, appex_aid_list[0].MerchCateCode, 2);
//	memcpy(extempAid.MerchID, appex_aid_list[0].MerchID, 15);
//	extempAid.MerchantNameLen = appex_aid_list[0].MerchantNameLen;
//	memcpy(extempAid.MerchantName, appex_aid_list[0].MerchantName, 20);
//	extempAid.TermTDOLLen = appex_aid_list[0].TermTDOLLen;
//	memcpy(extempAid.TermTDOL, appex_aid_list[0].TermTDOL, 64);
//	memcpy(extempAid.TermTransPredicable, appex_aid_list[0].TermTransPredicable, 4);
//	memcpy(extempAid.terminalcapability, appex_aid_list[0].terminalcapability, 3);
//	extempAid.terminaltype = appex_aid_list[0].terminaltype;
	memcpy(extempAid.RemovalTimeout, appex_aid_list[0].RemovalTimeout, 2);
	extempAid.Implementation = 0x26;
	extempAid.ZeroAmtAllowFlag = appex_aid_list[0].ZeroAmtAllowFlag;
	extempAid.StatusCheckFlag = appex_aid_list[0].StatusCheckFlag;
	memcpy(extempAid.CLAppCap, "\x36\x00\x40\x03\xF9", 5);
	memcpy(extempAid.ATOL, appex_aid_list[0].ATOL, 64);
	extempAid.ATOLLen = appex_aid_list[0].ATOLLen;
	memcpy(extempAid.MTOL, "\x8C\x00\x00\x57\x00\x00", 64);
	extempAid.MTOLLen = appex_aid_list[0].MTOLLen;
//	memcpy(extempAid.ATDTOL, appex_aid_list[0].ATDTOL, 64);
//	extempAid.ATDTOLLen = appex_aid_list[0].ATDTOLLen;

	AddAPPEXAID(&extempAid);
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
//	memcpy(extempAid.TransCurcyCode, "\x09\x78", 2);
//	extempAid.TransCurcyExp = appex_aid_list[0].TransCurcyExp;
//	memcpy(extempAid.TransReferCurcyCode, appex_aid_list[0].TransReferCurcyCode, 2);
//	extempAid.TransReferCurcyExp = appex_aid_list[0].TransReferCurcyExp;
//	memcpy(extempAid.AcquireID, appex_aid_list[0].AcquireID, 6);
//	memcpy(extempAid.TermID, appex_aid_list[0].TermID, 8);
//	memcpy(extempAid.MerchCateCode, appex_aid_list[0].MerchCateCode, 2);
//	memcpy(extempAid.MerchID, appex_aid_list[0].MerchID, 15);
//	extempAid.MerchantNameLen = appex_aid_list[0].MerchantNameLen;
//	memcpy(extempAid.MerchantName, appex_aid_list[0].MerchantName, 20);
//	extempAid.TermTDOLLen = appex_aid_list[0].TermTDOLLen;
//	memcpy(extempAid.TermTDOL, appex_aid_list[0].TermTDOL, 64);
//	memcpy(extempAid.TermTransPredicable, appex_aid_list[0].TermTransPredicable, 4);
//	memcpy(extempAid.terminalcapability, appex_aid_list[0].terminalcapability, 3);
//	extempAid.terminaltype = appex_aid_list[0].terminaltype;
	memcpy(extempAid.RemovalTimeout, appex_aid_list[0].RemovalTimeout, 2);
	extempAid.Implementation = 0x26;
	extempAid.ZeroAmtAllowFlag = appex_aid_list[0].ZeroAmtAllowFlag;
	extempAid.StatusCheckFlag = appex_aid_list[0].StatusCheckFlag;
	memcpy(extempAid.CLAppCap, "\x36\x00\x40\x03\xF9", 5);
	memcpy(extempAid.ATOL, appex_aid_list[0].ATOL, 64);
	extempAid.ATOLLen = appex_aid_list[0].ATOLLen;
	memcpy(extempAid.MTOL, "\x8C\x00\x00\x57\x00\x00", 64);
	extempAid.MTOLLen = appex_aid_list[0].MTOLLen;
//	memcpy(extempAid.ATDTOL, appex_aid_list[0].ATDTOL, 64);
//	extempAid.ATDTOLLen = appex_aid_list[0].ATDTOLLen;

	AddAPPEXAID(&extempAid);
}

unsigned int convertToUnsignedInt(const unsigned char* data, int length)
{
    unsigned int result = 0;
	int i;

    for(i = 0; i < length; i++)
	{
        result = result * 10 + (data[i] - '0');
    }
    return result;
}

const char* convertToCString(const unsigned char* data, int length)
{
    char* cString = (char*)malloc(length + 1);
    memcpy(cString, data, length);
    cString[length] = '\0';
    return cString;
}

const char* formatIPAddress(const char* cString)
{
    int len = strlen(cString);
    char formattedString[16] = {0};  // 最大IP地址长度为15，加上结尾的'\0'
    int formattedIndex = 0;
	int i;

    for(i = 0; i < len; i += 3)
	{
        if (i > 0)
		{
            formattedString[formattedIndex++] = '.';
        }
        formattedString[formattedIndex++] = cString[i];
        formattedString[formattedIndex++] = cString[i + 1];
        formattedString[formattedIndex++] = cString[i + 2];
    }
    formattedString[formattedIndex] = '\0';
    return strdup(formattedString);
}

void removeLeadingZeros(char* ipAddress) {
    if (ipAddress == NULL) {
        return;
    }

    int len = strlen(ipAddress);
    char* delimiter = ".";
    char* token = strtok(ipAddress, delimiter);
    int firstToken = 1;

    while (token != NULL) {
        int value = atoi(token);
        char strippedValue[4];
        sprintf(strippedValue, "%d", value);

        // 移除无意义的前导零
        char trimmedValue[4];
        sprintf(trimmedValue, "%d", value);

        if (firstToken) {
            strcpy(ipAddress, trimmedValue);
            firstToken = 0;
        } else {
            strcat(ipAddress, ".");
            strcat(ipAddress, trimmedValue);
        }

        token = strtok(NULL, delimiter);
    }
}



unsigned char IntBitMapLen(int num)
{
	unsigned char i;

	for(i = 0; i < 10; i++)
	{
		if(num / 10 > 0)
		{
			num /= 10;
		}
		else
		{
			break;
		}
	}

	return i+1;
}

void SendTCPTest(void)
{
	u8 data[] = "\x11\x22\x33\x44";

	BCTCSendData(data, 4);
}

//unsigned char pure_Outcome_FinancialTransactionCompleted_TransactionApproved(PURETradeUnionStruct *tempApp_UnionStruct)
//{
//	unsigned char CVM = tempApp_UnionStruct->EMVTradeParam->PureCVMParameter;
//	unsigned char CVMRes;
//
//	emvbase_avl_gettagvalue_spec(EMVTAG_CVMResult, &CVMRes, 2, 1);
//	if(0x00 != CVMRes && 0x02 != CVMRes)
//	{
//		CVM = SDK_OUTCOME_CVM_NA;
//	}
//
//	tempApp_UnionStruct->SetOutcome(SDK_OUTCOME_RESULT_APPROVED, SDK_OUTCOME_START_NA, CVM, 1, 0, 1, 1, SDK_OUTCOME_AIP_CONTACTCHIP, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
//	tempApp_UnionStruct->SendOutcome();
//	sdkmSleep(100);
//	if(SDK_OUTCOME_CVM_OBTAINSIGNATURE == tempApp_UnionStruct->EMVTradeParam->PureCVMParameter)
//	{
//		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_APPROVEDSIGN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
//	}
//	else
//	{
//		tempApp_UnionStruct->SetUIRequest(SDK_UI_MSGID_APPROVED, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
//	}
//
//	tempApp_UnionStruct->SendUIRequest(PURE_UIREQ_OUTCOME);
//	return RLT_EMV_OFFLINE_APPROVE;
//}


void HostOutcomeTest(void)
{

	sdkEMVBaseTransInit();
	sdkEMVBaseConfigTLV("\x9F\x02", "\x00\x00\x00\x00\x00\x01", 6);
	sdkEMVBaseConfigTLV("\x9F\x03", "\x00\x00\x00\x00\x00\x00", 6);
	sdkEMVBaseConfigTLV("\x9F\x26", "\xAA\xBB\xCC\xDD\xEE\xFF\x11\x22", 8);
	sdkEMVBaseConfigTLV("\x82", "\x1D\x0D", 2);
	sdkEMVBaseConfigTLV("\x9F\x36", "\x00\x1D", 2);
	sdkEMVBaseConfigTLV("\x9F\x27", "\x40", 1);
	sdkEMVBaseConfigTLV("\x9F\x10", "\x10\x20\x30\x40\x50\x60\x70\x80", 8);
	sdkEMVBaseConfigTLV("\x9F\x1A", "\x09\x78", 2);
	sdkEMVBaseConfigTLV("\x95", "\x00\x00\x00\x00\x00", 5);

	sdkEMVBaseConfigTLV("\x9F\x06", "\xD9\x99\x99\x99\x99\x10\x10", 7);
	sdkEMVBaseConfigTLV("\x9C", "\x00", 1);

	sdkCleanOutcomeParam();
	sdkSetOutcomeParam(SDK_OUTCOME_RESULT_APPROVED, SDK_OUTCOME_START_NA, 0x20, 1, 0, 1, 1, SDK_OUTCOME_AIP_CONTACTCHIP, 0, SDK_OUTCOME_FIELDOFFREQ_NA, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	BCTCSendOutCome();
	sdkmSleep(100);

	sdkCleanUIRequestParam();
	sdkSetUIRequestParam(SDK_UI_MSGID_APPROVEDSIGN, SDK_UI_STATUS_CARDREADSUCCESS, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	BCTCSendUIRequest(PURE_UIREQ_OUTCOME);
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

int sdkGetRandom(unsigned char *pheRdm, int siNum)
{
	u16 outlen;
	ddi_manage_get_random((u16)siNum, pheRdm,&outlen);
	TraceHex("ddi", "random:", pheRdm, outlen);
    return SDK_OK;
}

unsigned short sdkReadPosSn(unsigned char *pasDest)
{
	u16 outlen=64;
    if(pasDest == NULL)
    {
        return SDK_PARA_ERR;
    }

	ddi_manage_get_sn(pasDest, &outlen);
    return outlen;
}

#define SLOT_IC_CARD 0x00
#define SLOT_RF_CARD 0x04

s32 sdkIccOpenIcDev(void)
{
	return ddi_icc_open(SLOT_IC_CARD);
}

s32 sdkIccGetCardStatus(s32 siTimerCnt)
{
    s32 rslt;
    long TimerID;
	u8 buf[6] = {0};

    if(siTimerCnt < 0)
    {
        return SDK_PARA_ERR;
    }
    TimerID = sdkTimerGetId();

    while(1)
    {
        rslt = ddi_icc_read_card_status(SLOT_IC_CARD, buf);
		Trace("ddi", "ddi_icc_read_card_status get card status: %02X\r\n", buf[1]);

		if(buf[1] == 0x01)
        {
            return SDK_OK;
        }

        if( 0 == siTimerCnt || sdkTimerIsEnd(TimerID, siTimerCnt))
        {
            return SDK_ICC_NOCARD;
        }
    }
}

s32 sdkIccCloseIcDev(void)
{
	ddi_icc_power_off(SLOT_IC_CARD);
	return ddi_icc_close(SLOT_IC_CARD);
}

int sdkIccOpenRfDev()
{
	return ddi_icc_open(SLOT_RF_CARD);
}

int sdkIccPowerOnAndSeek()
{
	u8 buf[6];
//	long TimerID = 0;
	int ret,rslt,key;
//s32 siTimerCnt

//    if(siTimerCnt < 0)
//    {
//        return SDK_PARA_ERR;
//    }
//    TimerID = sdkTimerGetId();


//    while(1)
//    {
		memset(buf, 0, sizeof(buf));
		ret = ddi_icc_read_card_status(SLOT_RF_CARD, buf);
		Trace("ddi", "ddi_icc_read_card_status get card status: %02X\r\n", buf[1]);
	//		Trace("ddi", "ddi_icc_read_card_status ret: %d\r\n", ret);

		if(buf[1] == 0x00)	//No card
		{
			return SDK_ICC_NOCARD;
		}
		else if(buf[1] == 0x02) //Multi Card Collision
		{
			return SDK_ICC_MUTICARD;
		}
		else if(buf[1] == 0x03 || buf[1] == 0x04)
		{
			return SDK_OK;
		}
		else
		{
			return SDK_ERR;
		}

//        if( 0 == siTimerCnt || sdkTimerIsEnd(TimerID, siTimerCnt))
//        {
//            return SDK_ICC_NOCARD;
//        }
//	}

}

int sdkIccResetIcc(u8 cardMode)
{
	u8 atr[100]={0};
	u8 len,ic_card_type;
	int ret;

	if(cardMode == SDK_ICC_ICC)
	{
		ret = ddi_icc_power_on(SLOT_IC_CARD, 0x01, atr, &len, &ic_card_type);
	}
	else if(cardMode == SDK_ICC_RF)
	{
		ret = ddi_icc_power_on(SLOT_RF_CARD, 0x06, atr, &len, &ic_card_type);
	}

	if(0x00 == ret)
	{
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}
}

int sdkIccPowerDown()
{
	return ddi_icc_power_off(0x06);
}

int sdkIccCloseRfDev(void)
{
	return ddi_icc_close(SLOT_RF_CARD);
}

int sdkIccRemoveCard(void)
{
	return ddi_icc_rf_remove();
}

void APDUTest(void)
{
	s32 key,ret;
	u8 detect_card_flag=0;
	u8 ppse_cmd[]="\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31\x00";
	u8 ppse_rsp[64]={0};
	u16 recv_len=10;
	u8 cmd1[]="\x00\xA4\x04\x00\x07\xD9\x99\x99\x99\x99\x10\x10\x00";

	sdkDispClearScreen();
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Plz tap card", SDK_DISP_DEFAULT);
	sdkDispBrushScrecen();

	ret = sdkIccOpenRfDev();
	Trace("test", "sdkIccOpenRfDev ret = %d\r\n", ret);
	if(ret != 0)
	{
		sdkDispClearRow(SDK_DISP_LINE2);
		sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Open RF fail", SDK_DISP_DEFAULT);
		sdkDispBrushScrecen();
	}
	else
	{
		sdkDispClearRow(SDK_DISP_LINE2);
		sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Open RF success", SDK_DISP_DEFAULT);
		sdkDispBrushScrecen();
	}

	while (1)
	{
		sdkmSleep(1000);
		key = sdkKbGetKey();
		if(SDK_KEY_ESC == key)
		{
			return;
		}

		ret = sdkIccPowerOnAndSeek();
		Trace("test", "sdkIccPowerOnAndSeek ret = %d\r\n", ret);
		if(ret == SDK_OK)
		{
			sdkDispClearRow(SDK_DISP_LINE3);
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Check Card success", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			detect_card_flag = 1;
			break;
		}
		else if(SDK_ICC_MUTICARD == ret)
		{
			sdkDispClearRow(SDK_DISP_LINE3);
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Multi card collision", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
		}
		else if(SDK_ICC_NOCARD == ret)
		{
			continue;
		}
		else
		{
			sdkDispClearRow(SDK_DISP_LINE3);
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Check Card error", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			break;
		}

	}

	if(detect_card_flag)
	{
		ret = sdkIccResetIcc(SDK_ICC_RF);
		Trace("test", "sdkIccResetIcc ret = %d\r\n", ret);
		if(ret == SDK_OK)
		{
			sdkDispClearRow(SDK_DISP_LINE4);
			sdkDispFillRowRam(SDK_DISP_LINE4, 0, "Reset card success", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();

			ret = ddi_icc_trans_apdu(0x04, ppse_cmd, 20, ppse_rsp, &recv_len);
			Trace("test", "ddi_icc_trans_apdu ret = %d\r\n", ret);
			if(ret == DDI_OK)
			{
				sdkDispClearRow(SDK_DISP_LINE4);
				sdkDispFillRowRam(SDK_DISP_LINE4, 0, "APDU success", SDK_DISP_DEFAULT);
				sdkDispBrushScrecen();
			}
			else
			{
				sdkDispClearRow(SDK_DISP_LINE4);
				sdkDispFillRowRam(SDK_DISP_LINE4, 0, "APDU fail", SDK_DISP_DEFAULT);
				sdkDispBrushScrecen();
				return;
			}
		}
		else
		{
			sdkDispClearRow(SDK_DISP_LINE4);
			sdkDispFillRowRam(SDK_DISP_LINE4, 0, "Reset card fail", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			return;
		}
	}

	ddi_icc_power_off(0x06);
	ddi_icc_close(SLOT_RF_CARD);

	sdkmSleep(3000);


	sdkDispClearRow(SDK_DISP_LINE2);
	sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Second Tap", SDK_DISP_DEFAULT);
	sdkDispBrushScrecen();

	ret = sdkIccOpenRfDev();
	Trace("test", "sdkIccOpenRfDev ret = %d\r\n", ret);
	if(ret != 0)
	{
		sdkDispClearRow(SDK_DISP_LINE2);
		sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Open RF fail", SDK_DISP_DEFAULT);
		sdkDispBrushScrecen();
	}
	else
	{
		sdkDispClearRow(SDK_DISP_LINE2);
		sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Open RF success", SDK_DISP_DEFAULT);
		sdkDispBrushScrecen();
	}

	while (1)
	{
		sdkmSleep(500);
		key = sdkKbGetKey();
		if(SDK_KEY_ESC == key)
		{
			return;
		}

		ret = sdkIccPowerOnAndSeek();
		Trace("test", "sdkIccPowerOnAndSeek ret = %d\r\n", ret);
		if(ret == SDK_OK)
		{
			sdkDispClearRow(SDK_DISP_LINE3);
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Check Card success", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			detect_card_flag = 1;
			break;
		}
		else if(SDK_ICC_MUTICARD == ret)
		{
			sdkDispClearRow(SDK_DISP_LINE3);
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Multi card collision", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
		}
		else if(SDK_ICC_NOCARD == ret)
		{
			continue;
		}
		else
		{
			sdkDispClearRow(SDK_DISP_LINE3);
			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Check Card error", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
			break;
		}

	}

	if(detect_card_flag)
	{
		ret = sdkIccResetIcc(SDK_ICC_RF);
		Trace("test", "sdkIccResetIcc ret = %d\r\n", ret);
		if(ret == SDK_OK)
		{
			sdkDispClearRow(SDK_DISP_LINE4);
			sdkDispFillRowRam(SDK_DISP_LINE4, 0, "Reset card success", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();

			ret = ddi_icc_trans_apdu(0x04, cmd1, sizeof(cmd1), ppse_rsp, &recv_len);
			Trace("test", "ddi_icc_trans_apdu ret = %d\r\n", ret);
		}
		else
		{
			sdkDispClearRow(SDK_DISP_LINE4);
			sdkDispFillRowRam(SDK_DISP_LINE4, 0, "Reset card fail", SDK_DISP_DEFAULT);
			sdkDispBrushScrecen();
		}
	}

	return;
}

void RandNumTest(void)
{
	u8 randnum[4]={0};
	int ret=0,i;

	ret = sdkGetRandom(randnum, 4);
	Trace("test", "sdkGetRandom ret = %d\r\n", ret);

	for(i = 0;i < 500;i++)
	{
		sdkGetRandom(randnum, 4);
		serial_send(gSerialPortId, randnum, 4);
	}
}

void ReadSNTest(void)
{
	u8 input[] = {
	0x04,0x22,0x22,0x99,0x99,0x99,0x99,0x99,
	0x99,0xFF,0xFF,0x12,0x49,0x00,0x00,0x01,
	0x01,0x01,0x7C,0x01,0x9D,0xFE,0x17,0x68,
	0xB7,0x8C,0x0F,0xCE,0x8C,0x1D,0xEF,0xFB,
	0x7C,0x00,0x67,0x4F,0x38,0x48,0xD3,0xE3,
	0xE8,0x2D,0x61,0x02,0xF6,0x09,0x0C,0x4B,
	0x7F,0xFC,0x3F,0x9A,0xF9,0xC4,0x03,0x10,
	0x08,0xB1,0x61,0x48,0x48,0xA0,0x8D,0x92,
	0x46,0x7D,0x44,0xCE,0x87,0x4C,0x8F,0xE3,
	0xE2,0x41,0x28,0x80,0xD7,0xED,0x6B,0x0C,
	0x74,0xDD,0x08,0x70,0x13,0xA8,0x74,0xFE,
	0x23,0x85,0x4E,0x5D,0xED,0x90,0x96,0x58,
	0xF4,0x55,0xAB,0x08,0x4A,0xD6,0xB8,0x30,
	0xD0,0xCB,0x6A,0x0A,0x3C,0x7B,0xC2,0x43,
	0xFD,0xBF,0xC3,0x7E,0xD8,0x50,0xA2,0x90,
	0x03,0x30,0xF0,0x83,0xA8,0x3F,0xC6,0x90,
	0xAF,0x62,0x00,0xC0,0xDB,0x5C,0x1C,0xD7,
	0xB1,0xB1,0x2D,0x31,0xD2,0xFD,0x8A,0x21,
	0x03,0x5F,0x20,0x05,0x55,0x53,0x45,0x52,
	0x31,0x8C,0x18,0x9F,0x02,0x06,0x9F,0x03,
	0x06,0x9F,0x1A,0x02,0x95,0x05,0x5F,0x2A,
	0x02,0x9A,0x03,0x9C,0x01,0x9F,0x37,0x04,
	0x9F,0x34,0x03,0x8D,0x1A,0x8A,0x02,0x9F,
	0x02,0x06,0x9F,0x03,0x06,0x9F,0x1A,0x02,
	0x95,0x05,0x5F,0x2A,0x02,0x9A,0x03,0x9C,
	0x01,0x9F,0x37,0x04,0x9F,0x34,0x03,0x9F,
	0x08,0x02,0x00,0x01,0x9F,0x07,0x02,0xFF,
	0xC0,0x5F,0x28,0x02,0x00,0x56,0x9F,0x42,
	0x02,0x09,0x78,0x8E,0x10,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x42,0x03,0x5E,
	0x03,0x1F,0x03,0x00,0x00,0x5F,0x25,0x03,
	0x10,0x10,0x04,0x5F,0x24,0x03,0x49,0x12,
	0x31,0x5A,0x08,0x22,0x22,0x99,0x99,0x99,
	0x99,0x99,0x99,0x5F,0x34,0x01,0x00,0x9F,
	0x0D,0x05,0x00,0x00,0x00,0x00,0x00,0x9F,
	0x0E,0x05,0x00,0x00,0x00,0x00,0x00,0x9F,
	0x0F,0x05,0x00,0x00,0x00,0x00,0x00,0x57,
	0x13,0x22,0x22,0x99,0x99,0x99,0x99,0x99,
	0x99,0xD4,0x91,0x27,0x02,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x9F,0x49,0x03,0x9F,
	0x37,0x04,0x7D,0x00};
	u8 output[20]={0};

	TraceHex("test", "input", input, sizeof(input));
	SHA1(input, sizeof(input), output);
	TraceHex("test", "output", output, 20);

}

void DisplayStringTest(void)
{
	int i;

	for(i = 0; i < 1000; i++)
	{
		sdkTestIccDispText("Multi Card Collision");
		sdkTestIccDispText("Read Card error,Tx Stop");
		sdkTestIccDispText("Online Approve");
		sdkTestIccDispText("Online Decline");
		sdkTestIccDispText("Offline Approve");
		sdkTestIccDispText("Offline Decline");
		sdkTestIccDispText("Switch Interface");
		sdkTestIccDispText("See Phone");
		sdkTestIccDispText("End Application");
	}
}

int sdkSysSetCurAppDir(unsigned char *pasDir, int len)
{
	if(NULL == pasDir)
	{
		return SDK_PARA_ERR;
	}
	Trace("extern", "pasDir: %s\r\n", pasDir);
	Trace("extern", "input len: %d\r\n", len);
	memset(config_dir, 0, sizeof(config_dir));
	memcpy(config_dir, pasDir, len);
	return SDK_OK;
}

int sdkSysGetCurAppDir(unsigned char *pasData)
{
    if(NULL == pasData) //shijianglong 2013.01.30 15:58
    {
        return SDK_PARA_ERR;
    }
    strcpy(pasData, "/sdcard/pure/");
    return SDK_OK;
}

long sdkTimerGetId()
{
	return get_cur_msec();
}

bool sdkTimerIsEnd(long uiId, u32 uiMs)
{
    long lCurid = 0;

    lCurid = get_cur_msec();
//	Trace("ddi", "sdkTimerGetId ret: %d\r\n", lCurid);
//	Trace("ddi", "sdkTimerGetId input uiId: %d\r\n", uiId);
//	Trace("ddi", "diff : %d\r\n", lCurid - uiId);
    if(lCurid < uiId) // ���ʱ�䳯ǰУ���� �Ǿͳ�ʱ shiweisong 2013.09.09 16:18
    {
//		Trace("ddi", "true flag1\r\n");
        return true;
    }

    if(lCurid - uiId < uiMs)
    {
        return false;
    }
    else
    {
//		Trace("ddi", "true flag2\r\n");
        return true;
    }

}

s32 sdkDevContactSendAPDU(const u8 *pheInBuf, s32 siInLen, u8 *pheOutBuf, s32 *psiOutLen)
{
    s32 rslt = 0;
	s32 size = 300;

    if((NULL == pheInBuf) || (NULL == pheOutBuf) || (NULL == psiOutLen) || siInLen < 0)
    {
        return SDK_PARA_ERR;
    }

    TraceHex("emv", "contact c-apdu:", pheInBuf, siInLen);

    rslt = ddi_icc_trans_apdu(CARD_TYPE_IC1, pheInBuf, siInLen, pheOutBuf, psiOutLen);

    if(rslt == DDI_OK)
    {
    	TraceHex("emv", "contact r-apdu:", pheOutBuf, *psiOutLen);
        return SDK_OK;
    }
    else if( rslt == 8)
    {
        return SDK_TIME_OUT;
    }
    else
    {
        return SDK_ERR;
    }

    return SDK_ERR;
}

s32 sdkDevContactlessSendAPDU(const u8 *pheInBuf, u16 siInLen, u8 *pheOutBuf, s16 *psiOutLen)
{
    s32 ret = 0,rslt = 0;
	u32 size = 512;
//	static long timerid = 0;
//	long timerid2 = 0;
	u16 outBufLen = 0;

    if((NULL == pheInBuf) || (NULL == pheOutBuf) || (NULL == psiOutLen) || siInLen < 0)
    {
        return SDK_PARA_ERR;
    }

//	if((0 == memcmp(pheInBuf,"\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31\x00",( ( (u16)(siInLen) ) > 18 ? 18 : ( (u16)(siInLen) ) ) ) ) )
//    {
//        timerid = sdkTimerGetId();
//    }

	TraceHex("apdu", "pheInBuf:", pheInBuf, siInLen);
#if 1
	ret = ddi_icc_trans_apdu(CARD_TYPE_CLCPU, pheInBuf, siInLen, pheOutBuf, &outBufLen);
	Trace("extern", "ddi_icc_trans_apdu ret = %d\r\n", ret);
//	timerid2 = sdkTimerGetId();

//	if(timerid != 0 && (timerid2 - timerid > 1000))
//	{
		rslt = DetecteOther();
		Trace("lishiyao", "DetecteOther ret = %d\r\n", rslt);
		if(rslt == SDK_EQU)
		{
			*psiOutLen = -1;
			return SDK_ERR;
		}
//	}

//	Trace("extern", "ddi_icc_trans_apdu ret = %d\r\n", ret);
#else
    while(1)
    {
		ret = ddi_icc_trans_apdu(CARD_TYPE_CLCPU, pheInBuf, siInLen, pheOutBuf, &outBufLen);

        timerid2 = sdkTimerGetId();
    	if(timerid != 0 && (timerid + 1000 < timerid2))
    	{
            rslt = DetecteOther();
            if(rslt == SDK_EQU)
            {
                *psiOutLen = -1;
	            return SDK_ERR;
            }
    	}
        if(memcmp(pheInBuf, "\x00\xB2", 2) != 0)
        {
            break;
        }
        sdkmSleep(10);
    }

#endif
    if(ret == DDI_OK)
    {
		*psiOutLen = outBufLen;
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
	s32 i,ret;
	FILE *fp;

    if (NULL == pasFile || NULL == pheSrc || siStart < 0 || siSrclen < 0) {
		Trace("ddi", "Input Param invalid\r\n");
		return SDK_PARA_ERR;
    }

    if (siStart != 0)
	{
        i = sdkGetFileSize(pasFile);
        if (siStart > i && i > 0)
        {
            return SDK_PARA_ERR;
   		}
    }
//	Trace("ddi", "want open file:%s", pasFile);
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
	fseek(fp, 0, SEEK_END);
	i = ftell(fp);
	if (siStart > i)
	{
		fclose(fp);
		return SDK_PARA_ERR;
	}
	fseek(fp, siStart, SEEK_SET);

	ret = fwrite(pheSrc, sizeof(u8), siSrclen, fp);
	if(ret == siSrclen)
	{
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
//	s32 fp=0;
	FILE *fp = NULL;
	u32 i;

    if (NULL == pasFile || NULL == pheDest || NULL == psiDestlen || siOffset < 0)
	{
        return SDK_PARA_ERR;
    }

//	Trace("test", "want read byte: %d\r\n", *psiDestlen);
//
//	Trace("test", "want open file: %s\r\n", pasFile);
	fp = fopen(pasFile, "rb");
	if(fp == NULL)
	{
		return SDK_FUN_NULL;																						//�ļ���ʧ��
	}

    if( 0 != fseek(fp, siOffset, SEEK_SET) )
    {
        fclose(fp);
        return SDK_ERR;
    }

	i = *psiDestlen;
	*psiDestlen = fread(pheDest, sizeof(u8), i, fp);
//	Trace("test", "read byte: %d\r\n", *psiDestlen);
//	Trace("test", "want read byte: %d\r\n", i);
//	TraceHex("test", "read content", pheDest, *psiDestlen);
	if(*psiDestlen != i)
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
//	s32 fp;
	FILE *fp=NULL;

	if (NULL == pasFile)
	{
		return SDK_PARA_ERR;
	}

	fp = fopen(pasFile, "rb");
	if(NULL == fp)
	{
		return SDK_FUN_NULL;																						//�ļ���ʧ��
	}

	fseek(fp, 0, SEEK_END);

	fileSize = ftell(fp);

	fclose(fp);

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
	Trace("extern", "remove ret = %d\r\n", ret);
	if(ret == 0)
	{
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}
}


#define DEBUG_FOR_KERNEL
//#undef DEBUG_FOR_KERNEL
