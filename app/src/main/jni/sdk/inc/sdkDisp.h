#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



#ifndef SDK_IO_H
#define SDK_IO_H



#define SDK_DISP_LINE1              0                                              //显示行号1  //display line number 1
#define SDK_DISP_LINE2              1                                              //显示行号2  //display line number 2
#define SDK_DISP_LINE3              2                                              //显示行号3  //display line number 3
#define SDK_DISP_LINE4              3                                              //显示行号4  //display line number 4
#define SDK_DISP_LINE5              4                                              //显示行号5  //display line number 5
#define SDK_DISP_LINE_MAX           5                                              //显示行号最大数//the max line number of display

#define SDKDISPMAXLINENO            5
#define SDKDISPMAXLINELEN           20

#define BIT_0     (0x1ul << 0)
#define BIT_1     (0x1ul << 1)
#define BIT_2     (0x1ul << 2)
#define BIT_3     (0x1ul << 3)
#define BIT_4     (0x1ul << 4)
#define BIT_5     (0x1ul << 5)
#define BIT_6    (0x1ul << 6)
#define BIT_7     (0x1ul << 7)
#define BIT_8     (0x1ul << 8)
#define BIT_9     (0x1ul << 9)
#define BIT_10   (0x1ul << 10)
#define BIT_11     (0x1ul << 11)
#define BIT_12     (0x1ul << 12)
#define BIT_13     (0x1ul << 13)
#define BIT_14     (0x1ul << 14)
#define BIT_15     (0x1ul << 15)
#define BIT_16     (0x1ul << 16)
#define BIT_17     (0x1ul << 17)
#define BIT_18     (0x1ul << 18)
#define BIT_19     (0x1ul << 19)
#define BIT_20     (0x1ul << 20)
#define BIT_21     (0x1ul << 21)
#define BIT_22     (0x1ul << 22)
#define BIT_23     (0x1ul << 23)
#define BIT_24     (0x1ul << 24)
#define BIT_25     (0x1ul << 25)
#define BIT_26     (0x1ul << 26)
#define BIT_27     (0x1ul << 27)
#define BIT_28     (0x1ul << 28)
#define BIT_29     (0x1ul << 29)
#define BIT_30     (0x1ul << 30)
#define BIT_31     (0x1ul << 31)


/**************************** 显示属性定义Display Attribute Definition********************************/
#define SDK_DISP_FDISP              BIT_0                                          //正显//normal display
#define SDK_DISP_NOFDISP            0x00                                           //反显//revert display
#define SDK_DISP_INCOL              BIT_1                                           //插入一列//insert a row
#define SDK_DISP_LDISP              BIT_2                                           //左对齐//flush left
#define SDK_DISP_CDISP              BIT_3                                           //居中//center
#define SDK_DISP_RDISP              BIT_4                                           //右对齐//flush right
#define SDK_DISP_NOFDISPLINE        BIT_5                                           //反显//revert display
#define SDK_DISP_CTRLA              BIT_7                                           //当本行字符满一行时，后跟有换行'\xa'或回车'\xd'字符,CTRLA取作用,当该位为0时不起作用//when a line is full of characters, behind with new line character '\xa' or enter character '\xd' and CTRLA is usefull, if this bit is 0 then useless
#define SDK_DISP_DEFAULT            (SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL)   //正显居中//normal display, center
#define SDK_DISP_LEFT_DEFAULT       (SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL)   //左对齐//flush left
#define SDK_DISP_RIGHT_DEFAULT      (SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL)   //右对齐//flush right

#define SDK_DISP_OVERLINE BIT_8   //上面有标记横线
#define SDK_DISP_DOWNLINE BIT_9   //下面有标记横线
#define SDK_DISP_SIDELINE BIT_10   //左右有框线

