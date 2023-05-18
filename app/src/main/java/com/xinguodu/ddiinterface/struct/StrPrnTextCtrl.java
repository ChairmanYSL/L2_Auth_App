package com.xinguodu.ddiinterface.struct;

/**
 * Created by jinniu on 2015/9/2.
 */
public class StrPrnTextCtrl {
	private int m_align;
	private int m_offset;
	private int m_font;
	private int m_ascsize;
	private int m_asczoom;
	private int m_nativesize;
	private int m_nativezoom;

	public int getAlign() {
		return m_align;
	}

	public int getOffset() {
		return m_offset;
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

	public int setAlign(int align) {
		m_align = align;
		return 0;
	}

	public int setOffset(int offset) {
		m_offset = offset;
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
}
