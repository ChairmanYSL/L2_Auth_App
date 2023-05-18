/**
************************************************************************************************************************
*                                        COPYRIGHT (C) 2013 Kingsee Tech. Co., Ltd.
*
* @file     emv_ics.h
* @brief   	ICS - Implementation Conformance Statement
* @author   Kingsee Development Team - chenjinhai@szzt.com.cn
* @version  V43D1.0.28
*
************************************************************************************************************************
*/
#ifndef EMV_ICS_H_
#define EMV_ICS_H_


#define __ICS

/**
 * AS   : Application Selection
 * Macro:
 * AS_Support_PSE                   : [O]Support PSE selection Method
 * AS_Support_CardHolder_Confirm    : [O]Support Cardholder Confirmation
 * AS_Support_Prefferd_Order        : [O]Does Terminal have a preferred order of displaying applications
 * AS_Support_Partial_AID           : [M]Does terminal perform partial AID selection
 * AS_Support_Multi_Language        : [O]Does the terminal have multi language support
 * AS_Support_Common_Charset        : [M]Does the terminal support the Common Character Set as defined in Annex B Table 20 Book 4
 * AS_Support_EMV_Language_Select   : [O]Does the Terminal support the EMV Language Selection method
 *
 * @see EMV 4.3a ICS Level2
 */
#define AS_Support_PSE                      (0x0080)
#define AS_Support_CardHolder_Confirm       (0x0040)
#define AS_Support_Preferred_Order          (0x0020)
#define AS_Support_Partial_AID              (0x0010)
#define AS_Support_Multi_Language           (0x0008)
#define AS_Support_Common_Charset           (0x0004)
#define AS_Support_EMV_Language_Select      (0x0002)

/**
 * DA   : Data Authentication
 * IPKC : Issuer Public Key Certificate
 * CAPK : Certification Authority Public Key
 * Macro:
 * DA_Support_IPKC_Revoc_Check      : [O]During data authentication does the terminal check validity for revocation of Issuer Public Key Certificate
 * DA_Support_Default_DDOL          : [C]Does the terminal contain a default DDOL
 * DA_Support_CAPKLoad_Fail_Action  : [O]Is operator action required when loading of CA Public Key fails
 * DA_Support_CAPK_Checksum         : [O]CA Public Key verified with CA Public Key Check Sum
 *
 * @see EMV 4.3a ICS Level2
 */
#define DA_Support_IPKC_Revoc_Check         (0x0180)
#define DA_Support_Default_DDOL             (0x0140)
#define DA_Support_CAPKLoad_Fail_Action     (0x0120)
#define DA_Support_CAPK_Checksum            (0x0110)

/**
 * CV   : Cardholder Verification
 * CVM  : Cardholder Verification Methods
 * Macro:
 * CV_Support_Bypass_PIN            : [O]Terminal supports bypass PIN Entry
 * CV_Support_PIN_Try_Counter       : [O]Terminal supports Get Data for PIN Try Counter
 * CV_Support_Fail_CVM              : [M]Terminal supports Fail CVM
 * CV_Support_Amounts_before_CVM    : [O]Are amounts known before CVM processing
 * CV_Support_Bypass_ALL_PIN        : [O]If the terminal supports bypass PIN Entry: When selecting to bypass a PIN method, all other PIN entry methods are also considered bypassed.
 *
 * @see EMV 4.3a ICS Level2
 */
#define CV_Support_Bypass_PIN               (0x0280)
#define CV_Support_PIN_Try_Counter          (0x0240)
#define CV_Support_Fail_CVM                 (0x0220)
#define CV_Support_Amounts_before_CVM       (0x0210)
#define CV_Support_Bypass_ALL_PIN           (0x0208)

