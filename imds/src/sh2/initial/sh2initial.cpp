/*
 * sh2initial.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: level2
 */

#include <stdio.h>
#include "Sh2Reference.h"
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"

int main(int argc,char* argv[])
{
	Profile* pf=Profile::Instance("sh2initial");

	char shm_path[256];
	char ref_path[256];
	if (argc==3)
	{
		strncpy(shm_path,argv[1],sizeof(shm_path));
		strncpy(ref_path,argv[2],sizeof(ref_path));
	}
	else if (argc==1)
	{
		const char* v=pf->getEnv("sh2_shm_path");
		if (!v)
		{
			ERROR("sh2_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
		v=pf->getEnv("sh2_ref_path");
		if (!v)
		{
			ERROR("sh2_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("sh2_initial shm_path ref_path");
		exit(1);
	}
	INFO("sh2initial %s %s",shm_path,ref_path);
	Sh2Reference sh2;
	if (sh2.initial(ref_path,shm_path))
	{
		INFO("sh2initial ok");
		return 0;
	}
	ERROR("sh2initial error");
	return 1;
}




