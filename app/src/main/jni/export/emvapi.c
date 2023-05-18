#include "emvapi.h"

int emv_add_aidparam(u8* par, int len)
{
	int ret;
	if(par == NULL || len <= 0 || len > 1020)
	{
		Trace("Download AID", "Error Code:-2\r\n");
		Trace("Download AID", "Input Param invalid\r\n");
		return SDK_ERR;
	}

	ret = TlvToAIDStruct(par, len);
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		return SDK_ERR;
	}

}

int emv_add_capkparam(u8* par, int len)
{
	int ret;
	if(par == NULL || len <= 0 || len > 1020)
	{
		Trace("Download AID", "Error Code:-2\r\n");
		Trace("Download AID", "Input Param invalid\r\n");
		return SDK_ERR;
	}

	ret = TlvToCAPKStruct(par, len);
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		return SDK_ERR;
	}
}

int emv_add_cardblack(void* blk)
{
	EMV_CARDBLACK temp;
	EMV_CARDBLACK *src = (EMV_CARDBLACK *)blk;
	int fp,ret,len;
	unsigned char fn[64];

	if(blk == NULL)
	{
		Trace("Download Exception File", "Error Code:-2\r\n");
		Trace("Download Exception File", "Info:Input Param invalid\r\n");
		return -1;
	}



	sdkSysGetCurAppDir(fn);
    strcat(fn, "BlackList");

	len = sdkGetFileSize(fn);
	if((len > 0) && ((len % sizeof(EMV_CARDBLACK)) != 0))
	{
		sdkDelFile(fn);
	}

	ret = sdkReadFile(fn, (unsigned char *)(&temp), 0, sizeof(EMV_CARDBLACK));
	if(ret == sizeof(EMV_CARDBLACK))
	{
		if((src->pan_len == temp.pan_len) && (!memcmp(src->pan, temp.pan, src->pan_len)) && (src->psn == temp.psn))
		{
			return 0;
		}
	}
	else if(ret == 0)
	{

	}
	else
	{
		Trace("Download Exception File", "Error Code:-5\r\n");
		Trace("Download Exception File", "Info:Read File len invalid\r\n");
		return -1
	}

	ret = sdkInsertFile(fn, (unsigned char *)src, 0, sizeof(EMV_CARDBLACK));
	if(ret != sizeof(EMV_CARDBLACK))
	{
		Trace("Download Exception File", "Error Code:-6\r\n");
		Trace("Download Exception File", "Info:Read File len invalid\r\n");
		return -1
	}
	else
	{
		return 0;
	}
}

int emv_add_certblack(void *blk)
{
	EMV_CERTBLACK temp;
	EMV_CERTBLACK *src = (EMV_CERTBLACK *)blk;
	int fp,ret,len;
	unsigned char fn[64];

	if(blk == NULL)
	{
		Trace("Download Revocation Cert", "Error Code:-2\r\n");
		Trace("Download Revocation Cert", "Info:Input Param invalid\r\n");
		return -1;
	}

	sdkSysGetCurAppDir(fn);
    strcat(fn, "Revokey");

	len = sdkGetFileSize(fn);
	if((len > 0) && ((len % sizeof(EMV_CARDBLACK)) != 0))
	{
		sdkDelFile(fn);
	}

	ret = sdkReadFile(fn, (unsigned char *)(&temp), 0, sizeof(EMV_CARDBLACK));
	if(ret == sizeof(EMV_CARDBLACK))
	{
		if((src->pki == temp.pki) && (!memcmp(src->rid, temp.rid, 5)) && (!memcmp(src->csn, temp.csn, 3)))
		{
			return 0;
		}
	}
	else if(ret == 0)
	{

	}
	else
	{
		Trace("Download Revocation Cert", "Error Code:-5\r\n");
		Trace("Download Revocation Cert", "Info:Read File len invalid\r\n");
		return -1
	}

	ret = sdkInsertFile(fn, (unsigned char *)src, 0, sizeof(EMV_CARDBLACK));
	if(ret != sizeof(EMV_CARDBLACK))
	{
		Trace("Download Revocation Cert", "Error Code:-6\r\n");
		Trace("Download Revocation Cert", "Info:Write File len invalid\r\n");
		return -1
	}
	else
	{
		return 0;
	}
}

