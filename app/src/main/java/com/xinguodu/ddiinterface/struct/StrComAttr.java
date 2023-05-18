package com.xinguodu.ddiinterface.struct;

/**
 * Created by xiaox on 15/12/4.
 */
public class StrComAttr {
    private int m_baud;
    private int m_databits;
    private int m_parity;
    private int m_stopbits;

    public int getBaud() {
        return m_baud;
    }

    public int getDatabits() {
        return m_databits;
    }

    public int getParity() {
        return m_parity;
    }

    public int getStopbits() {
        return m_stopbits;
    }

    public int setBaud(int Baud) {
        m_baud = Baud;
        return 0;
    }

    public byte setDatabits(int Databits) {
        m_databits = Databits;
        return 0;
    }

    public byte setParity(int Parity) {
        m_parity = Parity;
        return 0;
    }

    public byte setStopbits(int Stopbits) {
        m_stopbits = Stopbits;
        return 0;
    }
}
