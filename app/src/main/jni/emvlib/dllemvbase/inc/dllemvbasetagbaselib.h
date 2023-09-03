#ifndef _DLLEMVBASETAGBASELIB_H_
#define _DLLEMVBASETAGBASELIB_H_

#define EMVTAG_SZZTICS								 "\x1F\x01\x00"
#define EMVTAG_PUREStatusCheckSupportFlag			 "\x1F\x02\x00"
#define EMVTAG_IndustryIDNo                          "\x42\x00\x00"
#define EMVTAG_CardAID                               "\x4F\x00\x00"
#define EMVTAG_AppLabel                              "\x50\x00\x00"
#define EMVTAG_PayPassTrack1Data                     "\x56\x00\x00"
#define EMVTAG_Track2Equ                             "\x57\x00\x00"
#define EMVTAG_PAN                                   "\x5A\x00\x00"
#define EMVTAG_CardHoldName                          "\x5F\x20\x00"
#define EMVTAG_AppExpireDate                         "\x5F\x24\x00"
#define EMVTAG_AppEffectDate                         "\x5F\x25\x00"
#define EMVTAG_IssuCountryCode                       "\x5F\x28\x00"
#define EMVTAG_TransCurcyCode                        "\x5F\x2A\x00"
#define EMVTAG_LangPrefer                            "\x5F\x2D\x00"
#define EMVTAG_ServiceCode                           "\x5F\x30\x00"
#define EMVTAG_PANSeq                                "\x5F\x34\x00"
#define EMVTAG_TransCurcyExp                         "\x5F\x36\x00"
#define EMVTAG_IssuerURL                             "\x5F\x50\x00"
#define EMVTAG_IBAN                            	  	 "\x5F\x53\x00"	//20160529_LHD ADD
#define EMVTAG_PAYPASS_AccountType                   "\x5F\x57\x00"
#define EMVTAG_FCITemplate                           "\x6F\x00\x00"
#define EMVTAG_SCRIPT1                               "\x71\x00\x00"
#define EMVTAG_SCRIPT2                               "\x72\x00\x00"
#define EMVTAG_RMTF2                                 "\x77\x00\x00"
#define EMVTAG_RMTF1                                 "\x80\x00\x00"
#define EMVTAG_AmtAuthBin                            "\x81\x00\x00"
#define EMVTAG_AIP                                   "\x82\x00\x00"
#define EMVTAG_DFName                                "\x84\x00\x00"
#define EMVTAG_PUREMSUES                             "\x85\x00\x00"		//PURE Memory Slot Update Entry Setting
#define EMVTAG_AppPriority                           "\x87\x00\x00"
#define EMVTAG_AuthorCode                            "\x89\x00\x00"
#define EMVTAG_AuthRespCode                          "\x8A\x00\x00"
#define EMVTAG_CDOL1                                 "\x8C\x00\x00"
#define EMVTAG_CDOL2                                 "\x8D\x00\x00"
#define EMVTAG_CVMList                               "\x8E\x00\x00"
#define EMVTAG_CAPKI                                 "\x8F\x00\x00"
#define EMVTAG_IPKCert                               "\x90\x00\x00"
#define EMVTAG_IssuerAuthenData                      "\x91\x00\x00"
#define EMVTAG_IPKRem                                "\x92\x00\x00"
#define EMVTAG_SignStatAppData                       "\x93\x00\x00"
#define EMVTAG_AFL                                   "\x94\x00\x00"
#define EMVTAG_TVR                                   "\x95\x00\x00"
#define EMVTAG_TDOL                                  "\x97\x00\x00"
#define EMVTAG_TCHashValue                           "\x98\x00\x00"
#define EMVTAG_PIN                                   "\x99\x00\x00"
#define EMVTAG_TransDate                             "\x9A\x00\x00"
#define EMVTAG_TSI                                   "\x9B\x00\x00"
#define EMVTAG_TransTypeValue                        "\x9C\x00\x00"
#define EMVTAG_DDFName                               "\x9D\x00\x00"
#define EMVTAG_AcquireID                             "\x9F\x01\x00"
#define EMVTAG_AmtAuthNum                            "\x9F\x02\x00"
#define EMVTAG_AmtOtherNum                           "\x9F\x03\x00"
#define EMVTAG_AmtOtherBin                           "\x9F\x04\x00"
#define EMVTAG_AppDiscretData                        "\x9F\x05\x00"
#define EMVTAG_TermAID                               "\x9F\x06\x00"
#define EMVTAG_AUC                                   "\x9F\x07\x00"
#define EMVTAG_AppVer                                "\x9F\x08\x00"
#define EMVTAG_AppVerNum                             "\x9F\x09\x00"
#define EMVTAG_ASRPD                     			 "\x9F\x0A\x00"
#define EMVTAG_CardHoldNameExt                       "\x9F\x0B\x00"
#define EMVTAG_IACDefault                            "\x9F\x0D\x00"
#define EMVTAG_IACDenial                             "\x9F\x0E\x00"
#define EMVTAG_IACOnline                             "\x9F\x0F\x00"
#define EMVTAG_IssuAppData                           "\x9F\x10\x00"
#define EMVTAG_ICTI                                  "\x9F\x11\x00"
#define EMVTAG_AppPreferName                         "\x9F\x12\x00"
#define EMVTAG_LOATC                                 "\x9F\x13\x00"
#define EMVTAG_LCOL                                  "\x9F\x14\x00"
#define EMVTAG_MerchCateCode                         "\x9F\x15\x00"
#define EMVTAG_MerchID                               "\x9F\x16\x00"
#define EMVTAG_PINTryCount                           "\x9F\x17\x00"
#define EMVTAG_MIRPANLast4Digits					 "\x9F\x19\x00"
#define EMVTAG_TOKENREQUESTID					 	 "\x9F\x19\x00"
#define EMVTAG_CountryCode                           "\x9F\x1A\x00"
#define EMVTAG_FloorLimit                            "\x9F\x1B\x00"
#define EMVTAG_TermID                                "\x9F\x1C\x00"
#define EMVTAG_TRMData                               "\x9F\x1D\x00"
#define EMVTAG_IFD_SN                                "\x9F\x1E\x00"
#define EMVTAG_Track1Discret                         "\x9F\x1F\x00"
#define EMVTAG_Track2Discret                         "\x9F\x20\x00"
#define EMVTAG_TransTime                             "\x9F\x21\x00"
#define EMVTAG_TermCAPKI                             "\x9F\x22\x00"
#define EMVTAG_UCOL                                  "\x9F\x23\x00"
#define EMVTAG_PayAccountRefer                 	  	 "\x9F\x24\x00"
#define EMVTAG_PANLast4Digits					     "\x9F\x25\x00"
#define EMVTAG_AppCrypt                              "\x9F\x26\x00"
#define EMVTAG_CryptInfo                             "\x9F\x27\x00"
#define EMVTAG_ExpressPayKernelID             		 "\x9F\x2A\x00"
#define EMVTAG_ICCPIN_EPKCert                        "\x9F\x2D\x00"
#define EMVTAG_ICCPIN_EPKExp                         "\x9F\x2E\x00"
#define EMVTAG_ICCPIN_EPKRem                         "\x9F\x2F\x00"
#define EMVTAG_IPKExp                                "\x9F\x32\x00"
#define EMVTAG_TermCapab                             "\x9F\x33\x00"
#define EMVTAG_CVMResult                             "\x9F\x34\x00"
#define EMVTAG_TermType                              "\x9F\x35\x00"
#define EMVTAG_ATC                                   "\x9F\x36\x00"
#define EMVTAG_UnpredictNum                          "\x9F\x37\x00"
#define EMVTAG_PDOL                                  "\x9F\x38\x00"
#define EMVTAG_POSEntryMode                          "\x9F\x39\x00"
#define EMVTAG_AmtReferCurcy                         "\x9F\x3A\x00"
#define EMVTAG_AppReferCurcy                         "\x9F\x3B\x00"
#define EMVTAG_TransReferCurcyCode                   "\x9F\x3C\x00"
#define EMVTAG_TransReferCurcyExp                    "\x9F\x3D\x00"
#define EMVTAG_TermAddCapab                          "\x9F\x40\x00"
#define EMVTAG_TransSeqCount                         "\x9F\x41\x00"
#define EMVTAG_AppCurcyCode                          "\x9F\x42\x00"
#define EMVTAG_AppReferCurcyExp                      "\x9F\x43\x00"
#define EMVTAG_AppCurcyExp                           "\x9F\x44\x00"
#define EMVTAG_DataAuthCode                          "\x9F\x45\x00"
#define EMVTAG_ICCPKCert                             "\x9F\x46\x00"
#define EMVTAG_ICCPKExp                              "\x9F\x47\x00"
#define EMVTAG_ICCPKRem                              "\x9F\x48\x00"
#define EMVTAG_DDOL                                  "\x9F\x49\x00"
#define EMVTAG_SDATagList                            "\x9F\x4A\x00"
#define EMVTAG_SignDynAppData                        "\x9F\x4B\x00"
#define EMVTAG_ICCDynNum                             "\x9F\x4C\x00"
#define EMVTAG_LogEntry                              "\x9F\x4D\x00"
#define EMVTAG_MerchantName                   		 "\x9F\x4E\x00"
#define EMVTAG_AppDualCurcyCode             		 "\x9F\x50\x00"
#define EMVTAG_OfflineAccumulatorBalance             "\x9F\x50\x00"	//
#define EMVTAG_JCBCardholderVerifyStatus             "\x9F\x50\x00"	//
#define EMVTAG_DRDOL                                 "\x9F\x51\x00"
#define EMVTAG_JCBTermcmpIndicator                   "\x9F\x52\x00"
#define EMVTAG_DPASCVR                               "\x9F\x52\x00" // D-PAS_CT
#define EMVTAG_MCHIPTransCateCode                    "\x9F\x53\x00"	//
#define EMVTAG_JCBDynamicTIP						 "\x9F\x53\x00"	//
#define EMVTAG_DSODSCard                             "\x9F\x54\x00"
#define EMVTAG_MembProductID                      	 "\x9F\x5A\x00"
#define EMVTAG_PaywaveAppProgId                      "\x9F\x5A\x00"
#define EMVTAG_ProductMembNum                        "\x9F\x5B\x00"	//
#define EMVTAG_DSDOL                                 "\x9F\x5B\x00"	//
#define EMVTAG_IssuerScriptREsult                    "\x9F\x5B\x00"	//
#define EMVTAG_DSOperatorID                          "\x9F\x5C\x00"	//
#define EMVTAG_JCBMSDObjectList                      "\x9F\x5C\x00"	//
#define EMVTAG_qPBOCOfflineSpendAmount               "\x9F\x5D\x00"	//
#define EMVTAG_AppCapabilitiesInfor                  "\x9F\x5D\x00"	//
#define EMVTAG_DSID                                  "\x9F\x5E\x00"
#define EMVTAG_DSSlotAvail                           "\x9F\x5F\x00"	//
#define EMVTAG_JCBOfflineBalance                     "\x9F\x5F\x00"	//
#define EMVTAG_PaypassCVC3Track1                     "\x9F\x60\x00"	//
#define EMVTAG_JCBIssuerUpdateParameter              "\x9F\x60\x00"	//
#define EMVTAG_PaypassCVC3Track2                     "\x9F\x61\x00"	//
#define EMVTAG_CardHoldIdNo                          "\x9F\x61\x00"	//
#define EMVTAG_NSICCSAppControl                      "\x9F\x61\x00"	//NSICCS
#define EMVTAG_PaypassPCVC3Track1                    "\x9F\x62\x00"	//
#define EMVTAG_CardHoldIdType                        "\x9F\x62\x00"	//
#define EMVTAG_NSICCSGeographicControl               "\x9F\x62\x00"	//NSICCS
#define EMVTAG_PaypassPunatcTrack1                   "\x9F\x63\x00"	//
#define EMVTAG_CardIdentifyInfo                      "\x9F\x63\x00"	//
#define EMVTAG_NSICCSPinEncControl                   "\x9F\x63\x00"	//NSICCS
#define EMVTAG_Paypassnatctrack1                     "\x9F\x64\x00"
#define EMVTAG_NSICCSIssuerScriptLimit               "\x9F\x64\x00"	//NSICCS
#define EMVTAG_PaypassPcvc3Track2                    "\x9F\x65\x00"
#define EMVTAG_NSICCSIssuerACProprietaryData         "\x9F\x65\x00" //NSICCS
#define EMVTAG_TermTransPredicable                   "\x9F\x66\x00"	//
#define EMVTAG_PaypassPunatcTrack2                   "\x9F\x66\x00"	//
#define EMVTAG_NSICCSIssuerCountryCode               "\x9F\x66\x00"	//NSICCS
#define EMVTAG_PaypassNatcTrack2                     "\x9F\x67\x00"
#define EMVTAG_FromFactor		                     "\x9F\x67\x00"
#define EMVTAG_NSICCSPinTryLimit                     "\x9F\x67\x00" //NSICCS
#define EMVTAG_NSICCSDerivationKeyIndex              "\x9F\x68\x00" //NSICCS
#define EMVTAG_CardAuthData                   		 "\x9F\x69\x00"	//
#define EMVTAG_PayPassUDOL                    		 "\x9F\x69\x00"	//
#define EMVTAG_NSICCSIssuerProprietaryInternalData   "\x9F\x69\x00"	//NSICCS
#define EMVTAG_paypassun                             "\x9F\x6A\x00"
#define EMVTAG_NSICCSIssuerProprietaryRecordData     "\x9F\x6A\x00" //NSICCS
#define EMVTAG_PaypassMagTrack2                      "\x9F\x6B\x00"
#define EMVTAG_NSICCSTerminalProprietaryData         "\x9F\x6B\x00" //NSICCS
#define EMVTAG_CardTransPredicable                   "\x9F\x6C\x00"
#define EMVTAG_NSICCSIssuerBankID                    "\x9F\x6C\x00" //NSICCS
#define EMVTAG_paypassMagAppVer                      "\x9F\x6D\x00"	//
#define EMVTAG_ECResetThreshold                      "\x9F\x6D\x00"	//
#define EMVTAG_ClessReaderCapab						 "\x9F\x6D\x00"	//express
#define EMVTAG_NSICCSBeneficiaryBankID               "\x9F\x6D\x00"	//NSICCS
#define EMVTAG_EnhancedClessReaderCapab         	 "\x9F\x6E\x00" //express
#define EMVTAG_FormFactIndicator                     "\x9F\x6E\x00"	//
#define EMVTAG_PaypassThirdPartyData                 "\x9F\x6E\x00"	//
#define EMVTAG_CLPaymentDeviceType					 "\x9F\x6E\x00"	//FOR MB
#define EMVTAG_JCBDeviceInfo						 "\x9F\x6E\x00" //sjz 2020.6.19
#define EMVTAG_DSSlotManage                          "\x9F\x6F\x00"
#define EMVTAG_ProtectedDataEnvelope1                "\x9F\x70\x00"	//
#define EMVTAG_PUREGDDOL							 "\x9F\x70\x00"	//
#define EMVTAG_MIRCDAResult							 "\x9F\x70\x00"	//
#define EMVTAG_CardInfAndPayCapab                	 "\x9F\x70\x00" //express
#define EMVTAG_ProtectedDataEnvelope2                "\x9F\x71\x00"	//
#define EMVTAG_PUREGDDOLResult						 "\x9F\x71\x00"	//	//PURE GDDOL result data need 512 length .can not insert into emvtree.!!!!
#define EMVTAG_MIRCardProcessRequirement			 "\x9F\x71\x00"	//
#define EMVTAG_MobileCVMResult                		 "\x9F\x71\x00" //express
#define EMVTAG_DPASCPR                               "\x9F\x71\x00" // D-PAS
#define EMVTAG_ProtectedDataEnvelope3                "\x9F\x72\x00"	//
#define EMVTAG_PUREMSID								 "\x9F\x72\x00"	//	//PURE Memory Slot Identifier
#define EMVTAG_ProtectedDataEnvelope4                "\x9F\x73\x00"	//
#define EMVTAG_PURESCRIPTResult						 "\x9F\x73\x00"	//
#define EMVTAG_ProtectedDataEnvelope5                "\x9F\x74\x00"	//
#define EMVTAG_VLPIssuAuthorCode                     "\x9F\x74\x00"	//
#define EMVTAG_PUREDEUResult                     	 "\x9F\x74\x00"	//	//PURE Data Elements Update Result
#define EMVTAG_UnprotectedDataEnvelope1              "\x9F\x75\x00"	//
#define EMVTAG_PUREECHO		                     	 "\x9F\x75\x00"	//	//PURE ECHO
#define EMVTAG_UnprotectedDataEnvelope2              "\x9F\x76\x00"	//
#define EMVTAG_PURETransData		                 "\x9F\x76\x00"	//
#define EMVTAG_UnprotectedDataEnvelope3              "\x9F\x77\x00"	//
#define EMVTAG_MaxECCashBalance                      "\x9F\x77\x00"	//
#define EMVTAG_PUREDedicatedData		    		 "\x9F\x77\x00"	//
#define EMVTAG_AppSpecVersion						 "\x9F\x77\x00"	//
#define EMVTAG_UnprotectedDataEnvelope4              "\x9F\x78\x00"
#define EMVTAG_BancomatFormFactorIndicator           "\x9F\x78\x00"	//lhd add for bancomat
#define EMVTAG_BancomatIACSwitchInterface            "\x9F\x79\x00"	//lhd add for bancomat
#define EMVTAG_UnprotectedDataEnvelope5              "\x9F\x79\x00"	//
#define EMVTAG_VLPAvailableFund                      "\x9F\x79\x00"	//
#define EMVTAG_VLPIndicator                          "\x9F\x7A\x00"
#define EMVTAG_VLPTransLimit                         "\x9F\x7B\x00"
#define EMVTAG_MCLSecureCardFrame                    "\x9F\x7B\x00"	//20210408_lhd NDOT AFC DATA STRUCTURE
#define EMVTAG_CustomerExclusiveData                 "\x9F\x7C\x00"	//
#define EMVTAG_paypassMerchantCustomData             "\x9F\x7C\x00"	//
#define EMVTAG_JCBPartnerDiscretionaryData           "\x9F\x7C\x00"	//sjz 2020.6.19
#define EMVTAG_DSSummary1                            "\x9F\x7D\x00"
#define EMVTAG_DPASVERNO                             "\x9F\x7D\x00" // D-PAS
#define EMVTAG_NSICCSCardLifeCycleData               "\x9F\x7D\x00" //NSICCS
#define EMVTAG_MobileSupportIndicator                "\x9F\x7E\x00"
#define EMVTAG_DPASDCVVV2                            "\x9F\x7E\x00" // D-PAS
#define EMVTAG_DSUnpredictableNumber                 "\x9F\x7F\x00"
#define EMVTAG_DPASDCVVV1                            "\x9F\x80\x00" // D-PAS
#define EMVTAG_PUREMSUEType1						 "\xA2\x00\x00"		//PURE Memory Slot Update Entry type1 need 260 length .can not insert into emvtree.!!!!
#define EMVTAG_PUREMSUEType2						 "\xA3\x00\x00"		//PURE Memory Slot Update Entry type2 need 260 length .can not insert into emvtree.!!!!
#define EMVTAG_PUREMSUEType3						 "\xA4\x00\x00"		//PURE Memory Slot Update Entry type3 need 260 length .can not insert into emvtree.!!!!
#define EMVTAG_FCIProterty                           "\xA5\x00\x00"
#define EMVTAG_FCIDisData                            "\xBF\x0C\x00"
#define EMVTAG_DPASDSUpdateTemplate                  "\xBF\x10\x00" // D-PAS
#define EMVTAG_DPASWriteDataStorageTemplate          "\xBF\x11\x00" // D-PAS
#define EMVTAG_MIRSignedApplicationTags              "\xBF\x61\x00"
#define EMVTAG_MIRUnsignApplicationTags              "\xBF\x62\x00"
#define EMVTAG_PUREMSUTemp						 	 "\xBF\x70\x00"		//PURE Memory Slot Update Template need 800 length .can not insert into emvtree.!!!!
#define EMVTAG_PUREMSReadTemp						 "\xBF\x71\x00"		//PURE Memory Slot Read Template
#define EMVTAG_PURECCID                            	 "\xC5\x00\x00"
#define EMVTAG_PURETTPI                            	 "\xC7\x00\x00"
#define EMVTAG_PURECRMCD                           	 "\xCD\x00\x00"		//PURE CRM Currency Code
#define EMVTAG_DPASOfflineBalance                    "\xD1\x00\x00" // D-PAS
#define EMVTAG_WalletProviderNo                      "\xD1\x00\x00" // BANCOMAT
#define EMVTAG_PAYWAVEAP_RCTL                        "\xDF\x00\x00"
#define EMVTAG_PAYWAVEAP_CVMLimit                    "\xDF\x01\x00"	//
#define EMVTAG_PUREDF01			                     "\xDF\x01\x00"	//
#define EMVTAG_PAYWAVEAP_FloorLimit                  "\xDF\x02\x00"	//
#define EMVTAG_PUREDF02			                     "\xDF\x02\x00"	//
#define EMVTAG_PAYWAVEAP_EDDAVersion                 "\xDF\x03\x00"	//
#define EMVTAG_PUREDF03			                     "\xDF\x03\x00"	//
#define EMVTAG_RupayServiceAvailabilityInfo			 "\xDF\x03\x00"	//
#define EMVTAG_PAYWAVEAP_CVMREQ                      "\xDF\x04\x00"	//
#define EMVTAG_PUREDF04			                     "\xDF\x04\x00"	//
#define EMVTAG_PAYWAVEAP_DispOfflineFundInd          "\xDF\x05\x00"	//
#define EMVTAG_PUREDF05			                     "\xDF\x05\x00"	//
#define EMVTAG_PaywaveRCP                            "\xDF\x06\x00"	//
#define EMVTAG_PUREDF06			                     "\xDF\x06\x00"	//
#define EMVTAG_PUREDF07			                     "\xDF\x07\x00"	//
#define EMVTAG_RupayServiceDirectory			     "\xDF\x07\x00"	//
#define EMVTAG_PUREDF08			                     "\xDF\x08\x00"
#define EMVTAG_PUREDF09			                     "\xDF\x09\x00"
#define EMVTAG_PUREDF0A			                     "\xDF\x0A\x00"
#define EMVTAG_PURECOTC							 	 "\xDF\x13\x00"	//CONSECUTIVE_OFFLINE_TRANS_COUNTER,only for Algeria customer
#define EMVTAG_RupayServiceManageInfo			     "\xDF\x15\x00"
#define EMVTAG_RupayServiceID			     		 "\xDF\x16\x00"
#define EMVTAG_PURELCOL								 "\xDF\x1B\x00"
#define EMVTAG_PUREUCOL								 "\xDF\x1C\x00"
#define EMVTAG_RupayServiceSummary			     	 "\xDF\x22\x00"
#define EMVTAG_PUREZeroAmountAllowedFlag			 "\xDF\x22\x00"
#define EMVTAG_RupayServiceSignature			     "\xDF\x23\x00"
#define EMVTAG_JCBCDCVMLIMIT						 "\xDF\x24\x00"
#define EMVTAG_RupayServiceRelatedData			     "\xDF\x33\x00"
#define EMVTAG_RupayCardCVMLimit			     	 "\xDF\x34\x00"
#define EMVTAG_RupayTermAddCapabExtern			     "\xDF\x3A\x00"
#define EMVTAG_DPASCardFeatureVersionNum   			 "\xDF\x3A\x00" // D-PAS
#define EMVTAG_RupayAUCExtern			     		 "\xDF\x3B\x00"
#define EMVTAG_DPASCardFeatureDescriptor			 "\xDF\x3B\x00" // D-PAS
#define EMVTAG_DPASExtendedLoggingData			     "\xDF\x3C\x00" // D-PAS
#define EMVTAG_DPASDataStorageDirectory			     "\xDF\x3D\x00" // D-PAS
#define EMVTAG_CardID			                     "\xDF\x3E\x00" // D-PAS
#define EMVTAG_MIRTransRecoveryCounter         		 "\xDF\x41\x00"
#define EMVTAG_PUREAppVerUL							 "\xDF\x42\x00" //Algeria Special Request,Terminal App Version Upper Limit
#define EMVTAG_PUREAppVerLL							 "\xDF\x43\x00" //Algeria Special Request,Terminal App Version Lower Limit
#define EMVTAG_RupayServiceTermData         		 "\xDF\x45\x00"
#define EMVTAG_DPASCDCVMStatus         		         "\xDF\x45\x00" // D-PAS_CT
#define EMVTAG_RupayPRMicc        		 			 "\xDF\x49\x00"
#define EMVTAG_POSCII                                "\xDF\x4B\x00"	//
#define EMVTAG_RupayPrePRMicc                        "\xDF\x4B\x00"	//
#define EMVTAG_RupayCardTransLimit                   "\xDF\x4C\x00"
#define EMVTAG_CirqueLogEntry                        "\xDF\x4D\x00"
#define EMVTAG_RupayPRMacqKeyIndexCard               "\xDF\x4E\x00"
#define EMVTAG_MIRTerminalFloorLimit         		 "\xDF\x51\x00"
#define EMVTAG_MIRTerminalNoCVMLimit         		 "\xDF\x52\x00"
#define EMVTAG_MIRTermCLLimit_NonCDCVM	 	 		 "\xDF\x53\x00"
#define EMVTAG_MIRTermclLimit_CDCVM         		 "\xDF\x54\x00" //
#define EMVTAG_RupayPRMacqKCV         		 		 "\xDF\x54\x00"	//
#define EMVTAG_MIRTerminalTPMCapabilities         	 "\xDF\x55\x00"
#define EMVTAG_MIRTransRecoveryLimit			 	 "\xDF\x56\x00"
#define EMVTAG_DSInputCARD                           "\xDF\x60\x00"
#define EMVTAG_DSDigestH                             "\xDF\x61\x00"	//
#define EMVTAG_QCARDPLUS                             "\xDF\x61\x00"	//
#define EMVTAG_RupayICCDynamicSignatureRecordID      "\xDF\x61\x00"	//
#define EMVTAG_DSODSInfo                             "\xDF\x62\x00"
#define EMVTAG_DSODSTerm                             "\xDF\x63\x00"
#define EMVTAG_ODAalgrthFlag                         "\xDF\x69\x00"
#define EMVTAG_MIROperationDOL         	 			 "\xDF\x6F\x00"
#define EMVTAG_MIRApplicationInfo         	 	 	 "\xDF\x70\x00"
#define EMVTAG_MIRTransProcessingMode         		 "\xDF\x71\x00"
#define EMVTAG_MIRIssuerCertificateSN         		 "\xDF\x73\x00"
#define EMVTAG_MIRKernelVerificationResults          "\xDF\x74\x00"
#define EMVTAG_RUPAYDF7C							 "\xDF\x7C\x00"
#define EMVTAG_PURETAID		                         "\xDF\x7F\x00"
#define EMVTAG_DSSummary2                            "\xDF\x81\x01"
#define EMVTAG_DSSummary3                            "\xDF\x81\x02"
#define EMVTAG_BalanceBeforeGAC                      "\xDF\x81\x04"
#define EMVTAG_BalanceAfterGAC                       "\xDF\x81\x05"
#define EMVTAG_PayassDataNeeded                      "\xDF\x81\x06"
#define EMVTAG_CDOL1RelatedData                      "\xDF\x81\x07"
#define EMVTAG_DSACType                              "\xDF\x81\x08"
#define EMVTAG_DSInputTerm                           "\xDF\x81\x09"
#define EMVTAG_DSODSInfoReader                       "\xDF\x81\x0A"
#define EMVTAG_DSSummaryStatus                       "\xDF\x81\x0B"
#define EMVTAG_KernelID                              "\xDF\x81\x0C"
#define EMVTAG_DSVNTerm                              "\xDF\x81\x0D"
#define EMVTAG_POSTGACPDS                            "\xDF\x81\x0E"
#define EMVTAG_PREGACPDS                             "\xDF\x81\x0F"
#define EMVTAG_ProfirstWF                            "\xDF\x81\x10"
#define EMVTAG_PDOLData                              "\xDF\x81\x11"
#define EMVTAG_TagsToRead                            "\xDF\x81\x12"
#define EMVTAG_DRDOLRelatedData                      "\xDF\x81\x13"
#define EMVTAG_ReferenceControlParameter             "\xDF\x81\x14"
#define EMVTAG_ErrorIndication                       "\xDF\x81\x15"
#define EMVTAG_UserInterReqData                      "\xDF\x81\x16"
#define EMVTAG_CARDINPUTCAP                          "\xDF\x81\x17"
#define EMVTAG_PAYPASSCVMRequired                    "\xDF\x81\x18"
#define EMVTAG_PAYPASSNOCVMRequired                  "\xDF\x81\x19"
#define EMVTAG_TermUDOL                              "\xDF\x81\x1A"
#define EMVTAG_KernelConfiguration                   "\xDF\x81\x1B"
#define EMVTAG_MaxLifeTimeTornLog                    "\xDF\x81\x1C"
#define EMVTAG_MaxNumTornLog                         "\xDF\x81\x1D"
#define EMVTAG_magcvmrequirer                        "\xDF\x81\x1E"
#define EMVTAG_SecurityCapability                    "\xDF\x81\x1F"
#define EMVTAG_TACDefault                            "\xDF\x81\x20"
#define EMVTAG_TACDenial                             "\xDF\x81\x21"
#define EMVTAG_TACOnline                             "\xDF\x81\x22"
#define EMVTAG_clessofflinelimitamt                  "\xDF\x81\x23"
#define EMVTAG_clessofflineamt                       "\xDF\x81\x24"
#define EMVTAG_ondevRTCL                             "\xDF\x81\x25"
#define EMVTAG_termcvm_limit                         "\xDF\x81\x26"
#define EMVTAG_TimeOutValue                          "\xDF\x81\x27"
#define EMVTAG_IDSStatus                             "\xDF\x81\x28"
#define EMVTAG_OutcomeParameterSet                   "\xDF\x81\x29"
#define EMVTAG_Paypassconverttrack1                  "\xDF\x81\x2A"
#define EMVTAG_DPASconverttrack1                     "\xDF\x81\x2A" // D-PAS
#define EMVTAG_Paypassconverttrack2                  "\xDF\x81\x2B"
#define EMVTAG_DPASconverttrack2                     "\xDF\x81\x2B" // D-PAS
#define EMVTAG_magnocvmrequirer                      "\xDF\x81\x2C"
#define EMVTAG_MessageHoldTime                       "\xDF\x81\x2D"
#define EMVTAG_HoldTimeValue                         "\xDF\x81\x30"
#define EMVTAG_PhoneMessageTable					 "\xDF\x81\x31"
#define EMVTAG_MinimumRelayResistanceGracePeriod     "\xDF\x81\x32"
#define EMVTAG_MaximumRelayResistanceGracePeriod     "\xDF\x81\x33"
#define EMVTAG_TerminalExpectedTTFRRCAPDU            "\xDF\x81\x34"
#define EMVTAG_TerminalExpectedTTFRRPAPDU            "\xDF\x81\x35"
#define EMVTAG_RelayResistanceAccuracyThreshold      "\xDF\x81\x36"
#define EMVTAG_RRTTimeMismatchThreshold              "\xDF\x81\x37"

