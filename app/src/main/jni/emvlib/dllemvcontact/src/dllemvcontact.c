#include "dllemvbase.h"
#include "dllemvcontact.h"
#include "dllemvcontactprivate.h"

/*****************************************************************************
** Descriptions:  »ñÈ¡ÄÚºË°æ±¾
** Parameters:

** Returned value:  RLT_EMV_OK
					RLT_EMV_ERR;
** Created By:sujianzhong
** Remarks:
*****************************************************************************/
unsigned char EmvContact_GetKernelVerInfo(int machine_code, unsigned char *ver)
{
	unsigned char ret = RLT_EMV_ERR;

	if(NULL == ver) return ret;

	switch(machine_code)
	{
		case 0xAD://G2+
		case 0x99://G2
		case 0x35://G2M
		case 0x36://G2N
		case 0x1F://G2Q
		case 0xBF://G25
			strcpy(ver, "XGD-EMVL2 Version V4.3f");
			ret = RLT_EMV_OK;
			break;


		case 0x26://K300
		case 0x28://K300+
		case 0x29://K300+ American
			strcpy(ver, "XGD-EMVL2 Version V4.3f");
			ret = RLT_EMV_OK;
			break;


        case 0x03://N82
            strcpy(ver, "XGD-EMVL2 Version V4.3f");
			ret = RLT_EMV_OK;
			break;

        case 0x01://N86
        case 0x02://N6
            strcpy(ver, "XGD-EMVCT Version V4.3");
			ret = RLT_EMV_OK;
			break;

		default:

			break;
	}

	return ret;
}



static int GetCompileTime(char *pasCompileTime, const char *pasDate, const char *pasTime)
{
	char temp_date[64] = {0},str_year[5] = {0}, str_month[4] = {0}, str_day[3] = {0};
	char temp_time[64] = {0},str_hour[2] = {0}, str_min[2] = {0}, str_sec[2] = {0};
	char en_month[12][4]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char tempCompileTime[20] = {0};
	int i = 0;

	if(pasCompileTime == NULL || pasDate == NULL || pasTime == NULL)
	{
		return 0;
	}

	sprintf(temp_date,"%s",pasDate);    //"Sep 7 2012"
	sscanf(temp_date,"%s %s %s",str_month, str_day, str_year);

	//sprintf(temp_time, "%s", pasTime);
	//sscanf(temp_time, "%s:%s:%s", str_hour, str_min, str_sec);


	for(i=0; i < 12; i++)
	{
		if(strncmp(str_month,en_month[i],3)==0)
		{
		    memset(str_month, 0, sizeof(str_month));
	  		sprintf(str_month, "%02d", i + 1);
	        break;
	    }
	}

	if(strlen(str_day)==1)//ÈôÈÕÆÚÎª1Î»ÐèÒªÇ°²¹0x30
	{
		str_day[1]=str_day[0];
		str_day[0]=0x30;
	}



	sprintf(pasCompileTime, "%s%s%s ", str_year, str_month, str_day);
	//sprintf(tempCompileTime, " %s%s%s", str_hour, str_min, str_sec);
	strcat(pasCompileTime, pasTime);

	return 0;
}



void EmvContact_GetLibVersion(unsigned char *version)
{
	GetCompileTime(version, __DATE__, __TIME__);//Èç2020Äê01ÔÂ21ÈÕ±àÒë£¬Êä³översionÎª:200121
	//strcat(version,"001");
}

unsigned char EmvContact_ReadSelectRetData(EMVBASE_SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, unsigned char *Rfu, unsigned char *RfuLen)
{
    unsigned char j, k;
    unsigned short index, indexFCI, indexFCIProp, len, lenFCI, lenFCIProp;
    int indexIssuerDiscret, lenIssuerDiscret;
	unsigned short indexttemp = 0, tagLENLen = 0, tagLen = 0; // for USA GP  2021.09.18


    index = 0;
	*RfuLen = 0;

    if(DataOut[index] != 0x6F)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    index++;

    if(EMVBase_ParseExtLen(DataOut, &index, &lenFCI))
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    indexFCI = index;

    while(index < indexFCI + lenFCI)
    {
        if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }

        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
        else if(DataOut[index] == 0x84)
        {
            if(selectRet->DFNameExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
            index++;
            selectRet->DFNameLen = DataOut[index];

            if(selectRet->DFNameLen > 16) { return RLT_ERR_EMV_IccDataFormat; }
            memcpy(selectRet->DFName, DataOut + index + 1, DataOut[index]);
            index += selectRet->DFNameLen + 1;
            selectRet->DFNameExist = 1;
        }
        else if(!memcmp(DataOut + index, "\x9F\x11", 2))
        {
            if(selectRet->ICTIExist)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(DataOut[index + 2] != 1)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            index += 3;
            selectRet->ICTI = DataOut[index];
            index++;
            selectRet->ICTIExist = 1;
        }
        else if(!memcmp(DataOut + index, "\x9F\x12", 2))
        {
            if(selectRet->PreferNameExist)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            index += 2;
            selectRet->PreferNameLen = DataOut[index];
            j = DataOut[index];

            if(selectRet->PreferNameLen > 16) { selectRet->PreferNameLen = 16; }
            index++;
            memcpy(selectRet->PreferName, DataOut + index, selectRet->PreferNameLen);
            index += j;
            selectRet->PreferNameExist = 1;
        }
        else if(DataOut[index] == 0xA5)
        {
            selectRet->FCIPropExist = 1;
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &lenFCIProp))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexFCIProp = index;


            while(index < indexFCIProp + lenFCIProp)
            {
                if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
                else if(DataOut[index] == 0x88)
                {
                    if(selectRet->SFIExist == 1) { return RLT_ERR_EMV_IccDataFormat; }

                    if(DataOut[index + 1] != 1)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    index += 2;
                    selectRet->SFI = DataOut[index];
                    index++;
                    selectRet->SFIExist = 1;
                }
                else if(!memcmp(DataOut + index, "\x5F\x2D", 2))             //Language preference
                {
                    index += 2;
                    selectRet->LangPreferLen = DataOut[index];
                    j = DataOut[index];

                    if(selectRet->LangPreferLen > 8) { selectRet->LangPreferLen = 8; }
                    index++;
                    memcpy(selectRet->LangPrefer, DataOut + index, selectRet->LangPreferLen);
                    index += j;
                    selectRet->LangPreferExist = 1;
                }
                else if(!memcmp(DataOut + index, "\x9F\x11", 2))             //ICTI
                {
                    if(selectRet->ICTIExist)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(DataOut[index + 2] != 1)
                    {
                        return RLT_ERR_EMV_IccDataFormat; 					//with wrong len,select next app.
                    }
                    index += 3;
                    selectRet->ICTI = DataOut[index];
                    index++;
                    selectRet->ICTIExist = 1;
                }
                else if(DataOut[index] == 0x50)               //App Label
                {
                    index++;
                    selectRet->AppLabelLen = DataOut[index];
                    j = DataOut[index];

                    if(selectRet->AppLabelLen > 16) { selectRet->AppLabelLen = 16; }
                    index++;
                    memcpy(selectRet->AppLabel, DataOut + index, selectRet->AppLabelLen);
                    index += j;
                    selectRet->AppLabelExist = 1;
                }
                else if(DataOut[index] == 0x87)              //App Priority Indicator
                {
                    if(selectRet->PriorityExist == 1) { return RLT_ERR_EMV_IccDataFormat; }

                    if(DataOut[index + 1] != 1)
                    {
                        return RLT_ERR_EMV_IccDataFormat;                                        //with wrong len,select next app.
                    }
                    index += 2;
                    selectRet->Priority = DataOut[index];
                    index++;
                    selectRet->PriorityExist = 1;
                }
                else if(!memcmp(DataOut + index, "\x9F\x38", 2))             //PDOL
                {
                    if(selectRet->PDOLExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
                    index += 2;

                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    selectRet->PDOLLen = len;
                    index += selectRet->PDOLLen;
                    selectRet->PDOLExist = 1;
                }
                else if(!memcmp(DataOut + index, "\x9F\x12", 2))             //App Prefer Name
                {
                    if(selectRet->PreferNameExist)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    index += 2;
                    selectRet->PreferNameLen = DataOut[index];
                    j = DataOut[index];

                    if(selectRet->PreferNameLen > 16) { selectRet->PreferNameLen = 16; }
                    index++;
                    memcpy(selectRet->PreferName, DataOut + index, selectRet->PreferNameLen);
                    index += j;
                    selectRet->PreferNameExist = 1;
                }
                else if(!memcmp(DataOut + index, "\xBF\x0C", 2))             //FCI Issuer Discretionary Data
                {
                    if(selectRet->IssuerDiscretExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
                    index += 2;

                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    selectRet->IssuerDiscretLen = len;
                    selectRet->IssuerDiscretExist = 1;

                    indexIssuerDiscret = index;
                    lenIssuerDiscret = len;

                    while(index < indexIssuerDiscret + lenIssuerDiscret)
                    {
                        if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
                        else if(!memcmp(DataOut + index, "\x9F\x4D", 2))		//Log Entry
                        {
                            if(selectRet->LogEntryExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
                            index += 2;

                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
                            {
                                return RLT_ERR_EMV_IccDataFormat;
                            }
                            selectRet->LogEntryLen = len;
                            index += selectRet->LogEntryLen;
                            selectRet->LogEntryExist = 1;
                        }
                        else
                        {
                        	tagLen=0;
							indexttemp = index;
                            k = DataOut[index];

                            if((k & 0x1F) == 0x1F)
                            {
                                index++;
								tagLen++;
                            }
                            index++;
							tagLen++;

							tagLENLen = index;
                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
                            {
                                return RLT_ERR_EMV_IccDataFormat;
                            }

							tagLENLen = index-tagLENLen;
							tagLen = (len+tagLENLen+tagLen);

							if((tagLen > 0) && (NULL != Rfu)) // for USA GP  2021.09.18
							{
								memcpy(Rfu+(*RfuLen), DataOut+indexttemp, tagLen);
								*RfuLen += tagLen;
							}

                            index += len;
                        }
                    }
                }
                else                //other unknown TLV data
                {
                    k = DataOut[index];

                    if((k & 0x1F) == 0x1F)
                    {
                        index++;
                    }
                    index++;

                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    index += len;
                }
            }

            if(index != indexFCIProp + lenFCIProp)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
        else if(!memcmp(DataOut + index, "\x9F\x38", 2))     //for case v2cl0320006
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        else
        {
            k = DataOut[index];

            if((k & 0x1F) == 0x1F)
            {
                index++;
            }
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            index += len;
        }
    }

    if(index != indexFCI + lenFCI)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_SelectDFRetData(EMVBASE_SELECT_RET *selectRet, EMVBASE_APDU_RESP *apdu_r)
{
    unsigned char sfi;
	unsigned char len = 0;


    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)
    {
        memset(selectRet, 0, sizeof(EMVBASE_SELECT_RET));

        if(EmvContact_ReadSelectRetData(selectRet, apdu_r->DataOut, apdu_r->LenOut, NULL, &len) != RLT_EMV_OK)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(selectRet->DFNameExist == 0 || selectRet->FCIPropExist == 0 || selectRet->SFIExist == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        sfi = selectRet->SFI;

        if(sfi < 1 || sfi > 10 )
        {
            return RLT_ERR_EMV_IccDataFormat;            //sfi must be 1-10
        }
        return RLT_EMV_OK;
    }
    else if(apdu_r->SW1 == 0x6A && apdu_r->SW2 == 0x81)//20190529_LHD
    {
        return RLT_ERR_EMV_CardBlock;
    }
    else
    {
        return RLT_ERR_EMV_IccReturn;
    }
}


void EmvContact_PostReadReocrd(EMVBASE_APDU_SEND *apdu_s, unsigned char sfi, unsigned char record_num)
{
    memcpy(apdu_s->Command, "\x00\xB2", 2);
    apdu_s->Command[2] = record_num;    //record number
    apdu_s->Command[3] = sfi | 0x04;
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;
}

unsigned char EmvContact_ReadRecordRetData(EMVBASE_APDU_RESP *apdu_r, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char k;
    int matchNum;
    unsigned short index, len;
    unsigned short indexRecord, indexEntry, lenRecord, lenEntry;
    unsigned char *tempappnum;
    EMVBASE_RECORD_PSE recordPSE;
    EMVBASE_LISTAPPDATA *tempAppAppData;
    EMVBASE_TERMAPP tempAppTermApp;
	unsigned char *rfu = NULL; // for USA GP  2021.09.18

    tempAppAppData = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;
    tempappnum = &(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum);


    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00) 	//PSE Dir Record found ok.
    {
        index = 0;

        if(apdu_r->DataOut[index] != 0x70) { return RLT_ERR_EMV_IccDataFormat; }
        index++;


        if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenRecord))
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        indexRecord = index;

        while(index < indexRecord + lenRecord)
        {
            if(index >= apdu_r->LenOut)
			{
				if(NULL != rfu)
            	{
					emvbase_free(rfu);
				}
				return RLT_ERR_EMV_IccDataFormat;
			}

            if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
            {
                index++;
                continue;
            }
            else if(apdu_r->DataOut[index] == 0x61)
            {
                index++;

                if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenEntry))
                {
                	if(NULL != rfu)
                	{
						emvbase_free(rfu);
					}
                    return RLT_ERR_EMV_IccDataFormat;
                }
                indexEntry = index;
                memset((unsigned char*)&recordPSE, 0, sizeof(EMVBASE_RECORD_PSE));

                while(index < indexEntry + lenEntry)
                {
                    if(index >= apdu_r->LenOut)
					{
						if(NULL != rfu)
	                	{
							emvbase_free(rfu);
						}
						return RLT_ERR_EMV_IccDataFormat;
					}

                    if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
                    {
                        index++;
                        continue;
                    }
                    else if(apdu_r->DataOut[index] == 0x9D)                  //DDF
                    {
                        if(recordPSE.DFNameExist == 1)
						{
							if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
							return RLT_ERR_EMV_IccDataFormat;
						}
                        index++;
                        recordPSE.DFNameLen = apdu_r->DataOut[index];
                        index++;
                        memcpy(recordPSE.DFName, apdu_r->DataOut + index, recordPSE.DFNameLen);
                        index += recordPSE.DFNameLen;
                        recordPSE.DFNameExist = 1;
                        recordPSE.Type = 1;
                    }
                    else if(apdu_r->DataOut[index] == 0x4F)                  //ADF
                    {
                        if(recordPSE.DFNameExist == 1)
						{
							if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
							return RLT_ERR_EMV_IccDataFormat;
						}
                        index++;
                        recordPSE.DFNameLen = apdu_r->DataOut[index];
                        index++;
                        memcpy(recordPSE.DFName, apdu_r->DataOut + index, recordPSE.DFNameLen);
                        index += recordPSE.DFNameLen;
                        recordPSE.DFNameExist = 1;
                        recordPSE.Type = 2;
                    }
                    else if(apdu_r->DataOut[index] == 0x50)
                    {
                        if(recordPSE.AppLabelExist == 1)
						{
							if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
							return RLT_ERR_EMV_IccDataFormat;
						}
                        index++;
                        recordPSE.AppLabelLen = apdu_r->DataOut[index];
                        index++;
                        memcpy(recordPSE.AppLabel, apdu_r->DataOut + index, recordPSE.AppLabelLen);
                        index += recordPSE.AppLabelLen;
                        recordPSE.AppLabelExist = 1;
                    }
                    else if(!memcmp(apdu_r->DataOut + index, "\x9F\x12", 2))                 //App Prefer Name
                    {
                        if(recordPSE.PreferNameExist == 1)
						{
							if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
							return RLT_ERR_EMV_IccDataFormat;
						}
                        index += 2;
                        recordPSE.PreferNameLen = apdu_r->DataOut[index];
                        index++;

                        if(recordPSE.PreferNameLen > 16) {recordPSE.PreferNameLen = 16; }
                        memcpy(recordPSE.PreferName, apdu_r->DataOut + index, recordPSE.PreferNameLen);
                        index += recordPSE.PreferNameLen;
                        recordPSE.PreferNameExist = 1;
                    }
                    else if(apdu_r->DataOut[index] == 0x87)
                    {
                        if(recordPSE.PriorityExist == 1)
						{
							if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
							return RLT_ERR_EMV_IccDataFormat;
						}
                        index += 2;
                        recordPSE.Priority = apdu_r->DataOut[index];
                        index++;
                        recordPSE.PriorityExist = 1;
                    }
                    else if(apdu_r->DataOut[index] == 0x73)
                    {
                        if(recordPSE.DirDiscretExist == 1)
						{
							if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
							return RLT_ERR_EMV_IccDataFormat;
						}
                        index++;

                        if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                        {
                        	if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
                            return RLT_ERR_EMV_IccDataFormat;
                        }
                        recordPSE.DirDiscretLen = len;
                        recordPSE.DirDiscretExist = 1;

						rfu = (unsigned char *)emvbase_malloc(recordPSE.DirDiscretLen); // for USA GP 2021.09.18
						if(NULL == rfu)
						{
							return RLT_EMV_ERR;
						}
						memcpy(rfu, apdu_r->DataOut + index, recordPSE.DirDiscretLen);

						index += recordPSE.DirDiscretLen;
                    }
                    else                    //ingnore other data
                    {
                        k = apdu_r->DataOut[index];

                        if((k & 0x1F) == 0x1F) { index++; }
                        index++;

                        if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                        {
                        	if(NULL != rfu)
		                	{
								emvbase_free(rfu);
							}
                            return RLT_ERR_EMV_IccDataFormat;
                        }
                        index += len;
                    }
                }

                if(index != indexEntry + lenEntry)
                {
                	if(NULL != rfu)
                	{
						emvbase_free(rfu);
					}
                    return RLT_ERR_EMV_IccDataFormat;
                }

                if(recordPSE.Type == 2 && recordPSE.DFNameExist == 1 && recordPSE.AppLabelExist == 1 && (*tempappnum) < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidateMaxNum)          // ADF
                {
                    //The ADF doesn't match with terminal AID,continue process next entry without adding it to candidate list.
                    matchNum = tempEmvContact_UnionStruct->CheckMatchTermAID(recordPSE.DFName, recordPSE.DFNameLen, &tempAppTermApp,NULL,0);

                    if(matchNum < 0 )
                    {
                        continue;
                    }

                    (tempAppAppData + (*tempappnum))->AidInTermLen = tempAppTermApp.AIDLen;
                    memcpy((tempAppAppData + (*tempappnum))->AidInTerm, tempAppTermApp.AID, (tempAppAppData + (*tempappnum))->AidInTermLen);
                    memcpy((tempAppAppData + (*tempappnum))->AID, recordPSE.DFName, recordPSE.DFNameLen);
                    (tempAppAppData + (*tempappnum))->AIDLen = recordPSE.DFNameLen;
                    memcpy((tempAppAppData + (*tempappnum))->AppLabel, recordPSE.AppLabel, recordPSE.AppLabelLen);
                    (tempAppAppData + (*tempappnum))->AppLabelLen = recordPSE.AppLabelLen;

                    if(recordPSE.PreferNameExist == 1)
                    {
                        memcpy((tempAppAppData + (*tempappnum))->PreferName, recordPSE.PreferName, recordPSE.PreferNameLen);
                        (tempAppAppData + (*tempappnum))->PreferNameLen = recordPSE.PreferNameLen;
                    }

                    if(recordPSE.PriorityExist == 1)
                    {
                        (tempAppAppData + (*tempappnum))->Priority = recordPSE.Priority;
                    }

					if(recordPSE.DirDiscretExist == 1) // for USA GP 2021.09.18
					{
						(tempAppAppData + (*tempappnum))->rfu = rfu;
						(tempAppAppData + (*tempappnum))->rfuLen = recordPSE.DirDiscretLen;
					}

                    (*tempappnum)++;
                }
                else if(recordPSE.Type == 1 && recordPSE.DFNameExist == 1)
                {
                	if(NULL != rfu)
                	{
						emvbase_free(rfu);
                        rfu = NULL;
					}
                }
                else
                {
                	if(NULL != rfu)
                	{
						emvbase_free(rfu);
					}
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }
            else            //unknown tag data
            {
                k = apdu_r->DataOut[index];

                if((k & 0x1F) == 0x1F) { index++; }
                index++;

                if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                {
                	if(NULL != rfu)
                	{
						emvbase_free(rfu);
					}
                    return RLT_ERR_EMV_IccDataFormat;
                }
                index += len;
            }
        }

        if(index != indexRecord + lenRecord)
        {
        	if(NULL != rfu)
        	{
				emvbase_free(rfu);
			}
            return RLT_ERR_EMV_IccDataFormat;
        }
    }     //if PSE directory record found.(Read Record)
    else if(apdu_r->SW1 == 0x6A && apdu_r->SW2 == 0x83) //no DF Record for this record number.
    {
        return RLT_EMV_OTHER;
    }
    else
    {
        return RLT_ERR_EMV_IccReturn;
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_SelectDDF(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    EMVBASE_SELECT_RET selectRet;
    unsigned char Recordnum;
    EMVBASE_LISTAPPDATA *tempAppAppData;
    unsigned char i;


    EMVBase_COMMAND_SELECT("1PAY.SYS.DDF01", 14, 0, &apdu_s);

    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    retCode = EmvContact_SelectDFRetData(&selectRet, &apdu_r);

    if(retCode != RLT_EMV_OK)
    {
        return retCode;
    }
    Recordnum = 1;

    while(1)
    {
        EmvContact_PostReadReocrd(&apdu_s, selectRet.SFI << 3, Recordnum);
        tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

        if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
	    {
	        return RLT_ERR_EMV_APDUTIMEOUT;
	    }
        retCode = EmvContact_ReadRecordRetData(&apdu_r, tempEmvContact_UnionStruct);

        if(retCode != RLT_EMV_OK)
        {
            break;
        }
        Recordnum++;
    }

	#if 0//20190530_lhd
	if(retCode == RLT_EMV_OTHER)
	{
		retCode = RLT_EMV_OK;
	}
	#endif

    tempAppAppData = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;

    //save DF name
    //20190530_lhd  if(retCode == RLT_EMV_OK && tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)
    if(retCode == RLT_EMV_OTHER && tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)
    {
        emvbase_avl_createsettagvalue(EMVTAG_DFName, selectRet.DFName, selectRet.DFNameLen);

        if(selectRet.ICTIExist)
        {
            emvbase_avl_settag(EMVTAG_ICTI, selectRet.ICTI);

            for(i = 0; i < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum; i++)
            {
                (tempAppAppData + i)->ICTILen = 1;
                (tempAppAppData + i)->ICTI = selectRet.ICTI;
            }
        }
    }

    return retCode;
}


void EmvContact_PostReadApp(unsigned short i, unsigned char nextflag, EMVBASE_TERMAPP *temptermapp, EMVBASE_APDU_SEND *apdu_s)
{
    if(nextflag)
    {
        memcpy(apdu_s->Command, "\x00\xA4\x04\x02", 4);       //ADF,select next app with same AID
    }
    else
    {
        memcpy(apdu_s->Command, "\x00\xA4\x04\x00", 4);       //ADF
    }
    apdu_s->Lc = (temptermapp + i)->AIDLen;

    memcpy(apdu_s->DataIn, (temptermapp + i)->AID, (temptermapp + i)->AIDLen);
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;
}


unsigned char EmvContact_SelectTermRetData(EMVBASE_APDU_RESP *apdu_r, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char appnumindex, EMVBASE_TERMAPP *temptermapp)
{
    EMVBASE_SELECT_RET selectRet;
    unsigned char retCode;
    EMVBASE_LISTAPPDATA *tempAppData;
    unsigned char *tempapplistnum;
	unsigned char *rfu = NULL; // for USA GP  2021.09.18
	unsigned char rfuLen = 0; // for USA GP  2021.09.18


    tempAppData = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;
    tempapplistnum = &(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum);



    if(apdu_r->SW1 == 0x6A && apdu_r->SW2 == 0x81 ) 	//card block or command not support
    {
        return RLT_ERR_EMV_CardBlock;
    }
    else if(apdu_r->SW1 == 0x62 && apdu_r->SW2 == 0x83) //App is blocked
    {
        return RLT_ERR_EMV_APPBLOCK;
    }
    else if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00) //Select successful
    {
        memset((unsigned char*)&selectRet, 0, sizeof(EMVBASE_SELECT_RET));
		rfu = emvbase_malloc(256);
        if(NULL == rfu)
        {
            return RLT_EMV_ERR;
        }
		memset(rfu, 0, 256);
        retCode = EmvContact_ReadSelectRetData(&selectRet, apdu_r->DataOut, apdu_r->LenOut, rfu, &rfuLen);

        if(retCode != RLT_EMV_OK)
        {
            if(NULL != rfu)
            {
                emvbase_free(rfu);
			    rfu = NULL;
            }
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(selectRet.DFNameExist == 0 || selectRet.FCIPropExist == 0)
        {
            if(NULL != rfu)
            {
                emvbase_free(rfu);
			    rfu = NULL;
            }
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(!memcmp((temptermapp + appnumindex)->AID, selectRet.DFName, (temptermapp + appnumindex)->AIDLen))
        {
            if((temptermapp + appnumindex)->AIDLen == selectRet.DFNameLen)
            {
                retCode = EMVCONTACT_EXTRACTMATCH;
                (tempAppData + *tempapplistnum)->AidInTermLen = (temptermapp + appnumindex)->AIDLen;
                memcpy((tempAppData + *tempapplistnum)->AidInTerm, (temptermapp + appnumindex)->AID, (tempAppData + *tempapplistnum)->AidInTermLen);
            }
            else
            {
                retCode = EMVCONTACT_PARTICALMATCH;

                if((temptermapp + appnumindex)->ASI != AID_PARTIAL_MATCH)
                {
                    retCode = EMVCONTACT_NOTMATCH;
                    return retCode;
                }
                (tempAppData + *tempapplistnum)->AidInTermLen = (temptermapp + appnumindex)->AIDLen;
                memcpy((tempAppData + *tempapplistnum)->AidInTerm, (temptermapp + appnumindex)->AID, (tempAppData + *tempapplistnum)->AidInTermLen);
            }
        }
        else
        {
           if(NULL != rfu)
            {
                emvbase_free(rfu);
			    rfu = NULL;
            }
            return RLT_ERR_EMV_IccDataFormat;
        }
        memcpy((tempAppData + *tempapplistnum)->AID, selectRet.DFName, selectRet.DFNameLen);
        (tempAppData + *tempapplistnum)->AIDLen = selectRet.DFNameLen;


        memcpy((tempAppData + *tempapplistnum)->AppLabel, selectRet.AppLabel, selectRet.AppLabelLen);
        (tempAppData + *tempapplistnum)->AppLabelLen = selectRet.AppLabelLen;

        if(selectRet.PriorityExist == 1)
        {
            (tempAppData + *tempapplistnum)->Priority = selectRet.Priority;
        }

        if(selectRet.LangPreferExist == 1)
        {
            memcpy((tempAppData + *tempapplistnum)->LangPrefer, selectRet.LangPrefer, selectRet.LangPreferLen);
            (tempAppData + *tempapplistnum)->LangPreferLen = selectRet.LangPreferLen;
        }

        if(selectRet.ICTIExist == 1)
        {
            (tempAppData + *tempapplistnum)->ICTI = selectRet.ICTI;
            (tempAppData + *tempapplistnum)->ICTILen = 1;
        }

        if(selectRet.PreferNameExist == 1)
        {
            memcpy((tempAppData + *tempapplistnum)->PreferName, selectRet.PreferName, selectRet.PreferNameLen);
            (tempAppData + *tempapplistnum)->PreferNameLen = selectRet.PreferNameLen;
        }

		if(0 != rfuLen) // for USA GP 2021.09.18
		{
			(tempAppData + *tempapplistnum)->rfu = (unsigned char *)emvbase_malloc(rfuLen);
			if(NULL == (tempAppData + *tempapplistnum)->rfu)
			{
				if(NULL != rfu)
                {
                    emvbase_free(rfu);
    			    rfu = NULL;
                }
				return RLT_EMV_ERR;
			}
			else
			{
				memcpy((tempAppData + *tempapplistnum)->rfu, rfu, rfuLen);
				(tempAppData + *tempapplistnum)->rfuLen = rfuLen;
			}
			if(NULL != rfu)
            {
                emvbase_free(rfu);
			    rfu = NULL;
            }

		}
		else
		{
			if(NULL != rfu)
            {
                emvbase_free(rfu);
			    rfu = NULL;
            }
		}

        return retCode;
    }
    else
    {
        return RLT_EMV_OK;
    }
}


unsigned char EmvContact_SelectFromTerm(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    unsigned char AppNumIndex = 0;
    EMVBASE_TERMAPP *temptermapp;
    unsigned char temptermlistnum;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    EMVBASE_LISTAPPDATA *tempAppData;
    unsigned char bSecond = 0;
    unsigned char *tempapplistnum, i;

    temptermapp = (EMVBASE_TERMAPP *)emvbase_malloc(sizeof(EMVBASE_TERMAPP) * 40);

    tempAppData = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;
    tempapplistnum = &(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum);

    for(i = 0; i < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidateMaxNum; i++)
    {
        memset(tempAppData + i, 0, sizeof(EMVBASE_LISTAPPDATA));
    }

    *tempapplistnum = 0;
    tempEmvContact_UnionStruct->ReadTermAID(&temptermapp[0], &temptermlistnum);

    while(AppNumIndex < temptermlistnum)
    {
		if((*tempapplistnum) >= tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidateMaxNum)
		{
			break;
		}
        EmvContact_PostReadApp(AppNumIndex, bSecond, temptermapp, &apdu_s);

        tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

		if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
		{
			emvbase_free(temptermapp);
			return RLT_ERR_EMV_APDUTIMEOUT;
		}

        retCode = EmvContact_SelectTermRetData(&apdu_r, tempEmvContact_UnionStruct, AppNumIndex, &temptermapp[0]);

        if(retCode == EMVCONTACT_EXTRACTMATCH)
        {
            (*tempapplistnum)++;
        }
        else if(retCode == EMVCONTACT_PARTICALMATCH)
        {
            bSecond = 1;
            (*tempapplistnum)++;
            continue;
        }
        else if(retCode == EMVCONTACT_NOTMATCH)
        {
            bSecond = 1;
            continue;
        }
        else if(retCode == RLT_ERR_EMV_CardBlock)
        {
            if(bSecond == 0)
            {
                emvbase_free(temptermapp);
                return RLT_ERR_EMV_CardBlock;
            }
        }
        else if(retCode == RLT_ERR_EMV_APPBLOCK)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->appblockflag = 1;
            bSecond = 1;
            continue;
        }
        AppNumIndex++;
        bSecond = 0;
    }

    emvbase_free(temptermapp);
    return RLT_EMV_OK;
}


unsigned char EmvContact_GetAppCandidate(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retcode = RLT_EMV_OK;

    retcode = EmvContact_SelectDDF(tempEmvContact_UnionStruct);

	if(retcode == RLT_EMV_OTHER) //20190530_lhd
    {
        if(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum == 0)
        {
            retcode = RLT_ERR_EMV_PSENOTFOUND;
        }
    }

    //20190530_lhd if(retcode == RLT_EMV_OK || retcode == RLT_ERR_EMV_CardBlock || retcode == RLT_ERR_EMV_APDUTIMEOUT)
    if(retcode == RLT_EMV_OTHER || retcode == RLT_ERR_EMV_CardBlock || retcode == RLT_ERR_EMV_APDUTIMEOUT)//20190530_lhd
    {
		if (retcode == RLT_EMV_OTHER)//20190530_lhd
		{
			retcode = RLT_EMV_OK;
		}
        return retcode;
    }
    retcode = EmvContact_SelectFromTerm(tempEmvContact_UnionStruct);

    return retcode;
}


unsigned char EmvContact_AppSelect(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retcode;

    retcode = EmvContact_GetAppCandidate(tempEmvContact_UnionStruct);

    return retcode;
}


unsigned char EmvContact_ChooseAppList(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i, j, bPriority, bConfirm, temp;
    EMVBASE_LISTAPPDATA *tempAppAppData;


    tempAppAppData = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;


    tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo = 0xff;


    if(tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo > tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)
    {
        bPriority = 1;
        bConfirm = 1;

        for(i = 0; i < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum; i++)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i] = (tempAppAppData + i)->Priority & 0x0F;
            tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppSeq[i] = i;
        }

        if(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum == 1)
        {
            if((tempAppAppData->Priority & 0x80) || tempEmvContact_UnionStruct->EmvTradeParam->bretforminit)
            {

                tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.seqtype = 0;
                return RLT_EMV_USERSELECT;
            }
            else
            {
                tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo = 0;
            }
        }
        else
        {
            for(j = 0; j < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum - 1; j++)
            {
                for(i = 0; i < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum - 1; i++)
                {
                    if(((tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i] > tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i + 1]) &&  tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i + 1] != 0) ||
                        (tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i] == 0 && tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i + 1] != 0))
                    {
                        temp = tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i];
                        tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i] = tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i + 1];
                        tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppPriority[i + 1] = temp;
                        temp = tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppSeq[i];
                        tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppSeq[i] = tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppSeq[i + 1];
                        tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.AppSeq[i + 1] = temp;
                    }
                }
            }

            for(i = 0; i < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum; i++)
            {
                if(((tempAppAppData + i)->Priority & 0x80) == 0) { bConfirm = 0; }
            }

            if(bPriority == 1)
            {
                tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.seqtype = 2;
                return RLT_EMV_USERSELECT;
            }

            else
            {
                tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData.seqtype = 1;
                return RLT_EMV_USERSELECT;
            }
        }
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_ChooseApp(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;


    EMVCONTACTDISPSEQLISTDATA *tempdispseqlistdata;


    tempdispseqlistdata = &(tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData);

    if(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum < 1)
    {
        return RLT_ERR_EMV_NoAppSel;
    }
    retCode = EmvContact_ChooseAppList(tempEmvContact_UnionStruct);

    if(retCode == RLT_EMV_USERSELECT)
    {
        return RLT_EMV_USERSELECT;
    }

    if(tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo > tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)
	{
        return RLT_ERR_EMV_CancelTrans;
    }
    else
    {
        return RLT_EMV_OK;
    }
}


unsigned char EmvContact_UserSelectResult(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVCONTACTDISPSEQLISTDATA *tempdispseqlistdata;

    tempdispseqlistdata = &(tempEmvContact_UnionStruct->EmvTradeParam->DispSeqListData);

    if(tempdispseqlistdata->SelectedResult == RLT_EMV_OK)
    {
        if(tempdispseqlistdata->seqtype == 0)
        {
            if(tempdispseqlistdata->SelectedNum == 0)
            {
                tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo = 0;
                return RLT_EMV_OK;
            }
        }
        else if(tempdispseqlistdata->seqtype == 1)
        {
            if(tempdispseqlistdata->SelectedNum <= tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)
            {
                tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo = tempdispseqlistdata->SelectedNum;
                return RLT_EMV_OK;
            }
        }
        else if(tempdispseqlistdata->seqtype == 2)
        {
            if(tempdispseqlistdata->SelectedNum <= tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)
            {
                tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo = tempdispseqlistdata->AppSeq[tempdispseqlistdata->SelectedNum];
                return RLT_EMV_OK;
            }
        }
    }
    else
    {
        return RLT_ERR_EMV_CancelTrans;
    }

    return RLT_ERR_EMV_IccReturn;
}


unsigned char EmvContact_RebuildApplist(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i;

    EMVBASE_LISTAPPDATA *tempapplist;


    tempapplist = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;


    if(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum > 1)
    {
        for(i = tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo; i < tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum - 1; i++)
        {
            EMVBase_AppCopy(i, i + 1, tempapplist);
        }

        (tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum)--;
		EMVBase_Trace("9f02 test2:after del one app,the AppListCandidatenum = %d\r\n",(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum));
        return RLT_EMV_OK;
    }
    else
    {
        return RLT_ERR_EMV_NoAppSel;
    }
}

