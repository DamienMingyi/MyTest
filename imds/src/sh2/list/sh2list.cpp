/*
 * sh2list.cpp
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */
#include "Sh2PlatSort.h"
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"

int main(int argc, char **argv)
{
	Profile* pf=Profile::Instance("sh2list");
	char shm_path[256];
	if(argc == 1)
	{
		const char* v=pf->getEnv("sh2_shm_path");
		if (!v)
		{
			ERROR("sh2_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
	}else if(argc == 2)
	{
		strncpy(shm_path,argv[1],sizeof(argv[1]));
	}else
	{
		ERROR("argument error! usage:");
		ERROR("sh2list sh2_shm_path");
		exit(1);
	}
	Sh2PlatSort sh2platsort;
	if(!sh2platsort.load(shm_path))
	{
		ERROR("load shared memory %s failed!",shm_path);
		exit(1);
	}
	sh2platsort.update();
	return 0;
}