#define EMVTAG_TerminalRelayResistanceEntropy			"\xDF\x83\x01"
#define EMVTAG_DeviceRelayResistanceEntropy				"\xDF\x83\x02"
#define EMVTAG_MinTimeForProcessingRelayResistanceAPDU 	"\xDF\x83\x03"
#define EMVTAG_MaxTimeForProcessingRelayResistanceAPDU 	"\xDF\x83\x04"
#define EMVTAG_DeviceEstimatedTransTForRRRAPDU			"\xDF\x83\x05"
#define EMVTAG_MeasureRelayResistanceProcessingTime		"\xDF\x83\x06"
#define EMVTAG_RRPCounter								"\xDF\x83\x07"

#define EMVTAG_PUREKernelCap						 "\xDF\x83\x08"	//Pure only
#define EMVTAG_PUREMTOL								 "\xDF\x83\x09"	//Pure only

#define EMVTAG_MIRTACDenial                          "\xFF\x01\x00"
#define EMVTAG_MIRTACOnline                          "\xFF\x02\x00"
#define EMVTAG_MIRTACDefault                         "\xFF\x03\x00"
#define EMVTAG_MIRDataExchangeTagLis                 "\xFF\x04\x00"
#define EMVTAG_TornRecord                            "\xFF\x81\x01"
#define EMVTAG_WriteBeforeGAC                        "\xFF\x81\x02"
#define EMVTAG_WriteAfterGAC                         "\xFF\x81\x03"
#define EMVTAG_PaypassDataToSend                     "\xFF\x81\x04"
#define EMVTAG_PaypassDataRecord                     "\xFF\x81\x05"
#define EMVTAG_DiscretionaryData                     "\xFF\x81\x06"
#define EMVTAG_TermDDOL                              "\xFF\x81\x07"
#define EMVTAG_TermTDOL                              "\xFF\x81\x09"
#define EMVTAG_Threshold                             "\xFF\x81\x0A"
#define EMVTAG_MaxTargetPercent                      "\xFF\x81\x0B"
#define EMVTAG_TargetPercent                         "\xFF\x81\x0C"
#define EMVTAG_bShowRandNum                          "\xFF\x81\x0D"
#define EMVTAG_bForceOnline                          "\xFF\x81\x0E"
#define EMVTAG_RupayCardKpsk                         "\xFF\x81\x10"
#define EMVTAG_RupayServiceQualifier                 "\xFF\x81\x11"
#define EMVTAG_TagsToReadYet						 "\xFF\x81\x12"
#define EMVTAG_TagsToWriteYetAfterGenAC				 "\xFF\x81\x13"
#define EMVTAG_TagsToWriteYetBeforeGenAC			 "\xFF\x81\x14"
#define EMVTAG_TagsMissingPDOLDataFlag				 "\xFF\x81\x15"
#define EMVTAG_BancomatCCVR							 "\xFF\x81\x20" //lhd add for bancomat
#define EMVTAG_BancomatTACSwitchInterface            "\xFF\x81\x21"	//lhd add for bancomat

