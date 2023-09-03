#ifndef _DDI_FIRMWARE_
#define _DDI_FIRMWARE_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _FirmwareDownloadCallback
{
    void (*onStart)(char *name);

    void (*onProgress)(char *name, unsigned int progress);

    void (*onFail)(char *name, char *err_msg);

    void (*onSuccess)(char *name);

} FirmwareDownloadCallback;

int ddi_firmware_get_terminal_cert_info(uint8_t *cert_name, uint8_t cert_name_len, uint8_t cert_publisher_and_version[48]);

/** 发送固件升级数据。
 * @param[in] file_count 文件个数。
 * @param[in] file_name 文件名称。
 * @param[in] file_size 文件总长度。
 * @param[in] file_crc 整个文件的CRC。
 * @param[in] file_offset 文件偏移。
 * @param[in] file_data 当前文件数据。
 * @param[in] data_len 当前文件数据长度。
 * @param[out] error_buf 错误信息提示。
 * @param[out] error_info_len 错误信息长度。
 * @return 状态码。
 */
int ddi_firmware_send_firmware_update_data(uint8_t file_count, uint8_t file_name[51], uint32_t file_size,
                                           uint32_t file_crc, uint32_t file_offset, uint8_t *file_data, uint16_t data_len,
                                           uint8_t error_buf[256], uint8_t *error_info_len);

/** SP进入固件升级模式。发送这条指令时，SP可能处于core状态，需要重启后判断标志进入boot模式。AP要每隔50ms发送一次这条指令，直到收到成功响应。
 * @return 状态码。
 */
int ddi_firmware_enter_update_mode();

/** 清除下载标志并退出下载状态。
 * @return 状态码。
 */
int ddi_firmware_clear_download_flag_and_exit_download_mode();

int ddi_firmware_download_boot(char *boot_path, FirmwareDownloadCallback *callback);

/**
 * 下载除boot以外的其他固件，下载前需要调用ddi_firmware_enter_download_mode，结束后需要调用ddi_firmware_exit_download_mode
 * @param firmware_path 
 * @param callback 
 * @return int 
 */
int ddi_firmware_download(char *firmware_path, FirmwareDownloadCallback *callback);

int ddi_firmware_enter_download_mode();

int ddi_firmware_exit_download_mode();

int ddi_firmware_download_start();

int ddi_firmware_download_end();

#ifdef __cplusplus
}
#endif

#endif