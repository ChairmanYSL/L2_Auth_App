/**
************************************************************************************************************************
*                         			  COPYRIGHT (C) 2015 Kingsee Tech. Co., Ltd.
*
* @file     emv_type.h
* @brief    EMV内核接口 - 类型定义
* @author   Kingsee Development Team - chenjinhai@szzt.com.cn
* @version  V43D1.0.28
*
************************************************************************************************************************
*/
#ifndef EMV_TYPE_H_
#define EMV_TYPE_H_


/**
 * 交易结果
 */
enum EMV_Status
{
    EMV_OK                          = (0),              //!< 成功
    EMV_FAIL                        = (-1),             //!< 失败

    /************************** 通用错误（流程无关） **************************/
    EMV_ERR                         = (-100),
    EMV_NULPORTING                  = (EMV_ERR -  1),   //!< 移植接口NULL
    EMV_GETTERMPARAMERR             = (EMV_ERR -  2),   //!< 加载终端配置参数失败
    EMV_GETTERMAPPSERR              = (EMV_ERR -  3),   //!< 加载终端应用列表失败
    EMV_ALLOCBUFFERERR              = (EMV_ERR -  4),   //!< 分配的全局缓存空间不足
    EMV_UNINITIALIZED               = (EMV_ERR -  5),   //!< 内核未初始化
    EMV_NONIDLE                     = (EMV_ERR -  6),   //!< 内核非空闲状态
    EMV_AMOUNTOVER                  = (EMV_ERR -  7),   //!< 交易金额超限
    EMV_AMOUNTZERO                  = (EMV_ERR -  8),   //!< 交易金额为零
    EMV_QPBOCUNSUPPORTED            = (EMV_ERR -  9),   //!< 终端交易属性不支持QPBOC
    EMV_CARDTYPEILL                 = (EMV_ERR - 10),   //!< 卡片通讯方式不允许
    EMV_PROCESSOVER                 = (EMV_ERR - 11),   //!< 流程已结束
    EMV_UNKNOWNSTATUS               = (EMV_ERR - 12),   //!< 未知流程状态
    EMV_APPSELUNDONE                = (EMV_ERR - 13),   //!< 内核未完成应用选择
    EMV_PBOCLOGSFINOTFOUND          = (EMV_ERR - 14),   //!< 交易日志入口没找到
    EMV_PBOCLOGFMTNOTFOUND          = (EMV_ERR - 15),   //!< 交易日志格式没找到
    EMV_PBOCLOGRDRECFAILED          = (EMV_ERR - 16),   //!< 交易日志记录读取失败
    EMV_PBOCLOGRECNOINV             = (EMV_ERR - 17),   //!< 交易日志记录号无效
    EMV_SELCANDIDATENOTREQ          = (EMV_ERR - 18),   //!< 内核未请求选择候选应用列表
    EMV_SELACCOUNTNOTREQ            = (EMV_ERR - 19),   //!< 内核未请求选择帐户类型
    EMV_CHECKIDNOTREQ               = (EMV_ERR - 20),   //!< 内核未请求查验持卡人证件
    EMV_ONLINEPINNOTREQ             = (EMV_ERR - 21),   //!< 内核未请求输入联机PIN
    EMV_OFFLINEPINNOTREQ            = (EMV_ERR - 22),   //!< 内核未请求输入脱机PIN
    EMV_BYPASSPINNOTREQ             = (EMV_ERR - 23),   //!< 内核未请求确认是否跳过PIN输入
    EMV_FORCEONLINENOTREQ           = (EMV_ERR - 24),   //!< 内核未请求确认是否强制联机
    EMV_GOONLINENOTREQ              = (EMV_ERR - 25),   //!< 内核未请求联机授权
    EMV_ISSREFERRALNOTREQ           = (EMV_ERR - 26),   //!< 内核未请求发卡行参考

