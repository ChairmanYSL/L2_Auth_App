#include "sdkoutcome.h"
#include "sdktypedef.h"
#include <string.h>
#include <stdlib.h>
#include "appglobal.h"

SDKOUTCOMEPARAM gstOutcome = {0};
SDKUIREQUESTPARAM gstUIRequest = {0};

void sdkCleanOutcomeParam()
{
	gstOutcome.Result = SDK_OUTCOME_RESULT_NA;
	gstOutcome.Start = SDK_OUTCOME_START_NA;
	gstOutcome.CVM = SDK_OUTCOME_CVM_NA;
	gstOutcome.UIRequestonOutcomePresent = 0;
	gstOutcome.UIRequestonRestartPresent = 0;
	gstOutcome.DataRecordPresent = 0;
	gstOutcome.DiscretionaryDataPresent = 0;
	gstOutcome.AlternateInterfacePreference = SDK_OUTCOME_AIP_NA;
	gstOutcome.Receipt = 0;
	gstOutcome.FieldOffRequest = SDK_OUTCOME_FIELDOFFREQ_NA;
	memset(gstOutcome.RemovalTimeout, 0, 2);
	gstOutcome.OnlineResponseData = SDK_OUTCOME_ONLINERESPDATA_NA;
}

void sdkCleanUIRequestParam()
{
	unsigned char tmpbuf[2] = {0};
	int len, ret;
	memset(&gstUIRequest, 0, sizeof(SDKUIREQUESTPARAM));
	ret = sdkEMVBaseReadTLV("\x5F\x2D", tmpbuf, &len);
	if(ret == SDK_OK)
	{
		if(len >= 8)
		{
			len = 8;
			memcpy(gstUIRequest.LanguagePerference, tmpbuf, len);
		}
		else
		{
			memcpy(gstUIRequest.LanguagePerference, tmpbuf, len);
			memset(&gstUIRequest.LanguagePerference[len], "\xFF", 8-len);
		}
	}
	ret = sdkEMVBaseReadTLV("\x5F\x2A", tmpbuf, &len);
	if(ret == SDK_OK)
	{
		memcpy(gstUIRequest.CurrencyCode, tmpbuf, 2);
	}
	else
	{
		ret = sdkEMVBaseReadTLV("\xCD", tmpbuf, &len);
		if(ret == SDK_OK)
		{
			memcpy(gstUIRequest.CurrencyCode, tmpbuf, 2);
		}
	}
}

s32 sdkSetOutcomeParam(u8 Result, u8 Start, u8 CVM, u8 UIRequestonOutcomePresent, u8 UIRequestonRestartPresent, u8 DataRecordPresent, u8 DiscretionaryDataPresent, u8 AlternateInterfacePreference, u8 Receipt, u8 FieldOffRequest, u8 *RemovalTimeout, u8 OnlineResponseData)
{
	sdkCleanOutcomeParam();
	gstOutcome.Result = Result;
	gstOutcome.Start = Start;
	gstOutcome.CVM = CVM;
	gstOutcome.UIRequestonOutcomePresent = UIRequestonOutcomePresent;
	gstOutcome.UIRequestonRestartPresent = UIRequestonRestartPresent;
	gstOutcome.DataRecordPresent = DataRecordPresent;
	gstOutcome.DiscretionaryDataPresent = DiscretionaryDataPresent;
	gstOutcome.AlternateInterfacePreference = AlternateInterfacePreference;
	gstOutcome.Receipt = Receipt;
	gstOutcome.FieldOffRequest = FieldOffRequest;
	if(RemovalTimeout)
	{
		memcpy(gstOutcome.RemovalTimeout, RemovalTimeout, 2);
	}
	gstOutcome.OnlineResponseData = OnlineResponseData;

	return SDK_OK;
}