/**
 * TRM  : Terminal Risk Management
 * Macro:
 * TRM_Support_FloorLimit           : [O]Floor limit checking (Mandatory for offline only terminals and offline terminals with online capability)
 * TRM_Support_RandomSelect         : [O]Random Transaction Selection (Mandatory for offline terminals with online capability, except when cardholder controlled)
 * TRM_Support_VelocityCheck        : [O]Velocity Checking (Mandatory for offline only terminals and offline terminals with online capability)
 * TRM_Support_TransLog             : [O]Transaction Log
 * TRM_Support_ExceptionFile        : [O]Exception File
 * TRM_Support_AIPBased             : [M]Performance of Terminal Risk Management irrespective of AIP setting (expected behavior)
 * TRM_Use_EMV_LogPolicy            : [O]EMV has a different log policy with PBOC2, marked here
 *
 * @see EMV 4.3a ICS Level2
 */
#define TRM_Support_FloorLimit              (0x0380)
#define TRM_Support_RandomSelect            (0x0340)
#define TRM_Support_VelocityCheck           (0x0320)
#define TRM_Support_TransLog                (0x0310)
#define TRM_Support_ExceptionFile           (0x0308)
#define TRM_Support_AIPBased                (0x0304)
#define TRM_Use_EMV_LogPolicy               (0x0302)

/**
 * TAA  : Terminal Action Analysis
 * TAC  : Terminal Action Codes
 * DAC  : Default Action Codes
 * Macro:
 * TAA_Support_TAC                  : [O]Does the terminal support the Terminal Action Codes
 * TAA_Support_DAC_before_1GenAC    : [C]Does Offline Only Terminal process Default Action Codes prior to First Generate AC
 * TAA_Support_DAC_after_1GenAC     : [C]Does Offline Only Terminal process Default Action Codes after First Generate AC
 * TAA_Support_Skip_DAC_OnlineFail  : [C]Does this online only terminal skip processing TAC/IAC-Default and automatically request an AAC when unable to go online
 * TAA_Support_DAC_OnlineFail       : [C]Does this online only terminal process TAC/IAC-Default as normal when unable to go online
 * TAA_Support_CDAFail_Detected     : [C]Device capable of detecting CDA failure before Terminal Action Analysis
 * TAA_Support_CDA_Always_in_ARQC   : [C]CDA always requested in a first Gen AC, ARQC request
 * *TAA_Support_CDA_Never_in_ARQC   : [C]CDA never requested in a first Gen AC, ARQC request
 * TAA_Support_CDA_Alawys_in_2TC    : [C]CDA always requested in a second Gen AC when successful host response is received, with TC request
 * *TAA_Support_CDA_Never_in_2TC    : [C]CDA never requested in a second Gen AC when successful host response is received, with TC request
 *
 * @see EMV 4.3a ICS Level2
 */
#define TAA_Support_TAC                     (0x0480)
#define TAA_Support_DAC_before_1GenAC       (0x0440)
#define TAA_Support_DAC_after_1GenAC        (0x0420)
#define TAA_Support_Skip_DAC_OnlineFail     (0x0410)
#define TAA_Support_DAC_OnlineFail          (0x0408)
#define TAA_Support_CDAFail_Detected        (0x0404)
#define TAA_Support_CDA_Always_in_ARQC      (0x0402)
#define TAA_Support_CDA_Alawys_in_2TC       (0x0401)

/**
 * CP  : Completion Process
 * Macro:
 * CP_Support_Force_Online          : [O]Transaction Forced Online Capability
 * CP_Support_Force_Accept          : [O]Transaction Forced Acceptance Capability
 * CP_Support_Advices               : [O]Does terminal Support Advices
 * CP_Support_Issuer_VoiceRef       : [C]Does the terminal support Issuer initiated Voice Referrals
 * CP_Support_Batch_Data_Capture    : [O]Does the terminal support Batch Data Capture
 * CP_Support_Online_Data_capture   : [O]Does the terminal support Online Data Capture
 * CP_Support_Default_TDOL          : [O]Does the terminal support a Default TDOL
 *
 * @see EMV 4.3a ICS Level2
 */
