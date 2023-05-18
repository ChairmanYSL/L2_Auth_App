package com.nexgo.emv.print;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Matrix;

import com.nexgo.common.ByteUtils;
import com.nexgo.emv.util.BmpUtils;
import com.xinguodu.ddiinterface.Ddi;
import com.xinguodu.ddiinterface.DdiConstant;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by xiaox on 16/4/20.
 */
public class PrinterManager {

    private final String TAG = "PrinterManager";
    private boolean startPrintIsRunning = false;
    private int letterSpacing = 8;
    private List<PrinterData> tmpPrinters = new ArrayList<>();
    private Context mcontext;
    BmpUtils bmpUtils = new BmpUtils();

    public PrinterManager(Context context) {
        mcontext = context;
    }

    public int initPrinter() {
        return SdkResult.Success;
    }

    public void setLetterSpacing(int value) {
        letterSpacing = value;
    }

    public void setGray(GrayLevelEnum level) {
        switch (level) {
            case LEVEL_0:
                Ddi.ddi_thmprn_ioctl_setGray(0);
                break;
            case LEVEL_1:
                Ddi.ddi_thmprn_ioctl_setGray(1);
                break;
            case LEVEL_2:
                Ddi.ddi_thmprn_ioctl_setGray(2);
                break;
        }
    }

    public int startPrint() {
        if (startPrintIsRunning) {
            return SdkResult.Printer_Busy;

        }
        if (tmpPrinters.size() == 0) {
            return SdkResult.Printer_Wrong_Package;
        }
//        new Thread() {
//            public void run() {
                int result = SdkResult.Fail;
                try {
                    startPrintIsRunning = true;
                    int state = -1;
                    final byte[] data = bmpUtils.toBmpBytes(mcontext,tmpPrinters);
                    tmpPrinters.clear();
                    Ddi.ddi_thmprn_open();
                    state = Ddi.ddi_thmprn_print_oneBitBMPImageByBuffer(data, data.length);
                    if (state == DdiConstant.DDI_OK) {
                        result = SdkResult.Success;
                    } else {
                        result = SdkResult.Printer_Print_Fail;
                    }
                    tmpPrinters.clear();
                    if (result == SdkResult.Success) {
                        result = waitForFinish();
                        Ddi.ddi_thmprn_close();
                    } else {
                        Ddi.ddi_thmprn_close();
                        Ddi.ddi_thmprn_open();
                        state = Ddi.ddi_thmprn_get_status();
                        Ddi.ddi_thmprn_close();
                        result = convertStatus(state);
                        if (result == SdkResult.Success) {
                            //上面打印失败了,查状态又成功，说明上面的错误不是打印机的错识,有可能是入参有误等
                            result = SdkResult.Printer_Print_Fail;
                        }
                    }
                    startPrintIsRunning = false;
                } catch (Exception e) {
                    e.printStackTrace();
                } finally {
                    startPrintIsRunning = false;
                }
//            }
//        }.start();
        return SdkResult.Success;
    }

    public int getStatus() {
        try {
            //此时如果缺纸，直接返回3  不会像以前那样返回 2了 ，因此返回3 缺纸   4 过热，  无需循环检测,5电量低
            Ddi.ddi_thmprn_open();
            int state = Ddi.ddi_thmprn_get_status();
            int result = convertStatus(state);
            Ddi.ddi_thmprn_close();
            return result;
        } catch (Exception e) {
            e.printStackTrace();
            return SdkResult.Fail;
        }
    }

    public int appendPrnStr(String text, FontSizeEnum fontsize, AlignEnum align, boolean isBoldFont) {
        try {
            PrinterData pd = new PrinterData();
            pd.setType(0);
            switch (fontsize) {
                case SMALL:
                    pd.setFontSize(24);
                    break;
                case MIDDLE:
                    pd.setFontSize(30);
                    break;
                case BIG:
                    pd.setFontSize(40);
                    break;
            }
            switch (align) {
                case LEFT:
                    pd.setAlignType(0);
                    break;
                case CENTER:
                    pd.setAlignType(2);
                    break;
                case RIGHT:
                    pd.setAlignType(1);
            }
            pd.setIsBoldFont(isBoldFont);
            pd.setText(text);
            pd.setLetterSpacing(letterSpacing);
            tmpPrinters.add(pd);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return SdkResult.Success;
    }

    public int appendImage(Bitmap bitmap, AlignEnum align) {
        try {
            if (bitmap == null) return SdkResult.Printer_Wrong_Package;
            if (bitmap.getWidth() > 384) {
                Matrix matrix = new Matrix();
                // 缩放原图
                matrix.postScale(384f / (float) bitmap.getWidth(), 384f / (float) bitmap.getWidth());
                bitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
            }
            int height = bitmap.getHeight();
            PrinterData pd = new PrinterData();
            pd.setType(1);
            pd.setBitmap(bitmap);
            switch (align) {
                case LEFT:
                    pd.setAlignType(0);
                    break;
                case CENTER:
                    pd.setAlignType(2);
                    break;
                case RIGHT:
                    pd.setAlignType(1);
            }
            tmpPrinters.add(pd);
            return SdkResult.Success;
        } catch (Exception e) {
            e.printStackTrace();
            return SdkResult.Fail;
        }
    }

    public void feedPaper(int value) {
        try {
            int result = SdkResult.Printer_Print_Fail;
            Ddi.ddi_thmprn_open();
            if (value >= 0) {
                result = Ddi.ddi_thmprn_feed_paper(value);
            }
            if (result == DdiConstant.DDI_OK) {
                waitForFinish();
            }
            Ddi.ddi_thmprn_close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void cutPaper() {
        int state;
        int result;
        Ddi.ddi_thmprn_open();
        state = Ddi.ddi_thmprn_feed_paper(80);
        if (state == DdiConstant.DDI_OK) {
            result = SdkResult.Success;
        } else {
            result = SdkResult.Printer_Print_Fail;
        }
        if (result == SdkResult.Success) {
            while (true) {
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                state = Ddi.ddi_thmprn_get_status();
                result = convertStatus(state);
                if (result == SdkResult.Printer_UnFinished) continue;
                break;
            }
            Ddi.ddi_thmprn_close();
        } else {
            Ddi.ddi_thmprn_close();
            Ddi.ddi_thmprn_open();
            state = Ddi.ddi_thmprn_get_status();
            Ddi.ddi_thmprn_close();
            result = convertStatus(state);
            if (result == SdkResult.Success) {
                //上面打印失败了,查状态又成功，说明上面的错误不是打印机的错识,有可能是入参有误等
                result = SdkResult.Printer_Print_Fail;
            }
        }
    }

    private int waitForFinish() {
        int result;
        while (true) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            result = Ddi.ddi_thmprn_get_status();
            result = convertStatus(result);
            if (result == SdkResult.Printer_UnFinished) continue;
            break;
        }
        return result;
    }

    private int convertStatus(int ddiState) {
        int result;
        switch (ddiState) {
            case 1:
                result = SdkResult.Success;
                break;
            case 2:
                result = SdkResult.Printer_UnFinished;
                break;
            case 3:
                result = SdkResult.Printer_PaperLack;
                break;
            case 4:
                result = SdkResult.Printre_TooHot;
                break;
            default:
                result = SdkResult.Printer_Print_Fail;
                break;
        }
        return result;
    }
}
