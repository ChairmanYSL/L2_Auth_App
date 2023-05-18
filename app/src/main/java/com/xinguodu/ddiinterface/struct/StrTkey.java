package com.xinguodu.ddiinterface.struct;

public class StrTkey{
	private byte[]  m_indata =  new byte[512];
	private byte[]  m_outdata =  new byte[512];
	private int m_len;

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
