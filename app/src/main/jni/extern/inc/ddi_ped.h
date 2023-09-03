#ifndef _DDI_PED_
#define _DDI_PED_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _TPKeyBoardInfo
{
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;
    int keyBoardHeight;
    int keyBoardWidth;
    int xGap;
    int yGap;
    int keyBoardRowNum;
    int keyBoardColNum;
} TPKeyBoardInfo;

typedef struct
{

    uint8_t m_keygroupidx;      /*密钥组号*/
    uint8_t m_srckeytype;       /*发散该密钥的源密钥的密钥类型，不得低于dstkeytype所在的密钥级别*/
    uint8_t m_srckeyindex;      /*源密钥索引号*/
    uint8_t m_dstkeytype;       /*目的密钥类*/
    uint8_t m_dstkeyindex;      /*目的密钥索引号*/
    uint32_t m_dstkeylen;       /*目的密钥数据长度*/
    uint8_t m_dstkeyvalue[256]; /*目的密钥内容*/

} ST_PED_KEY_INFO;

typedef struct
{

    uint8_t m_checkMode;      /*校验模式*/
    uint32_t m_checkdatalen;  /*校验数据长度*/
    uint8_t m_checkdata[256]; /*校验数据缓冲区*/

} ST_PED_KCV_INFO;

typedef struct
{

    uint32_t m_pinblockdatalen;  /*pinblock数据长度*/
    uint8_t m_pinblockdata[512]; /*pinblock数据缓冲区*/
    uint8_t m_ksnlen;            /*ksn len*/
    uint8_t m_ksn[32];           /*ksn数据缓冲区(HEX格式KSN)*/

} ST_PED_PINBLOCKDATA;

typedef struct
{

    uint32_t m_macdatalen;       /*macblock数据长度*/
    uint8_t m_macblockdata[256]; /*macblock数据缓冲区*/
    uint8_t m_ksnlen;            /*ksn len*/
    uint8_t m_ksn[32];           /*ksn数据缓冲区(HEX格式KSN)*/
} ST_PED_MACDATA;

typedef struct
{

    uint32_t m_accountdatalen;       /*macblock数据长度*/
    uint8_t m_accountblockdata[256]; /*macblock数据缓冲区*/
    uint8_t m_ksnlen;                /*ksn len*/
    uint8_t m_ksn[32];               /*ksn数据缓冲区(HEX格式KSN)*/
} ST_PED_ACCOUNTDATA;

/** 下载主密钥。
 * @param[in] m_key_type 主密钥类型 0:TMK；1:DUKPTK。
 * @param[in] alg_type 主密钥下载算法类型,0 – 算法1，大趋主密钥下载方法；1: 算法2：云POS主密钥下载方案。
 * @param[in] data_buffer 主密钥索引号+24个字节主密钥+从SP中获取到的16字节随机数+CRC-CCITT(2字节)，总共44字节数据。
 * @param[out] sp_m_key_type 主密钥类型,  0:TMK；1:DUKPTK；2:用户密钥；3:传输密钥。TODO 为什么跟传入的m_key_type不一致。
 * @param[out] check_type 校验位类型。
 * @param[out] check_value 校验位。
 * @return 状态码。
 */
int ddi_ped_load_main_key(uint8_t m_key_type, uint8_t alg_type, uint8_t data_buffer[44], uint8_t *sp_m_key_type, uint8_t *check_type, uint8_t check_value[4]);

/** 下载工作密钥。
 * @param[in] m_key_index 主密钥索引号(0-9)。
 * @param[in] w_key_index 工作密钥索引。
 * @param[in] w_key_type 工作密钥类型，0x00 – TPK；0x01 – TAK；0x02 – TDK。
 * @param[in] alg_type 工作密钥加密校验算法，0x00 – 主密钥加密，4字节校验位采用明文对8个字节0加密取前8个字节；0x01 – 没有check value；0x02 – 明文密钥。
 * @param[in] key_len 密钥数据长度。
 * @param[in] key_data 密钥数据。
 * @return 状态码。
 */
int ddi_ped_load_work_key(uint8_t m_key_index, uint8_t w_key_index, uint8_t w_key_type, uint8_t alg_type, uint8_t key_len, uint8_t *key_data);

