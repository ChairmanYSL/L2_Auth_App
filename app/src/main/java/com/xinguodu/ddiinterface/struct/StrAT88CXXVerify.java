package com.xinguodu.ddiinterface.struct;

public class StrAT88CXXVerify{

	private int i;
	private int m_type;
	private int m_mode;
	private byte[]  m_key =  new byte[3];
	private byte m_index;
	
	public int getType()
	{
		return m_type;
	}
	public int getMode()
	{
		return m_mode;
	}
	public byte[] getKey()
	{
		return m_key;
	}
	public byte getIndex()
	{
		return m_index;
	}
	public int setType(int Type)
	{
		m_type = Type;
		return 0;
	}
	public int setMode(int Mode)
	{
		m_mode = Mode;
		return 0;
	}
	public int setKey(byte[] Key)
	{
		for(i=0;i<3&&i<Key.length;i++)
		{
			m_key[i] = Key[i];
		}
		return 0;
	}
	public byte setIndex(byte Index)
	{
		m_index = Index;
		return 0;
	}
}
