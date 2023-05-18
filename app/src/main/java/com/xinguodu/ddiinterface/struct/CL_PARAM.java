package com.xinguodu.ddiinterface.struct;

/**
 * Created by xiaox on 15/9/29.
 */
public class CL_PARAM {
    private int i;
    private byte ModGsP;
    private byte RFCfg_A;
    private byte RFCfg_B;
    private byte RFOLevel;
    private byte RxTreshold_A;
    private byte RxTreshold_B;
    private byte[] RFU = new byte[25];
    private byte crc;

    public byte getModGsP() {
        return ModGsP;
    }

    public byte getRFCfg_A() {
        return RFCfg_A;
    }

    public byte getRFCfg_B() {
        return RFCfg_B;
    }

    public byte getRFOLevel() {
        return RFOLevel;
    }

    public byte getRxTreshold_A() {
        return RxTreshold_A;
    }

    public byte getRxTreshold_B() {
        return RxTreshold_B;
    }

    public byte[] getRFU() {
        return RFU;
    }

    public byte getcrc() {
        return crc;
    }

    public int setModGsP(byte m) {
        ModGsP = m;
        return 0;
    }

    public int setRFCfg_A(byte a) {
        RFCfg_A = a;
        return 0;
    }

    public int setRFCfg_B(byte b) {
        RFCfg_B = b;
        return 0;
    }

    public int setRFOLevel(byte l) {
        RFOLevel = l;
        return 0;
    }

    public int setRxTreshold_A(byte a) {
        RxTreshold_A = a;
        return 0;
    }

    public int setRxTreshold_B(byte b) {
        RxTreshold_B = b;
        return 0;
    }

    public int setRFU(byte[] r) {
        for (i = 0; i < 10 && i < r.length; i++) {
            RFU[i] = r[i];
        }
        return 0;
    }

    public int setcrc(byte c) {
        crc = c;
        return 0;
    }
}