unsigned char EmvContact_checkifwrongtag(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char *tag, unsigned char taglen, EMVBASETAGCVLITEM *retrunitem, unsigned char Redundantflag)
{
    int i;
    unsigned char tagknown = 0;
    unsigned char present = 0;
    unsigned char empty = 1;
    unsigned char tagprivateclass = 0;
    unsigned char updateRA = 0;
    unsigned char temptag[3];
    unsigned char temptaglen;
    EMVBASETAGCVLITEM *item = NULL, *item1 = NULL;
    unsigned char ret = 2;
    int ret1;
    unsigned char *rundatabuf;
    unsigned int rundatabuflen;


    memset(temptag, 0, sizeof(temptag));
    temptaglen = (taglen > 3) ? 3 : (taglen);
    memcpy(temptag, tag, temptaglen);
    item = emvbase_avl_gettagitempointer(temptag);

    if(item != NULL)
    {
        present = 1;

        if(item->SupAppType & EMVTAGTYPE_EMV)
        {
            tagknown = 1;
        }
        updateRA = item->updatecondition & EMVTAGUPDATECONDITION_RA;

        if(item->len)
        {
            empty = 0;
        }
    }

    if(present == 0)      //check if tag known
    {

        ret1 = emvbase_TagBaseLib_read(tag, taglen,retrunitem, EMVTAGTYPE_EMV);

        if(ret1 == 0)
        {
            tagknown = 1;
            updateRA = retrunitem->updatecondition & EMVTAGUPDATECONDITION_RA;
        }
    }
    else
    {
        memcpy(retrunitem, item, sizeof(EMVBASETAGCVLITEM));
    }

    if((tag[0] & 0x20) == 0x00)
    {
        tagprivateclass = 1;
    }

    if(updateRA)
    {
        ret = 1;
    }

	if(Redundantflag && tagprivateclass)
	{
        rundatabuflen = tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataLen;

        if(tempEmvContact_UnionStruct->EmvTradeParam->RedundantData == NULL)
    	{
    		tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataMaxLen = 256;
			tempEmvContact_UnionStruct->EmvTradeParam->RedundantData = (unsigned char *)emvbase_malloc(256);
			if(tempEmvContact_UnionStruct->EmvTradeParam->RedundantData)
			{
				memset(tempEmvContact_UnionStruct->EmvTradeParam->RedundantData,0,256);
			}
    	}

        if(rundatabuflen && tempEmvContact_UnionStruct->EmvTradeParam->RedundantData)
        {
            if(EMVBase_CheckifRedundantData(temptag, tempEmvContact_UnionStruct->EmvTradeParam->RedundantData, rundatabuflen))
            {

                ret = 0;
            }
        }
		if(tempEmvContact_UnionStruct->EmvTradeParam->RedundantData)
		{
			if((temptaglen + rundatabuflen) < tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataMaxLen)
			{
		        memcpy(&tempEmvContact_UnionStruct->EmvTradeParam->RedundantData[rundatabuflen], temptag, temptaglen);
				tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataLen = (rundatabuflen + temptaglen);
			}
			else
			{
				if(tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataMaxLen < 512)
				{
					unsigned char *p = (unsigned char *)emvbase_malloc(512);
					if(p)
					{
						memset(p,0,512);
						memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->RedundantData,rundatabuflen);
						if(tempEmvContact_UnionStruct->EmvTradeParam->RedundantData)
						{
							emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->RedundantData);
						}
						tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataMaxLen = 512;
						tempEmvContact_UnionStruct->EmvTradeParam->RedundantData = p;
						memcpy(&tempEmvContact_UnionStruct->EmvTradeParam->RedundantData[rundatabuflen], temptag, temptaglen);
						tempEmvContact_UnionStruct->EmvTradeParam->RedundantDataLen = (rundatabuflen + temptaglen);
					}
				}
			}
		}
    }

    return ret;
}


unsigned char EmvC_ParseAndStoreCardResponse(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char *DataOut, unsigned short *DataOutIndex, unsigned char *parsebInTable, unsigned char readstep)
{
    unsigned char i, k;
    unsigned short index, len, tagdatalen;
    unsigned char tmpdata, tagindex, tmpval, addvalue, ifemvtag;
    unsigned char bInTable;
    unsigned char tag[3], taglen;
    unsigned char ret;
    unsigned char needdonextstep = 1;
    EMVBASETAGCVLITEM tagitem;

    index = *DataOutIndex;
    bInTable = 0;

    if(DataOut[index] == 0xFF || DataOut[index] == 0x00)
    {
        index++;
        *DataOutIndex = index;
        *parsebInTable = bInTable;

        return RLT_EMV_OK;
    }
    memset(tag, 0, sizeof(tag));
    taglen = 0;
    tag[taglen++] = DataOut[index];

    if((tag[0] & 0x1f) == 0x1f)
    {
        tag[taglen++] = DataOut[index + 1];

        if(tag[1] & 0x80)
        {
            tag[taglen++] = DataOut[index + 2];
        }
    }

    ret = EmvContact_checkifwrongtag(tempEmvContact_UnionStruct, tag, taglen, &tagitem, readstep);

    if(ret == 0)
    {

        return RLT_ERR_EMV_IccDataFormat;
    }
    else if(ret == 2)
    {

    }
    else
    {
        index += taglen;

        if(EMVBase_ParseExtLen(DataOut, &index, &len))
        {

            return RLT_ERR_EMV_IccDataFormat;
        }
        tagdatalen = len;

        if(len)
        {
            if(len > tagitem.maxlen || len < tagitem.minlen)
            {
                if(memcmp(tag, EMVTAG_AppLabel, taglen) == 0 || memcmp(tag, EMVTAG_AppPreferName, taglen) == 0 ||
                   memcmp(tag, EMVTAG_ICTI, taglen) == 0 || memcmp(tag, EMVTAG_LangPrefer, taglen) == 0 ||
                   memcmp(tag, EMVTAG_LogEntry, taglen) == 0 ||
                   memcmp(tag, EMVTAG_VLPAvailableFund, taglen) == 0 || memcmp(tag, EMVTAG_VLPIssuAuthorCode, taglen) == 0 ||
                   memcmp(tag, EMVTAG_CardTransPredicable, taglen) == 0 || memcmp(tag, EMVTAG_qPBOCOfflineSpendAmount, taglen) == 0 ||
                   memcmp(tag, EMVTAG_FormFactIndicator, taglen) == 0 || memcmp(tag, EMVTAG_CardIdentifyInfo, taglen) == 0 ||
                   memcmp(tag, EMVTAG_CardHoldName, taglen) == 0 || memcmp(tag, EMVTAG_CardHoldNameExt, taglen) == 0 ||
                   memcmp(tag, EMVTAG_ECResetThreshold, taglen) == 0 || memcmp(tag, EMVTAG_MaxECCashBalance, taglen) == 0 ||
                   memcmp(tag, EMVTAG_CardAuthData, taglen) == 0 ||  memcmp(tag, EMVTAG_TermTransPredicable, taglen) == 0 ||
                   memcmp(tag, EMVTAG_CardHoldIdType, taglen) == 0 || memcmp(tag, EMVTAG_CardHoldIdNo, taglen) == 0 )
                {
                    if(len > tagitem.maxlen)
                    {
                        tagdatalen = tagitem.maxlen;
                    }
                }
                else
                {

                    return RLT_ERR_EMV_IccDataFormat;
                }
            }
        }

        emvbase_avl_createsettagvalue(tag, &DataOut[index], tagdatalen);

        index += len;
        bInTable = 1;
    }

    if(bInTable == 0)
    {
        index += taglen;

        if(EMVBase_ParseExtLen(DataOut, &index, &len))
        {

            return RLT_ERR_EMV_IccDataFormat;
        }
        index += len;

        bInTable = 1;
    }

    *DataOutIndex = index;
    *parsebInTable = bInTable;

    return RLT_EMV_OK;
}

unsigned char EmvContact_FinalReadSelectRetData(EMVBASE_SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char j, k;
    unsigned short index, indexFCI, indexFCIProp, len, lenFCI, lenFCIProp, templen;
    unsigned short indexIssuerDiscret, lenIssuerDiscret;
    unsigned char ret;
    unsigned char bIntable;
    EMVBASETAGCVLITEM *emvitem;

    index = 0;

    if(DataOut[index] != 0x6F)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    index++;

    if(EMVBase_ParseExtLen(DataOut, &index, &lenFCI))
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    indexFCI = index;

    while(index < indexFCI + lenFCI)
    {
        if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }

        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
        else if(DataOut[index] == 0x84)
        {
            if(selectRet->DFNameExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &templen))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            selectRet->DFNameLen = templen;

            if(selectRet->DFNameLen > 16) { return RLT_ERR_EMV_IccDataFormat; }
            memcpy(selectRet->DFName, DataOut + index, templen);
            selectRet->DFNameExist = 1;

            index += templen;

            emvbase_avl_createsettagvalue(EMVTAG_DFName, selectRet->DFName, selectRet->DFNameLen);
        }
        else if(DataOut[index] == 0xA5)
        {
            selectRet->FCIPropExist = 1;
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &lenFCIProp))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexFCIProp = index;

            while(index < indexFCIProp + lenFCIProp)
            {
                ret = EmvC_ParseAndStoreCardResponse(tempEmvContact_UnionStruct, DataOut, &index, &bIntable, 0);

                if(ret != RLT_EMV_OK)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }

            if(index != indexFCIProp + lenFCIProp)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(tempEmvContact_UnionStruct->EmvTradeParam->ReadLogFlag == 0)
            {
                indexIssuerDiscret = 0;
                emvitem = emvbase_avl_gettagitempointer(EMVTAG_FCIDisData);

                if(emvitem != NULL)
                {
                    while(indexIssuerDiscret < emvitem->len)
                    {
                        ret = EmvC_ParseAndStoreCardResponse(tempEmvContact_UnionStruct, emvitem->data, &indexIssuerDiscret, &bIntable, 0);
                        if(ret != RLT_EMV_OK)
                        {
                            return RLT_ERR_EMV_IccDataFormat;
                        }
                    }

                    if(indexIssuerDiscret != emvitem->len)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_deletetag(EMVTAG_FCIDisData);
                }
            }
        }
        else if(DataOut[index] == 0x9F && DataOut[index + 1] == 0x38)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        else
        {
            k = DataOut[index];

            if((k & 0x1F) == 0x1F)
            {
                index++;
            }
            index++;

            if(EMVBase_ParseExtLen(DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            index += len;
        }
    }

    if(index != indexFCI + lenFCI)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_FinalSelectRetData(EMVBASE_APDU_RESP *apdu_r, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char k, i;
    EMVBASE_SELECT_RET selectRet;
    EMVBASE_LISTAPPDATA *tempselectedapp;
    EMVBASE_LISTAPPDATA *tempappdata;
    unsigned char tempselectedappno;
    unsigned char tempselectappno;
    unsigned char indexPODL, TermTranFlag;
    unsigned char *tempappnum;

	unsigned char isSupportDataStorage = 0;
	unsigned char isSupportExtendedLogging = 0;
	EMVBASETAGCVLITEM *item = NULL;
	EMVBASETAGCVLITEM *item2 = NULL;


    tempappnum = &(tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidatenum);
    tempselectedapp = tempEmvContact_UnionStruct->EmvTradeParam->SelectedApp;
    tempappdata = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;
    tempselectedappno = tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo;


    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)		//Select OK
    {
        memset((unsigned char*)&selectRet, 0, sizeof(EMVBASE_SELECT_RET));

        if(EmvContact_FinalReadSelectRetData(&selectRet, apdu_r->DataOut, apdu_r->LenOut, tempEmvContact_UnionStruct) != RLT_EMV_OK)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        if(selectRet.DFNameExist == 0 || selectRet.FCIPropExist == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        memcpy(tempselectedapp->AID, (tempappdata + tempselectedappno)->AID, (tempappdata + tempselectedappno)->AIDLen);
        tempselectedapp->AIDLen = (tempappdata + tempselectedappno)->AIDLen;

    }
    else    //current app selected fail,delete it from app list and select again.
    {
        if(apdu_r->SW1 == 0x62 && apdu_r->SW2 == 0x83)      //APP is blocked
        {
            tempEmvContact_UnionStruct->EmvTradeParam->appblockflag = 1;
			memset((unsigned char *)&selectRet, 0, sizeof(EMVBASE_SELECT_RET));
            if(tempEmvContact_UnionStruct->EmvTradeParam->ReadLogFlag)
            {
                if(EmvContact_FinalReadSelectRetData(&selectRet, apdu_r->DataOut, apdu_r->LenOut, tempEmvContact_UnionStruct) != RLT_EMV_OK)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }
                return RLT_EMV_OK;
            }
        }
        return RLT_EMV_APPSELECTTRYAGAIN;
    }

	//Optional Feature Check
	isSupportDataStorage = emvbase_avl_gettagvalue(EMVTAG_DPASSupportDataStorage);
	isSupportExtendedLogging = emvbase_avl_gettagvalue(EMVTAG_DPASSupportExtendedLogging);
	EMVBase_Trace("EMV-info: isSupportDataStorage: %d\r\n", isSupportDataStorage);
	EMVBase_Trace("EMV-info: isSupportExtendedLogging: %d\r\n", isSupportExtendedLogging);

	item = emvbase_avl_gettagitempointer(EMVTAG_DPASCardFeatureVersionNum);
	item2 = emvbase_avl_gettagitempointer(EMVTAG_DPASCardFeatureDescriptor);

	tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
	tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable = 0;

	if((item != NULL) && (item2 != NULL))
	{
		EMVBase_Trace("EMV-info: Card Feature Version Num(TagDF3A): %02x\r\n", item->data[0]);
		EMVBase_TraceHex("EMV-info: Card Feature Descriptor(TagDF3B): \r\n", item2->data, item2->len);
		if((item->data[0] == 0x02) && (item2->len > 3))
		{
			if(isSupportExtendedLogging)
			{
				if(item2->data[0] & 0x02)
				{
					EMVBase_Trace("EMV-info: Extended Logging Enable\r\n");
					tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable = 1;
				}
			}

			if(isSupportDataStorage)
			{
				if((item2->data[0] & 0x01) && (item2->data[1] & 0xF8) && (item2->data[2]))
				//if(item2->data[0] & 0x01)
				{
					EMVBase_Trace("EMV-info: Data Storage Enable\r\n");
					tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 1;
				}
			}
		}
	}

    return EMVCONTACT_READAIDPARAMETERS;
}


unsigned char EmvContact_FinalSelect(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char tempselectappno;
    EMVBASE_LISTAPPDATA *tempAppAppData;


    tempAppAppData = tempEmvContact_UnionStruct->EmvTradeParam->AppListCandidate;
    tempselectappno = tempEmvContact_UnionStruct->EmvTradeParam->SelectedAppNo;
    EMVBase_COMMAND_SELECT((tempAppAppData + tempselectappno)->AID, (tempAppAppData + tempselectappno)->AIDLen, 0, &apdu_s);
    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    retCode = EmvContact_FinalSelectRetData(&apdu_r, tempEmvContact_UnionStruct);

	 if(retCode == RLT_EMV_OK || retCode == EMVCONTACT_READAIDPARAMETERS)
    {
        if(emvbase_avl_checkiftagexist(EMVTAG_AppLabel) == 0)
        {
            if((tempAppAppData + tempselectappno)->AppLabelLen)
            {
                emvbase_avl_createsettagvalue(EMVTAG_AppLabel, (tempAppAppData + tempselectappno)->AppLabel, (tempAppAppData + tempselectappno)->AppLabelLen);
            }
        }

        if(emvbase_avl_checkiftagexist(EMVTAG_AppPreferName) == 0)
        {
            if((tempAppAppData + tempselectappno)->PreferNameLen)
            {
                emvbase_avl_createsettagvalue(EMVTAG_AppPreferName, (tempAppAppData + tempselectappno)->PreferName, (tempAppAppData + tempselectappno)->PreferNameLen);
            }
        }

        if(emvbase_avl_checkiftagexist(EMVTAG_LangPrefer) == 0)
        {
            if((tempAppAppData + tempselectappno)->LangPreferLen)
            {
                emvbase_avl_createsettagvalue(EMVTAG_LangPrefer, (tempAppAppData + tempselectappno)->LangPrefer, (tempAppAppData + tempselectappno)->LangPreferLen);
            }
        }

        if((tempAppAppData + tempselectappno)->ICTILen)
        {
            emvbase_avl_settag(EMVTAG_ICTI, (tempAppAppData + tempselectappno)->ICTI);
        }
        else
        {
            emvbase_avl_deletetag(EMVTAG_ICTI);
        }
    }

    return retCode;
}

unsigned char EmvContact_FinalSelectedApp(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;


	emvbase_avl_deletetag(EMVTAG_DPASCardFeatureVersionNum);
	emvbase_avl_deletetag(EMVTAG_DPASCardFeatureDescriptor);

    retCode = EmvContact_FinalSelect(tempEmvContact_UnionStruct);

    if(retCode == RLT_EMV_OK || retCode == RLT_ERR_EMV_APDUTIMEOUT)
    {
        return retCode;
    }
    else if(retCode == EMVCONTACT_READAIDPARAMETERS)
    {
        return retCode;
    }

	tempEmvContact_UnionStruct->EmvTradeParam->bretforminit = 1;
    return RLT_EMV_APPSELECTTRYAGAIN;
}

unsigned char EmvContact_CommandReadDSD(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command,"\x80\xCA\xDF\x3D",4);
	apdu_s->Lc=0;
	apdu_s->Le=256;

	apdu_s->EnableCancel = 1;
	return RLT_EMV_OK;
}

void EmvContact_FormReadAflData(EMVBASE_APDU_SEND *apdu_s, unsigned char tempAFL_RecordNum, unsigned char SFI)
{
    memcpy(apdu_s->Command, "\x00\xB2", 2);
    apdu_s->Command[2] = tempAFL_RecordNum;     //record number
    apdu_s->Command[3] = (SFI & 0xF8) | 0x04; 	//SFI
    apdu_s->Lc = 0;
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;
}

unsigned char EmvContact_ReadDataStorage(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	unsigned char retCode = 0;;
	EMVBASETAGCVLITEM *item = NULL;
	EMVBASETAGCVLITEM *item2 = NULL;
	EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	unsigned short index = 0;
	unsigned short len = 0;
	int DirectoryNum = 0;
	int i = 0;
	unsigned char *DirectoryEntryData = NULL;
	unsigned char SFI = 0;
	int ret = 0;


	if((NULL == tempEmvContact_UnionStruct->CheckDataContainer) || (NULL == tempEmvContact_UnionStruct->StoreContainerContent))
	{
		return RLT_EMV_OK;
	}

	// step1
	EmvContact_CommandReadDSD(&apdu_s);
    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
    	tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
    	return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00) // step2
	{
		if((0xDF != apdu_r.DataOut[0]) || (0x3D != apdu_r.DataOut[1]))
		{
			tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
			return RLT_ERR_EMV_IccDataFormat;
		}

		index += 2;
		EMVBase_ParseExtLen(apdu_r.DataOut, &index, &len);
		// step2a
		if((len < 8) || ((len-8) % 10 != 0))
		{
			tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
			return RLT_ERR_EMV_IccDataFormat;
		}

		emvbase_avl_createsettagvalue(EMVTAG_DPASDataStorageDirectory, &(apdu_r.DataOut[index]), len);
	}
	else
	{
		tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
		return RLT_ERR_EMV_IccReturn;
	}

	// step3
	EMVBase_Hash(&(apdu_r.DataOut[index]), len, tempEmvContact_UnionStruct->EmvTradeParam->DSDHashValue); // DSDHash used during ODA

	if(8 == len)
	{
		EMVBase_Trace("EMV-info: 8 == len\r\n");
		return RLT_EMV_OK;
	}

	DirectoryEntryData = (unsigned char *)emvbase_malloc(len-8);
	if(NULL == DirectoryEntryData)
	{
		EMVBase_Trace("EMV-error: memory error\r\n");
		return RLT_EMV_ERR;
	}

	memcpy(DirectoryEntryData, &(apdu_r.DataOut[index+8]), len-8);
	DirectoryNum = (len-8)/10;

	item = emvbase_avl_gettagitempointer(EMVTAG_DPASCardFeatureDescriptor);
	if(NULL == item)
    {
    	emvbase_free(DirectoryEntryData);

        return RLT_ERR_EMV_IccDataMissing;
    }
	SFI = item->data[1] & 0xF8;

	retCode == RLT_EMV_OK;
	if(0 != DirectoryNum) // step4
	{
		for(i=0; i<DirectoryNum; i++) // step5, 6, 7, 8, 10, 11
		{
			//Is the Directory Entry's Container ID included in the Data Container Read List?
			ret = tempEmvContact_UnionStruct->CheckDataContainer(&DirectoryEntryData[i*10]); // DirectoryEntryData[i*10]-DirectoryEntryData[i*10+3]
			if(RLT_EMV_OK != ret)
			{
				continue;
			}

			memset(&apdu_s, 0x00, sizeof(apdu_s));
			memset(&apdu_r, 0x00, sizeof(apdu_r));
			EmvContact_FormReadAflData(&apdu_s, DirectoryEntryData[i*10 + 4], SFI);
            tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
            if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
            {
            	tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
                retCode = RLT_ERR_EMV_APDUTIMEOUT;
				break;
            }
			if((apdu_r.SW1 != 0x90) || (apdu_r.SW2 != 0x00))
			{
            	tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable = 0;
                retCode = RLT_ERR_EMV_IccReturn;
				break;
            }

			if(apdu_r.DataOut[1] & 0x80)
			{
				tempEmvContact_UnionStruct->StoreContainerContent(&DirectoryEntryData[i*10], apdu_r.DataOut+3, apdu_r.LenOut-3); // step9
			}
			else
			{
				tempEmvContact_UnionStruct->StoreContainerContent(&DirectoryEntryData[i*10], apdu_r.DataOut+2, apdu_r.LenOut-2); // step9
			}
		}
	}

	emvbase_free(DirectoryEntryData);

	// step12-calling back beforeGPO could process
	//12a, 12b, 13 and 14 processed in D_PAS_PDOLProcess

	return retCode;
}

unsigned char EmvContact_PDOLProcess(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char *pdolData;
    unsigned short index, indexOut;
    unsigned char i, k, m, bInTable;
    unsigned char len;
    EMVBASETAGCVLITEM *item;
    unsigned char *PDOL;
    unsigned char tag[3], taglen;
    EMVBASETAGCVLITEM *pdoldataitem = NULL;
    unsigned char tempTermTransPredicable;
    unsigned short templen, tempPDOLlen;
    unsigned char PDOLbexist = 0;
    unsigned char *pdata, *buf;
	unsigned short DOLDataMaxLen = 255;//20201217 lsy
	unsigned char errflag = 0;

    tempEmvContact_UnionStruct->EmvTradeParam->onlinetradestatus = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_PDOL);

    if(item != NULL)
    {
        if(item->len)
        {
            PDOLbexist = 1;
        }
    }

    if(PDOLbexist == 0)
    {
        memcpy(apdu_s->Command, "\x80\xA8\x00\x00", 4);
        apdu_s->Lc = 2;
        memcpy(apdu_s->DataIn, "\x83\x00", 2);
        apdu_s->Le = 256;
        apdu_s->EnableCancel = 1;

		if(tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable)
		{
			item = emvbase_avl_gettagitempointer(EMVTAG_DPASExtendedLoggingData);
			if(item != NULL)
			{
				if(item->len > 0 && item->len <= 32)
		        {
		        	(apdu_s->Lc) += 3;
					(apdu_s->Lc) += item->len;
					memcpy((unsigned char*)&apdu_s->DataIn[2], "\xDF\x3C", 2);
					apdu_s->DataIn[4] = item->len;
				    memcpy((unsigned char*)&apdu_s->DataIn[5], item->data, item->len);
				}
			}
		}
        return RLT_EMV_OK;
    }
    pdata = (unsigned char *)emvbase_malloc(1024);
    pdolData = &pdata[0];
    PDOL = &pdata[300];
    buf = &pdata[700];

    memset(pdolData, 0, 255);

    index = 0;
    indexOut = 0;

    memset(PDOL, 0, 300);

    if(item->len > 255)
    {
        tempPDOLlen = 255;
    }
    else
    {
        tempPDOLlen = item->len;
    }
    memcpy(PDOL, item->data, tempPDOLlen);

	errflag = 0;//20201217 lsy
    while(index < tempPDOLlen)  		//Process PDOL
    {
        if(PDOL[index] == 0xFF || PDOL[index] == 0x00) {index++; continue; }
        memset(buf, 0, 255);
        bInTable = 0;

        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = PDOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = PDOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = PDOL[index + 2];
            }
        }

        if(!memcmp(tag, "\x9F\x7A", 2))
        {
        }
        else if(!memcmp(tag, "\xDF\x69", 2))
        {
            if(emvbase_avl_gettagvalue(EMVTAG_ODAalgrthFlag))
            {
                tempEmvContact_UnionStruct->EmvTradeParam->ODASMstaus = 1;
            }
        }

		if((memcmp(tag, "\x9F\x50", 2) < 0) || (memcmp(tag, "\x9F\x7F", 2) > 0) || (!memcmp(tag, "\x9F\x7A", 2)))
		{
        	pdoldataitem = emvbase_avl_gettagitempointer(tag);

        	if(pdoldataitem != NULL)
	        {
	            index += taglen;

	            if(EMVBase_ParseExtLen(PDOL, &index, &templen))
	            {
	                emvbase_free(pdata);
	                return RLT_ERR_EMV_IccDataFormat;
	            }
	            k = templen;
	            m = pdoldataitem->len;
				if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
				{
					errflag = 1;
					break;
				}
	            if(pdoldataitem->datafomat & EMVTAGFORMAT_N)	//numeric
	            {
	                if(k >= m)
	                {
	                    if(m)
	                    {
	                        memcpy(&buf[k - m], pdoldataitem->data, m);
	                    }
	                    memcpy(&pdolData[indexOut], buf, k);
	                }
	                else
	                {
	                    if(m)
	                    {
	                        memcpy(buf, pdoldataitem->data, m);
	                    }
	                    memcpy(&pdolData[indexOut], &buf[m - k], k);
	                }
	            }
	            else if(pdoldataitem->datafomat & EMVTAGFORMAT_CN)	//compact numeric
	            {
	                if(m)
	                {
	                    memset(buf, 0xFF, 255);
	                    memcpy(buf, pdoldataitem->data, m);
	                    memcpy(&pdolData[indexOut], buf, k);
	                }
	                else
	                {
	                    memset(buf, 0x00, 255);
	                    memcpy(&pdolData[indexOut], buf, k);
	                }
	            }
	            else	//other formats
	            {
	                if(m)
	                {
	                    memcpy(buf, pdoldataitem->data, m);
	                }
	                memcpy(&pdolData[indexOut], buf, k);
	            }
	            indexOut += k;
	            bInTable = 1;
	        }
		}

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(PDOL, &index, &templen))
            {
                emvbase_free(pdata);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
			if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
			{
				errflag = 1;
				break;
			}
            memcpy(&pdolData[indexOut], buf, k);
            indexOut += k;
        }
    }

	if (errflag)//20201217 lsy
	{
		emvbase_free(pdata);
		EMVBase_Trace("EmvContact_PDOLProcess doldata over maxdollen error !!!!!!\r\n");
		return RLT_EMV_ERR;
	}
    emvbase_avl_createsettagvalue(EMVTAG_PDOLData, pdolData, indexOut);


    memcpy(apdu_s->Command, "\x80\xA8\x00\x00", 4);
    apdu_s->Lc = indexOut + 2;
    len = 0;
    apdu_s->DataIn[len++] = 0x83;

    if((indexOut & 0x80) > 0)
    {
        (apdu_s->Lc)++;
        apdu_s->DataIn[len++] = 0x81;
        apdu_s->DataIn[len++] = indexOut;
    }
    else
    {
        apdu_s->DataIn[len++] = indexOut;
    }
    memcpy((unsigned char*)&apdu_s->DataIn[len], pdolData, indexOut);
	len += indexOut;

	if(tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable)
	{
		item = emvbase_avl_gettagitempointer(EMVTAG_DPASExtendedLoggingData);
		if(item != NULL)
		{
			if(item->len > 0 && item->len <= 32)
	        {
	        	(apdu_s->Lc) += 3;
				(apdu_s->Lc) += item->len;
				memcpy((unsigned char*)&apdu_s->DataIn[len], "\xDF\x3C", 2);
				len += 2;
				apdu_s->DataIn[len++] = item->len;
			    memcpy((unsigned char*)&apdu_s->DataIn[len], item->data, item->len);
			}
		}
	}

    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    emvbase_free(pdata);
    return RLT_EMV_OK;
}


unsigned char EmvContact_GPORetData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
    unsigned short index, indexTemp, len, lenTemp, cplen, j, temp;
    unsigned char i, k;
    unsigned char bInTable;
    unsigned int unknowlen;
    unsigned char tag;
    unsigned char ret;
    EMVBASETAGCVLITEM *item;



    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)		//Get Processing Options OK
    {
        index = 0;

        if(apdu_r->DataOut[index] == 0x80)				//primitive data
        {
            if(apdu_r->LenOut < 2)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(index + len != apdu_r->LenOut)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(len < 2)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(emvbase_avl_checkiftagexist(EMVTAG_AIP))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            emvbase_avl_createsettagvalue(EMVTAG_AIP, apdu_r->DataOut + index, 2);


            index += 2;
            k = (len - 2) % 4;

            if(k != 0)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = (len - 2) / 4;
            cplen = ((unsigned short)k) * 4;

            if(cplen)
            {
                if(emvbase_avl_checkiftagexist(EMVTAG_AFL))
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }
                emvbase_avl_createsettagvalue(EMVTAG_AFL, apdu_r->DataOut + index, cplen);
            }

            for(j = 0; j < k; j++)
            {
                if(apdu_r->DataOut[index + j * 4] == 0)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }

            index += cplen;
        }
        else if(apdu_r->DataOut[index] == 0x77)      //TLV coded data
        {
            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenTemp))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexTemp = index;

            while(index < indexTemp + lenTemp)
            {
                if(index >= apdu_r->LenOut)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }

                if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00) { index++; continue; }

				temp = index;
                ret = EmvC_ParseAndStoreCardResponse(tempEmvContact_UnionStruct, apdu_r->DataOut, &index, &bInTable, 2);

                if(ret != RLT_EMV_OK)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }

				if(apdu_r->DataOut[temp] == 0x57)//add for sred
				{

					tempEmvContact_UnionStruct->CheckTrack2Mask(temp-indexTemp, index-indexTemp, 0, &(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked), &(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen), &(tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData));
					EMVBase_Trace("tempApp_UnionStruct->EMVTradeParam->bTrack2DataMasked = %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked);
		    		EMVBase_Trace("tempApp_UnionStruct->EMVTradeParam->Track2DataLen = %d\r\n",tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen);
					EMVBase_Trace("tempApp_UnionStruct->EMVTradeParam->Track2OffsetInAuthData = %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData);
				}

            }

            if(index != indexTemp + lenTemp)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
        else{ return RLT_ERR_EMV_IccDataFormat; }
    }
    else if(apdu_r->SW1 == 0x69 && apdu_r->SW2 == 0x85)
    {
        return RLT_ERR_EMV_NotAccept;
    }
    else
    {
        return RLT_ERR_EMV_IccReturn;
    }

    if(index != apdu_r->LenOut)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(emvbase_avl_checkiftagexist(EMVTAG_AFL) == 0 || emvbase_avl_checkiftagexist(EMVTAG_AIP) == 0)
    {
        return RLT_ERR_EMV_IccDataMissing;
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_InitialApp(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;

    tempEmvContact_UnionStruct->EmvTradeParam->ODASMstaus = 0;
    retCode = EmvContact_PDOLProcess(tempEmvContact_UnionStruct, &apdu_s);

    if(retCode != RLT_EMV_OK)
    {
        return retCode;
    }
   tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }
    retCode = EmvContact_GPORetData(tempEmvContact_UnionStruct, &apdu_r);

    return retCode;
}

unsigned char EmvContact_CheckReadAFL(EMVBASETAGCVLITEM *item)
{
    unsigned short t, i, j;
    unsigned char AFL_Num;
    unsigned char *AFL;


    AFL_Num = item->len >> 2;

    if(AFL_Num == 0)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
    AFL = item->data;

    for(i = 0; i < AFL_Num; i++)
    {
        j = i << 2;

        t = AFL[j];
        t >>= 3;

        if(t == 0 || t >= 31)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(AFL[j + 1] < 1)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(AFL[j + 2] < AFL[j + 1])
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(AFL[j + 3] > (AFL[j + 2] - AFL[j + 1] + 1))
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
    }

    return RLT_EMV_OK;
}

//void EmvContact_CheckSensitiveData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned short startpos, unsigned short endpos, unsigned char authdataflag)
//{
//    EMVBASETAGCVLITEM *item;
//    unsigned char i,j,temp[38],temp2[38];
//
//
//    tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked = 0;
//    item = emvbase_avl_gettagitempointer(EMVTAG_Track2Equ);
//
//    if(item != NULL)
//    {
//        if((item->len < 20) && (item->data != NULL))
//        {
//            if(item->len > 7)
//			{
//				if(!memcmp(item->data, "XGD-PCI", 7))
//				{
//					tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked = 1;
//				}
//			}
//			else
//			{
//				if(!memcmp(item->data, "XGD-PCI", item->len))
//				{
//					tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked = 1;
//				}
//			}
//        }
//    }
//	EMVBase_Trace("EmvContact_SDVerify--startpos: %d\r\n", startpos);
//	EMVBase_Trace("EmvContact_SDVerify--endpos: %d\r\n", endpos);
//	EMVBase_Trace("EmvContact_SDVerify--item->len: %d\r\n", item->len);
//
//    if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && authdataflag)
//    {
//        if(endpos > startpos+item->len)
//        {
//            tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData += (endpos - item->len);
//            tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen = item->len; // endpos-startpos;
//        }
//    }
//}