    /************************** 错误状态（流程相关） **************************/
    AS_ERR                          = (-1100),          //!< 应用选择错误
    AS_SELSW6A81                    = (AS_ERR -  1),    //!<
    AS_SELERR                       = (AS_ERR -  2),    //!<
    AS_FCIFMTILL                    = (AS_ERR -  3),    //!<
    AS_FCINO6F                      = (AS_ERR -  4),    //!<
    AS_FCINO84                      = (AS_ERR -  5),    //!<
    AS_FCINOA5                      = (AS_ERR -  6),    //!<
    AS_FCINO88                      = (AS_ERR -  7),    //!<
    AS_FCI88VALILL                  = (AS_ERR -  8),    //!<
    AS_FCINO50                      = (AS_ERR -  9),    //!<
    AS_FCINOBF0C                    = (AS_ERR - 10),    //!<
    AS_FCIBF0CVALILL                = (AS_ERR - 11),    //!<
    AS_POSERR9F38                   = (AS_ERR - 12),    //!<
    AS_POSERR9F11                   = (AS_ERR - 13),    //!<
    AS_POSERR9F12                   = (AS_ERR - 14),    //!<
    AS_SUBDUPBF0C                   = (AS_ERR - 15),    //!<
    AS_RDRECERR                     = (AS_ERR - 16),    //!<
    AS_RECFMTILL                    = (AS_ERR - 17),    //!<
    AS_RECNO70                      = (AS_ERR - 18),    //!<
    AS_REC70HAS9F12                 = (AS_ERR - 19),    //!<
    AS_REC70HAS42                   = (AS_ERR - 20),    //!<
    AS_REC70HAS5F50                 = (AS_ERR - 21),    //!<
    AS_APPTMPLILL                   = (AS_ERR - 22),    //!<
    AS_APPTMPLNO50                  = (AS_ERR - 23),    //!<
    AS_NULCANDIDATE                 = (AS_ERR - 24),    //!<
    AS_CANDIDATESELERR              = (AS_ERR - 25),    //!<
    AS_NO9F5D                       = (AS_ERR - 26),    //!<
    AS_NO9F4D                       = (AS_ERR - 27),    //!<
    AS_AIDPARGETERR                 = (AS_ERR - 28),    //!<
    AS_PREPROCAMTOVER               = (AS_ERR - 29),    //!<
    AS_PREPROCAMTZERO               = (AS_ERR - 30),    //!<
    AS_SELACCOUNTERR                = (AS_ERR - 31),    //!<
    AS_IADTCREQAC                   = (AS_ERR - 32),    //!<
    AS_TERMINATE                    = (AS_ERR - 33),    //!<

    IA_ERR                          = (-1200),          //!< 应用初始化错误
    IA_NOPDOL                       = (IA_ERR -  1),    //!<
    IA_PDOLNO9F66                   = (IA_ERR -  2),    //!<
    IA_PDOLPACKERR                  = (IA_ERR -  3),    //!<
    IA_GPOSW6985                    = (IA_ERR -  4),    //!<
    IA_GPOSWERR                     = (IA_ERR -  5),    //!<
    IA_GPORSPERR                    = (IA_ERR -  6),    //!<
    IA_GPORSP80                     = (IA_ERR -  7),    //!<
    IA_GPORSP80LENERR               = (IA_ERR -  8),    //!<
    IA_GPORSP77NOAIP                = (IA_ERR -  9),    //!<
    IA_GPORSP77AIPLENERR            = (IA_ERR - 10),    //!<
    IA_GPORSP77NOAFL                = (IA_ERR - 11),    //!<
    IA_GPORSP77AFLLENERR            = (IA_ERR - 12),    //!<
    IA_GPORSPUNEXPECTTAG            = (IA_ERR - 13),    //!<
    IA_NOIAD                        = (IA_ERR - 14),    //!<
    IA_GPORSPNOATC                  = (IA_ERR - 15),    //!<
    IA_GPORSPATCLENERR              = (IA_ERR - 16),    //!<
    IA_GPORSPNOAC                   = (IA_ERR - 17),    //!<
    IA_GPORSPACLENERR               = (IA_ERR - 18),    //!<
    IA_POSERRATC                    = (IA_ERR - 19),    //!<
    IA_NOT2EQU                      = (IA_ERR - 20),    //!<
    IA_NOAFL                        = (IA_ERR - 21),    //!<
    IA_AFLERR                       = (IA_ERR - 22),    //!<

    RD_ERR                          = (-1300),          //!< 读应用数据错误
    RD_AFLILL                       = (RD_ERR -  1),    //!<
    RD_READRECORDERR                = (RD_ERR -  2),    //!<
    RD_RECORDILL                    = (RD_ERR -  3),    //!<
    RD_SAVEDATAERR                  = (RD_ERR -  4),    //!<
    RD_NOPAN                        = (RD_ERR -  5),    //!<
    RD_NOCDOL1                      = (RD_ERR -  6),    //!<
    RD_NOCDOL2                      = (RD_ERR -  7),    //!<
    RD_5F24ERR                      = (RD_ERR -  8),    //!<
    RD_5F25ERR                      = (RD_ERR -  9),    //!<

