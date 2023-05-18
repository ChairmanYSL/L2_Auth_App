#include "appglobal.h"

#define WAITSIO0    0
#define WAITSIO1    1
#define WAITSIO2    2
#define WAITSIO3    3
#define WAITSIO4    4

s32 OpenComm()
{
    return sdkCommOpenUart(NULL);
}

s32 CloseComm()
{
    return sdkCommCloseUart(0);
}


s32 BCTCSendData(unsigned char* buf, unsigned short len)
{
    unsigned char *tempbuf;
    unsigned int i;
    unsigned char high, low;
    s32 ret;
//	extern gSerialPortId;

    tempbuf = (u8 *)sdkGetMem(len + 3);
    memset(tempbuf, 0, len + 3);

    tempbuf[0] = 0x02;     //STX
    tempbuf[1] = buf[0];   //MsgType

    low = (len - 1) & 0xFF;
    high = ((len - 1) >> 8) & 0xFF;

    tempbuf[2] = high;
    tempbuf[3] = low;

    for(i = 1; i < len; i++)
    {
        tempbuf[3 + i] = buf[i];
    }

    TraceHex("", "send_data", tempbuf, len + 3);
    Trace("chenjun","send_data len %d\r\n",len + 3);

    ret =  sdkCommUartSendData(gSerialPortId, tempbuf, len + 3);
    Trace("chenjun","send_data return %d\r\n",ret);


    sdkFreeMem(tempbuf);
    return ret;
}

s32 BCTCRecvDatatemp(u8 *buf, int maxsizelen)
{
    s32 rCount;
	u16 timeout = 5;
	bool timeoutflag;

	timeout = gstHostTimeout;
	if(timeout < 3 || timeout > 99)
	{
		timeout = 5;
	}

	sdkTimerStar(timeout * 1000);

	Trace("BCTC", "maxsizelen = %d\r\n", maxsizelen);
	rCount = sdkCommUartRecvData(gSerialPortId, buf, maxsizelen, 1);
	Trace("BCTC", "Recv len = %d\r\n", rCount);
	TraceHex("BCTC", "Recv data", buf, rCount);
	if(rCount <= 0)
	{

		while (1)
		{
			rCount = sdkCommUartRecvData(gSerialPortId, buf, maxsizelen, 1);
			timeoutflag = sdkTimerIsEnd();
			Trace("BCTC", "timeoutflag = %d\r\n", timeoutflag);
			Trace("BCTC", "rCount = %d\r\n", rCount);
			if(rCount > 0 || timeoutflag == true)
			{
				break;
			}
			if(sdkKbGetKey() == SDK_KEY_ESC)
			{
				break;
			}
		}
		if(rCount > 0)
		{
			return rCount;
		}
		return SDK_ERR;
	}
	else
	{
		return rCount;
	}
}

s32 BCTCRecvData(u8 *buf, int maxsizelen)
{
	unsigned char *recv = NULL;
	int ret,index = 0;
	int len,len_all = -1;
	bool finish = false;
//	sdkTimerStar(1500);
//	unsigned char header[4] = {0};
//
//
//	recv = (unsigned char *)sdkGetMem(maxsizelen);
//	memset(recv , 0, maxsizelen);

	return sdkCommUartRecvData(gSerialPortId, buf, maxsizelen, 1000);
//	if(ret == 4)
//	{
//		len = header[2]*256+header[3];
//		len_all = len;
//		Trace("BCTC", "tlv len = %d\r\n", len);
//	}
//	TraceHex("BCTC", "header:", header, 4);
//	index = 0;
//	while(1)
//	{
//		ret = sdkCommUartRecvData(gSerialPortId, recv+index, len, 1500);
//		if(ret != -1)
//		{
//			if(ret != len)
//			{
//				len -= ret;
//				index += ret;
//			}
//			else
//			{
//				finish = true;
//				break;
//			}
//		}
//		if(sdkTimerIsEnd())
//		{
//			break;
//		}
//	}
//
//	if(finish)
//	{
//		memcpy(buf, header, 4);
//		memcpy(buf+4, recv, len_all);
//		TraceHex("BCTC", "body", recv, len_all);
//	}
//	sdkFreeMem(recv);
//	return len_all+4;
}


s32 BCTCRecvDataTTemp(u8 *buf, int maxsizelen)
{
    unsigned char retCode;
    unsigned char recStep = WAITSIO0;
    unsigned char data;
    s32 rCount;
    unsigned short templeni = 0, tempdataLen = 0;
	u16 timeout = 5;


	timeout = gstHostTimeout;
	if(timeout < 3 || timeout > 99)
	{
		timeout = 30;
	}

    sdkKbKeyFlush();
    retCode = SDK_OK;
	Trace("chenjun","BCTCRecvData timeout = %d\r\n",timeout);
	sdkTimerStar(timeout * 1000);

    while(1)
    {
        rCount = sdkCommUartRecvData(gSerialPortId, &data, 1, 1);
		Trace("lishiyao", "BCTCRecvData rCount = %d\r\n", rCount);
		Trace("lishiyao", "data:%02X\r\n", data);
        if(rCount == 1)
        {
			//Trace("lishiyao", "BCTCRecvData recStep = %d\r\n", recStep);
            switch(recStep)
            {
                 case WAITSIO0:

                   if(data == 0x02)
                   {
                       recStep = WAITSIO1;
                       templeni = 0;
                       buf[0] = data;
                   }
                   break;

                 case WAITSIO1:
                   buf[templeni + 1] = data;
                   templeni++;

                   if(templeni == 3)
                   {
                       tempdataLen = (unsigned short)buf[2] * 256 + (unsigned short)buf[3];
                       recStep = WAITSIO2;
                       templeni += 1;
                   }
				   //Trace("lishiyao", "in WAITSIO1 tempdataLen = %d\r\n", tempdataLen);
                   break;

                 case WAITSIO2:
                   buf[templeni] = data;
                   templeni++;

                   if((templeni - 4) == tempdataLen)
                   {
                       tempdataLen = templeni;
                       recStep = WAITSIO3;
					   //Trace("lishiyao", "in WAITSIO2 tempdataLen = %d\r\n", tempdataLen);
                       return tempdataLen;
                   }
                   break;

                 case WAITSIO3:
//				 Trace("lishiyao", "in WAITSIO3 tempdataLen = %d\r\n", tempdataLen);
                   return tempdataLen;

                 default:
                   break;
            }
        }

        if( sdkTimerIsEnd())
        {
            break;
        }

        if(sdkKbGetKey() == SDK_KEY_ESC) {break; }
    }

    return SDK_ERR;
}

