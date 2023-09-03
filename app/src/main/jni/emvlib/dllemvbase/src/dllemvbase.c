#include "dllemvbase.h"
#include "dllemvbasedebug.h"
#include <stdarg.h>
#include <stdio.h>

EMVBase_LogOutput *gstemvbaselogoutput = NULL;

void EMVBase_SetLogOutput(EMVBase_LogOutput *pstLogOutput)
{
	gstemvbaselogoutput = NULL;
	if(pstLogOutput && pstLogOutput->EmvBase_Printf && pstLogOutput->logoutputswitch)
	{
		gstemvbaselogoutput = pstLogOutput;
	}
}

void EMVBase_Trace(char *fmt, ...)
{
#ifdef EMVBASE_DEBUG
	va_list args;
	unsigned char printbuffer[256] = {0};

	if(gstemvbaselogoutput && gstemvbaselogoutput->EmvBase_Printf && gstemvbaselogoutput->logoutputswitch)
	{
		va_start ( args, fmt );
		vsprintf ( printbuffer, fmt, args );
		va_end ( args );
		gstemvbaselogoutput->EmvBase_Printf("%s",printbuffer);
	}
#endif
}

void  EMVBase_TraceHex(unsigned char const* pasInfo, void const *pheData, int siLen)
{
#ifdef EMVBASE_DEBUG

    int i;
    unsigned char temp[64] = {0};
    unsigned char buf[16] = {0};
    unsigned char const* src  = (unsigned char const*)pheData;

	if(pasInfo)
	{
	    strcpy(temp, pasInfo);
	    EMVBase_Trace("%s:",temp);
	    memset(temp, 0, sizeof(temp));
	}

    for(i = 0; i < siLen; i++)
    {
        if((i != 0) && (i % 16 == 0))
        {
            EMVBase_Trace("%s\r\n",temp);
            memset(temp, 0, sizeof(temp));
        }
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%02X ", src[i]);
        strcat(temp, buf);
    }
    EMVBase_Trace("%s\r\n",temp);
#endif
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

	for(i=0; i < 12; i++)
	{
		if(strncmp(str_month,en_month[i],3)==0)
		{
		    memset(str_month, 0, sizeof(str_month));
	  		sprintf(str_month, "%02d", i + 1);
	        break;
	    }
	}

	if(strlen(str_day)==1)//é”Ÿæ–¤æ‹·é”Ÿæ–¤æ‹·é”Ÿæ–¤æ‹·ä¸º1ä½é”Ÿæ–¤æ‹·è¦å‰é”Ÿæ–¤æ‹·0x30
	{
		str_day[1]=str_day[0];
		str_day[0]=0x30;
	}

	sprintf(pasCompileTime, "%s%s%s", str_year, str_month, str_day);
	sprintf(tempCompileTime, " %s", pasTime);
	strcat(pasCompileTime, tempCompileTime);

	return 0;
}

void EMVBase_GetLibVersion(unsigned char *version)
{
	GetCompileTime(version, __DATE__, __TIME__);//å¦‚2020å¹´01æœˆ21æ—¥ç¼–è¯‘ï¼Œè¾“å‡ºversionä¸º:200121
}

unsigned char EMVBase_ParseLen(unsigned char *ParseData,unsigned short index,unsigned short *DataLen)
{
     unsigned char t;
     unsigned short j,k,templen;


	k = index;
	if(ParseData[k]<=127)
	{
		templen=ParseData[k];
		k++;
	}
	else
	{
		templen=0;
		t=ParseData[k]&0x7F;
		for(j=1;j<=t;j++)
		{
		    if((k+j)>299)
		    {
		    	return 1;
		    }
			templen=templen*256+ParseData[k+j];
		}
		k+=t+1;
	}
	*DataLen = templen;
	return 0;
}

unsigned char EMVBase_ParseExtLen(unsigned char *ParseData,unsigned short *index,unsigned short *DataLen)
{
     unsigned char t;
     unsigned short j,k,templen;


	k = *index;
	if(ParseData[k]<=127)
	{
		templen=ParseData[k];
		k++;
	}
	else
	{
		templen=0;
		t=ParseData[k]&0x7F;
		if (t>2)
		{
	    	return 1;
		}
		for(j=1;j<=t;j++)
		{
			if((k+j)>299)
		    {
		    	return 1;
		    }
			templen=templen*256+ParseData[k+j];
		}
		k+=t+1;
	}
	*index = k;
	*DataLen = templen;
	return 0;
}


unsigned char EMVBase_ParseExtLenWithMaxLen(unsigned char *ParseData,unsigned short *index,unsigned short *DataLen,unsigned short MaxLen)
{
     unsigned char t;
     unsigned short j,k,templen;


	k = *index;
	if(ParseData[k]<=127)
	{
		templen=ParseData[k];
		k++;
	}
	else
	{
		templen=0;
		t=ParseData[k]&0x7F;
		if (t>2)
		{
	    	return 1;
		}
		for(j=1;j<=t;j++)
		{
			if((k+j)>MaxLen)
		    {
		    	return 1;
		    }
			templen=templen*256+ParseData[k+j];
		}
		k+=t+1;
	}
	*index = k;
	*DataLen = templen;
	return 0;
}


