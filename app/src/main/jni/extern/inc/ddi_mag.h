#ifndef _DDI_MAG_
#define _DDI_MAG_

#ifdef __cplusplus
extern "C"
{
#endif

/** 开启读卡器，该命令用于打开磁头，允许读取磁条卡信息，判断是否插入IC卡。
 * @param[in] read_mode 读卡模式，0x01读取磁道信息；0x02 检查是否有IC卡插入；0x04检查是否有非接卡在天线区；可组合使用，如0x07为三种都等待。
 * @param[in] timeout 超时时间，1-255，单位秒。
 * @param[out] card_type 卡类型，0x01  刷卡结束；0x02  IC卡已插入；0x04  发现RF卡。
 * @return 状态码。
 */
int ddi_mag_open(uint8_t read_mode, uint8_t timeout, uint8_t *card_type);

/** 读磁条卡。
 * @param[in] read_mode 磁条信息读取模式，0x01 读取第一磁道数据，0x02 读取第二磁道数据，0x04 读取第三磁道数据，可组合使用。
 * @param[in] mask 主账号屏蔽掩码，帐号 BCD，0 与 F 分别表示需要显示或屏蔽的位。
 * @param[in] alg_type 算法类型，磁道密钥加密算法类型。
 * @param[in] key_index 密钥索引，密钥索引号。
 * @param[in] enc_import_data 加密导入数据，需要和磁道信息一起使用TDK进行加密的数据，最长32字节。
 * @param[in] enc_import_data_len 加密导入数据长度,十进制。
 * @param[out] ret_enc_mode
 * @param[out] ksn KSN号(暂定为全0)。
 * @param[out] rand 随机数，用于分散工作密钥的信息(暂定为全0)。
 * @param[out] pan 主账号，BCD格式，左靠，右补 F，中间有屏蔽部分用 E 屏蔽。
 * @param[out] yymm YYMM，有效期, 2 字节 BCD 压缩。
 * @param[out] channel1_len 一磁道长度，十六进制显示。
 * @param[out] channel2_len 二磁道长度，十六进制显示。
 * @param[out] channel3_len 三磁道长度，十六进制显示。
 * @param[out] enc_channel23 二三磁道加密数据，根据磁道信息读取模式组合加密，均 BCD 压缩，奇数补 F。
 * @param[out] enc_channel23_len 二三磁道加密数据长度，十进制。
 * @param[out] enc_channel1 一磁道加密数据。
 * @param[out] enc_channel1_len 一磁道加密数据，十进制。
 * @param[out] validity_code_channel2 二磁的有效期和服务代码，包含 4 位有效期和 3 位服务代码, 当 3 位服务代码的第一位的值为“2”或者 “6” 时表示该卡可能存在 IC 卡。
 * @param[out] other_enc 其他加密数据。
 * @param[out] other_enc_len 其他加密数据长度。
 * @return 状态码。
 */
int ddi_mag_read_card_plain(uint8_t  read_mode, uint8_t mask[10], uint8_t alg_type,
                                    uint8_t key_index, uint8_t *enc_import_data, uint16_t enc_import_data_len,
                                    uint8_t *ret_enc_mode, uint8_t ksn[8], uint8_t rand[8], uint8_t pan[10],
                                    uint8_t yymm[2], uint8_t *channel1_plain, uint8_t *channel1_len,
                                    uint8_t *channel2_plain, uint8_t *channel2_len, uint8_t *channel3_plain,
                                    uint8_t *channel3_len, uint8_t validity_code_channel2[7],
                                    uint8_t *other_enc, uint16_t *other_enc_len);

int ddi_mag_read_card(uint8_t read_mode, uint8_t mask[10], uint8_t alg_type,
                                 uint8_t key_index, uint8_t *enc_import_data, uint16_t enc_import_data_len,
                                 uint8_t *ret_enc_mode, uint8_t ksn[8], uint8_t rand[8], uint8_t pan[10],
                                 uint8_t yymm[2], uint8_t *channel1_len, uint8_t *channel2_len, uint8_t *channel3_len,
                                 uint8_t *enc_channel23, uint16_t *enc_channel23_len, uint8_t *enc_channel1,
                                 uint16_t *enc_channel1_len, uint8_t validity_code_channel2[7],
                                 uint8_t *other_enc, uint16_t* other_enc_len);

/** 关闭读卡器。用于关闭各个读卡器。
 * @param[in] close_mode 关闭类型。0x01关闭磁头；0x02关闭IC卡读卡器；0x04关闭RF卡读卡器；可组合使用，如0x07为三种都关闭。
 * @return 状态码。
 */
int ddi_mag_close(uint8_t close_mode);

/**
 * 打开磁条卡读卡器。
 * @return 状态码。
 */
int ddi_mag_open_only();

/**
 * 关闭磁条卡读卡器。
 * @return 状态码。
 */
int ddi_mag_close_only();

/**
 * 读取磁条卡读卡状态，立即返回。
 * @param[out] buf 当前状态。
 * @return 状态码。
 */
int ddi_mag_read_card_status(uint8_t buf[4]);


#ifdef __cplusplus
}
#endif
#endif