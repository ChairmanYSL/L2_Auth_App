package com.nexgo.emv;

import android.app.AlertDialog;
import android.app.Application;
import android.widget.ImageView;

import com.nexgo.emv.key.PosKeyboard;
import com.nexgo.emv.lcd.PosLcd;
import com.nexgo.emv.MainActivity;

public class XGDApp extends Application {

    private static XGDApp singleton = new XGDApp();
    private PosLcd tm_lcd;
    private PosKeyboard tm_keypad;
    public MainActivity mainActivity;
    public MainApplication mainApplication;

    public static XGDApp getInstance() {
        return singleton;
    }

    public PosLcd getPosLcd() {
        return tm_lcd;
    }

    public void setPosLcd(PosLcd posLcd) {
        this.tm_lcd = posLcd;
    }

    public PosKeyboard getPosKeyboard() {
        return tm_keypad;
    }

    public void setPosKeyboard(PosKeyboard posKeyboard) {
        this.tm_keypad = posKeyboard;
    }

    public MainApplication getMainApplication(){return mainApplication;}

    public void setMainApplication(MainApplication mainApplication){this.mainApplication = mainApplication;}

}
