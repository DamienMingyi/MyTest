/*
 * FixedIncomeInitial.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: level2
 */

#include "FixedIncomeReference.h"
#include "../public/program/Profile.h"
#include "../public/program/simplelog.h"

int main(int argc, char* argv[])
{
	Profile* pf=Profile::Instance("shfiinitial");

	char shm_path[256];
	char ref_path[256];
	if (argc == 3)
	{
		strncpy(shm_path,argv[1], sizeof(shm_path));
		strncpy(ref_path,argv[2], sizeof(ref_path));
	}
	else if (argc == 1)
	{
		const char* v = pf->getEnv("shfi_shm_path");
		if (!v)
		{
			ERROR("shfi_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v, sizeof(shm_path));
		v = pf->getEnv("shfi_ref_path");
		if (!v)
		{
			ERROR("shfi_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v ,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("shfiinitial shm_path ref_path");
		exit(1);
	}
	INFO("shfiinitial %s %s",shm_path, ref_path);
	FixedIncomeReference opt;
	if (opt.initial(ref_path, shm_path))
	{
		INFO("shfiinitial ok");
		return 0;
	}
	ERROR("shoinitial error");
	return 1;
}




