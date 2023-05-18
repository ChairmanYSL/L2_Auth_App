package com.nexgo.emv;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.inputmethodservice.KeyboardView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.LinearLayout;

import com.nexgo.emv.key.PosKeyboardView;
import com.nexgo.emv.lcd.PosLcd;
import android.widget.ImageView;
import com.nexgo.emv.XGDApp;

public class MainActivity extends Activity {

    private PosLcd tm_lcd;
    private PosKeyboardView kbView;
    private MainApplication mainApplication;
    private AlertDialog mAlertDialog;
    private  ImageView imageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);//绑定xml文件


        DisplayMetrics dm = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        tm_lcd = new PosLcd(this,dm.widthPixels, (int)(dm.heightPixels * 0.4),m_handler);

        LinearLayout tm_layout = (LinearLayout) findViewById(R.id.lcdView);
        tm_layout.addView(tm_lcd);

        kbView = new PosKeyboardView(this.getBaseContext(), m_handler,
                (KeyboardView) findViewById(R.id.keyboard_view),
                R.layout.poskeyboard, tm_lcd, new PosKeyboardView.keyListener() {
            @Override
            public void sendKeycode(int keyCode) {
                Log.e("txq","keyCode:"+keyCode);
                kbView.m_PosKb.setKeyCode(keyCode);
            }
        });

        mainApplication = (MainApplication)getApplication();

        dialogInit();

        XGDApp.getInstance().mainActivity = this;
        XGDApp.getInstance().setPosLcd(tm_lcd);
        XGDApp.getInstance().setPosKeyboard(kbView.m_PosKb);
        XGDApp.getInstance().setMainApplication(mainApplication);

        Thread newThread;
        newThread = new Thread(new Runnable() {
            @Override
            public void run() {
                JniNative.appmain();
            }
        });
        newThread.setPriority(10);
        newThread.start();
    }

    private Handler m_handler = new Handler(){
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 1:
                    //Log.d("case 1","case1------------");
                    tm_lcd.invalidate();
                    break;
                case 2:
                    Log.d("nexgo", "dialog");
                    if (mAlertDialog != null) {
                        mAlertDialog.show();
                    }

                    break;
                case 3:
                    Log.d("nexgo", "SHOW RFLOGO");
                    if (imageView != null)
                    {
                        imageView.setImageResource(R.drawable.symbol);
                        imageView.setX(150);
                        imageView.setY(110);
                    }
                    break;

                case 4:
                    Log.d("nexgo", "CLOSE RFLOGO");
                    if (imageView != null) {

                        imageView.setImageResource(0);
                        //imageView.setWillNotDraw(true);
                    }
                    break;
            }
            super.handleMessage(msg);
        }
    };

    private void dialogInit() {
        Log.d("nexgo", "dialogInit");
        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
        builder.setTitle("Warning");
        builder.setMessage("Exit EmvL2 test?");
        //builder.setIcon(R.drawable.symbol);


        builder.setNegativeButton("Canel", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
            }
        });

        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
//                JniNative.appoutprocess();
//                System.exit(0);
            }
        });

        mAlertDialog = builder.create();
    }

    protected void dialog() {

        m_handler.sendEmptyMessage(2);

    }

    protected void ShowRfLogo() {

        m_handler.sendEmptyMessage(3);

    }

    protected void CloseRfLogo() {

        m_handler.sendEmptyMessage(4);

    }

}
