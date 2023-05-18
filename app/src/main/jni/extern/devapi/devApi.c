#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "devApi.h"
#include <dlfcn.h>
#include "sdkDebug.h"


#include <android/log.h>

#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

DDI_API gstddiapi = {0};


void StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int nLen) {
    char h1, h2;
    unsigned char s1, s2;
    int i;

    for (i = 0; i < nLen; i++) {
        h1 = pbSrc[2 * i];
        h2 = pbSrc[2 * i + 1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        pbDest[i] = s1 * 16 + s2;
    }
}

void HexToStr(unsigned char *pbDest, unsigned char *pbSrc, int nLen) {
    char ddl, ddh;
    int i;

    for (i = 0; i < nLen; i++) {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}

void U16ToBcd(unsigned char *Bcd, unsigned short const Src, unsigned int Len) {
    unsigned int i;
    unsigned short num = Src;
    for (i = Len; i > 0; i--) {
        *(Bcd + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }
}

int ddi_sys_get_time(unsigned char *lpTime)//����OK
{
    //DDI_SHOW_FUN_NAME;
    //zhouxiaoxin
    unsigned char year, adjust, temp[16] = {0}, ret;
    unsigned short yearsaved = 0;
    time_t sec;
    struct tm *pTm;

    sec = time(NULL);
    pTm = localtime(&sec);
    year = pTm->tm_year;
    year = year % 100;

    U16ToBcd(&lpTime[0], year, 1);
    U16ToBcd(&lpTime[1], pTm->tm_mon + 1, 1);
    U16ToBcd(&lpTime[2], pTm->tm_mday, 1);
    U16ToBcd(&lpTime[3], pTm->tm_hour, 1);
    U16ToBcd(&lpTime[4], pTm->tm_min, 1);
    U16ToBcd(&lpTime[5], pTm->tm_sec, 1);

    return 1;
}

int ddi_sys_get_tick(unsigned       int *nTick)  //ϵͳ�δ�//����OK
{
    struct timeval t0;
    gettimeofday(&t0, NULL);
    *nTick = t0.tv_sec * 1000 + t0.tv_usec / 1000;
    return DDI_OK;
}

void *ddi_k_malloc(unsigned int nbytes)
{
    return (void *) malloc(nbytes);
}

void ddi_k_free(void *ap)
{
    free(ap);
}
