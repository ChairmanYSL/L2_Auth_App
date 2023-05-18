package com.nexgo.emv;

import android.app.Application;
import android.content.Intent;
import android.media.AudioFormat;
import android.media.AudioTrack;
import android.os.Handler;
import android.support.v4.content.LocalBroadcastManager;
import android.widget.Toast;

import com.szzt.android.util.SzztDebug;
import com.szzt.sdk.device.Device;
import com.szzt.sdk.device.DeviceManager;
import com.szzt.sdk.device.DeviceManager.DeviceManagerListener;
import com.szzt.sdk.device.FileSystem;
import com.szzt.sdk.device.barcode.Barcode;
import com.szzt.sdk.device.barcode.CameraScan;
import com.szzt.sdk.device.beep.Beep;
import com.szzt.sdk.device.card.ContactlessCardReader;
import com.szzt.sdk.device.card.IDCardReader;
import com.szzt.sdk.device.card.MagneticStripeCardReader;
import com.szzt.sdk.device.card.SmartCardReader;
import com.szzt.sdk.device.emv.EmvInterface;
import com.szzt.sdk.device.led.Led;
import com.szzt.sdk.device.pinpad.PinPad;
import com.szzt.sdk.device.port.SerialPort;
import com.szzt.sdk.device.printer.Printer;
import com.szzt.sdk.system.HwSecurityManager;
import com.szzt.sdk.system.SystemManager;
import com.szzt.sdk.system.net.NetManager;

import java.io.InputStream;


public class MainApplication extends Application {
    private DeviceManager mDeviceManager;
    private SystemManager mSystemManager;
    private boolean isConnect = false;
    LocalBroadcastManager mLocalBroadcastManager;
    public static String ACTION_SERVICE_CONNECTED = "DEVICEMANAGER_SERVICE_CONNECTED";
    public static String ACTION_SERVICE_DISCONNECTED = "DEVICEMANAGER_SERVICE_DISCONNECTED";
    Intent mServiceConnectedIntent = new Intent(ACTION_SERVICE_CONNECTED);
    Intent mServiceDisConnectedIntent = new Intent(ACTION_SERVICE_DISCONNECTED);
    public static int printIndex = 0;

    @Override
    public void onCreate() {
        // TODO Auto-generated method stub
        mDeviceManager = DeviceManager.createInstance(this);
        mDeviceManager.start(deviceManagerListener);
        getSystemManager();
        mLocalBroadcastManager = LocalBroadcastManager.getInstance(this);
        super.onCreate();
    }

    public SerialPort getSerialPortWrapperImpl() {
        Device[] pinPads = mDeviceManager.getDeviceByType(Device.TYPE_SERIALPORT);
        if (pinPads != null)
            return (SerialPort) pinPads[0];
        return null;
    }
    
   /* public Android getAndroid(){
    	Device[] android = mDeviceManager.getDeviceByType(Device.TYPE_ANDROID);
    	if(android!=null){
    		return (Android) android[0];
    	}return null ;
    }*/
    
    /*public Identifier getIdentifier(){
    	Device[] identify = mDeviceManager.getDeviceByType(Device.TYPE_IDC);
    	if(identify!=null){
    		return (Identifier)identify[0];
    	}return null ;
    }*/

    public Led getLedWrapperImpl() {
        Device[] pinPads = mDeviceManager.getDeviceByType(Device.TYPE_LED);
        if (pinPads != null)
            return (Led) pinPads[0];
        return null;
    }

    public FileSystem getFileSystemWrapperImpl() {
        Device[] pinPads = mDeviceManager.getDeviceByType(Device.TYPE_FILESYSTEM);
        if (pinPads != null)
            return (FileSystem) pinPads[0];
        return null;
    }
   /* public Net getNetWrapperImpl(){
    	Device[] pinPads = mDeviceManager.getDeviceByType(Device.TYPE_NET);
        if (pinPads != null)
            return (Net) pinPads[0];
        return null;
    }*/

    public SmartCardReader getSmartCardReader() {
        Device[] smartCardReaders = mDeviceManager.getDeviceByType(Device.TYPE_SMARTCARDREADER);
        if (smartCardReaders != null)
            return (SmartCardReader) smartCardReaders[0];
        return null;
    }

    public PinPad getPinPad() {
        Device[] pinPads = mDeviceManager.getDeviceByType(Device.TYPE_PINPAD);
        if (pinPads != null)
            return (PinPad) pinPads[0];
        return null;
    }
    