int emv_clr_aidparam()
{
	int ret;
	ret = sdkEMVBaseDelAllAIDLists();
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int emv_clr_capkparam()
{
	int ret;
	ret = sdkEMVBaseDelAllCAPKLists();
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int emv_clr_cardblack()
{
	int fp,ret,len;
	unsigned char fn[64];

	sdkSysGetCurAppDir(fn);
    strcat(fn, "BlackList");

	ret = sdkDelFile(fn);
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		Trace("Delete Exception File", "Error Code:-7\r\n");
		Trace("Delete Exception File", "Info:Remove File Error\r\n");
		return -1
	}
}

int emv_clr_certblack(void)
{
	int fp,ret,len;
	unsigned char fn[64];

	sdkSysGetCurAppDir(fn);
    strcat(fn, "Revokey");

	ret = sdkDelFile(fn);
	if(ret == SDK_OK)
	{
		return 0;
	}
	else
	{
		Trace("Delete Revocation Cert", "Error Code:-7\r\n");
		Trace("Delete Revocation Cert", "Info:Remove File Error\r\n");
		return -1
	}
}

//unsigned char* emv_alloc_buffer(int *size)
//{
//	return NULL;
//}
//
//extern unsigned char gAppListCandicateMaxNum;
//
//int emv_get_candidate_list(void *list)
//{
//	int listNum = gAppListCandicateMaxNum;
//	SDK_EMVBASE_CL_AIDLIST *entrypointlist = NULL;
//	int actNum, ret, i;
//	EMV_CANDIDATE *dest = NULL;
//
//	entrypointlist = (SDK_EMVBASE_CL_AIDLIST *)sdkGetMem(sizeof(SDK_EMVBASE_CL_AIDLIST) * listNum);
//	if(NULL == entrypointlist)
//	{
//		Trace("ZTSDK-API", "Get mem error!Check malloc\r\n");
//		return -4;
//	}
//	memset(entrypointlist, 0, sizeof(SDK_EMVBASE_CL_AIDLIST) * listNum);
//
//	ret = sdkEMVBaseGetEntryPointAIDList(entrypointlist, listNum, &actNum);
//
//	Trace("ZTSDK-API", "sdkEMVBaseGetEntryPointAIDList  ret = %d\r\n", ret);
//	if(1 == ret)
//	{
//		dest = (EMV_CANDIDATE *)sdkGetMem(sizeof(EMV_CANDIDATE)*actNum);
//		if(NULL == entrypointlist)
//		{
//			Trace("ZTSDK-API", "Get mem error!Check malloc\r\n");
//			return -4;
//		}
//		memset(dest, 0 , sizeof(EMV_CANDIDATE)*actNum);
//
//		for(i = 0; i < actNum; i++)
//		{
//			(dest+i)->aid_len = (entrypointlist+i)->AIDLen;
//			memcpy((dest+i)->aid, (entrypointlist+i)->AID, (dest+i)->aid_len);
//			(dest+i)->lable_len = (entrypointlist+i)->AppLabelLen;
//			memcpy((dest+i)->lable, (entrypointlist+i)->AppLabel, (dest+i)->label)
//			(dest+i)->preferred_name_len = (entrypointlist+i)->PreferNameLen;
//			memcpy((dest+i)->preferred_name, (entrypointlist+i)->PreferName, (dest+i)->preferred_name_len);
//			(dest+i)->priority = (entrypointlist+i)->Priority;
//			(dest+i)->rsv[0] = i;
//		}
//		return actNum;
//	}
//	else
//	{
//		return ret;
//	}
//}


