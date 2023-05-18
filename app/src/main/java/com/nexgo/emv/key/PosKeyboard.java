package com.nexgo.emv.key;

import android.content.Context;
import android.graphics.Paint;
import android.inputmethodservice.Keyboard;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.nexgo.emv.lcd.PosLcd;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

/**
 * add by zhoulingling
 */
public class PosKeyboard extends Keyboard {

	static public final char[][] keyCharTable = { { '1', 'q', 'z', '.', 'Q', 'Z' }, { '2', 'a', 'b', 'c', 'A', 'B', 'C' },
			{ '3', 'd', 'e', 'f', 'D', 'E', 'F' }, { '4', 'g', 'h', 'i', 'G', 'H', 'I' }, { '5', 'j', 'k', 'l', 'J', 'K', 'L' },
			{ '6', 'm', 'n', 'o', 'M', 'N', 'O' }, { '7', 'p', 'r', 's', 'P', 'R', 'S' }, { '8', 't', 'u', 'v', 'T', 'U', 'V' },
			{ '9', 'w', 'x', 'y', 'W', 'X', 'Y' }, { '0', ',', '*', '#', '@', '%', '$' }, };

	static public final char NOKEY = 0XFF;

	static public final char KEY1 = '1';
	static public final char KEY2 = '2';
	static public final char KEY3 = '3';
	static public final char KEY4 = '4';
	static public final char KEY5 = '5';
	static public final char KEY6 = '6';
	static public final char KEY7 = '7';
	static public final char KEY8 = '8';
	static public final char KEY9 = '9';
	static public final char KEY0 = '0';

	static public final char KEYENTER = 0x01;
	static public final char KEYCANCEL = 0x02;
	static public final char KEYCLEAR = 0x0A;
	static public final char KEYBCKSPACE = 0x11;
	static public final char KEYUP = 0x04;
	static public final char KEYDOWN = 0x05;
	static public final char KEYMENU = 0x22;
	static public final char KEYEXIT = 0x55;
	static public final char KEYALPHA = 0x0C;
	static public final char KEYFN = 0x0F;
//	static public final char KEYF1 = 0x05;
//	static public final char KEYF2 = 0x06;
//	static public final char KEYF3 = 0x07;
//	static public final char KEYF4 = 0x08;

	static public final char MAXKEYBUFF = 16;

	static public final char INPUTMODE_NUMERIC = 0x00;
	static public final char INPUTMODE_STRING = 0x01;
	static public final char INPUTMODE_PWD = 0x02;
	static public final char INPUTMODE_AMT = 0x03;

	private List<Integer> keyList;
	private Integer m_keyCode;
	private PosLcd lcd;
	private Context context;
	Handler handler;

	public PosKeyboard(Context context, Handler handler, int xmlLayoutResId, PosLcd m_lcd) {
		super(context, xmlLayoutResId);
		// TODO Auto-generated constructor stub
		m_keyCode = new Integer(0);
		keyList = new ArrayList<Integer>();
		lcd = m_lcd;
		this.context = context;
		this.handler = handler;
	}

	public void Kb_Flush() {
		keyList.clear();
	}

	public void KbStdin(int keyCode) {
		if (keyList.size() >= MAXKEYBUFF) {
			return;
		}

		m_keyCode = keyCode;
		keyList.add(m_keyCode);
	}

	public int Kb_Hit() {
		if (keyList.size() > 0) {
			return 1;
		} else {
			return 0;
		}
	}
	public void setKeyCode(int keyCode){
		Log.e("txq","setKeyCode:"+keyList.size());
		keyList.add(keyCode);
	}