#define CP_Support_Force_Online             (0x0580)
#define CP_Support_Force_Accept             (0x0540)
#define CP_Support_Advice                   (0x0520)
#define CP_Support_Issuer_VoiceRef          (0x0510)
#define CP_Support_Batch_Data_Capture       (0x0508)
#define CP_Support_Online_Data_capture      (0x0504)
#define CP_Support_Default_TDOL             (0x0502)

/**
 * MISC : Miscellaneous
 * Macro:
 * MISC_Support_Account_Select          : [O]Does the terminal support account type selection
 * MISC_Support_ISDL_Greater_than_128   : [O]Is the Issuer Script device limit greater than 128 bytes
 * MISC_Support_Internal_Date_Mana      : [O]Does the terminal support Internal Date Management
 *
 * @see EMV 4.3a ICS Level2
 */
#define MISC_Support_Account_Select         (0x0680)
#define MISC_Support_ISDL_Greater_than_128  (0x0640)
#define MISC_Support_Internal_Date_Mana     (0x0620)


#define __TC

/**
 * TC: Terminal Capabilities
 * Byte1: Card Data Input Capability
 * Byte2: CVM Capability
 * Byte3: Security Capability
 */
#define TC_Manual_Key_Entry                 (0x0080)
#define TC_Magnetic_Stripe                  (0x0040)
#define TC_IC_With_Contacts                 (0x0020

#define TC_Plaintext_PIN                    (0x0180)
#define TC_Enciphered_PIN_Online            (0x0140)
#define TC_Signature_Paper                  (0x0120)
#define TC_Enciphered_PIN_Offline           (0x0110)  // EMV4.2b
#define TC_No_CVM_Required                  (0x0108)
#define TC_Cardholder_Cert                  (0x0101)  // PBOC2.0

#define TC_SDA                              (0x0280)
#define TC_DDA                              (0x0240)
#define TC_Card_Capture                     (0x0220)
#define TC_CDA                              (0x0208)


#define __ATC

/**
 * ATC : Additional Terminal Capabilities
 * Byte 1: Transaction Type Capability
 * Byte 2: Transaction Type Capability
 * Byte 3: Terminal Data Input Capability
 * Byte 4: Terminal Data Output Capability
 * Byte 5: Terminal Data Output Capability
 */
#define ATC_Cash                            (0x0080)
#define ATC_Goods                           (0x0040)
#define ATC_Services                        (0x0020)
#define ATC_Cashback                        (0x0010)
#define ATC_Inquiry                         (0x0008)
#define ATC_Transfer                        (0x0004)
#define ATC_Payment                         (0x0002)
#define ATC_Administrative                  (0x0001)

#define ATC_Cash_Deposit                    (0x0180)

#define ATC_Numeric_Keys                    (0x0280)
#define ATC_Alphabetic_Special_Keys         (0x0240)
#define ATC_Command_Keys                    (0x0220)
#define ATC_Function_Keys                   (0x0210)

#define ATC_Print_Attendant                 (0x0380)
#define ATC_Print_Cardholder                (0x0340)
#define ATC_Display_Attendant               (0x0320)
#define ATC_Display_Cardholder              (0x0310)
#define ATC_Code_Table_10                   (0x0302)
#define ATC_Code_Table_9                    (0x0301)

#define ATC_Code_Table_8                    (0x0480)
#define ATC_Code_Table_7                    (0x0440)
#define ATC_Code_Table_6                    (0x0420)
#define ATC_Code_Table_5                    (0x0410)
#define ATC_Code_Table_4                    (0x0408)
#define ATC_Code_Table_3                    (0x0404)
#define ATC_Code_Table_2                    (0x0402)
#define ATC_Code_Table_1                    (0x0401)




