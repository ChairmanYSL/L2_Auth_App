/**
************************************************************************************************************************
*                                        COPYRIGHT (C) 2014 Kingsee Tech. Co., Ltd.
*
* @file     emv_ics.h
* @brief   	EMV相关标签
* @author   Kingsee Development Team - chenjinhai@szzt.com.cn
* @version  V43D1.0.28
*
************************************************************************************************************************
*/
#ifndef EMV_TAG_H_
#define EMV_TAG_H_

/** [42] - Issuer Identification Number (IIN) | 发卡行识别码（IIN）, a6, 3 */
#define TAG_0042_IIN   0X0042

/** 应用标识符-卡片|Application Dedicated File (ADF) Name[4F][b][5-16] */
#define TAG_004F_AID   0X004F

/** 应用标签|Application Label[50][ans1-16][1-16] */
#define TAG_0050_APPLICATION_LABEL   0X0050

/** [57] - Track2 Equivalent Data | 二磁道等效数据, b, var. up to 19 */
#define TAG_0057_TRACK2_EQUIVALENT_DATA   0X0057

/** [5A] - Application PAN | 应用主账号（PAN）, cn var. up to 19, var. up to 10 */
#define TAG_005A_APPLICATION_PAN   0X005A

/** [61] - Application Template */
#define TAG_0061_APPLICATION_TEMPLATE   0X0061

/** [6F] - FCI (FILE CONTROL INFO) Template */
#define TAG_006F_FCI_TEMPLATE   0X006F

/** [70] - AEF (Application Elementary File) Data Template */
#define TAG_0070_AEF_DATA_TEMPLATE   0X0070

/** [71] - Issuer Script Template 1 */
#define TAG_0071_ISSUER_SCRIPT_TEMPLATE_1   0X0071

/** [72] - Issuer Script Template 2 */
#define TAG_0072_ISSUER_SCRIPT_TEMPLATE_2   0X0072

/** [73] - Directory Discretionary Template */
#define TAG_0073_DIRECTORY_DISCRETIONARY_TEMPLATE   0X0073

/** [77] - Response Message Template Format 2 */
#define TAG_0077_RESPONSE_MESSAGE_TEMPLATE_FORMAT_2   0X0077

/** [80] - Response Message Template Format 1 */
#define TAG_0080_RESPONSE_MESSAGE_TEMPLATE_FORMAT_1   0X0080

/** [81] - Amount Authorised Binary */
#define TAG_0081_AMOUNT_AUTHORISED_BINARY   0X0081

/** [82] - AIP (Application Interchange Profile) */
#define TAG_0082_AIP   0X0082

/** [83] - Command Template */
#define TAG_0083_COMMAND_TEMPLATE   0X0083

/** [84] - DF (Dedicated File) Name */
#define TAG_0084_DF_NAME   0X0084

/** [86] - Issuer Script Command */
#define TAG_0086_ISSUER_SCRIPT_COMMAND   0X0086

/** 应用优先指示器|Application Priority Indicator (API)[87][b8][1] */
#define TAG_0087_API   0X0087

/** [88] - SFI (Short File Indicator) */
#define TAG_0088_SFI   0X0088

/** [89] - Authorisation Code */
#define TAG_0089_AUTH_CODE   0X0089

/** [8A] - Authorisation Response Code (ARC) | 授权响应码, an2, 2 */
#define TAG_008A_ARC   0X008A

/** [8C] - CDOL 1 (Card Risk Management Data Object List) */
#define TAG_008C_CDOL_1   0X008C

/** [8D] - CDOL 2 (Card Risk Management Data Object List) */
#define TAG_008D_CDOL_2   0X008D

/** [8E] - CVM (Cardholder Verification Method) List */
#define TAG_008E_CVM_LIST   0X008E

/** [8F] - Certification Authority Public Key Index */
#define TAG_008F_CERTIFICATION_AUTHORITY_PUBLIC_KEY_INDEX   0X008F

/** [90] - Issuer Public Key Certificate */
#define TAG_0090_ISSUER_PUBLIC_KEY_CERTIFICATE   0X0090

/** [91] - Issuer Authentication Data */
#define TAG_0091_ISSUER_AUTHENTICATION_DATA   0X0091

/** [92] - Issuer Public Key Remainder */
#define TAG_0092_ISSUER_PUBLIC_KEY_REMAINDER   0X0092

