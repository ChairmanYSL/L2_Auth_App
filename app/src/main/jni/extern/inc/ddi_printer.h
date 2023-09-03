#ifndef _DDI_PRINTER_
#define _DDI_PRINTER_

#ifdef __cplusplus
extern "C"
{
#endif

/** 打印机状态。
 */
typedef enum PrinterStatus {
    /** 打印机空闲。*/
    PRINTER_STATUS_FREE = 0x00,
    /** 打印机正在打印。*/
    PRINTER_STATUS_WORKING = 0x01,
    /** 打印机缺纸。*/
    PRINTER_STATUS_OUT_OF_PAPER = 0x02,
    /** 打印机过热。*/
    PRINTER_STATUS_OVER_HEAT = 0x03,
    /** 打印机故障。*/
    PRINTER_STATUS_FAULT = 0x04,
    /** 其他故障。*/
    PRINTER_STATUS_OTHER_ERROR = 0x05,
} PrinterStatus;

/** 打开打印机。
 * @param[out] printer_type 打印机类型，0x00 –热敏；0x01 – 针打。
 * @param[out] pointers_per_line 打印机一行点数。
 * @return 状态码。
 */
int ddi_printer_open(uint8_t *printer_type, uint16_t *pointers_per_line);

/** 关闭打印机。
 * @return 状态码。
 */
int ddi_printer_close();

/** 走纸。
 * @param[in] pointers 走纸点数数。
 * @return 状态码。
 */
int ddi_printer_feed_paper(uint32_t pointers);

/** 打印图形。
 * @param[in] pic_data 图片数据。
 * @param[in] data_len 数据长度。
 * @return 状态码。
 */
int ddi_printer_print_picture(uint8_t *pic_data, uint16_t data_len);

/** 设置打印参数。
 * @param[in] param_type 参数类型。
 * @param[in] param 打印参数。
 * @param[in] param_len 打印参数长度。
 * @return 状态码。
 */
int ddi_printer_set_parameter(uint8_t param_type, uint8_t *param, uint8_t param_len);

/** 获取打印状态。
 * @return PrinterStatus枚举值。
 */
PrinterStatus ddi_printer_get_status();

#ifdef __cplusplus
}
#endif

#endif