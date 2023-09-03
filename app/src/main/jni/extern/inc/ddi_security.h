#ifndef _DDI_SECURITY_
#define _DDI_SECURITY_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*OnPosStatusChangeListener)(unsigned int action, unsigned int status, unsigned char *data, unsigned int dataLen);


/** 获取SP状态。
 * @param[out] status 4字节的状态信息。第1字节：SP当前状态，第2字节：安全信息1，第3字节：安全信息2，第 4字节：安全信息3。
 * @return 状态码。
 */
int ddi_security_get_status(uint8_t status[4]);

/**
 * 发送触发响应报文。
 */
int ddi_security_response_trigger(uint8_t serialno);

/** 读取触发信息。
 * @param[out] trigger_status 触发时寄存器值。
 * @param[out] cur_status 当前触发寄存器值，可以通过该值来判断是否还处于触发状态。
 * @param[out] trigger_count 触发计数。
 * @param[out] trigger_mode 解触发方式：0：触发未启用；1：后台解触发；2：任意数据解触发。
 * @return 状态码。
 */
int ddi_security_get_trigger_info(uint32_t *trigger_status, uint32_t *cur_status, uint32_t *trigger_count, uint32_t *trigger_mode);

/** 向SP申请安全处理数据。
 * @param[in] action_type 操作类型，0x30 – 解触发；0x31-  release/debug状态切换；0x32- 防切机重置；0x33-  SMT初始化（删除所有文件，及默认不做防切机处理）。
 * @param[out] ret_action_type 操作类型。
 * @param[out] trigger_count 触发计数(10进制)。
 * @param[out] sn 20字节SN号,不足补0x20。
 * @param[out] rand 4个字节随机数, 10机制格式。
 * @param[out] preventpkid 所使用的防切机证书ID。
 * @return 状态码。
 */
int ddi_security_request_safe_process_data(uint8_t action_type, uint8_t *ret_action_type, uint32_t *trigger_count, uint8_t sn[32], uint8_t sp_cpu_id[32], uint8_t rand[4], uint8_t *preventpkid);

/** 向SP发送安全处理数据，该命令用于发生触发时，终端不处于触发状态时解除触发状态。
 * @param[in] data 加密数据，使用防切机私钥对(操作类型+触发计数+SN号+随机数)进行加密的数据。
 * @param[datalen] 加密数据长度
 * @return 状态码。
 */
int ddi_security_send_safe_process_data(uint8_t *data, uint16_t datalen);

/** 向SP发送获取当前release/debug状态。
 * @param[in] rand 16字节随机数，使用sp认证公钥对16个字节随机数加密后发送给SP。
 * @param[in] rand_len 随机数长度。
 * @param[out] enc_state 加密数据,使用16个字节随机数对(1个字节状态码+4个字节crc+3个字节0x00)加密后发送给AP。状态码说明: 0x00 – release  状态；0x01 – debug状态。
 * @return 状态码。
 */
int ddi_security_get_debug_state(uint8_t *rand, uint16_t rand_len, uint8_t enc_state[8]);

/** 开启触发检测。
 * @return 触发级别。0: 触发未启用；1：后台解触发； 2：任意数据解触发。
 */
uint8_t ddi_security_enable_trigger_detect();

/**
 * 获取使用防切机公钥加密的随机数。
 * @param[out] buf 随机数buffer。
 * @return 状态码。
 */
int ddi_security_get_enc_random(uint8_t *buf, int buflen);

int ddi_security_register_pos_state_listener(OnPosStatusChangeListener listener);

int ddi_security_unregister_pos_state_listener(OnPosStatusChangeListener listener);

#ifdef __cplusplus
}
#endif

#endif