	public int Kb_GetKey() {
		try {
			Thread.sleep(100);
		}catch (Exception e){}
		if(keyList.size()<=0){
			return 0;
		}
		int keyCode=keyList.get(0);
		keyList.remove(0);
		Log.e("txq","Kb_GetKey:"+keyCode);
		switch (keyCode){
		case 0x01:
			keyCode = KEYUP;
			break;
		case 0x02:
			keyCode = KEYDOWN;
			break;
		case 0x03:
			keyCode = KEYMENU;
			break;
		case 0x43:
			keyCode = KEYCLEAR;
			break;
		case 0x3E:
			keyCode = KEYENTER;
			break;
		case 0x3F:
			keyCode = KEYCANCEL;
			break;
		case 0x40:
			keyCode = KEYBCKSPACE;
			break;
		case 0x15:
			keyCode = KEYFN;
			break;
		}
		return keyCode;
		// Integer keyValue;

//		int value;
//		while (true) {
//			if (Kb_Hit() == 1) {
//				Log.e("1111111111111111","Kb_Hit: 1");
//				break;
//			}
//
//			try {
//				Thread.sleep(100);
//			} catch (InterruptedException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
//		}
//		Log.e("1111111111111111","Kb_Hit: 2");
//		m_keyCode = keyList.get(0);
//		Log.e("1111111111111111","Kb_Hit: 3");
//		keyList.remove(0);
//		Log.e("1111111111111111","Kb_Hit: 4");
//		value = m_keyCode.intValue();
//		Log.e("1111111111111111", "Kb_GetKey value = " + value);
//
//		if(value == 0x05){
//			Message msg = new Message();
//			msg.what = 2;
//			handler.sendMessage(msg);
//		}
//
//		switch (value){
//			case 0x01:
//				value = KEYUP;
//				break;
//			case 0x02:
//				value = KEYDOWN;
//				break;
//			case 0x03:
//				value = KEYMENU;
//				break;
//			case 0x43:
//				value = KEYCLEAR;
//				break;
//			case 0x3E:
//				value = KEYENTER;
//				break;
//			case 0x3F:
//				value = KEYCANCEL;
//				break;
//			case 0x40:
//				value = KEYBCKSPACE;
//				break;
//			case 0x15:
//				value = KEYFN;
//				break;
//		}
//
//		return value;
	}

