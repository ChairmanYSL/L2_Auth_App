package com.nexgo.emv.print;

import android.graphics.Bitmap;

/**
 * 打印数据类
 * Created by jinniu on 2015/10/26.
 */
public class PrinterData implements Cloneable{
    private int type;   //打印内容类型 0：文本    1：图片
    private String text; //打印内容
    private int letterSpacing;
    private int fontSize;  //字体大小
    private boolean isBoldFont; //是否粗体
    private Bitmap bitmap;  //图片路径
    private int alignType;

    public PrinterData() {

    }

    public int getAlignType() {
        return alignType;
    }

    public void setAlignType(int alignType) {
        this.alignType = alignType;
    }

    public int getLetterSpacing() {
        return letterSpacing;
    }

    public void setLetterSpacing(int letterSpacing) {
        this.letterSpacing = letterSpacing;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public int getFontSize() {
        return fontSize;
    }

    public void setFontSize(int fontSize) {
        this.fontSize = fontSize;
    }

    public void setIsBoldFont(boolean isBoldFont) {
        this.isBoldFont = isBoldFont;
    }

    public boolean getIsBoldFont() {
        return isBoldFont;
    }

    public Bitmap getBitmap() {
        return bitmap;
    }

    public void setBitmap(Bitmap bitmap) {
        this.bitmap = bitmap;
    }
    @Override
    public Object clone() {
        PrinterData o = null;
        try{
            o = (PrinterData)super.clone();
        }catch(CloneNotSupportedException e) {
            e.printStackTrace();
        }
        return o;
    }
}