    DA_ERR                          = (-1400),          //!< 脱机数据认证错误
    DA_READRECORDFAIL               = (DA_ERR -  1),    //!<
    DA_NOCAPKI                      = (DA_ERR -  2),    //!<
    DA_CAPKNOTFOUNT                 = (DA_ERR -  3),    //!<
    DA_CAPKALGERR                   = (DA_ERR -  4),    //!<
    DA_CAPKEXPIRED                  = (DA_ERR -  5),    //!<
    DA_CAPKCHKERR                   = (DA_ERR -  6),    //!<
    DA_NOISSPKCERT                  = (DA_ERR -  7),    //!<
    DA_NOISSPKEXP                   = (DA_ERR -  8),    //!<
    DA_NOICCPKCERT                  = (DA_ERR -  9),    //!<
    DA_NOICCPKEXP                   = (DA_ERR - 10),    //!<
    DA_NODDOL                       = (DA_ERR - 11),    //!<
    DA_NODEFDDOL                    = (DA_ERR - 12),    //!<
    DA_DDOLNO9F37                   = (DA_ERR - 13),    //!<
    DA_DDOLPACKERR                  = (DA_ERR - 14),    //!<
    DA_IAUTHERR                     = (DA_ERR - 15),    //!<
    DA_IAUTHRSPERR                  = (DA_ERR - 16),    //!<
    DA_IAUTHRSP80LENERR             = (DA_ERR - 17),    //!<
    DA_IAUTHRSP77NOSDAD             = (DA_ERR - 18),    //!<
    DA_IAUTHRSP77SDADERR            = (DA_ERR - 19),    //!<
    DA_IAUTHRSPUNEXPECTTAG          = (DA_ERR - 20),    //!<
    DA_SDADLENERR                   = (DA_ERR - 21),    //!<
    DA_SDADRECOVERERR               = (DA_ERR - 22),    //!<
    DA_SDADHEADERERR                = (DA_ERR - 23),    //!<
    DA_SDADTAILERERR                = (DA_ERR - 24),    //!<
    DA_SDADFMTERR                   = (DA_ERR - 25),    //!<
    DA_SDADHASHVERR                 = (DA_ERR - 26),    //!<
    DA_NOSDAD                       = (DA_ERR - 27),    //!<
    DA_9F69ERR                      = (DA_ERR - 28),    //!<
    DA_FDDAVERUNSUP                 = (DA_ERR - 29),    //!<
    DA_NO9F37                       = (DA_ERR - 30),    //!<
    DA_FDDAVERERR                   = (DA_ERR - 31),    //!<
    DA_NOSSAD                       = (DA_ERR - 32),    //!<
    DA_SSADLENERR                   = (DA_ERR - 33),    //!<
    DA_SSADRECOVERERR               = (DA_ERR - 34),    //!<
    DA_SSADHEADERERR                = (DA_ERR - 35),    //!<
    DA_SSADTAILERERR                = (DA_ERR - 36),    //!<
    DA_SSADFMTERR                   = (DA_ERR - 37),    //!<
    DA_SSADHASHVERR                 = (DA_ERR - 38),    //!<
    DA_9F4AERR                      = (DA_ERR - 39),    //!<
    DA_CIDNOTMATCHED                = (DA_ERR - 40),    //!<
    DA_CDAHASHV1ERR                 = (DA_ERR - 41),    //!<
    DA_CDAHASHV2ERR                 = (DA_ERR - 42),    //!<
    DA_ICCDATAMISSING               = (DA_ERR - 43),    //!<
    DA_CERTLENERR                   = (DA_ERR - 44),    //!<
    DA_CERTRECOVERERR               = (DA_ERR - 45),    //!<
    DA_CERTHEADERERR                = (DA_ERR - 46),    //!<
    DA_CERTTAILERERR                = (DA_ERR - 47),    //!<
    DA_CERTFMTERR                   = (DA_ERR - 48),    //!<
    DA_CERTHASHVERR                 = (DA_ERR - 49),    //!<
    DA_PANNOTMATCHED                = (DA_ERR - 50),    //!<
    DA_CERTEXPIRED                  = (DA_ERR - 51),    //!<
    DA_CERTBLOCKED                  = (DA_ERR - 52),    //!<
    DA_CERTALGERR                   = (DA_ERR - 53),    //!<
    DA_PKMODLENERR                  = (DA_ERR - 54),    //!<
    DA_NOPKREM                      = (DA_ERR - 55),    //!<
    DA_NOISSPK                      = (DA_ERR - 56),    //!<
    DA_NOICCPK                      = (DA_ERR - 57),    //!<
    DA_NOPINPKCERT                  = (DA_ERR - 58),    //!<
    DA_NOPINPKEXP                   = (DA_ERR - 59),    //!<
    DA_GETCHALLENGEERR              = (DA_ERR - 60),    //!<
    DA_ENCPINRECOVERERR             = (DA_ERR - 61),    //!<

    RE_ERR                          = (-1500),          //!< 处理限制错误

    CV_ERR                          = (-1600),          //!< 持卡人验证错误
    CV_CVMLISTERR                   = (CV_ERR -  1),    //!<
    CV_ONLINEPINFAIL                = (CV_ERR -  2),    //!<
    CV_OFFLINEPINFAIL               = (CV_ERR -  3),    //!<

    RM_ERR                          = (-1700),          //!< 终端风险管理错误

