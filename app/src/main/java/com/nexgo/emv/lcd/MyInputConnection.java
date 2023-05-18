package com.nexgo.emv.lcd;

import android.view.View;
import android.view.inputmethod.BaseInputConnection;

public class MyInputConnection extends BaseInputConnection{
	private String inputStr = "";
	
	public MyInputConnection(View targetView, boolean fullEditor) {
		super(targetView, fullEditor);
		// TODO Auto-generated constructor stub
	}

	@Override
	public boolean commitText(CharSequence text, int newCursorPosition) {
		// TODO Auto-generated method stub
		//return super.commitText(text, newCursorPosition);
		inputStr += (String)text;
		return true;
	}

	public String getInputStr() {
		return inputStr;
	}

	public void setInputStr(String inputStr) {
		this.inputStr = inputStr;
	}

	
}
