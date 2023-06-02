//
// Created by mai on 2016/5/19.
//


#include <jni.h>
#include "sdktypedef.h"
#include "sdkDebug.h"


#ifndef NULL
#define NULL 0
#endif

JNIEnv* jniEnv;


jclass DeviceOpera = NULL;
jobject mDeviceOpera = NULL;

//java层表内数据操作方�?
jmethodID testCallJava = NULL;
jmethodID Lcd_Printf = NULL;
jmethodID Lcd_ClearLine = NULL;
jmethodID Lcd_PrintfXY = NULL;
jmethodID Kb_GetKey = NULL;
jmethodID wavBeeper = NULL;
jmethodID OpenSerial = NULL;
jmethodID SendSerial = NULL;
jmethodID ReadSerial = NULL;
jmethodID CloseSerial = NULL;
jmethodID openRF = NULL;
jmethodID ResetRF = NULL;
jmethodID CloseRF = NULL;
jmethodID TimerisEnd = NULL;
jmethodID TimerStart = NULL;
jmethodID Kb_Flush=NULL;
jmethodID OpenWifi = NULL;
jmethodID ReadWifi = NULL;
jmethodID SendWifi = NULL;
jmethodID CloseWifi = NULL;
jmethodID ListenTCP = NULL;
jmethodID CheckWifi = NULL;


JNIEXPORT jint JNICALL Java_com_nexgo_emv_JniNative_appmain(JNIEnv *env, jobject obj)
{
	jniEnv = env;
	main();
}

void freeDeviceObject();

