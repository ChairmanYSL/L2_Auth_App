
#ifndef SDK_TOOLS_H
#define SDK_TOOLS_H

#define SDK_SYS_RET_ERR_MAX          (-100)     //sdk内部自定义错误必须小于这个//sdk interior user-defined error must be smaller than this
#define SDK_ICC_EMV_ERR_BASE         (SDK_SYS_RET_ERR_MAX - 100)     //(-200)
#define SDK_DISP_ERR_BASE            (SDK_ICC_EMV_ERR_BASE - 200)
#define SDK_APP_RET_ERR_MAX          (-10000)   //用户自定义必小于这个//user-defined must be smaller than this
#define  SDK_SYS_RET_OK_MIN          (100)      //sdk内部自定义必大于这个//sdk interior user-defined must be bigger than this
#define  SDK_SYS_CONTINUE            (SDK_SYS_RET_OK_MIN + 1)
#define  SDK_SYS_NEXT                (SDK_SYS_CONTINUE + 1)

#define  SDK_ICC_EMV_OK_BASE         (SDK_SYS_RET_OK_MIN + 100)
#define  SDK_COMM_OK_BASE            (SDK_ICC_EMV_OK_BASE + 200)
#define  SDK_PRINT_OK_BASE           (SDK_COMM_OK_BASE + 200)


enum SDK_FILE_RSLT
{
    SDK_FILE_NONE = SDK_SYS_RET_ERR_MAX + (-5),                     //文件不存在或者打开失败//file does not exist or open file failed
    SDK_FILE_CRCERR = SDK_SYS_RET_ERR_MAX + (-4),                   // 文件校验错//check file error
    SDK_FILE_ERROR = SDK_SYS_RET_ERR_MAX + (-3),                    // 文件打开或创建失败//open or create file failed
    SDK_FILE_SEEK_ERROR = SDK_SYS_RET_ERR_MAX + (-2),               // 写文件定位越界//write file positioning cross the border
    SDK_FILE_EOF = SDK_SYS_RET_ERR_MAX + (-1),                      // 读操作时碰到文件尾//get end-of -file when writing
    SDK_FILE_OK = SDK_SYS_RET_OK_MIN + (1)                         // 文件操作成功//file operations success
};

enum SDK_ALIGN_MODE
{
    SDK_LEFT = SDK_SYS_RET_OK_MIN + 1,                              // 左边//left
    SDK_RIGHT = SDK_SYS_RET_OK_MIN + 2,                             // 右边//right
    SDK_BOTH = SDK_SYS_RET_OK_MIN + 3                              // 两边//both sides
};                                                                  // 删除的空格位置或填补后的对齐方式AlignMode(对齐方式)：//the place of deleted bland space or align mode after filling

extern void sdkmSleep(const s32 siMs);
extern s32 sdkGetRandom(u8 *pheRdm, s32 siNum);
extern bool sdkDf(const u8 *pasDiskCata, s32 *psiAvailSize, s32 *psiUsedSize, s32 *psiUsedRate);

extern s32 sdkReadPosSn(u8 *pasDest);
extern s32 sdkGetRtc(u8 *pbcDest);
extern s32 sdkSetRtc(const u8 *pbcSrc);


//石玮松 2012.08.09 15:35  定时器请使用下面两个函数//functions about timer
extern bool sdkTimerIsEnd();
extern void sdkTimerStar(unsigned int uiMs);
extern const u8* sdkTimerGetAsc(void);

extern s32 sdkInputIp(s32 siDsipLine, const u8 * pAsPrompt, s32 siInputLine, u8 *pAsIP, u8 uAlign, s32 siTimeOut);

/*****************************其他接口函数调用透出Other functions call these functions****************************/
extern bool sdkJudgeTime(const u8 *pbcTime, s32 siBcdLen);
//extern bool sdkIsVisibleStr(u8 const *pasSrc, s32 siSrclen);
extern bool sdkIsBcdNum(u8 const *pheSrc, s32 siSrclen);
extern s32 sdkTruncateChar(const u8 *pasSrc, s32 siAvailableChar);
extern s32 sdkIniWriteKey(const u8 *pasFile, const u8 *pasSection, const u8 *pasKey, const u8 *pheValue);
extern s32 sdkIniReadKey(const u8 *pasFile, const u8 *pasSection, const u8 *pasKey, u8 *pheValue);

//extern s32 sdkToolsGetVersion(u8 *pasDest);  //fusuipu modefied at 2013-1-29
extern s32 sdkStrpos(const u8 *pasStr, const u8 asChr);
extern s32 sdkTrim(u8 *pasStr, enum SDK_ALIGN_MODE eMode);
extern s32 sdkIniTrimBracket(u8 *pasStr, enum SDK_ALIGN_MODE eMode);
extern s32 sdkRegulateAmount(u8 *pasDest, const u8 *pbcAmount);
extern s32 sdkFormBlankAlignChar(enum SDK_ALIGN_MODE eMode, u8 *pasDest, const u8 *pasSrc, s32 siAvailableChar);
extern s32 sdkCmpYear(u8 bcYear1, u8 bcYear2);
extern bool sdkIsAsciiNum(u8 const *pheSrc);
extern s32 sdkDelFileData(const u8 *pasFile, s32 iStart, s32 iDelLen);
extern s32 sdkSplitStr(u8 *pheDest, u8 const *pheSrc, s32 siSrartPos, s32 siDestLen);
extern s32 sdkSavePosSn(const u8 *pasSrc);
extern s32 sdkGetBmpSize(s32 *pWidth, s32 *pHeight, const u8 *pBmpFile);

#endif
