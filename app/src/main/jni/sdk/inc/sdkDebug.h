#ifndef SDKDEBUG_H
#define SDKDEBUG_H

#define ZT_SDK_DEBUG
//#undef ZT_SDK_DEBUG

#include "sdktypedef.h"

extern s32 sdkDebugTraceHex(u8 const* pasTag, u8 const* pasInfo, void const *pheDate, s32 siLen,u8 const *pasfun,s32 line);
extern void SdkDebugTrace(u8 const *pasTag, u8 const*  pasFile, u8 const *pFun, int siLine,  char const *pheFmt, ...);


#ifdef ZT_SDK_DEBUG
#define Trace(ptag, arg...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)__FUNCTION__, __LINE__, ##arg)
#define  TraceHex(pasTag, pasInfo, pheDate, uiLen)   sdkDebugTraceHex((u8 const*)pasTag, (u8 const*)pasInfo, pheDate, uiLen,(u8 const*)__FUNCTION__,__LINE__)
#define Assert(exp)
#define Verify(exp)    ((void)(exp))
#else
#define Trace(ptag, arg...)
#define  TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Assert(exp)
#define Verify(exp)    ((void)(exp))
#endif

#endif
