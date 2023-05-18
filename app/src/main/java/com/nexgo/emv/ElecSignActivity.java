package com.nexgo.emv;

import android.app.Activity;
import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Environment;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * 作者:liuting on 2016/9/9 01:16
 * 邮箱:liuting@xinguodu.com
 * 项目名：bleFT
 * 包名：com.xgd.showdemo.activity
 * TODO: 电签演示
 */
public class ElecSignActivity extends Activity {
    private ImageView mIv_arrow;
    private ImageView mIv_sign;
    private Bitmap mBitmap;
    private Canvas mCanvas;
    private Paint mPaint;
    private final int paintStrokeWidth = 4;

    private int mStartX;
    private int mStartY;
    private TextView mTv_confirm;
    private TextView mTv_repeat;

    private ProgressDialog mProgressDialog;
    private Logger log = LoggerFactory.getLogger(ElecSignActivity.class.getSimpleName());

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_elecsign);
        mIv_arrow = (ImageView) findViewById(R.id.arrow_title);
        mIv_sign = (ImageView) findViewById(R.id.iv_elecsign);
        mTv_confirm = (TextView) findViewById(R.id.tv_sign_confirm);
        mTv_repeat = (TextView) findViewById(R.id.tv_sign_repeat);
        mProgressDialog = new ProgressDialog(this);
        mProgressDialog.setCanceledOnTouchOutside(false);
        log = LoggerFactory.getLogger(getClass().getName());
        initData();
        initEvent();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            init();
        }
    }

    private void initData() {
    }


    private void init() {
        mPaint = new Paint();
        mPaint.setColor(Color.BLACK);
        mPaint.setAntiAlias(true);
        mPaint.setDither(true);
        mPaint.setColor(Color.BLACK);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeJoin(Paint.Join.ROUND);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setStrokeWidth(paintStrokeWidth);
        mBitmap = Bitmap.createBitmap(mIv_sign.getWidth(), mIv_sign.getHeight(), Bitmap.Config.ARGB_8888);
        mCanvas = new Canvas(mBitmap);
        mCanvas.drawColor(Color.WHITE);
    }

    private void clear() {
        mBitmap = null;
        mIv_sign.setImageBitmap(null);
        mBitmap = Bitmap.createBitmap(mIv_sign.getWidth(), mIv_sign.getHeight(), Bitmap.Config.ARGB_8888);
        mCanvas = new Canvas(mBitmap);
        mCanvas.drawColor(Color.WHITE);
    }

    private void initEvent() {
        mTv_confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                saveBitmap(mBitmap);
                finish();
            }
        });

        mTv_repeat.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                clear();
            }
        });


        mIv_arrow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        mIv_sign.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        mStartX = (int) event.getX();
                        mStartY = (int) event.getY();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        int moveX = (int) event.getX();
                        int moveY = (int) event.getY();
                        mCanvas.drawLine(mStartX, mStartY, moveX, moveY, mPaint);
                        mIv_sign.setImageBitmap(mBitmap);
                        mStartX = moveX;
                        mStartY = moveY;
                        break;
                    default:
                        break;
                }
                return true;
            }
        });
    }

    private void saveBitmap(Bitmap bitmap){
        String filePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "SignatureEmv.jpg";
        try {
            File file = new File(filePath);
            if (file.exists()) {
                file.delete();
            }
            if (!file.exists()) {
                file.createNewFile();
            }
            FileOutputStream out = new FileOutputStream(file);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.flush();
            out.close();
        }catch (IOException e){
            log.error("save file error", e);
        }
    }

}
