/** @brief 错误定义。
 * @file ddi_global.h
 * @author yinlijun
 * @version v1.0.0
 * @date 2017.1.17
 */

#ifndef _DDI_RESULT_H_
#define _DDI_RESULT_H_

#ifdef __cplusplus
extern "C"
{
#endif

/** 内部错误。*/
#define STATUS_CODE_ERROR_INTERNAL (-1)
/** 处理成功。*/
#define STATUS_CODE_SUCCESS (0x00)
/** 指令码不支持。*/
#define STATUS_CODE_CMD_NOT_SUPPORT (0x01)
/** 参数错误。*/
#define STATUS_CODE_INVALID_PARAM (0x02)
/** 可变数据域长度错误。*/
#define STATUS_CODE_DATA_LEN (0x03)
/** 帧格式错误。*/
#define STATUS_CODE_FRAME_FORMAT (0x04)
/** CRC校验错误。*/
#define STATUS_CODE_INVALID_CRC (0x05)
/** 其他错误。*/
#define STATUS_CODE_OTHER_ERROR (0x06)
/** 超时。*/
#define STATUS_CODE_TIMEOUT (0x07)
/** 操作失败。*/
#define STATUS_CODE_ACTION_FAILED (0x08)
/** 复位失败或者卡片类型错误。*/
#define STATUS_CODE_RESET_FAILED_OR_INVALID_CARD_TYPE (0x09)
/** 非安全状态。*/
#define STATUS_CODE_UNSAFE_STAUS (0x0A)
/** 认证失败。*/
#define STATUS_CODE_AUTH_FAILED (0x0B)


#define MAX_SN_LEN (32)
#define MAX_VERSION_LEN (20)
#define MAX_UPDATE_ERROR_INFO_LEN (255)
#define MAX_MAG_CARD_ENC_IMPORT_DATA_LEN (32)
#define MAX_MAG_CARD_CHANNEL_PLAIN_DATA_LEN (120)
#define MAX_MAG_CARD_CHANNEL_ENC_DATA_LEN (120)
#define MAX_MAG_CARD_OTHER_END_DATA_LEN (32)
#define MAX_ICC_APDU_LEN (320)
#define MAX_ICC_ATR_LEN (99)
#define MAX_M1_USER_DATA_LEN (255)
#define MAX_PINPAD_PINBLOCK_LEN (20)
#define MAX_PINPAD_MAC_LEN (18)
#define MAX_FACTORY_UDPATE_MAIN_KEY_RESULT_LEN (200)
#define MAX_TRANSPARENT_COMMAND_RESULT_LEN (1024)
#define MAX_FIELD55_DATA_LEN (64)

#define APSPCOMM_DEFAULTTIMEOUT_DOWNLOADDUKPT  15000 

#ifdef __cplusplus
}
#endif

#endif
