/*
 * sz1v5list.cpp
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#include "Sz1v5PlatSort.h"
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"

int main(int argc, char **argv)
{
	Profile* pf=Profile::Instance("sz1v5list");
	char shm_path[256];
	if(argc == 1)
	{
		const char* v=pf->getEnv("sz1_shm_path");
		if (!v)
		{
			ERROR("sz1_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
	}else if(argc == 2)
	{
		strncpy(shm_path,argv[1],sizeof(argv[1]));
	}else
	{
		ERROR("argument error! usage:");
		ERROR("sz1v5list sz1_shm_path");
		exit(1);
	}
	Sz1v5PlatSort sz1v5platsort;
	if(!sz1v5platsort.load(shm_path))
	{
		ERROR("load shared memory %s failed!",shm_path);
		exit(1);
	}
	sz1v5platsort.update();
	return 0;
}



