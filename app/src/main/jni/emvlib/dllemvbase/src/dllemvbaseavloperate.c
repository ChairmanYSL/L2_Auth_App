#include "dllemvbase.h"
#include "emvbaseavl.h"
#include "dllemvbasedebug.h"


struct emvbase_avl_table *emvbasetree = NULL;
unsigned int gemvbasecurtagtype = EMVTAGTYPE_EMV;

void emvbase_avl_settagtype(unsigned int tagtype)
{
	gemvbasecurtagtype = tagtype;
}

unsigned int emvbase_avl_gettagtype(void)
{
	return gemvbasecurtagtype;
}

static int	emvbase_compare_EMVtagitems(const void *pa, const void *pb, void *param)
{
  const EMVBASETAGCVLITEM *itema = (EMVBASETAGCVLITEM *)pa;
  const EMVBASETAGCVLITEM *itemb = (EMVBASETAGCVLITEM *)pb;
  int i;
  unsigned int a = 0;
  unsigned int b = 0;

  for(i = 0;i < 3;i++)
  {
  	a = (a<<8) + itema->Tag[i];
	b = (b<<8) + itemb->Tag[i];
  }
  
  if (a < b)
    return -1;
  else if (a > b)
    return +1;
  else
    return 0;  
}


void emvbase_avl_deletetagdata(void *avl_item, void *avl_param)
{
	EMVBASETAGCVLITEM *item;
    
	item = (EMVBASETAGCVLITEM *)avl_item;
    
    if(emvbasetree == NULL)
    {
    	return;

    }
    
	if(item != NULL)
	{
		if(item->data != NULL)
		{
			emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, item->data);
			item->data = NULL;
		}
		emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, item);
	}
    
	avl_item = NULL;
}


int emvbase_avl_createtree(void)
{
	if(emvbasetree != NULL)
	{
		emvbase_avl_destorytree();
	}
    
	emvbasetree = emvbase_avl_create (emvbase_compare_EMVtagitems, NULL, NULL);
	if (emvbasetree == NULL)
	{
		return 1;
	}

	return 0;
}


void emvbase_avl_destorytree(void)
{
	if(emvbasetree != NULL)
	{
		emvbase_avl_destroy(emvbasetree, emvbase_avl_deletetagdata);
		emvbasetree = NULL;
	}
}


unsigned char emvbase_avl_checktreestatus(void)
{
	if(emvbasetree != NULL)
	{
		return 1;
	}
	
	return 0;
}


void emvbase_avl_deletetag(unsigned char *tag)
{
	EMVBASETAGCVLITEM *item;
	EMVBASETAGCVLITEM *itemreturn;

    if(emvbasetree == NULL)
    {
    	return;
    }

	item = emvbase_avl_gettagitempointer(tag);
	if(item == NULL)
	{
		return;
	}
	
	itemreturn = (EMVBASETAGCVLITEM *)emvbase_avl_delete(emvbasetree,(void *)item);

	if(itemreturn != NULL)
	{
		if(itemreturn->data != NULL)
		{
			emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, itemreturn->data);
			itemreturn->data = NULL;
		}
		emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, itemreturn);
	}
}