    AA_ERR                          = (-1800),          //!< 终端行为分析错误
    AA_CDOLPACKERR                  = (AA_ERR -  1),    //!<
    AA_GACERR                       = (AA_ERR -  2),    //!<
    AA_GACRSPERR                    = (AA_ERR -  3),    //!<
    AA_GACRSP80LENERR               = (AA_ERR -  4),    //!<
    AA_GACRSP80VALERR               = (AA_ERR -  5),    //!<
    AA_GACRSP77NOCID                = (AA_ERR -  6),    //!<
    AA_GACRSP77NOATC                = (AA_ERR -  7),    //!<
    AA_GACRSP77NOSDAD               = (AA_ERR -  8),    //!<
    AA_GACRSP77NO9F26               = (AA_ERR -  9),    //!<
    AA_GACRSPUNEXPECTTAG            = (AA_ERR - 10),    //!<
    AA_CIDAAR                       = (AA_ERR - 11),    //!<
    AA_CDAREQUIRE77                 = (AA_ERR - 12),    //!<
    AA_CIDERR                       = (AA_ERR - 13),    //!<
    AA_CIDTERMINATE                 = (AA_ERR - 14),    //!<

    CO_ERR                          = (-1900),          //!< 交易结束错误
    CO_NOTONLINE                    = (CO_ERR -  1),    //!<
    CO_SENDTAPCARDERR				= (CO_ERR -  2),	//!<二次拍卡读卡失败
    CO_RRPERR						= (CO_ERR -  3),	//!<RRP失败
    CO_CCCERR						= (CO_ERR -  4),	//!<CCC失败
    CO_PREGACBALANCEREADERR			= (CO_ERR -  5),	//!<GAC之前读余额失败
    CO_TRANSANALYSERR				= (CO_ERR -  6),	//!<交易结果分析失败
    CO_POSTGACPUTDATAERR			= (CO_ERR -  7),	//!<GAC之后PUT DATA失败
    CO_POSTGPOPUTDATAERR			= (CO_ERR -  8),	//!<GPO之后PUT DATA失败

    CL_ERR                          = (-2000),          //!< ＱＰＢＯＣ错误
    CL_DAFAIL                       = (CL_ERR -  1),    //!<

    /************************** 中间状态（流程相关） **************************/
    EMV_STA_IDLE = 1,                                   //!< 空闲状态，可以执行交易流程
    EMV_STA_CANDIDATES_BUILT = 2,                       //!< 候选列表创建完成
    EMV_REQ_SEL_CANDIDATES = 3,                         //!< 请求选择候选应用列表
    EMV_REQ_SEL_CANDIDATES_AGAIN = 4,                   //!< 请求再次选择候选应用列表
    EMV_STA_APP_SELECTED = 5,                           //!< 应用选择完成
    EMV_REQ_SEL_ACCOUNTTYPE = 6,                        //!< 请求选择帐户类型
    EMV_STA_APP_INITIALIZED = 7,                        //!< 应用初始化完成
    EMV_REQ_DEACTIVATE_CARD = 8,                        //!< 请求移走非接触式卡片（QPBOC专用）
    EMV_STA_CARD_DEACTIVATED = 9,                       //!< 非接触式卡片已移除（QPBOC专用）
    EMV_STA_READ_APP_DATA_COMPLETED = 10,               //!< 读应用数据完成
    EMV_STA_DATA_AUTH_COMPLETED = 11,                   //!< 脱机数据认证完成
    EMV_STA_PROCESS_RESTRICT_COMPLETED = 12,            //!< 处理限制完成
    EMV_REQ_CHECK_ID = 13,                              //!< 请求持卡人出示证件
    EMV_REQ_ONLINE_PIN = 14,                            //!< 请求持卡人输入联机PIN
    EMV_REQ_OFFLINE_PIN = 15,                           //!< 请求持卡人输入脱机PIN
    EMV_REQ_OFFLINE_PIN_LAST = 16,                      //!< 请求持卡人输入脱机PIN（最后一次）
    EMV_REQ_CONFIRM_BYPASS_PIN = 17,                    //!< 请求确认是否跳过PIN输入
    EMV_STA_CARDHOLDER_VERIFY_COMPLETED = 18,           //!< 持卡人验证完成
    EMV_REQ_CONFIRM_FORCE_ONLINE = 19,                  //!< 请求确认是否强制联机
    EMV_STA_RISK_MANAGEMENT_COMPLETED = 20,             //!< 终端风险管理完成
    EMV_STA_ACTION_ANALYSE_COMPLETED = 21,              //!< 终端行为分析完成
    EMV_REQ_GO_ONLINE = 22,                             //!< 请求联机
    EMV_REQ_ISSUER_REFERRAL = 23,                       //!< 请求发卡行参考
    EMV_REQ_GETCARDNO = 24,								//!<读记录完成，可以获取卡号
	EMV_REQ_SELECT_NEXT = 26,							//!<请求重新选择AID
    EMV_REQ_READCAARD_AGAIN = 27,						//!<请求重新读卡(要求先下电后重新上电寻卡复位)
	EMV_STA_PREGACBAlANCEREAD_COMPLETED = 34,			//!<GAC之前读余额流程完成
	EMV_STA_POSTGACPUTDATA_COMPLETED = 35,				//!<GAC之后Put data流程完成
	EMV_STA_TORNRECOVER = 36,							//!<闪卡恢复
	EMV_STA_PUTDATAAFTERGPO_COMPLETED = 38,				//!<GPO之后Put data流程完成
	EMV_STA_GETDATAAFTERGPO_COMPLETED = 39,				//!<GPO之后Get data流程完成
	EMV_STA_POSTGACBAlANCEREAD_COMPLETED = 40,			//!<GAC之后读余额流程完成
	EMV_REQ_PREPROCESS_LOADAIDPARAM = 41,				//!<请求进行预处理，加载AID参数
	EMV_REQ_SECONDTAP = 42,								//!<请求二次拍卡进行联机处理

