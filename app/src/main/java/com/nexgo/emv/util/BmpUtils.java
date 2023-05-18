package com.nexgo.emv.util;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.os.Environment;

import com.nexgo.emv.print.PrinterData;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * bmp文件工具类
 */
public class BmpUtils {
    private static final String TAG = "xgdBmpUtils";
    public static Typeface typeface;

    public boolean isFileExist(String fileName) {
        File file = new File(Environment.getExternalStorageDirectory()
                + File.separator + fileName);
        return file.exists();
    }

    /**
     * create multiple director
     *
     * @param director 文件夹名称
     * @return true or false
     */
    public boolean createDir(String director) {
        if (isFileExist(director)) {
            return true;
        } else {
            File file = new File(Environment.getExternalStorageDirectory()
                    + File.separator + director);
            if (!file.exists()) {
                file.mkdirs();
                return false;
            }
            return true;
        }
    }


    public void deleteFile(File file) {
        if (file.exists()) {
            if (file.isFile()) {
                file.delete();
            } else if (file.isDirectory()) {
                File files[] = file.listFiles();
                for (int i = 0; i < files.length; i++) {
                    this.deleteFile(files[i]);
                }
            }
            file.delete();
        } else {

        }
    }

    public void setTypeface(Typeface typeface) {
        this.typeface = typeface;
    }