int emvbase_avl_findtag(unsigned char *tag,unsigned char taglen,void *param)
{
	EMVBASETAGCVLITEM *item;
	EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
    
	if(emvbasetree == NULL)
	{
		return EMVBASETAGNOTEXIST;
	}
	
	memset((unsigned char *)&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (taglen>3) ? 3:(taglen);
    
    memcpy(tempitem.Tag,tag,savetaglen);
    
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
	if(item == NULL)
	{
		return EMVBASETAGNOTEXIST;
	}
    
	return (item->len)?(EMVBASETAGEXIST):(EMVBASETAGEMPTY);
}


int emvbase_avl_readtag(unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short *tagdatalen,void *param)
{
    unsigned int len;
	EMVBASETAGCVLITEM *item;
	EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
    
    
	if(emvbasetree == NULL)
	{
		return EMVBASETAGNOTEXIST;
	}
    
	memset((unsigned char *)&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (taglen>3) ? 3:(taglen);
    
    memcpy(tempitem.Tag,tag,savetaglen);
    
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
	if(item == NULL)
	{
		return EMVBASETAGNOTEXIST;
	}

    len = *tagdatalen;
    *tagdatalen = (item->len >len)?(len):(item->len);
	if(*tagdatalen)
	{
	    if(item->data != NULL)
	    {
			memcpy(tagdata,item->data,*tagdatalen);
	    }
		
	}
    
	return (item->len)?(EMVBASETAGEXIST):(EMVBASETAGEMPTY);
}


int emvbase_avl_inserttag(EMVBASETAGCVLITEM *storeditem,unsigned char *tag,unsigned char taglen,unsigned char *tagdata,unsigned short tagdatalen,void *param)
{
    EMVBASETAGCVLITEM *item;
    EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
	unsigned char itemfind = 0;
	int rslt;

    if(tag == NULL)
    {
    	return 1;
    }
    if(memcmp(tag,"\x00\x00\x00",3) == 0)
	{
		return 1;
    }
	
	if(emvbasetree == NULL)
	{
		return 1;
	}
	
	memset((unsigned char *)&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (taglen>3) ? 3:(taglen);
    
    memcpy(tempitem.Tag,tag,savetaglen);
	
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
	if(item != NULL)
	{
		itemfind = 1;
		if(item->len)
		{
		    if(item->data != NULL)
		    {
				emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, item->data);
				item->data = NULL;
		    }
			item->len = 0;
		}
	}
    else
    {
    	item = (EMVBASETAGCVLITEM *)emvbasetree->avl_alloc->libavl_malloc(emvbasetree->avl_alloc,sizeof(EMVBASETAGCVLITEM));
		memset((unsigned char *)item,0,sizeof(EMVBASETAGCVLITEM));
        memcpy(item->Tag,tag,savetaglen);
    }

    if(storeditem != NULL)
    {
    	memcpy((unsigned char *)item,(unsigned char *)storeditem,sizeof(EMVBASETAGCVLITEM));
    }
    else
    {
    	memset((unsigned char *)&tempitem,0,sizeof(EMVBASETAGCVLITEM));
		rslt = emvbase_TagBaseLib_read(tag,taglen,&tempitem,gemvbasecurtagtype);
		if(rslt == 1)
		{
		    if(itemfind == 0)
		    {
				if(itemfind == 0&&rslt == 1)
				{
			    	memcpy(tempitem.Tag,tag,savetaglen);
					tempitem.updatecondition = EMVTAGUPDATECONDITION_K;
					tempitem.datafomat = EMVTAGFORMAT_B;
	                tempitem.SupAppType = EMVTAGTYPE_NULL;

					memcpy((unsigned char *)item,(unsigned char *)&tempitem,sizeof(EMVBASETAGCVLITEM));
				}
		    }
		}
        else
        {
        	memcpy((unsigned char *)item,&tempitem,sizeof(EMVBASETAGCVLITEM));
        }
		
    }
    
	
	item->len = tagdatalen;
    if(item->len)
    {
    	item->data = (unsigned char *)emvbasetree->avl_alloc->libavl_malloc(emvbasetree->avl_alloc,item->len);
    }
	if(tagdata != NULL && item->len)
	{
    	memcpy(item->data,tagdata,item->len);
	}

	if(memcmp(item->Tag,"\x00\x00\x00",3) == 0)  
	{
		if(itemfind == 0)
		{
		    if(item != NULL)
		    {
				if(item->data != NULL)
			    {
					emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, item->data);
					item->data = NULL;
			    }
				emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, item);
				item = NULL;
		    }
			
		}
		return 1;
	}
	
	
	if(itemfind == 0)
	{
		emvbase_avl_insert(emvbasetree,item);
	}
    	
	return 0;
}

int emvbase_avl_createsettagvalue(unsigned char *tag,unsigned char *tagdata,unsigned short tagdatalen)
{
	return emvbase_avl_inserttag(NULL,tag,strlen((char *)tag),tagdata,tagdatalen,NULL);
}

int emvbase_avl_settag(unsigned char *tag,unsigned char tagdata)
{
	return emvbase_avl_inserttag(NULL,tag,strlen((char *)tag),&tagdata,1,NULL);
}
unsigned char emvbase_avl_gettagvalue(unsigned char *tag)
{
    int rst=0;
	unsigned short len=0;
	unsigned char data=0;
    
	len = 1;
	rst = emvbase_avl_readtag(tag,strlen((char *)tag),&data,&len,NULL);
    if(rst == EMVBASETAGEXIST)
    {
    	if(len)
    	{
    		return data;
    	}
    }
	
	return 0;
}

