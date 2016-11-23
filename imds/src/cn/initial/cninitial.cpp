/*
 * crossinitial.cpp
 *
 *  Created on: 2016年10月24日
 *      Author: mds
 */


#include <stdio.h>

#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"
#include "cnReference.h"

int main(int argc,char* argv[])
{
	Profile* pf=Profile::Instance("cninitial");
	char shm_path[256];
	char ref_path[256];
	if (argc==3)
	{
		strncpy(shm_path,argv[1],sizeof(shm_path));
		strncpy(ref_path,argv[2],sizeof(ref_path));
	}
	else if (argc==1)
	{
		const char* v=pf->getEnv("cn_shm_path");
		if (!v)
		{
			ERROR("cn_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
		v=pf->getEnv("cn_ref_path");
		if (!v)
		{
			ERROR("cn_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("cninitial shm_path ref_path");
		exit(1);
	}
	DEBUG("cninitial %s %s",shm_path,ref_path);
	cnReference cross;
	if (cross.initial(ref_path,shm_path))
	{
		DEBUG("cninitial ok");
		return 0;
	}
	ERROR("cninitial error");
	return 1;
}