   /* public D5000Printer getD5000P() {
        Device[] pinPads = mDeviceManager.getDeviceByType(Device.TYPE_D5000);
        if (pinPads != null)
            return (D5000Printer) pinPads[0];
        return null;
    }*/

    public EmvInterface getEmvInterface() {
        return mDeviceManager.getEmvInterface();
    }

    AudioTrack at;

    public synchronized int wavePlay(byte[] value, final float volume) {
        if (at != null) {
            at.stop();
            at.release();
            at = null;
        }
        byte[] buffer = value;
        int pcmlen = value.length;
        int channel = 1;
        at = new AudioTrack(7, 8000,
                channel,
                AudioFormat.ENCODING_PCM_8BIT,
                pcmlen,
                AudioTrack.MODE_STATIC);
        at.write(buffer, 0, pcmlen);
        at.play();
        return 0;
    }

    private Handler mHandler = new Handler();

    public Printer getPrinter() {
        Device[] printers = mDeviceManager.getDeviceByType(Device.TYPE_PRINTER);
        if (printers != null)
            return (Printer) printers[0];
        return null;
    }

    public Beep getBeep() {
        Device[] printers = mDeviceManager.getDeviceByType(Device.TYPE_BEEP);
        if (printers != null)
            return (Beep) printers[0];
        return null;
    }

    public ContactlessCardReader getContactlessCardReader() {
        Device[] contactlessCards = mDeviceManager
                .getDeviceByType(Device.TYPE_CONTACTLESSCARDREADER);
        if (contactlessCards != null)
            return (ContactlessCardReader) contactlessCards[0];
        return null;
    }

    public MagneticStripeCardReader getMagneticStripeCardReader() {
        Device[] magStripCards = mDeviceManager
                .getDeviceByType(Device.TYPE_MAGSTRIPECARDREADER);
        if (magStripCards != null)
            return (MagneticStripeCardReader) magStripCards[0];

        return null;
    }

    public IDCardReader getIDCardReader() {
        Device[] idCards = mDeviceManager.getDeviceByType(Device.TYPE_IDCARDREADER);
        if (idCards != null)
            return (IDCardReader) idCards[0];
        return null;
    }


    public SystemManager getSystemManager() {
        if (mSystemManager == null) {
            mSystemManager = SystemManager.getInstance(this, new SystemManager.SystemManagerListener() {
                @Override
                public int serviceEventNotify(int i) {
                    return 0;
                }
            });
        }
        return mSystemManager;
    }

    public HwSecurityManager getHwSecurityManager() {
        getSystemManager();
        if (mSystemManager != null) {
            return mSystemManager.getHwSecurityManager();
        } else return null;
    }

    public NetManager getNetworkManager() {
        getSystemManager();
        return mSystemManager.getNetManager();
    }

    public boolean isDeviceManagerConnetcted() {
        return isConnect;
    }

    @Override
    public void onTerminate() {
        mDeviceManager.stop();
        DeviceManager.destroy();
        mDeviceManager = null;
        super.onTerminate();
    }

    public CameraScan getCameraScanImpl() {
        Device[] barcode = mDeviceManager.getDeviceByType(Device.TYPE_CAMERA_SCAN);
        if (barcode != null)
            return (CameraScan) barcode[0];

        return null;
    }

    public Barcode getBarcodeImpl() {
        Device[] barcode = mDeviceManager.getDeviceByType(Device.TYPE_BARCODE);
        if (barcode != null)
            return (Barcode) barcode[0];

        return null;
    }

    DeviceManagerListener deviceManagerListener = new DeviceManagerListener() {
        @Override
        public int serviceEventNotify(
                int event) {
            if (event == EVENT_SERVICE_CONNECTED) {
                isConnect = true;
                //mLocalBroadcastManager.sendBroadcast(mServiceConnectedIntent);
            } else if (event == EVENT_SERVICE_VERSION_NOT_COMPATABLE) {
                Toast.makeText(MainApplication.this, "SDK Version is not compatable!!!", Toast.LENGTH_SHORT).show();
            } else if (event == EVENT_SERVICE_DISCONNECTED) {
                isConnect = false;
                mDeviceManager.start(deviceManagerListener);
                //mLocalBroadcastManager.sendBroadcast(mServiceDisConnectedIntent);
            }
            return 0;
        }

        @Override
        public int deviceEventNotify(
                Device device,
                int event) {
            // TODO Auto-generated method stub
            return 0;
        }
    };
}