int sdkSendOutcome()
{
	unsigned char SendCommData[1024]={0};
	unsigned char SendDataLen = 0;
	int i,ret,len;
	unsigned char Flag = 0x00;
	unsigned char Tag[3],Data[256];


	SendCommData[0] = 0x02;
	SendCommData[1] = BCTC_MNG_TransResult_SEND;

	i =	4;
	memcpy(SendCommData+i, "\xDF\x81\x29\x09", 4);
	i += 4;
	SendCommData[i++] = gstOutcome.Result;
	SendCommData[i++] = gstOutcome.Start;
	SendCommData[i++] = gstOutcome.OnlineResponseData;
	SendCommData[i++] = gstOutcome.CVM;

	if(gstOutcome.UIRequestonOutcomePresent)
	{
		Flag |= 0x80;
	}
	if(gstOutcome.UIRequestonRestartPresent)
	{
		Flag |= 0x40;
	}
	if(gstOutcome.DataRecordPresent)
	{
		Flag |= 0x20;
	}
	if(gstOutcome.DiscretionaryDataPresent)
	{
		Flag |= 0x10;
	}
	if(gstOutcome.Receipt)
	{
		Flag |= 0x08;
	}

	SendCommData[i++] = Flag;
	SendCommData[i++] = gstOutcome.AlternateInterfacePreference;
	SendCommData[i++] = gstOutcome.FieldOffRequest;
	memcpy(SendCommData+i, gstOutcome.RemovalTimeout, 2);
	i += 2;

	if(gstOutcome.DataRecordPresent)	//pack data record
	{
		memcpy(SendCommData+i, "\xFF\x81\x05", 3);
		i += 3;
		sdkEMVBaseReadTLV("\x9F\x02", Data, &len);	//FIXME:just for temp test
		memcpy(SendCommData+i, "\x9F\x02\x06", 3);
		i += 3;
		memcpy(SendCommData+i, Data, len);
		i += len;
	}

	SendDataLen = i - 4;
	SendCommData[2] = (SendDataLen >> 8)&0xFF;	//high byte
	SendCommData[3] = SendDataLen&0xFF;	//low byte

	ret = sdkCommUartSendData(gSerialPortId, SendCommData, i);
	Trace("Send Outcome", "sdkCommUartSendData ret = %d\r\n", ret);

	return ret;
}

s32 sdkSetUIRequestParam(u8 MessageID, u8 Status, u8 HoldTime, u8 *LanguagePerference, u8 ValueQualifier, u8 *Value, u8 *CurrencyCode)
{
	sdkCleanUIRequestParam();
	gstUIRequest.MessageID = MessageID;
	gstUIRequest.Status = Status;
	gstUIRequest.HoldTime = HoldTime;
	if(LanguagePerference)
	{
		memcpy(gstUIRequest.LanguagePerference, LanguagePerference, 8);
	}
	gstUIRequest.ValueQualifier = ValueQualifier;
	if(Value)
	{
		memcpy(gstUIRequest.Value, Value, 6);
	}
	if(CurrencyCode)
	{
		memcpy(gstUIRequest.CurrencyCode, CurrencyCode, 2);
	}

	return SDK_OK;
}

int sdkSendUIRequest(int type)
{
	unsigned char SendCommData[1024]={0};
	unsigned char SendDataLen = 0;
	int i,ret,len;
	unsigned char Flag = 0x00;
	unsigned char Tag[3],Data[256];

	SendCommData[0] = 0x02;
	SendCommData[1] = BCTC_MNG_TermDispUI_SEND;
	i = 4;

	if(SDK_UIREQ_OUTCOME == type)
	{
		memcpy(SendCommData+i, "\xDF\x81\x16\x16", 4);
	}
	else if(SDK_UIREQ_OUTCOME == type)
	{
		memcpy(SendCommData+i, "\xDF\x81\x17\x16", 4);
	}
	else
	{
		return SDK_ERR;
	}

	i += 4;
	SendCommData[i++] = gstUIRequest.MessageID;
	SendCommData[i++] = gstUIRequest.Status;
	i += 2;
	SendCommData[i++] = gstUIRequest.HoldTime;
	memcpy(SendCommData+i, gstUIRequest.LanguagePerference, 8);
	i += 8;
	if(SDK_UI_VALUEQUALIFIER_BALANCE == gstUIRequest.ValueQualifier)
	{
		SendCommData[i++] = gstUIRequest.ValueQualifier;
		ret = sdkEMVBaseReadTLV("\x9F\x50", Data, &len);
		if(SDK_OK == ret)
		{
			memcpy(SendCommData+i, Data, len);
		}
		else
		{
			memcpy(SendCommData+i, "\x00\x00\x00\x00\x00\x00", 6);
		}
	}
	memcpy(SendCommData+i, gstUIRequest.CurrencyCode, 2);
	i += 2;

	SendDataLen = i - 4;
	SendCommData[2] = (SendDataLen >> 8)&0xFF;	//high byte
	SendCommData[3] = SendDataLen&0xFF;	//low byte

	ret = sdkCommUartSendData(gSerialPortId, SendCommData, i);
	Trace("Send UI Request", "sdkCommUartSendData ret = %d\r\n", ret);
	return ret;
}

