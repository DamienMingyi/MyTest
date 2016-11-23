/*
 * OptionClosing.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: level2
 */


#include <stdio.h>
#include "SettlePrice.h"
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"

int main(int argc,char* argv[])
{
	Profile* pf=Profile::Instance("shoclose");

	char shm_path[256];
	char ref_path[256];
	if (argc == 3)
	{
		strncpy(shm_path,argv[1],sizeof(shm_path));
		strncpy(ref_path,argv[2],sizeof(ref_path));
	}
	else if (argc==1)
	{
		const char* v=pf->getEnv("sho_shm_path");
		if (!v)
		{
			ERROR("sho_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
		v=pf->getEnv("sho_ref_path");
		if (!v)
		{
			ERROR("sho_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("shoclose shm_path ref_path");
		exit(1);
	}
	INFO("shoclose %s %s",shm_path,ref_path);
	SettlePrice opt;
	if (opt.close(ref_path,shm_path))
	{
		INFO("shoclose ok");
		return 0;
	}
	ERROR("shoclose error");
	return 1;
}