/** [93] - Signed Static Application Data */
#define TAG_0093_SIGNED_STATIC_APPLICATION_DATA   0X0093

/** [94] - Application File Locator (AFL) | 应用文件定位器, var., var. up to 252 */
#define TAG_0094_AFL   0X0094

/** [95] - TVR (Terminal Verification Results) */
#define TAG_0095_TVR   0X0095

/** [97] - TDOL (Transaction Certificate Data Object List) */
#define TAG_0097_TDOL   0X0097

/** [98] - TC (Transaction Certificate) Hash Value */
#define TAG_0098_TC_HASH_VALUE   0X0098

/** [99] - Transaction PIN (Personal Identification Number) Data */
#define TAG_0099_TRANSACTION_PIN_DATA   0X0099

/** [9A] - Transaction Date */
#define TAG_009A_TRANSACTION_DATE   0X009A

/** [9B] - Transaction Status Info */
#define TAG_009B_TSI   0X009B

/** [9C] - Transaction Type */
#define TAG_009C_TRANSACTION_TYPE   0X009C

/** [9D] - DDF (Directory Definition File) Name */
#define TAG_009D_DDF_NAME   0X009D

/** [A5] - FCI (File Control Info) Proprietary Template */
#define TAG_00A5_FCI_PROPRIETARY_TEMPLATE   0X00A5

/** [5F20] - Cardholder Name */
#define TAG_5F20_CARDHOLDER_NAME   0X5F20

/** [5F24] - Application Expiration Date */
#define TAG_5F24_APPLICATION_EXPIRATION_DATE   0X5F24

/** [5F25] - Application Effective Date */
#define TAG_5F25_APPLICATION_EFFECTIVE_DATE   0X5F25

/** [5F28] - Issuer Country Code */
#define TAG_5F28_ISSUER_COUNTRY_CODE   0X5F28

/** [5F2A] - Transaction Currency Code | 交易货币代码, n2, 1 */
#define TAG_5F2A_TRANSACTION_CURRENCY_CODE   0X5F2A

/** [5F2D] - Language Preference */
#define TAG_5F2D_LANGUAGE_PREFERENCE   0X5F2D

/** [5F30] - Service Code */
#define TAG_5F30_SERVICE_CODE   0X5F30

/** [5F34] - Application PAN Sequence | 应用主账号序列号, n2, 1 */
#define TAG_5F34_APPLICATION_PAN_SEQUENCE   0X5F34

/** [5F36] - Transaction Currency Exponent */
#define TAG_5F36_TRANSACTION_CURRENCY_EXPONENT   0X5F36

/** [5F50] - Issuer URL | 发卡行URL\n ans, var. */
#define TAG_5F50_ISSUER_URL   0X5F50

/** [5F57] - Account Type | 帐户类型\n n2, 1 */
#define TAG_5F57_ACCOUNT_TYPE   0X5F57

/** [9F01] - Acquirer Identifier */
#define TAG_9F01_ACQUIRER_IDENTIFIER   0X9F01

/** [9F02] - Amount Authorised Numeric */
#define TAG_9F02_AMOUNT_AUTHORISED_NUMERIC   0X9F02

/** [9F03] - Amount Other Numeric */
#define TAG_9F03_AMOUNT_OTHER_NUMERIC   0X9F03

/** [9F04] - Amount Other Binary */
#define TAG_9F04_AMOUNT_OTHER_BINARY   0X9F04

/** [9F05] - Application Discretionary Data */
#define TAG_9F05_APPLICATION_DISCRETIONARY_DATA   0X9F05

/** [9F06] - AID (Terminal) */
#define TAG_9F06_AID_TERMINAL   0X9F06

/** [9F07] - AUC (Application Usage Control) */
#define TAG_9F07_AUC   0X9F07

/** [9F08] - Application Version Number Card */
#define TAG_9F08_APPLICATION_VERSION_NUMBER_CARD   0X9F08

/** [9F09] - Application Version Number Terminal */
#define TAG_9F09_APPLICATION_VERSION_NUMBER_TERMINAL   0X9F09

/** [9F0B] - Cardholder Name Extended */
#define TAG_9F0B_CARDHOLDER_NAME_EXTENDED   0X9F0B

/** [9F0D] - Issuer Action Code Default */
#define TAG_9F0D_ISSUER_ACTION_CODE_DEFAULT   0X9F0D