/** 用户密钥DES加密。
 * @param[in] key_type 密钥体系类型，固定填 0x02。
 * @param[in] w_key_index 工作密钥索引。
 * @param[in] w_key_type 用户密钥类型。
 * @param[in] des_alg_type DES算法类型，ALG_DES(0x04)或ALG_3DES(0x05)。
 * @param[in] plain_len 待加密数据长度，长度必须为8的倍数。
 * @param[in] plain 待加密数据。
 * @param[out] enc_data_buf 用于存储加密后的数据buffer。
 * @param[in,out] enc_data_buf_len 输入表示buffer大小，输出表示加密后的数据长度。
 * @return 状态码。
 */
int ddi_ped_des_enc_by_work_key(uint8_t key_type, uint8_t w_key_index, uint8_t w_key_type,
                                uint8_t des_alg_type, uint8_t plain_len, uint8_t *plain, uint8_t *enc_data_buf, uint8_t *enc_data_buf_len);

/** 获取随机密码键盘。
 * @param[in] pk 加密公钥。
 * @param[out] enckeys 随机密码键盘用公钥加密结果。
 * return 状态码。
 */
int ddi_ped_get_rand_keys(uint8_t pk[256], uint8_t enckeys[256]);

/** 获取PINBLOCK。
 * @param[in] w_key_index 工作密钥索引号(0-9)。
 * @param[in] key_mode  Key Mode，0x01 – DES；0x02 - DUKPTK
 * @param[in] alg_type 加密算法类型，0x00 – X9.8。
 * @param[in] pinblock_mode pinblock模式，0x00 – ISO9564 -0，0x01 – ISO9564 -1，0x02– ISO9564 - 3。
 * @param[in] pan 16个字节 PAN。
 * @param[in] pwd 持卡人密码顺序。
 * @param[out] pinblock_buf 用来存储pinblock的buffer。
 * @param[out] pinblock_len 输入表示pinblock buffer的长度，输出表示pinblock数据长度。
 * return 状态码。
 */
int ddi_ped_get_pinblock(uint8_t w_key_index, uint8_t key_mode, uint8_t alg_type, uint8_t pinblock_mode,
                         uint8_t pan[16], uint8_t *pwd, uint8_t *pinblock_buf, uint8_t *pinblock_len);

/** 计算MAC。
 * @param[in] key_type 密钥体系类型,固定为0x02。
 * @param[in] mac_key_idex MAC密钥索引(0-9)。
 * @param[in] w_key_type 用户密钥类型。
 * @param[in] mac_alg_type MAC算法类型。
 * @param[in] plain_len 算MAC数据的长度，高位在前。
 * @param[in] plain 算MAC数据。
 * @param[out] enc_data_buf 用于存储mac数据的buffer。
 * @param[in,out] enc_data_buf_len 输入表示buffer的长度，输出表示计算后的mac数据长度。
 * @return 状态码。
 */
int ddi_ped_calc_mac(uint8_t key_type, uint8_t mac_key_idex, uint8_t w_key_type, uint8_t mac_alg_type,
                     uint16_t plain_len, uint8_t *plain, uint8_t *enc_data_buf, uint8_t *enc_data_buf_len);

/** 删除主密钥。
 * @param[in] m_key_index 主密钥索引号(0-9)，0xFF – 删除所有密钥。
 * @return 状态码。
 */
int ddi_ped_remove_m_key(uint8_t m_key_index);

/** 格式化密钥区。
 * @return 状态码。
 */
int ddi_ped_format();

/** 获取21号文件中定义的59域的内容
 * @param[in] encrypt_factor 加密随机因子。
 * @param[in] data_len 加密随机因子的长度。
 * @param[out] result 返回结果。
 * @param[in,out] result_len 结果长度。
 * @return 状态码。
 */
int ddi_ped_get_field59_data(uint8_t *encrypt_factor, uint16_t encrypt_factor_len, uint8_t *result, uint16_t *result_len);

/** 获取明文硬件序列号
 * @param[out] result 返回结果。
 * @param[in,out] result_len 结果长度。
 * @return 状态码。
 */
int ddi_ped_get_plain_serial_no(uint8_t *result, uint16_t *result_len);

/** 获取密文硬件序列号
 * @param[in] encrypt_mode 加密模式 des/3des :0  SM4 :1
 * @param[in] plain_serial_no 硬件序列号。
 * @param[in] plain_serial_no_len 序列号长度。
 * @param[in] encrypt_factor 加密随机因子。
 * @param[in] data_len 加密随机因子的长度。
 * @param[out] result 返回结果。
 * @param[in,out] result_len 结果长度。
 * @return 状态码。
 */
int ddi_ped_get_encrypt_serial_no(uint16_t encrypt_mode, uint8_t *plain_serial_no, uint16_t serial_no_len, uint8_t *encrypt_factor, uint16_t encrypt_factor_len, uint8_t *result, uint16_t *result_len);