	/************************** 结束状态（流程相关） **************************/
    EMV_COMPLETED = 100,                                //!< 交易完成
    EMV_ACCEPTED_OFFLINE = 101,                         //!< 交易脱机接受
    EMV_DENIALED_OFFLINE = 102,                         //!< 交易脱机拒绝
    EMV_ACCEPTED_ONLINE = 103,                          //!< 交易联机接受
    EMV_DENIALED_ONLINE = 104,                          //!< 交易联机拒绝
    EMV_FALLBACK = 105,                                 //!< 交易降级
    EMV_SWITCH_INTERFACE = 106,							//!< 交易转界面
    EMV_SEEPHONE = 107,									//!<交易SEE PHONE
    EMV_ENDAPPLICATION = 111,							//!<交易终止
};

/**
 * 交易类型
 *
 * 00-19 - 借记\n
 * 20-29 - 贷记\n
 * 30-29 - 查询服务\n
 * 40-49 - 转账服务\n
 * 50-99 - 保留\n
 * F0-FF - 内核自定义\n
 */
enum EMV_TransTypes
{
    /* 借记 */
    EMV_TRANSTYPE_GOODS_SERVICES = 0x00,            //!< 商品和服务
    EMV_TRANSTYPE_CASH = 0x01,                      //!< 现金
    EMV_TRANSTYPE_ADJUSTMENT_DEBIT = 0x02,          //!< 调整（借记）
    EMV_TRANSTYPE_CHEQUE_GUARANTEE = 0x03,          //!< 支票担保（资金可担保）
    EMV_TRANSTYPE_CHEQUE_VERIFICATION = 0x04,       //!< 支票核实（资金可用但不担保）
    EMV_TRANSTYPE_EUROCHEQUE = 0x05,                //!< 欧洲支票
    EMV_TRANSTYPE_TRAVELLER_CHEQUE = 0x06,          //!< 旅行支票
    EMV_TRANSTYPE_LETTER_OF_CREDIT = 0x07,          //!< 信用证
    EMV_TRANSTYPE_GLRO = 0x08,                      //!< 直接转账（邮政银行业务）
    EMV_TRANSTYPE_GOODS_SERVICES_WITH_CASH = 0x09,  //!< 伴有现金支付的商品和服务
    EMV_TRANSTYPE_CASHBACK = 0x09,                  //!< 返现 FIXME
    /* 贷记 */
    EMV_TRANSTYPE_RETURNS = 0x20,                   //!< 退款
    EMV_TRANSTYPE_DEPOSITS = 0x21,                  //!< 存款
    EMV_TRANSTYPE_ADJUSTMENT_CREDIT = 0x22,         //!< 调整（贷记）
    EMV_TRANSTYPE_CHEQUE_DEPOSIT_GUARANTEE = 0x23,  //!< 支票存款担保
    EMV_TRANSTYPE_CHEQUE_DEPOSIT = 0x24,            //!< 支票存款
    /* 查询服务 */
    EMV_TRANSTYPE_AVAILABLE_FUNDS_INQUIRY = 0x30,   //!< 可用资金查询
    EMV_TRANSTYPE_BALANCE_INQUIRY = 0x31,           //!< 查询余额
    /* 转账服务 */
    EMV_TRANSTYPE_TRANFER = 0x40,                   //!< 持卡人账户转账
    /* 内核自定义 */
    _EMV_TRANSTYPE_PBOCLOG = 0xF0,                  //!< 读取交易日志
    _EMV_TRANSTYPE_READ_PAN = 0xF1,                 //!< 读主账号
    _EMV_TRANSTYPE_READ_TRACK2 = 0xF2,              //!< 读二磁道等效数据
    _EMV_TRANSTYPE_CL_INQUIRY = 0xF3,               //!< QPBOC余额查询
    _EMV_TRANSTYPE_EC_INQUIRY = 0xF4,               //!< 电子现金余额查询
    _EMV_TRANSTYPE_LITE_FLOW = 0xF5,                //!< 简化流程交易（读应用数据完成就结束）
    _EMV_TRANSTYPE_UNKNOWN = 0xFF,                  //!< 未知
};