/** [9F0E] - Issuer Action Code Denial */
#define TAG_9F0E_ISSUER_ACTION_CODE_DENIAL   0X9F0E

/** [9F0F] - Issuer Action Code Online */
#define TAG_9F0F_ISSUER_ACTION_CODE_ONLINE   0X9F0F

/** [9F10] - Issuer Application Data (IAD) | 发卡行应用数据, b, var. up to 32 */
#define TAG_9F10_IAD   0X9F10

/** [9F11] - Issuer Code Table Index */
#define TAG_9F11_ISSUER_CODE_TABLE_INDEX   0X9F11

/** 应用首选名称|Application Preferred Name[9F12][ans1-16][1-16] */
#define TAG_9F12_APPLICATION_PREFERRED_NAME   0X9F12

/** [9F13] - Last Online ATC (Application Transaction Counter) Register */
#define TAG_9F13_LAST_ONLINE_ATC_REGISTER   0X9F13

/** [9F14] - Lower Consecutive Offline Limit */
#define TAG_9F14_LOWER_CONSECUTIVE_OFFLINE_LIMIT   0X9F14

/** [9F15] - Merchant Category Code */
#define TAG_9F15_MERCHANT_CATEGORY_CODE   0X9F15

/** [9F16] - Merchant Identifier */
#define TAG_9F16_MERCHANT_IDENTIFIER   0X9F16

/** [9F17] - PIN (Personal Identification Number) Try Counter */
#define TAG_9F17_PIN_TRY_COUNTER   0X9F17

/** [9F18] - Issuer Script Identifier */
#define TAG_9F18_ISSUER_SCRIPT_IDENTIFIER   0X9F18

/** [9F1A] - Terminal Country Code */
#define TAG_9F1A_TERMINAL_COUNTRY_CODE   0X9F1A

/** [9F1B] - 终端最低限额, b, 4 */
#define TAG_9F1B_TERMINAL_FLOOR_LIMIT   0X9F1B

/** [9F1C] - Terminal Identification */
#define TAG_9F1C_TERMINAL_IDENTIFICATION   0X9F1C

/** [9F1D] - Terminal Risk Mangement Data */
#define TAG_9F1D_TERMINAL_RISK_MANGEMENT_DATA   0X9F1D

/** [9F1E] - IFD (Interface Device) Serial Number */
#define TAG_9F1E_IFD_SERIAL_NUMBER   0X9F1E

/** [9F1F] - Track1 Discretionary Data */
#define TAG_9F1F_TRACK1_DISCRETIONARY_DATA   0X9F1F

/** [9F20] - Track2 Discretionary Data */
#define TAG_9F20_TRACK2_DISCRETIONARY_DATA   0X9F20

/** [9F21] - Transaction Time */
#define TAG_9F21_TRANSACTION_TIME   0X9F21

/** [9F22] - 认证中心公钥索引 */
#define TAG_9F22_CAPK_INDEX   0X9F22

/** [9F23] - Upper Consecutive Offline Limit */
#define TAG_9F23_UPPER_CONSECUTIVE_OFFLINE_LIMIT   0X9F23

/** [9F26] - Application Cryptogram */
#define TAG_9F26_APPLICATION_CRYPTOGRAM   0X9F26

/** [9F27] - Cryptogram Info Data */
#define TAG_9F27_CRYPTOGRAM_INFO_DATA   0X9F27

/** [9F2D] - Icc PIN Encipherment Public Key Certificate */
#define TAG_9F2D_ICC_PIN_ENCIPHERMENT_PUBLIC_KEY_CERTIFICATE   0X9F2D

/** [9F2E] - Icc PIN Encipherment Public Key Exponent */
#define TAG_9F2E_ICC_PIN_ENCIPHERMENT_PUBLIC_KEY_EXPONENT   0X9F2E

/** [9F2F] - Icc PIN Encipherment Public Key Remainder */
#define TAG_9F2F_ICC_PIN_ENCIPHERMENT_PUBLIC_KEY_REMAINDER   0X9F2F

/** [9F32] - Issuer Public Key Exponent */
#define TAG_9F32_ISSUER_PUBLIC_KEY_EXPONENT   0X9F32

/** [9F33] - Terminal Capabilities */
#define TAG_9F33_TERMINAL_CAPABILITIES   0X9F33

