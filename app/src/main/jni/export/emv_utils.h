/**
************************************************************************************************************************
*                         			  COPYRIGHT (C) 2015 Kingsee Tech. Co., Ltd.
*
* @file     emv_utils.h
* @brief    EMV内核接口 - 实用工具
* @author   Kingsee Development Team - chenjinhai@szzt.com.cn
* @version  V43D1.0.28
*
************************************************************************************************************************
*/
#ifndef EMV_UTILS_H_
#define EMV_UTILS_H_


/*
************************************************************************************************************************
* INCLUDE FILES
************************************************************************************************************************
*/


/*
************************************************************************************************************************
* EXTERNS
************************************************************************************************************************
*/


/*
************************************************************************************************************************
* DEFINES
************************************************************************************************************************
*/

/**
 * TLV解析标记
 */
enum EMV_TLVDecodeFlags
{
    SINGLE_TLVOBJ = 0x01,                           //!< 解析单个的TLV数据
    STRING_TLVOBJ = 0x02,                           //!< 解析多个的TLV数据
    DECODE_LEVEL1 = 0x10,                           //!< 解析第一级
};

/**
 * TLV查找级别
 */
enum EMV_TLVFindLevels
{
    SEARCH_ALL_DESC = 0x01,                         //!< 查找指定索引下挂的子孙TLV
    SEARCH_ONLY_SON = 0x02,                         //!< 查找指定索引下挂的子TLV
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

/**
 * TLV解析数据格式
 */
typedef struct {
    u32 tagname;                                    //!< 标签名
    int parent;                                     //!< 父元素级别（从0开始）
    int childnum;                                   //!< 子元素个数
    int valuelen;                                   //!< 元素数据长度
    u8* pvalue;                                     //!< 元素数据
} EMV_TLVOBJ;


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
 * TLV数据解析
 *
 * @param   pData                                   [IN]TLV数据缓存
 * @param   nDataLen                                [IN]TLV数据缓存的长度
 * @param   pTLVs                                   [OU]TLV解析数据输出缓存
 * @param   nTLVs                                   [IN]TLV解析数据输出缓存的个数（最大支持的TLV个数）
 * @param   bFlags                                  [IN]解析标记，参考{@link EMV_TLVDecodeFlags}
 * @return  {@link EMV_OK}                          解析成功
 * @return  {@link EMV_FAIL}                        解析失败
 */
int EMV_TLV_Decode(u8 *pData, int nDataLen, EMV_TLVOBJ *pTLVs, int nTLVs, int bFlags);

/**
 * 查找指定TAG对应TLV解析数据（通过{@link EMV_TLV_Decode}解析）中的索引
 *
 * @param   nParent                                 [IN]查找位置索引，对应pTLVs数组的下标
 * @param   nTag                                    [IN]查找标签
 * @param   pTLVs                                   [IN]TLV解析数据缓存
 * @param   nLevel                                  [IN]查找级别，参考{@link EMV_TLVFindLevels}
 * @return  =0                                      没找到
 * @return  >0                                      nTag在pTLVs数组中对应的下标
 */
int EMV_TLV_Find(int nParent, u32 nTag, EMV_TLVOBJ *pTLVs, int nLevel);

/**
 * 使用DOL和对应数据打包成TLV数据
 *
 * @param   pDOL                                    [IN]数据对象列表（含TAG和LEN域）
 * @param   nDOLLen                                 [IN]数据对象列表的长度
 * @param   pVal                                    [IN]DOL对应数据（VAL域）
 * @param   nValLen                                 [IN]DOL对应数据的长度
 * @param   pOut                                    [OU]TLV数据输出缓存
 * @param   pOutLen                                 [OU]TLV数据输出长度缓存
 * @param   nOutMaxLen                              [IN]TLV数据输出缓存的最大长度
 * @return  {@link EMV_OK}                          打包成功
 * @return  {@link EMV_FAIL}                        打包失败
 */
int EMV_TLV_PacketByDOL(u8 *pDOL, int nDOLLen, u8 *pVal, int nValLen, u8 *pOut, int *pOutLen, int nOutMaxLen);

/**
 * 使用TAG数组去获取应用数据打包成TLV数据
 *
 * @param   pTags                                   [IN]TAG列表
 * @param   nTagNum                                 [IN]TAG个数
 * @param   pOut                                    [OU]TLV数据缓存
 * @param   pOutLen                                 [OU]TLV数据长度缓存
 * @param   nOutMaxLen                              [IN]TLV数据缓存最大长度
 * @return  {@link EMV_OK}                          打包成功
 * @return  {@link EMV_FAIL}                        打包失败
 */
int EMV_TLV_PacketByTAG(u32 *pTags, int nTagNum, u8 *pOut, int *pOutLen, int nOutMaxLen);


/**
 * ＩＣ卡命令接口
 *
 * @param   pInput                                  [IN]输入数据（C-APDU）
 * @param   nInlen                                  [IN]输入数据长度
 * @param   pOutput                                 [OU]输出数据缓存
 * @param   pOutlen                                 [OU]输出数据长度缓存（不含SW1和SW2）
 * @return  状态字(SW)，如0x9000,0x6A82,0x0000
 */
int EMV_ICC_Apdu(u8 *pInput, int nInlen, u8 *pOutput, int *pOutlen);

/**
 * ＩＣ卡取数据命令（GET DATA）
 * @param   nTag                                    [IN]标签，参考PBOC第5部分表B.10
 * @param   pOut                                    [OU]标签数据
 * @param   pOutLen                                 [OU]标签数据长度
 * @return  [>=0]                                   状态字(SW)，如0x9000,0x6A88,0x0000
 * @return  {@link EMV_UNINITIALIZED}               内核未初始化
 * @return  {@link EMV_APPSELUNDONE}                内核未完成应用选择
 */
int EMV_ICC_GetData(u32 nTag, u8 *pOut, int *pOutLen);

/**
 * ＩＣ卡读记录命令（READ RECORD）
 * @param   nSFI                                    [IN]SFI，短文件标识符
 * @param   nRecNo                                  [IN]记录号
 * @param   pOut                                    [OU]记录数据
 * @param   pOutLen                                 [OU]记录数据长度
 * @return  [>=0]                                   状态字(SW)，如0x9000,0x6A83,0x0000
 * @return  {@link EMV_UNINITIALIZED}               内核未初始化
 * @return  {@link EMV_APPSELUNDONE}                内核未完成应用选择
 */
int EMV_ICC_ReadRecord(u8 nSFI, u8 nRecNo, u8 *pOut, int *pOutLen);


/**
 * 读取卡片交易记录（PBOC）
 *
 * @param   nRecNo                                  [IN]记录号
 *   @arg   [0x00~0xff]                             交易日志记录号
 *   @arg   {@link PBOC_TRANSREC_SFI}               交易日志入口
 *   @arg   {@link PBOC_TRANSREC_NUM}               交易日志个数
 *   @arg   {@link PBOC_TRANSREC_FMT}               交易日志格式
 * @param   pOut                                    [OU]记录数据
 * @param   pOutLen                                 [OU]记录数据长度
 * @return  {@link EMV_OK}                          成功
 * @return  {@link EMV_UNINITIALIZED}               内核未初始化
 * @return  {@link EMV_APPSELUNDONE}                内核未完成应用选择
 * @return  {@link EMV_PBOCLOGSFINOTFOUND}          交易日志入口没找到
 * @return  {@link EMV_PBOCLOGFMTNOTFOUND}          交易日志格式没找到
 * @return  {@link EMV_PBOCLOGRDRECFAILED}          交易日志记录读取失败
 * @return  {@link EMV_PBOCLOGRECNOINV}             交易日志记录号无效
 */
int EMV_GetTransLog(int nRecNo, u8 *pOut, int *pOutLen);



#endif /* EMV_UTILS_H_ */
/*
************************************************************************************************************************
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ END OF FILE ~~~
************************************************************************************************************************
*/
