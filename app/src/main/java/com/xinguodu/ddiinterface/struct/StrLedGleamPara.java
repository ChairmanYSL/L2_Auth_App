package com.xinguodu.ddiinterface.struct;

public class StrLedGleamPara{
	private int m_led;
	private int m_ontime;
	private int m_offtime;
	private int m_duration;

	public int getLed()
	{
		return m_led;
	}
	public int getOntime()
	{
		return m_ontime;
	}
	public int getOfftime()
	{
		return m_offtime;
	}
	public int getDuration()
	{
		return m_duration;
	}

	public int setLed(int Led)
	{
		m_led = Led;
		return 0;
	}
	public byte setOntime(int Ontime)
	{
		m_ontime = Ontime;
		return 0;
	}
	public byte setOfftime(int Offtime)
	{
		m_offtime = Offtime;
		return 0;
	}
	public byte setDuration(int Duration)
	{
		m_duration = Duration;
		return 0;
	}
}