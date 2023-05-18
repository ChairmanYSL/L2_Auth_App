package com.nexgo.emv;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Build;
import android.os.Environment;
import android.util.Log;

import com.nexgo.emv.key.PosTimer;
import com.szzt.android.util.HexDump;
import com.szzt.sdk.device.beep.Beep;
import com.szzt.sdk.device.card.ContactlessCardReader;
import com.szzt.sdk.device.port.SerialPort;
import com.szzt.sdk.system.HwSecurityManager;

import java.io.File;
import java.nio.ByteBuffer;
import java.util.logging.Logger;


/**
 * Created by 28070 on 2016/5/19.
 */
public class DeviceOpera {
    private MainApplication mainApplication;
    private ContactlessCardReader mCard;
    private SerialPort port;
    public int portId;
    public PosTimer timer;
//    private HwSecurityManager hwSecurityManager;
    public DeviceOpera(){
        mainApplication = XGDApp.getInstance().getMainApplication();
        mCard=mainApplication.getContactlessCardReader();
        port=mainApplication.getSerialPortWrapperImpl();
        mainApplication.getFileSystemWrapperImpl();
//        hwSecurityManager=mainApplication.getHwSecurityManager();
    }

    public int testCallJava(int i) {

        Log.e("CallJava", "" + i);
        return i;
    }


    public void Lcd_Cls() {
        XGDApp.getInstance().getPosLcd().Lcd_Cls();
    }

    public void Lcd_Printf(String dispText) {

        Log.d("lishiyao", "Call Lcd_Printf: ");
        try {
            XGDApp.getInstance().getPosLcd().Lcd_Printf(dispText);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void Lcd_ClearLine(int startLine, int endLine) {
        XGDApp.getInstance().getPosLcd().Lcd_ClearLine(startLine, endLine);
    }

    public void Lcd_GotoXY(int line, int col) {
        XGDApp.getInstance().getPosLcd().Lcd_GotoXY(line, col);
    }

    public void Lcd_PrintfXY(int line, int col, String dispText, int attr) {
//        Log.d("lishiyao", "Call Lcd_PrintfXY: ");
        try {
            XGDApp.getInstance().getPosLcd().Lcd_PrintfXY(line, col, dispText, attr);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void Lcd_SetFont(int FontType) {
        XGDApp.getInstance().getPosLcd().Lcd_SetFont(FontType);
    }

    //按键处理
    public void Kb_Flush() {
        XGDApp.getInstance().getPosKeyboard().Kb_Flush();
    }

   /* public native void KbStdin(int keyCode) {
        XGDApp.getInstance().getPosKeyboard().KbStdin(keyCode);
    } */

    public int Kb_Hit() {
        return XGDApp.getInstance().getPosKeyboard().Kb_Hit();
    }

    public int Kb_GetKey() {
        return XGDApp.getInstance().getPosKeyboard().Kb_GetKey();

    }

    public void beeper(int timer) {
        Beep beeper = mainApplication.getBeep();
        beeper.beep(timer);
    }

    public int openRF() {
        int ret;
        if(mCard!=null)
        {
            ret = mCard.open();
            Log.d("lishiyao", "openRF ret: "+ret);
            return ret;
        }
        else
        {
            return -1200;
        }
    }



    public int ResetRF(){
        int ret=mCard.waitForCard(5*1000);
        if(ret==0){
            ret = mCard.powerOn(new byte[256]);
            Log.d("lishiyao", "ResetRF ret: "+ret);
        }
        return ret;
    }

    public void CloseRF(){
        mCard.cancel();
    }

    public int OpenSerial(){
//        Log.d("lishiyao", "call OpenSerial: ");
//        Log.d("lishiyao", "port: "+port.toString());
        if(port!=null)
        {
            portId = port.open("/dev/ttyUSB0", 115200);
//            Log.d("lishiyao", "port.getStatus: "+port.getStatus());
//            Log.d("lishiyao", "portId: "+portId);
            if(portId < 0)
            {
                port.close();
                port.open("/dev/ttyUSB0", 115200);
            }
            return portId;
        }
        else
        {
            return -100;
        }
    }

    public int SendSerial(byte [] sendData, int sendLen){
//        Log.d("lishiyao", "Call SendSerial: ");
        int ret;
        Log.d("lishiyao", "SendSerial data:"+ HexDump.getHexString(sendData));
//        Log.d("lishiyao", "portId: "+portId);
        ret = port.send(portId, sendData, sendLen);
        Log.d("lishiyao", "ret: "+ret);
        return ret;
    }

//    public byte [] GetRandom(int siNum){
//        byte []
//        hwSecurityManager.getRandom();
//    }

    public byte [] ReadSerial(int readLen, int timeout){
//        Log.d("lishiyao", "Call ReadSerial: ");
        int ret;
        byte [] receiveData = new byte[readLen];
        ret = port.receive(portId, receiveData, readLen, timeout);
//        Log.d("lishiyao", "data: "+HexDump.dumpHex(receiveData));
//        Log.d("lishiyao", "ret: "+ret);
        if(receiveData.toString() == "")

        if(ret <= 0)
        {
            return null;
        }
        if(ret>=4){
            if(receiveData[0]!=0x02){
                byte[] data=new byte[ret];
                System.arraycopy(receiveData,0,data,0,ret);
                Log.d("txq","data:"+HexDump.toHexString(data));
                return data;
            }
            Log.d("txq","data:"+HexDump.toHexString(receiveData,0,ret));
            byte[] d=new byte[4];
            d[2]=receiveData[2];
            d[3]=receiveData[3];
            Log.d("txq","data:"+HexDump.toHexString(d));
            int len=HexDump.byteToInt(d);
            long start=System.currentTimeMillis();
            Log.d("txq","len:"+len+":"+ret);
            int index=0;
            byte[] retData=new byte[len+4];
            System.arraycopy(receiveData,0,retData,index,ret);
            index+=ret;
            while (true){
                Log.d("txq","index:"+index);
                if(index>=retData.length){
                    return retData;
                }
               else{
                    ret=port.receive(portId, receiveData, readLen, timeout);
                    System.arraycopy(receiveData,0,retData,index,ret);
                    index+=ret;

                }
            }
        }
        return null;

    }

    public void CloseSerial(int portId){
        port.close(portId);
    }

    public void TimerStart(int ms){
        timer = new PosTimer(ms);
        timer.timerStart();
    }

    public boolean TimerisEnd(){
        return timer.checkTimerExpired();
    }
}
