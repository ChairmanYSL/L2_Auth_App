package com.nexgo.emv.key;

import android.inputmethodservice.Keyboard;

import java.util.Timer;
import java.util.TimerTask;

/**
 * add by zhoulinglong
 */
public class PosTimer {

	private Timer m_timer;
	private boolean m_expired;
	private long timeout;
	private TimerTask m_TimerTask;
	
	public PosTimer(long timoutMs)
	{
		m_timer = new Timer();
		m_expired = false;
		timeout = timoutMs;
	}
	
	public void timerStart()
	{
		m_TimerTask = new TimerTask() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				m_expired = true;
			}
		};
		
		m_timer.schedule(m_TimerTask, timeout);	
		
	}
	
	public boolean checkTimerExpired()
	{
		return m_expired;
	}


}
