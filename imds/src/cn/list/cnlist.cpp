/*
 * Crosslist.cpp
 *
 *  Created on: 2016年10月25日
 *      Author: mds
 */

#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"
#include "cnPlatSort.h"

int main(int argc, char **argv)
{
	Profile* pf=Profile::Instance("cnlist");
	char cn_shm_path[256];
	char cn_sh1_path[256];
	char cn_sz1_path[256];
	if(argc == 1)
	{
		const char* v=pf->getEnv("cn_shm_path");
		if (!v)
		{
			ERROR("cn_shm_path not found");
			exit(1);
		}
		strncpy(cn_shm_path,v,sizeof(cn_shm_path)-1);
		v=pf->getEnv("sh1_shm_path");
		if (!v)
		{
			ERROR("sh1_shm_path not found");
			exit(1);
		}
		strncpy(cn_sh1_path,v,sizeof(cn_sh1_path)-1);
		v=pf->getEnv("sz1_shm_path");
		if (!v)
		{
			ERROR("sz1_shm_path not found");
			exit(1);
		}
		strncpy(cn_sz1_path,v,sizeof(cn_sz1_path)-1);
	}else if(argc == 4)
	{
		strncpy(cn_shm_path,argv[1],sizeof(cn_shm_path)-1);
		strncpy(cn_sh1_path,argv[2],sizeof(cn_sh1_path)-1);
		strncpy(cn_sz1_path,argv[3],sizeof(cn_sz1_path)-1);
	}else
	{
		ERROR("argument error! usage:");
		ERROR("cnlist cn_shm_path sh1_shm_path sz1_shm_path");
		exit(1);
	}
	INFO("[%s][%s][%s]",cn_shm_path,cn_sh1_path,cn_sz1_path);
	cnPlatSort cnplatsort;
	if(!cnplatsort.load(cn_shm_path, cn_sh1_path, cn_sz1_path))
	{
		ERROR("load shared memory failed!");
		exit(1);
	}
	cnplatsort.update();
	return 0;
}