/**
 * 下发sn序列号&&sn key
 * @param download_type 0：明文  1：用传输密钥加密
 * @param serial_no 序列号
 * @param serial_no_len 序列号长度
 * @param serial_key 序列号key
 * @param serial_key_len 序列号key len
 * @param check_value 校验值
 * @return 状态码
 */
int ddi_ped_download_serial_key(uint16_t download_type, uint8_t *serial_no, uint16_t serial_no_len, uint8_t *serial_key, uint16_t serial_key_len, uint8_t *check_value);

int ddi_ped_pci_cardhorderpininput(int32_t pinlenmin, int32_t pinlenmax, int32_t timeout, int32_t bypassflag);

int ddi_ped_pci_getpinblock(int32_t command, uint8_t *keydata, int32_t keydatalen, uint8_t *pinblock, int32_t pinblockbuflen);

int ddi_ped_pci_getmac(int32_t command, uint8_t *keydata, int32_t keydatalen, uint8_t *macblock, int32_t macblockbuflen);
int ddi_ped_pci_getencryptedtrackdata(int32_t command, uint8_t *keydata, int32_t keydatalen, uint8_t *encryptdtrackdata, int32_t encryptedmagdatabuflen);

int ddi_ped_pci_getkeystatus(uint8_t groupindex, uint8_t keytype, uint8_t keyindex, uint8_t *tradedata, int32_t tradedatalen);

int ddi_ped_pci_getcardholderinputpin(int32_t command, uint8_t *pindata, int32_t pindatabuflen);

int ddi_ped_pci_generatetransdukptkey(uint8_t groupindex, uint8_t keyindex, uint8_t dukpttype);

int ddi_ped_pci_readtlsfile(uint8_t *tlsfilename, uint8_t *tlsfiledata, int32_t tlsfiledatalen);

int ddi_ped_pci_getrandomkeypadlayout(uint8_t *keypaddom, uint8_t keypaddombuflen);

int ddi_ped_pci_settpkeyboardinfo(TPKeyBoardInfo *sptpkeyboardinfo);

int ddi_ped_writekey(ST_PED_KEY_INFO *keyinfoin, ST_PED_KCV_INFO *kcvinfoin);

int ddi_ped_writeTIK(uint8_t ksnlenin, uint8_t *ksnin, ST_PED_KEY_INFO *keyinfoin, ST_PED_KCV_INFO *kcvinfoin);

int ddi_ped_entercardholderpininputmode(uint8_t keygroupindex, uint8_t pinkeytype, uint8_t pinkeyindex, uint8_t mode, uint8_t bypassenable,
                                        uint8_t minpinlen, uint8_t maxpinlen, uint32_t timeoutms);

int ddi_ped_exitcardholderpininputmode();

int ddi_ped_getpinblock(uint8_t keygroupidx, uint8_t pinkeytype, uint8_t pinkeyindex, uint8_t mode, uint8_t derivekeytype, uint32_t datainlen,
                        uint8_t *datain, ST_PED_PINBLOCKDATA *pinblockdataout);

int ddi_ped_getmac(uint8_t keygroupidx, uint8_t mackeytype, uint8_t mackeyindex, uint8_t mode, uint8_t derivekeytype, uint8_t derivekeyusage,
                   uint32_t datainlen, uint8_t *datain, ST_PED_MACDATA *macdataout);

int ddi_ped_encryptaccountdata(uint8_t keygroupidx, uint8_t tdkkeytype, uint8_t tdkkeyindex, uint8_t mode, uint8_t derivekeytype,
                               uint8_t derivekeyusage, uint32_t datainlen, uint8_t *datain, ST_PED_ACCOUNTDATA *accountdataout);

int ddi_ped_regentdesdukptkey(uint8_t keygroupidx, uint8_t tdesdukptkeyindex, uint8_t mode, uint32_t datainlen, uint8_t *datain);

int ddi_ped_regenaesdukptkey(uint8_t keygroupidx, uint8_t aesdukptkeyindex, uint8_t mode, uint32_t datainlen, uint8_t *datain);

int ddi_ped_getkeykcv(uint8_t keygroupidx, uint8_t keytype, uint8_t keyindex, ST_PED_KCV_INFO *kcvinfoout);

int ddi_ped_getpinkeyvalue(uint8_t *pindata, int32_t pindatabuflen);

#ifdef __cplusplus
}
#endif

#endif