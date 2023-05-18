#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef SDK_KEY_H
#define SDK_KEY_H

#include "sdkDisp.h"


#define    SDK_KEY_ERR           0

#define    SDK_KEY_0             0x0030
#define    SDK_KEY_1             0x0031
#define    SDK_KEY_2             0x0032
#define    SDK_KEY_3             0x0033
#define    SDK_KEY_4             0x0034
#define    SDK_KEY_5             0x0035
#define    SDK_KEY_6             0x0036
#define    SDK_KEY_7             0x0037
#define    SDK_KEY_8             0x0038
#define    SDK_KEY_9             0x0039
#define    SDK_KEY_10           0x002A    //*
#define    SDK_KEY_11           0x0023    //#

#define    SDK_KEY_ENTER         0x0001
#define    SDK_KEY_ESC           0x0002
//#define    SDK_KEY_DEL           0x0003 //shijianglong 2013.01.08 15:49石玮松确认去掉该键值
#define    SDK_KEY_UP            0x0004
#define    SDK_KEY_DOWN          0x0005
#define    SDK_KEY_F1            0x0006
#define    SDK_KEY_F2            0x0007
#define    SDK_KEY_F3            0x0008
#define    SDK_KEY_F4            0x0009
#define    SDK_KEY_CLEAR         0x000A
#define    SDK_KEY_DOUBLEZERO    0x000B
#define    SDK_KEY_ALPHA         0x000C
#define    SDK_KEY_PRINT         0x000D
#define    SDK_KEY_SIGN          0x000E
#define    SDK_KEY_FUNCTION      0x000F
#define    SDK_KEY_MULTITASK     0x0010
#define    SDK_KEY_BACKSPACE     0x0011
#define    SDK_KEY_LEFT          0x0012
#define    SDK_KEY_RIGHT         0x0013
#define    SDK_KEY_ANY           0xFFFF    //这个键用来表示任意键//this key is used to represent any key
//-----add by zhuoquan 20130222-----
#define    SDK_KEY_KEYOK         0x0014
#define    SDK_KEY_DIALOUT       0x0015
#define    SDK_KEY_NOHOOK        0x0016
#define    SDK_KEY_FLASH         0x0017
#define    SDK_KEY_REDIAL        0x0018
#define    SDK_KEY_SMS           0x0019
#define    SDK_KEY_SET           0x0020
#define    SDK_KEY_BILL          0x0021
#define    SDK_KEY_MENU          0x0022
#define    SDK_KEY_DIALIN        0x0024
#define    SDK_KEY_PAY           0x0025
//-------end------

#define    SDK_KEY_POWER_OFF     0x0026  //关机
#define    SDK_KEY_F5            0x0027     //根据松菊指示添加，在K390P机型里边，作为账单支付按键的键值

#define SDK_KEY_MASK_0                          BIT_0               //只允许'0'键//only allow key '0'
#define SDK_KEY_MASK_1                          BIT_1               //只允许'1'键//only allow key '1'
#define SDK_KEY_MASK_2                          BIT_2               //只允许'2'键//only allow key '2'
#define SDK_KEY_MASK_3                          BIT_3               //只允许'3'键//only allow key '3'
#define SDK_KEY_MASK_4                          BIT_4               //只允许'4'键//only allow key '4'
#define SDK_KEY_MASK_5                          BIT_5               //只允许'5'键//only allow key '5'
#define SDK_KEY_MASK_6                          BIT_6               //只允许'6'键//only allow key '6'
#define SDK_KEY_MASK_7                          BIT_7               //只允许'7'键//only allow key '7'
#define SDK_KEY_MASK_8                          BIT_8               //只允许'8'键//only allow key '8'
#define SDK_KEY_MASK_9                          BIT_9               //只允许'9'键//only allow key '9'
#define SDK_KEY_MASK_00                         BIT_10              //只允许双'0'键//only allow key double '0'
                                                                    //允许所有数字键(含00键)  //allow all numeric key(contain key 00)
#define SDK_KEY_MASK_01                         (BIT_0|BIT_1)
#define SDK_KEY_MASK_12                         (BIT_1|BIT_2)
#define SDK_KEY_MASK_123                        (BIT_1|BIT_2|BIT_3)
#define SDK_KEY_MASK_1234                       (BIT_1|BIT_2|BIT_3|BIT_4)
#define SDK_KEY_MASK_12345                      (BIT_1|BIT_2|BIT_3|BIT_4|BIT_5)

