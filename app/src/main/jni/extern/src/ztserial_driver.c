#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "ztserial_driver.h"
#include "debug.h"
//#include "../debug_log.h"
#define SERIAL "SERIAL"

#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


int serial_open(char *port,int bps)
{
    hal_sys_info(SERIAL, "Enter Function:[%s].\n", __FUNCTION__);
    hal_sys_info(SERIAL, "port=%s, speed=%d", port, bps);
    int iRet = open(port, O_RDWR);
    if (iRet > 0)
    {
        if (bps > 0)
        {
            serial_setOptions(iRet, 8, 1, 'n', bps);
        }
    }

    return iRet;
}

int serial_close(int inx)
{
    hal_sys_info(SERIAL, "Enter Function:[%s].\n", __FUNCTION__);
    hal_sys_info(SERIAL, "inx=%d", inx);
    return close(inx);
}

int isReadable(int fd, int timeout_ms)
{
    struct timeval    timeout;
    fd_set    readfds;
    int iret;
    timeout.tv_sec    = (timeout_ms)/1000;
    timeout.tv_usec    = (timeout_ms%1000)*1000;
    FD_ZERO(&readfds);
    FD_SET(fd,&readfds);
    iret = select((int)fd+1,&readfds,NULL,NULL,&timeout);
    if(iret > 0){
        if(FD_ISSET(fd,&readfds))
        {
            return 0;
        }
    }
    return -2;
}

long get_cur_msec()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000 + tv.tv_usec / 1000;
}

long get_cur_msec2(struct timeval* start, struct timeval* end)
{
    return (end->tv_sec - start->tv_sec)*1000 + (end->tv_usec - start->tv_usec) / 1000;
}


int isStop;

int serial_receve(int inx,unsigned char* pDataBuffer,int nDataLength,int nTimeout_MS)
{
    hal_sys_info(SERIAL, "Enter Function:[%s].\n", __FUNCTION__);
    hal_sys_info(SERIAL, "inx=%d, nDataLength=%d", inx, nDataLength);
    if (nTimeout_MS < -1 || pDataBuffer == NULL || nDataLength <= 0 || inx <= 0)
    {
        return -1;
    }
    int iRet = -1;
    long start = get_cur_msec();
    long diff = 0;
    int length = 0;
    int tatolSize = nDataLength;
    isStop = 1;
    int hasDate = 0;
    while (isStop)
    {
        if(tatolSize - length <= 0){
            break;
        }
        iRet = isReadable(inx, nTimeout_MS - diff);
        LOGI("%s(%d): serial_receve,isReadable = %d",__FUNCTION__,__LINE__,iRet);
        if (iRet == 0)
        {
            iRet = read(inx, pDataBuffer+length, (tatolSize - length));
            LOGI("%s(%d): serial_receve,read = %d",__FUNCTION__,__LINE__,iRet);
            if (iRet > 0)
            {
                hasDate = 1;
                length += iRet;
            }
            else
            {
                break;
            }
        }
        else
        {
            if (hasDate == 1)
            {
                break;
            }
        }
        diff = get_cur_msec() - start;
        LOGI("%s(%d): serial_receve,iRet = %d,diff = %d \n",__FUNCTION__,__LINE__,iRet,diff);
        if (nTimeout_MS != -1 && diff > nTimeout_MS)
        {
            LOGE("%s(%d): serial_receve time out ,diff = %d \n",__FUNCTION__,__LINE__,diff);
            break;
        }
    }
    if (length == 0)
    {
        length = -106;
    }
    LOGI("%s(%d): serial_receve return, size = %d \n",__FUNCTION__,__LINE__,length);
    return length;
}

int serial_send(int inx,unsigned char* pDataBuffer,int nDataLength)
{
    hal_sys_info(SERIAL, "Enter Function:[%s].\n", __FUNCTION__);
    hal_sys_info(SERIAL, "inx=%d", inx);
    int iRet = write(inx, pDataBuffer, nDataLength);
    hal_sys_info(SERIAL, "serial_send iRet=%d", iRet);
    return iRet;
}

static speed_t getBaudrate(int baudrate)
{
    switch(baudrate)
    {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return B115200;
    }
}

int serial_setOptions(int fd,int databits,int stopbits,int parity,int speed)
{
    hal_sys_info(SERIAL, "Enter Function:[%s].\n", __FUNCTION__);
    struct termios options;

    if (tcgetattr(fd, &options) !=  0)
    {
        hal_sys_info(SERIAL, "tcgetattr err");
        return -1;
    }

    cfsetispeed(&options, getBaudrate(speed));
    cfsetospeed(&options, getBaudrate(speed));

    options.c_cflag &= ~CSIZE;

    switch (databits)
    {
        case 5:
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            hal_sys_info(SERIAL, "databits err");
            return -1;
    }

    switch (parity)
    {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;       //Clear parity enable
            options.c_iflag &= ~INPCK;     //Enable parity checking
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;    //Disnable parity checking
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;    //Enable parity
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;      //Disnable parity checking
            break;
        case 'S':
        case 's':  //as no parity
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            hal_sys_info(SERIAL,"Unsupported parity\n");
            return -1;
    }

    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            hal_sys_info(SERIAL,"Unsupported stop bits\n");
            return -1;
    }

    //Set input parity option
    if (parity != 'n')
    options.c_iflag |= INPCK;

    //for test
    if (parity == 'e')
    options.c_iflag &= ~INPCK;

    //set Raw Mode
    options.c_lflag  &= ~(ICANON | IEXTEN | ECHO | ECHOE | ISIG );  //Input
    options.c_oflag  &= ~OPOST;   //Output

    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_iflag &= ~(INLCR | ICRNL);

    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        hal_sys_info(SERIAL, "tcsetattr err");
        return -1;
    }

    return 0;
}

int serial_cancel(int inx)
{
    hal_sys_info(SERIAL, "Enter Function:[%s].\n", __FUNCTION__);
    isStop = 0;
    return 0;
}

