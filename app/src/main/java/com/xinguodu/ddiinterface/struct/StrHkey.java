package com.xinguodu.ddiinterface.struct;

public class StrHkey{
	private int m_area;
	private int m_index;
	private int m_half;
	private byte[]  m_indata =  new byte[512];
	private byte[]  m_outdata =  new byte[512];
	private int m_len;

	public int getArea() {
		return m_area;
	}

	public void setArea(int m_area) {
		this.m_area = m_area;
	}

	public int getIndex() {
		return m_index;
	}

	public void setIndex(int m_index) {
		this.m_index = m_index;
	}

	public int getHalf() {
		return m_half;
	}

	public void setHalf(int m_half) {
		this.m_half = m_half;
	}

	public byte[] getIndata() {
		return m_indata;
	}

	public void setIndata(byte[] m_indata) {
		this.m_indata = m_indata;
	}

	public byte[] getOutdata() {
		return m_outdata;
	}

	public void setOutdata(byte[] m_outdata) {
		this.m_outdata = m_outdata;
	}

	public int getLen() {
		return m_len;
	}

	public void setLen(int m_len) {
		this.m_len = m_len;
	}
}
