#ifndef SDKTYPEDEF_H
#define SDKTYPEDEF_H

#define SDK_OK                       (1)
#define SDK_EQU                      (0)
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)
#define SDK_ESC                      (-3)
#define SDK_PARA_ERR                 (-4)
#define SDK_FUN_NULL                 (-5)
#define SDK_EBUSY                    (-6)
#define SDK_EIO                      (-7)
#define SDK_EDATA                    (-8)
#define SDK_ICC_NODATA				 (-9)

#define SDK_ICC_OVER_LIMIT			(-100)


#define s64  signed long
#define s32  signed int
#define s16  signed short
#define s8   signed char

#define u64 unsigned long
#define u32 unsigned int
#define u16 unsigned short
#define u8  unsigned char

#ifndef bool
#define bool unsigned int
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif
