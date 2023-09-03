#include "sdkoutcome.h"
#include "sdktypedef.h"
#include <string.h>
//#include <stdlib.h>


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
		memcpy(gstUIRequest.LanguagePerference, tmpbuf, 2);
	}
	else
	{
		memcpy(gstUIRequest.LanguagePerference, "\x09\x78", 2);
	}
	ret = sdkEMVBaseReadTLV("\x5F\x2A", tmpbuf, &len);
	if(ret == SDK_OK)
	{
		memcpy(gstUIRequest.CurrencyCode, tmpbuf, 2);
	}
	else
	{
		memcpy(gstUIRequest.CurrencyCode, "\x09\x78", 2);
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
	gstOutcome.DiscretionaryDataPresent = 0;
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

}

s32 sdkSetUIRequestParam(u8 MessageID, u8 Status, u8 HoldTime, u8 *LanguagePerference, u8 ValueQualifier, u8 *Value, u8 *CurrencyCode)
{
	sdkCleanUIRequestParam();
	gstUIRequest.MessageID = MessageID;
	gstUIRequest.Status = Status;
	gstUIRequest.HoldTime = HoldTime;
	if(LanguagePerference)
	{
		memcpy(gstUIRequest.LanguagePerference, LanguagePerference, 2);
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


}