#define SDK_KEY_MASK_DIGITAL                    (BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7|BIT_8|BIT_9|BIT_10)
#define SDK_KEY_MASK_10                          BIT_11               //只允许'*'键
#define SDK_KEY_MASK_11                         BIT_12              //只允许'#'键
#define SDK_KEY_MASK_BACKSPACE                   BIT_13             //只允许删除键//only allow key BACKSPACE
#define SDK_KEY_MASK_ESC                        BIT_14              //只允许取消键//only allow key ESC
#define SDK_KEY_MASK_ENTER                      BIT_15              //只允许确认键//only allow key ENTER
#define SDK_KEY_MASK_CLEAR                      BIT_16              //只允许清除键//only allow key CLEAR
#define SDK_KEY_MASK_UP                         BIT_17              //只允许上翻键//only allow key UP
#define SDK_KEY_MASK_DOWN                       BIT_18              //只允许下翻键//only allow key DOWN
#define SDK_KEY_MASK_F1                         BIT_19              //只允许F1键//only allow key F1
#define SDK_KEY_MASK_F2                         BIT_20              //只允许F2键//only allow key F2
#define SDK_KEY_MASK_F3                         BIT_21              //只允许F3键//only allow key 3
#define SDK_KEY_MASK_FEED                       BIT_22              //只允许进纸键//only allow key FEED
#define SDK_KEY_MASK_PRINT                      BIT_23              //只允许重打键//only allow key PRINT
#define SDK_KEY_MASK_FUNCTION                   BIT_24              //只允许功能键//only allow key FUNCTION
#define SDK_KEY_MASK_LOGIN                      BIT_25              //只允许签到键//only allow key LOGIN
#define SDK_KEY_MASK_ALPHA                      BIT_26              //只允许字母键//only allow key ALPHA
#define SDK_KEY_MASK_MULTTASK                   BIT_27              //多任务//multitask
#define SDK_KEY_MASK_LEFT                       BIT_28              //只允许左翻键//only allow key LEFT
#define SDK_KEY_MASK_RIGHT                      BIT_29              //只允许右翻键//only allow key RIGHT
#define SDK_KEY_MASK_F4                         BIT_30              //只允许F4键//only allow key F4
#define SDK_KEY_MASK_F5                         BIT_31              //只允许F5键
#define SDK_KEY_MASK_ALL                        0x7FFFFFFF          //允许所有的键//allow all keys

//*********************电话POS部分MASK用此组********************

#define SDK_KEY_MASK_SET                       BIT_22              //只允许设置键
#define SDK_KEY_MASK_BILL                      BIT_23              //只允许账单键
#define SDK_KEY_MASK_MENU                    BIT_25             //只允许菜单键
#define SDK_KEY_MASK_PAY                       BIT_27              //只允许支付键
#define SDK_KEY_MASK_PHONE                   BIT_30             //允许闪断、短信、免提、重拨、来电、去电6个键



#define MULKEYNUM       10                              //键数

typedef struct
{
    u8 TrueKey;
    u16 UpKey;
}SDK_KEY_TAB;

//*******************************************************************************
/*********************************************************************
标准版Standard Version
**********************************************************************/
extern s32 sdkKbGetKey(void);
extern void sdkKbKeyFlush (void);
extern s32 sdkKbWaitKey(u32 uiMask ,s32 sims);
extern s32 sdkKbGetCombinedKey(void);//shijianglong 2013.05.09 14:17
extern s32 sdkSysGetSysKeytab(u8 *poutkeytab, s32 siTabSize);
extern s32 sdkSysGetUserKeytab(SDK_KEY_TAB userkeytab[], u8 *userkeynum);
/*****************************************************************************
** Descriptions:	设置长按功能
** Parameters:    	bool enableLongPress  1:长按有功能 0：无
** Returned value:
** Created By:		朱金平  2014.07.08
** Remarks: 		默认长按无功能
*****************************************************************************/
extern s32 sdkKbkeySetLongPress( bool enableLongPress );

/*****************************************************************************
** Descriptions:	设置键盘背光灯是否自动熄灭
** Parameters:    	bool ctrl
** Returned value:
** Created By:		朱金平  2014.07.31
** Remarks: 		1.ctrl = 1，sdkKbSetLedDelayOffTimes设定的时间未操作自动关闭;
                    2.ctrl = 0，设定时间未操作不自动关闭;
*****************************************************************************/
extern s32 sdkKbSetBgLightEnableIdle(bool ctrl);

/*****************************************************************************
** Descriptions:	设置键盘无操作背光关闭时间
** Parameters:    	u8 uiTime
** Returned value:
** Created By:		朱金平  2014.07.31
** Remarks:
*****************************************************************************/
extern s32 sdkKbSetBgLightIdleTimes(u32 uiTime);

/****************************************************************************
  **Description:       控制键盘背光灯亮灭
  **Input parameters:  1--the led on  0--the led off
  **Output parameters:
********************Returned value:
********************Created by:            黄学佳(2013-4-15)

****************************************************************************/
extern s32 sdkKbSetBgLightOnOrOff( bool ctrl );

/*****************************************************************************
  **Description:       将秒转换为毫秒
  **Input parameters:  secondTimes:秒数(最大不超过4294967秒)
  **Output parameters: 毫秒数
** Created By:		    朱金平  2014.08.06
** Remarks:
*****************************************************************************/
#define SDK_SECOND_TO_MS( secondTimes )   ((secondTimes) * 1000 )


#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