#define BN_TTQ_SUP_MSD                              (0x0080)//!< 支持非接触式MSD
#define BN_TTQ_SUP_PBOC_CL                          (0x0040) //!< 支持非接触式借贷记应用
#define BN_TTQ_SUP_QPBOC                            (0x0020) //!< 支持qPBOC
#define BN_TTQ_SUP_PBOC                             (0x0010) //!< 支持接触式借贷记应用
#define BN_TTQ_SUP_OFFLINE_ONLY                     (0x0008) //!< 终端仅支持脱机
#define BN_TTQ_SUP_ONLINE_PIN                       (0x0004) //!< 支持联机PIN
#define BN_TTQ_SUP_SIGNATURE                        (0x0002) //!< 支持签名
#define BN_TTQ_REQ_ONLINEAC                         (0x0180) //!< 要求联机密文
#define BN_TTQ_REQ_CVM                              (0x0140) //!< 要求CVM
#define BN_TTQ_SUP_fDDA01                           (0x0380) //!< 终端支持"01"版本的fDDA，否则仅支持"00"版本的fDDA


#define BN_CTQ_REQ_ONLINE_PIN                       (0x0080) //!< 需要联机PIN
#define BN_CTQ_REQ_SIGNATURE                        (0x0040) //!< 需要签名
#define BN_CTQ_GO_ONLINE_IF_ODA_FAIL                (0x0020) //!< 如果脱机数据认证（ODA）失败，而且终端可联机则要求联机
#define BN_CTQ_TERMINATE_IF_ODA_FAIL_AND_SUP_PBOC   (0x0010) //!< 如果脱机数据认证（ODA）失败，而且终端支持PBOC则终止



#define __TVR

/**
 * TVR  : Terminal Verification Results -- Byte 1 (Leftmost)
 * Macro:
   OFFLINE_DA_NOT_PERFORMED                 : Offline data authentication was not performed
   SDA_FAILED                               : SDA failed
   ICC_DATA_MISSING                         : ICC data missing
   CARD_ON_EXCEPTION_FILE                   : Card appears on terminal exception file
   DDA_FAILED                               : DDA failed
   CDA_FAILED                               : CDA failed

 * EMV 4.2 Book 3...Annex C...C5
 */
#define OFFLINE_DA_NOT_PERFORMED                    0x0080
#define SDA_FAILED                                  0x0040
#define ICC_DATA_MISSING                            0x0020
#define CARD_ON_EXCEPTION_FILE                      0x0010
#define DDA_FAILED                                  0x0008
#define CDA_FAILED                                  0x0004
#define SDA_SELECTED                                0x0002

/**
 * TVR  : Terminal Verification Results -- Byte 2
 * Macro:
   APP_VER_NOT_MATCHED                      : ICC and terminal have different application versions
   EXPIRED_APP                              : Expired application
   APP_NOT_EFFECTIVE                        : Application not yet effective
   REQUESTED_SERVICE_NOT_ALLOWED            : Requested service not allowed for card product
   NEW_CARD                                 : New card

 * EMV 4.2 Book 3...Annex C...C5
 */
#define APP_VER_NOT_MATCHED                         0x0180
#define EXPIRED_APP                                 0x0140
#define APP_NOT_EFFECTIVE                           0x0120
#define REQUESTED_SERVICE_NOT_ALLOWED               0x0110
#define NEW_CARD                                    0x0108

/**
 * TVR  : Terminal Verification Results -- Byte 3
 * Macro:
   CV_NOT_SUCCESSFUL                        : Cardholder verification was not successful
   UNRECOGNISED_CVM                         : Unrecognised CVM
   PIN_TRY_LIMIT_EXCEEDED                   : PIN Try Limit exceeded
   PIN_PAD_NOT_PRESENT_OR_NOT_WORKING       : PIN entry required and PIN pad not present or not working
   PIN_PAD_PRESENT_BUT_PIN_NOT_ENTERED      : PIN entry required, PIN pad present, but PIN was not entered
   ONLINE_PIN_ENTERED                       : Online PIN entered

 * EMV 4.2 Book 3...Annex C...C5
 */