/**************************************错误码Error Code*************************************/
#define SDK_DISP_SIZEERRO           -101                                           //缓冲大小必须大于(屏幕宽x 屏幕高x4)  //the buffer must be bigger than (screen wide multiply screen high multiply 4)
#define SDK_DISP_MULTI_OVER_DISP    (-400 - 1) //???????????????                        //组合显示内容超过一行
/**************************颜色(单色屏)Color(monochrome screen)****************************************/

typedef struct
{
    s32 siX;                                                                       //宽//wide
    s32 siY;                                                                       //高//high
}SDK_DISP_PIXEL;                                                                    /* 屏幕像素 screen pixel*/

typedef enum
{
    SDK_DISP_BATTERY = 1,                               //电池图标
    SDK_DISP_PLUG,                                      //电源图标
    SDK_DISP_SIGNAL,                                    //信号图标
    SDK_DISP_GPRS,                                      //GPRS图标
    SDK_DISP_CDMA,                                      //CDMA图标
    SDK_DISP_WIFI,                                      //WIFI图标
    SDK_DISP_BT,                                 //蓝牙图标
	SDK_DISP_AUDIO,                                     //??????
    SDK_DISP_USB,                                     //USB????
} SDK_DISP_ICON;


typedef struct
{
     s32 siOverNode;                                    //超过一行显示的字符串
     s32 siStartByte;                                   //超行显示的字符串起始节点
     void *pVar;                                        //保留参数指针
}SDK_DISP_MULT_RET_INFO;

typedef struct{
	u8 fileName[50];
	u32 Offset;
	u32 len;
	u32 high;
}SDK_FONT;//zhouzhihua 2013.06.19 11:4


typedef struct                          //矩阵框格式
{
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
}SDK_DISP_SRECT;


typedef enum
{
    SDK_DISP_FONT_SMALL   = 12,
    SDK_DISP_FONT_NORMAL  = 16,
    SDK_DISP_FONT_BIG     = 24,
    SDK_DISP_FONT_LARGE   = 32,
    SDK_DISP_FONT_HUGE    = 48
}SDK_DISP_FONT;

typedef enum
{
    SDK_BLACK_COLOR_ID   = 0,
    SDK_YELLOW_COLOR_ID,
    SDK_LGREEN_COLOR_ID,
    SDK_BLUE_COLOR_ID,
    SDK_GREEN_COLOR_ID,
    SDK_RED_COLOR_ID,
    SDK_WINDOWS_BLUE_ID,
    SDK_WHITE_COLOR_ID
}SDK_COLOR_ID;


#define    SDK_DISP_BLACK_COLOR     sdkSysGetColorValue(SDK_BLACK_COLOR_ID)
#define    SDK_DISP_YELLOW_COLOR    sdkSysGetColorValue(SDK_YELLOW_COLOR_ID)
#define    SDK_DISP_LGREEN_COLOR    sdkSysGetColorValue(SDK_LGREEN_COLOR_ID)
#define    SDK_DISP_BLUE_COLOR      sdkSysGetColorValue(SDK_BLUE_COLOR_ID)
#define    SDK_DISP_GREEN_COLOR     sdkSysGetColorValue(SDK_GREEN_COLOR_ID)
#define    SDK_DISP_RED_COLOR       sdkSysGetColorValue(SDK_RED_COLOR_ID)
#define    SDK_DISP_WINDOWS_BLUE    sdkSysGetColorValue(SDK_WINDOWS_BLUE_ID)
#define    SDK_DISP_WHITE_COLOR     sdkSysGetColorValue(SDK_WHITE_COLOR_ID)

/**************************************函数定义Function Definition***********************************/
/*******************************************************************************
   标准版函数 Standard Functions
 ********************************************************************************/
