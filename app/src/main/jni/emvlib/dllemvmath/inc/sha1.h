#ifndef _SHA1_H
#define _SHA1_H

#define SHA1_MAC_LEN 20

typedef struct _SHA1_CTX{
        unsigned int state[5];
        unsigned int count[2];
        unsigned char buffer[64];
} SHA1_CTX;

#endif /* SHA1_H */

