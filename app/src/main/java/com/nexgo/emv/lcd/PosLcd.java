package com.nexgo.emv.lcd;


import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.FontMetrics;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.widget.ImageView;

import com.nexgo.emv.R;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;


/**
 * add by zhoulinglong
 */
public class PosLcd extends View {

	public static final char CFONT = 1;
	public static final char ASII = 0;

	public static final int ALIGN_CENTER = 0x01;
	public static final int ALIGN_LEFT = 0x02;
	public static final int ALIGN_RIGHT = 0x03;

	public static final char linecap = 16;

	public Canvas m_canvas;
	private Bitmap m_bmp;
	private Paint m_paint;
	private int m_width;
	private int m_height;
	private int m_curLine;
	private int m_curCol;
	private Drawable m_drawableDrawable;
	private int m_margin;
	private int m_fontType = CFONT;
	private int m_fontSize = 40;
	private byte m_dispMode;		//正显还是反显 1-正显 0-反显
	private Handler m_handler;
	public ImageView imageView;

	
	@SuppressWarnings("deprecation")
	public PosLcd(Context context, int width, int height, Handler handler) {
		super(context);
		this.m_paint = new Paint();
		this.m_paint.setTypeface(Typeface.createFromAsset(context.getAssets(), "fonts/DroidSansFallback.ttf"));
		this.m_paint.setTextSize(m_fontSize);
		this.m_margin = m_fontSize;
		this.m_width = width;
		this.m_height = height;
		this.m_curLine = this.getTop() + this.m_margin;
		this.m_curCol = this.getLeft();
		this.m_bmp = Bitmap.createBitmap(this.m_width, this.m_height,
				Config.ARGB_8888);
		this.m_bmp.eraseColor(Color.WHITE);
		this.setVisibility(VISIBLE);
		this.setDrawingCacheEnabled(true);
		this.m_canvas = new Canvas(this.m_bmp);

		LayoutParams tmp_layoutParams;
		tmp_layoutParams = new LayoutParams(this.m_width, this.m_height);
		this.setLayoutParams(tmp_layoutParams);
		this.m_drawableDrawable = new BitmapDrawable(this.m_bmp);
		this.setBackgroundDrawable(m_drawableDrawable);
		this.m_handler = handler;

	}

	public void Lcd_Cls() {
		// add your to do here
		Message msg = Message.obtain();
		this.m_bmp.eraseColor(Color.WHITE);

		msg.what = 1;

		if (this.m_handler.sendMessage(msg))
		{
//			Log.i("Lcd_Cls handler msg", "success");
		}
		else
		{
//			Log.i("Lcd_Cls handler msg", "fail");
		}
	}

	public void Lcd_ClearLine(int startLine, int endLine) {
		this.m_paint.setColor(Color.WHITE);
		this.m_curLine = (int) (startLine * (this.m_paint.getTextSize() + linecap) + this.getTop() + this.m_margin);
		Lcd_DrawBox(0, m_curLine - m_fontSize, m_width, m_curLine + ((endLine - startLine + 1) * ((int) (this.m_paint.getTextSize() + linecap))) - m_fontSize);
	}

	public void Lcd_Printf(String dispText) throws UnsupportedEncodingException {
		if (m_dispMode == 0x01)
			this.m_paint.setColor(Color.BLACK);
		else
			this.m_paint.setColor(Color.WHITE);

		this.m_canvas.drawText(dispText, this.m_curCol, this.m_curLine,
					this.m_paint);

		Message msg = new Message();
		msg.what = 1;

		if (this.m_handler.sendMessage(msg))
		{
//			Log.i("Lcd_Printf handler msg", "success");
		}
		else
		{
//			Log.i("Lcd_Printf handler msg", "fail");
		}
	}

	public void Lcd_GotoXY(int line, int col) {
		this.m_curCol = (int) (col * this.m_paint.getTextSize() + 0.5)
				+ this.getLeft();
		this.m_curLine = (int) (line * this.m_paint.getTextSize() + this
				.getTop()) + this.m_margin + line * linecap;
	}

	public void Lcd_PrintfXY(int line, int col, String dispText) throws UnsupportedEncodingException {
		this.Lcd_GotoXY(line, col);
		this.Lcd_Printf(dispText);
	}

	public void Lcd_PrintfXY(int line, int col, String dispText, int attr) throws UnsupportedEncodingException {
		Rect rect = new Rect();
        int len = dispText.length();
		this.m_paint.getTextBounds(dispText,0,len,rect);
		m_dispMode = (byte)(attr & 0x01);
		if (m_dispMode == 0x01) {
			this.m_paint.setColor(Color.WHITE);
		}
		else {
			this.m_paint.setColor(Color.BLACK);
		}

		if (((byte)attr & 0x04) == 0x04){ //左对齐
			this.m_curCol = col;
		} else if (((byte)attr & 0x08) == 0x08){	//中间对齐
			this.m_curCol = col + ((this.m_width - col) - rect.width()) / 2;
		} else if (((byte)attr & 0x10) == 0x10){	//右对齐
			this.m_curCol = this.m_width - rect.width() - m_fontSize;
		}
		this.m_curLine = (int) (line * (this.m_paint.getTextSize() + linecap) + this.getTop() + this.m_margin);
		Lcd_DrawBox(m_curCol - 5, m_curLine - m_fontSize, m_curCol + rect.width() + 24, m_curLine + (int) (this.m_paint.getTextSize() + linecap) - m_fontSize);
		this.Lcd_Printf(dispText);
	}

