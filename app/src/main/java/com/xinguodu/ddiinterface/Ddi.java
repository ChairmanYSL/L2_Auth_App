package com.xinguodu.ddiinterface;
import java.util.Arrays;
import android.os.Build;

import com.xinguodu.ddiinterface.struct.CL_PARAM;
import com.xinguodu.ddiinterface.struct.HSM_ObjectProperty;
import com.xinguodu.ddiinterface.struct.StrAT24CXX;
import com.xinguodu.ddiinterface.struct.StrAT88CXX;
import com.xinguodu.ddiinterface.struct.StrAT88CXXReadEc;
import com.xinguodu.ddiinterface.struct.StrAT88CXXVerify;
import com.xinguodu.ddiinterface.struct.StrComAttr;
import com.xinguodu.ddiinterface.struct.StrDukptInitInfo;
import com.xinguodu.ddiinterface.struct.StrHkey;
import com.xinguodu.ddiinterface.struct.StrIs23sc1604;
import com.xinguodu.ddiinterface.struct.StrIs23sc1604ReadRc;
import com.xinguodu.ddiinterface.struct.StrIs23sc1604Verify;
import com.xinguodu.ddiinterface.struct.StrKeyTAKpara;
import com.xinguodu.ddiinterface.struct.StrLedGleamPara;
import com.xinguodu.ddiinterface.struct.StrMfAuth;
import com.xinguodu.ddiinterface.struct.StrPrnCombTextCtrl;
import com.xinguodu.ddiinterface.struct.StrPrnTextCtrl;
import com.xinguodu.ddiinterface.struct.StrSle4428;
import com.xinguodu.ddiinterface.struct.StrSle4442;
import com.xinguodu.ddiinterface.struct.StrTkey;


//import xinguodu.ddiinterface.class;
//import com.xinguodu.ddiinterface;
//#define COMB_PRN_TEXT_MAX_LEN 256

public final class Ddi {

    static {
        System.loadLibrary("xgd_ddi_jni");
        ddi_ddi_sys_init();
        ddi_sys_set_timeout(30 * 1000);//默认k21休眠时间
    }

    public static synchronized  native int test(int a, int b);

    public static synchronized  native void ddi_ddi_sys_init();

    public static synchronized  native int ddi_sys_read_dsn(byte[] lpOut);

	public static synchronized native int ddi_sys_read_cfgcode(byte[] lpOut);
	
    public static synchronized  native int ddi_sys_get_firmwarever(byte[] lpOut,int nType);

    public static synchronized  native int ddi_sys_bat_status();

    public static synchronized  native int ddi_sys_mainBat_status();

    public static synchronized  native int ddi_sys_poweroff();

    public static synchronized  native int ddi_sys_download(int nType);

    public static synchronized  native int ddi_sys_set_timeout(int nTime);

    public static synchronized  native int ddi_sys_get_timeout(int[] nTime);

    public static synchronized  native int ddi_sys_get_chipID(byte[] nType);

    public static synchronized  native int ddi_security_rand(byte[] rand);

    public static synchronized  native int ddi_get_debugStatus();

	public static synchronized native int ddi_sys_twoway_auth();

	public static synchronized native int ddi_sys_twoway_auth_exit();

	public static synchronized native int ddi_sys_check_ieComm();

	public static synchronized native int ddi_sys_waitfor_ieComm();

	public static synchronized native int ddi_sys_check_usbcom();

	public static synchronized native int ddi_set_debugStatus(byte status,byte[] password);

    public static synchronized  native int ddi_delete_cert_byPassword(byte[] password);

    public static synchronized  native int ddi_sys_set_beep(int nTime);

    public static synchronized  native int ddi_sys_getCertHash(byte[] hash);

    public static synchronized  native int ddi_sys_setCertHash(byte[] hash);

    public static synchronized  native int ddi_BodyNumber_process_download();

	public static synchronized native int ddi_cfgcode_process_download();

	public static synchronized native int ddi_RTC_process_download();

	public static synchronized native int ddi_uart_comm_init();
	
	public static synchronized native int ddi_bind_hardware_info_process();

	public static synchronized native int ddi_k21_reboot_to_upgrade_mode();

	public static synchronized native int ddi_BodyNumber_process_serial_read();
	
	public static synchronized native int ddi_RTC_process_serial_read();
	
	public static synchronized native int ddi_open_port();

    public static synchronized  native int ddi_close_port();

    public static synchronized  native int ddi_com_open_sub(int nCom,int baud,int databits,int parity,int stopbits);

    public static synchronized int ddi_com_open(int nCom,StrComAttr ComAttr)
    {
        int baud,databits,parity,stopbits;
        baud = ComAttr.getBaud();
        databits = ComAttr.getDatabits();
        parity = ComAttr.getParity();
        stopbits = ComAttr.getStopbits();
        return ddi_com_open_sub(nCom,baud,databits,parity,stopbits);
    }

    public static synchronized  native int ddi_com_close(int nCom);

    public static synchronized  native int ddi_com_clear(int nCom);

    public static synchronized  native int ddi_com_read(int nCom,byte[] lpOut,int nLe);

    public static synchronized  native int ddi_com_write(int nCom,byte[] lpIn,int nLe);

    public static synchronized  native int ddi_com_ioctl(int nCom,int nCmd,int lParam,int wParam);

    public static synchronized  native int ddi_k21_com_clear(int nCom);

    public static synchronized  native int ddi_k21_com_read(int nCom,byte[] lpOut,int nLe);

    public static synchronized  native int ddi_k21_com_write(int nCom,byte[] lpIn,int nLe);


    public static synchronized  native int ddi_mag_open();

    public static synchronized  native int ddi_mag_close();

    public static synchronized  native int ddi_mag_clear();

    public static synchronized  native int ddi_mag_read(byte[] lpTrack1,byte[] lpTrack2,byte[] lpTrack3);