unsigned char EmvContact_ReadAFLRetData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, EMVBASE_APDU_RESP *apdu_r, unsigned char tempt, unsigned char authdataflag)
{
    unsigned char t;
    unsigned char i, k, bInTable;
    unsigned short index, indexAEF, len, lenAEF;
    unsigned char tempreadaflnum;
    unsigned char tag;
    unsigned char ret;
    unsigned short indexlen, lenTemp, indexTemp;
	unsigned short tempindex;

    if(!(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)) 	// ICC parameter Records found fail.
    {
        return RLT_ERR_EMV_IccReturn;
    }
    index = 0;

    t = tempt;
    t >>= 3;

    if((t >= 11) && (t <= 30))			//mofified according to SU Bullitin No.12
    {
        if(authdataflag)
        {
            if(apdu_r->DataOut[index] == 0x70)
            {
                if((tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + apdu_r->LenOut) > 2048)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }

            	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
				{
					tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 0;
					tempEmvContact_UnionStruct->EmvTradeParam->AuthData = (unsigned char *)emvbase_malloc(512);

					if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
					{
						return RLT_EMV_ERR;
					}

					tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 512;
					memset(tempEmvContact_UnionStruct->EmvTradeParam->AuthData,0,512);
            	}


				if((tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + apdu_r->LenOut) > tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen)
                {
                	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen < 1024)
            		{
						unsigned char *p = (unsigned char *)emvbase_malloc(1024);
						if(p == NULL)
						{
							return RLT_EMV_ERR;
						}
						memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->AuthData,tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
						emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
						tempEmvContact_UnionStruct->EmvTradeParam->AuthData = p;
						tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 1024;
            		}
					else if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen < 2048)
            		{
						unsigned char *p = (unsigned char *)emvbase_malloc(2048);
						if(p == NULL)
						{
							return RLT_EMV_ERR;
						}
						memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->AuthData,tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
						emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
						tempEmvContact_UnionStruct->EmvTradeParam->AuthData = p;
						tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 2048;
            		}
					else
					{
	                    return RLT_ERR_EMV_IccDataFormat;
					}
                }

                memcpy((unsigned char*)&tempEmvContact_UnionStruct->EmvTradeParam->AuthData[tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen], apdu_r->DataOut, apdu_r->LenOut);
                tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen += apdu_r->LenOut;
            }
            else
            {
                tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData = 1;
            }
        }

        //20170510 add for 4.3f
        indexlen = 0;
         if(apdu_r->DataOut[indexlen] == 0x70)		//tag 9f24
        {
            indexlen++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &lenTemp))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexTemp = indexlen;

            while(indexlen < indexTemp + lenTemp)
            {
                if(indexlen >= apdu_r->LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                if(apdu_r->DataOut[indexlen] == 0xFF || apdu_r->DataOut[indexlen] == 0x00) { indexlen++; continue; }

                else if(!memcmp(&apdu_r->DataOut[indexlen], "\x9F\x24", 2))
                {
                	//tag of Payment Account Reference
                    indexlen += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_PayAccountRefer, &apdu_r->DataOut[indexlen], len);
                    indexlen += len;
                }
				 else if(!memcmp(&apdu_r->DataOut[indexlen], "\x9F\x0A", 2))
                {
                    indexlen += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_ASRPD, &apdu_r->DataOut[indexlen], len);
                    indexlen += len;
                }
                else		//unknown data,out of this command.
                {
                    k = apdu_r->DataOut[indexlen];

                    if((k & 0x1F) == 0x1F)
                    {
                        indexlen++;
                    }
                    indexlen++;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &indexlen, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    indexlen += len;
                }
            }

            if(indexlen != indexTemp + lenTemp)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
		//end
        return RLT_EMV_OK;
    }

    while(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
    {
        index++;
    }

    if(apdu_r->DataOut[index] != 0x70)
    {

        return RLT_ERR_EMV_IccDataFormat;
    }
    index++;

    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenAEF))
    {

        return RLT_ERR_EMV_IccDataFormat;
    }

    if(authdataflag)     //add record to authentication data string if it is required.
    {
        if((tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + lenAEF) > 2048)
        {

            return RLT_ERR_EMV_IccDataFormat;
        }

    	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
		{
			tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 0;
			tempEmvContact_UnionStruct->EmvTradeParam->AuthData = (unsigned char *)emvbase_malloc(512);

			if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
			{
				return RLT_EMV_ERR;
			}

			tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 512;
			memset(tempEmvContact_UnionStruct->EmvTradeParam->AuthData,0,512);
    	}


		if((tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + apdu_r->LenOut) > tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen)
        {
        	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen < 1024)
    		{
				unsigned char *p = (unsigned char *)emvbase_malloc(1024);
				if(p == NULL)
				{
					return RLT_EMV_ERR;
				}
				memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->AuthData,tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
				emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
				tempEmvContact_UnionStruct->EmvTradeParam->AuthData = p;
				tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 1024;
    		}
			else if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen < 2048)
    		{
				unsigned char *p = (unsigned char *)emvbase_malloc(2048);
				if(p == NULL)
				{
					return RLT_EMV_ERR;
				}
				memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->AuthData,tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
				emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
				tempEmvContact_UnionStruct->EmvTradeParam->AuthData = p;
				tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 2048;
    		}
			else
			{

                return RLT_ERR_EMV_IccDataFormat;
			}
        }

        memcpy((unsigned char*)&tempEmvContact_UnionStruct->EmvTradeParam->AuthData[tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen], (unsigned char*)&apdu_r->DataOut[index], lenAEF);
        tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen += lenAEF;
    }
    indexAEF = index;


    while(index < indexAEF + lenAEF)
    {
        if(index >= apdu_r->LenOut)
        {

            return RLT_ERR_EMV_IccDataFormat;
        }

        if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
        {
            index++;
            continue;
        }
		if(apdu_r->DataOut[index] == 0x57)
        {
			if(authdataflag && memcmp(apdu_r->DataOut + index + 2,"\xFF\xFF\xFF",3) == 0)
			{
				tempEmvContact_UnionStruct->EmvTradeParam->AuthDataFixLen = tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen - lenAEF + index - indexAEF;
			}
        }

		tempindex = index;
        ret = EmvC_ParseAndStoreCardResponse(tempEmvContact_UnionStruct, apdu_r->DataOut, &index, &bInTable, 2);

        if(ret != RLT_EMV_OK)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

		if(apdu_r->DataOut[tempindex] == 0x57) // for mask tag57
        {
        	tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData = tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen - lenAEF;
            EMVBase_Trace("before check mask, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData = %d\r\n",tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData);
          //  EmvContact_CheckSensitiveData(tempEmvContact_UnionStruct, tempindex-indexAEF, index-indexAEF, authdataflag);
			tempEmvContact_UnionStruct->CheckTrack2Mask(tempindex-indexAEF, index-indexAEF, authdataflag, &(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked), &(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen), &(tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData));
			EMVBase_Trace("tempApp_UnionStruct->EMVTradeParam->bTrack2DataMasked = %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked);
		    EMVBase_Trace("tempApp_UnionStruct->EMVTradeParam->Track2OffsetInAuthData = %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData);
			EMVBase_Trace("tempApp_UnionStruct->EMVTradeParam->Track2DataLen = %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen);
        }
    }

    if(index != indexAEF + lenAEF)
    {

        return RLT_ERR_EMV_IccDataFormat;
    }

    if(index < apdu_r->LenOut)
    {

        return RLT_ERR_EMV_IccDataFormat;
    }
    return RLT_EMV_OK;
}

static const EMVBASETAG EmvbmandatoryTag[] =
{
    {EMVTAG_AppExpireDate},
    {EMVTAG_AFL},
    {EMVTAG_PAN},
    {EMVTAG_CDOL1},
    {EMVTAG_CDOL2},
    {"\x00\x00\x00"}
};

unsigned char EmvContact_CommandReadECCashBalance(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, "\x80\xCA\x9F\x79", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    return RLT_EMV_OK;
}

unsigned char EmvContact_ReadEcData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;

    EmvContact_CommandReadECCashBalance(&apdu_s);

    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
    {
        if(!memcmp(apdu_r.DataOut, "\x9F\x79", 2))
        {
            if(apdu_r.DataOut[2] != 6)
            {
                return RLT_ERR_EMV_IccCommand;
            }
            emvbase_avl_createsettagvalue(EMVTAG_VLPAvailableFund, &apdu_r.DataOut[3], 6);
        }
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_ReadEcResetThresholdCmd(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, "\x80\xCA\x9F\x6D", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    return RLT_EMV_OK;
}

unsigned char EmvContact_ReadEcResetThreshold(EMVCONTACTTradeUnionStruct *tempApp_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;

    EmvContact_ReadEcResetThresholdCmd(&apdu_s);

    tempApp_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
    {
        if(!memcmp(apdu_r.DataOut, "\x9F\x6D", 2))
        {
            if(apdu_r.DataOut[2] != 6)
            {
                return RLT_ERR_EMV_IccCommand;
            }
            emvbase_avl_createsettagvalue(EMVTAG_ECResetThreshold, &apdu_r.DataOut[3], 6);
        }
    }
    return RLT_EMV_OK;
}

unsigned char EmvContact_StrPos(unsigned char *Src, unsigned char chr)
{
    unsigned char Len;
    unsigned char i;

    Len = strlen((char *)Src);

    for(i = 0; i < Len; i++)
    {
        if(Src[i] == chr)
        {
            return i;
        }
    }

    return 0xFF;
}

unsigned char EmvContact_DealAFLData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i, j, temp[38], temp2[38];
    EMVBASETAGCVLITEM *item, *item1;

    unsigned char AIPbExist = 0;
    unsigned char Track2EquivalentbExist, IssuAppDatabExist, AppCryptbExist, ATCbExist, AFLbExist;
    unsigned char CryptInfobExist;
    unsigned char *CryptInfo, tempCryptInfo;
    unsigned char TransTypeValue;
    unsigned char TermTransPredicable1;

    i = 0;

    while(1)
    {
        if(memcmp(EmvbmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)EmvbmandatoryTag[i].Tag) == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        i++;
    }

    item = emvbase_avl_gettagitempointer(EMVTAG_SDATagList);

    if(item != NULL)
    {
        if(item->len)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL = 1;

            if(item->len == 1)
            {
                if(item->data[0] == 0x82)
                {
                    if(emvbase_avl_gettagvalue_spec(EMVTAG_AIP, temp, 0, 2))
                    {

                        return RLT_ERR_EMV_IccDataFormat;
                    }

					if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
					{
						tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 0;
						tempEmvContact_UnionStruct->EmvTradeParam->AuthData = (unsigned char *)emvbase_malloc(512);

						if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
						{
							return RLT_EMV_ERR;
						}

						tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 512;
						memset(tempEmvContact_UnionStruct->EmvTradeParam->AuthData,0,512);
	            	}


					if((tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + 2) > tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen)
	                {
	                	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen < 1024)
	            		{
							unsigned char *p = (unsigned char *)emvbase_malloc(1024);
							if(p == NULL)
							{
								return RLT_EMV_ERR;
							}
							memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->AuthData,tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
							emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
							tempEmvContact_UnionStruct->EmvTradeParam->AuthData = p;
							tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 1024;
	            		}
						else if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen < 2048)
	            		{
							unsigned char *p = (unsigned char *)emvbase_malloc(2048);
							if(p == NULL)
							{
								return RLT_EMV_ERR;
							}
							memcpy(p,tempEmvContact_UnionStruct->EmvTradeParam->AuthData,tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
							emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
							tempEmvContact_UnionStruct->EmvTradeParam->AuthData = p;
							tempEmvContact_UnionStruct->EmvTradeParam->AuthDataMaxLen = 2048;
	            		}
						else
						{
		                    return RLT_ERR_EMV_IccDataFormat;
						}
	                }


                    memcpy((unsigned char*)&tempEmvContact_UnionStruct->EmvTradeParam->AuthData[tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen], temp, 2);
                    tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen += 2;
                    tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL = 0;
                }
            }
        }
    }
    //CVM list exist
    item = emvbase_avl_gettagitempointer(EMVTAG_CVMList);

    if(item != NULL)
    {
        if(item->len % 2 != 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_ReadAppData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    EMVBASETAGCVLITEM *item;
    unsigned char aflexist = 0;
    unsigned char Read_AFL_Num = 0;
    unsigned char AFL_RecordNum = 0;
    unsigned char AFL_Num;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char authdataflag;

    tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen = 0;
    tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData = 0;
    tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL = 0;
	//tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked = 0;
    tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData = 0;
    tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen= 0;

    item = emvbase_avl_gettagitempointer(EMVTAG_AFL);

    if(item != NULL)
    {
        if(item->len >> 2)
        {
            aflexist = 1;
        }

        if((item->len % 4) != 0)
        {
            aflexist = 0;
        }
    }

    if(aflexist == 0)
    {
        return RLT_ERR_EMV_IccDataMissing;
    }

    retCode = EmvContact_CheckReadAFL(item);

    if(retCode != RLT_EMV_OK)
    {
        return retCode;
    }
    AFL_Num = item->len >> 2;
    tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen = 0;
    tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData = 0;

    for(Read_AFL_Num = 0; Read_AFL_Num < AFL_Num; Read_AFL_Num++)
    {
        AFL_RecordNum = item->data[(Read_AFL_Num << 2) + 1];

        while(AFL_RecordNum <= item->data[(Read_AFL_Num << 2) + 2])
        {
            EmvContact_FormReadAflData(&apdu_s, AFL_RecordNum, item->data[(Read_AFL_Num << 2) + 0]);


            tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

		    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
		    {
		        return RLT_ERR_EMV_APDUTIMEOUT;
		    }

            if(AFL_RecordNum < item->data[(Read_AFL_Num << 2) + 1] + item->data[(Read_AFL_Num << 2) + 3])
            {
                authdataflag = 1;
            }
            else
            {
                authdataflag = 0;
            }

            retCode = EmvContact_ReadAFLRetData(tempEmvContact_UnionStruct, &apdu_r, item->data[(Read_AFL_Num << 2) + 0], authdataflag);

            if(retCode != RLT_EMV_OK)
            {
                return retCode;
            }
            AFL_RecordNum++;
        }
    }


    retCode = EmvContact_DealAFLData(tempEmvContact_UnionStruct);
    if(retCode == RLT_EMV_OK)
    {
        if(emvbase_avl_gettagvalue(EMVTAG_VLPIndicator))
        {
            if(emvbase_avl_checkiftagexist(EMVTAG_VLPIssuAuthorCode))
            {
                retCode = EmvContact_ReadEcData(tempEmvContact_UnionStruct);

                if(retCode == RLT_EMV_OK)
                {
                    retCode = EmvContact_ReadEcResetThreshold(tempEmvContact_UnionStruct);
                }
            }
            else
            {
                emvbase_avl_settag(EMVTAG_VLPIndicator, 0);
            }
        }
    }

    return retCode;
}

//// mode: 0x00-sha1, 0x01-SM3, 0x02-SM3-Z
//void EmvContact_GetHashEx(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char mode, unsigned char offsetNum,
//	unsigned short *offset, unsigned short dataLen, unsigned char *data, unsigned char *pkData, unsigned char *hashValue)
//{
//	unsigned char *dataBuff = NULL;
//	unsigned short dataBuffLen = 0;
//	unsigned int hashValueLen = 0;
//	unsigned char i = 0;
//	int index = 0;
//
//
//	if((NULL == data) || (NULL == hashValue))
//	{
//		return;
//	}
//
//	if(NULL != tempEmvContact_UnionStruct->GetHashWithMagMask)
//	{
//		dataBuff = (unsigned char *)emvbase_malloc(dataLen + 5 + 66 + (offsetNum * 2));
//		if(0x02 == mode)
//		{
//			dataBuffLen = dataLen + 5 + 66 + (offsetNum * 2);
//		}
//		else
//		{
//			dataBuffLen = dataLen + 5 + (offsetNum * 2);
//		}
//
//		dataBuff[0] = mode;
//		dataBuff[1] = offsetNum;
//		index += 2;
//
//		while(i < offsetNum)
//		{
//			dataBuff[index] = offset[i] >> 8;
//			dataBuff[index + 1] = (unsigned char)offset[i];
//			index += 2;
//			i++;
//		}
//
//		dataBuff[index] = tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen;
//		dataBuff[index + 1] = dataLen >> 8;
//		dataBuff[index + 2] = (unsigned char)dataLen;
//		index += 3;
//
//		memcpy(dataBuff+index, data, dataLen);
//		index += dataLen;
//
//		if(NULL != pkData)
//		{
//			dataBuff[index] = 0x00;
//			dataBuff[index+1] = 0x40;
//			memcpy(dataBuff+index+2, pkData, 64);
//		}
//		EMVBase_Trace("EmvContact_GetHashEx-index: %d\r\n", index);
//		EMVBase_TraceHex("EmvContact_GetHashEx-dataBuff: ", dataBuff, dataBuffLen);
//
//		memset(hashValue, 0x00, sizeof(hashValue));
//		tempEmvContact_UnionStruct->GetHashWithMagMask(0x03, 1, dataBuffLen, dataBuff, &hashValueLen, hashValue);
//		EMVBase_TraceHex("EmvContact_GetHashEx-hashValue: ", hashValue, hashValueLen);
//		EMVBase_Trace("free problem flag1\r\n");
//		emvbase_free(dataBuff);
//		EMVBase_Trace("not free problem flag1\r\n");
//	}
//
//}

unsigned char EmvContact_TDOLProcess(unsigned char *TDOL, unsigned short TDOLLen, unsigned char *DOLData, unsigned short *DOLDataLen, unsigned short DOLDataMaxLen,
	unsigned short *Track2DataOffset, unsigned char *Track2DataLen)
{
    unsigned short index = 0;
    unsigned char *buf;
    unsigned char bInTable;
    unsigned char tag[3], taglen;
    EMVBASETAGCVLITEM *DOLdataitem;
    unsigned short templen, indexOut = 0, DOLLen;

    unsigned char i, k, m;
    unsigned char *DOL;
	unsigned char errflag = 0;//20201217 lsy


    buf = (unsigned char *)emvbase_malloc(255);
    DOL = (unsigned char *)emvbase_malloc(300);


    memset(DOL, 0, 300);

    DOLLen = (TDOLLen > 255) ? 255 : (TDOLLen);
    memcpy(DOL, TDOL, DOLLen);

	errflag = 0;//20201217 lsy
    while(index < DOLLen)  //Process TDOL
    {
        if(DOL[index] == 0xFF || DOL[index] == 0x00) {index++; continue; }
        memset(buf, 0, 255);
        bInTable = 0;


        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = DOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = DOL[index + 2];
            }
        }
		if((memcmp(tag, "\x9F\x50", 2) < 0) || (memcmp(tag, "\x9F\x7F", 2) > 0))
		{
        	DOLdataitem = emvbase_avl_gettagitempointer(tag);


        	if(DOLdataitem != NULL)
	        {
	            index += taglen;



	            if(EMVBase_ParseExtLen(DOL, &index, &templen))
	            {
	                emvbase_free(buf);
	                emvbase_free(DOL);
	                return RLT_ERR_EMV_IccDataFormat;
	            }
	            k = templen;
	            m = DOLdataitem->len;
				if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
				{
					errflag = 1;
					break;
				}


	            if(DOLdataitem->datafomat & EMVTAGFORMAT_N)          //numeric
	            {
	                if(k >= m)
	                {
	                    if(m)
	                    {
	                        memcpy(&buf[k - m], DOLdataitem->data, m);
	                    }
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	                else
	                {
	                    if(m)
	                    {
	                        memcpy(buf, DOLdataitem->data, m);
	                    }
	                    memcpy(&DOLData[indexOut], &buf[m - k], k);
	                }
	            }
	            else if(DOLdataitem->datafomat & EMVTAGFORMAT_CN)          //compact numeric
	            {
	                if(m)
	                {
	                    memset(buf, 0xFF, 255);
	                    memcpy(buf, DOLdataitem->data, m);
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	                else
	                {
	                    memset(buf, 0x00, 255);
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	            }
	            else            //other formats
	            {
	                if(m)
	                {
	                    memcpy(buf, DOLdataitem->data, m);
	                }
	                memcpy(&DOLData[indexOut], buf, k);
	            }

				if(0x57 == tag[0])  // 20201214
				{
					*Track2DataOffset = indexOut;
					*Track2DataLen = k;
				}

	            indexOut += k;
	            bInTable = 1;
	        }
		}

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
                emvbase_free(buf);
                emvbase_free(DOL);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
			if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
			{
				errflag = 1;
				break;
			}
            memcpy(&DOLData[indexOut], buf, k);
            indexOut += k;
        }
    }

    *DOLDataLen = indexOut;


    emvbase_free(buf);
    emvbase_free(DOL);
	if (errflag)//20201217 lsy
	{
		EMVBase_Trace("EmvContact_TDOLProcess doldata over maxdollen error !!!!!!\r\n");
		return RLT_EMV_ERR;
	}
    *DOLDataLen = indexOut;


    return RLT_EMV_OK;
}


unsigned char EmvContact_DOLProcessFunc(unsigned char type, unsigned char* CDDOL, unsigned short CDDOLLen, unsigned char* DOLData, unsigned short* DOLDataLen, unsigned short DOLDataMaxLen, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned short index = 0;
    unsigned char *buf;
    unsigned char bInTable;
    unsigned char tag[3], taglen;
    EMVBASETAGCVLITEM *DOLdataitem;
    unsigned short templen, indexOut = 0;
    unsigned char bHasUnpredictNum = 0;
    unsigned char AIP[2], TermCapab[3];
    unsigned char *TDOLData;
    unsigned short TDOLDataLen;
    EMVBASETAGCVLITEM * item;
    unsigned char CardTDOLDatabExist, TermTDOLDatabExist;
    unsigned char TCHashValue[20];
    unsigned char *DOL;
    unsigned short DOLLen;
    unsigned char i, k, m;
    unsigned char CAPKI = 0;

	unsigned short TrackOffsetInTDOLDataOffset = 0;
	unsigned char Track2DataLenInTDOL = 0;

	unsigned char errflag = 0;  //20201217 lsy

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
    memset(TermCapab, 0, sizeof(TermCapab));
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

    DOL = (unsigned char *)emvbase_malloc(300);
    buf = (unsigned char *)emvbase_malloc(255);

    memset(DOL, 0, 300);

    DOLLen = (CDDOLLen > 255) ? 255 : (CDDOLLen);
    memcpy(DOL, CDDOL, DOLLen);

	errflag = 0; //20201217 lsy
    while(index < DOLLen)
    {
        if(DOL[index] == 0xFF || DOL[index] == 0x00) {index++; continue; }
        memset(buf, 0, 255);
        bInTable = 0;

        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DOL[index];

        if((tag[0] & 0x1f) == 0x1f)
        {
            tag[taglen++] = DOL[index + 1];

            if(tag[1] & 0x80)
            {
                tag[taglen++] = DOL[index + 2];
            }
        }

        if(!memcmp(tag, "\x9F\x37", 2))
        {
            bHasUnpredictNum = 1;
        }

        //4.3e add 20160330
        if(!memcmp((unsigned char*)&DOL[index], "\x9F\x22", 2))
        {
            index += 3;
            emvbase_avl_gettagvalue_spec(EMVTAG_CAPKI, &CAPKI, 0, 1);
            DOLData[indexOut] = CAPKI;
            indexOut += 1;
            continue;
        }
        //end

        if(type == typeCDOL1 || type == typeCDOL2)
        {
            if(tag[0] == 0x98)
            {
                TDOLData = (unsigned char *)emvbase_malloc(512);
                memset(TDOLData, 0, 512);
                TDOLDataLen = 0;

                TermTDOLDatabExist = 0;
                item = emvbase_avl_gettagitemandstatus(EMVTAG_TDOL, &CardTDOLDatabExist);

                if(CardTDOLDatabExist == 0)
                {
                    item = emvbase_avl_gettagitemandstatus(EMVTAG_TermTDOL, &TermTDOLDatabExist);
                }

                if(item != NULL)
                {
                    if(item->len)
                    {
                        EmvContact_TDOLProcess(item->data, item->len, TDOLData, &TDOLDataLen, 512, &TrackOffsetInTDOLDataOffset, &Track2DataLenInTDOL);
                    }
                }

				if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && Track2DataLenInTDOL) // for tag57 mask
				{
					//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x00, 1, &TrackOffsetInTDOLDataOffset, TDOLDataLen, TDOLData, NULL, TCHashValue);
					tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x00, 1, &TrackOffsetInTDOLDataOffset, TDOLDataLen, TDOLData, NULL, TCHashValue);
				}
				else
				{
			    	EMVBase_Hash(TDOLData, TDOLDataLen, TCHashValue);
				}

                emvbase_free(TDOLData);

                if(TermTDOLDatabExist)
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x80);
                }
                emvbase_avl_createsettagvalue(EMVTAG_TCHashValue, TCHashValue, 20);
            }
        }

		if((memcmp(tag, "\x9F\x50", 2) < 0) || (memcmp(tag, "\x9F\x7F", 2) > 0))
		{
        	DOLdataitem = emvbase_avl_gettagitempointer(tag);

        	if(DOLdataitem != NULL)
	        {
	            index += taglen;

	            if(EMVBase_ParseExtLen(DOL, &index, &templen))
	            {
	                emvbase_free(DOL);
	                emvbase_free(buf);
	                return RLT_ERR_EMV_IccDataFormat;
	            }
	            k = templen;
	            m = DOLdataitem->len;

				if ((indexOut + k) > DOLDataMaxLen) //20201217 lsy
				{
					errflag = 1;
					break;
				}

	            if(DOLdataitem->datafomat & EMVTAGFORMAT_N)		//numeric
	            {
	                if(k >= m)
	                {
	                    if(m)
	                    {
	                        memcpy(&buf[k - m], DOLdataitem->data, m);
	                    }
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	                else
	                {
	                    if(m)
	                    {
	                        memcpy(buf, DOLdataitem->data, m);
	                    }
	                    memcpy(&DOLData[indexOut], &buf[m - k], k);
	                }
	            }
	            else if(DOLdataitem->datafomat & EMVTAGFORMAT_CN)	//compact numeric
	            {
	                if(m)
	                {
	                    memset(buf, 0xFF, 255);
	                    memcpy(buf, DOLdataitem->data, m);
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	                else
	                {
	                    memset(buf, 0x00, 255);
	                    memcpy(&DOLData[indexOut], buf, k);
	                }
	            }
	            else	//other formats
	            {
	                if(m)
	                {
	                    memcpy(buf, DOLdataitem->data, m);
	                }
	                memcpy(&DOLData[indexOut], buf, k);
	            }

				if(0x57 == tag[0])  // 20201224
				{
					EMVBase_Trace("DOLProcessFunc-deal tag57\r\n");
					if(type == typeDDOL)
					{
						tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInDDOLData = indexOut;
						tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InDDOLData = 1;
					}
					else if(type == typeCDOL1)
					{
						tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL1Data = indexOut;
						tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL1Data = 1;
					}
					else if(type == typeCDOL2)
					{
						tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL2Data = indexOut;
						tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data = 1;
					}
				}

	            indexOut += k;
	            bInTable = 1;
	        }
		}

        if(!bInTable)
        {
            index += taglen;

            if(EMVBase_ParseExtLen(DOL, &index, &templen))
            {
                emvbase_free(DOL);
                emvbase_free(buf);
                return RLT_ERR_EMV_IccDataFormat;
            }
            k = templen;
			if ((indexOut + k) > DOLDataMaxLen)//20201217 lsy
			{
				errflag = 1;
				break;
			}
            memcpy(&DOLData[indexOut], buf, k);
            indexOut += k;
        }
    }


    emvbase_free(DOL);
    emvbase_free(buf);

	if (errflag)//20201217 lsy
	{
		EMVBase_Trace("EmvContact_DOLProcessFunc doldata over maxdollen error !!!!!!\r\n");
		return RLT_EMV_ERR;
	}
    *DOLDataLen = indexOut;

    if(bHasUnpredictNum == 0)
    {
        if(type == typeDDOL)
        {
            return RLT_EMV_ERR;
        }
        else if(type == typeCDOL1)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->bCDOL1HasNoUnpredictNum = 1;

            if((AIP[0] & 0x01) && (TermCapab[2] & 0x08))
            {
                return RLT_EMV_OK;
            }
        }
        else if(type == typeCDOL2)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->bCDOL2HasNoUnpredictNum = 1;

            if((AIP[0] & 0x01) && (TermCapab[2] & 0x08))
            {
                return RLT_EMV_OK;
            }
        }
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_DOLProcess(unsigned char type, unsigned char * DOL, unsigned short DOLLen, unsigned char* DOLData, unsigned short* DOLDataLen, unsigned short DOLDataMaxLen, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)//20201217 lsy add max DOL Len
{
    unsigned char retCode;
    unsigned char TVR[5];

    retCode = EmvContact_DOLProcessFunc(type, DOL, DOLLen, DOLData, DOLDataLen,DOLDataMaxLen, tempEmvContact_UnionStruct);

    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

    if(TVR[4] & 0x80)		//if 'Default TDOL used' bit is 1
    {
        EmvContact_DOLProcessFunc(type, DOL, DOLLen, DOLData, DOLDataLen,DOLDataMaxLen, tempEmvContact_UnionStruct);
    }
    return retCode;
}

static const EMVBASETAG DDAmandatoryTag[] =
{
    {EMVTAG_CAPKI},
    {EMVTAG_ICCPKCert},
    {EMVTAG_ICCPKExp},
    {EMVTAG_IPKCert},
    {EMVTAG_IPKExp},
    {"\x00\x00\x00"}
};


unsigned char EmvContact_CheckDataMissDDA(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i;

    i = 0;

    while(1)
    {
        if(memcmp(DDAmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)DDAmandatoryTag[i].Tag) == 0)
        {
            return RLT_ERR_EMV_IccDataMissing;
        }
        i++;
    }

    if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL)
    {
        return RLT_ERR_EMV_IccDataMissing;
    }

    return RLT_EMV_OK;
}

unsigned char EmvContact_GetIPK(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *IPKData, IPKHash[20];
    unsigned char buf[9], bytePAN[8], byteIssuID[8];
    EMVBASE_IPK_RECOVER recovIPK;
    EMVBASETAGCVLITEM *item;
    unsigned char IPKCertexist;
    unsigned char IPKRemexist;
    unsigned char IPKExpexist;
    unsigned char PAN[10];
    unsigned short PANLen;
    unsigned char *IPKRem = NULL;
    int i, index;
    EMVBASE_CAPK_STRUCT *tempcapk;
	unsigned char tempcapkModulLen;
	int ret;
	unsigned int ipkdatalen;

    tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;

	tempcapkModulLen = tempcapk->ModulLen;

    IPKCertexist = 0;

	ipkdatalen = 0;
	item = emvbase_avl_gettagitempointer(EMVTAG_IPKRem);

    if(item != NULL)
    {
        if(item->len)
        {
            ipkdatalen += (item->len);
        }
    }

    item = emvbase_avl_gettagitempointer(EMVTAG_IPKCert);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKCertexist = 1;
        }
    }

    if(IPKCertexist == 0)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(item->len != tempcapk->ModulLen)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

	ipkdatalen += (item->len);
	ipkdatalen += 3;
	IPKData = (unsigned char *)emvbase_malloc(ipkdatalen);
	if(IPKData == NULL)
	{
		return RLT_EMV_ERR;
	}

    memset(IPKData, 0, ipkdatalen);

	EMVBase_RSARecover(tempcapk->Modul, tempcapk->ModulLen, tempcapk->Exponent, tempcapk->ExponentLen, item->data, IPKData);
    EMVBase_FillIPK(IPKData, &recovIPK, tempcapk);

    if(recovIPK.DataTrail != 0xBC)
    {
    	emvbase_free(IPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(recovIPK.DataHead != 0x6A)
    {
    	emvbase_free(IPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(recovIPK.CertFormat != 0x02)
    {
    	emvbase_free(IPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }
    //sign all the correspoding variables to the IPK
    index = 0;

    for(i = 0; i < tempcapk->ModulLen - 22; i++)
    {
        IPKData[i] = IPKData[i + 1];
    }

    index += tempcapk->ModulLen - 22;

    if(recovIPK.IPKLen > tempcapk->ModulLen - 36)
    {
        IPKRemexist = 0;
        item = emvbase_avl_gettagitempointer(EMVTAG_IPKRem);

        if(item != NULL)
        {
            if(item->len)
            {
                IPKRemexist = 1;
            }
        }

        if(IPKRemexist == 0)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20); //20190530_lhd add
        	emvbase_free(IPKData);
            return RLT_ERR_EMV_IccDataMissing;
        }
		IPKRem = (unsigned char *)emvbase_malloc(item->len);
		if(IPKRem == NULL)
		{
			emvbase_free(IPKData);
			return RLT_EMV_ERR;
		}
        memcpy(IPKRem, item->data, item->len);
        memcpy((unsigned char*)&IPKData[index], (unsigned char*)item->data, item->len);
        index += item->len;
    }
    IPKExpexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_IPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKExpexist = 1;
        }
    }

    if(IPKExpexist)
    {
        memcpy((unsigned char*)&IPKData[index], (unsigned char*)item->data, item->len);
        index += item->len;
    }
    memset(IPKHash, 0, 20);

    EMVBase_Hash(IPKData, index, IPKHash);

	emvbase_free(IPKData);

    if(recovIPK.HashInd == 0x01)
    {
        if(memcmp(recovIPK.HashResult, IPKHash, 20))
        {
        	if(IPKRem)
    		{
	        	emvbase_free(IPKRem);
    		}
            return RLT_ERR_EMV_IccDataFormat;
        }
    }
    else
    {
    	if(IPKRem)
		{
        	emvbase_free(IPKRem);
		}
        return RLT_ERR_EMV_IccDataFormat;
    }

    memset(PAN, 0, sizeof(PAN));
    emvbase_avl_gettagvalue_all(EMVTAG_PAN, PAN, &PANLen);

    //verify if leftmost 3-8 digits of PAN matches IssuID
    for(i = 0; i < 4; i++)
    {
        bytePAN[2 * i] = (PAN[i] & 0xF0) >> 4;
        bytePAN[2 * i + 1] = PAN[i] & 0x0F;
        byteIssuID[2 * i] = (recovIPK.IssuID[i] & 0xF0) >> 4;
        byteIssuID[2 * i + 1] = recovIPK.IssuID[i] & 0x0F;
    }

    for(i = 7; i >= 2; i--)
    {
        if(byteIssuID[i] != 0x0F)
        {
            if(memcmp(byteIssuID, bytePAN, i + 1))
            {
	        	if(IPKRem)
	    		{
		        	emvbase_free(IPKRem);
	    		}
                return RLT_ERR_EMV_IccDataFormat;
            }
            else
            {
                break;
            }
        }
    }

    if(i < 2)
	{
    	if(IPKRem)
		{
        	emvbase_free(IPKRem);
		}
        return RLT_ERR_EMV_IccDataFormat;
    }

    //verify if expiredate is later than current date
    if(EMVBase_ExpireDateVerify(recovIPK.ExpireDate) != RLT_EMV_OK)
    {
    	if(IPKRem)
		{
        	emvbase_free(IPKRem);
		}
        return RLT_EMV_ERR;
    }
    //check if IPK is revoked.
    memcpy(buf, tempcapk->RID, 5);
    buf[5] = tempcapk->CAPKI;
    memcpy((unsigned char*)&buf[6], recovIPK.CertSerial, 3);

	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->CAPK);
	tempEmvContact_UnionStruct->EmvTradeParam->CAPK = NULL;

	if(tempEmvContact_UnionStruct->termipkrevokecheck != NULL)
	{
		ret = tempEmvContact_UnionStruct->termipkrevokecheck(buf);//·µ»ØSDK_OK±íÊ¾²»ÊÇ»ØÊÕ¹«Ô¿
	    if(ret != 1)
	    {
	    	if(IPKRem)
			{
	        	emvbase_free(IPKRem);
			}
	        return RLT_EMV_ERR;
	    }
	}

    if(recovIPK.IPKAlgoInd != 0x01)  //other than '01' is not recognised.
    {
    	if(IPKRem)
		{
        	emvbase_free(IPKRem);
		}
        return RLT_ERR_EMV_IccDataFormat;
    }

    tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen = recovIPK.IPKLen;
	if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul == NULL)
	{
		tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = (unsigned char *)emvbase_malloc(recovIPK.IPKLen);
		if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul == NULL)
		{
			if(IPKRem)
			{
	        	emvbase_free(IPKRem);
			}
			return RLT_EMV_ERR;
		}
		memset(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul,0,recovIPK.IPKLen);

}

    if(recovIPK.IPKLen <= tempcapkModulLen - 36)
    {
        memcpy(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, recovIPK.IPKLeft, recovIPK.IPKLen);
    }
    else
    {
        memcpy(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, recovIPK.IPKLeft, tempcapkModulLen - 36);
        memcpy((unsigned char*)&tempEmvContact_UnionStruct->EmvTradeParam->IPKModul[tempcapkModulLen - 36], IPKRem, (recovIPK.IPKLen + 36) - tempcapkModulLen);
    }

	if(IPKRem)
	{
    	emvbase_free(IPKRem);
	}

    return RLT_EMV_OK;
}


unsigned char EmvContact_GetICCPK(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *ICCPKData, ICCPKHash[20];
    unsigned short index = 0;
    EMVBASE_ICCPK_RECOVER recovICCPK;
    EMVBASETAGCVLITEM *item, *item1;
    unsigned char *ICCPKToSign;
    unsigned char testlen;
    unsigned char tmppan1[20], tmppan1len, tmppan2[20], tmppan2len;
    unsigned char IPKExpexist, ICCPKCertexist, ICCPKRemexist, PANexist;
    unsigned char *ICCPKRem = NULL;
    unsigned char CardIdentifyInfo;
    unsigned int ICCPKToSignLen = 0;
	unsigned int fixlen = 0;
	unsigned short offset = 0;


    EMVBase_Trace("chenjun test2 for Segmentation fault\r\n");
	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen)
	{
		if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
		{
			EMVBase_Trace("EmvTradeParam->AuthData is freed\r\n");
			return RLT_EMV_ERR;
		}
	}
    IPKExpexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_IPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKExpexist = 1;
        }
    }

    if(IPKExpexist == 0)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    ICCPKCertexist = 0;
    item1 = emvbase_avl_gettagitempointer(EMVTAG_ICCPKCert);

    if(item1 != NULL)
    {
        if(item1->len)
        {
            ICCPKCertexist = 1;
        }
    }

    if(ICCPKCertexist == 0)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(item1->len != tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen)
    {
        return RLT_EMV_ERR;
    }

	ICCPKData = (unsigned char *)emvbase_malloc(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);

	if(ICCPKData == NULL)
	{
		return RLT_EMV_ERR;
	}
	memset(ICCPKData,0,tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
    EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen, item->data, item->len, item1->data, ICCPKData);

	//emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul);
	//tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = NULL;

    EMVBase_FillICCPK(ICCPKData, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen, &recovICCPK);


    EMVBase_Trace("chenjun test4 for Segmentation fault\r\n");
    if(recovICCPK.DataTrail != 0xBC)
    {
		emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    if(recovICCPK.DataHead != 0x6A)
    {
		emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    if(recovICCPK.CertFormat != 0x04)
    {
		emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    index = 0;

	ICCPKToSignLen = tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22;
	ICCPKToSignLen += 3;		//TAG_ICCPKExp
	ICCPKToSignLen += tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen;

    if(recovICCPK.ICCPKLen > (tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42))
    {
        ICCPKRemexist = 0;
        item1 = emvbase_avl_gettagitempointer(EMVTAG_ICCPKRem);

        if(item1 != NULL)
        {
            if(item1->len)
            {
                ICCPKRemexist = 1;
            }
        }

        if(ICCPKRemexist == 0)
        {
        	emvbase_free(ICCPKData);
			emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);       //ICC data missing
            return RLT_ERR_EMV_IccDataMissing;
        }

		ICCPKRem = (unsigned char *)emvbase_malloc(item1->len);
		if(ICCPKRem == NULL)
		{
			emvbase_free(ICCPKData);
			return RLT_EMV_ERR;
		}

		ICCPKToSignLen += item1->len;
		ICCPKToSign = (unsigned char *)emvbase_malloc(ICCPKToSignLen);

		if(ICCPKToSign == NULL)
		{
			emvbase_free(ICCPKData);
			emvbase_free(ICCPKRem);
			return RLT_EMV_ERR;
		}
		memcpy(ICCPKToSign, (unsigned char*)&ICCPKData[1], tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22);
		index += tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22;

        memcpy(ICCPKRem, item1->data, item1->len);
        memcpy((unsigned char*)&ICCPKToSign[index], (unsigned char*)item1->data, item1->len);
        index += item1->len;
    }
	else
	{
		ICCPKToSign = (unsigned char *)emvbase_malloc(ICCPKToSignLen);
		if(ICCPKToSign == NULL)
		{
			emvbase_free(ICCPKData);
			return RLT_EMV_ERR;
		}

		memcpy(ICCPKToSign, (unsigned char*)&ICCPKData[1], tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22);
		index += tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22;

	}

	emvbase_free(ICCPKData);

    ICCPKCertexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_ICCPKExp);

    if(item != NULL)
    {
        if(item->len)
        {
            ICCPKCertexist = 1;
        }
    }

    if(ICCPKCertexist == 0)
    {
        emvbase_free(ICCPKToSign);
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_ERR_EMV_IccDataFormat;
    }

    memcpy((unsigned char*)&ICCPKToSign[index], (unsigned char*)item->data, item->len);
    index += item->len;
	fixlen = index;
    memcpy((unsigned char*)&ICCPKToSign[index], tempEmvContact_UnionStruct->EmvTradeParam->AuthData, tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen;

	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
	tempEmvContact_UnionStruct->EmvTradeParam->AuthData = NULL;

	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataFixLen != 0xFFFF)
	{
		//memcpy(ICCPKToSign + fixlen + tempEmvContact_UnionStruct->EmvTradeParam->AuthDataFixLen + 2,"\x62\x17\x38\x00\x00\x00\x35\x92",8);
	}

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen) // for tag57 mask
	{
		offset = tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData + fixlen;
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x00, 1, &offset, index, ICCPKToSign, NULL, ICCPKHash);

        EMVBase_Trace("chenjun test5 for Segmentation fault\r\n");
		tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x00, 1, &offset, index, ICCPKToSign, NULL, ICCPKHash);

        EMVBase_Trace("chenjun test6 for Segmentation fault\r\n");
		EMVBase_TraceHex("EmvContact_GetICCPK-HashResult: ", recovICCPK.HashResult, 20);

        EMVBase_Trace("chenjun test7 for Segmentation fault\r\n");
	}
	else
	{
    	EMVBase_Hash(ICCPKToSign, index, ICCPKHash);
	}
	emvbase_free(ICCPKToSign);

    if(recovICCPK.HashInd == 0x01)  //SHA-1 algorithm
    {
        if(memcmp(recovICCPK.HashResult, ICCPKHash, 20))
        {
			if(ICCPKRem)
			{
				emvbase_free(ICCPKRem);
			}
            return RLT_EMV_ERR;
        }
    }
    else
    {
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_EMV_ERR;
    }


    PANexist = 0;
    item1 = emvbase_avl_gettagitempointer(EMVTAG_PAN);

    if(item1 != NULL)
    {
        if(item1->len)
        {
            PANexist = 1;
        }
    }

    if(PANexist == 0)
    {
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_ERR_EMV_IccDataFormat;
    }
    memset(tmppan1, 0, sizeof(tmppan1));
    EMVBaseBcdToAsc(tmppan1, recovICCPK.AppPAN, 10);
    tmppan1len = 20;

    for(testlen = 0; testlen < 20; testlen++)
    {
        if((tmppan1[testlen] == 'F') || (tmppan1[testlen] == 'f'))
        {
            tmppan1len = testlen;
            break;
        }
    }

    memset(tmppan2, 0, sizeof(tmppan2));
    EMVBaseBcdToAsc(tmppan2, item1->data, item1->len);
    tmppan2len = (item1->len) << 1;

    for(testlen = 0; testlen < tmppan2len; testlen++)
    {
        if((tmppan1[testlen] == 'F') || (tmppan2[testlen] == 'f'))
        {
            tmppan2len = testlen;
            break;
        }
    }

    if(tmppan1len != tmppan2len)
    {
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_EMV_ERR;
    }

    if(memcmp(tmppan1, tmppan2, tmppan1len))
    {
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_EMV_ERR;
    }

    //verify expiredate is later than current date
    if(EMVBase_ExpireDateVerify(recovICCPK.ExpireDate) != RLT_EMV_OK)
    {
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_EMV_ERR;
    }

    if(recovICCPK.ICCPKAlgoInd != 0x01)  //other than '01' is not recognised.
    {
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
        return RLT_EMV_ERR;
    }

    tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen = recovICCPK.ICCPKLen;

	if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul == NULL)
	{
		tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul = (unsigned char  *)emvbase_malloc(recovICCPK.ICCPKLen);

}

	if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul == NULL)
	{
		if(ICCPKRem)
		{
			emvbase_free(ICCPKRem);
		}
		return RLT_EMV_ERR;
	}

	memset(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul,0,recovICCPK.ICCPKLen);

    if(recovICCPK.ICCPKLen <= (tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42))
    {
        memcpy(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, recovICCPK.ICCPKLeft, recovICCPK.ICCPKLen);
    }
    else
    {
        memcpy(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, recovICCPK.ICCPKLeft, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42);
        memcpy((unsigned char*)&tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul[tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42], ICCPKRem, recovICCPK.ICCPKLen - tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen + 42);
    }

	if(ICCPKRem)
	{
		emvbase_free(ICCPKRem);
	}

    return RLT_EMV_OK;
}