int InitDeviceOpera()
{
	if(jniEnv == NULL)
	{
		return 0;
	}

	if(DeviceOpera == NULL) {
		jclass tmp = (*jniEnv)->FindClass(jniEnv,"com/nexgo/emv/DeviceOpera");
		if(tmp == NULL){
			return -1;
		}
        DeviceOpera = (*jniEnv)->NewGlobalRef(jniEnv, tmp);
        (*jniEnv)->DeleteLocalRef(jniEnv, tmp);
	}

    if (mDeviceOpera == NULL) {
        if (GetDeviceOperaInstance(DeviceOpera) != 1) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -1;
        }
    }

	if (Lcd_Printf == NULL) {
        Lcd_Printf = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "Lcd_Printf","(Ljava/lang/String;)V");
		if (Lcd_Printf == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

    if (Lcd_ClearLine == NULL) {
        Lcd_ClearLine = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "Lcd_ClearLine","(II)V");
        if (Lcd_ClearLine == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

    if (Lcd_PrintfXY == NULL) {
        Lcd_PrintfXY = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "Lcd_PrintfXY","(IILjava/lang/String;I)V");
        if (Lcd_PrintfXY == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
         }
    }

     if (Kb_GetKey == NULL) {
            Kb_GetKey = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "Kb_GetKey","()I");
            if (Kb_GetKey == NULL) {
                (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
                return -2;
             }
     }

    if(wavBeeper == NULL){
        wavBeeper = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "beeper","(I)V");
        if (wavBeeper == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

    if(OpenSerial == NULL){
        OpenSerial = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "OpenSerial","()I");
        if (OpenSerial == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

	if(SendSerial == NULL){
        SendSerial = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "SendSerial","([BI)I");
        if (SendSerial == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

	if(ReadSerial == NULL){
        ReadSerial = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "ReadSerial","(II)[B");
        if (ReadSerial == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

	if(CloseSerial == NULL){
	    CloseSerial = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "CloseSerial","(I)V");
	    if (CloseSerial == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(openRF == NULL){
	    openRF = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "openRF","()I");
	    if (openRF == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(ResetRF == NULL){
	    ResetRF = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "ResetRF","()I");
	    if (ResetRF == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(CloseRF == NULL){
	    CloseRF = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "CloseRF","()V");
	    if (CloseRF == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(TimerisEnd == NULL){
	    TimerisEnd = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "TimerisEnd","()Z");
	    if (TimerisEnd == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(TimerStart == NULL){
	    TimerStart = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "TimerStart","(I)V");
	    if (TimerStart == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(Kb_Flush == NULL){
	    Kb_Flush = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "Kb_Flush","()V");
	    if (Kb_Flush == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

//    if(OpenWifi == NULL){
//        OpenWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "OpenWifi","()V");
//        if (OpenWifi == NULL) {
//            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
//            return -2;
//        }
//    }
    if(OpenWifi == NULL){
        OpenWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "openTCP","(Ljava/lang/String;I)V");
        if (OpenWifi == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

//	if(SendWifi == NULL){
//        SendWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "SendWifiData","(Ljava/lang/String;I[B)V");
//        if (SendWifi == NULL) {
//            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
//            return -2;
//        }
//    }

	if(SendWifi == NULL){
        SendWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "sendTCP","([BI)V");
        if (SendWifi == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

//	if(ReadWifi == NULL){
//        ReadWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "ReadWifiData","([B)I");
//        if (ReadWifi == NULL) {
//            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
//            return -2;
//        }
//    }

	if(ReadWifi == NULL){
        ReadWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "readTCP","()[B");
        if (ReadWifi == NULL) {
            (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
            return -2;
        }
    }

//	if(CloseWifi == NULL){
//	    CloseWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "closeWifi","()V");
//	    if (CloseWifi == NULL) {
//	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
//	        return -2;
//	    }
//    }

	if(CloseWifi == NULL){
	    CloseWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "closeTCP","()V");
	    if (CloseWifi == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(ListenTCP == NULL){
	    ListenTCP = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "startListenTCP","(I)V");
	    if (ListenTCP == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	if(CheckWifi == NULL){
	    CheckWifi = (*jniEnv)->GetMethodID(jniEnv, DeviceOpera, "CheckWifiEnable","()I");
	    if (CheckWifi == NULL) {
	        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
	        return -2;
	    }
    }

	return 1;
}

int GetDeviceOperaInstance(jclass obj_class) {

	if(obj_class == NULL) {
		return 0;
	}

	jmethodID construction_id = (*jniEnv)->GetMethodID(jniEnv, obj_class, "<init>", "()V");
	if (construction_id == 0) {
		return -1;
	}

    mDeviceOpera = (*jniEnv)->NewObject(jniEnv, obj_class, construction_id);
	if ( mDeviceOpera == NULL) {
		return -2;
	}
	return 1;
}

int test(int i)
{
    int result = 1;
    jint j = 0;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
        j = i;
        result = (*jniEnv)->CallIntMethod(jniEnv, DeviceOpera, testCallJava, j);
        //Trace("CallJava", " i = %d", i);
    }

    freeDeviceObject();
    return result;
}

int dispTEST(char *dispData)
{
    int result = 1;
    jint j = 0;
    jstring jstr;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    //Trace("CallJava", " 1111 dispData = %s", dispData);
    if (result == 1) {

        jstr = (*jniEnv)->NewStringUTF(jniEnv, dispData);
        (*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, Lcd_Printf, jstr);
        //Trace("CallJava", " 222 dispData = %s", dispData);
    }

    freeDeviceObject();
    return result;
}

int clearLcdLine(int startLine, int endLine)
{
    int result = 1;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    //Trace("CallJava", " clearLcdLine = %d - %d", startLine, endLine);
    if (result == 1) {
        (*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, Lcd_ClearLine, startLine, endLine);
    }

    freeDeviceObject();
    return result;
}

int dispLcdLine(int line, int col, char* dispText, int attr)
{
    int result = 1;
    jstring jstr;


    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
        jstr = (*jniEnv)->NewStringUTF(jniEnv, dispText);
        (*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, Lcd_PrintfXY, line, col, jstr, attr);
        (*jniEnv)->DeleteLocalRef(jniEnv, jstr);
    }

    freeDeviceObject();
    return result;
}

int getKey()
{
    int result = 1;
	jint res;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
        result = 0;
        res = (*jniEnv)->CallIntMethod(jniEnv, mDeviceOpera, Kb_GetKey);
    }

    freeDeviceObject();
    return (int)res;
}

int getBeeper(int timer){
    int result = 1;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
        result = 0;
        (*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, wavBeeper,timer);
    }
    freeDeviceObject();
    return result;
}

int sdkIccOpenRfDev()
{
    jint result = 1;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
        result = (*jniEnv)->CallIntMethod(jniEnv, mDeviceOpera, openRF);
    }
    freeDeviceObject();
    return (int)result;

}

int sdkIccResetIcc()
{
    int result = 1;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
    	result= (*jniEnv)->CallIntMethod(jniEnv, mDeviceOpera, ResetRF);
    }
    freeDeviceObject();
    return result;
}

int sdkIccPowerDown()
{
    int result = 1;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
		result = 0;
        (*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, CloseRF);
    }
    freeDeviceObject();
    return result;
}

int sdkCommOpenUart()
{
    jint portId = -10;
	int result = 1;
    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }
    if (result == 1) {
        portId = (*jniEnv)->CallIntMethod(jniEnv, mDeviceOpera, OpenSerial);
    }
    freeDeviceObject();
    return (int)portId;
}

int sdkCommUartSendData(int ucCom, const unsigned char* pheSendData, int sendLen)
{
	int result = 1;
	jint res;
	jbyteArray byteArray;


    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }
    if (result == 1) {
        byteArray = (*jniEnv)->NewByteArray(jniEnv, (jsize)sendLen);
		(*jniEnv)->SetByteArrayRegion(jniEnv, byteArray, 0, (jsize)sendLen, pheSendData);
        res = (*jniEnv)->CallIntMethod(jniEnv, mDeviceOpera, SendSerial, byteArray, (jint)sendLen);
    }
    freeDeviceObject();

    return (int)res;
}

int sdkCommUartRecvData(int portId, unsigned char *pheRecvData, const int usRecvBufMaxLen, const int usRecvTimeOut)
{
	jint result = 1;
	jbyteArray byteArray;
	int len=0;
	jbyte *bytes;

    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }
    if (result == 1) {
        byteArray = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceOpera, ReadSerial, (jint)usRecvBufMaxLen, (jint)usRecvTimeOut);
		if(byteArray != NULL)
		{
			len = (*jniEnv)->GetArrayLength(jniEnv, byteArray);
			(*jniEnv)->GetByteArrayRegion(jniEnv,byteArray, 0,  len, (jbyte*)pheRecvData);
		}
		else
		{
			len = -1;
		}
	}
    freeDeviceObject();
//	TraceHex("ddi", "recv data from java:", pheRecvData, len);

    return (int)len;
}

int sdkCommCloseUart(int portId)
{
	int result = 1;
    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
		result = 0;
        (*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, CloseSerial, portId);
    }
    freeDeviceObject();

    return result;
}

bool sdkTimerIsEnd()
{
	int result = 1;
	jboolean flag;
    if(DeviceOpera == NULL || mDeviceOpera == NULL) {
        result = InitDeviceOpera();
    }

    if (result == 1) {
        flag = (*jniEnv)->CallBooleanMethod(jniEnv, mDeviceOpera, TimerisEnd);
    }
    freeDeviceObject();

	if(flag == JNI_TRUE)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void sdkTimerStar(unsigned int uiMs)
{
	int result = 1;
    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		(*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, TimerStart, uiMs);
	}
	freeDeviceObject();
}

void sdkOpenWifi(const char *IPAddress_cString, int port)
{
	int result = 1;
	jstring javaString;

    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		Trace("JNI", "IPAddress_cString: %s\r\n", IPAddress_cString);
		javaString = (*jniEnv)->NewStringUTF(jniEnv, IPAddress_cString);
		Trace("JNI", "after NewStringUTF pointer: %p\r\n", javaString);
		(*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, OpenWifi, javaString, (jint)port);
        (*jniEnv)->DeleteLocalRef(jniEnv, javaString);
	}
	freeDeviceObject();
}

void sdkSendWifiData(unsigned char *data, int dataLen)
{
	int result = 1;
	jbyteArray byteJava;

    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		byteJava = (*jniEnv)->NewByteArray(jniEnv, dataLen);
		(*jniEnv)->SetByteArrayRegion(jniEnv, byteJava, 0, dataLen, (jbyte *)data);
		(*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, SendWifi, byteJava, (jint)dataLen);
	}
	freeDeviceObject();
}

void sdkStartListenTCP(int port)
{
//	int result = 1;
//    if(DeviceOpera == NULL || mDeviceOpera == NULL)
//	{
//        result = InitDeviceOpera();
//    }
//	if(result == 1)
//	{
//		(*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, ListenTCP, (jint)port);
//	}
//	freeDeviceObject();
}

int sdkReadWifiData(unsigned char *data, int maxLen)
{
	int result = 1;
	jbyteArray dataJava;
	int len;

    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		dataJava = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceOpera, ReadWifi);
//		Trace("jni", "C receive data pointer = %p\r\n", dataJava);
		if(dataJava != NULL)
		{
			len = (*jniEnv)->GetArrayLength(jniEnv, dataJava);
			if(len > maxLen)
			{
				len = maxLen;
			}
			(*jniEnv)->GetByteArrayRegion(jniEnv,dataJava, 0,  len, (jbyte*)data);
			return len;
		}
		else
		{
			return -1;
		}
	}
	freeDeviceObject();
}