    /**
     * 字符串集合转换为bitmap,然后再转换为bmp文件
     *
     * @param pList 字符串集合
     * @return bmp文件
     */
    public Bitmap toBitmap(Context context,List<PrinterData> pList) {
//        if (pList.size() == 1) {
//            if (pList.get(0).getType() == 1) {
//                return pList.get(0).getBitmap();
//            }
//        }

        int x = 0, y = 0;
        final int width = 384;
        Bitmap bitmap = null;


        Paint paint = new Paint();
        if (typeface != null) {
            paint.setTypeface(typeface);
        } else {
//            paint.setTypeface(Typeface.create("宋体", Typeface.NORMAL));
            typeface = Typeface.createFromAsset(context.getAssets(), "fonts/DroidSansFallback.ttf");
            paint.setTypeface(typeface);
        }
        paint.setLinearText(true);

        int height = 0;
        List<PrinterData> tmpList = new ArrayList<>();
        PrinterData tmpPd;
        String text = "";
        int offset = 0;
        char[] chars;
        int cnt = 0;
        int i, len;

        //解决第一行老是少打字符的bug
        PrinterData tpd = new PrinterData();
        tpd.setType(0);
        tpd.setFontSize(24);
        tpd.setAlignType(0);
        tpd.setIsBoldFont(false);
        tpd.setText(text);
        tpd.setLetterSpacing(4);
        pList.add(0,tpd);
        for (PrinterData pd : pList) {
            offset = 0;
            cnt = 0;
            if (pd.getType() == 0) { //字符
                paint.setTextSize((float) pd.getFontSize());
                paint.setFakeBoldText(pd.getIsBoldFont());
                height += getTextHeigh(paint) + pd.getLetterSpacing();
                chars = pd.getText().toCharArray();
                for (i = 0; i < chars.length; i++) {
                    len = (int) paint.measureText(chars, i, 1);
                    cnt += len;
                    if (cnt > width) {
                        height += getTextHeigh(paint) + pd.getLetterSpacing();
                        text = pd.getText().substring(offset, i);
                        offset = i;
                        cnt = len;
                        tmpPd = (PrinterData) pd.clone();
                        tmpPd.setText(text);
                        tmpList.add(tmpPd);
                    }
                }
                text = pd.getText().substring(offset, i);
                tmpPd = (PrinterData) pd.clone();
                tmpPd.setText(text);
                tmpList.add(tmpPd);
            } else if (pd.getType() == 1) { //图片
                tmpList.add(pd);
                height += pd.getBitmap().getHeight() + pd.getLetterSpacing();
            }
        }
//        log.debug("画布总高度为" + height);
        try {
            bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
            Canvas canvas = new Canvas(bitmap);
            canvas.drawColor(Color.WHITE);
            Paint.FontMetrics fontMetrics = paint.getFontMetrics();
            for (PrinterData pd : tmpList) {
                if (pd.getType() == 0) { //字符
                    paint.setTextSize((float) pd.getFontSize());
                    paint.setFakeBoldText(pd.getIsBoldFont());
                    text = pd.getText();
                    int baseline = (int) (y + getTextHeigh(paint) - (fontMetrics.bottom - fontMetrics.top) / 2);

                    switch (pd.getAlignType()) {
                        case 0:
                            x = 0;
                            break;
                        case 1:
                            x = width - getTextWidth(paint, text);
                            break;
                        case 2:
                            x = (width - getTextWidth(paint, text)) / 2;
                            break;
                    }
                    canvas.drawText(text, x, baseline, paint);
                    y += getTextHeigh(paint);
                    y += pd.getLetterSpacing();
                } else if (pd.getType() == 1) { //图片
                    switch (pd.getAlignType()) {
                        case 0:
                            x = 0;
                            break;
                        case 1:
                            x = width - pd.getBitmap().getWidth();
                            break;
                        case 2:
                            x = (width - pd.getBitmap().getWidth()) / 2;
                            break;
                    }
                    canvas.drawBitmap(pd.getBitmap(), x, y, null);
                    y += pd.getBitmap().getHeight() + pd.getLetterSpacing();
                }
            }
            canvas.save(Canvas.ALL_SAVE_FLAG);
            canvas.restore();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return bitmap;
    }

    private static int getTextHeigh(Paint paint) {
        Paint.FontMetrics fontMetrics = paint.getFontMetrics();
        return (int) Math.ceil(fontMetrics.descent - fontMetrics.ascent);
//        return (int) Math.ceil(fontMetrics.descent - fontMetrics.top) + 2;
    }

    public static int getTextWidth(Paint paint, String str) {
        int iRet = 0;
        if (str != null && str.length() > 0) {
            int len = str.length();
            float[] widths = new float[len];
            paint.getTextWidths(str, widths);
            for (int j = 0; j < len; j++) {
                iRet += (int) Math.ceil(widths[j]);
            }
        }
        return iRet;
    }

//    public File toFile(List<PrinterData> pList, String directory, String fileName) {
//        Bitmap bitmap = toBitmap(pList);
//        if (bitmap == null) {
//            return null;
//        }
//        File file = writeToSDCardFromBitmap(bitmap, directory, fileName);
//        return file;
//    }

    /**
     * 多位bitmap转换为单位bmp文件
     *
     * @param bitmap    需要转换的bitmap
     * @param directory 转换后存放的文件夹名称
     * @param fileName  转换后的bmp文件名称
     * @return 返回file文件
     */
    public File writeToSDCardFromBitmap(Bitmap bitmap, String directory,
                                        String fileName) {

        if (bitmap == null)
            return null;
//        log.error("writeToSDCardFromBitmap");
        createDir(directory);
        FileOutputStream fileos = null;
        try {
            String filename = Environment.getExternalStorageDirectory()
                    + File.separator + directory + File.separator + fileName;
            File file = new File(filename);
            if (file.exists()) {
                deleteFile(file);
            }
            file.createNewFile();

            fileos = new FileOutputStream(filename);
            BmpUtils bmpUtils = new BmpUtils();
            final byte[] data = bmpUtils.toBmpBytes(bitmap);
            if (!bitmap.isRecycled()) bitmap.recycle();
            fileos.write(data);
            fileos.flush();
            fileos.close();
            return file;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        } finally {
            try {
                if (fileos != null) {
                    fileos.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public byte[] toBmpBytes(Context context,List<PrinterData> pList) {
        Bitmap bitmap = toBitmap(context,pList);
        byte[] data = toBmpBytes(bitmap);
        if (!bitmap.isRecycled()) bitmap.recycle();
        return data;
    }

    /**
     * 多位bitmap转换为单位bmp文件
     *
     * @param bitmap 需要转换的bitmap
     * @return 返回file文件
     */
    public byte[] toBmpBytes(Bitmap bitmap) {

        if (bitmap == null)
            return null;

        int nBmpWidth = bitmap.getWidth();
        int nBmpHeight = bitmap.getHeight();
        int nFixBmpHeight = ((nBmpHeight + 7) >> 3) << 3;
        int biBitCount = 1;
        int wWidth = ((((nBmpWidth * biBitCount) + 31) & ~31) >> 3);
        int bufferSize = wWidth * nFixBmpHeight;
        try {
            //bmp文件头
            int bfType = 0x4d42;
            long bfOffBits = 14 + 40 + 8;
            long bfSize = bfOffBits + bufferSize;
            int bfReserved1 = 0;
            int bfReserved2 = 0;
            ByteBuffer byteBuffer = ByteBuffer.allocate((int) bfSize);
            // 保存bmp文件头
            writeWord(byteBuffer, bfType);
            writeDword(byteBuffer, bfSize);
            writeWord(byteBuffer, bfReserved1);
            writeWord(byteBuffer, bfReserved2);
            writeDword(byteBuffer, bfOffBits);

            //bmp信息头
            long biSize = 40L;
            long biWidth = nBmpWidth;
            long biHeight = nFixBmpHeight;
            int biPlanes = 1;
//            int biBitCount = 1;
            long biCompression = 0L;
            long biSizeImage = bufferSize;
            long biXpelsPerMeter = 0L;
            long biYPelsPerMeter = 0L;
            long biClrUsed = 0L;
            long biClrImportant = 0L;
            // 保存bmp信息头
            writeDword(byteBuffer, biSize);
            writeLong(byteBuffer, biWidth);
            writeLong(byteBuffer, biHeight);
            writeWord(byteBuffer, biPlanes);
            writeWord(byteBuffer, biBitCount);
            writeDword(byteBuffer, biCompression);
            writeDword(byteBuffer, biSizeImage);
            writeLong(byteBuffer, biXpelsPerMeter);
            writeLong(byteBuffer, biYPelsPerMeter);
            writeDword(byteBuffer, biClrUsed);
            writeDword(byteBuffer, biClrImportant);
            writeLong(byteBuffer, 0xff000000L);
            writeLong(byteBuffer, 0xffffffffL);

            byte[] fix = new byte[(nFixBmpHeight - nBmpHeight) * wWidth];
            Arrays.fill(fix, (byte) 0xff);
            byteBuffer.put(fix);
            byte data[] = byteBuffer.array();
            int position = byteBuffer.position();
            int binary;
            int nCol, nRealCol;
            int wRow;
            int clr;
            int tmp;
            for (nCol = 0, nRealCol = nBmpHeight - 1; nCol < nBmpHeight; ++nCol, --nRealCol) {
                binary = 0;
                for (wRow = 0; wRow < (wWidth << 3); wRow++) {
                    if (wRow < nBmpWidth) {
                        clr = bitmap.getPixel(wRow, nCol);
                        tmp = (Color.red(clr) + Color.green(clr) + Color.blue(clr)) / 3 > 156 ? 1 : 0;
//                        tmp = (Color.green(clr) > 156 ? 1 : 0);
                        binary <<= 1;
                        binary |= tmp;
                    } else {
                        binary <<= 1;
                    }

                    if ((wRow + 1) % 8 == 0) {
                        data[position + wWidth * nRealCol + (((wRow + 1) >> 3) - 1)] = (byte) binary;
                        binary = 0;
                    }
                }
            }
            return data;

        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    private static void writeWord(ByteBuffer buffer, int value) throws IOException {
        buffer.put((byte) (value & 0xff));
        buffer.put((byte) (value >> 8 & 0xff));
    }

    private static void writeDword(ByteBuffer buffer, long value) throws IOException {
        buffer.put((byte) (value & 0xff));
        buffer.put((byte) (value >> 8 & 0xff));
        buffer.put((byte) (value >> 16 & 0xff));
        buffer.put((byte) (value >> 24 & 0xff));
    }

    private static void writeLong(ByteBuffer buffer, long value) throws IOException {
        buffer.put((byte) (value & 0xff));
        buffer.put((byte) (value >> 8 & 0xff));
        buffer.put((byte) (value >> 16 & 0xff));
        buffer.put((byte) (value >> 24 & 0xff));
    }
}