#define EMVTAG_SCRIPT71RESULT                        "\xFF\x81\x71"
#define EMVTAG_SCRIPT72RESULT                        "\xFF\x81\x72"
#define EMVTAG_CDOL2RelatedData                      "\xFF\x81\x73"
#define EMVTAG_SupportTermCheck                      "\xFF\x81\x74"
#define EMVTAG_DDOLRelatedData                       "\xFF\x81\x75"
#define EMVTAG_ICCPIN_EPKModul                       "\xFF\x81\x76"
#define EMVTAG_SupportOfflineZeroAmount              "\xFF\x81\x77" // D-PAS
#define EMVTAG_TermTypeModified                      "\xFF\x81\x78"
#define EMVTAG_JCBCombinationOptions                 "\xFF\x81\x78"
#define EMVTAG_SupportZeroAmount                     "\xFF\x81\x78" // D-PAS
#define EMVTAG_MIRODOLData                       	 "\xFF\x81\x79"
#define EMVTAG_JCBRemovalTimeout                     "\xFF\x81\x79"
#define EMVTAG_GPOReturnData                         "\xFF\x81\x79" // D-PAS
#define EMVTAG_JCBStaticTIP                       	 "\xFF\x81\x80"
#define EMVTAG_CLFLOORLIMTEPRE                       "\xFF\x81\x80" // D-PAS
#define EMVTAG_JCBThresholdRandomSelect              "\xFF\x81\x81"
#define EMVTAG_DPASSupportDeferredAuthorization      "\xFF\x81\x81" // D-PAS
#define EMVTAG_DPASSupportDataStorage                "\xFF\x81\x82" // D-PAS
#define EMVTAG_DPASSupportExtendedLogging            "\xFF\x81\x83" // D-PAS
#define EMVTAG_DPASSupportTearingRecovery            "\xFF\x81\x84" // D-PAS
#define EMVTAG_DPASDataGPOP1                         "\xFF\x81\x85" // D-PAS
#define EMVTAG_ONLINEENCPIN							 "\xFF\x81\x86"	//special tag