/**
 * 卡片类型
 */
enum EMV_CardTypes
{
    EMV_CARD_UNKNOWN = 0x00,                        //!< 未知
    EMV_CARD_CONTACT = 0x01,                        //!< 接触式
    EMV_CARD_CONTACTLESS = 0x02,                    //!< 非接触式
};

/**
 * 支付方式
 */
enum EMV_PayModes
{
    EMV_PAYMODE_UNKNOWN,                            //!< 未知
    EMV_PAYMODE_PBOC,                               //!< 标准借贷记（接触式或非接触式）
    EMV_PAYMODE_EC,                                 //!< 电子现金（接触式）
    EMV_PAYMODE_MSD,                                //!< MSD（非接触式，已废除）
    EMV_PAYMODE_QPBOC,                              //!< 快速借贷记（非接触式）
};

/**
 * 服务点输入方式
 */
enum EMV_PosEntryModes
{
    EMV_POSENTRYMODE_UNKNOWN = 0x00,                //!< 未知
    EMV_POSENTRYMODE_MANUAL = 0x01,                 //!< 手工输入
    EMV_POSENTRYMODE_QPBOC_CL = 0x07,               //!< 快速借贷记IC卡读入（非接触式）
    EMV_POSENTRYMODE_MAG_FALLBACK = 0x80,           //!< 磁条卡获取（FALLBACK）
    EMV_POSENTRYMODE_MAG = 0x90,                    //!< 磁条卡获取
    EMV_POSENTRYMODE_MSD_CL = 0x91,                 //!< MSD（非接触式，已废除）
    EMV_POSENTRYMODE_PBOC = 0x95,                   //!< 标准借贷记IC卡读入（接触式）
    EMV_POSENTRYMODE_PBOC_CL = 0x98,                //!< 标准借贷记IC卡读入（非接触式）
};

/**
 * 帐户类型
 */
enum EMV_AccountTypes
{
    EMV_ACCOUNT_DEFAULT = 0x00,                     //!< 默认-未指定（Default）
    EMV_ACCOUNT_SAVINGS = 0x10,                     //!< 储蓄账户（Savings）
    EMV_ACCOUNT_CHEQUE_DEBIT = 0x20,                //!< 支票账户/借记账户（Cheque/Debit）
    EMV_ACCOUNT_CREDIT = 0x30,                      //!< 信用账户（Credit）
};

/**
 * 提示信息索引
 */
enum EMV_MessageIDs
{
    EMV_MSG_CAPK_NotFound,                         	//!< 提示CA公钥未找到
    EMV_MSG_CAPK_Expired,                          	//!< 提示CA公钥已过期
    EMV_MSG_CAPK_Invalid,                          	//!< 提示CA公钥不可用

    EMV_MSG_ServiceNotAllowed,                     	//!< 提示不允许服务
    EMV_MSG_PinTryLimitExceeded,                   	//!< 提示PIN尝试次数超限
    EMV_MSG_IssuerAuthenticationFailed,            	//!< 提示发卡行认证失败

    EMV_MSG_BlacklistCard,                         	//!< 提示黑名单卡
    EMV_MSG_AppExpired,                            	//!< 提示应用已失效
};

/**
 *  用户操作返回值
 */
enum EMV_OperResults
{
    EMV_OPER_OK = 0,                                //!< 确认 / 是
    EMV_OPER_CANCEL = -1,                           //!< 取消 / 否
    EMV_OPER_BACK = -2,                             //!< 返回
    EMV_OPER_QUIT = -3,                             //!< 退出
    EMV_OPER_TIMEOUT = -4,                          //!< 超时
    EMV_OPER_BYPASS = -5,                           //!< 跳过（没有输入直接确认）
    EMV_OPER_UNSUPPORTED = -6,                      //!< 不支持
    EMV_OPER_REPORT_APP = -7,                       //!< 不处理，直接上报应用
};

/**
 * 公钥算法标识
 */
enum EMV_PubKeyAlgorithms
{
    EMV_PUBKEY_ALG_RSA = 0x01,                      //!< 签名算法：RSA，       加密算法：RSA，       哈希算法：SHA-1
    EMV_PUBKEY_ALG_ECC2 = 0x02,                     //!< 签名算法：ECC(ECDSA)，加密算法：ECC(ECIES)，哈希算法：SHA-256
    EMV_PUBKEY_ALG_ECC4 = 0x03,                     //!< 签名算法：ECC(ECDSA)，加密算法：ECC(ECIES)，哈希算法：SHA-512
    EMV_PUBKEY_ALG_SM2 = 0x04,                      //!< 签名算法：SM2，       加密算法：SM2，       哈希算法：SM3
};