unsigned char EmvContact_initDynamicAuth(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	EMVBASE_CAPK_STRUCT *tempcapk;

	tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;

	if(tempcapk == NULL)
	{
		return RLT_EMV_ERR;
	}

    if(EmvContact_CheckDataMissDDA(tempEmvContact_UnionStruct) != RLT_EMV_OK)
    {

        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);       //ICC data missing

        return RLT_ERR_EMV_IccDataMissing;
    }

    if(tempcapk->ModulLen == 0)
    {
            return RLT_EMV_ERR;
	}

    if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen == 0)
    {
        if(EmvContact_GetIPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)                  //retrieval of the issuer public key modulus
        {
            return RLT_EMV_ERR;
        }

    }
    EMVBase_Trace("chenjun test1 for Segmentation fault\r\n");
    if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen == 0)
    {
        if(EmvContact_GetICCPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)                //retrieval of the ICC public key
        {

            return RLT_EMV_ERR;
        }

        EMVBase_Trace("chenjun test3 for Segmentation fault\r\n");
    }

    return RLT_EMV_OK;
}


unsigned char EmvContact_FormDynSignGenerate(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char *TempDDOLData;
    unsigned short TempDDOLDataLen;
    unsigned char bTermDDOL, bDDOL;
    EMVBASETAGCVLITEM *ddolitem = NULL;
    unsigned char *DDOL;
    unsigned short DDOLLen;


    ddolitem = emvbase_avl_gettagitemandstatus(EMVTAG_DDOL, &bDDOL);

    if(bDDOL == 0)
    {
        ddolitem = emvbase_avl_gettagitemandstatus(EMVTAG_TermDDOL, &bTermDDOL);

        if(bTermDDOL == 0)
        {
            return RLT_ERR_EMV_TermDataMissing;
        }
    }
    DDOL = (unsigned char *)emvbase_malloc(300);
    TempDDOLData = (unsigned char *)emvbase_malloc(300);
    memset(DDOL, 0, 300);
    DDOLLen = (ddolitem->len > 255) ? 255 : (ddolitem->len);
    memcpy(DDOL, ddolitem->data, DDOLLen);


    if(EmvContact_DOLProcess(typeDDOL, DDOL, DDOLLen, TempDDOLData, &TempDDOLDataLen, 255, tempEmvContact_UnionStruct) != RLT_EMV_OK)//20121221 lsy
    {
    	EMVBase_Trace("EMV-error: DDOL data error\r\n");
        emvbase_free(DDOL);
        emvbase_free(TempDDOLData);
        return RLT_EMV_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_DDOLRelatedData, TempDDOLData, TempDDOLDataLen);

    memcpy(apdu_s->Command, "\x00\x88\x00\x00", 4);
    apdu_s->Lc = TempDDOLDataLen;
    memcpy(apdu_s->DataIn, TempDDOLData, TempDDOLDataLen);
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;

    emvbase_free(DDOL);
    emvbase_free(TempDDOLData);
    return RLT_EMV_OK;
}


unsigned char EmvContact_DynSignGenerate(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
    unsigned char k;
    unsigned short index, indexTemp, len, lenTemp;


    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)   //Internal authenticaton OK
    {
        if(apdu_r->LenOut == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;		//added on 2002.09.29. for JCB Card AP 37.
        }
        index = 0;

        if(apdu_r->DataOut[index] == 0x80)			//primitive data
        {
            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            //2CL.035.00,if data format parse error,should terminate transaction.
            if(index + len != apdu_r->LenOut)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(len == 0)               //sxl EMV4.1e
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            emvbase_avl_createsettagvalue(EMVTAG_SignDynAppData, &apdu_r->DataOut[index], len);
        }
        else if(apdu_r->DataOut[index] == 0x77)      //TLV coded data,assume that SDAD is at first.
        {
            index++;

            if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenTemp))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            indexTemp = index;

            while(index < indexTemp + lenTemp)
            {
                if(index >= apdu_r->LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00) { index++; continue; }

                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x4B", 2))
                {                 //tag of Signed Dyn App Data
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)                       //sxl emv4.1e
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_SignDynAppData, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //20170509 add for 4.3f
                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x24", 2))
                {                 //tag of Payment Account Reference
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)                       //sxl emv4.1e
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_PayAccountRefer, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //end
                //20190603 lhd add for 4.3h
                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x19", 2))
                {
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)                       //sxl emv4.1e
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_TOKENREQUESTID, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //end
                //20200526  for 4.3i
                else if(!memcmp(&apdu_r->DataOut[index], "\x9F\x25", 2))
                {
                    index += 2;

                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len == 0)
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    emvbase_avl_createsettagvalue(EMVTAG_PANLast4Digits, &apdu_r->DataOut[index], len);
                    index += len;
                }
                //end
                else                //unknown data,out of this command.
                {
                    k = apdu_r->DataOut[index];

                    if((k & 0x1F) == 0x1F)
                    {
                        index++;
                    }
                    index++;
                    if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    index += len;
                }
            }

            if(index != indexTemp + lenTemp)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
        else
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData) == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        return RLT_EMV_OK;
    }
    else{ return RLT_ERR_EMV_IccReturn; }
}


unsigned char EmvContact_FillSDA(unsigned char* SDAData, SIGN_DYN_APPDATA_RECOVER* recovSDA, unsigned char tempICCPKModulLen)
{
    unsigned char LDD, i;

    recovSDA->DataHead = SDAData[0];
    recovSDA->DataFormat = SDAData[1];
    recovSDA->HashInd = SDAData[2];
    recovSDA->ICCDynDataLen = SDAData[3];
    LDD = SDAData[3];

    if(LDD > tempICCPKModulLen - 25)
    {
        return RLT_EMV_ERR;
    }
    memcpy(recovSDA->ICCDynData, (unsigned char*)&SDAData[4], LDD);
    memcpy(recovSDA->PadPattern, (unsigned char*)&SDAData[LDD + 4], tempICCPKModulLen - LDD - 25);

    for(i = 0; i < 20; i++)
    {
        recovSDA->HashResult[i] = SDAData[tempICCPKModulLen - 21 + i];
    }

    recovSDA->DataTrail = SDAData[tempICCPKModulLen - 1];
    return RLT_EMV_OK;
}


unsigned char EmvContact_DynSignVerify(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *SDAToSign, SDAHash[20], *SDAData;
    unsigned int index;
    SIGN_DYN_APPDATA_RECOVER *recovSDA;
    unsigned char SignDynAppDatabExist, ICCPKExpbExist;
    EMVBASETAGCVLITEM *SignDynAppDataitem = NULL, *ICCPKExpitem = NULL, *DDOLDataitem = NULL;
	unsigned short offset = 0;
	EMVBASETAGCVLITEM *item = NULL;
	EMVBASETAGCVLITEM *item2 = NULL;


    SignDynAppDataitem = emvbase_avl_gettagitemandstatus(EMVTAG_SignDynAppData, &SignDynAppDatabExist);
    ICCPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPKExp, &ICCPKExpbExist);

    if(SignDynAppDatabExist == 0 || ICCPKExpbExist == 0)
    {
        return RLT_EMV_ERR;
    }

    if(SignDynAppDataitem->len != tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen)
    {
        return RLT_EMV_ERR;
    }

    SDAData = (unsigned char *)emvbase_malloc(248);
    EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen, ICCPKExpitem->data, ICCPKExpitem->len, SignDynAppDataitem->data, SDAData);

    recovSDA = (SIGN_DYN_APPDATA_RECOVER *)emvbase_malloc(sizeof(SIGN_DYN_APPDATA_RECOVER));

    if(EmvContact_FillSDA(SDAData, recovSDA, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen) != RLT_EMV_OK)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataTrail != 0xBC)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataHead != 0x6A)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataFormat != 0x05)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }
    DDOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_DDOLRelatedData);

    if(DDOLDataitem == NULL)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
        return RLT_EMV_ERR;
    }
    index = 0;
    SDAToSign = (unsigned char *)emvbase_malloc(512);
    memcpy(SDAToSign, (unsigned char*)&SDAData[1], tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen - 22);

    emvbase_free(SDAData);

    index += tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen - 22;
	offset = index;

    if(DDOLDataitem->data != NULL)
    {
        memcpy((unsigned char*)&SDAToSign[index], DDOLDataitem->data, DDOLDataitem->len);
    }
    index += DDOLDataitem->len;

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InDDOLData) // for tag57 mask
	{
		offset += tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInDDOLData;
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x00, 1, &offset, index, SDAToSign, NULL, SDAHash);
	    tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x00, 1, &offset, index, SDAToSign, NULL, SDAHash);
	}
	else
	{
    	EMVBase_Hash(SDAToSign, index, SDAHash);
	}

    emvbase_free(SDAToSign);

    if(recovSDA->HashInd == 0x01)  //SHA-1 algorithm
    {
        if(memcmp(recovSDA->HashResult, SDAHash, 20))
        {
            emvbase_free(recovSDA);
            return RLT_EMV_ERR;
        }
    }
    else
    {
        emvbase_free(recovSDA);
        return RLT_EMV_ERR;
    }

	if(tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable)
	{
		if(memcmp(recovSDA->ICCDynData + (recovSDA->ICCDynDataLen-20), tempEmvContact_UnionStruct->EmvTradeParam->DSDHashValue, 20))
	    {
	        emvbase_free(recovSDA);
	        return RLT_EMV_ERR;
	    }
	}

	if(tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable || tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable)
	{
		item = emvbase_avl_gettagitempointer(EMVTAG_CardID);
		if(NULL == item)
		{
			emvbase_free(recovSDA);
	        return RLT_EMV_ERR;
		}

		item2 = emvbase_avl_gettagitempointer(EMVTAG_DPASCardFeatureDescriptor);
		if(memcmp(item2->data + 3, item->data, item->len))
	    {
	        emvbase_free(recovSDA);
	        return RLT_EMV_ERR;
	    }
	}

    emvbase_avl_createsettagvalue(EMVTAG_ICCDynNum, &recovSDA->ICCDynData[1], recovSDA->ICCDynData[0]);

    emvbase_free(recovSDA);
    return RLT_EMV_OK;
}

static const EMVBASETAG SDAmandatoryTag[] =
{
    {EMVTAG_CAPKI},
    {EMVTAG_IPKCert},
    {EMVTAG_IPKExp},
    {EMVTAG_SignStatAppData},
    {"\x00\x00\x00"}
};

unsigned char EmvContact_CheckDataMissSDA(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i;

    i = 0;

    while(1)
    {
        if(memcmp(SDAmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)SDAmandatoryTag[i].Tag) == 0)
        {
            return RLT_ERR_EMV_IccDataMissing;
        }
        i++;
    }

    return RLT_EMV_OK;
}

void EmvContact_FillSSA(unsigned char* SSAData, SIGN_STAT_APPDATA_RECOVER* recovSSA, unsigned char tempIPKModulLen)
{
    unsigned char i;

    recovSSA->DataHead = SSAData[0];
    recovSSA->DataFormat = SSAData[1];
    recovSSA->HashInd = SSAData[2];
    memcpy(recovSSA->DataAuthCode, (unsigned char*)&SSAData[3], 2);
    memcpy(recovSSA->PadPattern, (unsigned char*)&SSAData[5], tempIPKModulLen - 26);

    for(i = 0; i < 20; i++)
    {
        recovSSA->HashResult[i] = SSAData[tempIPKModulLen - 21 + i];
    }

    recovSSA->DataTrail = SSAData[tempIPKModulLen - 1];
}


unsigned char EmvContact_SDVerify(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char SSAHash[20];
    unsigned short index = 0;
    unsigned char *SSAData;
    SIGN_STAT_APPDATA_RECOVER *recovSSA;
    unsigned char SignStatAppDatabExist, IPKExpbExist;
    EMVBASETAGCVLITEM *SignStatAppDataitem = NULL, *IPKExpitem = NULL;
	EMVBASETAGCVLITEM *item = NULL;
	EMVBASETAGCVLITEM *item2 = NULL;

    unsigned char *SSAToSign;
	unsigned short fixlen = 0;
	unsigned short offset = 0;


    SignStatAppDataitem = emvbase_avl_gettagitemandstatus(EMVTAG_SignStatAppData, &SignStatAppDatabExist);
    IPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_IPKExp, &IPKExpbExist);

    if(SignStatAppDatabExist == 0 || IPKExpbExist == 0)
    {
        return RLT_EMV_ERR;
    }

    if(SignStatAppDataitem->len != tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen)
    {
        return RLT_EMV_ERR;
    }

    SSAData = (unsigned char *)emvbase_malloc(248);
    memset(SSAData, 0, 248);
    EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen, IPKExpitem->data, IPKExpitem->len, SignStatAppDataitem->data, SSAData);

    recovSSA = (SIGN_STAT_APPDATA_RECOVER *)emvbase_malloc(sizeof(SIGN_STAT_APPDATA_RECOVER));
    EmvContact_FillSSA(SSAData, recovSSA, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);


    if(recovSSA->DataTrail != 0xBC)
    {
        emvbase_free(SSAData);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

    if(recovSSA->DataHead != 0x6A)
    {
        emvbase_free(SSAData);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

    if(recovSSA->DataFormat != 0x03)
    {
        emvbase_free(SSAData);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }
    SSAToSign = (unsigned char *)emvbase_malloc(2048 + 512);


    index = 0;
    memcpy(SSAToSign, (unsigned char*)&SSAData[1], tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22);
    index += tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22;

    emvbase_free(SSAData);

    if((index + tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen) > (2048 + 512))
    {
        emvbase_free(SSAToSign);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

	fixlen = index;

	EMVBase_TraceHex("EmvContact_SDVerify-AuthData: ", tempEmvContact_UnionStruct->EmvTradeParam->AuthData, tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);

	memcpy((unsigned char*)&SSAToSign[index], tempEmvContact_UnionStruct->EmvTradeParam->AuthData, tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen;

	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataFixLen != 0xFFFF)
	{
		//memcpy(SSAToSign + fixlen + tempEmvContact_UnionStruct->EmvTradeParam->AuthDataFixLen + 2,"\x47\x61\x73\x90\x01\x01\x00\x10",8);
	}

	EMVBase_Trace("EmvContact_SDVerify--bTrack2DataMasked: %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked);
	EMVBase_Trace("EmvContact_SDVerify--Track2DataLen: %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen);
	EMVBase_Trace("EmvContact_SDVerify--Track2OffsetInAuthData: %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData);

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen) // for tag57 mask
	{
		EMVBase_Trace("EmvContact_SDVerify--index: %d\r\n", index);
		EMVBase_TraceHex("EmvContact_SDVerify-SSAToSign: ", SSAToSign, index);

		offset = tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData + fixlen;
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x00, 1, &offset, index, SSAToSign, NULL, SSAHash);

	    tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x00, 1, &offset, index, SSAToSign, NULL, SSAHash);
		EMVBase_TraceHex("EmvContact_GetICCPK-HashResult: ", recovSSA->HashResult, 20);
	}
	else
	{
		EMVBase_Trace("EmvContact_SDVerify--line: %d\r\n", __LINE__);
    	EMVBase_Hash(SSAToSign, index, SSAHash);
	}

    if(recovSSA->HashInd == 0x01)  //SHA-1 algorithm
    {
        if(memcmp(recovSSA->HashResult, SSAHash, 20))
        {
            emvbase_free(SSAToSign);
            emvbase_free(recovSSA);
            return RLT_EMV_ERR;
        }
    }
    else
    {
        emvbase_free(SSAToSign);
        emvbase_free(recovSSA);
        return RLT_EMV_ERR;
    }

	if(tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable || tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable)
	{
		item = emvbase_avl_gettagitempointer(EMVTAG_CardID);
		if(NULL == item)
		{
			emvbase_free(SSAToSign);
    		emvbase_free(recovSSA);
			EMVBase_Trace("EmvContact_SDVerify--line: %d\r\n", __LINE__);
	        return RLT_EMV_ERR;
		}

		item2 = emvbase_avl_gettagitempointer(EMVTAG_DPASCardFeatureDescriptor);
		if(NULL == item2)
		{
			emvbase_free(SSAToSign);
    		emvbase_free(recovSSA);
			EMVBase_Trace("EmvContact_SDVerify--line: %d\r\n", __LINE__);
	        return RLT_EMV_ERR;
		}

		if(memcmp(item2->data + 3, item->data, item->len))
	    {
	    	emvbase_free(SSAToSign);
    		emvbase_free(recovSSA);
			EMVBase_Trace("EmvContact_SDVerify--line: %d\r\n", __LINE__);
	        return RLT_EMV_ERR;
	    }
	}

    emvbase_avl_createsettagvalue(EMVTAG_DataAuthCode, recovSSA->DataAuthCode, 2);


    emvbase_free(SSAToSign);
    emvbase_free(recovSSA);
    return RLT_EMV_OK;
}


unsigned char EmvContact_StaticAuth(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_CAPK_STRUCT *tempcapk;

    tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;

	if(tempcapk == NULL)
	{
		return RLT_EMV_ERR;
	}


    if(EmvContact_CheckDataMissSDA(tempEmvContact_UnionStruct) != RLT_EMV_OK)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);       //ICC data missing

        return RLT_ERR_EMV_IccDataMissing;
    }

    if(tempcapk->ModulLen == 0)
    {
            return RLT_EMV_ERR;
    }

    if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen == 0)
    {
        if(EmvContact_GetIPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)				//retrieval of the issuer public key modulus
        {
            return RLT_EMV_ERR;
        }
    }

    if(EmvContact_SDVerify(tempEmvContact_UnionStruct) != RLT_EMV_OK)				//Verification of the Signed Static Application Data by the terminal
    {
        return RLT_EMV_ERR;
    }

    return RLT_EMV_OK;
}

unsigned char EmvContact_DataAuth(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode = RLT_EMV_OK;
    unsigned char AIP[2];
    unsigned char TermCapab[3];
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char ODAFail = 0;

    memset(AIP, 0, sizeof(AIP));
    memset(TermCapab, 0, sizeof(TermCapab));

    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

	EMVBase_TraceHex("AIP",AIP,2);
	EMVBase_TraceHex("TermCapab 9F33",TermCapab,3);

    if((AIP[0] & 0x01) && (TermCapab[2] & 0x08))
    {
    	EMVBase_Trace("comes to CDA\r\n");
        if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL || tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData)          //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
        {
            ODAFail = 1;
        }
        else
        {
            if(EmvContact_initDynamicAuth(tempEmvContact_UnionStruct) != RLT_EMV_OK)
            {
                ODAFail = 1;
            }
        }

        if(ODAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);      //Offline Combined DDA/AC Generation failed
            emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);      //set bit 'Offline Data Authentication was performed' bit 1
        }
        return RLT_EMV_OK;
    }
    else if((AIP[0] & 0x20) && (TermCapab[2] & 0x40))           //ICC and terminal support dynamic data auth.
    {
    	EMVBase_Trace("comes to DDA\r\n");
        if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL || tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData)          //SDA_TL.tag is not EMV; represents data not available at the moment; refers to constructed Data Object
        {
            EMVBase_Trace("is not EMV SDA Tag List\r\n");
            ODAFail = 1;
        }
        else
        {
            ODAFail = 1;

            retCode = EmvContact_initDynamicAuth(tempEmvContact_UnionStruct);

            if(retCode == RLT_EMV_OK)
            {
                retCode = EmvContact_FormDynSignGenerate(tempEmvContact_UnionStruct, &apdu_s);
                EMVBase_Trace("EmvContact_FormDynSignGenerate ret = %d\r\n", retCode);

                if(retCode == RLT_EMV_OK)
                {
                	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InDDOLData)
                	{
                	    EMVBase_Trace("DDA with track2data\r\n");
                    	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInDDOLData);
					}
					else
					{
					    EMVBase_Trace("DDA without track2data\r\n");
                    	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
					}

                    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
                    {
                        retCode = RLT_ERR_EMV_APDUTIMEOUT;
                    }

                    if(retCode == RLT_EMV_OK)
                    {
                        retCode = EmvContact_DynSignGenerate(tempEmvContact_UnionStruct, &apdu_r);

                        if(retCode == RLT_EMV_OK)
                        {
                            retCode = EmvContact_DynSignVerify(tempEmvContact_UnionStruct);

                            if(retCode == RLT_EMV_OK)
                            {
                                ODAFail = 0;
                            }
                        }
                    }
                }
            }

            if(retCode == RLT_ERR_EMV_APDUTIMEOUT || retCode == RLT_ERR_EMV_IccDataFormat || retCode == RLT_ERR_EMV_IccReturn)
            {
                return retCode;
            }
        }

        if(ODAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x08);
        }
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);      //set bit 'Offline Data Authentication was performed' bit 1
        retCode = RLT_EMV_OK;
    }
    else if((AIP[0] & 0x40) && (TermCapab[2] & 0x80)) //ICC and terminal support static data auth.
    {
    	EMVBase_Trace("comes to SDA\r\n");
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x02);

        if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL || tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData)          //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object
        {
            ODAFail = 1;
        }
        else
        {
            if(EmvContact_StaticAuth(tempEmvContact_UnionStruct) != RLT_EMV_OK)               //Static data authentication
            {
                ODAFail = 1;
            }
        }

        if(ODAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x40);
        }
        else
        {
            emvbase_avl_setvalue_and(EMVTAG_TVR, 0, 0xBF);
        }
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);      //set bit 'Offline Data Authentication was performed' bit 1
        retCode = RLT_EMV_OK;
    }
    else    //all dynamic and static authentication and DDA/AC are not supported.
    {
    	EMVBase_Trace("ODA fail\r\n");
        //The bit should be set to 1 according to test script 2CI.023.00
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x80);                              //Offline Data Authentication not performed
        emvbase_avl_setvalue_and(EMVTAG_TSI, 0, 0x7F);                             //set bit 'Offline data authentication was performed' bit 0.
        retCode = RLT_EMV_OK;
    }
    return retCode;
}

static const EMVBASETAG SMDDAmandatoryTag[] =
{
    {EMVTAG_CAPKI},
    {EMVTAG_ICCPKCert},
    {EMVTAG_IPKCert},
    {"\x00\x00\x00"}
};

unsigned char EmvContact_SM_CheckDataMissDDA(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i;

    i = 0;

    while(1)
    {
        if(memcmp(SMDDAmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)SMDDAmandatoryTag[i].Tag) == 0)
        {
            return RLT_ERR_EMV_IccDataMissing;
        }
        i++;
    }

    if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL)
    {
        return RLT_ERR_EMV_IccDataMissing;
    }

    return RLT_EMV_OK;
}

unsigned char EmvContact_SM_FillIPK(SM_IPK_CRTFCT *IPKCertfct, unsigned char *IPKCert, unsigned char IPKCertLen, EMVBASE_CAPK_STRUCT *tempcapk)
{
    unsigned char i = 0;

	if(IPKCertLen != 142)
	{
        return RLT_EMV_ERR;
    }

    IPKCertfct->CertFormat = IPKCert[i++];
    memcpy(IPKCertfct->IssuID, (unsigned char*)&IPKCert[i], 4);
    i += 4;
    memcpy(IPKCertfct->ExpireDate, (unsigned char*)&IPKCert[i], 2);
    i += 2;
    memcpy(IPKCertfct->CertSerial, (unsigned char*)&IPKCert[i], 3);
    i += 3;
    IPKCertfct->IPKSIGNAlgoInd = IPKCert[i++];
    IPKCertfct->IPKEncrptAlgoInd = IPKCert[i++];
    IPKCertfct->IPKParamInd = IPKCert[i++];

    IPKCertfct->IPKLen = IPKCert[i++];

    memcpy((unsigned char*)&IPKCertfct->IPK, (unsigned char*)&IPKCert[i], IPKCertfct->IPKLen);
    i += IPKCertfct->IPKLen;
    memcpy((unsigned char*)&IPKCertfct->DGTLSGNTR, (unsigned char*)&IPKCert[i], tempcapk->ModulLen);
    i += tempcapk->ModulLen;


    if(i != IPKCertLen || IPKCertfct->IPKLen != 64)
    {
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}

unsigned char EmvContact_SM_GetIPK(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *IPKdata;
    unsigned short ipkdatalen;
    unsigned char buf[9], bytePAN[8], byteIssuID[8];
    SM_IPK_CRTFCT IPKCertfct;
    int i;
    EMVBASETAGCVLITEM *item;
    unsigned char IPKCertexist;
    EMVBASE_CAPK_STRUCT *tempcapk;
    unsigned char PAN[10];
    unsigned short PANLen;
	unsigned char ret;

    tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;

    IPKCertexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_IPKCert);

    if(item != NULL)
    {
        if(item->len)
        {
            IPKCertexist = 1;
        }
    }

    if(IPKCertexist == 0)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    memset((unsigned char *)&IPKCertfct, 0, sizeof(SM_IPK_CRTFCT));

    if(EmvContact_SM_FillIPK(&IPKCertfct, item->data, item->len, tempcapk))		//Êý¾Ý¸ñÊ½×ª»»
    {
        return RLT_EMV_ERR;
    }

    IPKdata = (unsigned char *)emvbase_malloc(78);
    memset(IPKdata, 0, 78);
    ipkdatalen = 0;


    if(IPKCertfct.CertFormat != 0x12)		//¼ì²éÖ¤Êé¸ñÊ½
    {
        emvbase_free(IPKdata);
        return RLT_EMV_ERR;
    }
    IPKdata[ipkdatalen++] = IPKCertfct.CertFormat;

    //verify if leftmost 3-8 digits of PAN matches IssuID
    memset(PAN, 0, sizeof(PAN));
    emvbase_avl_gettagvalue_all(EMVTAG_PAN, PAN, &PANLen);

    for(i = 0; i < 4; i++)
    {
        bytePAN[2 * i] = (PAN[i] & 0xF0) >> 4;
        bytePAN[2 * i + 1] = PAN[i] & 0x0F;
        byteIssuID[2 * i] = (IPKCertfct.IssuID[i] & 0xF0) >> 4;
        byteIssuID[2 * i + 1] = IPKCertfct.IssuID[i] & 0x0F;
    }

    for(i = 7; i >= 2; i--)
    {
        if(byteIssuID[i] != 0x0F)
        {
            if(memcmp(byteIssuID, bytePAN, i + 1))
            {
                emvbase_free(IPKdata);
                return RLT_ERR_EMV_IccDataFormat;
            }
            else
            {
                break;
            }
        }
    }

    if(i < 2)
    {
        emvbase_free(IPKdata);
        return RLT_ERR_EMV_IccDataFormat;
    }
    memcpy(&IPKdata[ipkdatalen], (unsigned char *)&IPKCertfct.IssuID[0], 4);
    ipkdatalen += 4;

    //verify if expiredate is later than current date
    if(EMVBase_ExpireDateVerify(IPKCertfct.ExpireDate) != RLT_EMV_OK)
    {
        emvbase_free(IPKdata);
        return RLT_EMV_ERR;
    }
    memcpy(&IPKdata[ipkdatalen], (unsigned char *)&IPKCertfct.ExpireDate[0], 2);
    ipkdatalen += 2;

	//check if IPK is revoked.
    memcpy(buf, tempcapk->RID, 5);
    buf[5] = tempcapk->CAPKI;
    memcpy((unsigned char*)&buf[6], IPKCertfct.CertSerial, 3);

	if(tempEmvContact_UnionStruct->termipkrevokecheck != NULL)
	{
		ret = tempEmvContact_UnionStruct->termipkrevokecheck(buf);//·µ»ØSDK_OK±íÊ¾²»ÊÇ»ØÊÕ¹«Ô¿
	    if(ret != 1)
	    {
	    	emvbase_free(IPKdata);
	        return RLT_EMV_ERR;
	    }
	}

    memcpy(&IPKdata[ipkdatalen], (unsigned char *)&IPKCertfct.CertSerial[0], 3);
    ipkdatalen += 3;

    if(IPKCertfct.IPKSIGNAlgoInd != 0x04)  //other than '01' is not recognised.
    {
		emvbase_free(IPKdata);
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(IPKCertfct.IPKParamInd != 0x11)  //other than '01' is not recognised.
    {
		emvbase_free(IPKdata);
		return RLT_ERR_EMV_IccDataFormat;

    }

    IPKdata[ipkdatalen++] = IPKCertfct.IPKSIGNAlgoInd;
    IPKdata[ipkdatalen++] = IPKCertfct.IPKEncrptAlgoInd;
    IPKdata[ipkdatalen++] = IPKCertfct.IPKParamInd;
    IPKdata[ipkdatalen++] = IPKCertfct.IPKLen;
    memcpy(&IPKdata[ipkdatalen], IPKCertfct.IPK, IPKCertfct.IPKLen);
    ipkdatalen += IPKCertfct.IPKLen;

//	ret = EMVBase_SM_SignatureVerf(IPKCertfct.DGTLSGNTR, IPKdata, ipkdatalen, tempcapk->Modul, tempcapk->ModulLen);

	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->CAPK);
	tempEmvContact_UnionStruct->EmvTradeParam->CAPK = NULL;
	emvbase_free(IPKdata);

	if(ret != RLT_EMV_OK)
	{
		return RLT_EMV_ERR;
	}

	if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul == NULL)
	{
		tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = (unsigned char *)emvbase_malloc(IPKCertfct.IPKLen);
		if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul == NULL)
		{
			return RLT_EMV_ERR;
		}
		memset(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul,0,IPKCertfct.IPKLen);

}

    tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen = IPKCertfct.IPKLen;
    memcpy(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, IPKCertfct.IPK, IPKCertfct.IPKLen);

    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_FillICCPK(SM_ICCPK_CRTFCT *ICCPKCertfct, unsigned char *ICCPKCert, unsigned char ICCPKCertLen, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i = 0;

	if(ICCPKCertLen != 148)
	{
        return RLT_EMV_ERR;
    }

    ICCPKCertfct->CertFormat = ICCPKCert[i++];
    memcpy(ICCPKCertfct->AppPAN, (unsigned char*)&ICCPKCert[i], 10);
    i += 10;
    memcpy(ICCPKCertfct->ExpireDate, (unsigned char*)&ICCPKCert[i], 2);
    i += 2;
    memcpy(ICCPKCertfct->CertSerial, (unsigned char*)&ICCPKCert[i], 3);
    i += 3;
    ICCPKCertfct->ICCPKSIGNAlgoInd = ICCPKCert[i++];
    ICCPKCertfct->ICCPKEncrptAlgoInd = ICCPKCert[i++];
    ICCPKCertfct->ICCPKParamInd = ICCPKCert[i++];
    ICCPKCertfct->ICCPKLen = ICCPKCert[i++];

    memcpy((unsigned char*)&ICCPKCertfct->ICCPK, (unsigned char*)&ICCPKCert[i], ICCPKCertfct->ICCPKLen);
    i += ICCPKCertfct->ICCPKLen;
    memcpy((unsigned char*)&ICCPKCertfct->DGTLSGNTR, (unsigned char*)&ICCPKCert[i], tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
    i += tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen;

    if(i != ICCPKCertLen || ICCPKCertfct->ICCPKLen != 64)
    {
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}

unsigned char EmvContact_SM_GetICCPK(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *ICCPKData;
    unsigned short iccpkdatalen;
    SM_ICCPK_CRTFCT ICCPKCertfct;
    unsigned char ICCPKCertexist;
    EMVBASETAGCVLITEM *item, *item1;
    unsigned char tmppan1[50], tmppan1len, tmppan2[50], tmppan2len;
    unsigned char testlen;
    unsigned char PANexist;
    unsigned char ret;

	unsigned char hashValue[32] = {0};
	unsigned short fixlen = 0;
	unsigned short offset = 0;


	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen)
	{
		if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
		{
			EMVBase_Trace("EmvTradeParam->AuthData is freed\r\n");
			return RLT_EMV_ERR;
		}
	}

    ICCPKCertexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_ICCPKCert);

    if(item != NULL)
    {
        if(item->len)
        {
            ICCPKCertexist = 1;
        }
    }

    if(ICCPKCertexist == 0)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    memset((unsigned char *)&ICCPKCertfct, 0, sizeof(SM_IPK_CRTFCT));

    if(EmvContact_SM_FillICCPK(&ICCPKCertfct, item->data, item->len, tempEmvContact_UnionStruct))
    {
        return RLT_EMV_ERR;
    }

    ICCPKData = (unsigned char *)emvbase_malloc(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + 84);
    memset(ICCPKData, 0, (tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + 84));
    iccpkdatalen = 0;

    if(ICCPKCertfct.CertFormat != 0x14)
    {
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }
    ICCPKData[iccpkdatalen++] = ICCPKCertfct.CertFormat;

    PANexist = 0;
    item1 = emvbase_avl_gettagitempointer(EMVTAG_PAN);

    if(item1 != NULL)
    {
        if(item1->len)
        {
            PANexist = 1;
        }
    }

    if(PANexist == 0)
    {
        emvbase_free(ICCPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }

    memset(tmppan1, 0, sizeof(tmppan1));
    EMVBaseBcdToAsc(tmppan1, ICCPKCertfct.AppPAN, 10);
    tmppan1len = 20;

    for(testlen = 0; testlen < 20; testlen++)
    {
        if((tmppan1[testlen] == 'F') || (tmppan1[testlen] == 'f'))
        {
            tmppan1len = testlen;
            break;
        }
    }

    memset(tmppan2, 0, sizeof(tmppan2));
    EMVBaseBcdToAsc(tmppan2, item1->data, item1->len);
    tmppan2len = (item1->len) << 1;

    for(testlen = 0; testlen < tmppan2len; testlen++)
    {
        if((tmppan1[testlen] == 'F') || (tmppan2[testlen] == 'f'))
        {
            tmppan2len = testlen;
            break;
        }
    }

    if(tmppan1len != tmppan2len)
    {
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    if(memcmp(tmppan1, tmppan2, tmppan1len))
    {
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }

    memcpy(&ICCPKData[iccpkdatalen], (unsigned char *)&ICCPKCertfct.AppPAN[0], 10);
    iccpkdatalen += 10;

    //verify if expiredate is later than current date
    if(EMVBase_ExpireDateVerify(ICCPKCertfct.ExpireDate) != RLT_EMV_OK)
    {
        emvbase_free(ICCPKData);
        return RLT_EMV_ERR;
    }
    memcpy(&ICCPKData[iccpkdatalen], (unsigned char *)&ICCPKCertfct.ExpireDate[0], 2);
    iccpkdatalen += 2;

    memcpy(&ICCPKData[iccpkdatalen], (unsigned char *)&ICCPKCertfct.CertSerial[0], 3);
    iccpkdatalen += 3;


    //Ëã·¨Ê¶±ð
    if(ICCPKCertfct.ICCPKSIGNAlgoInd != 0x04)  //other than '01' is not recognised.
    {
        emvbase_free(ICCPKData);
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(ICCPKCertfct.ICCPKParamInd != 0x11)  //other than '01' is not recognised.
    {
		emvbase_free(ICCPKData);
		return RLT_ERR_EMV_IccDataFormat;

    }
    ICCPKData[iccpkdatalen++] = ICCPKCertfct.ICCPKSIGNAlgoInd;
    ICCPKData[iccpkdatalen++] = ICCPKCertfct.ICCPKEncrptAlgoInd;
    ICCPKData[iccpkdatalen++] = ICCPKCertfct.ICCPKParamInd;
    ICCPKData[iccpkdatalen++] = ICCPKCertfct.ICCPKLen;
    memcpy(&ICCPKData[iccpkdatalen], ICCPKCertfct.ICCPK, ICCPKCertfct.ICCPKLen);
    iccpkdatalen += ICCPKCertfct.ICCPKLen;
	fixlen = iccpkdatalen;

    memcpy((unsigned char*)&ICCPKData[iccpkdatalen], tempEmvContact_UnionStruct->EmvTradeParam->AuthData, tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
    iccpkdatalen += tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen;

	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
	tempEmvContact_UnionStruct->EmvTradeParam->AuthData = NULL;

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen) // for tag57 mask
	{
		offset = tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData + fixlen;
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x02, 1, &offset, iccpkdatalen, ICCPKData, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, hashValue);

	    tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x02, 1, &offset, iccpkdatalen, ICCPKData, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, hashValue);
////		ret = EMVBase_SM_SignatureVerfWithHash(ICCPKCertfct.DGTLSGNTR, hashValue, 32, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
	}
	else
	{
//    	ret = EMVBase_SM_SignatureVerf(ICCPKCertfct.DGTLSGNTR, ICCPKData, iccpkdatalen, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
	}
	emvbase_free(ICCPKData);
	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul);

	tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = NULL;

    if(ret != RLT_EMV_OK)
    {
        return RLT_EMV_ERR;
    }

	if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul == NULL)
	{
		tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul = (unsigned char *)emvbase_malloc(ICCPKCertfct.ICCPKLen);
		if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul == NULL)
		{
			return RLT_EMV_ERR;
		}
		memset(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul,0,ICCPKCertfct.ICCPKLen);

}


    tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen = ICCPKCertfct.ICCPKLen;
    memcpy(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, ICCPKCertfct.ICCPK, ICCPKCertfct.ICCPKLen);

    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_initDynamicAuth(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	EMVBASE_CAPK_STRUCT *tempcapk;
	int ret=0; //20210105 lsy
	tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;

	if(tempcapk == NULL)
	{
		return RLT_EMV_ERR;
	}

    if(EmvContact_SM_CheckDataMissDDA(tempEmvContact_UnionStruct) != RLT_EMV_OK)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);       //ICC data missing
        return RLT_ERR_EMV_IccDataMissing;
    }

    if(tempcapk->ModulLen == 0)
    {
        return RLT_EMV_ERR;
	}

    if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen == 0)
    {
        if(EmvContact_SM_GetIPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)                  //retrieval of the issuer public key modulus
        {
            return RLT_EMV_ERR;
        }
    }

    if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen == 0)
    {
        ret = EmvContact_SM_GetICCPK(tempEmvContact_UnionStruct);
        if(ret != RLT_EMV_OK)                //retrieval of the ICC public key
        {
            return RLT_EMV_ERR;
        }
    }

    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_DynSignVerify(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned short index;
    unsigned char *SSAData;
    unsigned short SSADatalen;
    SM_SIGN_DYN_APPDATA SMSSA;
    unsigned char SignDynAppDatabExist;
    EMVBASETAGCVLITEM *SignDynAppDataitem = NULL, *DDOLDataitem = NULL;
	unsigned char retcode=0;

	unsigned char hashValue[32] = {0};
	unsigned short offset = 0;


    memset((unsigned char *)&SMSSA, 0, sizeof(SM_SIGN_DYN_APPDATA));
    index = 0;

    SignDynAppDataitem = emvbase_avl_gettagitemandstatus(EMVTAG_SignDynAppData, &SignDynAppDatabExist);

    if(SignDynAppDatabExist == 0)
    {
        return RLT_EMV_ERR;
    }
    SMSSA.DataFormat = SignDynAppDataitem->data[index++];
    SMSSA.ICCDynDataLen = SignDynAppDataitem->data[index++];
    memcpy(SMSSA.ICCDynData, &SignDynAppDataitem->data[index], SMSSA.ICCDynDataLen);
    index += SMSSA.ICCDynDataLen;
    memcpy(SMSSA.DGTLSGNTR, &SignDynAppDataitem->data[index], tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen;

    if(index != SignDynAppDataitem->len)
    {
        return RLT_EMV_ERR;
    }

    if(0x15 != SMSSA.DataFormat)
    {
        return RLT_EMV_ERR;
    }

	DDOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_DDOLRelatedData);
    if(DDOLDataitem == NULL)
    {
        return RLT_EMV_ERR;
    }

    SSAData = (unsigned char *)emvbase_malloc( DDOLDataitem->len +SMSSA.ICCDynDataLen + 2);
    memset(SSAData, 0, (DDOLDataitem->len + SMSSA.ICCDynDataLen + 2));

    index = 0;
    SSAData[index++] = SMSSA.DataFormat;
    SSAData[index++] = SMSSA.ICCDynDataLen;
    memcpy(&SSAData[index], SMSSA.ICCDynData, SMSSA.ICCDynDataLen);
    index += SMSSA.ICCDynDataLen;
	offset = index;

    if(DDOLDataitem->data != NULL)
    {
        memcpy(&SSAData[index], DDOLDataitem->data, DDOLDataitem->len);
        index += DDOLDataitem->len;
    }
    SSADatalen = index;

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InDDOLData) // for tag57 mask
	{
		offset += tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInDDOLData;
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x02, 1, &offset, index, SSAData, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, hashValue);

	    tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x02, 1, &offset, index, SSAData, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, hashValue);
