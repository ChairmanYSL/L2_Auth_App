package com.xinguodu.ddiinterface.struct;

/**
 * Created by jinniu on 2015/9/2.
 */
public class StrPrnCombTextCtrl {
	private int i;
	private int m_x0;
	private int m_y0;
	private int m_font;
	private int m_ascsize;
	private int m_asczoom;
	private int m_nativesize;
	private int m_nativezoom;
	private byte[] m_text = new byte[256];

	public int getX0() {
		return m_x0;
	}

	public int getY0() {
		return m_y0;
	}

	public int getFont() {
		return m_font;
	}

	public int getAscsize() {
		return m_ascsize;
	}

	public int getAsczoom() {
		return m_asczoom;
	}

	public int getNativesize() {
		return m_nativesize;
	}

	public int getNativezoom() {
		return m_nativezoom;
	}

	public byte[] getText() {
		return m_text;
	}

	public int setX0(int x0) {
		m_x0 = x0;
		return 0;
	}

	public int setY0(int y0) {
		m_y0 = y0;
		return 0;
	}

	public int setFont(int font) {
		m_font = font;
		return 0;
	}

	public int setAscsize(int ascsize) {
		m_ascsize = ascsize;
		return 0;
	}

	public int setAsczoom(int asczoom) {
		m_asczoom = asczoom;
		return 0;
	}

	public int setNativesize(int nativesize) {
		m_nativesize = nativesize;
		return 0;
	}

	public int setNativezoom(int nativezoom) {
		m_nativezoom = nativezoom;
		return 0;
	}

	public int setText(byte[] text) {
		for (i = 0; i < 256; i++) {
			m_text[i] = text[i];
		}
		return 0;
	}
}