	/**
	 * 该方法需要root权限
	 * @param datetime
	 */
	public void Lcd_SetTime(String datetime){
		try {
			Process process = Runtime.getRuntime().exec("su");
		//	String datetime = "20131023.112800"; // ���Ե����õ�ʱ�䡾ʱ���ʽ
													// yyyyMMdd.HHmmss��
			datetime = BytesToHexString(datetime.getBytes(), 6);
//			Log.i("", "datetime = " + datetime);
			datetime = "20" + datetime.substring(0, 6) + "." + datetime.substring(6, 12);
//			Log.i("", "datetime = " + datetime);

			DataOutputStream os = new DataOutputStream(process.getOutputStream());
			os.writeBytes("setprop persist.sys.timezone GMT\n");
			os.writeBytes("/system/bin/date -s " + datetime + "\n");
			os.writeBytes("clock -w\n");
			os.writeBytes("exit\n");
			os.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

    public static String BytesToHexString(byte[] bytes, int len)
    {
        char[] hexDigits = { '0', '1', '2', '3', '4', '5', '6', '7','8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        char[] chars = new char[len * 2];
        for (int i = 0; i < len; i++)
        {
            int b = bytes[i];
            chars[i * 2] = hexDigits[b >> 4];
            chars[i * 2 + 1] = hexDigits[b & 0xF];
        }
        return new String(chars);
    }

	public void Lcd_DrawBox(int left, int top, int right, int bottom) {
		this.m_paint.setStyle(Paint.Style.FILL);
//		this.m_paint.setStrokeWidth(this.m_margin);
		this.m_canvas.drawRect(left, top, right, bottom, this.m_paint);

		Message msg = Message.obtain();
		msg.what = 1;
		if (this.m_handler.sendMessage(msg))
		{
//			Log.i("Lcd_DrawBox send msg", "OK");
		}
		else
		{
//			Log.i("Lcd_DrawBox send msg", "Fail");
		}

		this.m_paint.setStyle(Paint.Style.FILL);
	}

	public void Lcd_SetFont(int FontType) {
		switch (FontType) {
		case CFONT:
			this.m_paint
					.setTextSize(m_fontSize);
			
			this.m_fontType = CFONT;
			break;
		case ASII:
			this.m_paint
					.setTextSize(m_fontSize - 8);
			this.m_fontType = ASII;
			break;
		default:
			this.m_paint
					.setTextSize(m_fontSize);
			this.m_fontType = ASII;
			break;
		}
	}

	public void Lcd_EraseOneWord() {
		Message msg = Message.obtain();

		msg.what = 1;
		this.m_handler.sendMessage(msg);
	}

	public void Lcd_EraseRect(int coord1X, int coord1Y, int coord2X, int coord2Y) {
		Message msg = Message.obtain();
		FontMetrics fm = this.m_paint.getFontMetrics();
		float fontHeigth = (float)Math.ceil(fm.descent - fm.ascent);

		int j = (coord1X - 2) * (int) (fontHeigth + linecap)
				+ this.getTop() + this.m_margin;
		int i = 0;

		for (; j < (coord2X-1) * (int) (fontHeigth + linecap)
				+ 2*(this.getTop() + this.m_margin); j++) {
			for (i = coord1Y; i < coord2Y; i++) {
				this.m_bmp.setPixel(i, j, Color.WHITE);
			}
		}

		msg.what = 1;
		this.m_handler.sendMessage(msg);
	}
	


	public int getM_width() {
		return m_width;
	}

	public void setM_width(int m_width) {
		this.m_width = m_width;
	}

	public int getM_height() {
		return m_height;
	}

	public void setM_height(int m_height) {
		this.m_height = m_height;
	}

	public int getM_curLine() {
		return m_curLine;
	}

	public void setM_curLine(int m_curLine) {
		this.m_curLine = m_curLine;
	}

	public int getM_curCol() {
		return m_curCol;
	}

	public void setM_curCol(int m_curCol) {
		this.m_curCol = m_curCol;
	}

	public int getM_margin() {
		return m_margin;
	}

	public void setM_margin(int m_margin) {
		this.m_margin = m_margin;
	}

	public static char getLinecap() {
		return linecap;
	}

	public Paint getM_paint() {
		return m_paint;
	}

	public void setM_paint(Paint m_paint) {
		this.m_paint = m_paint;
	}


	@Override
	public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
		// TODO Auto-generated method stub
		//return super.onCreateInputConnection(outAttrs);
		return new MyInputConnection(this, false);
	}
	
	

}