/** [9F34] - CVM (Cardholder Verification Method) Results */
#define TAG_9F34_CVM_RESULTS   0X9F34

/** [9F35] - Terminal Type */
#define TAG_9F35_TERMINAL_TYPE   0X9F35

/** [9F36] - ATC (Application Transaction Counter) */
#define TAG_9F36_ATC   0X9F36

/** [9F37] - Unpredicatable Number */
#define TAG_9F37_UNPREDICATABLE_NUMBER   0X9F37

/** [9F38] - PDOL (Processing Options Data Object List) */
#define TAG_9F38_PDOL   0X9F38

/** [9F39] - PoS (Point of Service) Entry Mode */
#define TAG_9F39_POS_ENTRY_MODE   0X9F39

/** [9F3A] - Amount Reference Currency */
#define TAG_9F3A_AMOUNT_REFERENCE_CURRENCY   0X9F3A

/** [9F3B] - Application Reference Currency */
#define TAG_9F3B_APPLICATION_REFERENCE_CURRENCY   0X9F3B

/** [9F3C] - Transaction Reference Currency */
#define TAG_9F3C_TRANSACTION_REFERENCE_CURRENCY   0X9F3C

/** [9F3D] - Transaction Reference Currency Exponent */
#define TAG_9F3D_TRANSACTION_REFERENCE_CURRENCY_EXPONENT   0X9F3D

/** [9F40] - Additional Terminal Capabilities */
#define TAG_9F40_ADDITIONAL_TERMINAL_CAPABILITIES   0X9F40

/** [9F41] - Transaction Sequence Counter */
#define TAG_9F41_TRANSACTION_SEQUENCE_COUNTER   0X9F41

/** [9F42] - Application Currency Code */
#define TAG_9F42_APPLICATION_CURRENCY_CODE   0X9F42

/** [9F43] - Application Reference Currency Exponent */
#define TAG_9F43_APPLICATION_REFERENCE_CURRENCY_EXPONENT   0X9F43

/** [9F44] - Application Currency Exponent */
#define TAG_9F44_APPLICATION_CURRENCY_EXPONENT   0X9F44

/** [9F45] - Data Authentication Code */
#define TAG_9F45_DATA_AUTHENTICATION_CODE   0X9F45

/** [9F46] - ICC Public Key Certificate */
#define TAG_9F46_ICC_PUBLIC_KEY_CERTIFICATE   0X9F46

/** [9F47] - ICC Public Key Exponent */
#define TAG_9F47_ICC_PUBLIC_KEY_EXPONENT   0X9F47

/** [9F48] - ICC Public Key Remainder */
#define TAG_9F48_ICC_PUBLIC_KEY_REMAINDER   0X9F48

/** [9F49] - DDOL (Dynamic Data Object List) */
#define TAG_9F49_DDOL   0X9F49

/** [9F4A] - SDA (Static Data Authentication) Tag List */
#define TAG_9F4A_SDA_TAG_LIST   0X9F4A

/** [9F4B] - Signed Dynamic Application Data */
#define TAG_9F4B_SIGNED_DYNAMIC_APPLICATION_DATA   0X9F4B

/** [9F4C] - ICC Dynamic Number */
#define TAG_9F4C_ICC_DYNAMIC_NUMBER   0X9F4C

/** [9F4D] - 交易日志入口 */
#define TAG_9F4D_LOG_SFI   0X9F4D

/** [9F4E] - Merchant Name and Location */
#define TAG_9F4E_MERCHANT_NAME   0X9F4E

/** [9F4F] - 交易日志格式 */
#define TAG_9F4F_LOG_FORMAT 0X9F4F

/** [9F5D] - Application Currency Code | 应用货币代码, n4, 2 */
#define TAG_9F51_APPLICATION_CURRENCY_CODE   0X9F51

/** [9F5D] - Available Offline Spending Amount (AOSA) | 可用脱机消费金额, n12, 6 */
#define TAG_9F5D_AVAILABLE_OFFLINE_SPENDING_AMOUNT   0X9F5D

/** [9F61] - 持卡人证件号, an40, 1-40 */
#define TAG_9F61_CARDHOLDER_DOCUMENTS_NUMBER   0X9F61

/** [9F62] - 持卡人证件类型, 定义可参考{@link PBOC_DocumentTypes}, cn1, 1 */
#define TAG_9F62_CARDHOLDER_DOCUMENTS_TYPE   0X9F62

