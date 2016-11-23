/*
 * OptionInitial.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: level2
 */

#include "OptionReference.h"

int main(int argc, char* argv[])
{
	Profile* pf=Profile::Instance("shoinitial");

	char shm_path[256];
	char ref_path[256];
	if (argc == 3)
	{
		strncpy(shm_path,argv[1], sizeof(shm_path));
		strncpy(ref_path,argv[2], sizeof(ref_path));
	}
	else if (argc == 1)
	{
		const char* v = pf->getEnv("sho_shm_path");
		if (!v)
		{
			ERROR("sho_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v, sizeof(shm_path));
		v = pf->getEnv("sho_ref_path");
		if (!v)
		{
			ERROR("sho_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v ,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("shoinitial shm_path ref_path");
		exit(1);
	}
	INFO("shoinitial %s %s",shm_path, ref_path);
	OptionReference opt;
	if (opt.initial(ref_path, shm_path))
	{
		INFO("shoinitial ok");
		return 0;
	}
	ERROR("shoinitial error");
	return 1;
}