#define CV_NOT_SUCCESSFUL                           0x0280
#define UNRECOGNISED_CVM                            0x0240
#define PIN_TRY_LIMIT_EXCEEDED                      0x0220
#define PIN_PAD_NOT_PRESENT_OR_NOT_WORKING          0x0210
#define PIN_PAD_PRESENT_BUT_PIN_NOT_ENTERED         0x0208
#define ONLINE_PIN_ENTERED                          0x0204

/**
 * TVR  : Terminal Verification Results -- Byte 4
 * Macro:
   TRANSACTION_EXCEEDS_FLOOR_LIMIT          : Transaction exceeds floor limit
   LOWER_CONSECUTIVE_OFFLINE_LIMIT_EXCEEDED : Lower consecutive offline limit exceeded
   UPPER_CONSECUTIVE_OFFLINE_LIMIT_EXCEEDED : Upper consecutive offline limit exceeded
   TRANSACTION_SELECTED_RANDOMLY_FOR_ONLINE : Transaction selected randomly for online processing
   MERCHANT_FORCED_TRANSACTION_ONLINE       : Merchant forced transaction online

 * EMV 4.2 Book 3...Annex C...C5
 */
#define TRANSACTION_EXCEEDS_FLOOR_LIMIT             0x0380
#define LOWER_CONSECUTIVE_OFFLINE_LIMIT_EXCEEDED    0x0340
#define UPPER_CONSECUTIVE_OFFLINE_LIMIT_EXCEEDED    0x0320
#define TRANSACTION_SELECTED_RANDOMLY_FOR_ONLINE    0x0310
#define MERCHANT_FORCED_TRANSACTION_ONLINE          0x0308

/**
 * TVR  : Terminal Verification Results -- Byte 5 (Rightmost)
 * Macro:
   DEFAULT_TDOL_USED                        : Default TDOL used
   ISSUER_AUTHENTICATION_FAILED             : Issuer authentication failed
   SCRIPT_PROCESSING_FAILED_BEFORE_2GENAC   : Script processing failed before final GENERATE AC
   SCRIPT_PROCESSING_FAILED_AFTER_2GENAC    : Script processing failed after final GENERATE AC

 * EMV 4.2 Book 3...Annex C...C5
 */
#define DEFAULT_TDOL_USED                           0x0480
#define ISSUER_AUTHENTICATION_FAILED                0x0440
#define SCRIPT_PROCESSING_FAILED_BEFORE_2GENAC      0x0420
#define SCRIPT_PROCESSING_FAILED_AFTER_2GENAC       0x0410


#define __TSI

/**
 * TSI  : Transaction Status Information --  Byte 1 (Leftmost)
 * Macro:
   OFFLINE_DA_PERFORMED                     : Offline data authentication was performed
   CV_PERFORMED                             : Cardholder verification was performed
   CARD_RISK_MANA_PERFORMED                 : Card risk management was performed
   ISSUER_AUTH_PERFORMED                    : Issuer authentication was performed
   TERM_RISK_MANA_PERFORMED                 : Terminal risk management was performed
   SCRIPT_PROCESSING_PERFORMED              : Script processing was performed

 * EMV 4.2 Book 3...Annex C...C6
 */
#define OFFLINE_DA_PERFORMED                        0x0080
#define CV_PERFORMED                                0x0040
#define CARD_RISK_MANA_PERFORMED                    0x0020
#define ISSUER_AUTH_PERFORMED                       0x0010
#define TERM_RISK_MANA_PERFORMED                    0x0008
#define SCRIPT_PROCESSING_PERFORMED                 0x0004











#endif /* EMV_ICS_H_ */
/*
************************************************************************************************************************
*                                        COPYRIGHT (C) 2013 Kingsee Tech. Co., Ltd.               ~~~ END OF FILE ~~~
************************************************************************************************************************
*/
