#include "sdktypedef.h"
#include <android/log.h>

#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

void SdkDebugTrace(u8 const *pasTag, u8 const*  pasFile, u8 const *pasFun, int line,  char const *pheFmt, ...)
{
    u8 temp[2048 + 50];
    va_list varg;

    sprintf((char*)temp, "TAG:%s FUN:%s LINE:%d\r\n", pasTag, pasFun, line);
    va_start(varg, pheFmt);
    vsprintf(&temp[strlen(temp)], pheFmt, varg);
    LOGD("%s\r\n",temp);
    va_end(varg);
}

s32  sdkDebugTraceHex(u8 const* pasTag, u8 const* pasInfo, void const *pheDate, s32 siLen, u8 const *pasfun, s32 line) //lint !e765
{
    u16 i;
    u8 temp[2048] = {0};
    u8 buf[128];
    u8 const* src  = (u8 const*)pheDate;

    sprintf((char*)temp, "TAG:%s FUN:%s  LINE:%d  LEN:%d\r\n", pasTag, pasfun, line,siLen);
	LOGD("%s\r\n",temp);

    memset(temp, 0, 2048);
    strcpy(temp, pasInfo);
	LOGD("%s\r\n",temp);
    memset(temp, 0, 2048);

    for(i = 0; i < siLen; i++)
    {
        if(i % 16 == 0)
        {
            strcat(temp, "\r\n");
        }
        memset(buf, 0, sizeof(buf));
        sprintf((char*)buf, "%02X ", src[i]);
        strcat(temp, buf);
    }

    strcat(temp, "\r\n");
	LOGD("%s\r\n",temp);

}