////		retcode = EMVBase_SM_SignatureVerfWithHash(SMSSA.DGTLSGNTR, hashValue, 32, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
	}
	else
	{
//		retcode = EMVBase_SM_SignatureVerf(SMSSA.DGTLSGNTR, SSAData, SSADatalen, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
	}
	if(RLT_EMV_OK != retcode)
	{
        emvbase_free(SSAData);
        return RLT_EMV_ERR;
    }
	emvbase_free(SSAData);
    emvbase_avl_createsettagvalue(EMVTAG_ICCDynNum, &SMSSA.ICCDynData[1], SMSSA.ICCDynData[0]);

    return RLT_EMV_OK;
}

static const EMVBASETAG SMSDAmandatoryTag[] =
{
    {EMVTAG_CAPKI},
    {EMVTAG_IPKCert},
    {EMVTAG_SignStatAppData},
    {"\x00\x00\x00"}
};

unsigned char EmvContact_SM_CheckDataMissSDA(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i;

    i = 0;

    while(1)
    {
        if(memcmp(SMSDAmandatoryTag[i].Tag, "\x00\x00\x00", 3) == 0)
        {
            break;
        }

        if(emvbase_avl_checkiftagexist((unsigned char *)SMSDAmandatoryTag[i].Tag) == 0)
        {
            return RLT_ERR_EMV_IccDataMissing;
        }
        i++;
    }

    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_SDVerify(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned short index;
    unsigned char *SSAData;
    SM_SIGN_STAT_APPDATA SMSSA;
    unsigned char ret;
    unsigned char SignStatAppDatabExist;
    EMVBASETAGCVLITEM *SignStatAppDataitem = NULL;

	unsigned char hashValue[32] = {0};
	unsigned short fixlen = 0;
	unsigned short offset = 0;


	if(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen)
	{
		if(tempEmvContact_UnionStruct->EmvTradeParam->AuthData == NULL)
		{
			EMVBase_Trace("EmvTradeParam->AuthData is freed\r\n");
			return RLT_EMV_ERR;
		}
	}

    memset((unsigned char *)&SMSSA, 0, sizeof(SM_SIGN_STAT_APPDATA));
    index = 0;

    SignStatAppDataitem = emvbase_avl_gettagitemandstatus(EMVTAG_SignStatAppData, &SignStatAppDatabExist);

    if(SignStatAppDatabExist == 0)
    {
        return RLT_EMV_ERR;
    }

    SMSSA.DataFormat = SignStatAppDataitem->data[index++];
    memcpy(SMSSA.DataAuthCode, &SignStatAppDataitem->data[index], 2);
    index += 2;
    memcpy(SMSSA.DGTLSGNTR, &SignStatAppDataitem->data[index], tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen;

    if(index != SignStatAppDataitem->len)
    {
		return RLT_EMV_ERR;
    }

    if(0x13 != SMSSA.DataFormat)
    {
        return RLT_EMV_ERR;
    }
    index = 0;
    SSAData = (unsigned char *)emvbase_malloc(tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + 3);
    memset(SSAData, 0, (tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen + 3));

    SSAData[index++] = SMSSA.DataFormat;
    memcpy(&SSAData[index], SMSSA.DataAuthCode, 2);
    index += 2;

	fixlen = index;

    memcpy((unsigned char*)&SSAData[index], tempEmvContact_UnionStruct->EmvTradeParam->AuthData, tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->AuthDataLen;

	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->AuthData);
	tempEmvContact_UnionStruct->EmvTradeParam->AuthData = NULL;

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen) // for tag57 mask
	{
		offset = tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInAuthData + fixlen;
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x02, 1, &offset, index, SSAData, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, hashValue);

	    tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x02, 1, &offset, index, SSAData, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, hashValue);
////		ret = EMVBase_SM_SignatureVerfWithHash(SMSSA.DGTLSGNTR, hashValue, 32, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
	}
	else
	{
//    	ret = EMVBase_SM_SignatureVerf(SMSSA.DGTLSGNTR, SSAData, index, tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);       //½øÐÐÇ©Ãû£¬ÆäÖÐÊý×ÖÇ©ÃûµÄ³¤¶ÈºÍNCAÒ»Ñù£¬Îª64
	}

	emvbase_free(SSAData);
	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul);
	tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = NULL;

    if(ret != RLT_EMV_OK)
    {
        return RLT_EMV_ERR;
    }

    emvbase_avl_createsettagvalue(EMVTAG_DataAuthCode, SMSSA.DataAuthCode, 2);

    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_StaticAuth(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_CAPK_STRUCT *tempcapk;

    tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;

	if(tempcapk == NULL)
	{
		return RLT_EMV_ERR;
	}


    if(EmvContact_SM_CheckDataMissSDA(tempEmvContact_UnionStruct) != RLT_EMV_OK)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);       //ICC data missing

        return RLT_ERR_EMV_IccDataMissing;
    }

    if(tempcapk->ModulLen == 0)
    {
            return RLT_EMV_ERR;
    }

    if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen == 0)
    {
        if(EmvContact_SM_GetIPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)				//retrieval of the issuer public key modulus
        {
            return RLT_EMV_ERR;
        }
    }

    if(EmvContact_SM_SDVerify(tempEmvContact_UnionStruct) != RLT_EMV_OK)				//Verification of the Signed Static Application Data by the terminal
    {
        return RLT_EMV_ERR;
    }

    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_DataAuth(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode = RLT_EMV_OK;
    unsigned char AIP[2];
    unsigned char TermCapab[3];
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char ODAFail = 0;

    memset(AIP, 0, sizeof(AIP));
    memset(TermCapab, 0, sizeof(TermCapab));

    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

	EMVBase_TraceHex("AIP",AIP,2);
	EMVBase_TraceHex("TermCapab 9F33",TermCapab,3);

    if((AIP[0] & 0x01) && (TermCapab[2] & 0x08))
    {
    	EMVBase_Trace("comes to SM CDA\r\n");
        if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL || tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData)          //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
        {
            ODAFail = 1;
        }
        else
        {
            if(EmvContact_SM_initDynamicAuth(tempEmvContact_UnionStruct) != RLT_EMV_OK)
            {
                ODAFail = 1;
            }
        }

        if(ODAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);      //Offline Combined DDA/AC Generation failed
            emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);      //set bit 'Offline Data Authentication was performed' bit 1
        }
        return RLT_EMV_OK;
    }
    else if((AIP[0] & 0x20) && (TermCapab[2] & 0x40))           //ICC and terminal support dynamic data auth.
    {
		EMVBase_Trace("comes to SM DDA\r\n");
        if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL || tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData)          //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object;//processing of SDA Tag List failed
        {
            ODAFail = 1;
        }
        else
        {
            ODAFail = 1;
            retCode = EmvContact_SM_initDynamicAuth(tempEmvContact_UnionStruct);
            if(retCode == RLT_EMV_OK)
            {
                retCode = EmvContact_FormDynSignGenerate(tempEmvContact_UnionStruct, &apdu_s);
                if(retCode == RLT_EMV_OK)
                {
                	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InDDOLData)
                	{
                    	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInDDOLData);
					}
					else
					{
                    	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
					}

                    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
                    {
                        retCode = RLT_ERR_EMV_APDUTIMEOUT;
                    }

                    if(retCode == RLT_EMV_OK)
                    {
                        retCode = EmvContact_DynSignGenerate(tempEmvContact_UnionStruct, &apdu_r);
                        if(retCode == RLT_EMV_OK)
                        {
                            retCode = EmvContact_SM_DynSignVerify(tempEmvContact_UnionStruct);
                            if(retCode == RLT_EMV_OK)
                            {
                                ODAFail = 0;
                            }
                        }
                    }
                }
            }

            if(retCode == RLT_ERR_EMV_APDUTIMEOUT || retCode == RLT_ERR_EMV_IccDataFormat || retCode == RLT_ERR_EMV_IccReturn)
            {
                return retCode;
            }
        }

        if(ODAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x08);
        }
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);      //set bit 'Offline Data Authentication was performed' bit 1
        retCode = RLT_EMV_OK;
    }
    else if((AIP[0] & 0x40) && (TermCapab[2] & 0x80)) 		//ICC and terminal support static data auth.
    {
    	EMVBase_Trace("comes to SM SDA\r\n");
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x02);

        if(tempEmvContact_UnionStruct->EmvTradeParam->bErrSDATL || tempEmvContact_UnionStruct->EmvTradeParam->bErrAuthData)          //SDA_TL.tag not EMV; represents data not available at the moment; refers to constructed Data Object
        {
            ODAFail = 1;
        }
        else
        {
            retCode = EmvContact_SM_StaticAuth(tempEmvContact_UnionStruct);
            if(retCode != RLT_EMV_OK)               //Static data authentication
            {
                ODAFail = 1;
            }
        }

        if(ODAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x40);
        }
        else
        {
            emvbase_avl_setvalue_and(EMVTAG_TVR, 0, 0xBF);
        }
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);      //set bit 'Offline Data Authentication was performed' bit 1
        retCode = RLT_EMV_OK;
    }
    else    //all dynamic and static authentication and DDA/AC are not supported.
    {
    	EMVBase_Trace("SM ODA fail\r\n");
        //The bit should be set to 1 according to test script 2CI.023.00
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x80);                              //Offline Data Authentication not performed
        emvbase_avl_setvalue_and(EMVTAG_TSI, 0, 0x7F);                             //set bit 'Offline data authentication was performed' bit 0.
        retCode = RLT_EMV_OK;
    }
    return retCode;
}



void EmvContact_CheckForceOnline(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	unsigned char bForceOnline;

	bForceOnline = emvbase_avl_gettagvalue(EMVTAG_bForceOnline);

	if(bForceOnline)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x08);
	}
}

unsigned char EmvContact_IccGetPAN(unsigned char *pasOutPAN)
{
    unsigned char temp[128] = {0};
    unsigned char bcPAN[128] = {0};
    unsigned int i, j, len;
    EMVBASETAGCVLITEM *item = NULL, *panitem = NULL;
    unsigned char tagexistflag, pantagexistflag;
    unsigned char Track2Len, Track2Data[20];


    if(NULL == pasOutPAN)
    {
        return RLT_EMV_ERR;
    }
    panitem = emvbase_avl_gettagitemandstatus(EMVTAG_PAN, &pantagexistflag);
    item = emvbase_avl_gettagitemandstatus(EMVTAG_Track2Equ, &tagexistflag);

	#if 0
    if(tagexistflag)
    {
        for(i = item->len - 1; i > 1; i--)
        {
            if(item->data[i] != 0xff) { break; }
        }

        j = i + 1;

        if((item->data[j - 1] & 0x0f) == 0x0f)
        {
            i = j * 2 - 1;
        }
        else{ i = j * 2; }

        if(i > 37)
        {
            i = 37;
        }
        Track2Len = i;
        memcpy(Track2Data, item->data, (i + 1) / 2);

        if(Track2Len)
        {
            len = (Track2Len + 1) / 2;

            if(len > 19) { len = 19; }
            memset(bcPAN, 0, sizeof(bcPAN));
            EMVBaseBcdToAsc(temp, Track2Data, len);

            len = strlen((char *)temp);

            for(i = 0; i < len; i++)
            {
                if(temp[i] == 'D')
                {
                    break;
                }
            }

            len = i;

            if(len > 19) { len = 19; }
            memcpy(pasOutPAN, temp, len);
            return RLT_EMV_OK;
        }
    }
    else
	#endif
	if(pantagexistflag)
    {
        memset(temp, 0, sizeof(temp));
        EMVBaseBcdToAsc(temp, panitem->data, panitem->len);

        if((panitem->data[panitem->len - 1] & 0x0F) == 0x0F)
        {
            temp[panitem->len * 2 - 1] = '\0';
        }
        memcpy(pasOutPAN, temp, strlen((char *)temp));
        return RLT_EMV_OK;
    }

    return RLT_EMV_ERR;
}

void EmvContact_CheckVer(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char CardAppVerbExist;
    unsigned char TermAppVerbExist;
	EMVBASETAGCVLITEM *Termitem = NULL,*Carditem = NULL;

    Carditem = emvbase_avl_gettagitemandstatus(EMVTAG_AppVerNum,&CardAppVerbExist);
	Termitem = emvbase_avl_gettagitemandstatus(EMVTAG_AppVer,&TermAppVerbExist);

	if(CardAppVerbExist&&TermAppVerbExist)
	{
		if(Carditem->len != Termitem->len)
		{
			emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x80);
		}
		else
		{
			if(memcmp(Carditem->data,Termitem->data,Termitem->len))
			{
				emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x80);
			}
		}
	}
}

void EmvContact_CheckAUC(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	unsigned char bTestFail;
    unsigned char TermType,AUC[2],AUCbExist;
	unsigned char IssuCountryCode[2],CountryCode[2],IssuCountryCodebExist;
	unsigned char TermAmtOther[6];
    unsigned char TermAddCapab[5];
	unsigned char TermAmtOtherBin = 0;
	unsigned char TransType;

	bTestFail=0;
	memset(AUC,0,sizeof(AUC));
    AUCbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_AUC,AUC,0,2));

	TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);
//	TransType = tempEmvContact_UnionStruct->EmvTradeParam->TransType;
	TransType = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);

	memset(TermAddCapab,0,sizeof(TermAddCapab));
	emvbase_avl_gettagvalue_spec(EMVTAG_TermAddCapab,TermAddCapab,0,5);

    memset(IssuCountryCode,0,sizeof(IssuCountryCode));
	IssuCountryCodebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IssuCountryCode,IssuCountryCode,0,2));
    memset(CountryCode,0,sizeof(CountryCode));
	emvbase_avl_gettagvalue_spec(EMVTAG_CountryCode,CountryCode,0,2);

	memset(TermAmtOther,0,sizeof(TermAmtOther));
    emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum,TermAmtOther,0,6);
	if(memcmp(TermAmtOther,"\x00\x00\x00\x00\x00\x00",6))
	{
		TermAmtOtherBin = 1;
	}

	if(AUCbExist)	//AUC exist.
	{
		if((TermType==0x14 || TermType==0x15 || TermType==0x16 )
			&& (TermAddCapab[0]&0x80))	//The termianl is ATM
		{
			if(!(AUC[0]&0x02))
				bTestFail=1;
		}
		else		//The terminal is not ATM
		{
			if(!(AUC[0]&0x01))
				bTestFail=1;
		}
		if(IssuCountryCodebExist){		//Issuer country code exist
			if(!memcmp(IssuCountryCode,CountryCode,2))		//domestic
			{
				if(TransType == EMVBASE_TRANS_CASH)
				{
					if(!(AUC[0]&0x80))
						bTestFail=1;
				}

				if(TransType == EMVBASE_TRANS_GOODS)
				{
					if( !(AUC[0]&0x28))
						bTestFail=1;
				}
				if(TransType == EMVBASE_TRANS_CASHBACK)
				{
					if(!(AUC[1]&0x80))
						bTestFail=1;
				}
			}
			else							//international,terminal country code differ from issuer country code
			{
				if(TransType == EMVBASE_TRANS_CASH)
				{
					if(!(AUC[0]&0x40))
						bTestFail=1;
				}

				if(TransType == EMVBASE_TRANS_GOODS )
				{
					if( !(AUC[0]&0x14))
						bTestFail=1;
				}

				if(TransType == EMVBASE_TRANS_CASHBACK)
				{
					if(!(AUC[1]&0x40))
						bTestFail=1;
				}
			}
		}
	}


	if(bTestFail)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x10);
	}

}

unsigned char EmvContact_CheckDateFormat(unsigned char * date)
{
	unsigned char i,k,n[3];

	for(i=0;i<3;i++){
		k=date[i];
		if((k&0x0F) > 9) return RLT_EMV_ERR;
		if(((k&0xF0)>>4) > 9) return RLT_EMV_ERR;
		n[i]=((k&0xF0)>>4)*10 + (k&0x0F);
	}
	if(n[1]<1 || n[1]>12) return RLT_EMV_ERR;
	if(n[2]<1 || n[2]>31) return RLT_EMV_ERR;
	if(n[1]==0x02)
	{
		if(n[2]>29) return RLT_EMV_ERR;
	}
	return RLT_EMV_OK;
}

unsigned char EmvContact_CheckExpDate(EMVCONTACTTradeUnionStruct *tempApp_UnionStruct)
{
	unsigned char currentDate[4],EffectDate[4],ExpireDate[4];
	unsigned char AppEffectDateexist,AppExpireDateexist;
	EMVBASETAGCVLITEM *item;

    emvbase_avl_gettagvalue_spec(EMVTAG_TransDate,&currentDate[1],0,3);

	if(currentDate[1]>0x49) currentDate[0]=0x19;
	else currentDate[0]=0x20;

    AppEffectDateexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_AppEffectDate);
    if(item != NULL)
    {
    	if(item->len>=3)
    	{
    		AppEffectDateexist = 1;
    	}
    }

	if(AppEffectDateexist)	//App effective date exist
	{
		if(EmvContact_CheckDateFormat(item->data) == RLT_EMV_ERR) return RLT_EMV_ERR;

        if(item->data[0]>0x49) EffectDate[0]=0x19;
		else EffectDate[0]=0x20;

		memcpy((unsigned char*)&EffectDate[1],item->data,3);
		if(memcmp(currentDate,EffectDate,4)<0)
		{
		    emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x20);
		}
	}

	AppExpireDateexist = 0;
    item = emvbase_avl_gettagitempointer(EMVTAG_AppExpireDate);
    if(item != NULL)
    {
    	if(item->len>=3)
    	{
    		AppExpireDateexist = 1;
    	}
    }

	if(AppExpireDateexist)		//App expiration date exist
	{
		if(EmvContact_CheckDateFormat(item->data)==RLT_EMV_ERR) return RLT_EMV_ERR;

		if(item->data[0]>0x49) ExpireDate[0]=0x19;
		else ExpireDate[0]=0x20;

		memcpy((unsigned char*)&ExpireDate[1],item->data,3);
		if(memcmp(currentDate,ExpireDate,4)>0)
		{
			emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x40);
		}
	}
	return RLT_EMV_OK;
}

unsigned char EmvContact_ProcessRestrict(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char asPAN[20] = {0};
    int retCode;

    if(emvbase_avl_gettagtype() == EMVTAGTYPE_EMV)
    {
    	EmvContact_CheckForceOnline(tempEmvContact_UnionStruct);
    }

    if(tempEmvContact_UnionStruct->CheckCardNo != NULL)
    {
        //if(emvbase_avl_checkiftagexist(EMVTAG_PAN) || emvbase_avl_checkiftagexist(EMVTAG_Track2Equ))
        if(emvbase_avl_checkiftagexist(EMVTAG_PAN))
        {
            if(EmvContact_IccGetPAN(asPAN) == RLT_EMV_OK)
            {
                retCode = (*tempEmvContact_UnionStruct->CheckCardNo)(asPAN);
                if(retCode != 1)
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x10);
                }
            }
        }
    }


	EmvContact_CheckVer(tempEmvContact_UnionStruct);
	EmvContact_CheckAUC(tempEmvContact_UnionStruct);
	if(EmvContact_CheckExpDate(tempEmvContact_UnionStruct) != RLT_EMV_OK) return RLT_EMV_ERR;
	return RLT_EMV_OK;
}

unsigned char EmvContact_InitCardHolderVerify(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, CVMSTRCUT *CVMdata)
{
    unsigned char i;
    unsigned char AIP[2];
    unsigned char CVMResult[3];
    unsigned char CVMListbExist;
    EMVBASETAGCVLITEM *CVMListitem;

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);

	EMVBase_TraceHex("AIP",AIP,2);

    if(!(AIP[0] & 0x10))
    {
        CVMResult[0] = 0x3f;
        CVMResult[1] = 0;
        CVMResult[2] = CVMR_UNKNOWN;
        emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);
        return RLT_EMV_OK;
    }

    //get cvm list in the card, if not available, return
    CVMListitem = emvbase_avl_gettagitemandstatus(EMVTAG_CVMList, &CVMListbExist);

    if(CVMListbExist == 0x00)  //CVM List is absent.
    {
    	EMVBase_Trace("CVM List missing\r\n");
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);
        CVMResult[0] = 0x3f;
        CVMResult[1] = 0;
        CVMResult[2] = CVMR_UNKNOWN;
        emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);
        return RLT_EMV_OK;
    }

    if(CVMListitem->len < 10 || (CVMListitem->len % 2))
    {
    	EMVBase_Trace("CVM List len is not correct. Len is %d\r\n",CVMListitem->len);
        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x20);	//ICC Data Missing
        CVMResult[0] = 0x3f;
        CVMResult[1] = 0;
        CVMResult[2] = CVMR_UNKNOWN;
        emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);
        return RLT_EMV_OK;
    }

	EMVBase_TraceHex("CVM List 8E",CVMListitem->data,CVMListitem->len);

    memcpy(&CVMdata->CVM_X[2], CVMListitem->data, 4);
    memcpy(&CVMdata->CVM_Y[2], &CVMListitem->data[4], 4);

    CVMdata->CVRListLen = (CVMListitem->len - 8) >> 1;

    for(i = 0; i < CVMdata->CVRListLen; i++)
    {
        CVMdata->CVRList[i].method = CVMListitem->data[i * 2 + 8];
        CVMdata->CVRList[i].condition = CVMListitem->data[i * 2 + 9];
    }


    CVMResult[0] = 0x3f;
    CVMResult[1] = 0;
    CVMResult[2] = CVMR_FAIL;
    emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);

    return RLT_EMV_OTHER;
}


unsigned char EmvContact_CheckIfSupportCVM(unsigned char method)
{
    unsigned char temp;
    unsigned char TermCapab[3];

    temp = method & 0x3F;

    memset(TermCapab, 0, sizeof(TermCapab));
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

    switch(temp)
    {
         case 0x00:
            {return 1; }
                       break;

         case 0x01: //PLAINTEXT PIN VERIFICATION PERFORMED BY ICC
            {if((TermCapab[1] & 0x80) == 0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x02: //enciphered PIN verification online
            {if((TermCapab[1] & 0x40) == 0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x03: //Plaintext PIN verification performed by ICC and signature(paper)
            {if((TermCapab[1] & 0xA0) != 0xA0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x04: //enciphered PIN verification performed by ICC
            {if((TermCapab[1] & 0x10) == 0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x05: //enciphered PIN verification performed by ICC and signature(paper)
            {if((TermCapab[1] & 0x30) != 0x30)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x1E: //signature (paper)
            {if((TermCapab[1] & 0x20) == 0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x1F: //no CVM required
            {if((TermCapab[1] & 0x08) == 0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

         case 0x20: //PBOC ID
            {if((TermCapab[1] & 0x01) == 0)
             {
                 return 0;
             }
             else
             {
                 return 1;
             }}
              break;

		 case 0x21: //DPAS CDCVM
            {return 1;}
              break;

         default:
            {
                return 0;
            }
            break;
    }

    return 0;
}

unsigned long EmvContact_ByteArrayToInt(unsigned char* buf,unsigned char bufLen)
{
	unsigned char i;
	unsigned long temp;
	temp=0;
	for(i=0;i<bufLen;i++)
	{
		temp=(temp<<8)+buf[i];
	}
	return temp;
}

unsigned char EmvContact_AnalyCvmlist_cmpamount(unsigned char *CVMamount,unsigned char type)
{
    unsigned char TransCurcyCodebExist,TransCurcyCode[2];
    unsigned char TermAmtAuth[6],TermAmtCVM[6];
	unsigned char AppCurcyCodebExist,AppCurcyCode[2];
	unsigned int TermAmt;

	memset(TransCurcyCode,0,sizeof(TransCurcyCode));
    TransCurcyCodebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TransCurcyCode,TransCurcyCode,0,2));

	memset(AppCurcyCode,0,sizeof(AppCurcyCode));
	AppCurcyCodebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_AppCurcyCode,AppCurcyCode,0,2));

    memset(TermAmtAuth,0,sizeof(TermAmtAuth));
	emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum,TermAmtAuth,0,6);

	if(TransCurcyCodebExist==0x00 || AppCurcyCodebExist == 0)		//Application Currency Code is not present
	{
		return 0;
	}


	if( !memcmp(TransCurcyCode,AppCurcyCode,2))			//under x shouldn't include case of equal x.(EMV2000 2CJ.077.02)
	{
		TermAmt = EmvContact_ByteArrayToInt(&CVMamount[2],4);
		memset(TermAmtCVM,0,sizeof(TermAmtCVM));
		EMVBaseU32ToBcd(TermAmtCVM,TermAmt,6);

		if(type == 0)
		{
			if(memcmp(TermAmtAuth,TermAmtCVM,6) < 0)
			{
				return 1;
			}
		}
		else
		{
			if(memcmp(TermAmtAuth,TermAmtCVM,6) > 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

unsigned short EmvContact_AnalyCvmlist(unsigned char method, unsigned char condition, unsigned char TransType, unsigned char TermType, unsigned char *CVM_X, unsigned char *CVM_Y)
{
    unsigned char needcvm = 0, nextcvm = 0;
    unsigned short returnvalue;
    unsigned char TermAmtOther[6];
    unsigned char TermAmtOthernotzero = 0;
    unsigned char ret;


    switch(condition)
    {
         case 0x00:       //always
            {needcvm = 1; }
                          break;

         case 0x01:       //if cash or cashback
            {if(TransType == EMVBASE_TRANS_CASH && (TermType & 0x0F) > 3 )
             {
                 needcvm = 1;
             }
             else{ nextcvm = 1; }}
                                 break;

         case 0x02:       //if not cash or cashback
            {memset(TermAmtOther, 0, sizeof(TermAmtOther));
             emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum, TermAmtOther, 0, 6);

             if(memcmp(TermAmtOther, "\x00\x00\x00\x00\x00\x00", 6))
             {
                 TermAmtOthernotzero = 1;
             }

             if(TransType != EMVBASE_TRANS_CASH  && TransType != EMVBASE_TRANS_CASHBACK && TermAmtOthernotzero == 0)
             {
                 needcvm = 1;
             }
             else{ nextcvm = 1; }}
                                 break;

         case 0x03:       //if terminal support CVM   //sxl arm_linux
            {if(EmvContact_CheckIfSupportCVM(method))
             {
                 needcvm = 1;
             }
             else
             {
                 nextcvm = 1;
             }}
              break;

         case 0x04:       //if manual cash (added in EMV4.1,SU16)
            {if(TransType == EMVBASE_TRANS_CASH && (TermType & 0x0F) <= 3 )
             {
                 needcvm = 1;
             }
             else{ nextcvm = 1; }}
                                 break;

         case 0x05:       //if purchase with cashback (added in EMV4.1,SU16)
            {memset(TermAmtOther, 0, sizeof(TermAmtOther));
             emvbase_avl_gettagvalue_spec(EMVTAG_AmtOtherNum, TermAmtOther, 0, 6);

             if(memcmp(TermAmtOther, "\x00\x00\x00\x00\x00\x00", 6))
             {
                 TermAmtOthernotzero = 1;
             }

             if(TransType == EMVBASE_TRANS_CASHBACK|| TermAmtOthernotzero )
             {
                 needcvm = 1;
             }
             else{ nextcvm = 1; }}
                                 break;

         case 0x06:       //if trans is in App currency and under X value
            {ret  = EmvContact_AnalyCvmlist_cmpamount(CVM_X, 0);

             if(ret == 0)
             {
                 nextcvm = 1;
             }
             else
             {
                 needcvm = 1;
             }}
              break;

         case 0x07:       //if trans is in App currency and over X value
            {ret  = EmvContact_AnalyCvmlist_cmpamount(CVM_X, 1);

             if(ret == 0)
             {
                 nextcvm = 1;
             }
             else
             {
                 needcvm = 1;
             }}
              break;

         case 0x08:       //if trans is in App currency and under Y value
            {ret  = EmvContact_AnalyCvmlist_cmpamount(CVM_Y, 0);

             if(ret == 0)
             {
                 nextcvm = 1;
             }
             else
             {
                 needcvm = 1;
             }}
              break;

         case 0x09:       //if trans is in App currency and over Y value
            {ret  = EmvContact_AnalyCvmlist_cmpamount(CVM_Y, 1);

             if(ret == 0)
             {
                 nextcvm = 1;
             }
             else
             {
                 needcvm = 1;
             }}
              break;

         default:
            {
                nextcvm = 1;
            }
            break;
    }

    returnvalue = nextcvm;
    returnvalue = (returnvalue << 8) | needcvm;

    return returnvalue;
}


unsigned char EmvContact_GetPINTryCount(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, "\x80\xCA\x9F\x17", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    return RLT_EMV_OK;
}


unsigned char EmvContact_GetPinTryCount(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char PINTryCount;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned short index;
    unsigned short DataLen;

    if(emvbase_avl_checkiftagexist(EMVTAG_PINTryCount))
    {
        PINTryCount = emvbase_avl_gettagvalue(EMVTAG_PINTryCount);

        if(PINTryCount == 0)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
            return EMVCONTACT_ERR_CVMFail;
        }
        else
        {
            return RLT_EMV_OK;
        }
    }
    EmvContact_GetPINTryCount(&apdu_s);
    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
    {
        if(memcmp(apdu_r.DataOut, "\x9F\x17", 2) == 0)
        {
            index = 2;
            EMVBase_ParseExtLen(apdu_r.DataOut, &index, &DataLen);

            if(index + DataLen == apdu_r.LenOut)
            {
                if(DataLen == 1)
                {
                    emvbase_avl_settag(EMVTAG_PINTryCount, apdu_r.DataOut[index]);
                }
            }
        }
    }

    if(emvbase_avl_checkiftagexist(EMVTAG_PINTryCount))
    {
        PINTryCount = emvbase_avl_gettagvalue(EMVTAG_PINTryCount);

        if(PINTryCount == 0)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
            return EMVCONTACT_ERR_CVMFail;
        }
    }
    return RLT_EMV_OK;
}

unsigned char EmvContact_chartobin(unsigned char bchar)
{
	if ((bchar >= '0')&&(bchar <= '9'))
		return(bchar-'0');
	else
	{
		if ((bchar >= 'A')&&(bchar <= 'F'))
			return(bchar-'A'+10);
		if ((bchar >= 'a')&&(bchar <= 'f'))
			return(bchar-'a'+10);
		else
            return(0);        // old value is 0x0f
	}
}


void EmvContact_PIN_asc_to_bcd(unsigned char *bcd, unsigned char *asc, unsigned char asc_len)
{
	unsigned char i,j;
	unsigned char bchar, bchar1, bchar2;


	for (i=0,j=0; j<asc_len; i++)
	{
		bchar1 = asc[j++];
		bchar = EmvContact_chartobin(bchar1);
		bchar *= 16;
		if(j==asc_len)
		{
			bcd[i] = bchar+0x0F;
			break;
		}
		bchar1 = asc[j++];
		bchar2 = EmvContact_chartobin(bchar1);
		bcd[i] = bchar + bchar2;
	}
}


unsigned char EmvContact_PlaintextCommand(EMVBASE_APDU_SEND *apdu_s, unsigned char *tempPIN)
{
    memcpy(apdu_s->Command, "\x00\x20\x00\x80", 4);
    apdu_s->Lc = 8;
    memcpy(apdu_s->DataIn, tempPIN, 8);
    apdu_s->Le = 0;
    apdu_s->EnableCancel = 1;
    return RLT_EMV_OK;
}

unsigned char EmvContact_CVMPlainTextPin(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned short index;
    unsigned short DataLen;

    unsigned char offlineplaintext[20];
    unsigned char retCode;
    unsigned char tempPIN[8], PINLen, i;
	unsigned char externalpinpadflag = 0;

    while(1)
    {
        retCode = EmvContact_GetPinTryCount(tempEmvContact_UnionStruct);

        if(retCode != RLT_EMV_OK)
        {
            return retCode;
        }

        memset(offlineplaintext, 0, sizeof(offlineplaintext));
        retCode = tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN, offlineplaintext);


        if(retCode != RLT_EMV_OK)
        {
            if(retCode == RLT_ERR_EMV_CancelTrans)
            {
                return RLT_ERR_EMV_CancelTrans;
            }
            else if(retCode == RLT_ERR_EMV_InputBYPASS)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x08);
                emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0xDF);

                //return EMVCONTACT_ERR_CVMFail;
                return EMVCONTACT_ERR_Bypass;
            }
            else         //pin pad err
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                return EMVCONTACT_ERR_CVMFail;
            }
        }
        //verify pin input
        PINLen = strlen((char *)offlineplaintext);
        tempPIN[0] = 0x20 + PINLen;

        for(i = 1; i < 8; i++)
        {
            tempPIN[i] = 0xFF;
        }

		for(i = 0;i < PINLen;i++)
		{
			if(offlineplaintext[i] < 0x30 || offlineplaintext[i] > 0x39)
			{
				externalpinpadflag = 1;
				break;
			}
		}
		if(externalpinpadflag == 0)
		{
        	EmvContact_PIN_asc_to_bcd((unsigned char*)&tempPIN[1], offlineplaintext, strlen((char *)offlineplaintext));
		}
        EmvContact_PlaintextCommand(&apdu_s, tempPIN);

		/*if(externalpinpadflag) // may be move to Ex
		{
        	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, 0);
		}
		else*/
		{
        	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
		}
        if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
        {
            return RLT_ERR_EMV_APDUTIMEOUT;
        }

        if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
        {
            offlineplaintext[0] = 0;
            tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_INPUTPINSTATUS, offlineplaintext);
            emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0x7F);
            return RLT_EMV_OK;
        }
        else if(apdu_r.SW1 == 0x69 && (apdu_r.SW2 == 0x83 || apdu_r.SW2 == 0x84))
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
            return EMVCONTACT_ERR_CVMFail;
        }
        else if(apdu_r.SW1 == 0x63 && (apdu_r.SW2 & 0xF0) == 0xC0)
        {
            offlineplaintext[0] = 1;
            offlineplaintext[1] = apdu_r.SW2 - 0xC0;
            emvbase_avl_settag(EMVTAG_PINTryCount, offlineplaintext[1]);
            tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_INPUTPINSTATUS, offlineplaintext);

            if(apdu_r.SW2 == 0xC0)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
                return EMVCONTACT_ERR_CVMFail;
            }
        }
        else
        {
            return RLT_ERR_EMV_IccReturn;
        }
    }

    return RLT_EMV_OK;
}

