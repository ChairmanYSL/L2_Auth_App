#ifndef _DLLEMVBASEAVLOPERATE_H_
#define _DLLEMVBASEAVLOPERATE_H_

extern struct emvbase_avl_table *emvbasetree;
extern unsigned int gemvbasecurtagtype;

extern void emvbase_avl_settagtype(unsigned int tagtype);
extern unsigned int emvbase_avl_gettagtype(void);
extern int emvbase_avl_createtree(void);
extern void emvbase_avl_destorytree(void);
extern int emvbase_avl_findtag(unsigned char *tag,unsigned char taglen,void *param);
extern int emvbase_avl_readtag(unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short *tagdatalen,void *param);
extern int emvbase_avl_inserttag(EMVBASETAGCVLITEM *storeditem,unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short tagdatalen,void *param);
extern int emvbase_avl_settag(unsigned char *tag,unsigned char tagdata);
extern int emvbase_avl_createsettagvalue(unsigned char *tag,unsigned char *tagdata,unsigned short tagdatalen);
extern EMVBASETAGCVLITEM *emvbase_avl_gettagitempointer(unsigned char *tag);
extern unsigned char emvbase_avl_gettagvalue_spec(unsigned char *tag,unsigned char *tagdata,unsigned short strpos,unsigned short len);
extern void emvbase_avl_setvalue_and(unsigned char *tag,unsigned short  position,unsigned char anddata);
extern void emvbase_avl_setvalue_or(unsigned char *tag,unsigned short  position,unsigned char ordata);
extern unsigned char emvbase_avl_checkiftagexist(unsigned char *tag);
extern EMVBASETAGCVLITEM *emvbase_avl_gettagitemandstatus(unsigned char *tag,unsigned char *tagexistflag);
extern void emvbase_avl_deletetag(unsigned char *tag);
extern void emvbase_avl_printtagvalue(unsigned char *title,unsigned char *tag,unsigned short strpos,unsigned short len);
extern void emvbase_avl_printtagallvalue(unsigned char *title,unsigned char *tag);
extern unsigned char emvbase_avl_gettagvalue_all(unsigned char *tag,unsigned char *tagdata,unsigned short *len);
extern unsigned char emvbase_avl_gettagvalue(unsigned char *tag);
extern unsigned char emvbase_avl_checktreestatus(void);

#endif





