
#ifndef _DDI_MIFIRE_
#define _DDI_MIFIRE_

#ifdef __cplusplus
extern "C"
{
#endif

/** M1卡密钥认证。
 * @param[in] key_type 密钥类型，0x60 – KeyA；0x61 - KeyB。
 * @param[in] key 6字节密钥。
 * @param[in] serial 4个字节卡序列号。
 * @param[in] block 块号。
 * @return 状态码。
 */
int ddi_mifire_auth(uint8_t key_type, uint8_t key[6], uint8_t serial[4], uint8_t block);

/** M1卡读块数据。
 * @param[in] block 块号。
 * @param[out] data 当处理成功时返回该块数据。
 * @return 状态码。
 */
int ddi_mifire_read_block_data(uint8_t block,  uint8_t data[16]);

/** M1卡写块数据。
 * @param[in] block 块号。
 * @param[in] data 块数据。
 * @return 状态码。
 */
int ddi_mifire_write_block_data(uint8_t block, uint8_t data[16]);

/** M1卡增值操作。
 * @param[in] block 块号。
 * @param[in] data BCD格式增值数据。
 * @return 状态码。
 */
int ddi_mifire_add_value(uint8_t block, uint8_t data[6]);

/** M1卡减值操作。
 * @param[in] block 块号。
 * @param[in] data BCD格式减值数据。
 * @return 状态码。
 */
int ddi_mifire_sub_value(uint8_t block, uint8_t data[6]);

/** M1卡恢复块数据。
 * @param[in] src_block 源块号。
 * @param[in] dst_block 目的块号。
 * @return 状态码。
 */
int ddi_mifire_restore_block_value(uint8_t src_block, uint8_t dst_block);

/** M1卡读value块。
 * @param[in] block 块号。
 * @param[out] value 4个字节value 值。
 * @param[out] user_data_buf 用户数据buffer，可有可无。
 * @param[in,out] user_data_buf_len 输入表示buffer大小，输出表示读取到的用户数据长度。
 * @return 状态码。
 */
int ddi_mifire_read_value_block(uint8_t block, uint32_t *value, uint8_t* user_data_buf, uint16_t *user_data_buf_len);

/** M1卡写value块。
 * @param[in] block 块号。
 * @param[in] value 4字节value值，高位在前。
 * @return 状态码。
 */
int ddi_mifire_write_value_block(uint8_t block, uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
