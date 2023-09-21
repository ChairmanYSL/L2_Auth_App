#ifndef AFX_SDK_GLOBAL_H
#define AFX_SDK_GLOBAL_H

#define SDK_OK                       (1)
#define SDK_EQU                      (0)        //比较大小相等时用这个//use this if it's equal when compare size
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)       //超时//timeout
#define SDK_ESC                      (-3)
#define SDK_PARA_ERR                 (-4)
#define SDK_FUN_NULL                 (-5)       //函数体为空
#define SDK_EBUSY                    (-6)       /* 设备忙 */
#define SDK_EIO                      (-7)       /* 设备未打开 */
#define SDK_EDATA                    (-8)       /* 数据溢出或数据错误 */
#define SDK_ICC_NOCARD				 (-9)
#define SDK_ICC_MUTICARD			 (-10)

#include <stdio.h>
#include <string.h>
#include "sdktypedef.h"
#include "sdkDebug.h"
#include "sdkmaths.h"
#include "sdkemvbase.h"
#include "sdkkey.h"
#include "sdkmmi.h"
#include "sdkped.h"

#ifdef assert
#undef  assert
#endif

#endif