unsigned char EmvContact_CVMOnlineEncPin(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    unsigned char tempCreditPwd[20];
    unsigned char PINLen;
    unsigned char tempPIN[8];
    unsigned char i, k;
    unsigned char PAN[10];
    unsigned short PANLen;
    unsigned char buf[20], PanBlockAsc[16], PanBlockBcd[8];

    memset(tempCreditPwd, 0, sizeof(tempCreditPwd));
    retCode = tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_ONLINEPIN, tempCreditPwd);

    if(retCode == RLT_EMV_OK)
    {
        emvbase_avl_createsettagvalue(EMVTAG_PIN, tempCreditPwd, 8);
        emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x04); 	//Online PIN entered
        return RLT_EMV_OK;
    }
    else if(retCode == RLT_ERR_EMV_InputBYPASS)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x08);
        emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0xDF);
        return  EMVCONTACT_ERR_Bypass;
        //return EMVCONTACT_ERR_CVMFail;
    }

    if(retCode == RLT_ERR_EMV_CancelTrans)
    {
        return RLT_ERR_EMV_CancelTrans;
    }
    else
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
        return EMVCONTACT_ERR_CVMFail;
    }
}

void EmvContact_FillICCPIN_EPK(unsigned char* ICCPIN_EPKData, ICCPIN_EPK_RECOVER* recovICCPIN_EPK, unsigned char TempIPKModulLen)
{
    unsigned char i;

    recovICCPIN_EPK->DataHead = ICCPIN_EPKData[0];
    recovICCPIN_EPK->CertFormat = ICCPIN_EPKData[1];
    memcpy(recovICCPIN_EPK->AppPAN, (unsigned char*)&ICCPIN_EPKData[2], 10);
    memcpy(recovICCPIN_EPK->ExpireDate, (unsigned char*)&ICCPIN_EPKData[12], 2);
    memcpy(recovICCPIN_EPK->CertSerial, (unsigned char*)&ICCPIN_EPKData[14], 3);
    recovICCPIN_EPK->HashInd = ICCPIN_EPKData[17];
    recovICCPIN_EPK->ICCPIN_EPKAlgoInd = ICCPIN_EPKData[18];
    recovICCPIN_EPK->ICCPIN_EPKLen = ICCPIN_EPKData[19];
    recovICCPIN_EPK->ICCPIN_EPKExpLen = ICCPIN_EPKData[20];
    memcpy(recovICCPIN_EPK->ICCPIN_EPKLeft, (unsigned char*)&ICCPIN_EPKData[21], TempIPKModulLen - 42);

    for(i = 0; i < 20; i++)
    {
        recovICCPIN_EPK->HashResult[i] = ICCPIN_EPKData[TempIPKModulLen - 21 + i];
    }

    recovICCPIN_EPK->DataTrail = ICCPIN_EPKData[TempIPKModulLen - 1];
}


unsigned char EmvContact_GetICCPIN_EPK(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *ICCPIN_EPKData, *ICCPIN_EPKToSign, ICCPIN_EPKHash[20];
    unsigned char i, byteRecovPAN[20], byteICCPAN[20];
    unsigned short index;
    ICCPIN_EPK_RECOVER *recovICCPIN_EPK;

    unsigned char ICCPIN_EPKCertbExist = 0, ICCPIN_EPKExpbExist = 0;
    EMVBASETAGCVLITEM *ICCPIN_EPKCertitem, *ICCPIN_EPKExpitem;
    EMVBASETAGCVLITEM *item, *ICCPIN_EPKRemitem;
    EMVBASETAGCVLITEM *IPKExpitem = NULL;
    unsigned char IPKExpbExist;

    ICCPIN_EPKCertitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPIN_EPKCert, &ICCPIN_EPKCertbExist);
    ICCPIN_EPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPIN_EPKExp, &ICCPIN_EPKExpbExist);
    IPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_IPKExp, &IPKExpbExist);


    if(ICCPIN_EPKCertitem->len != tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen)
    {
        return RLT_EMV_ERR;
    }
    ICCPIN_EPKData = (unsigned char *)emvbase_malloc(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);
    recovICCPIN_EPK = (ICCPIN_EPK_RECOVER *)emvbase_malloc(sizeof(ICCPIN_EPK_RECOVER));


    EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen, IPKExpitem->data, IPKExpitem->len, ICCPIN_EPKCertitem->data, ICCPIN_EPKData);

	emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul);
	tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = NULL;

    EmvContact_FillICCPIN_EPK(ICCPIN_EPKData, recovICCPIN_EPK, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen);

    if(recovICCPIN_EPK->DataTrail != 0xBC)
    {
        emvbase_free(ICCPIN_EPKData);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }

    if(recovICCPIN_EPK->DataHead != 0x6A)
    {
        emvbase_free(ICCPIN_EPKData);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }

    if(recovICCPIN_EPK->CertFormat != 0x04)
    {
        emvbase_free(ICCPIN_EPKData);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }
    ICCPIN_EPKToSign = (unsigned char *)emvbase_malloc(1024);


    index = 0;
    memcpy(ICCPIN_EPKToSign, (unsigned char*)&ICCPIN_EPKData[1], tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22);
    index += tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 22;
	emvbase_free(ICCPIN_EPKData);

    if(recovICCPIN_EPK->ICCPIN_EPKLen > (tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42))
    {
        ICCPIN_EPKRemitem = emvbase_avl_gettagitempointer(EMVTAG_ICCPIN_EPKRem);

        if(ICCPIN_EPKRemitem != NULL)
        {
            if(ICCPIN_EPKRemitem->len)
            {
                memcpy((unsigned char*)&ICCPIN_EPKToSign[index], (unsigned char*)ICCPIN_EPKRemitem->data, ICCPIN_EPKRemitem->len);
                index += ICCPIN_EPKRemitem->len;
            }
        }
    }
    memcpy((unsigned char*)&ICCPIN_EPKToSign[index], (unsigned char*)ICCPIN_EPKExpitem->data, ICCPIN_EPKExpitem->len);
    index += ICCPIN_EPKExpitem->len;

    EMVBase_Hash(ICCPIN_EPKToSign, index, ICCPIN_EPKHash);

    if(recovICCPIN_EPK->HashInd == 0x01)  	//SHA-1 algorithm
    {
        if(memcmp(recovICCPIN_EPK->HashResult, ICCPIN_EPKHash, 20))
        {
            emvbase_free(ICCPIN_EPKToSign);
            emvbase_free(recovICCPIN_EPK);
            return RLT_EMV_ERR;
        }
    }
    else
    {
        emvbase_free(ICCPIN_EPKToSign);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }
    //verify if recovered PAN matches PAN in ICC
    item = emvbase_avl_gettagitempointer(EMVTAG_PAN);

    if(item == NULL)
    {
        emvbase_free(ICCPIN_EPKToSign);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }

    for(i = 0; i < 10; i++)
    {
        if(i < item->len)
        {
            byteICCPAN[2 * i] = (item->data[i] & 0xF0) >> 4;
            byteICCPAN[2 * i + 1] = item->data[i] & 0x0F;
        }
        byteRecovPAN[2 * i] = (recovICCPIN_EPK->AppPAN[i] & 0xF0) >> 4;
        byteRecovPAN[2 * i + 1] = recovICCPIN_EPK->AppPAN[i] & 0x0F;
    }

    for(i = 20; i > 0; i--)
    {
        if(byteRecovPAN[i - 1] != 0x0F)
        {
            if(memcmp(byteRecovPAN, byteICCPAN, i))
            {
                emvbase_free(ICCPIN_EPKToSign);
                emvbase_free(recovICCPIN_EPK);
                return RLT_EMV_ERR;
            }
            else
            {
                break;
            }
        }
    }

    if(i < 1)
    {
        emvbase_free(ICCPIN_EPKToSign);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }

    //verify expiredate is later than current date
    if(EMVBase_ExpireDateVerify(recovICCPIN_EPK->ExpireDate) != RLT_EMV_OK)
    {
        emvbase_free(ICCPIN_EPKToSign);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }

    if(recovICCPIN_EPK->ICCPIN_EPKAlgoInd != 0x01)  //other than '01' is not recognised.
    {
        emvbase_free(ICCPIN_EPKToSign);
        emvbase_free(recovICCPIN_EPK);
        return RLT_EMV_ERR;
    }

	tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul = (unsigned char	*)emvbase_malloc(recovICCPIN_EPK->ICCPIN_EPKLen);

	if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul == NULL)
	{
		emvbase_free(ICCPIN_EPKToSign);
		emvbase_free(recovICCPIN_EPK);
		return RLT_EMV_ERR;
	}

	tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen = recovICCPIN_EPK->ICCPIN_EPKLen;

    if(recovICCPIN_EPK->ICCPIN_EPKLen <= tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42)
    {
        memcpy(tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul, recovICCPIN_EPK->ICCPIN_EPKLeft, recovICCPIN_EPK->ICCPIN_EPKLen);
    }
    else
    {
        memcpy(tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul, recovICCPIN_EPK->ICCPIN_EPKLeft, tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42);
        memcpy((unsigned char*)&tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul[tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen - 42], ICCPIN_EPKRemitem->data, recovICCPIN_EPK->ICCPIN_EPKLen - tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen + 42);
    }

    emvbase_free(ICCPIN_EPKToSign);
    emvbase_free(recovICCPIN_EPK);
    return RLT_EMV_OK;
}

unsigned char EmvContact_GetPINEncipherEPK(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_CAPK_STRUCT *tempcapk;
    unsigned char ICCPIN_EPKCertbExist = 0, ICCPIN_EPKExpbExist = 0;
    EMVBASETAGCVLITEM *ICCPIN_EPKCertitem = NULL, *ICCPIN_EPKExpitem = NULL;
    unsigned char ICCPKCertbExist = 0, ICCPKExpbExist = 0;
    EMVBASETAGCVLITEM *ICCPKCertitem = NULL, *ICCPKExpitem = NULL;

    tempcapk = tempEmvContact_UnionStruct->EmvTradeParam->CAPK;
    ICCPIN_EPKCertitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPIN_EPKCert, &ICCPIN_EPKCertbExist);
    ICCPIN_EPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPIN_EPKExp, &ICCPIN_EPKExpbExist);
    ICCPKCertitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPKCert, &ICCPKCertbExist);
    ICCPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPKExp, &ICCPKExpbExist);


    if(ICCPIN_EPKCertbExist == 1 && ICCPIN_EPKExpbExist == 1)
    {
		if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen&& tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul)
        {
            return RLT_EMV_OK;
        }

        if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen == 0 || tempEmvContact_UnionStruct->EmvTradeParam->IPKModul == NULL)
        {
    		if(tempcapk == NULL)
			{
				return EMVCONTACT_ERR_CVMFail;
			}

	        if(tempcapk->ModulLen == 0)
	        {
	            return EMVCONTACT_ERR_CVMFail;
	        }

            if(EmvContact_GetIPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)			//retrieval of the issuer public key modulus
            {
                return EMVCONTACT_ERR_CVMFail;
            }
        }

        if(EmvContact_GetICCPIN_EPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)		//retrieval of the ICC PIN encipher public key
        {
            return EMVCONTACT_ERR_CVMFail;
        }
        return RLT_EMV_OK;
    }
    else if(ICCPKCertbExist && ICCPKExpbExist)
    {
    	if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen && tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul)
        {
            return RLT_EMV_OK;
        }

        if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModulLen == 0 || tempEmvContact_UnionStruct->EmvTradeParam->IPKModul == NULL)
        {
        	if(tempcapk == NULL)
			{
				return EMVCONTACT_ERR_CVMFail;
			}

	        if(tempcapk->ModulLen == 0)
	        {
	            return EMVCONTACT_ERR_CVMFail;
	        }

            if(EmvContact_GetIPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)		//retrieval of the issuer public key modulus
            {
                return EMVCONTACT_ERR_CVMFail;
            }
        }

        if(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen == 0 || tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul == NULL)
        {
            if(EmvContact_GetICCPK(tempEmvContact_UnionStruct) != RLT_EMV_OK)		//retrieval of the ICC public key
            {
                return EMVCONTACT_ERR_CVMFail;
            }
        }

        return RLT_EMV_OK;
    }
    else
    {
        return EMVCONTACT_ERR_CVMFail;
    }
}

unsigned char EmvContact_CommandGetChallenge(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, "\x00\x84\x00\x00", 4);

    apdu_s->Lc = 0;
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;
    return RLT_EMV_OK;
}

unsigned char EmvContact_CommandOfflineEnc(EMVBASE_APDU_SEND *apdu_s, unsigned char *EncPIN, unsigned char EncPINLen)
{
    memcpy(apdu_s->Command, "\x00\x20\x00\x88", 4);
    apdu_s->Lc = EncPINLen;
    memcpy(apdu_s->DataIn, EncPIN, EncPINLen);
    apdu_s->Le = 0;
    apdu_s->EnableCancel = 1;
    return RLT_EMV_OK;
}


unsigned char EmvContact_CVMPlainTextEncPin(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned short index;
    unsigned short DataLen;

    unsigned char offlineplaintext[20];
    unsigned char retCode;
    unsigned char tempPIN[8], PINLen, i;
    unsigned char Challenge[8];
    unsigned char *RandPad, RandLen;
    unsigned char *PINData, *EncPIN, EncPINLen;
    unsigned char ICCPIN_EPKExpbExist, ICCPIN_EPKCertbExist, ICCPKExpbExist;
    EMVBASETAGCVLITEM *ICCPIN_EPKExpitem = NULL, *ICCPKExpitem = NULL;
    unsigned char externalpinpadflag = 0;
	unsigned int externalpinpaddatalen = 0;


    while(1)
    {
        retCode = EmvContact_GetPinTryCount(tempEmvContact_UnionStruct);

        if(retCode != RLT_EMV_OK)
        {
            return retCode;
        }
        retCode = EmvContact_GetPINEncipherEPK(tempEmvContact_UnionStruct);

		if(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul)
		{
			emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->IPKModul);
			tempEmvContact_UnionStruct->EmvTradeParam->IPKModul = NULL;
		}

		if(tempEmvContact_UnionStruct->EmvTradeParam->CAPK)
		{
			emvbase_free(tempEmvContact_UnionStruct->EmvTradeParam->CAPK);
			tempEmvContact_UnionStruct->EmvTradeParam->CAPK = NULL;
		}

        if(retCode != RLT_EMV_OK)
        {
            return EMVCONTACT_ERR_CVMFail;
        }
        EmvContact_CommandGetChallenge(&apdu_s);
        tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

        if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
        {
            return RLT_ERR_EMV_APDUTIMEOUT;
        }

        if((!(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)) || (apdu_r.LenOut != 8))
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
            return EMVCONTACT_ERR_CVMFail;
        }
        memcpy(Challenge, apdu_r.DataOut, 8);



        memset(offlineplaintext, 0, sizeof(offlineplaintext));
        retCode = tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_OFFLINEENCPIN, offlineplaintext);


        if(retCode != RLT_EMV_OK)
        {
            if(retCode == RLT_ERR_EMV_CancelTrans)
            {
                return RLT_ERR_EMV_CancelTrans;
            }
            else if(retCode == RLT_ERR_EMV_InputBYPASS)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x08);
                emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0xDF);

               // return EMVCONTACT_ERR_CVMFail;
               return EMVCONTACT_ERR_Bypass;
            }
            else
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                return EMVCONTACT_ERR_CVMFail;
            }
        }

        //verify pin input
        PINLen = strlen((char *)offlineplaintext);
        tempPIN[0] = 0x20 + PINLen;

        for(i = 1; i < 8; i++)
        {
            tempPIN[i] = 0xFF;
        }
		for(i = 0;i < PINLen;i++)
		{
			if(offlineplaintext[i] < 0x30 || offlineplaintext[i] > 0x39)//è¯´æ˜Žè°ƒçš„æ˜¯å®‰å…¨çš„è¾“å¯†ç æŽ¥å£
			{
				externalpinpadflag = 1;
				break;
			}
		}

		if(externalpinpadflag == 0)
		{
        	EmvContact_PIN_asc_to_bcd((unsigned char*)&tempPIN[1], offlineplaintext, strlen((char *)offlineplaintext));
		}

		ICCPIN_EPKCertbExist = emvbase_avl_checkiftagexist(EMVTAG_ICCPIN_EPKCert);
		ICCPIN_EPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPIN_EPKExp, &ICCPIN_EPKExpbExist);

		if(ICCPIN_EPKCertbExist && ICCPIN_EPKExpbExist)
		{
			EncPINLen = tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen;
			RandLen = EncPINLen;
	        RandPad = (unsigned char *)emvbase_malloc(RandLen);
		}
		else
		{
	        EncPINLen = tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen;
			RandLen = EncPINLen;
	        RandPad = (unsigned char *)emvbase_malloc(RandLen);
		}

		if(RandLen > 17)
        {
            RandLen -= 17;
            tempEmvContact_UnionStruct->EMVB_RandomNum(RandPad, RandLen);
        }

        PINData = (unsigned char *)emvbase_malloc(EncPINLen+17);

        PINData[0] = 0x7F;
        memcpy((unsigned char*)&PINData[1], tempPIN, 8);
        memcpy((unsigned char*)&PINData[9], Challenge, 8);
        memcpy((unsigned char*)&PINData[17], RandPad, RandLen);

		EMVBase_Trace("EmvContact_SDVerify--RandLen: %d\r\n", RandLen);
		EMVBase_TraceHex("EmvContact_CVMPlainTextEncPin-PINData: ", PINData, RandLen+17);

        emvbase_free(RandPad);

		if(externalpinpadflag == 0)
		{
			EncPIN = (unsigned char *)emvbase_malloc(EncPINLen);
			memset(EncPIN,0,EncPINLen);
			if(ICCPIN_EPKCertbExist && ICCPIN_EPKExpbExist)
			{
				EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen, ICCPIN_EPKExpitem->data, ICCPIN_EPKExpitem->len, PINData, EncPIN);
			}
			else
			{
				ICCPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPKExp, &ICCPKExpbExist);
				EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen, ICCPKExpitem->data, ICCPKExpitem->len, PINData, EncPIN);
			}
		}
		else
		{
	        if(ICCPIN_EPKCertbExist && ICCPIN_EPKExpbExist)
	        {
//				EncPINLen = 8+1+tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen+1+ICCPIN_EPKExpitem->len+1;
				EMVBase_TraceHex("EmvContact_CVMPlainTextEncPin-Challenge: ", Challenge, 8);
				EMVBase_TraceHex("EmvContact_CVMPlainTextEncPin-ICCPINPKModul: ", tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen);
				EMVBase_TraceHex("EmvContact_CVMPlainTextEncPin-Expitem: ", ICCPIN_EPKExpitem->data, ICCPIN_EPKExpitem->len);


				EncPIN = (unsigned char *)emvbase_malloc(300);

				externalpinpaddatalen = 0;
				memcpy(&EncPIN[externalpinpaddatalen],Challenge,8);
	            externalpinpaddatalen += 8;
		        EncPIN[externalpinpaddatalen++] = tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen;
	            memcpy(&EncPIN[externalpinpaddatalen],tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModul,tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen);
				externalpinpaddatalen += tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen;

				EncPIN[externalpinpaddatalen++] = ICCPIN_EPKExpitem->len;
				memcpy(&EncPIN[externalpinpaddatalen],ICCPIN_EPKExpitem->data,ICCPIN_EPKExpitem->len);
				externalpinpaddatalen += ICCPIN_EPKExpitem->len;

				EncPIN[externalpinpaddatalen++] = RandLen;

				EncPINLen = tempEmvContact_UnionStruct->EmvTradeParam->ICCPINPKModulLen;
	        }
	        else
	        {
	        	ICCPKExpitem = emvbase_avl_gettagitemandstatus(EMVTAG_ICCPKExp, &ICCPKExpbExist);

//				EncPINLen = 8+1+tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen+1+ICCPKExpitem->len+1;
				EncPIN = (unsigned char *)emvbase_malloc(300);

				externalpinpaddatalen = 0;
				memcpy(&EncPIN[externalpinpaddatalen],Challenge,8);
	            externalpinpaddatalen += 8;
		        EncPIN[externalpinpaddatalen++] = tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen;
	            memcpy(&EncPIN[externalpinpaddatalen],tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul,tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
				externalpinpaddatalen += tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen;

				EncPIN[externalpinpaddatalen++] = ICCPKExpitem->len;
				memcpy(&EncPIN[externalpinpaddatalen],ICCPKExpitem->data,ICCPKExpitem->len);
				externalpinpaddatalen += ICCPKExpitem->len;
				EncPIN[externalpinpaddatalen++] = RandLen;

				EncPINLen = tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen;
			}

			if(EncPINLen)
			{
				if(tempEmvContact_UnionStruct->getofflineencdata != NULL)
				{
					(*tempEmvContact_UnionStruct->getofflineencdata)(EncPIN,externalpinpaddatalen);
					EncPINLen = 0;
				}
			}
		}
        EmvContact_CommandOfflineEnc(&apdu_s, EncPIN, EncPINLen);

        emvbase_free(PINData);
        emvbase_free(EncPIN);

		/*if(externalpinpadflag) // may be move to Ex
		{
        	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, 0);
		}
		else*/
		{
        	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
		}

        if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
        {
            return RLT_ERR_EMV_APDUTIMEOUT;
        }

        if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
        {
            offlineplaintext[0] = 0;
            tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_INPUTPINSTATUS, offlineplaintext);
            emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0x7F);
            return RLT_EMV_OK;
        }
        else if(apdu_r.SW1 == 0x69 && (apdu_r.SW2 == 0x83 || apdu_r.SW2 == 0x84))
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
            return EMVCONTACT_ERR_CVMFail;
        }
        else if(apdu_r.SW1 == 0x63 && (apdu_r.SW2 & 0xF0) == 0xC0)
        {
            offlineplaintext[0] = 1;
            offlineplaintext[1] = apdu_r.SW2 - 0xC0;
            emvbase_avl_settag(EMVTAG_PINTryCount, offlineplaintext[1]);
            tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_INPUTPINSTATUS, offlineplaintext);

            if(apdu_r.SW2 == 0xC0)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
                return EMVCONTACT_ERR_CVMFail;
            }
        }
        else
        {
            return RLT_ERR_EMV_IccReturn;
        }
    }

    return RLT_EMV_OK;
}


unsigned char EmvContact_CVMCardHolderID(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char CardHolderIdTypebExist, CardHolderIdNobExist;
    EMVBASETAGCVLITEM *CardHolderIdTypeitem = NULL, *CardHolderIdNoitem = NULL;
    unsigned char retCode;

    CardHolderIdTypeitem = emvbase_avl_gettagitemandstatus(EMVTAG_CardHoldIdType, &CardHolderIdTypebExist);
    CardHolderIdNoitem = emvbase_avl_gettagitemandstatus(EMVTAG_CardHoldIdNo, &CardHolderIdNobExist);

    if(CardHolderIdNobExist == 0 || CardHolderIdTypebExist == 0)
    {
        return EMVCONTACT_ERR_CVMFail;
    }

    if(CardHolderIdTypeitem->data[0] > 5 || CardHolderIdNoitem->len == 0)
    {
        return EMVCONTACT_ERR_CVMFail;
    }
    retCode = tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DATAAUTH_VERIFYIDCARD, NULL);

    if(retCode == RLT_EMV_OK)
    {
        return RLT_EMV_OK;
    }
    else
    {
        return EMVCONTACT_ERR_CVMFail;
    }
}

unsigned char EmvContact_CommandCDCVMStatus(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, "\x80\xCA\xDF\x45", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    return RLT_EMV_OK;
}

unsigned char EmvContact_GetCDCVMStatus(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned short index;
    unsigned short DataLen;

    if(emvbase_avl_checkiftagexist(EMVTAG_DPASCDCVMStatus))
    {
        return RLT_EMV_OK;
    }

    EmvContact_CommandCDCVMStatus(&apdu_s);
    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
    {
        if(memcmp(apdu_r.DataOut, "\xDF\x45", 2) == 0)
        {
            index = 2;
            EMVBase_ParseExtLen(apdu_r.DataOut, &index, &DataLen);

            if(index + DataLen == apdu_r.LenOut)
            {
                if(DataLen == 1)
                {
                    emvbase_avl_settag(EMVTAG_DPASCDCVMStatus, apdu_r.DataOut[index]);
					return RLT_EMV_OK;
                }
            }
        }
    }

    return EMVCONTACT_ERR_CVMFail;
}

unsigned char EmvContact_CommandCVR(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, "\x80\xCA\x9F\x52", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;

    return RLT_EMV_OK;
}

unsigned char EmvContact_GetCVR(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned short index;
    unsigned short DataLen;

    if(emvbase_avl_checkiftagexist(EMVTAG_DPASCVR))
    {
        return RLT_EMV_OK;
    }

    EmvContact_CommandCVR(&apdu_s);
    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00)
    {
        if(memcmp(apdu_r.DataOut, "\x9F\x52", 2) == 0)
        {
            index = 2;
            EMVBase_ParseExtLen(apdu_r.DataOut, &index, &DataLen);

            if(index + DataLen == apdu_r.LenOut)
            {
                if(DataLen == 7)
                {
                    emvbase_avl_createsettagvalue(EMVTAG_DPASCVR, &(apdu_r.DataOut[index]), 7);
					return RLT_EMV_OK;
                }
            }
        }
    }

    return EMVCONTACT_ERR_CVMFail;
}

unsigned char EmvContact_CVMCDCVM(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	unsigned char retCode = 0;
	unsigned char CDCVMStatus = 0;
    unsigned char CVRbExist = 0;
    EMVBASETAGCVLITEM *CVRitem = NULL;


	retCode = EmvContact_GetCDCVMStatus(tempEmvContact_UnionStruct);
	if(retCode != RLT_EMV_OK)
	{
	    return retCode;
	}

	CDCVMStatus = emvbase_avl_gettagvalue(EMVTAG_DPASCDCVMStatus);
	if(!(CDCVMStatus & 0x10))
	{
		tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE;
		return RLT_EMV_TERMINATE_TRANSERR;
	}

	retCode = EmvContact_GetCVR(tempEmvContact_UnionStruct);
	if(retCode != RLT_EMV_OK)
	{
	    return retCode;
	}

    CVRitem = emvbase_avl_gettagitemandstatus(EMVTAG_DPASCVR, &CVRbExist);
    if(CVRbExist == 0)
    {
        return EMVCONTACT_ERR_CVMFail;
    }

    if(CVRitem->data[4] & 0x20)
    {
    	emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x20);
        return EMVCONTACT_ERR_CVMFail;
    }
	if(!(CVRitem->data[6] & 0x02))
    {
    	retCode = tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_DISP_PERFORMCDCVM, NULL);

        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE;
		return RLT_EMV_TERMINATE_TRANSERR;
    }
	if(CVRitem->data[6] & 0x01)
    {
        return EMVCONTACT_ERR_CVMFail;
    }

	return RLT_EMV_OK;
}

unsigned char EmvContact_PerformCVM(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char method, unsigned char condition, unsigned char *TermCapab,unsigned char BypassFlag)
{
    unsigned char retCode = 0;
    unsigned char temp=0;
    unsigned char CVMResult[3]={0};
    temp = method & 0x3F;

    switch(temp)
    {
         case 0x00:
            {CVMResult[2] = CVMR_FAIL;
             retCode = EMVCONTACT_ERR_CVMFailALWAYS; }
                                              break;

         case 0x01:           //PLAINTEXT PIN VERIFICATION PERFORMED BY ICC
            {if((TermCapab[1] & 0x80) == 0)
             {
                 emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                 return EMVCONTACT_ERR_CVMNoSupport;
             }
             {
                 if(BypassFlag) //add for Subsequent Bypass PIN Entry
                 {
                    retCode = EMVCONTACT_ERR_Bypass;
                 }
                 else
                 {
                    retCode = EmvContact_CVMPlainTextPin(tempEmvContact_UnionStruct);

                 }

                 if(retCode == RLT_EMV_OK)
                 {
                     CVMResult[2] = CVMR_SUCCESS;
                 }
                 else if(retCode == EMVCONTACT_ERR_CVMFail || retCode == EMVCONTACT_ERR_Bypass)
                 {
                     CVMResult[2] = CVMR_FAIL;
                 }
             }}
              break;

         case 0x02:          //enciphered PIN verification online
            {if((TermCapab[1] & 0x40) == 0)
             {
                 emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                 return EMVCONTACT_ERR_CVMNoSupport;
             }

			 //input online enc pin
             {

                 if(BypassFlag)
                 {
                    retCode = EMVCONTACT_ERR_Bypass;
                 }
                 else
                 {
                    retCode = EmvContact_CVMOnlineEncPin(tempEmvContact_UnionStruct);
                 }

                 if(retCode == RLT_EMV_OK)
                 {
                     CVMResult[2] = CVMR_UNKNOWN;
                 }
                 else if(retCode == EMVCONTACT_ERR_CVMFail || retCode == EMVCONTACT_ERR_Bypass )
                 {
                     CVMResult[2] = CVMR_FAIL;
                 }
             }

            }
            break;

         case 0x03:         //Plaintext PIN verification performed by ICC and signature(paper)
            {if((TermCapab[1] & 0xA0) != 0xA0)
             {
                 if((TermCapab[1] & 0x80) == 0)
                 {
                     emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                 }
                 return EMVCONTACT_ERR_CVMNoSupport;
             }

             {

                 if(BypassFlag)
                 {
                    retCode = EMVCONTACT_ERR_Bypass;
                 }
                 else
                 {
                    retCode = EmvContact_CVMPlainTextPin(tempEmvContact_UnionStruct);

                 }

                 if(retCode == RLT_EMV_OK)
                 {
                     CVMResult[2] = CVMR_UNKNOWN;
                     tempEmvContact_UnionStruct->EmvTradeParam->bPrintReceipt = 1;
                 }
                 else if(retCode == EMVCONTACT_ERR_CVMFail  || retCode == EMVCONTACT_ERR_Bypass )
                 {
                     CVMResult[2] = CVMR_FAIL;
                 }
             }}
              break;

         case 0x04:         //enciphered PIN verification performed by ICC
            {if((TermCapab[1] & 0x10) == 0)
             {
                 if((TermCapab[1] & 0x80) == 0)
                 {

                     emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                 }
                 return EMVCONTACT_ERR_CVMNoSupport;
             }
             {
                 if(BypassFlag)
                 {
                    retCode = EMVCONTACT_ERR_Bypass;
                 }
                 else
                 {
                    retCode = EmvContact_CVMPlainTextEncPin(tempEmvContact_UnionStruct);

                 }


                 if(retCode == RLT_EMV_OK)
                 {
                     CVMResult[2] = CVMR_SUCCESS;
                 }
                 else if(retCode == EMVCONTACT_ERR_CVMFail || retCode == EMVCONTACT_ERR_Bypass)
                 {
                     CVMResult[2] = CVMR_FAIL;
                 }
             }}
              break;

         case 0x05:        //enciphered PIN verification performed by ICC and signature(paper)
            {if((TermCapab[1] & 0x30) != 0x30)
             {
                 if((TermCapab[1] & 0x80) == 0)
                 {
                     emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x10);
                 }
                 return EMVCONTACT_ERR_CVMNoSupport;
             }

             {
                 if(BypassFlag)
                 {
                    retCode = EMVCONTACT_ERR_Bypass;
                 }
                 else
                 {
                    retCode = EmvContact_CVMPlainTextEncPin(tempEmvContact_UnionStruct);

                 }

                 if(retCode == RLT_EMV_OK)
                 {
                     CVMResult[2] = CVMR_UNKNOWN;
					 tempEmvContact_UnionStruct->EmvTradeParam->bPrintReceipt = 1;
                 }
                 else if(retCode == EMVCONTACT_ERR_CVMFail || retCode == EMVCONTACT_ERR_Bypass)
                 {
                     CVMResult[2] = CVMR_FAIL;
                 }
             }}
              break;

         case 0x1E:       //signature (paper)
            {
                if((TermCapab[1] & 0x20) == 0)
                {
                    return EMVCONTACT_ERR_CVMNoSupport;
                }
                tempEmvContact_UnionStruct->EmvTradeParam->bPrintReceipt = 1;
                CVMResult[2] = CVMR_UNKNOWN;
                retCode = RLT_EMV_OK;
            }
            break;

         case 0x1F:       //no CVM required
            {
                if((TermCapab[1] & 0x08) == 0)
                {
                    return EMVCONTACT_ERR_CVMNoSupport;
                }
                emvbase_avl_setvalue_and(EMVTAG_TVR, 2, 0x7F);
                CVMResult[2] = CVMR_SUCCESS;
                retCode = RLT_EMV_OK;
            }
            break;

         case 0x20:
            {
                if((TermCapab[1] & 0x01) == 0)
                {
                    return EMVCONTACT_ERR_CVMNoSupport;
                }
                retCode = EmvContact_CVMCardHolderID(tempEmvContact_UnionStruct);

                if(retCode == RLT_EMV_OK)
                {
                    CVMResult[2] = CVMR_SUCCESS;
                }
                else if(retCode == EMVCONTACT_ERR_CVMFail)
                {
                    CVMResult[2] = CVMR_FAIL;
                }
            }
            break;

		 case 0x21:
            {
                retCode = EmvContact_CVMCDCVM(tempEmvContact_UnionStruct);

                if(retCode == RLT_EMV_OK)
                {
                    CVMResult[2] = CVMR_SUCCESS;
                }
                else if(retCode == EMVCONTACT_ERR_CVMFail)
                {
                    CVMResult[2] = CVMR_FAIL;
                }
            }
            break;

         default:
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x40);
                CVMResult[2] = CVMR_FAIL;
                return EMVCONTACT_ERR_CVMFail;
            }
            break;
    }

    CVMResult[0] = method;
    CVMResult[1] = condition;

    emvbase_avl_createsettagvalue(EMVTAG_CVMResult, CVMResult, 3);

    return retCode;
}