void sdkCloseWifi(void)
{
	int result = 1;
    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		(*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, CloseWifi);
	}
	freeDeviceObject();
}

void sdkKbKeyFlush(void)
{
	int result = 1;
    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		(*jniEnv)->CallVoidMethod(jniEnv, mDeviceOpera, Kb_Flush);
	}
	freeDeviceObject();
}

int sdkGetWifiEnable(void)
{
	int result = 1;
    if(DeviceOpera == NULL || mDeviceOpera == NULL)
	{
        result = InitDeviceOpera();
    }
	if(result == 1)
	{
		result = (*jniEnv)->CallIntMethod(jniEnv, mDeviceOpera, CheckWifi);
//		Trace("JNI", "sdkGetWifiEnable ret = %d\r\n", result);
		return result;
	}
	freeDeviceObject();
}

void freeDeviceObject(){
    //屏蔽是为了能在c调java可以打印
//    if(DeviceOpera != NULL)
//        (*jniEnv)->DeleteGlobalRef(jniEnv, DeviceOpera);
//    DeviceOpera = NULL;
//
//    //Trace("CallJava","freeDeviceObject 1111111111111111111111111111111");
//    if(mDeviceOpera != NULL)
//        (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceOpera);
//    mDeviceOpera = NULL;
//
//    testCallJava = NULL;

}


