package com.xinguodu.ddiinterface.struct;

/**
 * Created by xiaox on 15/9/29.
 */
public class StrSle4428 {
    private int i;
    private int m_protect;
    private int m_addr;
    private int m_le;

    public int getProtect() {
        return m_protect;
    }

    public int getAddr() {
        return m_addr;
    }

    public int getLe() {
        return m_le;
    }

    public int setProtect(int Protect) {
        m_protect = Protect;
        return 0;
    }

    public int setAddr(int Addr) {
        m_addr = Addr;
        return 0;
    }

    public int setLe(int Le) {
        m_le = Le;
        return 0;
    }
}
