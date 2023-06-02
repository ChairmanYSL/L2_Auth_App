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
#include <string.h>



#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


int sdkIccCloseRfDev(void)
{
	return 0;
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

void sdkDispClearScreen(){
    clearLcdLine(0, 5);
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
	memcpy(extempAid.TransCurcyCode, "\x09\x78", 2);
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
	extempAid.Implementation = 0x2C;
	extempAid.ZeroAmtAllowFlag = appex_aid_list[0].ZeroAmtAllowFlag;
	extempAid.StatusCheckFlag = appex_aid_list[0].StatusCheckFlag;
	memcpy(extempAid.CLAppCap, "\x36\x00\x00\x02\xE9", 5);
	memcpy(extempAid.ATOL, appex_aid_list[0].ATOL, 64);
	extempAid.ATOLLen = appex_aid_list[0].ATOLLen;
	memcpy(extempAid.MTOL, "\x8C\x00\x00\x57\x00\x00", 64);
	extempAid.MTOLLen = appex_aid_list[0].MTOLLen;
	memcpy(extempAid.ATDTOL, appex_aid_list[0].ATDTOL, 64);
	extempAid.ATDTOLLen = appex_aid_list[0].ATDTOLLen;

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

void HostOutcomeTest(void)
{
	sdkSetOutcomeParam(SDK_OUTCOME_RESULT_TRYAGAIN, SDK_OUTCOME_START_B, SDK_OUTCOME_CVM_NA, 1, 1, 0, 0, SDK_OUTCOME_AIP_NA, 0, 0x13, NULL, SDK_OUTCOME_ONLINERESPDATA_NA);
	BCTCSendOutCome();
	sdkmSleep(5000);
	sdkSetUIRequestParam(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_PROCESSINGERR, 0x13, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	BCTCSendUIRequest(PURE_UIREQ_OUTCOME);
	sdkmSleep(5000);
	sdkSetUIRequestParam(SDK_UI_MSGID_TRYAGAIN, SDK_UI_STATUS_READYTOREAD, 0, NULL, SDK_UI_VALUEQUALIFIER_NA, NULL, NULL);
	BCTCSendUIRequest(PURE_UIREQ_RESTART);
}
