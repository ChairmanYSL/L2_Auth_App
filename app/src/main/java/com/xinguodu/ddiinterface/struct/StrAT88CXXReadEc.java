package com.xinguodu.ddiinterface.struct;

public class StrAT88CXXReadEc{

	private int i;
	private int m_type;
	private int m_mode;
	private int m_index;
	
	public int getType()
	{
		return m_type;
	}
	public int getMode()
	{
		return m_mode;
	}
	public int getIndex()
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
	public int setIndex(int Index)
	{
		m_index = Index;
		return 0;
	}
}
