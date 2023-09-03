#ifndef EXTERN_API_H
#define EXTERN_API_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

extern int sdkSysSetCurAppDir(unsigned char *pasDir, int len);
extern int sdkGetRandom(unsigned char *pheRdm, int siNum);
extern unsigned short sdkReadPosSn(unsigned char *pasDest);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
