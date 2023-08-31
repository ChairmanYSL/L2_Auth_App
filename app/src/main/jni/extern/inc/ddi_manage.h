#ifndef _DDI_MANAGE_
#define _DDI_MANAGE_

#ifdef __cplusplus
extern "C"
{
#endif

/** SP软件复位。
 * @return 状态码。
 */
int ddi_manage_sp_reset();

/** SP取消所有操作。
 * @return 状态码。
 */
int ddi_manage_cancel_all_action();

/** SP蜂鸣器发声。
 * @param[in] time 次数。
 * @param[in] freq PWM频率。
 * @param[in] sound_time 发声时间。
 * @param[in] close_time 关时间。
 * @return 状态码。
 */
int ddi_manage_beeper_setting(uint8_t time, uint32_t freq, uint32_t sound_time, uint32_t close_time);

/** 控制SP LED灯状态。
 * @param[in] thiz SpCommClient对象。
 * @param[in]  size 灯的数量
 * @param[in] led_type 灯类型，0 – 蓝 1 – 黄 2 – 红 3 – 绿。
 * @param[in] status 灯状态，0 –关   1-亮   2- 闪  3-不变。
 * @param[in] on_after_flicker 闪灯后状态，0 –关   1-亮。
 * @param[in] on_time 灯闪亮灯周期时长，单位ms。
 * @param[in] off_time 灯闪亮灭周期时长，单位ms。
 * @param[in] total_time 灯闪总时间，单位ms。
 * @return 状态码
 */
int ddi_manage_led_setting(int size, uint8_t *led_type, uint8_t *status, uint8_t *on_after_flicker, uint16_t *on_time, uint16_t *off_time, uint32_t *total_time);

/** SP休眠时间设置。
 * @param[in] sleep_time 休眠时间。
 * @return 状态码。
 */
int ddi_manage_set_sleep_time(uint32_t sleep_time);

/** 获取SP休眠时间。
 * @return 休眠时间。
 */
uint32_t ddi_manage_get_sleep_time();

/** 设置设备SN号。
 * @param[in] sn SN号。
 * @param[in] sn_len SN号长度。
 * @return 状态码。
 */
int ddi_manage_set_sn(uint8_t *sn, uint16_t sn_len);

/** 获取设备SN号。
 * @param[out] sn_buf 用于存储SN号。
 * @param[in,out] buf_len 输入表示buffer长度，输出表示SN号的长度。
 * @return 状态码。
 */
int ddi_manage_get_sn(uint8_t *sn_buf, uint16_t *buf_len);

/** 获取SP版本信息。 TODO 2,3域是N组循环???
 * @param[in] action 版本位操作，0x01: 获取 BOOT版本号；0x02: 获取CORE版本号；0x04:获取 APP版本号；0x08:获取硬件版本号；按位与可以同时获取多项版本号信息，如0x0F,同时获取boot、core、app以及硬件版本号。
 * @param[out] versions_buf 用于存储版本号的buffer二维数组。
 * @param[out] versions_len 每个版本号的长度。
 * @param[in,out] buf_len 输入表示buffer长度，输出表示版本号的个数。
 * @return 状态码。
 */
int ddi_manage_get_version(uint8_t action, uint8_t **versions_buf, uint8_t *versions_len, uint8_t *buf_len);

/** 获取随机数。
 * @param[in] len 要获取的随机数的长度。
 * @param[out] rand_buf 用于存储随机数的buffer。
 * @param[in,out] buf_len 输入表示buffer长度，输出表示获取到的随机数的长度，最大4096字节。
 * @return 状态码。
 */
int ddi_manage_get_random(uint16_t len, uint8_t *rand_buf, uint16_t *buf_len);

/**
 * 获取ADC格式的硬件版本信息。
 * @param[out] version 获取adc硬件版本号
 * @return
 */
int ddi_manage_get_adc_hw_version(uint8_t *version);

int ddi_manage_sp_set_time(uint8_t *datetime, uint32_t datetimelen);

int ddi_manage_sp_get_time(uint8_t *datetime, uint32_t datetimelen);

/** 透传SP指令。
 * @param[in] cmd_major 主指令号。
 * @param[in] cmd_minor 次指令号。
 * @param[in] data 数据。
 * @param[in] data_len 数据长度。
 * @param[out] result 返回结果。
 * @param[in,out] result_len 结果长度。
 * @return 状态码。
 */
int ddi_manage_transmit(uint8_t cmd_major, uint8_t cmd_minor,uint8_t* data, uint16_t data_len,uint8_t* result, uint16_t *result_len);

int ddi_manage_write(uint8_t *data, uint16_t data_len);

int ddi_manage_read(uint8_t *data, uint16_t data_len);

/**
 * 将sp通讯服务挂起
 * @return int 
 */
int ddi_manage_suspend();

/**
 * 将sp通讯服务恢复
 * @return int 
 */
int ddi_manage_resume();

/**
 * 硬件复位SP
 * @return
 */
int ddi_manage_hw_reset();

#ifdef __cplusplus
}
#endif

#endif