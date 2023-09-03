#ifndef SHA1_H
#define SHA1_H

#define SHA1_MAC_LEN 20

typedef struct _SHA1_CTX{
        unsigned long state[5];
        unsigned long count[2];
        unsigned char buffer[64];
} SHA1_CTX;

#endif /* SHA1_H */

