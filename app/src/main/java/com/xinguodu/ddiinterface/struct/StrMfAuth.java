package com.xinguodu.ddiinterface.struct;

/**
 * Created by xiaox on 15/9/29.
 */
public class StrMfAuth {
    private int i;
    private byte m_authmode;
    private byte[] m_key = new byte[6];
    private byte[] m_uid = new byte[10];
    private byte m_block;

    public byte getAuthmode() {
        return m_authmode;
    }

    public byte[] getKey() {
        return m_key;
    }

    public byte[] getUid() {
        return m_uid;
    }

    public byte getBlock() {
        return m_block;
    }

    public int setAuthmode(byte Authmode) {
        m_authmode = Authmode;
        return 0;
    }

    public int setKey(byte[] Key) {
        for (i = 0; i < 6 && i < Key.length; i++) {
            m_key[i] = Key[i];
        }
        return 0;
    }

    public int setUid(byte[] Uid) {
        for (i = 0; i < 10 && i < Uid.length; i++) {
            m_uid[i] = Uid[i];
        }
        return 0;
    }

    public int setBlock(byte Block) {
        m_block = Block;
        return 0;
    }
}
