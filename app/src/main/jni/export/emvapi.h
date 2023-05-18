/**
************************************************************************************************************************
*                                     COPYRIGHT (C) 2015 Kingsee Tech. Co., Ltd.
*
* @file     emvapi.h
* @brief    EMV应用接口
* @author   Kingsee Development Team - chenjinhai@szzt.com.cn
* @version  V0.4.7
*
************************************************************************************************************************
*/
#ifndef EMVAPI_H_
#define EMVAPI_H_


/*
************************************************************************************************************************
* INCLUDE FILES
************************************************************************************************************************
*/

#include "emv_type.h"


/*
************************************************************************************************************************
* EXTERNS
************************************************************************************************************************
*/

#if defined(__cplusplus)
extern "C" {
#endif


/*
************************************************************************************************************************
* DEFINES
************************************************************************************************************************
*/

/**
 * EMV交易执行状态
 */
enum emv_process_status
{
    STATUS_COMPLETION = 2,              //!< 执行完成，信息描述见{@link emv_completion_info}
    STATUS_CONTINUE = 1,                //!< 执行未完，信息描述见{@link emv_continue_info}
    STATUS_ERROR = 0,                   //!< 执行报错，信息描述见{@link emv_error_info}
};

/**
 * EMV交易执行完成返回信息描述
 */
enum emv_completion_info
{
    APPROVE_OFFLINE = 1,                //!< 脱机接受
    APPROVE_ONLINE = 2,                 //!< 联机接受
    DECLINE_OFFLINE = 3,                //!< 脱机拒绝
    DECLINE_ONLINE = 4,                 //!< 联机拒绝
    FALLBACK = 5,                       //!< 降级处理
};

/**
 * EMV交易执行未完成返回信息描述
 */
enum emv_continue_info
{
    EMV_CANDIDATE_LIST = 1,             //!< notify Application show Application Candidate List
    EMV_APP_SELECTED = 2,               //!< Application Select Completed
    EMV_GET_PROC_OPTION = 3,            //!< Get Process Option Completed
    EMV_READ_APP_DATA = 4,              //!< Read Application Data Completed
    EMV_DATA_AUTH = 5,                  //!< Data Authentication Completed
    EMV_PROCESS_RESTRICT = 6,           //!< Process Restrict Completed
    EMV_ONLINE_ENC_PIN = 7,             //!< notify Application prompt Caldholder enter Online PIN
    EMV_PIN_BYPASS_CONFIRM = 8,         //!< notify  Application  confirm  to Accepted PIN Bypass or not
    EMV_CARDHOLDER_VERIFY = 9,          //!< Cardholder Verify Completed
    EMV_TERMINAL_RISK_MANAGEMENT = 10,  //!< Terminal Risk Management Completed
    EMV_PROCESS_ONLINE = 11,            //!< notify Application to Process Online
    EMV_ID_CHECK = 12,                  //!< notify  Application  Check  Cardholder's Identification

    /*
    STA_IDLE = 0,                       //!< 空闲状态，可以执行交易流程
    STA_CANDIDATES_BUILT,               //!< 候选列表创建完成
    REQ_SEL_CANDIDATES,                 //!< 请求选择候选应用列表
    REQ_SEL_CANDIDATES_AGAIN,           //!< 请求再次选择候选应用列表
    STA_APP_SELECTED,                   //!< 应用选择完成
    REQ_SEL_ACCOUNTTYPE,                //!< 请求选择帐户类型
    STA_APP_INITIALIZED,                //!< 应用初始化完成
    REQ_DEACTIVATE_CARD,                //!< 请求移走非接触式卡片（QPBOC专用）
    STA_CARD_DEACTIVATED,               //!< 非接触式卡片已移除（QPBOC专用）
    STA_READ_APP_DATA_COMPLETED,        //!< 读应用数据完成
    STA_DATA_AUTH_COMPLETED,            //!< 脱机数据认证完成
    STA_PROCESS_RESTRICT_COMPLETED,     //!< 处理限制完成
    REQ_CHECK_ID,                       //!< 请求持卡人出示证件
    REQ_ONLINE_PIN,                     //!< 请求持卡人输入联机PIN
    REQ_OFFLINE_PIN,                    //!< 请求持卡人输入脱机PIN
    REQ_OFFLINE_PIN_LAST,               //!< 请求持卡人输入脱机PIN（最后一次）
    REQ_CONFIRM_BYPASS_PIN,             //!< 请求确认是否跳过PIN输入
    STA_CARDHOLDER_VERIFY_COMPLETED,    //!< 持卡人验证完成
    REQ_CONFIRM_FORCE_ONLINE,           //!< 请求确认是否强制联机
    STA_RISK_MANAGEMENT_COMPLETED,      //!< 终端风险管理完成
    STA_ACTION_ANALYSE_COMPLETED,       //!< 终端行为分析完成
    REQ_GO_ONLINE,                      //!< 请求联机
    REQ_ISSUER_REFERRAL,                //!< 请求发卡行参考
*/
};

/**
 * EMV交易执行错误返回信息描述
 */
enum emv_error_info
{
    SUCCESS = 0,                        //!< no more information
    ERROR_APP_NO_INFO = 1,              //!< Selected Application do not in the Candidate List when Application Select
    ERROR_APP_ANALYSIS = 3,             //!< Parse Card Returned Data Error when Application Select
    ERROR_APP_BLOCKED = 4,              //!< card  return  6A81  when Application Select
    ERROR_APP_SELECT = 5,               //!< Error when Application Select
    ERROR_NO_AIPAFL = 6,                //!< Application Interchange Profile(AIP) and Application  File  Locator(AFL)  not  exist  when  Initialize Application
    ERROR_INIT_APP = 7,                 //!< Error when ApplicationInit
    ERROR_OTHER_CARD = 8,               //!< Error when other card insert used
    ERROR_EXPIRED_CARD = 9,             //!< Error when Initialize Application Data
    ERROR_APP_DATA = 10,                //!< Error when Read Application Data
    ERROR_AUTH_METHOD_BLOCKED = 11,     //!< card return 6983, command not allowed, authentication method blocked
    ERROR_REFDATA_INVALIDATED = 12,     //!< card return 6984, command not allowed, referenced data invalidated
    ERROR_COND_NOT_SATISFIED = 13,      //!< card return 6985, command not allowed, conditions of use not satisfied
    ERROR_FUNC_NOT_SUPPORTED = 14,      //!< card return 6A81, wrong parameter p1 p2, function not supported
    ERROR_FILE_NOT_FOUND = 15,          //!< card return 6A82, wrong parameter p1 p2, file not found
    ERROR_RECORD_NOT_FOUND = 16,        //!< card return 6A83, wrong parameter p1 p2, record not found
    ERROR_REFDATA_NOT_FOUND = 17,       //!< card return 6A88, referenced data (data objects) not found
    ERROR_SELFILE_INVALIDATED = 18,     //!< card return 6283, state  of non-volatile memory unchanged, selected file invalidated
    ERROR_AUTH_FAILED = 19,             //!< card return 6300, state of non-volatile memory changed, authentication failed
    ERROR_COUNTER_X = 20,               //!< card return 63Cx, state of non-volatile memory changed, counter provided by 'x'(from 0-15)
    ERROR_BLOCKED_NOSEL = 21,           //!< card blocked or select command not supported
    ERROR_ANALYSIS = 22,                //!< Parse Card Returned Data Error
    ERROR_READ_DATA = 23,               //!< Error when Processing Restrict
    ERROR_GEN_RANDOM = 24,              //!< Card Returned Data for SDA Overflow when Read Application Data
    ERROR_GEN_DOLBLOCK = 25,            //!< Generate DOL Block error when Data Authentication
    ERROR_GEN_AC = 26,                  //!< Generate AC error when Transaction Process
    ERROR_NO_CDOL1 = 27,                //!< CDOL1 not exist when Transaction Process
    ERROR_NO_CDOL2 = 28,                //!< CDOL2 not exist when Transaction Process
    ERROR_LOGIC = 29,                   //!< Logic Error when Transaction Process
    ERROR_CHIP_CANNOT_BE_READ = 30,     //!< Card Returned Unknown Response Code
    ERROR_PROCESS_CMD = 31,             //!< Process Command ERROR
    ERROR_AAR_ABORTED = 32,             //!< Card decision is AAR when Transaction Process
    ERROR_LOG_FILE = 33,                //!< Log File Error
    ERROR_SERVICE_NOT_ALLOWED = 34,     //!< Service not Allowed
    ERROR_PINENTERY_TIMEOUT = 35,       //!< PIN Entry timeout
    ERROR_OFFLINE_VERIFY = 36,          //!< Check Offline PIN Error when Cardholder Verify
    ERROR_NEED_ADVICE = 37,             //!< Trans need Advice
    ERROR_USER_CANCELLED = 38,          //!< Communication Error with Host, but the card need advice, halted the transaction
};


/*
************************************************************************************************************************
* ERROR CODES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
* DATA TYPES
************************************************************************************************************************
*/

typedef struct
{
    void (*pEMVProcessCallback)(u8* data);
    void (*pCardEventOccured)(int type);
} EMV_INIT_PARAM;


/*
************************************************************************************************************************
* GLOBAL VARIABLES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
* MACRO'S
************************************************************************************************************************
*/


/*
************************************************************************************************************************
* FUNCTION PROTOTYPES
************************************************************************************************************************
*/


/**
 * 分配全局缓存，用于存放应用数据（建议开7168字节以上）
 *
 * @param   size                                [OU]分配的全局缓存大小
 * @return  全局缓存的地址
 *
 * @warning 全局缓存在交易流程中不允许复用\n
 *          全局缓存如果被其它地方复用，再次使用EMV前必须重新初始化
 *
 * @note    库里面开的全局缓存空间是9216字节，应用可以重新实现该函数来增减空间
 */
u8* emv_alloc_buffer(int* size);


#define C_O_M_M__F_U_N_C__H


/**
 * 读取指定标签的数据和长度
 *
 * @param   tag                             [IN]标签值，例如: 0x9F02
 * @param   value                           [OU]标签对应的数据缓存
 * @param   length                          [OU]标签对应的数据长度
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_get_tag_data(u32 tag, u8* value, int* length);

/**
 * 获取EMV内核版本信息
 *
 * @param   ver                             [OU]版本信息字符串缓存
 * @return  {@link EMV_OK}                  成功
 */
int emv_get_version(u8* ver);

/**
 * 获取候选应用列表
 *
 * @param   list                            [OU]候选应用列表，格式为{@link EMV_CANDIDATE}
 * @return  候选应用个数
 */
int emv_get_candidate_list(void* list);


#define P_R_O_C__F_U_N_C__H


/**
 * 功能模块初始化
 *
 * @param   par                             [IN]初始化参数，详见{@link EMV_INIT_PARAM}
 */
void emv_initialize(EMV_INIT_PARAM* par);

/**
 * 设置交易类型
 *
 * @param   type                            [IN]交易类型，详见{@link EMV_TransTypes}
 */
void emv_set_trans_type(u8 type);

/**
 * 设置交易金额
 *
 * @param   amount                          [IN]交易金额（单位：分）
 */
void emv_set_trans_amount(u32 amount);

/**
 * 设置其他金额
 *
 * @param   amount                          [IN]其它金额（单位：分）
 */
void emv_set_other_amount(u32 amount);

/**
 * 交易预处理（非接专用）
 *
 * @param   online                          [IN]标记是否联机优先
 * @return  {@link EMV_OK}                  成功，允许非接触交易路径
 * @return  {@link EMV_FAIL}                失败，不允许非接触交易路径
 */
int emv_preprocess(u8 online);

/**
 * 设置卡片类型
 *
 * @param   type                            [IN]卡片类型（1-接触式，2-非接触式）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_set_cardtype(u8 type);

/**
 * 交易流程
 *
 * @return  [-1000 ~ -9999]                 错误状态返回值，具体参考{@link EMV_Status}
 * @return  [    1 ~    99]                 中间状态返回值，具体参考{@link EMV_Status}
 * @return  [  100 ~   199]                 结束状态返回值，具体参考{@link EMV_Status}
 */
int emv_process(void);

/**
 * 设置选择候选应用列表的结果
 *
 * @param   eResult                         [IN]选中应用对应候选应用列表的索引（0开始）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_SELCANDIDATENOTREQ}  内核未请求选择候选应用列表
 */
int emv_set_candidate_list_result(int index);

/**
 * 设置选择帐户类型的结果
 *
 * @param   result                          [IN]选中的帐户类型，参考{@link EMV_AccountTypes}
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_SELACCOUNTNOTREQ}    内核未请求选择帐户类型
 */
int emv_set_account_type_selected(int result);

/**
 * 设置查验持卡人证件的结果
 *
 * 根据IDType（9F62）和IDNumber（9F61），设置持卡人证件检查结果
 *
 * @param   result                          [IN]查验结果（0-不通过，1-通过）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_CHECKIDNOTREQ}       内核未请求查验持卡人证件
 */
int emv_set_check_id_result(int result);

/**
 * 设置联机PIN输入结果
 *
 * @param   result                          [IN]输入结果，参考{@link EMV_OperResults}
 *   @arg   {@link EMV_OPER_OK}             正常输入密码
 *   @arg   {@link EMV_OPER_BYPASS}         无密码（不输入密码直接按确认键跳过输入）
 *   @arg   {@link EMV_OPER_UNSUPPORTED}    不支持或工作异常
 *   @arg   {@link EMV_OPER_TIMEOUT}        超时退出
 *   @arg   {@link EMV_OPER_CANCEL}         按键取消
 *   @arg   {@link EMV_OPER_QUIT}           按键退出
 * @param   pinbuf                          [IN]联机PIN密文数据（为NULL表示不需要内核保存）
 * @param   pinlen                          [IN]联机PIN密文数据长度（最大支持256字节）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_ONLINEPINNOTREQ}     内核未请求输入联机PIN
 *
 * @note    pData和nDatalen只有在输入结果eResult为EMV_OPER_OK时有效，
 *          格式由应用自定义，内核只负责保存，保存的标签为{@link TAG_ONLINE_PIN}。
 */
int emv_set_online_pin_entered(int result, u8* pinbuf, int pinlen);

/**
 * 设置脱机PIN输入结果
 *
 * @param   result                          [IN]输入结果，参考{@link EMV_OperResults}
 *   @arg   {@link EMV_OPER_OK}             正常输入密码
 *   @arg   {@link EMV_OPER_BYPASS}         无密码（不输入密码直接按确认键跳过输入）
 *   @arg   {@link EMV_OPER_UNSUPPORTED}    不支持或工作异常
 *   @arg   {@link EMV_OPER_TIMEOUT}        超时退出
 *   @arg   {@link EMV_OPER_CANCEL}         按键取消
 *   @arg   {@link EMV_OPER_QUIT}           按键退出
 * @param   pinbuf                          [IN]脱机PIN原始数据（不支持NULL，格式为ASC）
 * @param   pinlen                          [IN]脱机PIN原始数据的长度（支持4 ~ 12个字符）
 * @return  {@link EMV_OK}                  设置成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_OFFLINEPINNOTREQ}    内核未请求输入脱机PIN
 *
 * @note    pData和nDatalen只有在输入结果eResult为EMV_OPER_OK时有效，
 *          格式必须是PIN的明文字符串，例如：密码="123456"，那么pData="123456"，nDataLen=6。
 */
int emv_set_offline_pin_entered(int result, u8* pinbuf, int pinlen);

/**
 * 设置持卡人确认是否跳过PIN输入的结果
 *
 * @param   result                          [IN]跳过标志（0-不跳过，1-跳过）
 * @return  {@link EMV_OK}                  设置成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_BYPASSPINNOTREQ}     内核未请求确认是否跳过PIN输入
 */
int emv_set_pin_bypass_confirmed(int result);

/**
 * 设置商户确认是否强制联机的结果
 *
 * @param   result                          [IN]强制联机标志（0-不强制联机，1-强制联机）
 * @return  {@link EMV_OK}                  设置成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_FORCEONLINENOTREQ}   内核未请求确认是否强制联机
 */
int emv_set_force_online(int result);

/**
 * 设置联机授权结果
 *
 * @param   result                          [IN]联机授权结果，参考{@link EMV_OnlineResults}
 *   @arg   {@link EMV_ONLINE_NOT}          未联机
 *   @arg   {@link EMV_ONLINE_FAIL}         联机失败
 *   @arg   {@link EMV_ONLINE_SUCC_ACCEPT}  联机成功，发卡行批准
 *   @arg   {@link EMV_ONLINE_SUCC_DENIAL}  联机成功，发卡行拒绝
 *   @arg   {@link EMV_ONLINE_SUCC_ISSREF}  联机成功，发卡行指引
 * @param   resp                            [IN]授权响应数据（相当于55域数据，TLV格式，需包含授权响应码）
 * @param   resplen                         [IN]授权响应数据长度
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_GOONLINENOTREQ}      内核未请求联机授权
 */
int emv_set_online_result(int result, u8* resp, int resplen);

/**
 * 设置发卡行参考结果
 *
 * @param   result                          [IN]发卡行参考结果（0-拒绝，1-批准）
 * @return  {@link EMV_OK}                  设置成功
 * @return  {@link EMV_UNINITIALIZED}       内核未初始化
 * @return  {@link EMV_ISSREFERRALNOTREQ}   内核未请求发卡行参考
 */
int emv_set_issref_result(int result);

/**
 * 获取交易是否需要上送通知
 *
 * @return  0                               不需要上送
 * @return  1                               需要上送
 */
int emv_is_need_advice(void);

/**
 * 获取交易是否需要签名
 *
 * @return  0                               不需要签名
 * @return  1                               需要签名
 */
int emv_is_need_signature(void);

/**
 * 结束交易流程
 *
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_process_exit(void);


#define P_A_R_A__F_U_N_C__H

/**
 * 设置终端参数
 *
 * @param   par                             [IN]终端参数(TLV格式)
 * @param   len                             [IN]终端参数长度（最大支持1020）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_set_termparam(u8* par, int len);

/**
 * 清除AID参数
 *
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_clr_aidparam(void);

/**
 * 增加AID参数
 *
 * @param   par                             [IN]AID参数(TLV格式)
 * @param   len                             [IN]AID参数长度（最大支持1020）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_add_aidparam(u8* par, int len);

/**
 * 清除CAPK参数
 *
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_clr_capkparam(void);

/**
 * 增加CAPK参数
 *
 * @param   par                             [IN]CAPK参数(TLV格式)
 * @param   len                             [IN]CAPK参数长度（最大支持1020）
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_add_capkparam(u8* par, int len);

/**
 * 清除卡片黑名单
 *
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_clr_cardblack(void);

/**
 * 增加卡片黑名单
 *
 * @param   blk                             [IN]卡片黑名单，格式为{@link EMV_CARDBLACK}
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_add_cardblack(void* blk);

/**
 * 清除证书黑名单
 *
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_clr_certblack(void);

/**
 * 增加证书黑名单
 *
 * @param   blk                             [IN]证书黑名单，格式为{@link EMV_CERTBLACK}
 * @return  {@link EMV_OK}                  成功
 * @return  {@link EMV_FAIL}                失败
 */
int emv_add_certblack(void* blk);


#if defined(__cplusplus)
}
#endif


#endif /* EMVAPI_H_ */
/*
************************************************************************************************************************
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END OF FILE ~~~
************************************************************************************************************************
*/