/**
 * 哈希算法标识
 */
enum EMV_HashAlgorithms
{
    EMV_HASH_ALG_SHA1 = 0x01,                       //!< SHA-1
    EMV_HASH_ALG_SHA256 = 0x03,                     //!< SHA-256
    EMV_HASH_ALG_SHA512 = 0x05,                     //!< SHA-512
    EMV_HASH_ALG_SM3 = 0x07,                        //!< SM3
};

/**
 * 联机结果
 */
enum EMV_OnlineResults
{
    EMV_ONLINE_NOT = 0,                             //!< 未联机
    EMV_ONLINE_FAIL,                                //!< 联机失败
    EMV_ONLINE_SUCC_ACCEPT,                         //!< 联机成功，发卡行批准
    EMV_ONLINE_SUCC_DENIAL,                         //!< 联机成功，发卡行拒绝
    EMV_ONLINE_SUCC_ISSREF,                         //!< 联机成功，发卡行指引
    _EMV_2GAC_AAC,                                  //!< 2GAC请求拒绝（内核专用）
};

/**
 * 证件类型
 */
enum PBOC_DocumentTypes
{
    PBOC_DOCUMENT_ID_CARD = 0x00,                   //!< 身份证
    PBOC_DOCUMENT_MILITARY_OFFICER = 0x01,          //!< 军官证
    PBOC_DOCUMENT_PASSPORT = 0x02,                  //!< 护照
    PBOC_DOCUMENT_ENTRY_PERMIT = 0x03,              //!< 入境证
    PBOC_DOCUMENT_INTERIM_ID_CARD = 0x04,           //!< 临时身份证
    PBOC_DOCUMENT_OTHER = 0x05,                     //!< 其它
};

/**
 * 内核自定义的交易日志记录号
 */
enum PBOC_SepcTransRecIndexs
{
    PBOC_TRANSREC_SFI = 0x100,                      //!< 交易日志入口
    PBOC_TRANSREC_NUM = 0x101,                      //!< 交易日志个数
    PBOC_TRANSREC_FMT = 0x102,                      //!< 交易日志格式
};


/**
 * 终端参数
 */
typedef struct {
    u8 ics[7];                                      //!< [----]IMPLEMENTATION CONFORMANCE STATEMENT
    u8 term_type;                                   //!< [9F35]终端类型
    u8 term_caps[3];                                //!< [9F33]终端性能
    u8 term_add_caps[5];                            //!< [9F40]终端附加性能
    u8 term_trans_qualifiers[4];                    //!< [9F66]终端交易属性

    u8 term_floor_limit[4];                         //!< [9F1B]终端最低限额

    u8 status_check_support_indicator;              //!< [----]状态检查支持指示器（QPBOC）
    u8 cl_trans_limit_exist;                        //!< ------终端非接触交易限额存在标记
    u8 cl_trans_limit[6];                           //!< [DF20]终端非接触交易限额
    u8 cl_floor_limit_exist;                        //!< ------终端非接触脱机最低限额存在标记
    u8 cl_floor_limit[6];                           //!< [DF19]终端非接触脱机最低限额
    u8 cvm_limit_exist;                             //!< ------终端执行CVM所需限额存在标记
    u8 cvm_limit[6];                                //!< [DF21]终端执行CVM所需限额

    u8 ec_support_indicator;                        //!< [9F7A]电子现金终端支持指示器
    u8 ec_trans_limit_exist;                        //!< ------电子现金终端交易限额存在标记
    u8 ec_trans_limit[6];                           //!< [9F7B]电子现金终端交易限额

    u8 sm_support_indicator;                        //!< [DF69]国密算法支持指示器
} EMV_TERMPARAM;

/**
 * 应用参数
 */