unsigned char emvbase_avl_gettagvalue_spec(unsigned char *tag,unsigned char *tagdata,unsigned short strpos,unsigned short len)
{
    EMVBASETAGCVLITEM *tagitem;

    tagitem = emvbase_avl_gettagitempointer(tag);
    if(tagitem != NULL)
    {
		if(tagitem->data != NULL)
		{
			if(strpos <= tagitem->len &&(strpos+len) <= tagitem->len)
			{
			    memcpy(tagdata,&tagitem->data[strpos],len);
				return 0;
			}
		}
	}
	return 1;
}

unsigned char emvbase_avl_gettagvalue_all(unsigned char *tag,unsigned char *tagdata,unsigned short *len)
{
    EMVBASETAGCVLITEM *tagitem;
	
    tagitem = emvbase_avl_gettagitempointer(tag);
    if(tagitem != NULL)
    {
		if(tagitem->data != NULL)
		{
			
		    memcpy(tagdata,tagitem->data,tagitem->len);
			*len = tagitem->len;
			return 0;
		}
	}

    *len = 0;
	return 1;
}

unsigned char *emvbase_avl_gettagvaluepointer(unsigned char *tag)
{
	EMVBASETAGCVLITEM *item;
	EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
    
    
	if(emvbasetree == NULL)
	{
		return NULL;
	}
    
	
	memset(&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (strlen((char *)tag)>3) ? 3:(strlen((char *)tag));
    
    memcpy(tempitem.Tag,tag,savetaglen);
    
	
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
	if(item == NULL || item->len == 0)
	{
		return NULL;
	}
    
	return item->data;
}


EMVBASETAGCVLITEM *emvbase_avl_gettagitempointer(unsigned char *tag)
{
	EMVBASETAGCVLITEM *item;
	EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
    
    
	if(emvbasetree == NULL)
	{
		return NULL;
	}
    
	
	memset(&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (strlen((char *)tag)>3) ? 3:(strlen((char *)tag));
    
    memcpy(tempitem.Tag,tag,savetaglen);
    
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
    
	return item;
}


void emvbase_avl_setvalue_and(unsigned char *tag,unsigned short  position,unsigned char anddata)
{
	EMVBASETAGCVLITEM *item;
    
	item = emvbase_avl_gettagitempointer(tag);
	if(item != NULL)
	{
		if(item->len > position)
		{
			item->data[position] &= anddata;
		}
	}
	
}

void emvbase_avl_setvalue_or(unsigned char *tag,unsigned short  position,unsigned char ordata)
{
	EMVBASETAGCVLITEM *item;

	item = emvbase_avl_gettagitempointer(tag);
	if(item != NULL)
	{
		if(item->len > position)
		{
			item->data[position] |= ordata;
		}
	}
}

EMVBASETAGCVLITEM *emvbase_avl_gettagitemandstatus(unsigned char *tag,unsigned char *tagexistflag)
{
    EMVBASETAGCVLITEM *item;

	*tagexistflag = 0;

	item = emvbase_avl_gettagitempointer(tag);
	if(item != NULL)
	{
		if(item->len)
		{
		    *tagexistflag = 1;
			return item;
		}
	}
	
	return NULL;
}


int emvbase_avl_settagvalue_usep(unsigned char *tag,unsigned short pos,unsigned char *tagdata,unsigned short tagdatalen)
{
	unsigned int rst;
	unsigned char *data;
	unsigned char findrst = 0;
	EMVBASETAGCVLITEM *item;
	EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
    
    
	if(emvbasetree == NULL)
	{
		return 1;
	}
    
	
	memset(&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (strlen((char *)tag)>3) ? 3:(strlen((char *)tag));
    
    memcpy(tempitem.Tag,tag,savetaglen);
   
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
	if(item == NULL)
	{
		findrst = 1;
	}
	else
	{
		if(item->len <= pos || (pos + tagdatalen)> item->len)
		{
			findrst = 1;
		}
		
	}

	if(findrst)
	{
	    if(tagdatalen)
	    {
			data = (unsigned char *)emvbasetree->avl_alloc->libavl_malloc(emvbasetree->avl_alloc,pos + tagdatalen);
			
			memset(data,0,pos + tagdatalen);
			if(item->data)
			{
				memcpy(data,item->data,pos);
				
			}
			memcpy(&data[pos],tagdata,tagdatalen);
            
            rst = emvbase_avl_inserttag(NULL,tag,strlen((char *)tag),data,pos + tagdatalen,NULL);
			
			emvbasetree->avl_alloc->libavl_free(emvbasetree->avl_alloc, data);
			
			return rst;
	    }
	}
    else
    {
		memcpy(&item->data[pos],tagdata,tagdatalen);
    }

	return 0;
	
}


unsigned char emvbase_avl_checkiftagexist(unsigned char *tag)
{
	EMVBASETAGCVLITEM *item;
    
	EMVBASETAGCVLITEM tempitem;
	unsigned char savetaglen;
    
    
	if(emvbasetree == NULL)
	{
		return 0;
	}
    
	memset(&tempitem,0,sizeof(EMVBASETAGCVLITEM));
	savetaglen = (strlen((char *)tag)>3) ? 3:(strlen((char *)tag));
    
    memcpy(tempitem.Tag,tag,savetaglen);
    
	item = (EMVBASETAGCVLITEM *)emvbase_avl_find (emvbasetree,(void *)&tempitem);
	if(item == NULL)
	{
		return 0;
	}
    
	return (item->len)?(1):(0);
}


void emvbase_avl_printtagvalue(unsigned char *title,unsigned char *tag,unsigned short strpos,unsigned short len)
{
#ifdef EMVBASE_DEBUG
	EMVBASETAGCVLITEM *tagitem;
	unsigned short currentlen = 0;
	
	EMVBase_TraceHex(title,tag,strlen((char *)tag));
		
	tagitem = emvbase_avl_gettagitempointer(tag);
    if(tagitem == NULL)
    {
    	EMVBase_Trace("NULL\r\n");
    }
	else
	{
	    EMVBase_Trace("len = %d\r\n",tagitem->len);
		if(tagitem->len <= strpos)
		{
			return;
		}
		currentlen = (tagitem->len - strpos);
		if(currentlen > len)
		{
			currentlen = len;
		}
		if(tagitem->data != NULL)
		{
			EMVBase_TraceHex(NULL,tagitem->data,currentlen);
		}
	}
#endif
}


void emvbase_avl_printtagallvalue(unsigned char *title,unsigned char *tag)
{
#ifdef EMVBASE_DEBUG
	EMVBASETAGCVLITEM *tagitem;

    EMVBase_TraceHex(title,tag,strlen((char *)tag));
	
	tagitem = emvbase_avl_gettagitempointer(tag);
    if(tagitem == NULL)
    {
    	EMVBase_Trace("NULL\r\n");
    }
	else
	{
	    EMVBase_Trace("len = %d\r\n",tagitem->len);
		if(tagitem->data != NULL)
		{
	    	EMVBase_TraceHex(NULL,tagitem->data,tagitem->len);
		}
	}
#endif
}

static void emvbase_avl_readtreetagdata_all(const struct emvbase_avl_node *node, int level,unsigned char *tag,int *taglen)
{
	/* You can set the maximum level as high as you like.
	Most of the time, you'll want to debug code using small trees,
	so that a large |level| indicates a ``loop'', which is a bug. */
	
    
	if (level > 16)
	{
		return;
	}
    
	if (node == NULL)
	{
		return;
	}
    
    if((*taglen/100)>30)  //最大长度控制,最大的个数应该做为参数传递进来//sxl?20131024
    {
    	return;
    }
	memcpy(&tag[*taglen],((EMVBASETAGCVLITEM *)(node->avl_data))->Tag,3);
    *taglen += 3;
    tag[*taglen] = ((EMVBASETAGCVLITEM *)(node->avl_data))->updatecondition;
	*taglen += 1;
	
    
	
	if ((node->avl_link[0] != NULL) || (node->avl_link[1] != NULL))
	{	  
		if(node->avl_link[0] != NULL)
		{
			emvbase_avl_readtreetagdata_all (node->avl_link[0], level + 1,tag,taglen);
		}
		if (node->avl_link[1] != NULL)
		{
			emvbase_avl_readtreetagdata_all (node->avl_link[1], level + 1,tag,taglen);
		}
	}
	
}


//20140320
void emvbase_avl_readtreetagdatatospecfile(unsigned char *tag,int *taglen)
{
	if(emvbasetree == NULL)
	{
		return;
	}
    
	
	emvbase_avl_readtreetagdata_all(emvbasetree->avl_root,0,tag,taglen);  //sxl?
    
	
}

