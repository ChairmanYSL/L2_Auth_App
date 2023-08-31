#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#include <stdarg.h>

#include <android/log.h>
#include "debug.h"
//#include "../debug_log.h"


void hal_sys_info_debug(const char *__LOG_TAG__, const char* pMessage, ...)
{
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_INFO, __LOG_TAG__, pMessage, lVarArgs);
	va_end(lVarArgs);
}

void hal_sys_info(const char *__LOG_TAG__, const char* pMessage, ...)
{
#if IS_NDK_LOG
#ifdef __USE_BY_DEBUG__
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_INFO, __LOG_TAG__, pMessage, lVarArgs);
	//__android_log_print(ANDROID_LOG_INFO, __LOG_TAG__, "\n");
	va_end(lVarArgs);
#endif
#endif
}

void hal_sys_error(const char *__LOG_TAG__, const char* pMessage, ...)
{
#ifdef __USE_BY_DEBUG__
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_ERROR, __LOG_TAG__, pMessage, lVarArgs);
    //__android_log_print(ANDROID_LOG_ERROR, __LOG_TAG__, "\n");
	va_end(lVarArgs);
#endif
}

void hal_sys_warn(const char *__LOG_TAG__, const char* pMessage, ...)
{
#ifdef __USE_BY_DEBUG__
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_WARN, __LOG_TAG__, pMessage, lVarArgs);
    //__android_log_print(ANDROID_LOG_WARN, __LOG_TAG__, "\n");
	va_end(lVarArgs);
#endif
}

void hal_sys_debug(const char *__LOG_TAG__, const char* pMessage, ...)
{
#ifdef __USE_BY_DEBUG__
	va_list lVarArgs;
	va_start(lVarArgs, pMessage);
	__android_log_vprint(ANDROID_LOG_DEBUG, __LOG_TAG__, pMessage, lVarArgs);
    //__android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG__, "\n");
	va_end(lVarArgs);
#endif
}

void hal_sys_dump(const char *__LOG_TAG__, const char * str, unsigned char * pBuf, unsigned int len)
{
#ifdef __USE_BY_DEBUG__
	unsigned int i, j, nRemain;
	char tmp[17];
	char *p;
	char *add = (char*)pBuf;
	unsigned int nLine = 0;

    char strLine[240];

    if(str)
    {
        hal_sys_info("LOG","%s: length = %d ", str,  len);
     //   printf("%s \n",str);
    }

	if(len == 0)
		return;

    nLine = len / 100;

	for(i = 0; i < nLine; i++)
	{
        memset(strLine, 0, sizeof(strLine));
        for(j = 0; j < 100; j++)
            sprintf(strLine + 2*j, "%02X ", *add++);
		__android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG__, "%s", strLine);
      //  printf("%s\n",strLine);
	}

    nRemain = len % 100;
	if(nRemain)
	{
        memset(strLine, 0,  sizeof(strLine));
		for(j = 0; j < nRemain; j++)
            sprintf(strLine + 2*j, "%02X ", *add++);
		__android_log_print(ANDROID_LOG_DEBUG, __LOG_TAG__, "%s", strLine);
       // printf("%s\n",strLine);
	}
   hal_sys_info("LOG","dump end");
#endif
	return;
}

void dbg(char* txt)
{
#ifdef __USE_BY_DEBUG__
    hal_sys_info("loger",txt);
#endif
}

void LOGI_HEX(const char *__LOG_TAG__, const char * str, unsigned char * pBuf, unsigned int len)
{
#ifdef __USE_BY_DEBUG__
	unsigned int i,j,pos;
	char tmpbuf[4096];

	if(str){
		sprintf(tmpbuf,"%s",str);
	}
	pos=strlen(str);

	if(len == 0){
		return;
	}

	len=(len>1024)?1024:len;

	tmpbuf[pos++]= '[';
	for(i=0;i<len;i++){
		sprintf(&tmpbuf[pos],"%02x,", pBuf[i]);
		pos+=3;
		if(pos>=4093)
			break;
	}
	tmpbuf[pos]='\0';
	__android_log_print(ANDROID_LOG_INFO, __LOG_TAG__, "%s][%d]\n",tmpbuf,len);
#endif
}