unsigned char EMVBase_ReadSelectRetData(EMVBASE_SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, EMVBase_UnionStruct *tempAppUnionStruct)
{
    int matchnum;
    EMVBASE_LISTAPPDATA *tempAppAppData;
    unsigned char *tempappnum;
    unsigned char j, k;
    unsigned short index, indexFCI, indexFCIProp, len, lenFCI, lenFCIProp;
    int indexIssuerDiscret, lenIssuerDiscret;
    int indexAppApp, lenAppApp;
    EMVBASE_TERMAPP tempAppTermApp;
    unsigned short templen;
	unsigned char extendedselect[11] = {0};
	unsigned short extendselectlen = 0;
	unsigned short indexttemp=0,tagLENLen=0,tagLen=0;
	int rfulen=0;

    tempappnum = &(tempAppUnionStruct->EntryPoint->AppListCandidatenum);
    tempAppAppData = tempAppUnionStruct->EntryPoint->AppListCandidate;
    *tempappnum = 0;


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
        if(index >= LenOut)
		{
			return RLT_ERR_EMV_IccDataFormat;
		}

        if(0xFF == DataOut[index] || 0x00 == DataOut[index])
		{
			index++;
			continue;
		}
        else if(0x84 == DataOut[index])
        {
            if(1 == selectRet->DFNameExist)
			{
				return RLT_ERR_EMV_IccDataFormat;
			}
            index++;

			if(EMVBase_ParseExtLen(DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            selectRet->DFNameLen = len;
			if(tempAppUnionStruct->CheckTag84 & 0x02)
			{
				if(selectRet->DFNameLen != 14)
				{
					return RLT_ERR_EMV_IccDataFormat;
				}
			}

            if(selectRet->DFNameLen > 16)
			{
				return RLT_ERR_EMV_IccDataFormat;
			}

            memcpy(selectRet->DFName, DataOut + index ,selectRet->DFNameLen);
			if(tempAppUnionStruct->CheckTag84 & 0x04)
			{
				if(memcmp(selectRet->DFName, "2PAY.SYS.DDF01", 14))
				{
					return RLT_ERR_EMV_IccDataFormat;
				}
			}
            index += selectRet->DFNameLen;
            selectRet->DFNameExist = 1;
        }
        else if(0xA5 == DataOut[index])
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
                if(index >= LenOut)
				{
					return RLT_ERR_EMV_IccDataFormat;
				}

                if(0xFF == DataOut[index] || 0x00 == DataOut[index])
				{
					index++;
					continue;
				}
                else if(!memcmp(DataOut + index, "\xBF\x0C", 2))
                {
                    if(1 == selectRet->IssuerDiscretExist)
					{
						return RLT_ERR_EMV_IccDataFormat;
					}
                    index += 2;

                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
                    {
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len > 253)
					{
						return RLT_ERR_EMV_IccDataFormat;
					}
                    selectRet->IssuerDiscretLen = len;
                    selectRet->IssuerDiscretExist = 1;

                    indexIssuerDiscret = index;
                    lenIssuerDiscret = len;

                    while(index < indexIssuerDiscret + lenIssuerDiscret)
                    {
                        if(index >= LenOut)
						{
							return RLT_ERR_EMV_IccDataFormat;
						}

                        if(0xFF == DataOut[index] || 0x00 == DataOut[index])
						{
							index++;
							continue;
						}
                        else if(0x61 == DataOut[index])
                        {
							if((*tempappnum) >= tempAppUnionStruct->EntryPoint->AppListCandidateMaxNum)
							{
								k = DataOut[index];
	                            index++;

	                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
	                            {
	                                return RLT_ERR_EMV_IccDataFormat;
	                            }
	                            index += len;
								continue;
							}


                        	selectRet->ApplicationTemplateExist = 1;
                            memset(tempAppAppData + (*tempappnum), 0, sizeof(EMVBASE_LISTAPPDATA));
                            index += 1;

                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
                            {
                                return RLT_ERR_EMV_IccDataFormat;
                            }
                            indexAppApp = index;
                            lenAppApp = len;

                            while(index < indexAppApp + lenAppApp)
                            {
                                if(index >= LenOut)
								{
									return RLT_ERR_EMV_IccDataFormat;
								}

                                if(0xFF == DataOut[index] || 0x00 == DataOut[index])
								{
									index++;
									continue;
								}
                                else if(0x4F == DataOut[index])
                                {
                                    index += 1;

                                    if(EMVBase_ParseExtLen(DataOut, &index, &templen))
                                    {
                                        return RLT_ERR_EMV_IccDataFormat;
                                    }
                                    (tempAppAppData + (*tempappnum))->AIDLen = templen;

                                    if((tempAppAppData + (*tempappnum))->AIDLen > 16)
									{
										(tempAppAppData + (*tempappnum))->AIDLen = 0;
									}

                                    j = (tempAppAppData + (*tempappnum))->AIDLen;
                                    memcpy((tempAppAppData + (*tempappnum))->AID, &DataOut[index], j);
                                    index += templen;
                                }
                                else if(0x50 == DataOut[index])		//AppLabel
                                {
                                    index += 1;

                                    if(EMVBase_ParseExtLen(DataOut, &index, &templen))
                                    {
                                        return RLT_ERR_EMV_IccDataFormat;
                                    }
                                    (tempAppAppData + (*tempappnum))->AppLabelLen = templen;

                                    if((tempAppAppData + (*tempappnum))->AppLabelLen > 16)
									{
										(tempAppAppData + (*tempappnum))->AppLabelLen = 16;
									}

                                    j = (tempAppAppData + (*tempappnum))->AppLabelLen;
                                    memcpy((tempAppAppData + (*tempappnum))->AppLabel, DataOut + index, j);
                                    index += templen;
                                }
                                else if(0x87 == DataOut[index])		//AppPriority
                                {
                                    index += 1;

                                    if(EMVBase_ParseExtLen(DataOut, &index, &templen))
                                    {
                                        return RLT_ERR_EMV_IccDataFormat;
                                    }

                                    if(templen != 1)
                                    {
                                        return RLT_ERR_EMV_IccDataFormat;
                                    }
                                    (tempAppAppData + (*tempappnum))->Priority = DataOut[index];
                                    index++;
                                }
								else if(!memcmp(DataOut+index,"\x9F\x2A",2))  //Kernel Identifier
								{
									index+=2;

									if(EMVBase_ParseExtLen(DataOut,&index,&templen))
									{
										return RLT_ERR_EMV_IccDataFormat;
									}

									(tempAppAppData + (*tempappnum))->KernelIdentifierLen  = templen;

									if((tempAppAppData + (*tempappnum))->KernelIdentifierLen > 8)
									{
										(tempAppAppData + (*tempappnum))->KernelIdentifierLen = 8;
									}
									memcpy((tempAppAppData + (*tempappnum))->KernelIdentifier,DataOut+index,(tempAppAppData + (*tempappnum))->KernelIdentifierLen);
									index+=templen;
								}
								else if(!memcmp(DataOut + index, "\x9F\x29", 2))
                                {
                                    index += 2;

                                    if(EMVBase_ParseExtLen(DataOut, &index, &templen))
                                    {
                                        return RLT_ERR_EMV_IccDataFormat;
                                    }
                                    extendselectlen = templen;

                                    if(extendselectlen <= 11)
                                    {
                                        memcpy(extendedselect, DataOut + index, extendselectlen);
                                    }
                                    else
                                    {
                                        extendselectlen = 0;
                                    }
                                    index += templen;
                                }
								else if(!memcmp(DataOut + index, "\x9F\x0A", 2))
								{
									index+=2;

									if(EMVBase_ParseExtLen(DataOut,&index,&templen))
									{
										return RLT_ERR_EMV_IccDataFormat;
									}

									(tempAppAppData + (*tempappnum))->ASRPDLen  = templen;

									if(templen > 0)
									{
										(tempAppAppData + (*tempappnum))->ASRPD = emvbase_malloc(templen);
										memcpy((tempAppAppData + (*tempappnum))->ASRPD,DataOut+index,templen);

									}
									index+=templen;
								}
                                else if(0x61 == DataOut[index])
                                {
                                    break;
                                }
                                else
                                {
                                	tagLen=0;
									indexttemp = index;
                                    k = DataOut[index];

                                    if(0x1F == (k & 0x1F))
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
									if(tagLen > 0)
									{
										if (NULL != (tempAppAppData + (*tempappnum))->rfu)
										{
											unsigned char *temp = (tempAppAppData + (*tempappnum))->rfu;

											(tempAppAppData + (*tempappnum))->rfu = emvbase_malloc((tempAppAppData + (*tempappnum))->rfuLen + tagLen);
											if((tempAppAppData + (*tempappnum))->rfu == NULL)
											{
												return RLT_EMV_ERR;
											}
											memcpy((tempAppAppData + (*tempappnum))->rfu,temp,(tempAppAppData + (*tempappnum))->rfuLen);
											emvbase_free(temp);
											temp=NULL;
											memcpy((tempAppAppData + (*tempappnum))->rfu + (tempAppAppData + (*tempappnum))->rfuLen, DataOut+indexttemp, tagLen);
											(tempAppAppData + (*tempappnum))->rfuLen  += tagLen;

										}
										else
										{
											(tempAppAppData + (*tempappnum))->rfu = emvbase_malloc(tagLen);
											if((tempAppAppData + (*tempappnum))->rfu == NULL)
											{
												return RLT_EMV_ERR;
											}
											memcpy((tempAppAppData + (*tempappnum))->rfu,DataOut+indexttemp,tagLen);
											(tempAppAppData + (*tempappnum))->rfuLen  = tagLen;
										}
									}

                                    index += len;
                                }
                            }

							if(tempAppUnionStruct->SupportExternSelect && extendselectlen && ((tempAppAppData + (*tempappnum))->AIDLen + extendselectlen) <= 16)
                            {
                                memcpy((tempAppAppData + (*tempappnum))->AID + (tempAppAppData + (*tempappnum))->AIDLen, extendedselect, extendselectlen);
                                (tempAppAppData + (*tempappnum))->AIDLen += extendselectlen;
                            }
							extendselectlen = 0;
							memset(extendedselect, 0, sizeof(extendedselect));

                            if((tempAppAppData + (*tempappnum))->AIDLen != 0 && (*tempappnum) < tempAppUnionStruct->EntryPoint->AppListCandidateMaxNum)
                            {
                                matchnum = tempAppUnionStruct->CheckMatchTermAID((tempAppAppData + (*tempappnum))->AID, (tempAppAppData + (*tempappnum))->AIDLen, &tempAppTermApp,(tempAppAppData + (*tempappnum))->KernelIdentifier, (tempAppAppData + (*tempappnum))->KernelIdentifierLen);

                                if(matchnum >= 0)
                                {
                                    (tempAppAppData + (*tempappnum))->AidInTermLen = tempAppTermApp.AIDLen;
                                    memcpy((tempAppAppData + (*tempappnum))->AidInTerm, tempAppTermApp.AID, (tempAppAppData + (*tempappnum))->AidInTermLen);
                                    (*tempappnum)++;
                                }
                            }
                        }
                        else if(!memcmp(DataOut + index, "\x9F\x4D", 2))	//Log Entry
                        {
                            if(selectRet->LogEntryExist == 1)
							{
								return RLT_ERR_EMV_IccDataFormat;
							}
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
                            k = DataOut[index];

                            if(0x1F == (k & 0x1F))
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
                }
                else		//other unknown TLV data
                {
                    k = DataOut[index];

                    if(0x1F == (k & 0x1F))
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
        else
        {
            k = DataOut[index];

            if(0x1F == (k & 0x1F))
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

unsigned char EMVBase_ReadSelectRetData_Jcb(EMVBASE_SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, EMVBase_UnionStruct *tempAppUnionStruct)
{
	  int matchnum;
    EMVBASE_LISTAPPDATA *tempAppAppData;
    EMVBASE_TERMAPP tempAppTermApp;
    unsigned char *tempappnum;
    //unsigned char *temptermlistnum;
    unsigned char j, k;     //i,
    unsigned short index, indexFCI, indexFCIProp, len, lenFCI, lenFCIProp;
    int indexIssuerDiscret, lenIssuerDiscret;
    int indexAppApp, lenAppApp;
    unsigned char priorityexist;
	unsigned short templen, ExtendedLen;
	unsigned char tempindex;
	unsigned char errindex = 0,i;
	unsigned char errindex2 = 0;
	unsigned short tempBF0Clen;
	unsigned char extendedselect[11] = {0};
	unsigned short extendselectlen = 0;


    tempappnum = &(tempAppUnionStruct->EntryPoint->AppListCandidatenum);
    tempAppAppData = tempAppUnionStruct->EntryPoint->AppListCandidate;
    *tempappnum = 0;


    index = 0;

    if(DataOut[index] != 0x6F)          //FCI?¡ê¡ã?¡À¨º¨º?
    {
        return RLT_ERR_EMV_IccDataFormat;         //FCI template
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
        else if(DataOut[index] == 0x84)         //DF??
        {
            if(selectRet->DFNameExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
            index++;

			if(EMVBase_ParseExtLen(DataOut, &index, &len))
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
            selectRet->DFNameLen = len;
			if(tempAppUnionStruct->CheckTag84 & 0x02)
			{
				if(selectRet->DFNameLen != 14) { return RLT_ERR_EMV_IccDataFormat; }
			}

			if(selectRet->DFNameLen > 16) { return RLT_ERR_EMV_IccDataFormat; }

			memcpy(selectRet->DFName, DataOut + index ,selectRet->DFNameLen);
			if(tempAppUnionStruct->CheckTag84 & 0x04)
			{
				if(memcmp(selectRet->DFName, "2PAY.SYS.DDF01", 14)) { return RLT_ERR_EMV_IccDataFormat; }
			}
            index += selectRet->DFNameLen;
            selectRet->DFNameExist = 1;
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

			EMVBase_Trace( "indexFCIProp=%d lenFCIProp=%d\r\n",indexFCIProp, lenFCIProp);
            while(index < indexFCIProp + lenFCIProp)
            {
                if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
                else if(!memcmp(DataOut + index, "\xBF\x0C", 2))             //FCI ¡¤¡é?¡§¡¤?¡Á??¡§¨°?¨ºy?Y
                {
                    if(selectRet->IssuerDiscretExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
                    index += 2;

                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
                    {
                    	EMVBase_Trace( "bf0c error!\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }

                    if(len > 253) { return RLT_ERR_EMV_IccDataFormat; }
                    selectRet->IssuerDiscretLen = len;                     //sxl 090211
                    EMVBase_Trace( "IssuerDiscretLen=%d index=%d\r\n", len, index);
                    //memcpy(selectRet->IssuerDiscret,DataOut+index,len);
                    selectRet->IssuerDiscretExist = 1;

                    indexIssuerDiscret = index;
                    lenIssuerDiscret = len;

                    while(index < indexIssuerDiscret + lenIssuerDiscret)
                    {
                    	EMVBase_Trace( "index=%d indexIssuerDiscret=%d lenIssuerDiscret=%d",index, indexIssuerDiscret,lenIssuerDiscret);
                        if(index >= LenOut) { return RLT_ERR_EMV_IccDataFormat; }

                        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
                        else if(DataOut[index] == 0x61)
                        {
                        	selectRet->ApplicationTemplateExist = 1;
                            memset((unsigned char *)&tempAppAppData[*tempappnum], 0, sizeof(EMVBASE_LISTAPPDATA));                           //??¨®|¨®?
                            priorityexist = 0;
                            index += 1;

                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
                            {
                            	EMVBase_Trace( "tag 61 error\r\n");
                                return RLT_ERR_EMV_IccDataFormat;
                            }
                            indexAppApp = index;
                            lenAppApp = len;

							errindex = index;
							if(len == 0)
							{
								EMVBase_Trace("tag 61 length zero!\r\n");
								return RLT_ERR_EMV_IccDataFormat;
							}

                            while(index < indexAppApp + lenAppApp)
                            {
                            	EMVBase_Trace( "index=%d LenOut=%d\r\n", index, LenOut);
                                if(index >= LenOut)
								{
									EMVBase_Trace( "error11111");
									//return ERR_EMV_IccDataFormat;
									break;
								}

                                if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
                                else if(DataOut[index] == 0x4F)                                 //AID
                                {
                                    index += 1;
                                    (tempAppAppData + (*tempappnum))->AIDLen = DataOut[index];
									if((tempAppAppData + (*tempappnum))->AIDLen > 16)
									{
										(tempAppAppData + (*tempappnum))->AIDLen = 0;
									}



									index++;
                                    j = (tempAppAppData + (*tempappnum))->AIDLen;
                                    memcpy((tempAppAppData + (*tempappnum))->AID, DataOut + index, j);
                                    EMVBase_TraceHex( "4F aid &len", DataOut + index, j);
                                    index += j;
                                }
                                else if(DataOut[index] == 0x50)                                 //AppLabel
                                {//500B 4A434220437265646974870101
                                	EMVBase_Trace("goto deal tag 50!\r\n");
                                    index += 1;
                                    (tempAppAppData + (*tempappnum))->AppLabelLen = DataOut[index];

                                    if((tempAppAppData + (*tempappnum))->AppLabelLen > 16) { (tempAppAppData + (*tempappnum))->AppLabelLen = 16; }
                                    index++;
                                    j = (tempAppAppData + (*tempappnum))->AppLabelLen;
                                    memcpy((tempAppAppData + (*tempappnum))->AppLabel, DataOut + index, j);
                                    index += j;
                                }
                                else if(DataOut[index] == 0x87)    //AppPriority
                                {
									EMVBase_Trace("goto deal tag 87!\r\n");
									index ++;
									templen = DataOut[index];
									if(templen == 1)//870201   870001	87020001   870101
									{
										index++;
										(tempAppAppData + (*tempappnum))->Priority = DataOut[index];
										index++;
										priorityexist = 1;

									}
									else
									{
										index++;
										templen = 0;
										index +=templen;
									}

                                }
								else if(memcmp(DataOut + index, "\x9F\x2A", 2) == 0)//kernel ID
								{//9F2A0105  9F2A04050001  9F2A0305000001  9F2A0005

									index += 2;
									templen = DataOut[index];
									EMVBase_Trace( "tag9F2A templen=%d\r\n", templen);
									if(templen == 1)
									{
										index++;
										EMVBase_Trace( "tag9F2A 1byte=%02x\r\n", DataOut[index]);
										if(DataOut[index] != 0x05)
										{
											templen = 0;
											(tempAppAppData + (*tempappnum))->AIDLen = 0;
											//index--;
										}
										index += templen;
									}
									else
									{
										index++;
										templen = 0;
										index += templen;
									}
								}
								else if(memcmp(DataOut + index, "\x9F\x29", 2) == 0)//Extened Selection zwb 180202
								{  //9F29021122  9F29031122  9F29011122  9F29001122
									EMVBase_Trace( "goto deal tag 9F29!\r\n");
								#if 0
									index += 2;
									templen = DataOut[index];
									if(templen == 2)
									{
										index++;
										index += templen;
									}
									else
									{
										index++;
										templen =  0;
										index += templen;
									}
								#else //sjz 2020.6.19
									index += 2;

									if(EMVBase_ParseExtLen(DataOut, &index, &templen))
									{
										return RLT_ERR_EMV_IccDataFormat;
									}
									if(templen <= 2)
									{
										extendselectlen = templen;

										if(extendselectlen <= 11)
										{
											memcpy(extendedselect, DataOut + index, extendselectlen);
										}
										else
										{
											extendselectlen = 0;
										}
										index += templen;
									}

								#endif
								}
                                else
                                {
									errindex = index;

                                    k = DataOut[index];
									EMVBase_Trace( "xxx k=%02x\r\n", k);

                                    if((k & 0x1F) == 0x1F)
                                    {
                                        index++;
                                    }
                                    index++;


									EMVBase_Trace( "index111=%d indexAppApp + lenAppApp=%d\r\n", index, indexAppApp + lenAppApp);
									EMVBase_Trace( "DataOut[index]=%02x\r\n", DataOut[index]);
									if(index < indexAppApp + lenAppApp)
									{
										if(EMVBase_ParseExtLen(DataOut, &index, &len))
										{
											break;
										}
										EMVBase_Trace( "test111111");
									}
									else
									{
										break;
									}

                                    index += len;
                                }

                            }

							EMVBase_Trace("00000index=%d LenOut=%d",index,LenOut);

							EMVBase_Trace("00000index=%d indexAppApp + lenAppApp=%d\r\n", index, indexAppApp + lenAppApp);
							if(index >  indexAppApp + lenAppApp /*&& index < LenOut*/)	// parase err
							{
								EMVBase_Trace("errindex=%d\r\n", errindex);
								index = errindex;
								for(i = errindex ; i < indexAppApp + lenAppApp ;  i++)
								{
									if(DataOut[i] == 0x61)
									{
										EMVBase_Trace("another tag 61");
										break;
									}
								}
								index = i;

							}

							EMVBase_Trace("aid len = %d\r\n",(tempAppAppData + (*tempappnum))->AIDLen);
							EMVBase_Trace("tempAppUnionStruct->SupportExternSelect=%d,extendlen=%d\r\n",tempAppUnionStruct->SupportExternSelect,extendselectlen);
							if(tempAppUnionStruct->SupportExternSelect && extendselectlen && ((tempAppAppData + (*tempappnum))->AIDLen + extendselectlen) <= 16)
                            {//sjz 2020.6.19
                                memcpy((tempAppAppData + (*tempappnum))->AID + (tempAppAppData + (*tempappnum))->AIDLen, extendedselect, extendselectlen);
                                (tempAppAppData + (*tempappnum))->AIDLen += extendselectlen;
                            }
							extendselectlen = 0;
							memset(extendedselect, 0, sizeof(extendedselect));

                             if((tempAppAppData + (*tempappnum))->AIDLen != 0 && (*tempappnum) < tempAppUnionStruct->EntryPoint->AppListCandidateMaxNum)
                            {
                                matchnum = tempAppUnionStruct->CheckMatchTermAID((tempAppAppData + (*tempappnum))->AID, (tempAppAppData + (*tempappnum))->AIDLen, &tempAppTermApp,(tempAppAppData + (*tempappnum))->KernelIdentifier, (tempAppAppData + (*tempappnum))->KernelIdentifierLen);
								EMVBase_Trace("matchnum = %d\r\n",matchnum);

                                if(matchnum >= 0)
                                {
                                    (tempAppAppData + (*tempappnum))->AidInTermLen = tempAppTermApp.AIDLen;
                                    memcpy((tempAppAppData + (*tempappnum))->AidInTerm, tempAppTermApp.AID, (tempAppAppData + (*tempappnum))->AidInTermLen);
                                    (*tempappnum)++;

									EMVBase_Trace("tempappnum = %d\r\n",(*tempappnum));
                                }
                            }
                        }
                        else if(!memcmp(DataOut + index, "\x9F\x4D", 2))                    //Log Entry¨¨???¨¨??¨²
                        {
                            if(selectRet->LogEntryExist == 1) { return RLT_ERR_EMV_IccDataFormat; }
                            index += 2;

                            if(EMVBase_ParseExtLen(DataOut, &index, &len))
                            {
                                return RLT_ERR_EMV_IccDataFormat;
                            }
                            selectRet->LogEntryLen = len;
                            //memcpy(selectRet->LogEntry,DataOut+index,selectRet->LogEntryLen);
                            index += selectRet->LogEntryLen;
                            selectRet->LogEntryExist = 1;
                        }
                        else
                        {
                            if(errindex2 == 0)
                            {
                            	errindex2 = index;
                            }
                            k = DataOut[index];
							EMVBase_Trace("errindex2 k=%02x\r\n", k);

                            if((k & 0x1F) == 0x1F)
                            {
                                index++;
                            }
                            index++;

							EMVBase_Trace("index2222=%d indexIssuerDiscret + lenIssuerDiscret=%d\r\n", index, indexIssuerDiscret + lenIssuerDiscret);

							if(index < indexIssuerDiscret + lenIssuerDiscret)
							{
								EMVBase_Trace("DataOut[index]=%02x\r\n", DataOut[index]);
								if(EMVBase_ParseExtLen(DataOut, &index, &len))
								{
									break;
								}
							}
							else
							{
								break;
							}

                            index += len;
                        }
                    }

					EMVBase_Trace("index3333=%d indexIssuerDiscret + lenIssuerDiscret=%d\r\n", index, indexIssuerDiscret + lenIssuerDiscret);
					if(index >	indexIssuerDiscret + lenIssuerDiscret)	// parase err
					{
						EMVBase_Trace("errindex2=%d\r\n", errindex2);
						index = errindex2;
						for(i = errindex2 ; i < indexIssuerDiscret + lenIssuerDiscret ; i++)
						{
							if(DataOut[i] == 0x61)
							{
								break;
							}
						}
						index = i;

					}
				}
                else                //other unknown TLV data
                {
                    k = DataOut[index];
					EMVBase_Trace( "2222k=%d\r\n",k);

                    if((k & 0x1F) == 0x1F)
                    {
                        index++;
                    }
                    index++;

                    if(EMVBase_ParseExtLen(DataOut, &index, &len))
                    {
						EMVBase_Trace( "yyyyyyyyyyyy\r\n");
                        return RLT_ERR_EMV_IccDataFormat;
                    }
                    index += len;
                }
            }

			EMVBase_Trace("IssuerDiscretLen=%d index=%d indexIssuerDiscret=%d\r\n", selectRet->IssuerDiscretLen, index, indexIssuerDiscret);
			if( selectRet->IssuerDiscretLen != (index - indexIssuerDiscret))// BF0C length value
			{// the length  must correspond to the length of value
				EMVBase_Trace("BF0C length value error!\r\n");
				return RLT_ERR_EMV_IccDataFormat;
			}

            if(index != indexFCIProp + lenFCIProp)
            {
				EMVBase_Trace( "A5 length value error!\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
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
				EMVBase_Trace( "aaaaaaaaaa\r\n");
                return RLT_ERR_EMV_IccDataFormat;
            }
            index += len;
        }
    }

    if(index != indexFCI + lenFCI)
    {
		EMVBase_Trace( "bbbbbbbbbbb\r\n");
        return RLT_ERR_EMV_IccDataFormat;
    }
    return RLT_EMV_OK;
}


unsigned char EMVBase_SelectDFRetData(EMVBASE_APDU_RESP *apdu_r, EMVBase_UnionStruct *tempAppUnionStruct)
{
    EMVBASE_SELECT_RET selectRet;

    if(apdu_r->SW1 == 0x90 && apdu_r->SW2 == 0x00)
    {
        memset((unsigned char*)&selectRet, 0, sizeof(EMVBASE_SELECT_RET));

		if(tempAppUnionStruct->PpseRespType == 1)
		{
			if(EMVBase_ReadSelectRetData_Jcb(&selectRet, apdu_r->DataOut, apdu_r->LenOut, tempAppUnionStruct) != RLT_EMV_OK)
			{
				return RLT_ERR_EMV_IccDataFormat;
			}
		}
		else
		{
			if(EMVBase_ReadSelectRetData(&selectRet, apdu_r->DataOut, apdu_r->LenOut, tempAppUnionStruct) != RLT_EMV_OK)
			{
				return RLT_ERR_EMV_IccDataFormat;
			}
		}

		if(tempAppUnionStruct->CheckTag84 & 0x01)
		{
			if(selectRet.DFNameExist == 0)
			{
				return RLT_ERR_EMV_IccDataFormat;
			}
		}
		if(selectRet.ApplicationTemplateExist == 0)
		{
			return RLT_EMV_TERMINATE_TRANSERR;
		}

        if(selectRet.FCIPropExist == 0)
        {
            return RLT_ERR_EMV_IccDataFormat;
        }

        if(selectRet.SFIExist == 1)
        {
            if(selectRet.SFI < 1 || selectRet.SFI > 10 )
            {
                return RLT_ERR_EMV_IccDataFormat;
            }
        }

        return RLT_EMV_OK;
    }
    else if(apdu_r->SW1 == 0x6A && apdu_r->SW2 == 0x81)
    {
        return RLT_ERR_EMV_CardBlock;
    }
	else if(apdu_r->SW1 == 0x6A && apdu_r->SW2 == 0x82)
	{
		return RLT_EMV_PPSE_REV_6A82;
	}
    else
    {
        return RLT_ERR_EMV_IccReturn;
    }
}


unsigned char EMVBase_SelectPPSE(EMVBase_UnionStruct *tempEMVBase_UnionStruct)
{
    EMVBASE_APDU_SEND apdu_s;
    EMVBASE_APDU_RESP apdu_r;
    unsigned char retCode;

	if(tempEMVBase_UnionStruct->rapdu != NULL)
	{
		memcpy(tempEMVBase_UnionStruct->rapdu,"\x00\x00",2);
	}

    EMVBase_COMMAND_SELECT("2PAY.SYS.DDF01", 14, 0, &apdu_s);

    tempEMVBase_UnionStruct->IsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
    {
        return RLT_ERR_EMV_APDUTIMEOUT;
    }

	if(tempEMVBase_UnionStruct->rapdu != NULL)
	{
		tempEMVBase_UnionStruct->rapdu[0]  = (apdu_r.LenOut + 2) / 256;
		tempEMVBase_UnionStruct->rapdu[1]  = (apdu_r.LenOut + 2) % 256;
		memcpy(tempEMVBase_UnionStruct->rapdu + 2,apdu_r.DataOut,apdu_r.LenOut);
		tempEMVBase_UnionStruct->rapdu[apdu_r.LenOut + 2]  = apdu_r.SW1;
		tempEMVBase_UnionStruct->rapdu[apdu_r.LenOut + 3]  = apdu_r.SW2;
	}

    retCode = EMVBase_SelectDFRetData(&apdu_r, tempEMVBase_UnionStruct);

    return retCode;
}

unsigned char EMVBase_ChooseApp(EMVBase_UnionStruct *tempEMVBase_UnionStruct)
{
    unsigned char i, j, temp;
    unsigned char *tempselectappno;
    unsigned char *tempappnum;
    unsigned char noprioritynum = 0;
    unsigned char *tmpseq = NULL;
    unsigned char *tmpapprtiority = NULL;
    unsigned char *AppPriority = NULL;
    unsigned char *AppSeq = NULL;
    EMVBASE_LISTAPPDATA *tempAppAppData;


    tempselectappno = &(tempEMVBase_UnionStruct->SelectedAppNo);
    tempappnum = &(tempEMVBase_UnionStruct->EntryPoint->AppListCandidatenum);
    tempAppAppData = tempEMVBase_UnionStruct->EntryPoint->AppListCandidate;


    if(*tempappnum < 1)
    {
        return RLT_ERR_EMV_NoAppSel;
    }
    *tempselectappno = 0xff;

	tmpseq = emvbase_malloc(tempEMVBase_UnionStruct->EntryPoint->AppListCandidateMaxNum);
	tmpapprtiority = emvbase_malloc(tempEMVBase_UnionStruct->EntryPoint->AppListCandidateMaxNum);
	AppPriority = emvbase_malloc(tempEMVBase_UnionStruct->EntryPoint->AppListCandidateMaxNum);
	AppSeq = emvbase_malloc(tempEMVBase_UnionStruct->EntryPoint->AppListCandidateMaxNum);

	if(tmpseq==NULL || tmpapprtiority==NULL || AppPriority==NULL || AppSeq==NULL)
	{
		if(tmpseq) emvbase_free(tmpseq);
		if(tmpapprtiority) emvbase_free(tmpapprtiority);
		if(AppPriority) emvbase_free(AppPriority);
		if(AppSeq) emvbase_free(AppSeq);
		return RLT_EMV_ERR;
	}


    if(*tempselectappno > *tempappnum)
    {
        for(i = 0; i < *tempappnum; i++)
        {
            AppPriority[i] = (tempAppAppData + i)->Priority & 0x0F;
            AppSeq[i] = i;
        }

        if(*tempappnum == 1)
        {
            *tempselectappno = 0;
        }
        else
        {
            for(j = 0; j < *tempappnum - 1; j++)
            {
                for(i = 0; i < *tempappnum - 1 - j; i++)
                {
                    if(AppPriority[i] > AppPriority[i + 1])
                    {
                        temp = AppPriority[i];
                        AppPriority[i] = AppPriority[i + 1];
                        AppPriority[i + 1] = temp;
                        temp = AppSeq[i];
                        AppSeq[i] = AppSeq[i + 1];
                        AppSeq[i + 1] = temp;
                    }
                }
            }

            noprioritynum = 0;

            for(i = 0; i < *tempappnum; i++)
            {
                if(AppPriority[i] == 0) { noprioritynum++; }
            }

            if(noprioritynum)              //have no priority app
            {
                memcpy(tmpapprtiority, &AppPriority[noprioritynum], *tempappnum - noprioritynum);
                memcpy(&tmpapprtiority[*tempappnum - noprioritynum], AppPriority, noprioritynum);
                memcpy(AppPriority, tmpapprtiority, *tempappnum);
                memcpy(tmpseq, &AppSeq[noprioritynum], *tempappnum - noprioritynum);
                memcpy(&tmpseq[*tempappnum - noprioritynum], AppSeq, noprioritynum);
                memcpy(AppSeq, tmpseq, *tempappnum);
            }
            *tempselectappno = AppSeq[0];
        }
    }

	if(tmpseq) emvbase_free(tmpseq);
	if(tmpapprtiority) emvbase_free(tmpapprtiority);
	if(AppPriority) emvbase_free(AppPriority);
	if(AppSeq) emvbase_free(AppSeq);

	return RLT_EMV_OK;
}

void EMVBase_AppCopy(unsigned char i, unsigned char j, EMVBASE_LISTAPPDATA *ListCandidate)
{
    memcpy((unsigned char*)&ListCandidate[i], (unsigned char*)&ListCandidate[j], sizeof(EMVBASE_LISTAPPDATA));
}

unsigned char EMVBase_RebuildApplist(EMVBase_UnionStruct *tempEMVBase_UnionStruct)
{
    unsigned char i;
    EMVBASE_LISTAPPDATA *tempapplist;

    tempapplist = tempEMVBase_UnionStruct->EntryPoint->AppListCandidate;

    if((tempEMVBase_UnionStruct->EntryPoint->AppListCandidatenum) > 1)
    {
        for(i = (tempEMVBase_UnionStruct->SelectedAppNo); i < (tempEMVBase_UnionStruct->EntryPoint->AppListCandidatenum) - 1; i++)
        {
            EMVBase_AppCopy(i, i + 1, tempapplist);
        }

        (tempEMVBase_UnionStruct->EntryPoint->AppListCandidatenum) --;
        return RLT_EMV_OK;
    }
    else
    {
        return RLT_ERR_EMV_NoAppSel;
    }
}

unsigned char EMVBase_CheckifRedundantData(unsigned char *tag, unsigned char *rundatabuf, unsigned int rundatalen)
{
    unsigned int index = 0;
    unsigned char temptag[4], temptaglen;
    EMVBASETAGCVLITEM tempemvtagitem;

    while(index < rundatalen)
    {
        temptaglen = 0;

        temptag[temptaglen++] = rundatabuf[index];

        if((temptag[0] & 0x1f) == 0x1f)
        {
            temptag[temptaglen++] = rundatabuf[index + 1];

            if(tag[1] & 0x80)
            {
                temptag[temptaglen++] = rundatabuf[index + 2];
            }
        }

        if(memcmp(tag, temptag, temptaglen) == 0)
        {
            return 1;
        }
        index += temptaglen;
    }

    return 0;
}


unsigned char EMVBase_CheckDateFormat(unsigned char * date)
{
	unsigned char i,k,n[3];
	for(i=0;i<3;i++)
	{
		k=date[i];
		if((k&0x0F) > 9)
		{
			return RLT_EMV_ERR;
		}
		if(((k&0xF0)>>4) > 9)
		{
			return RLT_EMV_ERR;
		}
		n[i]=((k&0xF0)>>4)*10 + (k&0x0F);
	}
	if(n[1]<1 || n[1]>12)
	{
		return RLT_EMV_ERR;
	}
	if(n[2]<1 || n[2]>31)
	{
		return RLT_EMV_ERR;
	}

	if(4 == n[1] || 6 == n[1] || 9 == n[1] || 11 == n[1])	//This months contains only 30 days
	{
		if(n[2] > 30)
		{
			return RLT_EMV_ERR;
		}
	}

	if(((0 == (n[0] % 4)) && (0 != (n[0] % 100))) || (0 == (n[0] % 400)))	//leap year
	{
		if(0x02 == n[1])
		{
			if(n[2]>29)
			{
				return RLT_EMV_ERR;
			}
		}
	}
	else
	{
		if(0x02 == n[1])
		{
			if(n[2]>28) return RLT_EMV_ERR;
		}
	}

	return RLT_EMV_OK;
}


void EMVBase_FillIPK(unsigned char* ipkData, EMVBASE_IPK_RECOVER* recovIPK, EMVBASE_CAPK_STRUCT *tempcapk)
{
    unsigned char i;

    recovIPK->DataHead = ipkData[0];
    recovIPK->CertFormat = ipkData[1];
    memcpy(recovIPK->IssuID, (unsigned char*)&ipkData[2], 4);
    memcpy(recovIPK->ExpireDate, (unsigned char*)&ipkData[6], 2);
    memcpy(recovIPK->CertSerial, (unsigned char*)&ipkData[8], 3);
    recovIPK->HashInd = ipkData[11];
    recovIPK->IPKAlgoInd = ipkData[12];
    recovIPK->IPKLen = ipkData[13];
    recovIPK->IPKExpLen = ipkData[14];

    memcpy((unsigned char*)&recovIPK->IPKLeft, (unsigned char*)&ipkData[15], tempcapk->ModulLen - 36);

    for(i = 0; i < 20; i++)
    {
        recovIPK->HashResult[i] = ipkData[tempcapk->ModulLen - 21 + i];
    }

    recovIPK->DataTrail = ipkData[tempcapk->ModulLen - 1];
}


void EMVBase_FillICCPK(unsigned char* ICCPKData, unsigned char tempIPKModulLen, EMVBASE_ICCPK_RECOVER* recovICCPK)
{
    unsigned char i;

    recovICCPK->DataHead = ICCPKData[0];
    recovICCPK->CertFormat = ICCPKData[1];
    memcpy(recovICCPK->AppPAN, (unsigned char*)&ICCPKData[2], 10);
    memcpy(recovICCPK->ExpireDate, (unsigned char*)&ICCPKData[12], 2);
    memcpy(recovICCPK->CertSerial, (unsigned char*)&ICCPKData[14], 3);
    recovICCPK->HashInd = ICCPKData[17];
    recovICCPK->ICCPKAlgoInd = ICCPKData[18];
    recovICCPK->ICCPKLen = ICCPKData[19];
    recovICCPK->ICCPKExpLen = ICCPKData[20];
    memcpy(recovICCPK->ICCPKLeft, (unsigned char*)&ICCPKData[21], tempIPKModulLen - 42);

    for(i = 0; i < 20; i++)
    {
        recovICCPK->HashResult[i] = ICCPKData[tempIPKModulLen - 21 + i];
    }

    recovICCPK->DataTrail = ICCPKData[tempIPKModulLen - 1];
}


void EMVBase_FillSSA(unsigned char* SSAData, EMVBASE_SIGN_STATDATA_RECOVER* recovSSA, unsigned char tempIPKModulLen)
{
    unsigned char i;

    recovSSA->DataHead = SSAData[0];
    recovSSA->DataFormat = SSAData[1];
    recovSSA->HashInd = SSAData[2];
    memcpy(recovSSA->DataAuthCode, (unsigned char*)&SSAData[3], 2);
    //memcpy(recovSSA->PadPattern, (unsigned char*)&SSAData[5], tempIPKModulLen - 26);

    for(i = 0; i < 20; i++)
    {
        recovSSA->HashResult[i] = SSAData[tempIPKModulLen - 21 + i];
    }

    recovSSA->DataTrail = SSAData[tempIPKModulLen - 1];
}

unsigned char EMVBase_FillSDA(unsigned char* SDAData, EMVBASE_SIGN_DYNDATA_RECOVER* recovSDA, unsigned char tempICCPKModulLen)
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
    //memcpy(recovSDA->PadPattern, (unsigned char*)&SDAData[LDD + 4], tempICCPKModulLen - LDD - 25);

    for(i = 0; i < 20; i++)
    {
        recovSDA->HashResult[i] = SDAData[tempICCPKModulLen - 21 + i];
    }

    recovSDA->DataTrail = SDAData[tempICCPKModulLen - 1];
    return RLT_EMV_OK;
}


unsigned char EMVBase_SM_FillIPK(EMVBASE_SM_IPK_CRTFCT *IPKCertfct, unsigned char *IPKCert, unsigned char IPKCertLen, EMVBASE_CAPK_STRUCT *tempcapk)
{
    unsigned char i = 0;

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
	if(IPKCertfct->IPKLen != 64)
	{
        return RLT_EMV_ERR;
    }
    memcpy((unsigned char*)&IPKCertfct->IPK, (unsigned char*)&IPKCert[i], IPKCertfct->IPKLen);
    i += IPKCertfct->IPKLen;
    memcpy((unsigned char*)&IPKCertfct->DGTLSGNTR, (unsigned char*)&IPKCert[i], 64);
    i += tempcapk->ModulLen;

    if(i != IPKCertLen)
    {
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}


unsigned char EMVBase_SM_FillICCPK(EMVBASE_SM_ICCPK_CRTFCT *ICCPKCertfct, unsigned char *ICCPKCert, unsigned char ICCPKCertLen)
{
    unsigned char i = 0;

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

	if(ICCPKCertfct->ICCPKLen != 64)
	{
        return RLT_EMV_ERR;
    }

    memcpy((unsigned char*)&ICCPKCertfct->ICCPK, (unsigned char*)&ICCPKCert[i], ICCPKCertfct->ICCPKLen);
    i += ICCPKCertfct->ICCPKLen;
    memcpy((unsigned char*)&ICCPKCertfct->DGTLSGNTR, (unsigned char*)&ICCPKCert[i], 64);
    i += 64;

    if(i != ICCPKCertLen)
    {
        return RLT_EMV_ERR;
    }
    return RLT_EMV_OK;
}



unsigned char EMVBase_ExpireDateVerify(unsigned char* ExpireDate)
{
    unsigned char CurDate[3], ExpDate[3];

    emvbase_avl_gettagvalue_spec(EMVTAG_TransDate, &CurDate[1], 0, 2);

    if(CurDate[1] > 0x49) { CurDate[0] = 0x19; }
    else{ CurDate[0] = 0x20; }

    if(ExpireDate[1] > 0x49) { ExpDate[0] = 0x19; }
    else{ ExpDate[0] = 0x20; }
    ExpDate[1] = ExpireDate[1];
    ExpDate[2] = ExpireDate[0];

    if(memcmp(CurDate, ExpDate, 3) > 0) //expired
    {
    	EMVBase_Trace(" Date Expire err !!! line=%d\r\n",__LINE__);
		EMVBase_TraceHex("CurDate",CurDate,3);
		EMVBase_TraceHex("ExpDate",ExpDate,3);
        return RLT_EMV_ERR;
    }
    else
    {
        return RLT_EMV_OK;
    }
}

void EMVBase_COMMAND_SELECT(unsigned char *DFFileName, unsigned char DFFileNameLen, unsigned char nextflag, EMVBASE_APDU_SEND *apdu_s)
{
    if(nextflag)
    {
        memcpy(apdu_s->Command, "\x00\xA4\x04\x02", 4);
    }
    else
    {
        memcpy(apdu_s->Command, "\x00\xA4\x04\x00", 4);
    }
    apdu_s->Lc = DFFileNameLen;
    memcpy(apdu_s->DataIn, DFFileName, DFFileNameLen);
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;
}

unsigned char EMVBase_CheckReadAFL(EMVBASETAGCVLITEM *item)
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

void EMVBase_FormReadAflData(EMVBASE_APDU_SEND *apdu_s, unsigned char tempAFL_RecordNum, unsigned char SFI)
{
    memcpy(apdu_s->Command, "\x00\xB2", 2);
    apdu_s->Command[2] = tempAFL_RecordNum;     //record number
    apdu_s->Command[3] = (SFI & 0xF8) | 0x04; 	//SFI
    apdu_s->Lc = 0;
    apdu_s->Le = 256;

    apdu_s->EnableCancel = 1;
}

void EMVBase_PostReadApp(unsigned short i, unsigned char nextflag, EMVBASE_TERMAPP *temptermapp, EMVBASE_APDU_SEND *apdu_s)
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

unsigned char EMVBase_ReadSelectTermRetData(EMVBASE_SELECT_RET* selectRet,unsigned char* DataOut,unsigned short LenOut)
{
	unsigned char j,k;
	unsigned short index,indexFCI,indexFCIProp,len,lenFCI,lenFCIProp;
	int indexIssuerDiscret,lenIssuerDiscret;


	index=0;
	if(DataOut[index]!=0x6F)	//FCIÄ£°å±êÊ¶
	{
		return RLT_ERR_EMV_IccDataFormat; //FCI template
	}
	index++;

    if(EMVBase_ParseExtLen(DataOut,&index,&lenFCI))
    {
    	return RLT_ERR_EMV_IccDataFormat;
    }
	indexFCI=index;

	while(index<indexFCI+lenFCI)
	{
		if(index>=LenOut) return RLT_ERR_EMV_IccDataFormat;
		if(DataOut[index]==0xFF || DataOut[index]==0x00) { index++; continue;}
		else if(DataOut[index]==0x84)	//DFÃû
		{
			if(selectRet->DFNameExist==1) return RLT_ERR_EMV_IccDataFormat;
			index++;
			selectRet->DFNameLen=DataOut[index];
			if(selectRet->DFNameLen>16) return RLT_ERR_EMV_IccDataFormat;
			memcpy(selectRet->DFName,DataOut+index+1,DataOut[index]);
			index+=selectRet->DFNameLen+1;
			selectRet->DFNameExist=1;
		}
		else if(!memcmp(DataOut+index,"\x9F\x11",2)) //ICTI
		{
		    if(selectRet->ICTIExist)
		    {
		    	return RLT_ERR_EMV_IccDataFormat;
		    }
            j = DataOut[index+2];
			index+=3;
			selectRet->ICTI=DataOut[index];
			index+=j;
			selectRet->ICTIExist=1;
		}
		else if(!memcmp(DataOut+index,"\x9F\x12",2)) //App Prefer Name
		{
		    if(selectRet->PreferNameExist)
		    {
		    	return RLT_ERR_EMV_IccDataFormat;
		    }
			index+=2;
			selectRet->PreferNameLen=DataOut[index];
			j = DataOut[index];
			if(selectRet->PreferNameLen > 16) selectRet->PreferNameLen = 16;
			index++;
			memcpy(selectRet->PreferName,DataOut+index,selectRet->PreferNameLen);
			index+=j;
			selectRet->PreferNameExist=1;
		}
		else if(DataOut[index]==0xA5)	//FCIÊý¾Ý×¨ÓÃÄ£°å
		{
			selectRet->FCIPropExist=1;
			index++;

			if(EMVBase_ParseExtLen(DataOut,&index,&lenFCIProp))
			{
				return RLT_ERR_EMV_IccDataFormat;
			}
			indexFCIProp=index;


			while(index<indexFCIProp+lenFCIProp)
			{
				if(index>=LenOut) return RLT_ERR_EMV_IccDataFormat;
				if(DataOut[index]==0xFF || DataOut[index]==0x00) { index++; continue;}
				else if(DataOut[index]==0x88)//SFIÎÄ¼þ
				{
					if(selectRet->SFIExist==1) return RLT_ERR_EMV_IccDataFormat;
					if(DataOut[index+1]!=1) return RLT_ERR_EMV_IccDataFormat;
					index+=2;
					selectRet->SFI=DataOut[index];
					index++;
					selectRet->SFIExist=1;
				}
				else if(!memcmp(DataOut+index,"\x5F\x2D",2)) //Language preference
				{
					index+=2;
					selectRet->LangPreferLen=DataOut[index];
					j = DataOut[index];
					if(selectRet->LangPreferLen > 8) selectRet->LangPreferLen = 8;
					index++;
					memcpy(selectRet->LangPrefer,DataOut+index,selectRet->LangPreferLen);
					index+=j;
					selectRet->LangPreferExist=1;
				}
				else if(!memcmp(DataOut+index,"\x9F\x11",2)) //ICTI
				{
				    if(selectRet->ICTIExist)
				    {
				    	return RLT_ERR_EMV_IccDataFormat;
				    }
                    j = DataOut[index+2];
					index+=3;
					selectRet->ICTI=DataOut[index];
					index+=j;
					selectRet->ICTIExist=1;
				}
				else if(DataOut[index]==0x50) //App Label
				{
				    index++;
					selectRet->AppLabelLen=DataOut[index];
					j= DataOut[index];
					if(selectRet->AppLabelLen > 16) selectRet->AppLabelLen = 16;
					index++;
					memcpy(selectRet->AppLabel,DataOut+index,selectRet->AppLabelLen);
					index+=j;
					selectRet->AppLabelExist=1;
				}
				else if(DataOut[index]==0x87)//App Priority Indicator
				{
					if(selectRet->PriorityExist==1) return RLT_ERR_EMV_IccDataFormat;
					if(DataOut[index+1]!=1) return RLT_ERR_EMV_IccDataFormat;
					index+=2;
					selectRet->Priority=DataOut[index];
					index++;
					selectRet->PriorityExist=1;
				}
				else if(!memcmp(DataOut+index,"\x9F\x38",2)) //PDOL
				{
					if(selectRet->PDOLExist==1) return RLT_ERR_EMV_IccDataFormat;
					index+=2;
					if(EMVBase_ParseExtLen(DataOut,&index,&len))
					{
						return RLT_ERR_EMV_IccDataFormat;
					}

					selectRet->PDOLLen=len;
					index+=selectRet->PDOLLen;
					selectRet->PDOLExist=1;
				}
				else if(!memcmp(DataOut+index,"\x9F\x12",2)) //App Prefer Name
				{
				    if(selectRet->PreferNameExist)
				    {
				    	return RLT_ERR_EMV_IccDataFormat;
				    }
					index+=2;
					selectRet->PreferNameLen=DataOut[index];
					j = DataOut[index];
					if(selectRet->PreferNameLen > 16) selectRet->PreferNameLen = 16;
					index++;
					memcpy(selectRet->PreferName,DataOut+index,selectRet->PreferNameLen);
					index+=j;
					selectRet->PreferNameExist=1;
				}
				else if(!memcmp(DataOut+index,"\xBF\x0C",2)) //FCI Issuer Discretionary Data
				{
					if(selectRet->IssuerDiscretExist==1) return RLT_ERR_EMV_IccDataFormat;
					index+=2;
					if(EMVBase_ParseExtLen(DataOut,&index,&len))
					{
						return RLT_ERR_EMV_IccDataFormat;
					}
					selectRet->IssuerDiscretLen=len;
					selectRet->IssuerDiscretExist=1;

					indexIssuerDiscret=index;
					lenIssuerDiscret=len;
					while(index<indexIssuerDiscret+lenIssuerDiscret)
					{
						if(index>=LenOut) return RLT_ERR_EMV_IccDataFormat;
						if(DataOut[index]==0xFF || DataOut[index]==0x00) { index++; continue;}
						else if(!memcmp(DataOut+index,"\x9F\x4D",2))//Log Entry
						{
							if(selectRet->LogEntryExist==1) return RLT_ERR_EMV_IccDataFormat;
							index+=2;
							if(EMVBase_ParseExtLen(DataOut,&index,&len))
							{
								return RLT_ERR_EMV_IccDataFormat;
							}
							selectRet->LogEntryLen=len;
							index+=selectRet->LogEntryLen;
							selectRet->LogEntryExist=1;
						}
						else
						{
							k=DataOut[index];
							if((k&0x1F)==0x1F)
								index++;
							index++;
							if(EMVBase_ParseExtLen(DataOut,&index,&len))
							{
								return RLT_ERR_EMV_IccDataFormat;
							}
							index+=len;
						}

					}

				}
				else//other unknown TLV data
				{
					k=DataOut[index];
					if((k&0x1F)==0x1F)
						index++;
					index++;
					if(EMVBase_ParseExtLen(DataOut,&index,&len))
					{
						return RLT_ERR_EMV_IccDataFormat;
					}
					index+=len;
				}
			}
			if(index!=indexFCIProp+lenFCIProp)
				return RLT_ERR_EMV_IccDataFormat;
		}
		else if(!memcmp(DataOut+index,"\x9F\x38",2))
		{
			return RLT_ERR_EMV_IccDataFormat;
		}
		else
		{
			k=DataOut[index];
			if((k&0x1F)==0x1F)
				index++;
			index++;
			if(EMVBase_ParseExtLen(DataOut,&index,&len))
			{
				return RLT_ERR_EMV_IccDataFormat;
			}
			index+=len;
		}

	}
	if(index!=indexFCI+lenFCI)
		return RLT_ERR_EMV_IccDataFormat;

	return RLT_EMV_OK;

}

unsigned char EMVBase_SelectTermRetData(EMVBASE_APDU_RESP *apdu_r,EMVBase_TermAidlistUnionStruct *temp_UnionStruct,unsigned char appnumindex,EMVBASE_TERMAPP *temptermapp)
{
    EMVBASE_SELECT_RET selectRet;
    unsigned char retCode;

    EMVBASE_LISTAPPDATA *tempAppData;
	unsigned char *tempapplistnum;


	tempAppData = temp_UnionStruct->AppListCandidate;
	tempapplistnum = &(temp_UnionStruct->AppListCandidatenum);


	if(apdu_r->SW1==0x90 && apdu_r->SW2==0x00) //Select successful
	{
		memset((unsigned char*)&selectRet,0,sizeof(EMVBASE_SELECT_RET));
		retCode=EMVBase_ReadSelectTermRetData(&selectRet,apdu_r->DataOut,apdu_r->LenOut);
		if(retCode!=RLT_EMV_OK)
		{
			return RLT_ERR_EMV_IccDataFormat;
		}

		if(selectRet.DFNameExist==0 || selectRet.FCIPropExist==0)
		{
			return RLT_ERR_EMV_IccDataFormat;
		}
		if(!memcmp((temptermapp+appnumindex)->AID,selectRet.DFName,(temptermapp+appnumindex)->AIDLen))
		{
			if((temptermapp+appnumindex)->AIDLen==selectRet.DFNameLen)
			{
				retCode = RLT_EMV_EXTRACTMATCH;
				(tempAppData+*tempapplistnum)->AidInTermLen = (temptermapp+appnumindex)->AIDLen;
				memcpy((tempAppData+*tempapplistnum)->AidInTerm,(temptermapp+appnumindex)->AID,(tempAppData+*tempapplistnum)->AidInTermLen);
			}
			else
			{
			    retCode = RLT_EMV_PARTICALMATCH;
				if((temptermapp+appnumindex)->ASI!=AID_PARTIAL_MATCH)
				{
					retCode = RLT_EMV_NOTMATCH;
					return retCode;
				}

				(tempAppData+*tempapplistnum)->AidInTermLen = (temptermapp+appnumindex)->AIDLen;
				memcpy((tempAppData+*tempapplistnum)->AidInTerm,(temptermapp+appnumindex)->AID,(tempAppData+*tempapplistnum)->AidInTermLen);
			}
		}
		else
		{
			return RLT_ERR_EMV_IccDataFormat;
		}

		memcpy((tempAppData+*tempapplistnum)->AID,selectRet.DFName,selectRet.DFNameLen);
		(tempAppData+*tempapplistnum)->AIDLen=selectRet.DFNameLen;


		memcpy((tempAppData+*tempapplistnum)->AppLabel,selectRet.AppLabel,selectRet.AppLabelLen);
		(tempAppData+*tempapplistnum)->AppLabelLen=selectRet.AppLabelLen;

		if(selectRet.PriorityExist==1)
		(tempAppData+*tempapplistnum)->Priority=selectRet.Priority;

		if(selectRet.LangPreferExist==1)
		{
			memcpy((tempAppData+*tempapplistnum)->LangPrefer,selectRet.LangPrefer,selectRet.LangPreferLen);
			(tempAppData+*tempapplistnum)->LangPreferLen=selectRet.LangPreferLen;
		}

		if(selectRet.ICTIExist==1)
		{
			(tempAppData+*tempapplistnum)->ICTI=selectRet.ICTI;
			(tempAppData+*tempapplistnum)->ICTILen = 1;
		}
		if(selectRet.PreferNameExist==1)
		{
			memcpy((tempAppData+*tempapplistnum)->PreferName,selectRet.PreferName,selectRet.PreferNameLen);
			(tempAppData+*tempapplistnum)->PreferNameLen=selectRet.PreferNameLen;
		}

		return retCode;
	}
	else
	{
		return RLT_EMV_OK;
	}
}

unsigned char EMVBase_SelectFromTerm(EMVBase_TermAidlistUnionStruct *temp_UnionStruct)
{
    unsigned char retCode;
    unsigned char AppNumIndex = 0;
    EMVBASE_TERMAPP *temptermapp;
	unsigned char temptermlistnum;
	EMVBASE_APDU_SEND apdu_s;
	EMVBASE_APDU_RESP apdu_r;
	EMVBASE_LISTAPPDATA *tempAppData;
	unsigned char bSecond = 0;
    unsigned char *tempapplistnum;


    tempAppData = temp_UnionStruct->AppListCandidate;
	tempapplistnum = &(temp_UnionStruct->AppListCandidatenum);
    temptermapp = (EMVBASE_TERMAPP *)emvbase_malloc(sizeof(EMVBASE_TERMAPP)*100);


	temp_UnionStruct->ReadTermAID(&temptermapp[0],&temptermlistnum);

	EMVBase_Trace("aid num in terminal is %d\r\n",temptermlistnum);

	if(temptermlistnum < 1)
	{
		emvbase_free(temptermapp);
		return RLT_ERR_EMV_NoAppSel;
	}

	while(AppNumIndex < temptermlistnum)
	{
		EMVBase_Trace("aid num in card is %d\r\n",(*tempapplistnum));

	    if((*tempapplistnum) >= temp_UnionStruct->AppListCandidateMaxNum)
    	{
			break;
    	}

		EMVBase_PostReadApp(AppNumIndex,bSecond,temptermapp,&apdu_s);

		temp_UnionStruct->IsoCommand(&apdu_s,&apdu_r);
		if(apdu_r.ReadCardDataOk != RLT_EMV_OK)
		{
		    emvbase_free(temptermapp);
			return RLT_ERR_EMV_APDUTIMEOUT;
		}

		retCode = EMVBase_SelectTermRetData(&apdu_r,temp_UnionStruct,AppNumIndex,&temptermapp[0]);

		if(retCode == RLT_EMV_EXTRACTMATCH)
		{
			(*tempapplistnum)++;
		}
		else if(retCode == RLT_EMV_PARTICALMATCH)
		{
			bSecond=1;
			(*tempapplistnum)++;
			continue;
		}
		else if(retCode == RLT_EMV_NOTMATCH)
		{
			bSecond=1;
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
			bSecond=1;
			continue;

		}

		AppNumIndex++;
		bSecond=0;
	}
	emvbase_free(temptermapp);
    return RLT_EMV_OK;
}


unsigned char EMVBase_ChooseTermApp(EMVBase_TermAidlistUnionStruct *temp_UnionStruct)
{
    unsigned char i, j, temp;
    unsigned char *tempappnum;
    unsigned char noprioritynum = 0;
    unsigned char *tmpapprtiority = NULL;
    unsigned char *AppPriority = NULL;
    EMVBASE_LISTAPPDATA *tempAppAppData;
	EMVBASE_LISTAPPDATA *tempList=NULL;

    tempappnum = &(temp_UnionStruct->AppListCandidatenum);
    tempAppAppData = temp_UnionStruct->AppListCandidate;


    if(*tempappnum < 1)
    {
        return RLT_ERR_EMV_NoAppSel;
    }

	tmpapprtiority = emvbase_malloc(temp_UnionStruct->AppListCandidateMaxNum);
	AppPriority = emvbase_malloc(temp_UnionStruct->AppListCandidateMaxNum);
	tempList = emvbase_malloc(temp_UnionStruct->AppListCandidateMaxNum*sizeof(EMVBASE_LISTAPPDATA));

	if(tmpapprtiority==NULL || AppPriority==NULL || tempList==NULL)
	{
		if(tmpapprtiority) emvbase_free(tmpapprtiority);
		if(AppPriority) emvbase_free(AppPriority);
		if(tempList) emvbase_free(tempList);
		return RLT_EMV_ERR;
	}

	EMVBase_Trace("EMVBase_ChooseTermApp before\r\n");
	for(i=0; i<*tempappnum; i++)
	{
		EMVBase_TraceHex("aid", tempAppAppData[i].AID, tempAppAppData[i].AIDLen);
	}

    for(i = 0; i < *tempappnum; i++)
    {
        AppPriority[i] = (tempAppAppData + i)->Priority & 0x0F;
    }

    if(*tempappnum == 1)
    {
    }
    else
    {
        for(j = 0; j < *tempappnum - 1; j++)
        {
            for(i = 0; i < *tempappnum - 1 - j; i++)
            {
                if(AppPriority[i] > AppPriority[i + 1])
                {
                    temp = AppPriority[i];
                    AppPriority[i] = AppPriority[i + 1];
                    AppPriority[i + 1] = temp;

					memset(tempList, 0, sizeof(EMVBASE_LISTAPPDATA));
					memcpy(&tempList[0], &tempAppAppData[i], sizeof(EMVBASE_LISTAPPDATA));
					memcpy(&tempAppAppData[i], &tempAppAppData[i+1], sizeof(EMVBASE_LISTAPPDATA));
					memcpy(&tempAppAppData[i+1], &tempList[0], sizeof(EMVBASE_LISTAPPDATA));
                }
            }
        }

        noprioritynum = 0;

        for(i = 0; i < *tempappnum; i++)
        {
            if(AppPriority[i] == 0) { noprioritynum++; }
        }

        if(noprioritynum)              //have no priority app
        {
            memcpy(tmpapprtiority, &AppPriority[noprioritynum], *tempappnum - noprioritynum);
            memcpy(&tmpapprtiority[*tempappnum - noprioritynum], AppPriority, noprioritynum);
            memcpy(AppPriority, tmpapprtiority, *tempappnum);

			memcpy(tempList, &tempAppAppData[noprioritynum], (*tempappnum - noprioritynum)*sizeof(EMVBASE_LISTAPPDATA));
			memcpy(&tempList[*tempappnum - noprioritynum], tempAppAppData, noprioritynum*sizeof(EMVBASE_LISTAPPDATA));
			memcpy(tempAppAppData, tempList, (*tempappnum)*sizeof(EMVBASE_LISTAPPDATA));
        }
    }

	EMVBase_Trace("EMVBase_ChooseTermApp after\r\n");
	for(i=0; i<*tempappnum; i++)
	{
		EMVBase_TraceHex("aid", tempAppAppData[i].AID, tempAppAppData[i].AIDLen);
	}
	if(tmpapprtiority) emvbase_free(tmpapprtiority);
	if(AppPriority) emvbase_free(AppPriority);
	if(tempList) emvbase_free(tempList);

	return RLT_EMV_OK;
}

