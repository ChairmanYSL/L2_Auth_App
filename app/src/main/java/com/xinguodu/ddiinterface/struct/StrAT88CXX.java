package com.xinguodu.ddiinterface.struct;

public class StrAT88CXX{

	private int i;
	private int m_type;
	private int m_addr;
	private int m_le;
	
	public int getType()
	{
		return m_type;
	}
	public int getAddr()
	{
		return m_addr;
	}
	public int getLe()
	{
		return m_le;
	}
	public int setType(int Type)
	{
		m_type = Type;
		return 0;
	}
	public int setAddr(int Addr)
	{
		m_addr = Addr;
		return 0;
	}
	public int setLe(int Le)
	{
		m_le = Le;
		return 0;
	}
}