typedef struct {
    EMV_TERMPARAM term;                             //!< 终端参数

    u8 aid[16];                                     //!< [9F06]应用标识符
    u8 aid_len;                                     //!< ------应用标识符长度

    u8 app_sel_indicator;                           //!< [DF01]应用选择指示器（0-部分匹配 1-完全匹配）
    u8 app_ver[2];                                  //!< [9F09]应用版本号

    u8 target_percent;                              //!< [DF17]随机选择目标百分数
    u8 max_target_percent;                          //!< [DF16]偏置随机选择最大目标百分数
    u8 threshold_value[4];                          //!< [DF15]偏置随机选择阀值

    u8 default_ddol_len;                            //!< ------默认动态数据认证数据对象列表长度
    u8 default_ddol[252];                           //!< [DF14]默认动态数据认证数据对象列表
    u8 default_tdol_len;                            //!< ------默认交易证书数据对象列表长度
    u8 default_tdol[252];                           //!< [----]默认交易证书数据对象列表

    u8 term_id[8];                                  //!< [9F1C]终端标识
    u8 term_ifd[8];                                 //!< [9F1E]终端接口设备序列号

    u8 term_country_code[2];                        //!< [9F1A]终端国家代码
    u8 trans_currency_code[2];                      //!< [5F2A]交易货币代码
    u8 trans_currency_exponent;                     //!< [5F36]交易货币指数
    u8 trans_ref_currency_code[2];                  //!< [9F3C]交易参考货币代码
    u8 trans_ref_currency_exponent;                 //!< [9F3D]交易参考货币指数

    u8 term_action_code_default_exist;              //!< ------终端行为代码（默认）存在标记
    u8 term_action_code_default[5];                 //!< [DF11]终端行为代码（默认）
    u8 term_action_code_online_exist;               //!< ------终端行为代码（联机）存在标记
    u8 term_action_code_online[5];                  //!< [DF12]终端行为代码（联机）
    u8 term_action_code_denial_exist;               //!< ------终端行为代码（拒绝）存在标记
    u8 term_action_code_denial[5];                  //!< [DF13]终端行为代码（拒绝）

    u8 term_risk_mana_data_len;                     //!< ------终端风险管理数据长度
    u8 term_risk_mana_data[8];                      //!< [9F1D]终端风险管理数据

    u8 acquirer_id[6];                              //!< [9F01]收单行标识

    u8 merchant_category_code[2];                   //!< [9F15]商户分类码
    u8 merchant_id[15];                             //!< [9F16]商户标识
    u8 merchant_name[20];                           //!< [9F4E]商户名称和位置

    u8 online_pin_support_indicator;                //!< [DF18]联机PIN支持指示器
} EMV_AIDPARAM;

/**
 * 终端应用列表（占20字节）
 */
typedef struct {
    u8 aid[16];                                     //!< 应用标识符
    u8 aid_len;                                     //!< 应用标识符长度
    u8 asi;                                         //!< 应用选择指示器（0-部分匹配 1-完全匹配）
    u8 enable;                                      //!< 有效标记
    u8 rsv;                                         //!< 保留
} EMV_TERMAPP;

/**
 * 候选应用列表
 */
typedef struct {

    u8 aid[16];                                     //!< 应用标识符
    u8 aid_len;                                     //!< 应用标识符长度
    u8 lable[16];                                   //!< 应用标签
    u8 lable_len;                                   //!< 应用标签长度
    u8 preferred_name[16];                          //!< 应用首选名称
    u8 preferred_name_len;                          //!< 应用首选名称长度
    u8 priority;                                    //!< 应用优先指示器
    u8 rsv[4];                                      //!< 保留（rsv[0]在内核中被用来表示终端应用列表索引）
} EMV_CANDIDATE;

/**
 * ＲＳＡ公钥格式
 */
typedef struct {
    u8 modulus[248];                                //!< 模
    u8 modulus_len;                                 //!< 模长
    u8 exponent[3];                                 //!< 指数
} EMV_RSAPUBKEY;

/**
 * ＳＭ２公钥格式
 */
typedef struct {
    u8 r[32];                                       //!< R
    u8 s[32];                                       //!< S
} EMV_SM2PUBKEY;

/**
 * 公钥格式
 */
typedef union {
    EMV_RSAPUBKEY rsa;                              //!< RSA公钥
    EMV_SM2PUBKEY sm2;                              //!< SM2公钥
} EMV_PUBLICKEY;

/**
 * ＣＡ公钥证书格式（占512字节）
 */
typedef struct {
    u8 rid[5];                                      //!< Registered Application Provider Identifier
    u8 index;                                       //!< Certification Authority Public Key Index
    u8 algorithm;                                   //!< Certification Authority Public Key Algorithm Indicator
    u8 hash_algorithm;                              //!< Certification Authority Hash Algorithm Indicator
    EMV_PUBLICKEY key;                              //!< Certification Authority Public Key
    u8 expiration_date[4];                          //!< Certificate Expiration Date
    u8 checksum[20];                                //!< Certification Authority Public Key Checksum
    u8 rsv[228];                                    //!< Reserved
} EMV_CAPKPARAM;

/**
 * 卡片黑名单记录格式（异常文件，占16字节）
 */
typedef struct {
    u8 pan[10];                                     //!< Application PAN
    u8 pan_len;                                     //!< The Length of Application PAN
    u8 psn;                                         //!< Application PAN Sequence Number
    u8 rsv[4];                                      //!< Reserved
} EMV_CARDBLACK;

/**
 * 证书黑名单记录格式（回收列表，占16字节）
 */
typedef struct {
    u8 rid[5];                                      //!< Registered Application Provider Identifier
    u8 pki;                                         //!< Certificate Authority Public Key Index
    u8 csn[3];                                      //!< Certificate Serial Number
    u8 rsv[7];                                      //!< Reserved
} EMV_CERTBLACK;


#endif /* EMV_TYPE_H_ */
/*
************************************************************************************************************************
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END OF FILE ~~~
************************************************************************************************************************
*/
