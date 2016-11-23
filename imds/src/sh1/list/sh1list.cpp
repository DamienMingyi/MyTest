/*
 * sh1list.cpp
 *
 *  Created on: 2016年10月19日
 *      Author: mds
 */
#include "Sh1PlatSort.h"
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"

int main(int argc, char **argv)
{
	Profile* pf=Profile::Instance("sh1list");
	char shm_path[256];
	if(argc == 1)
	{
		const char* v=pf->getEnv("sh1_shm_path");
		if (!v)
		{
			ERROR("sh1_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
	}else if(argc == 2)
	{
		strncpy(shm_path,argv[1],sizeof(argv[1]));
	}else
	{
		ERROR("argument error! usage:");
		ERROR("sh1list sh1_shm_path");
		exit(1);
	}
	Sh1PlatSort sh1platsort;
	if(!sh1platsort.load(shm_path))
	{
		ERROR("load shared memory %s failed!",shm_path);
		exit(1);
	}
	sh1platsort.update();
	return 0;
}