/** [9F63] - 产品标识信息, b, 16 */
#define TAG_9F63_PRODUCT_IDENTIFICATION_INFORMATION   0X9F63

/** [9F66] - Terminal Transaction Qualifiers (TTQ) | 终端交易属性 */
#define TAG_9F66_TERMINAL_TRANSACTION_QUALIFIERS   0X9F66

/** [9F69] - Card Authentication Related Data | 卡片认证相关数据  */
#define TAG_9F69_CARD_AUTHENTICATION_RELATED_DATA   0X9F69

/** [9F6C] - Card Transaction Qualifiers (CTQ) | 卡片交易属性 */
#define TAG_9F6C_CARD_TRANSACTION_QUALIFIERS   0X9F6C

/** [9F6D] - EC Reset Threshold | 电子现金重置阈值, n12, 6 */
#define TAG_9F6D_EC_RESET_THRESHOLD   0X9F6D

/** [9F74] - EC Issuer Authorization Code | 电子现金发卡行授权码, a, 6 */
#define TAG_9F74_EC_ISSUER_AUTHORIZATION_CODE   0X9F74

/** [9F77] - EC Balance Limit | 电子现金余额上限, n12, 6 */
#define TAG_9F77_EC_BALANCE_LIMIT   0X9F77

/** [9F78] - EC Single Transaction Limit | 电子现金单笔交易限额, n12, 6 */
#define TAG_9F78_EC_SINGLE_TRANSACTION_LIMIT   0X9F78

/** [9F79] - EC Balance | 电子现金余额, n12, 6 */
#define TAG_9F79_EC_BALANCE   0X9F79

/** [9F7A] - EC Terminal Support Indicator | 电子现金终端支持指示器, b, 1 */
#define TAG_9F7A_EC_TERMINAL_SUPPORT_INDICATOR   0X9F7A

/** [9F7B] - EC Terminal Transaction Limit | 电子现金终端交易限额, n12, 6 */
#define TAG_9F7B_EC_TERMINAL_TRANSACTION_LIMIT   0X9F7B

/** [BF0C] - FCI (File Control Info) Issuer Discretionary Data */
#define TAG_BF0C_FCI_ISSUER_DISCRETIONARY_DATA   0XBF0C

/** [DF01] - 应用选择指示器（ASI）, b, 1 */
#define TAG_DF01_ASI   0XDF01

/** [DF02] - 认证中心公钥模, b, 248 */
#define TAG_DF02_CAPK_MODULUS   0XDF02

/** [DF03] - 认证中心公钥校验值, b, ??? */
#define TAG_DF03_CAPK_CHECKSUM   0XDF03

/** [DF04] - 认证中心公钥指数, b, 1 or 3 */
#define TAG_DF04_CAPK_EXPONENT   0XDF04

/** [DF05] - 认证中心公钥有效期, n8, 8 */
#define TAG_DF05_CAPK_EXPIRATION_DATE   0XDF05

/** [DF06] - 认证中心公钥哈希算法标识, b, 1 */
#define TAG_DF06_CAPK_HASH_ALGORITHM_INDICATOR   0XDF06

/** [DF07] - 认证中心公钥算法标识, b, 1 */
#define TAG_DF07_CAPK_ALGORITHM_INDICATOR   0XDF07

/** [DF11] - 终端行为代码-缺省, b, 5 */
#define TAG_DF11_TAC_DEFAULT   0XDF11

/** [DF12] - 终端行为代码-联机, b, 5 */
#define TAG_DF12_TAC_ONLINE   0XDF12

/** [DF13] - 终端行为代码-拒绝, b, 5 */
#define TAG_DF13_TAC_DENIAL   0XDF13

/** [DF14] - 缺省DDOL */
#define TAG_DF14_DEFAULT_DDOL   0XDF14

/** [DF15] - 偏置随机选择阀值, b, 4 */
#define TAG_DF15_THRESHOLD_VALUE   0XDF15

/** [DF16] - 偏置随机选择最大目标百分数, b, 1 FXIME */
#define TAG_DF16_MAX_TARGET_PERCENTAGE   0XDF16

/** [DF17] - 随机选择目标百分数, b, 1 FXIME */
#define TAG_DF17_TARGET_PERCENTAGE   0XDF17

