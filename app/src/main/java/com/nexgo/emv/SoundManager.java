package com.nexgo.emv;

import android.media.AudioManager;
import android.media.SoundPool;
import android.util.Log;

import java.io.File;
import java.util.Timer;
import java.util.TimerTask;


/**
 * Created by xiaox on 2017/9/21.
 */

public class SoundManager {
    private static final int MAX_NUMER_STREAMS = 1;
    private static final int SOURCE_QUALITY = 0;
    private static final String BEEP_FILE_PATH ="/system/media/beep.wav";
    private final SoundPool mSoundPool;
    private int mSoundId;
    private Timer mTimer;
    private int mStreamId;

    public SoundManager() {
        mSoundPool = new SoundPool(MAX_NUMER_STREAMS, AudioManager.USE_DEFAULT_STREAM_TYPE, SOURCE_QUALITY);
    }

    public void beep(final int period) {
        if (period == 0) {
            if (mTimer != null) {
                mTimer.cancel();
                mTimer = null;
            }
            if (mStreamId != 0) {
                stop(mStreamId);
                mStreamId = 0;
            }
            return;
        }
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
                if (mSoundId == 0) {
                    File file = new File(BEEP_FILE_PATH);
                    if (file.exists()) {
                        mSoundId = mSoundPool.load(BEEP_FILE_PATH, 1);
                        mSoundPool.setOnLoadCompleteListener(new SoundPool.OnLoadCompleteListener() {
                            @Override
                            public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
                                play(mSoundId, period);
                            }
                        });
                    } else {
                        Log.e("SoundManager", "beep文件不存在");
                    }
                } else {
                    play(mSoundId, period);
                }
//            }
//        }).start();
    }


    private void play(int loadId, int period) {
        mStreamId = mSoundPool.play(loadId, 0.5f, 0.5f, 1, -1, 1f);
        mTimer = new Timer();
        mTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                stop(mStreamId);
                if (mTimer != null) {
                    mTimer.cancel();
                    mTimer = null;
                }
            }
        }, period);
    }


    private void stop(int streamId) {
        mSoundPool.stop(streamId);
    }
}
