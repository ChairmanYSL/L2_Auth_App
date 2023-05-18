#include "sdkGlobal.h"
#include <sys/time.h>
#include <unistd.h>

s32 sdkSetRtc(const u8 *pbcSrc)
{
//	time_t t;
//	struct tm *tm;
//
//	time(&t);
//	tm = localtime(&t);
//
//
//	tm->tm_year = 2020 - 1900; // Set the year
//	tm->tm_mon = 5 - 1; // Set the month
//	tm->tm_mday = 15; // Set the day
//	t = mktime(tm);
//	settimeofday(&t, NULL);
	return 0;
}

s32 sdkRegulateAmount(u8 *pasDest, const u8 *pbcAmount)
{
    u8 i, j = 0;
    u8 temp[32] = {0};

    if(pbcAmount == NULL || pasDest == NULL)
    {
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    sdkBcdToAsc(temp, pbcAmount, 6);                                            //�����ת����ASCII��
    i = (u8)strspn(temp, "0");

    if(i > 9)                                                                                   //ֻ�нǷ�
    {
        pasDest[0] = '0';
        pasDest[1] = '.';
        pasDest[2] = temp[10];
        pasDest[3] = temp[11];
        pasDest[4] = 0;
    }
    else
    {
        for(j = 0; j < (10 - i); j++)                                   //���ڽǷ�
        {
            pasDest[j] = temp[i + j];
        }

        pasDest[j++] = '.';
        pasDest[j++] = temp[10];                                                //��
        pasDest[j++] = temp[11];                                                //��
        pasDest[j++] = 0;
    }
    return SDK_OK;
}
