package com.xinguodu.ddiinterface.struct;

/**
 * Created by xiaox on 15/9/29.
 */
public class HSM_ObjectProperty {
    private int i;
    private byte[] m_CertName = new byte[10];
    private byte[] m_StrID = new byte[32];
    private byte[] m_StrLabel = new byte[32];
    byte m_nObjectType;
    byte m_nDataType;
    private byte[] m_nDataLength = new byte[4];


    public byte[] getCertName() {
        return m_CertName;
    }

    public byte[] getStrID() {
        return m_StrID;
    }

    public byte[] getStrLabel() {
        return m_StrLabel;
    }

    public byte getObjectType() {
        return m_nObjectType;
    }

    public byte getDataType() {
        return m_nDataType;
    }

    public byte[] getDataLength() {
        return m_nDataLength;
    }


    public int setCertName(byte[] CertName) {
        System.err.println("CertName.length: " + CertName.length + "\n");
        for (i = 0; i < 10 && i < CertName.length; i++) {
            m_CertName[i] = CertName[i];
        }
        return 0;
    }

    public int setStrID(byte[] StrID) {
        for (i = 0; i < 32 && i < StrID.length; i++) {
            m_StrID[i] = StrID[i];
        }
        return 0;
    }

    public int setStrLabel(byte[] StrLabel) {
        for (i = 0; i < 32 && i < StrLabel.length; i++) {
            m_StrLabel[i] = StrLabel[i];
        }
        return 0;
    }

    public int setObjectType(byte ObjectType) {
        m_nObjectType = ObjectType;
        return 0;
    }

    public int setDataType(byte DataType) {
        m_nDataType = DataType;
        return 0;
    }

    public int setDataLength(byte[] DataLength) {
        for (i = 0; i < 4 && i < DataLength.length; i++) {
            m_nDataLength[i] = DataLength[i];
        }
        return 0;
    }
}