	/**
	 * 暂时不使用该方法
	 * @param inputBuff
	 * @param minLen
	 * @param maxLen
	 * @param mode
	 * @param timeoutMs
	 * @return
	 */
	public int kb_GetStr(StringBuffer inputBuff, int minLen, int maxLen, int mode, int timeoutMs) {
//		Log.e("kb_GetStr ", "mimlen = " + minLen + " maxLen = " + maxLen + " mode = " + mode + " timeoutMs = " + timeoutMs);
		StringBuffer dispStr;
		StringBuffer inputStr;
		int keyCode;
		int keyCharTableRow = 0;
		int keycharTableCol = 0;
		boolean bNeedErace = false;
		int lm_curCol = lcd.getM_curCol();
		float lEraseLen = 0;
		Paint lm_Paint = lcd.getM_paint();
		PosTimer mytimer = new PosTimer(timeoutMs);

		if (inputBuff == null || minLen < 0 || maxLen < 0 || maxLen < minLen) {
			return -1;
		}

		dispStr = new StringBuffer();
		inputStr = new StringBuffer();

		if (mode == INPUTMODE_AMT) {
			dispStr.append("�� 0.00");
			try {
				lcd.Lcd_Printf(dispStr.toString());
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		mytimer.timerStart();

		while (true) {

			if (inputStr.length() >= 0 && inputStr.length() <= maxLen) {

				dispStr.delete(0, dispStr.length());

				if (mode == INPUTMODE_AMT) {
					if (inputStr.length() > 2) {
						dispStr.append("��");
						dispStr.append(inputStr.substring(0, inputStr.length() - 2));
						dispStr.append(".");
						dispStr.append(inputStr.substring(inputStr.length() - 2, inputStr.length()));
					} else {
						if (inputStr.length() == 1)
							dispStr.append("�� 0.0");
						else if (inputStr.length() == 2)
							dispStr.append("�� 0.");
						else {
							dispStr.append("�� 0.00");
						}

						dispStr.append(inputStr);
					}
				} else if (mode == INPUTMODE_PWD) {
					for (int i = 0; i < inputStr.length(); i++) {
						dispStr.append('*');
					}
				} else {
					dispStr.append(inputStr.toString());
					// Log.i("00000000000", "inputStr.toString() = " +
					// inputStr.toString());
				}

				if (bNeedErace || mode == INPUTMODE_AMT) {
					if (mode == INPUTMODE_AMT) {
						lEraseLen = lcd.getM_width() - lm_curCol;

					}
					bNeedErace = false;
				}

				try {
					lcd.Lcd_Printf(dispStr.toString());
				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			if (mytimer.checkTimerExpired()) {
				return -2;
			}

			if (Kb_Hit() != 0) {
				continue;
			}

			keyCode = Kb_GetKey();
//			Log.i("1111111111111", "Kb_GetKey() keyCode = " + keyCode);
//			Log.i("1111111111111", " inputBuff.append = " + inputStr.toString());
			System.out.println("maxLen =" + maxLen + ";inputStr.length()=" + inputStr.length());
			if (inputStr.length() >= maxLen && (keyCode > KEY0 && keyCode < KEY9)) {
				continue;
			}

			switch (keyCode) {
			case KEYENTER:
				if (inputStr.length() < minLen) {
					continue;
				}

				inputBuff.append(inputStr.toString());
//				Log.e("inputBuff.append ", " inputBuff.append = " + inputStr.toString());
				break;
			case KEY0:
				if (inputStr.length() >= maxLen || (mode == INPUTMODE_AMT && inputStr.length() == 0)) {
					continue;
				}
				inputStr.append('0');
				keyCharTableRow = 9;
				keycharTableCol = 0;
				continue;
			case KEY1:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('1');
				keyCharTableRow = 9;
				keycharTableCol = 0;
				continue;
			case KEY2:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('2');
				keyCharTableRow = 1;
				keycharTableCol = 0;
				continue;
			case KEY3:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('3');
				keyCharTableRow = 2;
				keycharTableCol = 0;
				continue;
			case KEY4:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('4');
				keyCharTableRow = 3;
				keycharTableCol = 0;
				continue;
			case KEY5:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('5');
				keyCharTableRow = 4;
				keycharTableCol = 0;
				continue;
			case KEY6:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('6');
				keyCharTableRow = 5;
				keycharTableCol = 0;
				continue;
			case KEY7:
				if (inputStr.length() > maxLen) {
					continue;
				}
				inputStr.append('7');
				keyCharTableRow = 6;
				keycharTableCol = 0;
				continue;
			case KEY8:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('8');
				keyCharTableRow = 7;
				keycharTableCol = 0;
				continue;
			case KEY9:
				if (inputStr.length() >= maxLen) {
					continue;
				}
				inputStr.append('9');
				keyCharTableRow = 8;
				keycharTableCol = 0;
				continue;
			case KEYCANCEL:

				return -3;
			case KEYBCKSPACE:
				if (inputStr.length() <= 0) {
					continue;
				}

				lEraseLen = (int) lm_Paint.measureText(inputStr.toString());
				inputStr.deleteCharAt(inputStr.length() - 1);
				bNeedErace = true;
				continue;
			case KEYCLEAR:
				if (inputStr.length() <= 0) {
					continue;
				}

				lEraseLen = (int) lm_Paint.measureText(inputStr.toString());
				inputStr.delete(0, inputStr.length());
				bNeedErace = true;

				continue;
			case KEYALPHA:

				if (inputStr.length() <= 0 || mode == INPUTMODE_NUMERIC || mode == INPUTMODE_AMT) {
					continue;
				}

				lEraseLen = (int) lm_Paint.measureText(inputStr.toString());
				bNeedErace = true;
				keycharTableCol++;

				switch (keyCharTableRow) {
				case 0:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 1:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 2:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
//					Log.i("inputStr", inputStr.toString());
					break;
				case 3:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 4:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 5:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 6:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 7:
					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 8:

					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				case 9:

					if (keycharTableCol >= keyCharTable[keyCharTableRow].length) {
						keycharTableCol = 0;
					}

					inputStr.deleteCharAt(inputStr.length() - 1);
					inputStr.append(keyCharTable[keyCharTableRow][keycharTableCol]);
					break;
				default:
					break;
				}
				continue;

			default:

				continue;
			}

//			System.out.println("inputStr=" + inputStr);
			break;
		}

		return 0;
	}

}