    public static synchronized  native int ddi_mag_ioctl(int nCmd,int lParam,int wParam);

    public static synchronized  native int ddi_mag_ioctl_for_java(int nCmd,int lplen,byte[] lParam,int[] wplen,byte[] wParam);

    public  int ddi_mag_ioctl_getVer(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_mag_ioctl_for_java(0,0,buf,wlen,wParam);

        return ret;
    }

	public  int ddi_mag_ioctl_setLrc(byte status)
	{
		int ret;
		int[] wlen = new int[1];
		byte[] buf = new byte[20];
		byte[] buf2 = new byte[20];
		buf[0] = status;
		ret = ddi_mag_ioctl_for_java(0x14,1,buf,wlen,buf2);
		
		return ret;
	}
	public native int ddi_iccpsam_open(int nSlot);

    public static synchronized  native int ddi_iccpsam_close(int nSlot);

    public static synchronized  native int ddi_iccpsam_poweron(int nSlot,byte[] lpAtr);

    public static synchronized  native int ddi_iccpsam_poweroff(int nSlot);

    public static synchronized  native int ddi_iccpsam_get_status(int nSlot);

    public static synchronized  native int ddi_iccpsam_exchange_apdu(int nSlot, byte[] lpCApdu,int lpCApduLen,byte[] lpRApdu,int[] lpRApduLen,int lpRApduSize);

    public static synchronized  native int ddi_iccpsam_ioctl(int nCmd,int lParam,int wParam);

    public static synchronized  native int ddi_iccpsam_ioctl_for_java(int nCmd,int lplen,byte[] lParam,int[] wplen,byte[] wParam);

    public  int ddi_iccpsam_ioctl_getVer(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_iccpsam_ioctl_for_java(0,0,buf,wlen,wParam);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_etud(byte slot,byte etu)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        wlen[0] = 1;
        buf[0] = slot;
        buf2[1] = etu;
        ret = ddi_iccpsam_ioctl_for_java(1,1,buf,wlen,buf2);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_memory_poweroff()
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(2,0,buf,wlen,buf2);

        return ret;
    }