/** [DF18] - 终端联机PIN支持能力, b, 1 */
#define TAG_DF18_ONLINE_PIN_SUPPORT_INDICATOR   0XDF18

/** [DF19] - 非接触脱机最低限额, cn, 6 */
#define TAG_DF19_CONTACTLESS_FLOOR_LIMIT   0XDF19

/** [DF20] - 非接触交易限额, cn, 6 */
#define TAG_DF20_CONTACTLESS_TRANSACTION_LIMIT   0XDF20

/** [DF21] - 持卡人验证方法（CVM）所需限额, cn, 6 */
#define TAG_DF21_CVM_REQUIRED_LIMIT   0XDF21

/** [DF31] - 发卡行脚本处理结果 */
#define TAG_DF31_ISSUER_SCRIPT_RESULTS   0XDF31

/** [DF4D] - 圈存日志入口, b16, 2 */
#define TAG_DF4D_LOAD_LOG_SFI   0XDF4D

/** [DF4D] - 圈存日志格式, b, var. */
#define TAG_DF4F_LOAD_LOG_FORMAT   0XDF4F

/** [DF69] - SM算法支持指示器, b8, 1 */
#define TAG_DF69_SM_SUPPORT_INDICATOR   0XDF69


/**
 * Self-defined tags for internal usage.
 * These tag names are just defined according to
 * current hash value spread. So if the hash
 * function changes in emvbuf.c leading to
 * a different spread, these tag names may
 * be changed too.
 * Always use these macros instead of
 * any real integer.
 */
#define TAG_PDOL_VALUE          0XB6
#define TAG_CDOL1_VALUE         0xB7
#define TAG_CDOL2_VALUE         0xB8
#define TAG_AUTH_DATA           0xB9
#define TAG_ISSUER_PK           0xBA
#define TAG_ICCARD_PK           0xBB
#define TAG_ONLINE_PIN          0xBD

/** Cumulative Total Transaction Amount | 累计脱机交易金额 */
#define TAG_CUMULATIVE_TOTAL_TRANSACTION_AMOUNT   0XC5
#define TAG_CTTA   TAG_CUMULATIVE_TOTAL_TRANSACTION_AMOUNT

/** Cumulative Total Transaction Amount Limit | 累计脱机交易金额限制数 */
#define TAG_CUMULATIVE_TOTAL_TRANSACTION_AMOUNT_LIMI   0XC6
#define TAG_CTTAL   TAG_CUMULATIVE_TOTAL_TRANSACTION_AMOUNT_LIMI

#define TAG_ISSUER_SCRIPT_71_BASE   0XDF8100
#define TAG_ISSUER_SCRIPT_72_BASE   0XDF8200



#define TAG_TRANSACTION_REFERENCE_CURRENCY_CONVERSION   0XDF8307





/** 通知指示器 */
#define TAG_ADVICE_INDICATOR   0XDF8405

/** 签名指示器 */
#define TAG_SIGNATURE_INDICATOR   0XDF8406


/** 一致性声明 */
#define TAG_ICS   0X1F01
/** 状态检查支持指示器 */
#define TAG_STATUS_CHECK_SUPPORT_INDICATOR   0X1F02
/** 默认TDOL */
#define TAG_DEFAULT_TDOL   0X1F03


//////////////////////////////////////////////////////
//0XDF8F00 ~ 0XDF8F7F : 非应用数据，内核专用
//////////////////////////////////////////////////////

/** 内核专用数据基值（非应用数据，内核专用） */
#define TAG_KERNEL_BASE   0XDF8800

/** 内核版本号（非应用数据，内核专用） */
#define TAG_KERNEL_VERSION   0XDF8F01

/** 内核ChkSum（非应用数据，内核专用） */
#define TAG_KERNEL_CHECKSUM   0XDF8F02

/** 配置ChkSum，0表示默认配置（非应用数据，内核专用） */
#define TAG_CONFIG_CHECKSUM   0xDF8F03

/** 应用候选列表（非应用数据，内核专用） */
#define TAG_CANDIDATE_LIST   0XDF8F04

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////


#endif /* EMV_TAG_H_ */
/*
************************************************************************************************************************
*                                        COPYRIGHT (C) 2014 Kingsee Tech. Co., Ltd.               ~~~ END OF FILE ~~~
************************************************************************************************************************
*/
