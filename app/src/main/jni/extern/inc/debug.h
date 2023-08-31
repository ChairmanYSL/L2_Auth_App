#ifndef __DEBUG_ASDK_2_JNI__H
#define __DEBUG_ASDK_2_JNI__H
#include <android/log.h>
#ifdef __cplusplus
extern "C" {
#endif
void hal_sys_info(const char *__LOG_TAG__, const char* pMessage, ...);
void hal_sys_error(const char *__LOG_TAG__, const char* pMessage, ...);
void hal_sys_warn(const char *__LOG_TAG__, const char* pMessage, ...);
void hal_sys_debug(const char *__LOG_TAG__, const char* pMessage, ...);
void hal_sys_dump(const char *__LOG_TAG__, const char * str, unsigned char * pBuf, unsigned int len);
void dbg(char* txt);
void LOGI_HEX(const char *__LOG_TAG__, const char * str, unsigned char * pBuf, unsigned int len);
void hal_sys_info_debug(const char *__LOG_TAG__, const char* pMessage, ...);
//#define __USE_BY_DEBUG__
#ifdef __USE_BY_DEBUG__
	#define   Dbg_Msg(tag,...)           {printf(__VA_ARGS__); hal_sys_info(tag, __VA_ARGS__);}
#else
	#define   Dbg_Msg(tag,...)
#endif

#ifdef __cplusplus
}
#endif

#endif