extern SDK_DISP_PIXEL sdkDispGetScreenPixel(void);
extern s32 sdkDispFillRowRam(s32 siRow, s32 usColid, u8 *pasStr, u32 ucAtr);
extern s32 sdkDispAt(s32 siX, s32 siy, const u8 *pasStr);
extern void sdkDispClearScreen(void);
extern s32 sdkDispClearRow(s32 siRow);
extern s32 sdkDispClearAt(s32 siX0, s32 siY0, s32 siX1, s32 siY1);
extern void sdkDispBrushScreen(void);
extern s32 sdkDispSetBackground(s32 siX0, s32 siY0, s32 siXlen, s32 siYlen, const u8 *pasBmpFileName);
extern s32 sdkDispShowBmp(s32 siX0, s32 siY0, s32 siLen, s32 siYlen, const u8 *pasBmpFileName);
extern s32 sdkDispDrawProgbar(s32 siRow, s32 siX1, s32 siX2, u8 ucValue);

/*****************************其他接口函数调用透出Other Functions Call These Functions****************************/
extern s32 sdkDispRow(s32 siRow, s32 col, const u8 * pasStr, u32 ucAtr);
extern s32 sdkDispClearRowRam(s32 siRow);
extern s32 sdkDispClearRowRamAt(s32 siRow, s32 siStartCol, s32 siEndCol);
extern s32 sdkDispRestoreScreen(const u16 *pusLcdmem, s32 siLen);
extern u32 sdkDispGetScreenBufferSize(void);

extern s32 sdkDispRowAt(s32 siRow, s32 sicol, const u8 *pasStr, u32 ucAtr);
extern s32 sdkDispSaveScreen(const u16 *pusLcdmem, s32 siLen);
extern s32 sdkDispDrawProgbarRam(s32 siRow, s32 siX1, s32 siX2, u8 ucValue);
extern s32 sdkDispDrawLine(s32 siX0, s32 siY0, s32 siX1, s32 siY1, u32 siColor); //shijianglong 2013.01.23 16:28
extern s32 sdkDispPutPixel(s32 siX, s32 siY, u32 siColor); //shijianglong 2013.01.10 15:57
extern s32 sdkDispGetPixel(s32 siX, s32 siY); //shijianglong 2013.01.10 15:57
extern s32 sdkDispIcon(const SDK_DISP_ICON eIcon, u8 flag, s32 siX, s32 siY); //shijianglong 2013.06.25 11:43
extern void sdkDispSetBackColor(u32 usColor);
extern s32 sdkDispDrawDotRam(s32 siX, s32 siY, u32 ucColor);
extern s32 sdkDispDrawDot(s32 siX, s32 siY, u32 ucColor);

extern void sdkDispSetFontColor(u32 usColor);
extern s32 sdkDispGetFontColor(void);
extern s32 sdkDispSetFontSize(const SDK_DISP_FONT siFontSize);
extern s32 sdkDispGetFontSize(void);
extern s32 sdkDispSetRect(SDK_DISP_SRECT *pRect, const s32 siX0, const s32 siY0, const s32 siX1, const s32 siY1);
extern s32 sdkDispSetRectColor(const SDK_DISP_SRECT *pstRect, const u32 usColor);
extern s32 sdkDispNofdispAt(s32 siStartX, s32 siStartY, s32 siEndX, s32 siEndY);
/*******************************************************************
   版    权:    新国都
   作    者:    朱金平
   函数名称:    sdkDispShowBmpdots
   函数功能:	显示图片(点阵数据)
   输入参数:
                dispRect,图片要显示的区域
                bmpWidth,bmpheight,图片宽与高
                pBmpDots 图片数据指针，单色点阵液晶字模，
                        纵向取模，字节倒序
   输出参数:    无
   返 回 值:    SDK_PARA_ERR:参数错误。SDK_OK:函数调用成功。SDK_ERR:失败
   修改备注:
   日期时间:    012.11.02 10:43:59
*******************************************************************/
extern s32 sdkDispShowBmpdots( const SDK_DISP_SRECT dispRect, u32 bmpWidth, u32 bmpheight, const u8 *pBmpDots );
extern s32 sdkDispStateBar( bool statBarFlag );
extern s32 sdkDispClearStateBar( void );

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