unsigned char EmvContact_CardHolderVerf(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    CVMSTRCUT CVMdata;
    unsigned char NowCVRListLen = 0;
    unsigned char needcvm, nextcvm;
    unsigned short needcvmnextcvm;
    unsigned char TransType, TermType;
    unsigned char TermCapab[3];
    unsigned char BypassFlag = 0;

    memset(&CVMdata, 0, sizeof(CVMSTRCUT));
    retCode = EmvContact_InitCardHolderVerify(tempEmvContact_UnionStruct, &CVMdata);

    if(retCode == RLT_EMV_OK)
    {
        return RLT_EMV_OK;
    }

    TransType = emvbase_avl_gettagvalue(EMVTAG_TransTypeValue);
    TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);

    memset(TermCapab, 0, sizeof(TermCapab));
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

	EMVBase_TraceHex("TermCapab 9F33",TermCapab,3);
	EMVBase_Trace("TransType 9C:%02X\r\n",TransType);
	EMVBase_Trace("TermType 9F35:%02X\r\n",TermType);

    while(NowCVRListLen < CVMdata.CVRListLen)
    {
        needcvmnextcvm = 0;
        needcvmnextcvm = EmvContact_AnalyCvmlist(CVMdata.CVRList[NowCVRListLen].method, CVMdata.CVRList[NowCVRListLen].condition, TransType, TermType, CVMdata.CVM_X, CVMdata.CVM_Y);


        needcvm = needcvmnextcvm & 0xff;
        nextcvm = (needcvmnextcvm >> 8) & 0xff;

        if(needcvm == 1)
        {
            retCode = EmvContact_PerformCVM(tempEmvContact_UnionStruct, CVMdata.CVRList[NowCVRListLen].method, CVMdata.CVRList[NowCVRListLen].condition, TermCapab, BypassFlag);

            if(retCode == RLT_EMV_OK)
            {
                emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x40);
                return retCode;
            }

            if(retCode == EMVCONTACT_ERR_Bypass)//add for subsequent bypass
            {
                BypassFlag = 1;
            }

            if(retCode == RLT_ERR_EMV_IccReturn || retCode == RLT_ERR_EMV_CancelTrans
				|| retCode == RLT_ERR_EMV_APDUTIMEOUT || retCode == RLT_EMV_TERMINATE_TRANSERR)
            {
                return retCode;
            }

            if(retCode == EMVCONTACT_ERR_CVMFailALWAYS)
            {
                emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x40);
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
                return RLT_EMV_OK;
            }

            if((CVMdata.CVRList[NowCVRListLen].method & 0x40) == 0x00)
            {
                emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x40);
                emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);
                return RLT_EMV_OK;
            }
            else{ nextcvm = 1; }}

        if(nextcvm == 1)
        {
            (NowCVRListLen)++;
        }
    }

    emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x40);
    emvbase_avl_setvalue_or(EMVTAG_TVR, 2, 0x80);	//set "Cardholder verification was not successful"
    return RLT_EMV_OK;
}

void EmvContact_RandTransSelect(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	unsigned int MinLimit,ThresholdMoney,TermAmtAuthBin;
	unsigned char TransTargetPercent;
	unsigned char bRandSelected;
	unsigned char FloorLimit[4];
	unsigned char Threshold[4];
	unsigned char RandomNum;
	unsigned char bShowRandNum;
	unsigned char AmtAuthNum[6];
	unsigned char MaxTargetPercent,TargetPercent;
    unsigned char  TVR[5];

    memset(FloorLimit,0,sizeof(FloorLimit));
    emvbase_avl_gettagvalue_spec(EMVTAG_FloorLimit,FloorLimit,0,4);
	memset(Threshold,0,sizeof(Threshold));
    emvbase_avl_gettagvalue_spec(EMVTAG_Threshold,Threshold,0,4);

	memset(AmtAuthNum,0,sizeof(AmtAuthNum));
    emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum,AmtAuthNum,0,6);
	EMVBaseBcdToU32(&TermAmtAuthBin,AmtAuthNum,6);

	MinLimit = EmvContact_ByteArrayToInt(FloorLimit,4);
	ThresholdMoney = EmvContact_ByteArrayToInt(Threshold,4);

	EMVBase_Trace("Tradeamount:%d Floorlimit:%d\r\n",TermAmtAuthBin,MinLimit);

	if(TermAmtAuthBin >= MinLimit)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR, 3, 0x80); //set 'transaction exceeds floor limit' bit 1.
		return;
	}

    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

    if(TVR[3] & 0x80)
    {
        return;
    }

	bRandSelected=0;


	tempEmvContact_UnionStruct->EMVB_RandomNum(&RandomNum,1);
	RandomNum = (unsigned char)(RandomNum%99+1);


	if(emvbase_avl_gettagvalue(EMVTAG_bShowRandNum))
	{
		emvbase_avl_settag(EMVTAG_bShowRandNum,RandomNum);
	}

	MaxTargetPercent = emvbase_avl_gettagvalue(EMVTAG_MaxTargetPercent);
	TargetPercent = emvbase_avl_gettagvalue(EMVTAG_TargetPercent);


	if(TermAmtAuthBin<ThresholdMoney)
	{
		if(RandomNum<=TargetPercent) bRandSelected=1;
	}
	else
	{
		if(MinLimit<ThresholdMoney+1) return;
		TransTargetPercent=(unsigned char)((MaxTargetPercent-TargetPercent)*(TermAmtAuthBin-ThresholdMoney)/(MinLimit-ThresholdMoney)+TargetPercent);
		if(RandomNum<=TransTargetPercent) bRandSelected=1;
	}

	if(bRandSelected==1)
	{
	    emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x10); //set 'Transaction selected randomly for online processing' bit 1.
	}

}

unsigned char EmvContact_CommandReadATC(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command,"\x80\xCA\x9f\x36",4);
	apdu_s->Lc=0;
	apdu_s->Le=256;

	apdu_s->EnableCancel = 1;
	return RLT_EMV_OK;
}

unsigned char EmvContact_CommandReadLOATC(EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command,"\x80\xCA\x9f\x13",4);
	apdu_s->Lc=0;
	apdu_s->Le=256;

	apdu_s->EnableCancel = 1;
	return RLT_EMV_OK;
}

unsigned char EmvContact_CheckVelocity(unsigned char bATCExist,unsigned char bLOATCExist)
{
	unsigned short counts=0,nAtc = 0,nLoatc = 0;
	unsigned char LOATC[2],ATC[2];
    unsigned char LCOL=0,UCOL=0;

	if(bATCExist==0 || bLOATCExist==0)	//ATC or LOATC is missing
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
		emvbase_avl_setvalue_or(EMVTAG_TVR,0,0x20);//set 'ICC data missing' bit 1.
	}

    memset(LOATC,0,sizeof(LOATC));
	emvbase_avl_gettagvalue_spec(EMVTAG_LOATC,LOATC,0,2);


	if(bLOATCExist==1 && LOATC[0]==0 && LOATC[1]==0)
		emvbase_avl_setvalue_or(EMVTAG_TVR,1,0x08);	//set 'New Card' bit 1

    memset(ATC,0,sizeof(ATC));
	emvbase_avl_gettagvalue_spec(EMVTAG_ATC,ATC,0,2);
	// ATC and LOATC are high byte ahead,low byte behind.
	nAtc = nAtc + ATC[0]*256+ATC[1];
	nLoatc = nLoatc + LOATC[0]*256+LOATC[1];

	if(nAtc<=nLoatc)
	{
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
		emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
	}
	else
	{
		counts=nAtc-nLoatc;
		LCOL = emvbase_avl_gettagvalue(EMVTAG_LCOL);
		UCOL = emvbase_avl_gettagvalue(EMVTAG_UCOL);//20190531_lhd


		if(counts > LCOL)
			emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x40);//set 'Lower consecutive online limit exceeded' bit 1.
		if(counts > UCOL)
			emvbase_avl_setvalue_or(EMVTAG_TVR,3,0x20);//set 'Upper consecutive online limit exceeded' bit 1.
	}

    return RLT_EMV_OK;
}

unsigned char EmvContact_TermRiskManage(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	unsigned char retCode = RLT_EMV_OK;
	unsigned char bLOATCExist = 0,bATCExist = 0;
    EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;

	emvbase_avl_setvalue_or(EMVTAG_TSI,0,0x08);

    EmvContact_RandTransSelect(tempEmvContact_UnionStruct);

    if(emvbase_avl_checkiftagexist(EMVTAG_LCOL) == 0|| emvbase_avl_checkiftagexist(EMVTAG_UCOL) == 0)
    {
    	return RLT_EMV_OK;
    }

	EmvContact_CommandReadATC(&apdu_s);
    tempEmvContact_UnionStruct->IsoCommand(&apdu_s,&apdu_r);
    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
    	return RLT_ERR_EMV_APDUTIMEOUT;
    }
    if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00)
	{
		if(!memcmp(apdu_r.DataOut,"\x9F\x36",2))		// ATC found
		{
		    bATCExist = 1;
		    emvbase_avl_createsettagvalue(EMVTAG_ATC,(unsigned char*)&apdu_r.DataOut[3],2);
		}
	}

    EmvContact_CommandReadLOATC(&apdu_s);
	 tempEmvContact_UnionStruct->IsoCommand(&apdu_s,&apdu_r);
    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
    	return RLT_ERR_EMV_APDUTIMEOUT;
    }
    if(apdu_r.SW1==0x90 && apdu_r.SW2==0x00)
	{
		if(!memcmp(apdu_r.DataOut,"\x9F\x13",2))// ATC found
		{
		    bLOATCExist = 1;
		    emvbase_avl_createsettagvalue(EMVTAG_LOATC,(unsigned char*)&apdu_r.DataOut[3],2);
		}
	}

    retCode = EmvContact_CheckVelocity(bATCExist,bLOATCExist);

	return RLT_EMV_OK;

}


unsigned char EmvContact_CheckIfdoCDA(void)
{
    unsigned char AIP[2];
    unsigned char TermCapab[3];
    unsigned char TVR[5];

    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);

    memset(TermCapab, 0, sizeof(TermCapab));
    emvbase_avl_gettagvalue_spec(EMVTAG_TermCapab, TermCapab, 0, 3);

    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

    if((AIP[0] & 0x01) && (TermCapab[2] & 0x08) && ((TVR[0] & 0x04) == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char EmvContact_TermActAnaly(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char i, k;
    unsigned char bFitIAC, bFitTAC;
    Card_TAC CardTAC;

    unsigned char TVR[5];
    unsigned char TACDenial[5];
    unsigned char IACDenial[5], IACDenialbExist;
    unsigned char TermType;
    unsigned char TACOnline[5], IACOnline[5], IACOnlinebExist;
    unsigned char TACDefault[5], IACDefault[5], IACDefaultbExist;
    unsigned char bExist;


	if(tempEmvContact_UnionStruct->EmvTradeParam->bRequestAAC)
	{
		return TAA_DENIAL;
	}

    IACDenialbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACDenial, IACDenial, 0, 5));

    if(IACDenialbExist == 0)   //IAC-denial not exist
    {
        memset(IACDenial, 0, 5);
    }
    memcpy(CardTAC.IACDenial, IACDenial, 5);


    IACOnlinebExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACOnline, IACOnline, 0, 5));

    if(IACOnlinebExist == 0)   //IAC-online not exist
    {
        memset(CardTAC.IACOnline, 0xFF, 5);
        memset(IACOnline, 0, 5);
    }
    else{ memcpy(CardTAC.IACOnline, IACOnline, 5); }
    IACDefaultbExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_IACDefault, IACDefault, 0, 5));

    if(IACDefaultbExist == 0)   //IAC-default not exist
    {
        memset(CardTAC.IACDefault, 0xFF, 5);
        memset(IACDefault, 0, 5);
    }
    else
    {
        memcpy(CardTAC.IACDefault, IACDefault, 5);   //lechan 20070207
    }
    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACDenial, TACDenial, 0, 5));

    if(bExist == 0)
    {
        memset(TACDenial, 0, 5);
    }
    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACOnline, TACOnline, 0, 5));

    if(bExist == 0)
    {
        memset(TACOnline, 0, 5);
        TACOnline[0] |= 0xC8;      //EMVapp,p35
    }
    bExist = !(emvbase_avl_gettagvalue_spec(EMVTAG_TACDefault, TACDefault, 0, 5));

    if(bExist == 0)
    {
        memset(TACDefault, 0, 5);
        TACDefault[0] |= 0xC8;
    }
    memset(TVR, 0, sizeof(TVR));
    emvbase_avl_gettagvalue_spec(EMVTAG_TVR, TVR, 0, 5);

	EMVBase_TraceHex("TVR",TVR,5);
	EMVBase_TraceHex("TACDefault",TACDefault,5);
	EMVBase_TraceHex("TACOnline",TACOnline,5);
	EMVBase_TraceHex("TACDenial",TACDenial,5);
	EMVBase_TraceHex("IACDefault",IACDefault,5);
	EMVBase_TraceHex("IACOnline",IACOnline,5);
	EMVBase_TraceHex("IACDenial",IACDenial,5);

    if(!memcmp(TVR, "\x00\x00\x00\x00\x00", 5)) { return TAA_OFFLINE; }
    bFitIAC = 0;
    bFitTAC = 0;

    for(i = 0; i < 5; i++)
    {
        k = TVR[i];

        if((k & CardTAC.IACDenial[i]) != 0) { bFitIAC = 1; }

        if((k & TACDenial[i]) != 0) { bFitTAC = 1; }
    }

    if(bFitIAC || bFitTAC) { return TAA_DENIAL; }

    TermType = emvbase_avl_gettagvalue(EMVTAG_TermType);
	EMVBase_Trace("TermType: %02x\r\n", TermType);
    TermType = TermType & 0x0F;

    if((TermType == 1 || TermType == 2 || TermType == 4 || TermType == 5) &&
       tempEmvContact_UnionStruct->EmvTradeParam->bAbleOnline)          //Terminal has Online capability
    {
		EMVBase_Trace("Terminal ActAnaly\r\n");
        bFitIAC = 0;
        bFitTAC = 0;

        for(i = 0; i < 5; i++)
        {
            k = TVR[i];

            if((k & CardTAC.IACOnline[i]) != 0) { bFitIAC = 1; }

            if((k & TACOnline[i]) != 0) { bFitTAC = 1; }
        }

        if(bFitIAC || bFitTAC) { return TAA_ONLINE; }
        else{ return TAA_OFFLINE; }
    }

	EMVBase_Trace("Unable Online Terminal ActAnaly\r\n");

    bFitIAC = 0;
    bFitTAC = 0;

    for(i = 0; i < 5; i++)
    {
        k = TVR[i];

        if((k & CardTAC.IACDefault[i]) != 0) { bFitIAC = 1; }

        if((k & TACDefault[i]) != 0) { bFitTAC = 1; }
    }


    if(bFitIAC || bFitTAC) { return TAA_DENIAL; }
    else{ return TAA_OFFLINE; }
}


void EmvContact_BcdSub(unsigned char *Dest,unsigned char *Src,unsigned char Len)
{
    unsigned char i;
    unsigned char high,low;
    unsigned char carry=0;

    for(i=Len;i>0;i--)
    {
         if( (*(Dest+i-1) & 0x0F) < ( (*(Src+i-1) & 0x0F) + carry) )
         {
            low = ((*(Dest+i-1) & 0x0F)+0x0A) - (*(Src+i-1) & 0x0F) - carry;
            carry=1;
         }
         else
         {
            low = (*(Dest+i-1) & 0x0F) - (*(Src+i-1) & 0x0F) - carry;
            carry=0;
         }

         if( ((*(Dest+i-1) & 0xF0) >> 4) < (((*(Src+i-1) & 0xF0) >> 4) + carry) )
         {
            high = (((*(Dest+i-1) & 0xF0) >> 4)+0x0A) - ((*(Src+i-1) & 0xF0) >> 4)  - carry;
            carry=1;
         }
         else
         {
            high = ((*(Dest+i-1) & 0xF0) >> 4) - ((*(Src+i-1) & 0xF0) >> 4)  - carry;
            carry=0;
         }
         *(Dest+i-1) = (high << 4) | low;
    }
}

unsigned char EmvContact_Ec_LessThanResetAmount(void)
{
    unsigned char AmtAuthNum[6];
    unsigned char VLPAvailableFund[6];
    unsigned char SubAmount[6];
    unsigned char ECResetThreshold[6];


    if(emvbase_avl_checkiftagexist(EMVTAG_VLPIssuAuthorCode))
    {
        memset(AmtAuthNum, 0, sizeof(AmtAuthNum));
        emvbase_avl_gettagvalue_spec(EMVTAG_AmtAuthNum, AmtAuthNum, 0, 6);

        memset(VLPAvailableFund, 0, sizeof(VLPAvailableFund));
        emvbase_avl_gettagvalue_spec(EMVTAG_VLPAvailableFund, VLPAvailableFund, 0, 6);

        memset(ECResetThreshold, 0, sizeof(ECResetThreshold));
        emvbase_avl_gettagvalue_spec(EMVTAG_ECResetThreshold, ECResetThreshold, 0, 6);

        memcpy(SubAmount, VLPAvailableFund, 6);
        EmvContact_BcdSub(SubAmount, AmtAuthNum, 6);

        if(memcmp(SubAmount, ECResetThreshold, 6) < 0)
        {
            return 1;
        }
    }
    return 0;
}

unsigned char EmvContact_CommandGAC1(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char TermAnaResult, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char retCode;
    unsigned short len;
    unsigned char SubAmount[6];
    unsigned char bCombineDDA = 0;
    unsigned char CDOL1bExist;
    EMVBASETAGCVLITEM *item;


    memcpy(apdu_s->Command, "\x80\xAE", 2);
    tempEmvContact_UnionStruct ->EmvTradeParam->Ec_LessThanResetAmount = 0;
    if(TermAnaResult == TAA_DENIAL)
    {
        memcpy((unsigned char*)&apdu_s->Command[2], "\x00\x00", 2);      //get AAC
        emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, "Z1", 2);


        if(EmvContact_CheckIfdoCDA())
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x80);
            emvbase_avl_setvalue_and(EMVTAG_TSI, 0, 0x7F);
        }
    }
    else if(TermAnaResult == TAA_ONLINE)
    {
        if(EmvContact_Ec_LessThanResetAmount())        //modify for EC
        {
            tempEmvContact_UnionStruct ->EmvTradeParam->Ec_LessThanResetAmount = 1;
        }
        memcpy(&apdu_s->Command[2], "\x80\x00", 2);      //get ARQC
    }
    else if(TermAnaResult == TAA_OFFLINE)
    {
        if(EmvContact_Ec_LessThanResetAmount())        //modify for EC
        {
            memcpy(&apdu_s->Command[2], "\x80\x00", 2);          //get ARQC
            tempEmvContact_UnionStruct ->EmvTradeParam->Ec_LessThanResetAmount = 1;
        }
        else
        {
            memcpy(&apdu_s->Command[2], "\x40\x00", 2);          //get TC
            emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, "Y1", 2);
        }
    }
    else
    {
        return RLT_EMV_ERR;
    }

    if(TermAnaResult == TAA_ONLINE || TermAnaResult == TAA_OFFLINE)
    {
        if(EmvContact_CheckIfdoCDA())
        {
            apdu_s->Command[2] |= 0x10;          //Combined DDA/AC requested.
        }
    }
    emvbase_avl_settag(EMVTAG_ReferenceControlParameter, apdu_s->Command[2]);

    item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL1, &CDOL1bExist);

    if(CDOL1bExist == 0)
    {
        return RLT_EMV_ERR;
    }
    retCode = EmvContact_DOLProcess(typeCDOL1, item->data, item->len, apdu_s->DataIn, &len, 255,tempEmvContact_UnionStruct);//20201217 lsy

    emvbase_avl_createsettagvalue(EMVTAG_CDOL1RelatedData, apdu_s->DataIn, len);

    apdu_s->Lc = len;
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;

    return retCode;
}


unsigned char EmvContact_GenerateAC(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, EMVBASE_APDU_RESP *apdu_r)
{
    unsigned char ret;
    unsigned char k, j, t, indexRet = 0;
    unsigned short index, indexTemp, len, lenTemp, orgindex, tmplen, startpos;
    unsigned char Primitivetype = 0;
    unsigned char countpadding = 0;
    unsigned char bIntable;
    unsigned char GenerateACRetDataflag;
    unsigned char CryptInfo, reqCrpt;
    unsigned char VLPAvailableFund[6];
    EMVBASETAGCVLITEM *item;

    tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetDataLen = 0;
    memset(tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetData, 0, 255);



    if(apdu_r->SW1 == 0x69 && apdu_r->SW2 == 0x85)
    {
        return EMVCONTACT_ERR_NEEDMAGCARD;
    }
    else if(!(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00))
    {
        return RLT_ERR_EMV_IccReturn;
    }

    emvbase_avl_deletetag(EMVTAG_SignDynAppData);
    emvbase_avl_deletetag(EMVTAG_CryptInfo);
    emvbase_avl_deletetag(EMVTAG_ATC);
    emvbase_avl_deletetag(EMVTAG_AppCrypt);

    index = 0;

    if(apdu_r->DataOut[index] == 0x80)		//primitive data
    {
        index++;

        if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenTemp))
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(apdu_r->LenOut != index + lenTemp)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(lenTemp < 11) { return RLT_ERR_EMV_IccDataFormat; }

        while(apdu_r->DataOut[index + countpadding] == 0)
        {
            countpadding++;

            if(countpadding > 2)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }

        emvbase_avl_settag(EMVTAG_CryptInfo, apdu_r->DataOut[index]);
        index++;
        emvbase_avl_createsettagvalue(EMVTAG_ATC, &apdu_r->DataOut[index], 2);
        index += 2;
        emvbase_avl_createsettagvalue(EMVTAG_AppCrypt, &apdu_r->DataOut[index], 8);
        index += 8;

        if(apdu_r->LenOut > index)
        {
            k = apdu_r->LenOut - index;

            if(k > 32) { return RLT_ERR_EMV_IccDataFormat; }
            emvbase_avl_createsettagvalue(EMVTAG_IssuAppData, (unsigned char*)&apdu_r->DataOut[index], k);
        }
    }
    else if(apdu_r->DataOut[index] == 0x77)  //TLV coded data
    {
        Primitivetype = 1;
        index++;

        if(EMVBase_ParseExtLen(apdu_r->DataOut, &index, &lenTemp))
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
        indexTemp = index;
        indexRet = 0;

        while(index < indexTemp + lenTemp)
        {
            if(index >= apdu_r->LenOut)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }

            if(apdu_r->DataOut[index] == 0xFF || apdu_r->DataOut[index] == 0x00)
            {
                index++;
                continue;
            }
            orgindex = index;

            bIntable = 0;
            GenerateACRetDataflag = 1;

            if(!memcmp((unsigned char*)&apdu_r->DataOut[index], "\x9F\x4B", 2))
            {
                GenerateACRetDataflag = 0;
            }

            ret = EmvC_ParseAndStoreCardResponse(tempEmvContact_UnionStruct, apdu_r->DataOut, &index, &bIntable, 0);

            if(ret != RLT_EMV_OK)
            {
                return ret;
            }

            if(GenerateACRetDataflag && (indexRet + index - orgindex <= 255))
            {
                memcpy(tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetData + indexRet, (unsigned char*)&apdu_r->DataOut[orgindex], index - orgindex);
                indexRet += index - orgindex;
            }
    	}

        if(index != indexTemp + lenTemp)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
    }
    else
    {

        return RLT_ERR_EMV_IccDataFormat;
    }

    if(emvbase_avl_checkiftagexist(EMVTAG_IssuAppData) && emvbase_avl_checkiftagexist(EMVTAG_VLPIssuAuthorCode))
    {
        item = emvbase_avl_gettagitempointer(EMVTAG_IssuAppData);

        if(item != NULL)
        {
            if(item->len >= 14)
            {
                memset(VLPAvailableFund, 0, sizeof(VLPAvailableFund));
                memcpy(&VLPAvailableFund[1], &item->data[item->len - 9], 5);
                emvbase_avl_createsettagvalue(EMVTAG_VLPAvailableFund, VLPAvailableFund, 6);
            }
        }
    }

    if((emvbase_avl_checkiftagexist(EMVTAG_CryptInfo) == 0) ||
       (emvbase_avl_checkiftagexist(EMVTAG_ATC) == 0))
    {
        return RLT_ERR_EMV_IccDataFormat;
    }

    if(Primitivetype == 0)
    {
        if(emvbase_avl_checkiftagexist(EMVTAG_AppCrypt) == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }
    }
    tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetDataLen = indexRet;

    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);
    reqCrpt = emvbase_avl_gettagvalue(EMVTAG_ReferenceControlParameter);

    if((CryptInfo & 0xc0) == 0x40 && (reqCrpt & TAA_OFFLINE))
    {
    }
    else if((CryptInfo & 0xc0) == 0x80 && ((reqCrpt & TAA_OFFLINE) || (reqCrpt & TAA_ONLINE)))
    {
    }
    else if((CryptInfo & 0xc0) == 0x00)
    {
    }
    else
    {
        return RLT_EMV_ONLINE_DECLINE;
    }

    //NO CDA
    if(emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData) == 0)
    {
        if(emvbase_avl_checkiftagexist(EMVTAG_AppCrypt) == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if((CryptInfo & 0xc0) == 0x00)       //AAC
        {
            if((reqCrpt & 0xC0) == 0x00)
            {
                if(reqCrpt & 0x10)
                {
                    return RLT_ERR_EMV_IccDataFormat;
                }
            }
        }
        else
        {
            if(reqCrpt & 0x10)
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }
    }
    ret = RLT_EMV_OK;

    emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x20);


    if((CryptInfo & 0x07) == 0x01) 				//Service not allowed
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 1, 0x10);
        ret = RLT_ERR_EMV_NotAccept;
    }

    if((CryptInfo & 0x08) == 0x08)              //Advice required
    {
        tempEmvContact_UnionStruct->EmvTradeParam->bAdvice = 1;
    }

    return ret;
}

unsigned char EmvContact_TermiAnalys(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    unsigned char TermAnaResult;
    unsigned short len;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char CDOL1bExist;
    EMVBASETAGCVLITEM *item = NULL;



    item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL1, &CDOL1bExist);

    if(CDOL1bExist)
    {
        if(EmvContact_DOLProcess(typeCDOL1, item->data, item->len, apdu_s.DataIn, &len, 255,tempEmvContact_UnionStruct) != RLT_EMV_OK)//20201217 lsy
        {
            if(EmvContact_CheckIfdoCDA())
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);
                emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);

                EmvContact_DOLProcess(typeCDOL1, item->data, item->len, apdu_s.DataIn, &len, 255,tempEmvContact_UnionStruct);//20201217 lsy
            }
        }
    }
    TermAnaResult = EmvContact_TermActAnaly(tempEmvContact_UnionStruct);

    retCode = EmvContact_CommandGAC1(tempEmvContact_UnionStruct, TermAnaResult, &apdu_s);

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL1Data)
	{
    	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL1Data);
	}
	else
	{
    	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
	}

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
    	return RLT_ERR_EMV_APDUTIMEOUT;
    }

    if(EmvContact_CheckIfdoCDA())
    {
        EMVBase_Trace("chenjun test1\r\n");
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);
    }
    return EmvContact_GenerateAC(tempEmvContact_UnionStruct, &apdu_r);
}

unsigned char EmvContact_AacRefuse(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char respCrpt;
    unsigned char CryptInfo = 0;

    respCrpt = emvbase_avl_gettagvalue(EMVTAG_ReferenceControlParameter);

    tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;

    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if((CryptInfo & 0x08) == 0x08)		//Advice required
    {
        tempEmvContact_UnionStruct->EmvTradeParam->bAdvice = 1;
    }

    if((respCrpt & 0x10) != 0)
    {
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);
    }
    return RLT_EMV_OK;
}

unsigned char EmvContact_SM_SM3Encrypt(unsigned char *pInput, unsigned short usLen, unsigned char *pOutput)
{
//    if(32 != SM3Encrypt(pInput, usLen, pOutput))
//    {
//        return RLT_EMV_ERR;
//    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_SM_CombineDDASignVerify(unsigned char type, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *SDAToSign, CDAHash[32];
    unsigned int index = 0;
    SM_SIGN_DYN_APPDATA SMSSA;
    SM_ICC_DYN_DATA SMICCDynData;
    EMVBASETAGCVLITEM *SignDynAppDataitem, *DOLDataitem;
    unsigned char UnpredictNum[4];
    unsigned char CryptInfo;
	unsigned char retcode=0;
	unsigned char offsetNum = 0;
	unsigned short offset[2] = {0};


    if((emvbase_avl_checkiftagexist(EMVTAG_CryptInfo) == 0) || (emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData) == 0)
       || (emvbase_avl_checkiftagexist(EMVTAG_ATC) == 0))
    {
        return EMVCONTACT_ERR_EMV_CDAFORMAT;
    }
    memset((unsigned char*)&SMICCDynData, 0, sizeof(ICC_DYN_DATA));
    memset((unsigned char*)&SMSSA, 0, sizeof(SM_SIGN_DYN_APPDATA));
    memset(CDAHash, 0, 20);


    SignDynAppDataitem = emvbase_avl_gettagitempointer(EMVTAG_SignDynAppData);

    if(SignDynAppDataitem == NULL)
    {
        return RLT_EMV_ERR;
    }

    index = 0;
    SMSSA.DataFormat = SignDynAppDataitem->data[index++];
    SMSSA.ICCDynDataLen = SignDynAppDataitem->data[index++];

    if(SignDynAppDataitem->len != (2 + SMSSA.ICCDynDataLen + tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen))
    {
        return RLT_EMV_ERR;
    }
    memcpy(SMSSA.ICCDynData, &SignDynAppDataitem->data[index], SMSSA.ICCDynDataLen);
    index += SMSSA.ICCDynDataLen;
    memcpy(SMSSA.DGTLSGNTR, &SignDynAppDataitem->data[index], tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen;

    if(index != SignDynAppDataitem->len)
    {
        return RLT_EMV_ERR;
    }

    if(0x15 != SMSSA.DataFormat)
    {
        return RLT_EMV_ERR;
    }

    index = 0;
    SMICCDynData.ICCDynNumLen = SMSSA.ICCDynData[index++];
    memcpy(SMICCDynData.ICCDynNum, (unsigned char *)&SMSSA.ICCDynData[index], SMICCDynData.ICCDynNumLen);
    index += SMICCDynData.ICCDynNumLen;
    SMICCDynData.CryptInfo = SMSSA.ICCDynData[index++];
    memcpy(SMICCDynData.AppCrypt, (unsigned char *)&SMSSA.ICCDynData[index], 8);
    index += 8;
    memcpy(SMICCDynData.HashResult, (unsigned char *)&SMSSA.ICCDynData[index], 32);
    index += 32;

    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if(SMICCDynData.CryptInfo != CryptInfo)
    {
		if((SMICCDynData.CryptInfo & 0xC0) == 0x80)
        {
            return EMVCONTACT_ERR_EMV_CDADifferCID;
        }
        else if((SMICCDynData.CryptInfo & 0xC0) == 0x40)
        {
            return EMVCONTACT_ERR_EMV_CDADifferCIDTC;
        }
        else
        {
            //return RLT_EMV_ERR;
			return EMVCONTACT_ERR_EMV_CDADifferCIDAAC;
        }
    }

    SDAToSign = (unsigned char *)emvbase_malloc(SMSSA.ICCDynDataLen + 6);
    memset(SDAToSign, 0, (SMSSA.ICCDynDataLen + 6));
    index = 0;


    SDAToSign[index++] = SMSSA.DataFormat;
    SDAToSign[index++] = SMSSA.ICCDynDataLen;
    memcpy((unsigned char *)&SDAToSign[index], SMSSA.ICCDynData, SMSSA.ICCDynDataLen);
    index += SMSSA.ICCDynDataLen;
    emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UnpredictNum, 0, 4);
    memcpy((unsigned char*)&SDAToSign[index], UnpredictNum, 4);
    index += 4;

////    if(RLT_EMV_OK != EMVBase_SM_SignatureVerf(SMSSA.DGTLSGNTR, SDAToSign, index, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen))
//	retcode = EMVBase_SM_SignatureVerf(SMSSA.DGTLSGNTR, SDAToSign, index, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
	if(RLT_EMV_OK != retcode)
    {
    	EMVBase_Trace("EMVBase_SM_SignatureVerf retcode=%d",retcode);
        emvbase_free(SDAToSign);
        return RLT_EMV_ERR;
    }
    emvbase_free(SDAToSign);

    //SM3 HASH
    SDAToSign = (unsigned char *)emvbase_malloc(1024);
    memset(SDAToSign, 0, 1024);
    memset(CDAHash, 0, 32);
    index = 0;

    DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_PDOLData);

    if(DOLDataitem != NULL)
    {
        if(DOLDataitem->len)
        {
            memcpy(SDAToSign, DOLDataitem->data, DOLDataitem->len);
            index += DOLDataitem->len;
        }
    }
    DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_CDOL1RelatedData);

    if(DOLDataitem != NULL)
    {
        if(DOLDataitem->len)
        {
        	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL1Data)
			{
				offset[offsetNum] = index + tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL1Data;
				offsetNum++;
			}

            memcpy(SDAToSign + index, DOLDataitem->data, DOLDataitem->len);
            index += DOLDataitem->len;
        }
    }

    if(type == GenerateAC2)
    {
        DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_CDOL2RelatedData);

        if(DOLDataitem != NULL)
        {
            if(DOLDataitem->len)
            {
            	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data)
				{
					offset[offsetNum] = index + tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL2Data;
					offsetNum++;
				}

                memcpy(SDAToSign + index, DOLDataitem->data, DOLDataitem->len);
                index += DOLDataitem->len;
            }
        }
    }
    memcpy(SDAToSign + index, tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetData, tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetDataLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetDataLen;

	#if 1//20190604_lhd
	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && offsetNum) // for tag57 mask
	{
		//EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x01, offsetNum, offset, index, SDAToSign, NULL, CDAHash);

		tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,  0x01, offsetNum, offset, index, SDAToSign, NULL, CDAHash);
	}
	else
	{
		if(RLT_EMV_OK != EmvContact_SM_SM3Encrypt(SDAToSign, (unsigned short)index, CDAHash))
	    {
	        EMVBase_Trace("emv","CDA SM3 fail");
	        emvbase_free(SDAToSign);
	        return RLT_EMV_ERR;
	    }
	}
	#endif

    emvbase_free(SDAToSign);

    if(memcmp(SMICCDynData.HashResult, CDAHash, 32))
    {
    	EMVBase_TraceHex("SMICCDynData.HashResult",SMICCDynData.HashResult,32);
    	EMVBase_TraceHex("CDAHash",CDAHash,32);

        return RLT_EMV_ERR;
    }

    emvbase_avl_createsettagvalue(EMVTAG_ICCDynNum, SMICCDynData.ICCDynNum, SMICCDynData.ICCDynNumLen);
    emvbase_avl_createsettagvalue(EMVTAG_AppCrypt, SMICCDynData.AppCrypt, 8);
    return RLT_EMV_OK;
}


void EmvContact_FillICCDynData(unsigned char* DynData, ICC_DYN_DATA* ICCDynData)
{
    unsigned char Ldn;

    Ldn = DynData[0];
    ICCDynData->ICCDynNumLen = Ldn;
    memcpy(ICCDynData->ICCDynNum, DynData + 1, Ldn);
    ICCDynData->CryptInfo = DynData[Ldn + 1];
    memcpy(ICCDynData->AppCrypt, DynData + Ldn + 2, 8);
    memcpy(ICCDynData->HashResult, DynData + Ldn + 10, 20);
}

unsigned char EmvContact_CombineDDASignVerify(unsigned char type, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char *SDAToSign, SDAHash[20] = {0}, *SDAData, i = 0;
    unsigned int index;
    SIGN_DYN_APPDATA_RECOVER *recovSDA;
    ICC_DYN_DATA ICCDynDataSign;
    EMVBASETAGCVLITEM *SignDynAppDataitem, *ICCPKExpitem, *DOLDataitem;
    unsigned char CryptInfo;
    unsigned char UnpredictNum[4];
	unsigned char offsetNum = 0;
	unsigned short offset[2] = {0};
	EMVBASETAGCVLITEM *item = NULL;
	EMVBASETAGCVLITEM *item2 = NULL;


    if((emvbase_avl_checkiftagexist(EMVTAG_CryptInfo) == 0) || (emvbase_avl_checkiftagexist(EMVTAG_SignDynAppData) == 0)
       || (emvbase_avl_checkiftagexist(EMVTAG_ATC) == 0))
    {
		EMVBase_Trace("EMVCONTACT_ERR_EMV_CDAFORMAT error! \r\n");
        return EMVCONTACT_ERR_EMV_CDAFORMAT;
    }
    memset((unsigned char*)&ICCDynDataSign, 0, sizeof(ICC_DYN_DATA));

    memset(SDAHash, 0, 20);

    SignDynAppDataitem = emvbase_avl_gettagitempointer(EMVTAG_SignDynAppData);

	EMVBase_Trace("SignDynAppDataitem->len: %d\r\n", SignDynAppDataitem->len);
	EMVBase_Trace("tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen: %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen);
    if(SignDynAppDataitem->len != tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen)
    {
		EMVBase_Trace("SignDynAppDataitem->len != tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen error! \r\n");
        return RLT_EMV_ERR;
    }
    ICCPKExpitem = emvbase_avl_gettagitempointer(EMVTAG_ICCPKExp);

    SDAData = (unsigned char *)emvbase_malloc(248);
    EMVBase_RSARecover(tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModul, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen, ICCPKExpitem->data, ICCPKExpitem->len, SignDynAppDataitem->data, SDAData);

    recovSDA = (SIGN_DYN_APPDATA_RECOVER *)emvbase_malloc(sizeof(SIGN_DYN_APPDATA_RECOVER));


    if(EmvContact_FillSDA(SDAData, recovSDA, tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen) != RLT_EMV_OK)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
		EMVBase_Trace("EmvContact_FillSDA error! \r\n");
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataTrail != 0xBC)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
		EMVBase_Trace("recovSDA->DataTrail != 0xBC error! \r\n");
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataHead != 0x6A)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
		EMVBase_Trace("recovSDA->DataHead != 0x6A error! \r\n");
        return RLT_EMV_ERR;
    }

    if(recovSDA->DataFormat != 0x05)
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAData);
		EMVBase_Trace("recovSDA->DataFormat != 0x05 error! \r\n");
        return RLT_EMV_ERR;
    }

    EmvContact_FillICCDynData(recovSDA->ICCDynData, &ICCDynDataSign);

	if(tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable)
	{
		if(memcmp(recovSDA->ICCDynData + (recovSDA->ICCDynDataLen-20), tempEmvContact_UnionStruct->EmvTradeParam->DSDHashValue, 20))
	    {
	        emvbase_free(recovSDA);
            emvbase_free(SDAData);
	        return RLT_EMV_ERR;
	    }
	}

	if(tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable || tempEmvContact_UnionStruct->EmvTradeParam->bExtendedLoggingEnable)
	{
		item = emvbase_avl_gettagitempointer(EMVTAG_CardID);
		if(NULL == item)
		{
			emvbase_free(recovSDA);
            emvbase_free(SDAData);
	        return RLT_EMV_ERR;
		}

		item2 = emvbase_avl_gettagitempointer(EMVTAG_DPASCardFeatureDescriptor);
		if(memcmp(item2->data + 3, item->data, item->len))
	    {
	        emvbase_free(recovSDA);
            emvbase_free(SDAData);
	        return RLT_EMV_ERR;
	    }
	}

    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if(ICCDynDataSign.CryptInfo != CryptInfo)
    {
		EMVBase_Trace("ICCDynDataSign.CryptInfo=%02X,CryptInfo=%02X  \r\n",ICCDynDataSign.CryptInfo,CryptInfo);
        if((ICCDynDataSign.CryptInfo & 0xC0) == 0x80)
        {
            emvbase_free(recovSDA);
            emvbase_free(SDAData);
            return EMVCONTACT_ERR_EMV_CDADifferCID;
        }
        else if((ICCDynDataSign.CryptInfo & 0xC0) == 0x40)
        {
            emvbase_free(recovSDA);
            emvbase_free(SDAData);
            return EMVCONTACT_ERR_EMV_CDADifferCIDTC;
        }
        else
        {
            emvbase_free(recovSDA);
            emvbase_free(SDAData);
            //return RLT_EMV_ERR;
            return EMVCONTACT_ERR_EMV_CDADifferCIDAAC;
        }
    }

    SDAToSign = (unsigned char *)emvbase_malloc(512);
    index = 0;
    memset(SDAToSign, 0, 512);
    memcpy(SDAToSign, (unsigned char*)&SDAData[1], tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen - 22);
    index += tempEmvContact_UnionStruct->EmvTradeParam->ICCPKModulLen - 22;

    emvbase_free(SDAData);

