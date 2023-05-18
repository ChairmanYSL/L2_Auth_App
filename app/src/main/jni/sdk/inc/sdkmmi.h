#ifndef AFX_SDK_SDKMMI_H
#define AFX_SDK_SDKMMI_H

#define SDK_MAX_STR_LEN             127                 //最大输入内容长度//the max input length
#define SDK_MAX_LINE_CHAR                               20                              //每行最大显示字符个数//the maximum number of display characters per line
#define SDK_KEY_BUF_SIZE                        128                                     //键盘输入缓冲的长度//the length of buffer of keyboard entry

/**************************** scanf 掩码定义scanf mask definition********************************/
#define SDK_MMI_NUMBER              0x00000001              //允许输入数字//allow enter number
#define SDK_MMI_SYMBOL              0x00000002              //输入特殊可见字符//enter special visible character
#define SDK_MMI_HEX                 0x00000004              //允许16进制//allow hex
#define SDK_MMI_LETTER              0x00000008              //允许输入字母//allow enter letter
#define SDK_MMI_HZ                  0x00000010              //允许输入汉字//allow enter Chinese characters
#define SDK_MMI_LEFT                0x00000020              //左对齐(为零则为右对齐)  //left-aligned(if 0 then right-aligned)
#define SDK_MMI_POINT               0x00000040              //输入带小数点数字(金额)  //enter numbers with a decimal point(amount)
#define SDK_MMI_PWD                 0x00000080              //输入密码(回显为*, 现在允许输入数字和字母-盲打(*^__^*) )//反显(为零则为正显)  //enter password(display *, now allow enter number and letter - touch type ) //reverse display(if 0 then normal display)
#define SDK_MMI_TIME                0x00000100              //输入时间//enter time
#define SDK_MMI_MULTTASK            0x00000200              //Function做多应用切换用-针对签到//Function is used to switch application - aim at login
#define SDK_MMI_DISP                0x00000400              //反显(为零则为正显)  //reverse display(if 0 then normal display)
#define SDK_MMI_STROKE              0x00000800              //笔画输入字符//STARTEND   //enter special visible characters
#define SDK_MMI_HAND                0x00001000              //手写输入
#define SDK_MMI_NOINPUT_QUIT        0x00002000              //允许在进入输入法之后没有输入字符前，直接确认退出
#define SDK_MMI_FARSI               0X00004000              //输入波斯文
#define SDK_MMI_RUSSIAN             0x00008000              //输入俄罗斯文
#define SDK_MMI_GEORGIA             0x00010000              //允许输入格鲁吉亚文
#define SDK_MMI_DEFAULT_STR   (SDK_MMI_NUMBER|SDK_MMI_LETTER|SDK_MMI_HZ)
//enum SDK_DISP_LINE;

/*********************************************************************
标准版Standard Version
**********************************************************************/
extern s32 sdkKbGetScanf(const s32 siOvertime , u8 *pheOut , const s32 siMinLen , const s32 siMaxLen , const u32 uiMode , const s32 siDispRow);
extern s32 sdkKbGetScanfEx(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen, const u32 uiMode, const s32 siDispRow, s32 (*psdkCallFun)(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow, void *pVar), void *pVarible, u32 siFirstMode);

#endif
