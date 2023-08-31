#include "appglobal.h"
#include "sdkoutcome.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "sdkDebug.h"


#define WAITSIO0    0
#define WAITSIO1    1
#define WAITSIO2    2
#define WAITSIO3    3
#define WAITSIO4    4

s32 OpenComm()
{
	u8 name[]="/dev/ttyGS8";
#if 0
    return sdkCommOpenUart(NULL);
#else
	return serial_open(name, 115200);
//	return SdkOpenComm();
#endif
}

s32 CloseComm()
{
	u8 name[]="/dev/ttyGS8";
#if 0
    return sdkCommCloseUart(0);
#else
	serial_close(gSerialPortId);
//	return SdkCloseComm(gSerialPortId);
#endif
}

s32 SdkOpenComm()
{

	int fd;
	struct termios options={0};

	fd = open("/dev/ttyGS8", O_RDWR | O_NOCTTY);
	Trace("Comm", "open fd = %d\r\n", fd);
	if (fd == -1)
	{
	    Trace("Comm", "open_port: Unable to open /dev/ttyGS8 - \r\n");
		return SDK_ERR;
	}

		// 获取当前设置
	tcgetattr(fd, &options);

	// 设置波特率
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);

	// 设置数据位，停止位等
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB; // 无奇偶校验
	options.c_cflag &= ~CSTOPB; // 1 停止位
	options.c_cflag &= ~CSIZE;	// 清除数据位设置
	options.c_cflag |= CS8; 	// 8 数据位

	// 应用设置
	tcsetattr(fd, TCSANOW, &options);

	return fd;
}

s32 SdkCloseComm(int fd)
{
	serial_close(fd);
//	close(fd);
}

s32 SdkCommWrite(int fd, unsigned char *buf, int sendLen)
{
	int ret;
	Trace("Comm", "input fd = %d\r\n", fd);
	TraceHex("Comm", "input buf", buf, sendLen);
//	ret = write(fd, buf, sendLen);
	ret = serial_send(fd, buf, sendLen);
	Trace("Comm", "write ret = %d\r\n", ret);
	return ret;
}

#if 0
s32 SdkCommRead(int fd, unsigned char *buf, int readLen)
{

	int bytes_read = read(fd, buf, readLen);
	Trace("Comm", "read ret = %d\r\n", bytes_read);
	if(bytes_read < 0)
	{
		Trace("Comm", "read comm error\r\n");
		return SDK_ERR;
	}
	else
	{
		return bytes_read;
	}
}
#else
s32 SdkCommRead(int fd, unsigned char *buf, int readLen)
{
	return serial_receve(fd, buf, readLen, 300);
}
#endif


bool SdkCommGetStatus()
{
	if(gSerialPortId < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

s32 BCTCSendData(unsigned char* buf, unsigned short len)
{
    u8 *tempbuf;
    u32 i;
    u8 high, low;
    s32 ret;

    tempbuf = (u8 *)sdkGetMem(len + 3);
    memset(tempbuf, 0, len + 3);

    tempbuf[0] = 0x02;     //STX
    tempbuf[1] = buf[0];   //MsgType
    Trace("BCTC", "send MsgType: %02X\r\n", buf[0]);

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

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
//		ret = sdkCommUartSendData(gSerialPortId, tempbuf, len + 3);
		ret = SdkCommWrite(gSerialPortId, tempbuf, len + 3);
		Trace("app","send_data return %d\r\n",ret);
	}
	else if(HOST_TRANS_WIFI == gHostTransType)
	{
		sdkSendWifiData(tempbuf, len + 3);
		ret = len + 3;
	}

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
	Trace("Host", "gHostTransType = %d\r\n", gHostTransType);
	Trace("Host", "gSerialPortId = %d\r\n", gSerialPortId);

	if(HOST_TRANS_SERIAL == gHostTransType)
	{
//		return sdkCommUartRecvData(gSerialPortId, buf, maxsizelen, 1000);
		return SdkCommRead(gSerialPortId, buf, maxsizelen);
	}
	else if(HOST_TRANS_WIFI == gHostTransType)
	{
		return sdkReadWifiData(buf, maxsizelen);
	}
}


s32 BCTCRecvDataTTemp(u8 *buf, int maxsizelen)
{
    u8 retCode;
    u8 recStep = WAITSIO0;
    u8 data;
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
