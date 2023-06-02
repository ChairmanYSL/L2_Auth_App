#ifndef AFX_CONTACTLESS_H
#define AFX_CONTACTLESS_H

#define PAYPASSTRADE_CONSUME 0x00
#define PAYPASSTRADE_REFUND  0x20
#define PAYPASSTRADE_PURWITHCASH 0x09
#define PAYPASSTRADE_CASH    0x01


#define PAYEAVE_REFUND 0x01
#define PAYEAVE_CASH   0x02
#define PAYEAVE_CASHBACK 0x03

#define OTHERAMOUNTPOS 6
#define OTHERAMOUNTLEN 6


typedef enum
{
 	JCB_GOODS=0,
	JCB_REFUND=1,
 	JCB_CASH=2,
	JCB_CASHBACK=3,
}JCB_TRANSTYPE;


extern void contactless_promptremovecardsoundled(void);
#endif

