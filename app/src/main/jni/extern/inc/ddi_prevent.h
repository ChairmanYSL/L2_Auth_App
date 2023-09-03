#ifndef _DDI_PREVENT_
#define _DDI_PREVENT_

#ifdef __cplusplus
extern "C"
{
#endif

/** 向SP发送获取防切机信息指令。
 * @param[out] sp_cpu_id SP CPU ID，32个字节，不足补0x00。
 * @param[out] pos_sn pos sn号，32个字节不足补0x00。
 * @param[out] rand 16个字节随机数。
 * @return 状态码。
 */
int ddi_prevent_get_avoid_replace_info(uint8_t sp_cpu_id[32], uint8_t pos_sn[32], uint8_t rand[16]);

/** 向SP发送获取防切机绑定数据及SP认证私钥。
 * @param[in] encrypt 加密数据，使用防切机私钥对防切机绑定数据进行加密。
 * @param[in] priv_key_len sp认证私钥信息长度，ID3-ID4的数据长度。
 * @param[in] priv_key_plain_len sp认证私钥明文信息长度。
 * @param[in] priv_key_enc_data sp认证私钥加密数据，使用16个字节随机数加密。
 * @param[in] enc_data_len sp认证私钥加密数据长度。
 * @return 状态码。
 */
int ddi_prevent_send_avoid_replace_binding_data(uint8_t encrypt[256], uint16_t priv_key_len, uint16_t priv_key_plain_len, uint8_t *priv_key_enc_data, uint16_t enc_data_len);

/** 读取防切机证书包hash。
 * @param[in] cert_publisher 证书发行者, 16个字节随机数。
 * @param[out] hash_info (64字节asc hash值+16个字节随机数)。
 * @return 状态码。
 */
int ddi_prevent_set_cert_package_info(uint8_t *cert_package_info, uint32_t cert_package_info_len);

/** 获取终端证书信息。
 * @param[in] cert_name 证书名称，"trendit"/"acquirer"。
 * @param[in] cert_name_len 证书名称长度。
 * @param[out] cert_publisher_and_version 证书发行者及版本号。
 * @return 状态码。
 */
int ddi_prevent_get_cert_package_info(uint8_t *cert_package_info, uint32_t *cert_package_info_len);


/** 工厂设置主密钥。
 * @param[in] alg_type 算法标识。
 * @param[in] key_index 密钥索引号。
 * @param[in] key_enc 主密钥TMK密文。
 * @param[in] ken_enc_len 密文长度。
 * @param[in] key_check_value 4字节主密钥校验值。
 * @param[out] result 返回结果。
 * @return 状态码。
 */
int ddi_prevent_upadta_factory_main_key(uint8_t alg_type, uint8_t key_index, uint8_t* key_enc, uint16_t key_enc_len, uint8_t key_check_value[4]);

/**
 * 获取sn key check value。
 * @param[out] snkeycheckvalue sn key check value buffer。
 * @param[out] snkeycheckvalue_len sn key check value len。
 * @return 状态码。
 */
int ddi_prevent_get_snkey_checkvalue(uint8_t *snkeycheckvalue, uint16_t *snkeycheckvalue_len);

#ifdef __cplusplus
}
#endif

#endif