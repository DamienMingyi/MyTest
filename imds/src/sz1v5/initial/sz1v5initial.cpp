/*
 * sz1v5initial.cpp
 *
 *  Created on: May 31, 2016
 *      Author: level2
 */

#include <stdlib.h>
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"
#include "Sz1v5Reference.h"

int main(int argc, char* argv[])
{
	Profile* pf = Profile::Instance("sz1v5initial");

	char shmpath[256];
	char refPath[256];

	if (argc == 3)
	{
		strncpy(shmpath, argv[1], sizeof(shmpath));
		strncpy(refPath, argv[2], sizeof(refPath));
	}
	else if (argc == 1)
	{
		const char* v=pf->getEnv("sz1_shm_path");
		if (!v)
		{
			ERROR("sz1_shm_path not found!");
			exit(1);
		}
		strncpy(shmpath, v, sizeof(shmpath));
		v = pf->getEnv("sz1_ref_path");
		if (!v)
		{
			ERROR("sz1_ref_path not found!");
			exit(1);
		}
		strncpy(refPath, v, sizeof(refPath));
	}
	else
	{
		ERROR("argument number error!");
		ERROR("usage: sz1v5initial shm path, ref path");
		exit(1);
	}
	INFO("sz1v5initial %s %s", shmpath, refPath);
	Sz1v5Reference sz1v5;
	if (!sz1v5.initial(refPath, shmpath))
	{
		ERROR("sz1v5 initial failed!");
		exit(1);
	}
	INFO("sz1v5 initial successful!");

	return 0;
}