#if 0
    if(GenerateAC2 == type)
    {
        i = tempEmvContact_UnionStruct->EmvTradeParam->bCDOL2HasNoUnpredictNum;
    }

    if(GenerateAC1 == type)
    {
        i = tempEmvContact_UnionStruct->EmvTradeParam->bCDOL1HasNoUnpredictNum;
    }

    if(i == 0)
    {
        emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UnpredictNum, 0, 4);
        memcpy((unsigned char*)&SDAToSign[index], UnpredictNum, 4);
        index += 4;
    }
    else
    {
    }
#else
	emvbase_avl_gettagvalue_spec(EMVTAG_UnpredictNum, UnpredictNum, 0, 4);
	memcpy((unsigned char*)&SDAToSign[index], UnpredictNum, 4);
	index += 4;
#endif
    if(recovSDA->HashInd != 0x01)		//SHA-1 algorithm
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAToSign);
		EMVBase_Trace("recovSDA->HashInd != 0x01 error! \r\n");
        return RLT_EMV_ERR;
    }
    EMVBase_Hash(SDAToSign, index, SDAHash);


    if(memcmp(recovSDA->HashResult, SDAHash, 20))
    {
        emvbase_free(recovSDA);
        emvbase_free(SDAToSign);
		EMVBase_Trace("recovSDA->HashResult error! \r\n");
        return RLT_EMV_ERR;
    }
    emvbase_free(recovSDA);

    memset(SDAToSign, 0, 512);
    memset(SDAHash, 0, 20);
    index = 0;

    DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_PDOLData);

    if(DOLDataitem != NULL)
    {
        if(DOLDataitem->len)
        {
            memcpy(SDAToSign, DOLDataitem->data, DOLDataitem->len);
            index += DOLDataitem->len;
        }
    }

    DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_CDOL1RelatedData);

    if(DOLDataitem != NULL)
    {
        if(DOLDataitem->len)
        {
        	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL1Data)
			{
				offset[offsetNum] = index + tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL1Data;
				offsetNum++;
			}

            memcpy(SDAToSign + index, DOLDataitem->data, DOLDataitem->len);
            index += DOLDataitem->len;
        }
    }

    if(type == GenerateAC2)
    {
        DOLDataitem = emvbase_avl_gettagitempointer(EMVTAG_CDOL2RelatedData);

        if(DOLDataitem != NULL)
        {
            if(DOLDataitem->len)
            {
            	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data)
				{
					offset[offsetNum] = index + tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL2Data;
					offsetNum++;
				}

                memcpy(SDAToSign + index, DOLDataitem->data, DOLDataitem->len);
                index += DOLDataitem->len;
            }
        }
    }

    memcpy(SDAToSign + index, tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetData, tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetDataLen);
    index += tempEmvContact_UnionStruct->EmvTradeParam->GenerateACRetDataLen;

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && offsetNum) // for tag57 mask
	{
//		EmvContact_GetHashEx(tempEmvContact_UnionStruct, 0x00, offsetNum, offset, index, SDAToSign, NULL, SDAHash);
        EMVBase_Trace("emv-info:tag57 is mask, CDOL contains tag57\r\n");
		tempEmvContact_UnionStruct->GetHashWithMagMask(tempEmvContact_UnionStruct->EmvTradeParam->Track2DataLen,0x00, offsetNum, offset, index, SDAToSign, NULL, SDAHash);
	}
	else
	{

        EMVBase_Trace("emv-info:get hash,public\r\n");
    	EMVBase_Hash(SDAToSign, index, SDAHash);
	}

    emvbase_free(SDAToSign);
    EMVBase_TraceHex("emv-info:recovered hash value", ICCDynDataSign.HashResult, 20);

    EMVBase_TraceHex("emv-info:calculated hash value", SDAHash, 20);
    if(memcmp(ICCDynDataSign.HashResult, SDAHash, 20))
    {
		EMVBase_Trace("ICCDynDataSign.HashResult error! \r\n");
        return RLT_EMV_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_ICCDynNum, ICCDynDataSign.ICCDynNum, ICCDynDataSign.ICCDynNumLen);
    emvbase_avl_createsettagvalue(EMVTAG_AppCrypt, ICCDynDataSign.AppCrypt, 8);


    return RLT_EMV_OK;
}


unsigned char EmvContact_CombineDDA(unsigned char type, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;

    if(tempEmvContact_UnionStruct->EmvTradeParam->ODASMstaus)
    {
        retCode = EmvContact_SM_CombineDDASignVerify(type, tempEmvContact_UnionStruct);
    }
    else
    {
        retCode = EmvContact_CombineDDASignVerify(type, tempEmvContact_UnionStruct);
    }

    if(retCode != RLT_EMV_OK)
    {
        return retCode;
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_TcOffLine(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char respCrpt, retCode;
    unsigned char bCombineDDAFail = 0;

    tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_APPROVE;

    respCrpt = emvbase_avl_gettagvalue(EMVTAG_ReferenceControlParameter);

    if((respCrpt & 0x10) != 0)
    {
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);

        retCode = EmvContact_CombineDDA(GenerateAC1, tempEmvContact_UnionStruct);

        if(retCode != RLT_EMV_OK)
        {
            if(retCode == EMVCONTACT_ERR_EMV_CDAFORMAT)
            {
                return retCode;
            }
            bCombineDDAFail = 1;
        }

        if(bCombineDDAFail)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);

            if(retCode == EMVCONTACT_ERR_EMV_CDADifferCID)
            {
                return EMVCONTACT_ERR_EMV_CDADifferCID;
            }
            else
            {
                tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
                return RLT_EMV_OK;
            }
        }
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_ArQcOnline(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char respCrpt, retCode = RLT_EMV_OK;

    respCrpt = emvbase_avl_gettagvalue(EMVTAG_ReferenceControlParameter);
    tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_ONLINE_WAIT;

    if((respCrpt & 0x10) != 0)
    {
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);                              //Offline Data Authentication was performed
        retCode = EmvContact_CombineDDA(GenerateAC1, tempEmvContact_UnionStruct);

        if(retCode != RLT_EMV_OK)
        {
            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);

            if(retCode == EMVCONTACT_ERR_EMV_CDADifferCIDTC)
            {
                tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
                return RLT_EMV_OK;
            }
			else if(EMVCONTACT_ERR_EMV_CDADifferCIDAAC == retCode)//20190530_lhd
			{
				tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
                return RLT_EMV_OK;
			}
            else
            {
                return EMVCONTACT_ERR_EMV_CDADifferCID;
            }
        }
        retCode = RLT_EMV_OK;
    }
    return retCode;
}

unsigned char EmvContact_CommandGAC2(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct, unsigned char TermAnaResult, EMVBASE_APDU_SEND *apdu_s)
{
    unsigned char retCode;    //TermAnaResult,CardAnaResult;//0-Denial,1-Online,2-Offline
    unsigned short len;
    unsigned char CDOL2bExist;
    EMVBASETAGCVLITEM *item = NULL;


    memcpy(apdu_s->Command, "\x80\xAE", 2);

    if(TermAnaResult == TAA_DENIAL)
    {
        memcpy((unsigned char*)&apdu_s->Command[2], "\x00\x00", 2);      //get AAC
    }
    else
    {
        return RLT_EMV_ERR;
    }
    emvbase_avl_createsettagvalue(EMVTAG_ReferenceControlParameter, &apdu_s->Command[2], 1);

    item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL2, &CDOL2bExist);

    if(CDOL2bExist == 0)
    {
        return RLT_EMV_ERR;
    }
    retCode = EmvContact_DOLProcess(typeCDOL2, item->data, item->len, apdu_s->DataIn, &len,255, tempEmvContact_UnionStruct);//20201217 lsy

    emvbase_avl_createsettagvalue(EMVTAG_CDOL2RelatedData, apdu_s->DataIn, len);

    apdu_s->Lc = len;
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;

    return RLT_EMV_OK;
}

unsigned char EmvContact_TransProcess(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char CryptInfo;


    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if((CryptInfo & 0xC0) == 0x00)
    {
        retCode = EmvContact_AacRefuse(tempEmvContact_UnionStruct);
    }
    else if((CryptInfo & 0xC0) == 0x40)
    {
        retCode = EmvContact_TcOffLine(tempEmvContact_UnionStruct);
    }
    else if((CryptInfo & 0xC0) == 0x80)
    {
        retCode = EmvContact_ArQcOnline(tempEmvContact_UnionStruct);
    }

    if(retCode != EMVCONTACT_ERR_EMV_CDADifferCID)
    {
        return retCode;
    }
    emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, "Z1", 2);
    retCode = EmvContact_CommandGAC2(tempEmvContact_UnionStruct, TAA_DENIAL, &apdu_s);

    if(retCode != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_IccDataFormat;
    }
	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data)
	{
    	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL2Data);
	}
	else
	{
    	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
	}

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }
    retCode = EmvContact_GenerateAC(tempEmvContact_UnionStruct, &apdu_r);

    if(retCode != RLT_EMV_OK)
    {
        return retCode;
    }
    tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
    return retCode;
}


unsigned char EmvContact_CommandIssuerAuthen(EMVBASE_APDU_SEND *apdu_s, unsigned char *IssuAuthenData, unsigned char IssuAuthenDataLen)
{
    memcpy(apdu_s->Command, "\x00\x82\x00\x00", 4);

    memcpy(apdu_s->DataIn, IssuAuthenData, IssuAuthenDataLen);
    apdu_s->Lc = IssuAuthenDataLen;
    apdu_s->Le = 0;  //no Le


    apdu_s->EnableCancel = 0;
    return RLT_EMV_OK;
}

unsigned char EmvContact_IssuerAuthen(EMVBASE_APDU_RESP *apdu_r)
{
    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)
    {
        return RLT_EMV_OK;
    }
    else
    {
        return RLT_ERR_EMV_IccReturn;
    }
}


unsigned char EmvContact_DealIssuerAuthenData(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode = RLT_EMV_OK;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;

    EMVBASETAGCVLITEM *item;
    unsigned char AIP[2];


    memset(AIP, 0, sizeof(AIP));
    emvbase_avl_gettagvalue_spec(EMVTAG_AIP, AIP, 0, 2);

    if((AIP[0] & 0x04) == 0)
    {
        return RLT_EMV_OK;
    }
    item = emvbase_avl_gettagitempointer(EMVTAG_IssuerAuthenData);

    if(item == NULL)
    {
        return RLT_EMV_OK;
    }

    if(item->len == 0)
    {
        return RLT_EMV_OK;
    }
    emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x10);  //Issuer authentication was performed
    EmvContact_CommandIssuerAuthen(&apdu_s, item->data, item->len);

    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk == RLT_EMV_OK)
    {
        retCode = EmvContact_IssuerAuthen(&apdu_r);
    }
    else
    {
        retCode = RLT_ERR_EMV_APDUTIMEOUT;

        emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x40);
        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE;
        return retCode;
    }

    if(retCode != RLT_EMV_OK)
    {
        emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x40);
    }
    return RLT_EMV_OK;
}


unsigned char EmvContact_ReadScriptNum(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned short index;
    unsigned short len;
    unsigned char j, k, retCode = RLT_EMV_OK;
    unsigned char IssuScriptNum;
    unsigned char i;
    unsigned char tag[4];
    unsigned char IssuScript[300];
    unsigned short IssuScriptLen;


    IssuScriptNum = 0;
    memset(tag, 0, sizeof(tag));

    for(i = 0; i < 2; i++)
    {
        if(i == 0)
        {
            tag[0] = 0x71;

            if(emvbase_avl_checkiftagexist(tag) == 0)
            {
                continue;
            }
        }
        else
        {
            tag[0] = 0x72;

            if(emvbase_avl_checkiftagexist(tag) == 0)
            {
                continue;
            }
        }
        retCode = RLT_EMV_OK;
        memset(IssuScript, 0, sizeof(IssuScript));
        emvbase_avl_gettagvalue_all(tag, IssuScript, &IssuScriptLen);
        index = 0;

        while(index < IssuScriptLen)
        {
            if(IssuScript[index] == 0x71 || IssuScript[index] == 0x72)
            {
                index++;

                if(IssuScript[index] <= 127)
                {
                    len = IssuScript[index];
                    len += 2;
                    k = 0;
                }
                else
                {
                    len = 0;
                    k = IssuScript[index] & 0x7F;

                    for(j = 1; j <= k; j++)
                    {
                        len = len * 256 + IssuScript[index + j];
                    }

                    len += k + 2;
                }
                index--;

                if(index + len > IssuScriptLen)
                {
                    retCode = EMVCONTACT_ERR_EMV_ScriptFormat;
                    break;
                }
                IssuScriptNum++;
                index += len;
            }
            else
            {
                retCode = EMVCONTACT_ERR_EMV_ScriptFormat;
                break;
            }
        }


        if(retCode != RLT_EMV_OK)
        {
            IssuScriptNum++;
        }
    }

    return IssuScriptNum;
}

unsigned char EmvContact_SendScriptCommand(unsigned char* Script, unsigned char ScriptLen, EMVBASE_APDU_SEND *apdu_s)
{
    memcpy(apdu_s->Command, Script, 4);
    memcpy(apdu_s->DataIn, (unsigned char*)&Script[5], Script[4]);
    apdu_s->Lc = Script[4];
    apdu_s->Le = 0;  //no Le
    apdu_s->EnableCancel = 0;

    return RLT_EMV_OK;
}

unsigned char EmvContact_ScriptProcess(unsigned char tag, EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned short lenScript, index, Scriptindex;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    EMVBASETAGCVLITEM *item;
    unsigned char *IssuScript;
    unsigned short lenScriptPerformed, IssuScriptlen;
    unsigned char *ScriptResult;
    unsigned char ScriptResultlen;
    unsigned char curScriptID[4];
    unsigned char ScriptK;
    unsigned char ifscriptlenoverfloor;
    unsigned char numScriptCom;
    unsigned char retCode = RLT_EMV_OK;
    unsigned char dealscripterr;
    unsigned char script_amount[6];
    unsigned char VLPAvailableFund[6] = {0};
    unsigned char VLPTransLimit[6] = {0};

	EMVBase_Trace("emv-info:comes to deal script\r\n");
	if(tempEmvContact_UnionStruct->EmvTradeParam->Transflow == TRANS_LOAD)
    {
        if(EmvContact_ReadScriptNum(tempEmvContact_UnionStruct) > 1)
        {
        	EMVBase_Trace("emv-error:script num >1\r\n");
            return EMVCONTACT_ERR_PBOCLOAD;
        }
    }

    if(tag == 0x71)
    {
        item = emvbase_avl_gettagitempointer(EMVTAG_SCRIPT1);
    }
    else if(tag == 0x72)
    {
        item = emvbase_avl_gettagitempointer(EMVTAG_SCRIPT2);
    }
    else
    {
        return RLT_EMV_OK;
    }

    if(item == NULL)
    {
    	EMVBase_Trace("emv-info:script is null\r\n");
        return RLT_EMV_OK;
    }

    if(item->len == 0)
    {
    	EMVBase_Trace("emv-info:script len = 0\r\n");
        return RLT_EMV_OK;
    }
    IssuScript = (unsigned char *)emvbase_malloc(512);
    ScriptResult = (unsigned char *)emvbase_malloc(300);

    index = 0;
    memset(IssuScript, 0, 512);

    if(item->len > 512)
    {
        IssuScriptlen = 512;
    }
    else
    {
        IssuScriptlen = item->len;
    }
    memcpy(IssuScript, item->data, IssuScriptlen);
	EMVBase_TraceHex("emv-info:IssuScript ",item->data,IssuScriptlen);
    ScriptResultlen = 0;
    memset(ScriptResult, 0, 300);


    lenScriptPerformed = 0;
    while(index < IssuScriptlen)
    {
        if(IssuScript[index] == tag)//71 or 72
        {
            index++;

            if(EMVBase_ParseExtLen(IssuScript, &index, &lenScript))
            {
            	EMVBase_Trace("emv-info,script parse error\r\n");
                retCode = EMVCONTACT_ERR_EMV_ScriptFormat;
                break;
            }
            Scriptindex = index;

            memset(curScriptID, 0, sizeof(curScriptID));

            ifscriptlenoverfloor = 1;
            numScriptCom = 0;
            dealscripterr = 0;

            while(index < (Scriptindex + lenScript))
            {
                if(ifscriptlenoverfloor)
                {
                    ifscriptlenoverfloor = 0;
					if(lenScript > 127)//add for script len 256--cj
					{
						 lenScriptPerformed += (lenScript + 3);
					}
					else
					{
						lenScriptPerformed += (lenScript + 2);
					}

                    if(lenScriptPerformed > MAXSCRIPTLEN)
                    {
                        dealscripterr = 1;
						EMVBase_Trace("emv-info:dealscripterr err1\r\n");
                        break;
                    }
                }

                if(!memcmp(&IssuScript[index], "\x9F\x18\x04", 3))
                {
                    memcpy(curScriptID, &IssuScript[index + 3], 4);
                    index += 7;
                }
                else if(!memcmp(&IssuScript[index], "\x9F\x18\x00", 3))
                {
                    index += 3;
                }
                else if(IssuScript[index] == 0x86)
                {

					if(IssuScript[index + 1] & 0x80)//add for script len 256--cj
					{
						ScriptK = IssuScript[index + 2];
						index = index + 1;
					}
					else
					{
						 ScriptK = IssuScript[index + 1];
					}

					#if 0
                    if((ScriptK < 5) || (IssuScript[index + 3] & 0x01) == 0x01
                       || (IssuScript[index + 3] & 0xF0) == 0x60 || (IssuScript[index + 3] & 0xF0) == 0x90 || ((ScriptK != IssuScript[index + 6] + 5)))
					#endif
					if((ScriptK < 5) || (IssuScript[index + 3] & 0x01) == 0x01
                       || (IssuScript[index + 3] & 0xF0) == 0x60 || (IssuScript[index + 3] & 0xF0) == 0x90 ||
                       (((ScriptK != IssuScript[index + 6]+5) && (ScriptK != IssuScript[index + 6]+6)) || ( (index+ScriptK+2) > (Scriptindex + lenScript))))
                    {

						EMVBase_Trace("emv-info:ScriptK = %d\r\n",ScriptK);
						EMVBase_Trace("emv-info:IssuScript[index + 3] & 0x01 = %02x\r\n",IssuScript[index + 3] & 0x01);
						EMVBase_Trace("emv-info:IssuScript[index + 3] & 0xF0 = %02x\r\n",IssuScript[index + 3] & 0xF0);
						EMVBase_Trace("emv-info:IssuScript[index + 6]+5 = %d\r\n",IssuScript[index + 6]+5);
						EMVBase_Trace("emv-info:IssuScript[index + 6]+6 = %d\r\n",IssuScript[index + 6]+6);
						EMVBase_Trace("emv-info:index+ScriptK+2 =%d   Scriptindex + lenScript = %d\r\n",index+ScriptK+2,Scriptindex + lenScript);

						EMVBase_Trace( "IssuScript Error\r\n");
						EMVBase_Trace("emv-info:dealscripterr err2\r\n");
                        dealscripterr = 1;
                        break;
                    }

                    EmvContact_SendScriptCommand((unsigned char*)&IssuScript[index + 2], ScriptK, &apdu_s);

                    tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

                    if((apdu_r.SW1 == 0x90 && apdu_r.SW2 == 0x00) || apdu_r.SW1 == 0x62 || apdu_r.SW1 == 0x63)
                    {
                        index += (ScriptK + 2);
                        numScriptCom++;
                    }
                    else
                    {
                        numScriptCom++;
						EMVBase_Trace("emv-info:dealscripterr err3\r\n");
                        dealscripterr = 1;
                        break;
                    }
                }
                else
                {
                	EMVBase_Trace("emv-info:dealscripterr err4\r\n");
                    dealscripterr = 1;
                    break;
                }
            }

            if(dealscripterr)
            {
            	EMVBase_Trace("emv-info:dealscripterr = 1\r\n");
                emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x04);

                if(tag == 0x71)
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x20);                  //Script processing failed before final GENERATE AC
                }
                else
                {
                    emvbase_avl_setvalue_or(EMVTAG_TVR, 4, 0x10);                  //Script processing failed after final GENERATE AC
                }

                if(numScriptCom)
                {
                    ScriptResult[ScriptResultlen] = 0x10;

                    if(numScriptCom < 15)
                    {
                        ScriptResult[ScriptResultlen] |= numScriptCom;
                    }
                    else
                    {
                        ScriptResult[ScriptResultlen] |= 0x0F;
                    }
                }
                memcpy(&ScriptResult[ScriptResultlen + 1], curScriptID, 4);
                ScriptResultlen += 5;

                index = Scriptindex + lenScript;

				if(tempEmvContact_UnionStruct->EmvTradeParam->Transflow == TRANS_LOAD)
                {
                    if(ScriptResultlen)
                    {
                        if(tag == 0x71)
                        {
                            emvbase_avl_createsettagvalue(EMVTAG_SCRIPT71RESULT, ScriptResult, ScriptResultlen);
                        }
                        else
                        {
                            emvbase_avl_createsettagvalue(EMVTAG_SCRIPT72RESULT, ScriptResult, ScriptResultlen);
                        }
                    }
                    return EMVCONTACT_ERR_PBOCLOAD;
                }
            }
            else
            {
                emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x04);

                if(numScriptCom)
                {
                    ScriptResult[ScriptResultlen] = 0x20;

                    memcpy(&ScriptResult[ScriptResultlen + 1], curScriptID, 4);
                    ScriptResultlen += 5;
                }
            }
        }
        else
        {
            retCode = EMVCONTACT_ERR_EMV_ScriptFormat;
			break;
        }
    }

    if(ScriptResultlen)
    {
    	EMVBase_Trace("emv-info:ScriptResultlen is true\r\n");
        if(tag == 0x71)
        {
            emvbase_avl_createsettagvalue(EMVTAG_SCRIPT71RESULT, ScriptResult, ScriptResultlen);
        }
        else
        {
            emvbase_avl_createsettagvalue(EMVTAG_SCRIPT72RESULT, ScriptResult, ScriptResultlen);
        }
    }
    emvbase_free(IssuScript);
    emvbase_free(ScriptResult);
	EMVBase_Trace("emv-info:retCode = %d\r\n");
    return retCode;
}

unsigned char EmvContact_WriteDataStorageCommand(unsigned char *data, unsigned char dataLen, EMVBASE_APDU_SEND *apdu_s)
{
	EMVBase_Trace("EMV-info: DF3F data len: %d\r\n", dataLen);
	EMVBase_TraceHex("EMV-info: DF3F data: %d", data, dataLen);

    memcpy(apdu_s->Command, "\x80\xDA\xDF\x3F", 4);
    memcpy(apdu_s->DataIn, data, dataLen);
    apdu_s->Lc = dataLen;
    apdu_s->Le = 0;  //no Le
    apdu_s->EnableCancel = 0;

    return RLT_EMV_OK;
}

unsigned char EmvContact_WriteDataStorage(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
	EMVBASETAGCVLITEM *item = NULL;
	unsigned short index = 0;
	unsigned short len = 0;
	unsigned char *writeDataStorageTemplate = NULL;
	unsigned short TemplateLen = 0;
	EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;


	if(!tempEmvContact_UnionStruct->EmvTradeParam->bDataStorageEnable)
	{
		EMVBase_Trace("EMV-info: Data Storage Enable = false\r\n");
		return RLT_EMV_OK;
	}

	item = emvbase_avl_gettagitempointer(EMVTAG_DPASDSUpdateTemplate);
	if(NULL == item)
	{
		EMVBase_Trace("EMV-info: Data Storage Update Template(TagBF10) missing\r\n");
		return RLT_EMV_OK;
	}

	TemplateLen = item->len;
	writeDataStorageTemplate = (unsigned char *)emvbase_malloc(TemplateLen);
	if(NULL == writeDataStorageTemplate)
	{
		EMVBase_Trace("EMV-info: memory error\r\n");
		return RLT_EMV_OK;
	}
	memcpy(writeDataStorageTemplate, item->data, TemplateLen);


	index = 0;
	while(index < TemplateLen)
	{
		if(!memcmp(&(writeDataStorageTemplate[index]), "\xDF\x3F", 2))
		{
	        index += 2;

	        if(EMVBase_ParseExtLen(writeDataStorageTemplate, &index, &len))
	        {
	        	EMVBase_Trace("EMV-error:  Data Store data objects(TagDF3F) data error\r\n");
	        	emvbase_free(writeDataStorageTemplate);
	            return RLT_ERR_EMV_IccDataFormat;
	        }

			EmvContact_WriteDataStorageCommand(writeDataStorageTemplate+index, len, &apdu_s);
	        tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
	        if(apdu_r.SW1 != 0x90 || apdu_r.SW2 != 0x00)
	        {
	        	EMVBase_Trace("EMV-error: SW != 9000\r\n");
				emvbase_free(writeDataStorageTemplate);
				return RLT_ERR_EMV_IccReturn;
	        }

	        index += len;
	    }
		else
		{
			EMVBase_Trace("EMV-error:  TagBF10 data error\r\n");
			emvbase_free(writeDataStorageTemplate);
			return RLT_ERR_EMV_IccDataFormat;
		}
	}

	if(index != TemplateLen)
	{
		EMVBase_Trace("EMV-error: TagBF10 data error\r\n");
		emvbase_free(writeDataStorageTemplate);
		return RLT_ERR_EMV_IccDataFormat;
	}

	emvbase_free(writeDataStorageTemplate);

	return RLT_EMV_OK;
}

unsigned char EmvContact_OnLineDeal(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char issuerauthendataexist, issuerscript1dataexist, issuerscript2dataexist;
    unsigned char AuthRespCode[2];
    unsigned char CDOL2bExist;
    EMVBASETAGCVLITEM *item;
    unsigned short len;

    //check if need do issuer authen or issuer script
    issuerauthendataexist = emvbase_avl_checkiftagexist(EMVTAG_IssuerAuthenData);

    if(issuerauthendataexist)
    {
        retCode = EmvContact_DealIssuerAuthenData(tempEmvContact_UnionStruct);

        if(retCode != RLT_EMV_OK)
        {
            return RLT_EMV_OK;
        }
    }
    issuerscript1dataexist = emvbase_avl_checkiftagexist(EMVTAG_SCRIPT1);

    if(issuerscript1dataexist)
    {
        retCode = EmvContact_ScriptProcess(0x71, tempEmvContact_UnionStruct);

        if(retCode == EMVCONTACT_ERR_PBOCLOAD)
        {
            return retCode;
        }
    }
    memset(AuthRespCode, 0, sizeof(AuthRespCode));
    emvbase_avl_gettagvalue_spec(EMVTAG_AuthRespCode, AuthRespCode, 0, 2);

    memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
    memcpy(apdu_s.Command, "\x80\xAE", 2);


    if(!memcmp(AuthRespCode, "00", 2) ||
       !memcmp(AuthRespCode, "10", 2) ||
       !memcmp(AuthRespCode, "11", 2) ) 		//Issuer approval,according to p172 of VIS140 terminal specification.
    {
        memcpy(&apdu_s.Command[2], "\x40\x00", 2);
    }
    else if(!memcmp(AuthRespCode, "01", 2) ||
            !memcmp(AuthRespCode, "02", 2))
    {
        retCode = tempEmvContact_UnionStruct->EMVB_InputCreditPwd(EMVB_NEEDREFER, NULL);

        if(retCode == RLT_EMV_OK)
        {
            memcpy(&apdu_s.Command[2], "\x40\x00", 2);
        }
        else
        {
            memcpy(&apdu_s.Command[2], "\x00\x00", 2);
        }
    }
    else
    {
        memcpy(&apdu_s.Command[2], "\x00\x00", 2);
    }

    if(apdu_s.Command[2] & 0x40)
    {
        if(EmvContact_CheckIfdoCDA())
        {
            apdu_s.Command[2] |= 0x10;
        }
    }
    item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL2, &CDOL2bExist);

    if(CDOL2bExist == 0)
    {
        return RLT_EMV_ERR;
    }

    if(EmvContact_DOLProcess(typeCDOL2, item->data, item->len, apdu_s.DataIn, &len, 255,tempEmvContact_UnionStruct) != RLT_EMV_OK)//20201217 lsy
    {
        if(apdu_s.Command[2] & 0x10)       	//CDA
        {
            apdu_s.Command[2] &= 0xEF;

            memcpy(&apdu_s.Command[2], "\x00\x00", 2);          //get AAC

            emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);
            emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);

            EmvContact_DOLProcess(typeCDOL2, item->data, item->len, apdu_s.DataIn, &len, 255,tempEmvContact_UnionStruct);//20201217 lsy
        }
    }
    apdu_s.Lc = len;

    emvbase_avl_createsettagvalue(EMVTAG_CDOL2RelatedData, apdu_s.DataIn, apdu_s.Lc);
    apdu_s.Le = 256;
    apdu_s.EnableCancel = 1;

    emvbase_avl_settag(EMVTAG_ReferenceControlParameter, apdu_s.Command[2]);

	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data)
	{
    	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL2Data);
	}
	else
	{
    	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
	}

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        retCode = RLT_ERR_EMV_APDUTIMEOUT;
    }
    else
    {
        retCode = RLT_EMV_OK;
    }

    if(retCode == RLT_EMV_OK)
    {
        retCode = EmvContact_GenerateAC(tempEmvContact_UnionStruct, &apdu_r);
    }


    if(retCode != RLT_EMV_OK)
    {
		if(retCode == RLT_EMV_ONLINE_DECLINE)
		{
			tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_ONLINE_DECLINE;
		}
        else if(retCode == RLT_ERR_EMV_NotAccept)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE_NOTACCEPT;
        }
        else
        {
            tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE;
        }
        return RLT_EMV_OK;
    }

    if(emvbase_avl_gettagvalue(EMVTAG_CryptInfo) & 0x40)
    {
        if(apdu_s.Command[2] & 0x10)
        {
            emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);
            retCode = EmvContact_CombineDDA(GenerateAC2, tempEmvContact_UnionStruct);

            if(retCode != RLT_EMV_OK)
            {
                emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);
                tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_ONLINE_DECLINE;
                return RLT_EMV_OK;
            }
        }
        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_ONLINE_APPROVE;
        return RLT_EMV_OK;
    }
    else
    {
        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_ONLINE_DECLINE;
        return RLT_EMV_OK;
    }
}

unsigned char EmvContact_UnableGoOnlineDeal(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode, respCrpt;
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char CryptInfo;
    unsigned char CDOL2bExist;
    EMVBASETAGCVLITEM *item = NULL;
    unsigned short len;


    CryptInfo = emvbase_avl_gettagvalue(EMVTAG_CryptInfo);

    if((CryptInfo & 0x08) == 0x08)
    {
        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE;
        return RLT_EMV_OK;
    }
    memset(&apdu_s, 0, sizeof(EMVBASE_APDU_SEND));
    memcpy(apdu_s.Command, "\x80\xAE", 2);


    tempEmvContact_UnionStruct->EmvTradeParam->bAbleOnline = 0;

    if((EmvContact_TermActAnaly(tempEmvContact_UnionStruct) == TAA_OFFLINE))
    {
        memcpy(&apdu_s.Command[2], "\x40\x00", 2);      	//get TC
        emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, "Y3", 2);

        if(EmvContact_CheckIfdoCDA())
        {
            apdu_s.Command[2] |= 0x10;
        }
    }
    else
    {
        memcpy(&apdu_s.Command[2], "\x00\x00", 2); 			//get AAC
        emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, "Z3", 2);
    }
    item = emvbase_avl_gettagitemandstatus(EMVTAG_CDOL2, &CDOL2bExist);

    if(CDOL2bExist == 0)
    {
        return RLT_EMV_ERR;
    }

    if(EmvContact_DOLProcess(typeCDOL2, item->data, item->len, apdu_s.DataIn, &len, 255,tempEmvContact_UnionStruct) != RLT_EMV_OK)//20201217 lsy
    {
        if(apdu_s.Command[2] & 0x10)       //CDA
        {
            apdu_s.Command[2] &= 0xEF;
        }
        memcpy(&apdu_s.Command[2], "\x00\x00", 2);      //get AAC
        emvbase_avl_createsettagvalue(EMVTAG_AuthRespCode, "Z3", 2);

        emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);
        emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);

        EmvContact_DOLProcess(typeCDOL2, item->data, item->len, apdu_s.DataIn, &len, 255,tempEmvContact_UnionStruct);//20201217 lsy
    }
    apdu_s.Lc =  len;
    emvbase_avl_createsettagvalue(EMVTAG_CDOL2RelatedData, apdu_s.DataIn, apdu_s.Lc);
    apdu_s.Le = 256;
    apdu_s.EnableCancel = 1;

    emvbase_avl_settag(EMVTAG_ReferenceControlParameter, apdu_s.Command[2]);

	EMVBase_Trace("UnableGoOnlineDeal-bTrack2InCDOL2Data: %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data);
	EMVBase_Trace("UnableGoOnlineDeal-bTrack2DataMasked: %d\r\n", tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked);
	if(tempEmvContact_UnionStruct->EmvTradeParam->bTrack2DataMasked && tempEmvContact_UnionStruct->EmvTradeParam->bTrack2InCDOL2Data)
	{
    	tempEmvContact_UnionStruct->IsoCommandEx(&apdu_s, &apdu_r, tempEmvContact_UnionStruct->EmvTradeParam->Track2OffsetInCDOL2Data);
	}
	else
	{
    	tempEmvContact_UnionStruct->IsoCommand(&apdu_s, &apdu_r);
	}

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }
    retCode = EmvContact_GenerateAC(tempEmvContact_UnionStruct, &apdu_r);

    if(retCode != RLT_EMV_OK)
    {
        if(retCode == RLT_ERR_EMV_NotAccept)
        {
            tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE_NOTACCEPT;
        }
        else
        {
            tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_TERMINATE;
        }
        return RLT_EMV_OK;
    }

    if(emvbase_avl_gettagvalue(EMVTAG_CryptInfo) & 0x40)
    {
        if(apdu_s.Command[2] & 0x10)
        {
            emvbase_avl_setvalue_or(EMVTAG_TSI, 0, 0x80);
            retCode = EmvContact_CombineDDA(GenerateAC2, tempEmvContact_UnionStruct);

			if(retCode != RLT_EMV_OK)
			{
				emvbase_avl_setvalue_or(EMVTAG_TVR, 0, 0x04);
				tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
				return RLT_EMV_OK;
			}
        }
        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_APPROVE;
        return RLT_EMV_OK;
    }
    else
    {
        tempEmvContact_UnionStruct->EmvTradeParam->TransResult = RESULT_OFFLINE_DECLINE;
        return RLT_EMV_OK;
    }
}


unsigned char EmvContact_PreTransComplete(EMVCONTACTTradeUnionStruct *tempEmvContact_UnionStruct)
{
    unsigned char retCode = RLT_EMV_OK;
    unsigned char issuerscript2dataexist;
    unsigned char AuthRespCode[2];
    unsigned char bReserval = 0;

    issuerscript2dataexist = emvbase_avl_checkiftagexist(EMVTAG_SCRIPT2);

    if(issuerscript2dataexist)
    {
        retCode = EmvContact_ScriptProcess(0x72, tempEmvContact_UnionStruct);
    }

	if(retCode == EMVCONTACT_ERR_PBOCLOAD)
	{
		return retCode;
	}

	EmvContact_WriteDataStorage(tempEmvContact_UnionStruct);

	return RLT_EMV_OK;
}