#define EMVTAGUPDATECONDITION_DET		0x80
#define EMVTAGUPDATECONDITION_K         0x40
#define EMVTAGUPDATECONDITION_ACT       0x10
#define EMVTAGUPDATECONDITION_RA        0x20


#define EMVTAGFORMAT_N      1
#define EMVTAGFORMAT_B      2
#define EMVTAGFORMAT_ANS    4
#define EMVTAGFORMAT_CN     8
#define EMVTAGFORMAT_AN     0x10



#define EMVTAGTYPE_ALL          0xFFFFFFFF
#define EMVTAGTYPE_PAYPASS      0x00000001
#define EMVTAGTYPE_PAYWAVE      0x00000002
#define EMVTAGTYPE_QPBOC        0x00000004
#define EMVTAGTYPE_MB		    0x00000008
#define EMVTAGTYPE_EMV          0x00000010
#define EMVTAGTYPE_EXPRESSPAY   0x00000020
#define EMVTAGTYPE_JSPEEDY   	0x00000040
#define EMVTAGTYPE_DPAS		   	0x00000080
#define EMVTAGTYPE_MIR		   	0x00000100
#define EMVTAGTYPE_PURE		   	0x00000200
#define EMVTAGTYPE_RUPAY		0x00000400
#define EMVTAGTYPE_NSICCS		0x00000800
#define EMVTAGTYPE_BANCOMAT		0x00001000
#define EMVTAGTYPE_NULL         0x80000000



#define EMVBASETAGNOTEXIST  0
#define EMVBASETAGEMPTY     1
#define EMVBASETAGEXIST     2


typedef struct _EMVBASETAG
{
	unsigned char Tag[3];
}EMVBASETAG;

typedef struct{

	unsigned char Tag[3];
	unsigned char updatecondition;
	unsigned short minlen;
	unsigned short maxlen;
	unsigned char datafomat;
	unsigned int SupAppType;

}EMVBASETAGITEM;


typedef struct{

	unsigned char Tag[3];
	unsigned char updatecondition;
	unsigned short minlen;
	unsigned short maxlen;
	unsigned char datafomat;
	unsigned int SupAppType;
	unsigned short len;
	unsigned char *data;

}EMVBASETAGCVLITEM;

extern int emvbase_TagBaseLib_read(unsigned char *tag,unsigned char taglen,EMVBASETAGCVLITEM *emvtagitem,unsigned int apptype);

#endif


