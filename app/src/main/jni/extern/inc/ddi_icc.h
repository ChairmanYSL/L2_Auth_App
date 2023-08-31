#ifndef _DDI_RF_
#define _DDI_RF_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/** IC卡非接检测。
 * @param[out] card_state 8字节卡状态。
 * @param[out] picc_type 非接卡类型，0x00 – CPU卡；0x01 – M1卡；0x02 – M2卡。
 * @param[out] csn 4字节CSN。
 * @return 状态码。
 */
int ddi_icc_check(uint8_t card_state[8], uint8_t *picc_type, uint8_t csn[4]);

/** IC卡非接上电。
 * @param[in] slot 卡slot，0x00 IC1；0x01PSAM1；0x02PSAM2；0x03 PSAM3；0x04 非接触卡。
 * @param[in] card_type 卡类型，0x00 RF  CPU；0x01 RF  MIFARE卡。
 * @param[out] atr 复位数据，最长99字节，十进制。
 * @param[out] atr_len 复位数据长度。
 * @param[out] ic_card_type IC卡类型，0x01   CPU卡；0x02   4428；	0x03   4442；	0x04   SAM；	0x05   PSAM。
 * @return 状态码。
 */
int ddi_icc_power_on(uint8_t slot,  uint8_t card_type, uint8_t *atr, uint8_t *atr_len, uint8_t *ic_card_type);

/** IC卡非接下电。
 * @param[in] card_type 卡类型，0x00 IC1；0x01 SAM1；0x02 SAM2；0x03 SAM3；0x04非接触卡。
 * @return 状态码。
 */
int ddi_icc_power_off(uint8_t card_type);


/**
 * 打开IC卡非接卡读卡器。
 * @param[in] slot 卡槽。
 * @return 状态码。
 */
int ddi_icc_open(uint8_t slot);

/**
 * 关闭IC卡非接卡读卡器。
 * @param[in] slot 卡槽。
 * @return 状态码。
 */
int ddi_icc_close(uint8_t slot);

/**
 * 读取IC卡非接卡读卡状态，立即返回。
 * @param[in] slot 卡槽。
 * @param[out] buf 当前状态。
 * @return 状态码。
 */
int ddi_icc_read_card_status(uint8_t slot, uint8_t buf[6]);

/** 非接移卡。
 * @param[in] card_type 卡类型，0x00 IC1；0x01 SAM1；0x02 SAM2；0x03 SAM3；0x04非接触卡。
 * @return 状态码。
 */
int ddi_icc_rf_remove();

/** IC卡非接通讯，用于透传APDU到IC卡或RF卡。
 * @param[in] thiz SpCommClient对象。
 * @param[in] card_type 卡类型。0x00 IC1；0x01 SAM1；0x02 SAM2；0x03 SAM3；0x04 非接触式CPU卡；0x05 M1。
 * @param[in] send_apdu APDU指令数据（M1卡的数据格式需要细分）。
 * @param[in] send_apdu_len APDU指令数据长度。
 * @param[out] recv_apdu 应答apdu数据。
 * @param[out] recv_apdu_len 应答apdu数据长度。
 * @return 状态码。
 */
int ddi_icc_trans_apdu(uint8_t card_type, uint8_t *send_apdu, uint16_t send_apdu_len, uint8_t* recv_apdu, uint16_t* recv_apdu_len);

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#endif