    public static synchronized int ddi_iccpsam_ioctl_sle4428_reset()
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(3,0,buf,wlen,buf);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4428_read(StrSle4428 Sle4428,byte[] data,int[] dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Sle4428.getProtect());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Sle4428.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(Sle4428.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(4,12,buf,dataLen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4428_write(StrSle4428 Sle4428,byte[] data,int dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];


        bufTemp = IntToByte(Sle4428.getProtect());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Sle4428.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(Sle4428.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = dataLen;
        ret = ddi_iccpsam_ioctl_for_java(5,12,buf,wlen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4428_readec(int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];

        ret = ddi_iccpsam_ioctl_for_java(6,0,buf,wlen,buf2);
//
        time[0] = ByteToInt(buf2);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4428_verify(byte[] password,int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];

        ret = ddi_iccpsam_ioctl_for_java(7,2,password,wlen,buf);

        time[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4428_updatesc(byte[] password)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];

        ret = ddi_iccpsam_ioctl_for_java(8,0,buf,wlen,password);

        return ret;
    }


    public  int ddi_iccpsam_ioctl_sle4442_reset()
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(9,0,buf,wlen,buf2);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4442_read(StrSle4442 Sle4442,byte[] data,int[] dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Sle4442.getArea());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Sle4442.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(Sle4442.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(10,12,buf,dataLen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4442_write(StrSle4442 Sle4442,byte[] data,int dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];


        bufTemp = IntToByte(Sle4442.getArea());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Sle4442.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(Sle4442.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = dataLen;
        ret = ddi_iccpsam_ioctl_for_java(11,12,buf,wlen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4442_readec(int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];

        ret = ddi_iccpsam_ioctl_for_java(12,0,buf,wlen,buf2);

        time[0] = ByteToInt(buf2);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4442_verify(byte[] password,int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];

        ret = ddi_iccpsam_ioctl_for_java(13,2,password,wlen,buf);

        time[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_sle4442_updatesc(byte[] password)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];

        ret = ddi_iccpsam_ioctl_for_java(14,0,buf,wlen,password);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT24CXX_read(StrAT24CXX AT24CXX,byte[] data,int[] dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];



        bufTemp = IntToByte(AT24CXX.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT24CXX.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(AT24CXX.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(15,12,buf,dataLen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT24CXX_write(StrAT24CXX AT24CXX,byte[] data,int dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(AT24CXX.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT24CXX.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(AT24CXX.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = dataLen;
        ret = ddi_iccpsam_ioctl_for_java(16,12,buf,wlen,data);


        return ret;
    }


    public  int ddi_iccpsam_ioctl_AT88SCXX_reset(byte type)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        buf[0] = type;
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(17,1,buf,wlen,buf2);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT88SCXX_read(StrAT88CXX AT88CXX,byte[] data,int[] dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(AT88CXX.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT88CXX.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(AT88CXX.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(18,12,buf,dataLen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT88SCXX_write(StrAT88CXX AT88CXX,byte[] data,int dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(AT88CXX.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT88CXX.getAddr());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(AT88CXX.getLe());
        System.arraycopy(bufTemp, 0, buf, 8, 4);

        wlen[0] = dataLen;
        ret = ddi_iccpsam_ioctl_for_java(19,12,buf,wlen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT88SCXX_readec(StrAT88CXXReadEc AT88CXXReadEc,int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(AT88CXXReadEc.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT88CXXReadEc.getMode());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(AT88CXXReadEc.getIndex());
        System.arraycopy(bufTemp, 0, buf, 8, 4);

        ret = ddi_iccpsam_ioctl_for_java(20,12,buf,wlen,buf2);

        time[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT88SCXX_verify(StrAT88CXXVerify AT88CXXVerify,int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(AT88CXXVerify.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT88CXXVerify.getMode());
        System.arraycopy(bufTemp, 0, buf, 4, 4);

        bufTemp = AT88CXXVerify.getKey();
        System.arraycopy(bufTemp, 0, buf, 8, 3);
        buf[11] = AT88CXXVerify.getIndex();

        ret = ddi_iccpsam_ioctl_for_java(21,12,buf,wlen,buf2);

        time[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_AT88SCXX_updatesc(StrAT88CXXVerify AT88CXXVerify)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(AT88CXXVerify.getType());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(AT88CXXVerify.getMode());
        System.arraycopy(bufTemp, 0, buf, 4, 4);

        bufTemp = AT88CXXVerify.getKey();
        System.arraycopy(bufTemp, 0, buf, 8, 3);
        buf[11] = AT88CXXVerify.getIndex();
        wlen[0] = 0;

        ret = ddi_iccpsam_ioctl_for_java(22,12,buf,wlen,buf2);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_IS23SC1604_reset()
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(23,0,buf,wlen,buf2);

        return ret;
    }

    public  int ddi_iccpsam_ioctl_IS23SC1604_read(StrIs23sc1604 Is23sc1604,byte[] data,int[] dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Is23sc1604.getAddr());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Is23sc1604.getLe());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(24,8,buf,dataLen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_IS23SC1604_write(StrIs23sc1604 Is23sc1604,byte[] data,int dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Is23sc1604.getAddr());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Is23sc1604.getLe());
        System.arraycopy(bufTemp, 0, buf, 4, 4);

        wlen[0] = dataLen;
        ret = ddi_iccpsam_ioctl_for_java(25,8,buf,wlen,data);


        return ret;
    }


    public  int ddi_iccpsam_ioctl_IS23SC1604_erase(StrIs23sc1604 Is23sc1604,byte[] data,int[] dataLen)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Is23sc1604.getAddr());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Is23sc1604.getLe());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        wlen[0] = 0;
        ret = ddi_iccpsam_ioctl_for_java(26,8,buf,dataLen,data);


        return ret;
    }

    public  int ddi_iccpsam_ioctl_IS23SC1604_readec(StrIs23sc1604ReadRc Is23sc1604ReadRc,int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Is23sc1604ReadRc.getZone());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Is23sc1604ReadRc.getMode());
        System.arraycopy(bufTemp, 0, buf, 4, 4);

        ret = ddi_iccpsam_ioctl_for_java(27,8,buf,wlen,buf2);

        time[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_IS23SC1604_verify(StrIs23sc1604Verify Is23sc1604Verify,int[] time)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Is23sc1604Verify.getZone());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Is23sc1604Verify.getMode());
        System.arraycopy(bufTemp, 0, buf, 4, 4);

        bufTemp = Is23sc1604Verify.getKey();
        System.arraycopy(bufTemp, 0, buf, 8, 2);
        ret = ddi_iccpsam_ioctl_for_java(28,10,buf,wlen,buf2);

        time[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_iccpsam_ioctl_IS23SC1604_updatesc(StrIs23sc1604Verify Is23sc1604Verify)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[256];
        byte[] bufTemp = new byte[20];

        bufTemp = IntToByte(Is23sc1604Verify.getZone());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Is23sc1604Verify.getMode());
        System.arraycopy(bufTemp, 0, buf, 4, 4);

        bufTemp = Is23sc1604Verify.getKey();
        System.arraycopy(bufTemp, 0, buf, 8, 2);

        ret = ddi_iccpsam_ioctl_for_java(22,10,buf,wlen,buf2);

        return ret;
    }

    public static synchronized  native int ddi_rf_open();

    public static synchronized  native int ddi_rf_close();

    public static synchronized  native int ddi_rf_poweron(int nType);

    public static synchronized  native int ddi_rf_poweroff();

    public static synchronized  native int ddi_rf_get_status();

    public static synchronized  native int ddi_rf_activate();

    public static synchronized  native int ddi_rf_exchange_apdu( byte[] lpCApdu,int lpCApduLen,byte[] lpRApdu,int[] lpRApduLen,int lpRApduSize);

    public static synchronized  native int ddi_rf_remove();

    public static synchronized  native int ddi_rf_ioctl(int nCmd,int lParam,int wParam);

    public static synchronized  native int ddi_rf_ioctl_for_java(int nCmd,int lplen,byte[] lParam,int[] wplen,byte[] wParam);

    public  int ddi_rf_ioctl_getVer(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_rf_ioctl_for_java(0,0,buf,wlen,wParam);



        return ret;
    }

    public static synchronized int ddi_rf_ioctl_Sak(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_rf_ioctl_for_java(1,0,buf,wlen,wParam);
        return ret;
    }

    public static synchronized int ddi_rf_ioctl_Uid(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_rf_ioctl_for_java(2,0,buf,wlen,wParam);
        return ret;
    }

    public static synchronized int ddi_rf_ioctl_Mf_Auth(StrMfAuth MfAuth)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        byte[] bufTemp = new byte[20];


        buf[0] = MfAuth.getAuthmode();
        Arrays.fill(bufTemp,(byte)0x00);
        bufTemp = MfAuth.getKey();
        for(i=0;i<6;i++)
        {
            buf[i+1] = bufTemp[i];
        }
        Arrays.fill(bufTemp,(byte)0x00);
        bufTemp = MfAuth.getUid();
        for(i=0;i<10;i++)
        {
            buf[i+7] = bufTemp[i];
        }
        buf[17] = MfAuth.getBlock();
        ret = ddi_rf_ioctl_for_java(3,18,buf,wlen,buf2);
        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_readRaw(byte block,byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        buf[0] = block;
        ret = ddi_rf_ioctl_for_java(4,1,buf,wlen,wParam);
        return ret;
    }
    public  static synchronized int ddi_rf_ioctl_writeRaw(byte block,byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        buf[0] = block;
        wlen[0] = 16;
        ret = ddi_rf_ioctl_for_java(5,1,buf,wlen,wParam);
        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_readValue(byte block,int[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        buf[0] = block;
        ret = ddi_rf_ioctl_for_java(6,1,buf,wlen,buf);

        wParam[0] = ByteToInt(buf);
        return ret;
    }

    public  int ddi_rf_ioctl_writeValue(byte block, int wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[20];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        buf[0] = block;

        bufTemp = IntToByte(wParam);
        System.arraycopy(bufTemp, 0, buf2, 0, 4);
        wlen[0] = 4;
        ret = ddi_rf_ioctl_for_java(7,1,buf,wlen,buf2);

        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_incValue(byte block, int wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        byte[] bufTemp = new byte[20];
        buf[0] = block;

        bufTemp = IntToByte(wParam);
        System.arraycopy(bufTemp, 0, buf2, 0, 4);
        wlen[0] = 4;
        ret = ddi_rf_ioctl_for_java(8,1,buf,wlen,buf2);

        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_decValue(byte block, int wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        byte[] bufTemp = new byte[20];
        buf[0] = block;

        bufTemp = IntToByte(wParam);
        System.arraycopy(bufTemp, 0, buf2, 0, 4);
        wlen[0] = 4;
        ret = ddi_rf_ioctl_for_java(9,1,buf,wlen,buf2);

        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_baclupValue(byte fblock, byte tblock)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];
        buf[0] = fblock;
        buf2[0] = tblock;
        wlen[0] = 1;

        ret = ddi_rf_ioctl_for_java(10,1,buf,wlen,buf2);

        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_setParam(CL_PARAM param)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[60];
        byte[] buf2 = new byte[60];
        byte[] bufTemp = new byte[60];

        buf[0] = param.getModGsP();
        buf[1] = param.getRFCfg_A();
        buf[2] = param.getRFCfg_B();
        buf[3] = param.getRFOLevel();
        buf[4] = param.getRxTreshold_A();
        buf[5] = param.getRxTreshold_B();
        Arrays.fill(bufTemp,(byte)0x00);
        bufTemp = param.getRFU();
        for(i=0;i<25;i++)
        {
            buf[i+6] = bufTemp[i];
        }
        buf[31] = param.getcrc();

        ret = ddi_rf_ioctl_for_java(11,32,buf,wlen,buf2);
        return ret;
    }

    public  static synchronized int ddi_rf_ioctl_getParam(CL_PARAM param)
    {
        int ret,i;
        int[] wlen = new int[1];
        byte[] buf = new byte[60];
        byte[] buf2 = new byte[60];
        byte[] bufTemp = new byte[60];

        ret = ddi_rf_ioctl_for_java(12,0,buf,wlen,buf2);
        if(0==ret&&32==wlen[0])
        {
            param.setModGsP(buf2[0]);
            param.setRFCfg_A(buf2[1]);
            param.setRFCfg_B(buf2[2]);
            param.setRFOLevel(buf2[3]);
            param.setRxTreshold_A(buf2[4]);
            param.setRxTreshold_B(buf2[5]);
            for(i=0;i<25;i++)
            {
                bufTemp[i] = buf2[i+6];
            }
            param.setRFU(bufTemp);
            param.setcrc(buf2[31]);
        }
        return ret;
    }

    public static synchronized  native int ddi_thmprn_open();

    public static synchronized  native int ddi_thmprn_close();

    public static synchronized  native int ddi_thmprn_feed_paper(int nPixels);

    public static synchronized  native int ddi_thmprn_back_paper(int nPixels);

    public static synchronized  native int ddi_thmprn_print_image(int nOrgLeft, int nImageWidth, int nImageHeight,  byte[] lpImage);

    public static synchronized  native int ddi_thmprn_print_image_file(int nOrgLeft, int nImageWidth, int nImageHeight,  byte[] lpImageName);

    public static synchronized  native int ddi_thmprn_print_text_sub(int align,int offset,int font,int ascsize,int asczoom,int nativesize,int nativezoom,byte[] text);   //

    public static synchronized int ddi_thmprn_print_text(StrPrnTextCtrl textCtrl,byte[] text)
    {
        int ret;
        int align,offset,font,ascsize,asczoom,nativesize,nativezoom;

        align = textCtrl.getAlign();
        offset = textCtrl.getOffset();
        font = textCtrl.getFont();
        ascsize = textCtrl.getAscsize();
        asczoom = textCtrl.getAsczoom();
        nativesize = textCtrl.getNativesize();
        nativezoom = textCtrl.getNativezoom();

        ret = ddi_thmprn_print_text_sub(align,offset,font,ascsize,asczoom,nativesize,nativezoom,text);

        return ret;
    }
    public static synchronized  native int ddi_thmprn_print_comb_text_sub2(Object... list);

    public static synchronized  native int ddi_thmprn_print_comb_text_sub(int nNum,int[] m_x0, int[] m_y0,int[] m_font,int[] m_ascsize,
                                                                          int[] m_asczoom,int[] m_nativesize,int[] m_nativezoom,Object... list);   //

    public  int ddi_thmprn_print_comb_text(int nNum,StrPrnCombTextCtrl[] textCtrl)
    {
        int ret=0,i;

        int[] x0 = new int[256];
        int[] y0 = new int[256];
        int[] font = new int[256];
        int[] ascsize = new int[256];
        int[] asczoom = new int[256];
        int[] nativesize = new int[256];
        int[] nativezoom = new int[256];
        byte[][] text = new byte[256][256];

        if(nNum>256)
            return -1;

        for(i=0;i<nNum;i++)
        {
            x0[i] = textCtrl[i].getX0();
            y0[i] = textCtrl[i].getY0();
            font[i] = textCtrl[i].getFont();
            ascsize[i] = textCtrl[i].getAscsize();
            asczoom[i] = textCtrl[i].getAsczoom();
            nativesize[i] = textCtrl[i].getNativesize();
            nativezoom[i] = textCtrl[i].getNativezoom();
            text[i] = textCtrl[i].getText();
        }
//		ret = ddi_thmprn_print_comb_text_sub(nNum,x0,y0,font,ascsize,asczoom,nativesize,nativezoom,text);

        return ret;
    }

    public static synchronized  native int ddi_thmprn_get_status();

    public static synchronized  native int ddi_thmprn_ioctl(int nCmd,int lParam,int wParam);

    public static synchronized  native int ddi_thmprn_test();

    public static synchronized  native int ddi_thmprn_print_oneBitBMPImage(byte[] imageName,int nameLen);

    public static synchronized  native int ddi_thmprn_print_oneBitBMPImageByBuffer(byte[] imageBuf,int bufLen);

    public static synchronized  native int ddi_thmprn_totalDot(byte[] dotBuf,int bufLen);

    public static synchronized  native int ddi_thmprn_print_blackBlock(int line);

    public static synchronized  native int ddi_thmprn_ioctl_for_java(int nCmd,int lplen,byte[] lParam,int[] wplen,byte[] wParam);

    public  int ddi_thmprn_ioctl_getVer(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_thmprn_ioctl_for_java(0,0,buf,wlen,wParam);


        return ret;
    }
    public static synchronized int ddi_thmprn_ioctl_setGray(int gray)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[20];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];

        bufTemp = IntToByte(gray);
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        wlen[0] = 0;

        ret = ddi_thmprn_ioctl_for_java(1,4,buf,wlen,buf2);


        return ret;
    }

    public static synchronized  native int ddi_innerkey_open();

    public static synchronized  native int ddi_innerkey_close();

    public static synchronized  native int ddi_innerkey_inject(int nKeyArea,int nIndex, byte[] lpKeyData);

    public static synchronized  native int ddi_innerkey_encrypt(int nKeyArea,int nIndex,int nLen,  byte[] lpIn, byte[] lpOut);

    public static synchronized  native int ddi_innerkey_decrypt(int nKeyArea,int nIndex,int nLen,  byte[] lpIn, byte[] lpOut);

    public static synchronized  native int ddi_innerkey_ioctl(int nCmd,int lParam,int wParam);

    public static synchronized  native int ddi_innerkey_ioctl_for_java(int nCmd,int lplen,byte[] lParam,int[] wplen,byte[] wParam);

    public  int ddi_innerkey_ioctl_getVer(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
//		ALOGE("ddi_innerkey_ioctl_getVer....................................................................\n");
        ret = ddi_innerkey_ioctl_for_java(0,0,buf,wlen,wParam);


        return ret;
    }

    public static synchronized int ddi_innerkey_ioctl_tkey_inject(int area,int index)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[20];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];


        bufTemp = IntToByte(area);
        System.arraycopy(bufTemp, 0, buf, 0, 4);

        bufTemp = IntToByte(index);
        System.arraycopy(bufTemp, 0, buf2, 0, 4);
        wlen[0] = 4;

        ret = ddi_innerkey_ioctl_for_java(1,4,buf,wlen,buf2);


        return ret;
    }

    public static synchronized int ddi_innerkey_ioctl_tkey_encrypt(StrTkey tkey)
    {
        int ret,i,len;
        int[] wlen = new int[1];
        byte[] buf = new byte[512];
        byte[] buf2 = new byte[512];
        len = tkey.getLen();
        if(len>512)
            return -3;
        buf = tkey.getIndata();
        wlen[0] = 0;

        ret = ddi_innerkey_ioctl_for_java(2,len,buf,wlen,buf2);

        tkey.setOutdata(buf2);

        return ret;
    }

    public  int ddi_innerkey_ioctl_tkey_decrypt(StrTkey tkey)
    {
        int ret,i,len;
        int[] wlen = new int[1];
		byte[] buf = new byte[525];
		byte[] buf2 = new byte[525];
        len = tkey.getLen();
        if(len>512)
            return -3;
        buf = tkey.getIndata();
        wlen[0] = 0;

        ret = ddi_innerkey_ioctl_for_java(3,len,buf,wlen,buf2);

        tkey.setOutdata(buf2);

        return ret;
    }

    public  static synchronized int ddi_innerkey_ioctl_hkey_encrypt(StrHkey hkey)
    {
        int ret,i,len;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[512];
		byte[] buf = new byte[525];
		byte[] buf2 = new byte[525];
        int lpLen;
        len = hkey.getLen();
        if(len>512)
            return -3;

        bufTemp = IntToByte(hkey.getArea());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(hkey.getIndex());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(hkey.getHalf());
        System.arraycopy(bufTemp, 0, buf, 8, 4);


        bufTemp = hkey.getIndata();
        System.arraycopy(bufTemp, 0, buf, 12, len);


        lpLen = 12+len;
        wlen[0] = 0;

        ret = ddi_innerkey_ioctl_for_java(4,lpLen,buf,wlen,buf2);
        hkey.setOutdata(Arrays.copyOfRange(buf2,0,wlen[0]));
//		if(wlen<=512)
//		{
//			byte[] buf3 = new byte[wlen];
//			for(i=0;i<wlen;i++)
//			{
//				buf3[i] = buf2[i];
//			}
//			hkey.setOutdata(buf3);
//		}
        return ret;
    }

    public  static synchronized int ddi_innerkey_ioctl_hkey_decrypt(StrHkey hkey)
    {
        int ret,i,len;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[512];
        byte[] buf = new byte[512];
        byte[] buf2 = new byte[512];
        int lpLen;
        len = hkey.getLen();
        if(len>512)
            return -3;

        bufTemp = IntToByte(hkey.getArea());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(hkey.getIndex());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(hkey.getHalf());
        System.arraycopy(bufTemp, 0, buf, 8, 4);


        bufTemp = hkey.getIndata();
        System.arraycopy(bufTemp, 0, buf, 12, len);

        lpLen = 12+len;
        wlen[0] = 0;

        ret = ddi_innerkey_ioctl_for_java(5,lpLen,buf,wlen,buf2);
        hkey.setOutdata(buf2);

        return ret;
    }

    public  int ddi_innerkey_ioctl_key_check(int area,int index)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[4];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];

        bufTemp = IntToByte(area);
        System.arraycopy(bufTemp, 0, buf, 0, 4);

        bufTemp = IntToByte(index);
        System.arraycopy(bufTemp, 0, buf2, 0, 4);
        wlen[0] = 4;

        ret = ddi_innerkey_ioctl_for_java(6,4,buf,wlen,buf2);


        return ret;
    }

    public  int ddi_innerkey_ioctl_tak_encryptTak(StrKeyTAKpara TAKpara)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[4];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];

        bufTemp = IntToByte(TAKpara.getArea());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(TAKpara.getTargetIndex());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(TAKpara.getSourceIndex());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        bufTemp = IntToByte(TAKpara.getSaveIndex());
        System.arraycopy(bufTemp, 0, buf, 12, 4);

        wlen[0] = 0;

        ret = ddi_innerkey_ioctl_for_java(7,16,buf,wlen,buf2);


        return ret;
    }

    public static synchronized  native int spi_ddi_certmodule_open();

    public static synchronized  native int spi_ddi_certmodule_close();


    public static synchronized  native int spi_ddi_certmodule_save_sub(byte[] CertName,byte[]  StrID,byte[]  StrLabel,
                                                                       byte ObjectType,byte DataType,byte[]  Length ,
                                                                       byte[] pObjectData,int nDataLength);

    public  int spi_ddi_certmodule_save(HSM_ObjectProperty pObjectProperty,byte[] pObjectData,int nDataLength)
    {
        int ret,i;

        byte[] CertName = new byte[10];
        byte[] StrID = new byte[32];
        byte[] StrLabel = new byte[32];
        byte ObjectType,DataType;
        byte[] DataLength = new byte[4];


        CertName = pObjectProperty.getCertName();
        StrID = pObjectProperty.getStrID();
        StrLabel = pObjectProperty.getStrLabel();
        ObjectType = pObjectProperty.getObjectType();
        DataType = pObjectProperty.getDataType();
        DataLength = pObjectProperty.getDataLength();


        ret = spi_ddi_certmodule_save_sub(CertName,StrID,StrLabel,ObjectType,DataType,
                DataLength,pObjectData,nDataLength);

        return ret;
    }


    public static synchronized  native int spi_ddi_certmodule_readByName_sub(byte[] CertName,byte[]  StrID,byte[]  StrLabel,
                                                                             byte[] ObjectType,byte[] DataType,byte[]  Length ,
                                                                             byte[] pObjectData,int[] nDataLength);

    public  int spi_ddi_certmodule_readByName(HSM_ObjectProperty pObjectProperty,byte[] pObjectData,int[] nDataLength)
    {
        int ret;
        byte[] CertName = new byte[10];
        byte[] StrID = new byte[32];
        byte[] StrLabel = new byte[32];
        byte[] ObjectType  = new byte[1];
        byte[] DataType  = new byte[1];
        byte[] DataLength = new byte[4];


        CertName = pObjectProperty.getCertName();



        ret = spi_ddi_certmodule_readByName_sub(CertName,StrID,StrLabel,ObjectType,DataType
                ,DataLength,pObjectData,nDataLength);

        pObjectProperty.setCertName(CertName);
        pObjectProperty.setStrID(StrID);
        pObjectProperty.setStrLabel(StrLabel);
        pObjectProperty.setObjectType(ObjectType[0]);
        pObjectProperty.setDataType(DataType[0]);


        return ret;

    }


//	public static synchronized  native int spi_ddi_certmodule_readByID_sub(byte[] CertName,byte[]  StrID,byte[]  StrLabel,byte[]  StrPassword,
//		int ObjectType,int DataType,int CertUseMode,int CertLevel,byte[]  Signer_StrID ,byte[]  Signer_StrLabel,
//		byte[] pObjectData,int[] nDataLength);

//	public  int spi_ddi_certmodule_readByID(HSM_ObjectProperty[] pObjectProperty,byte[] pObjectData,int[] nDataLength)
//	{
//		int ret,i;
//
//		byte[] CertName = new byte[10];
//		byte[] StrID = new byte[32];
//		byte[] StrLabel = new byte[32];
//		byte[] StrPassword = new byte[32];
//		int ObjectType,DataType,CertUseMode,CertLevel;
//		byte[] Signer_StrID = new byte[32];
//		byte[] Signer_StrLabel = new byte[32];
//

//		CertName = pObjectProperty[0].getCertName();
//		StrID = pObjectProperty[0].getStrID();
//		StrLabel = pObjectProperty[0].getStrLabel();
//		StrPassword = pObjectProperty[0].getPassword();
//		ObjectType = pObjectProperty[0].getObjectType();
//		DataType = pObjectProperty[0].getDataType();
//		CertUseMode = pObjectProperty[0].getCertUseMode();
//		CertLevel = pObjectProperty[0].getCertLevel();
//		Signer_StrID = pObjectProperty[0].getSignerStrID();
//		Signer_StrLabel = pObjectProperty[0].getSignerStrLabel();

//
//		ret = spi_ddi_certmodule_readByID_sub(CertName,StrID,StrLabel,StrPassword,ObjectType,DataType,
//			CertUseMode,CertLevel,Signer_StrID,Signer_StrLabel,pObjectData,nDataLength);

//		return ret;
//	}
//

    public static synchronized  native int spi_ddi_certmodule_querycount(int[] certnum);


    public static synchronized  native int spi_ddi_certmodule_delete_sub(int certid,byte[] CertName,byte[]  StrID,byte[]  StrLabel,
                                                                         byte ObjectType,byte DataType,byte[]  Length ,
                                                                         byte[] verifydata);

    public  int spi_ddi_certmodule_delete(int certid,HSM_ObjectProperty pObjectProperty,byte[] verifydata)
    {
        int ret,i;

        byte[] CertName = new byte[10];
        byte[] StrID = new byte[32];
        byte[] StrLabel = new byte[32];
        byte ObjectType = 0,DataType = 0;
        byte[] DataLength = new byte[4];


        CertName = pObjectProperty.getCertName();
        ret = spi_ddi_certmodule_delete_sub(certid,CertName,StrID,StrLabel,ObjectType,DataType,DataLength,verifydata);

        return ret;
    }


    public static synchronized  native int spi_ddi_certmodule_deleteall();


    public static synchronized  native int ddi_led_open();

    public static synchronized  native int ddi_led_close();

    public static synchronized  native int ddi_led_sta_set(int nLed, int nSta);

    public static synchronized  native int ddi_led_ioctl_for_java(int nCmd,int lplen,byte[] lParam,int[] wplen,byte[] wParam);

    public  int ddi_led_ioctl_getVer(byte[] wParam)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] buf = new byte[20];
        ret = ddi_led_ioctl_for_java(0,0,buf,wlen,wParam);


        return ret;
    }
    public  int ddi_led_ioctl_gleam(StrLedGleamPara Para)
    {
        int ret;
        int[] wlen = new int[1];
        byte[] bufTemp = new byte[4];
        byte[] buf = new byte[20];
        byte[] buf2 = new byte[20];

        bufTemp = IntToByte(Para.getLed());
        System.arraycopy(bufTemp, 0, buf, 0, 4);
        bufTemp = IntToByte(Para.getOntime());
        System.arraycopy(bufTemp, 0, buf, 4, 4);
        bufTemp = IntToByte(Para.getOfftime());
        System.arraycopy(bufTemp, 0, buf, 8, 4);
        bufTemp = IntToByte(Para.getDuration());
        System.arraycopy(bufTemp, 0, buf, 12, 4);
        wlen[0] = 0;
        ret = ddi_led_ioctl_for_java(1,16,buf,wlen,buf2);


        return ret;
    }

    public static synchronized  native int ddi_dukpt_open();

    public static synchronized  native int ddi_dukpt_close();

    public static synchronized  native int ddi_dukpt_inject_sub(byte m_groupindex,byte m_keyindex,byte[] m_initkey,
                                                                byte m_keylen,byte m_ksnindex,byte[] m_initksn,byte m_ksnlen);  //

    public int ddi_dukpt_inject(StrDukptInitInfo initInfo)
    {
        int ret;
        byte groupindex,keyindex,keylen,ksnindex,ksnlen;
        byte[] initkey = new byte[24];
        byte[] initksn = new byte[20];

        groupindex = initInfo.getGroupindex();
        keyindex = initInfo.getKeyindex();
        keylen = initInfo.getKeylen();
        ksnindex = initInfo.getKeyindex();
        ksnlen = initInfo.getKsnlen();
        initkey = initInfo.getInitkey();
        initksn = initInfo.getInitksn();

        ret = ddi_dukpt_inject_sub(groupindex,keyindex,initkey,keylen,ksnindex,initksn,ksnlen);

        return ret;
    }

    public static synchronized  native int ddi_dukpt_encrypt(int nKeyGroup,int nKeyIndex,int nLen,  byte[] lpIn, byte[] lpOut, int nMode);

    public static synchronized  native int ddi_dukpt_decrypt(int nKeyGroup,int nKeyIndex,int nLen,  byte[] lpIn, byte[] lpOut, int nMode);

    public static synchronized  native int ddi_dukpt_getksn(int nKeyGroup,int nKeyIndex,int[] nLen, byte[] lpOut);

    public static synchronized  native int ddi_dukpt_ioctl(int nCmd,int lParam,int wParam);


    public static synchronized  native int setGpioValue(int gpio_no, int gpio_val);

    public static synchronized  native int getGpioValue(int gpio_no);

    public static synchronized  native int setScannerEnable(int on);

    public static synchronized  native int getScannerEnableStatus();

    public static synchronized  native int setScannerPowerOn(int on);

    public static synchronized  native int getScannerPowerOnStatus();

    public static synchronized  native int setScannerUsbSwitch(int on);

    public static synchronized  native int getScannerUsbSwitchStatus();

    public static synchronized  native int ddi_key_open();

    public static synchronized  native int ddi_key_close();

    public static synchronized  native int ddi_key_clear();

    public static synchronized  native int ddi_key_read(int[] key);

    public static synchronized  native int ddi_key_ioctl(int nCmd,int lParam,int wParam);


    public static synchronized  native int ddi_pin_input(int pinlenmin,int pinlenmax,int timeout,int bypassflag);

    public static synchronized  native int ddi_pin_input_press(byte[] keycode);

    public static synchronized  native int ddi_pin_input_cancel();

    public static synchronized  native int ddi_pin_getonlinepinblock(int pinAlgorithmMode,int keyindex,int cardlen,byte[] carddata,byte[] pinblockdata);





    public static synchronized  native int ddi_security_getstatus(byte[] statusdata);

    public static synchronized  native int ddi_spi_communication_test(byte[] data,int nLen);

    public static synchronized  native int ddi_sys_get_Hardware_Ver();

    public static synchronized  native int ddi_spi_ddi_sys_set_dsn(int nLen,byte[] data);
//	public static synchronized  native int ddi_TcpClientEntry();


    //emv

    public static synchronized  native int ddi_rf_emv_get_version(byte[] statusdata);

    public static synchronized  native int ddi_rf_emv_open();

    public static synchronized  native int ddi_rf_emv_close();

    public static synchronized  native int ddi_rf_carrier_ctl(byte ctl);

    public static synchronized  native int ddi_rf_emv_polling(byte pol);

    public static synchronized  native int ddi_rf_emv_send_wupa();

    public static synchronized  native int ddi_rf_emv_send_wupb();

    public static synchronized  native int ddi_rf_emv_send_rats();

    public static synchronized  native int ddi_rf_emv_send_attrib();

    public static synchronized  native int ddi_rf_emv_digital();

    public static synchronized  native int ddi_rf_emv_get_param_sub(byte[] data);

    public static synchronized  native int ddi_rf_emv_set_param_sub(byte[] data);



	//GM PROTOCOL
	//aaron add 7.12.2016
	public static synchronized native int ddi_thk88_open();
	public static synchronized native int ddi_thk88_close();
	public static synchronized native int ddi_thk88_read_version(byte[] pVersion, int versionLen);
	public static synchronized native int ddi_thk88_read_model(byte[] pModel, int ModelLen);
	public static synchronized native int ddi_thk88_sleep(byte sleep);
	public static synchronized native int ddi_thk88_reset(byte reset);
	public static synchronized native int ddi_sm1_encrypt_sw(byte[]plain,int plain_len,byte[]cipher,int[]cipher_len,byte[]sk,byte[]ak,byte[]ek,byte mode,byte[]init_vect);
	public static synchronized native int ddi_sm1_decrypt_sw(byte[]cipher,int cipher_len,byte[]plain,int[]plain_len,byte[]sk,byte[]ak,byte[]ek,byte mode,byte[]init_vect);
	public static synchronized native int ddi_sm2_gen_keypair_sw(byte[]pubkey,byte[]prikey);
	public static synchronized native int ddi_sm2_encrypt_sw(byte[]plain,int plain_len,byte[]cipher,int[]cipher_len,byte[]pubkey);
	public static synchronized native int ddi_sm2_decrypt_sw(byte[]cipher,int cipher_len,byte[]plain,int[]plain_len,byte[]prikey);
	public static synchronized native int ddi_sm2_sign_sw(byte[]id,int id_len,byte[]plain,int plain_len,byte[]pubkey,byte[]prikey,byte[]cipher);
	public static synchronized native int ddi_sm2_verify_sw(byte[]id,int id_len,byte[]plain,int plain_len,byte[]pubkey,byte[]cipher);
	public static synchronized native int ddi_sm3_hash_sw(byte group,byte[]msg,int msg_len,byte[]hash);
	public static synchronized native int ddi_sm3_id_hash_sw(byte[]id,int id_len,byte[]msg,int msg_len,byte[]pubkey,byte[]hash);
	public static synchronized native int ddi_sm4_encrypt_sw(byte[]plain,int plain_len,byte[]cipher,int[]cipher_len,byte[]key,byte mode,byte[]init_vect);
	public static synchronized native int ddi_sm4_decrypt_sw(byte[]cipher,int cipher_len,byte[]plain,int[]plain_len,byte[]key,byte mode,byte[]init_vect);
	public static synchronized native int ddi_thk88_erase(int startAddr, int eraseLen);
	public static synchronized native int ddi_thk88_program(int startAddr, byte[] pProgramData, int programDataLen);
	public static synchronized native int ddi_thk88_check(int startAddr, byte[] pCheckData, int checkDataLen);



	//rtc 
	public static synchronized native int ddi_dev_get_rtc_time(byte[] Dt,int[] len);
	public static synchronized native int ddi_dev_set_rtc_time(byte[] Dt,int len);

	
	public static synchronized native int ddi_spi_reset();

    public static synchronized  native int ddi_spi_logon();

    public static synchronized  native int ddi_spi_logoff();

    public static synchronized  native int ddi_sys_get_time(byte[] nTime);

    public static synchronized  native int ddi_sys_get_tick(int[] nTime);

    public static synchronized  native int ddi_sys_getSystemTime(int[] nTime);

	public static synchronized native int ddi_sys_ddi_version(byte[] version);

//	public native int ddi_sys_ieComm_delete_cert();

//	public native int ddi_sys_ieComm_set_debugStatus();

    //	public static synchronized  native int ddi_get_FPC_image(byte[] image,int offset,int length);     //
    public static synchronized int ddi_get_model()
    {
        Build bd = new Build();
        String s1 = "Octopus A83 F1";
        String s2 = "msm8909";
        String s2_2 = "N5";
        String model = bd.MODEL;
        System.err.println("ddi_get_version:"+model);
        if(s1.equals(model)==true)
            return 1;
        else if(s2.equals(model)==true)
            return 2;
        else if(s2_2.equals(model)==true)
            return 2;
        else
            return 0;
    }
    int b_ntohl(byte[] buff)
    {
        int data = 0;
        data += ((buff[0]<<24)&0xff000000);
        data += ((buff[1]<<16)&0x00ff0000);
        data += ((buff[2]<<8 )&0x0000ff00);
        data += ((buff[3]<<0 )&0x000000ff);
        return data;
    }

    static synchronized byte[] IntToByte(int data)
    {
        byte[]  buf =  new byte[4];
        buf[0] = (byte)(data&0xff);
        buf[1] = (byte)(data>>8&0xff);
        buf[2] = (byte)(data>>16&0xff);
        buf[3] = (byte)(data>>24&0xff);
        return buf;
    }
    static synchronized int ByteToInt(byte[] data)
    {
        int temp = 0;
        temp+=((data[0]<<0 )&0x000000ff);
        temp+=((data[1]<<8 )&0x0000ff00);
        temp+=((data[2]<<16 )&0x00ff0000);
        temp+=((data[3]<<24 )&0xff000000);

        return temp;
    }

}
