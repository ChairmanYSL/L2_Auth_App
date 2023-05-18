package com.xinguodu.ddiinterface.struct;

/**
 * Created by jinniu on 2015/9/2.
 */
public class StrDukptInitInfo {
	private int i;
	private byte m_groupindex;
	private byte m_keyindex;
	private byte[] m_initkey = new byte[24];
	private byte m_keylen;
	private byte m_ksnindex;
	private byte[] m_initksn = new byte[20];
	private byte m_ksnlen;

	public byte getGroupindex() {
		return m_groupindex;
	}

	public byte getKeyindex() {
		return m_keyindex;
	}

	public byte getKeylen() {
		return m_keylen;
	}

	public byte getKsnindex() {
		return m_ksnindex;
	}

	public byte getKsnlen() {
		return m_ksnlen;
	}

	public byte[] getInitkey() {
		return m_initkey;
	}

	public byte[] getInitksn() {
		return m_initksn;
	}

	public int setGroupindex(byte groupindex) {
		m_groupindex = groupindex;
		return 0;
	}

	public int setKeyindex(byte keyindex) {
		m_keyindex = keyindex;
		return 0;
	}

	public int setKeylen(byte keylen) {
		m_keylen = keylen;
		return 0;
	}

	public int setKsnindex(byte ksnindex) {
		m_ksnindex = ksnindex;
		return 0;
	}

	public int setKsnlen(byte ksnlen) {
		m_ksnlen = ksnlen;
		return 0;
	}

	public int setInitkey(byte[] initkey) {
		for (i = 0; i < 24; i++) {
			m_initkey[i] = initkey[i];
		}
		return 0;
	}

	public int setInitksn(byte[] initksn) {
		for (i = 0; i < 20; i++) {
			m_initksn[i] = initksn[i];
		}
		return 0;
	}
}
