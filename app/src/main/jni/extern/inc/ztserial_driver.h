
#ifndef _ZTSERIAL_DRIVER_H   
#define _ZTSERIAL_DRIVER_H   

#ifdef __cplusplus
extern "C"
{
#endif  
int serial_open(char *port,int bps);
int serial_close(int inx);
int serial_receve(int inx,unsigned char* pDataBuffer,int nDataLength,int nTimeout_MS);
int serial_send(int inx,unsigned char* pDataBuffer,int nDataLength);
int serial_setOptions(int inx,int tDataBit,int tStopBit,int tParity,int bps);
int serial_cancel(int inx);
long get_cur_msec();
#ifdef __cplusplus
}
#endif

#endif /* _ZTSERIAL_DRIVER_H */