/*
 * hkpInitial.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: level2
 */

#include <stdio.h>
#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"
#include "../../shhk/initial/ShhkReference.h"

int main(int argc,char* argv[])
{
	Profile* pf=Profile::Instance("shhkinitial");
	char shm_path[256];
	char ref_path[256];
	if (argc==3)
	{
		strncpy(shm_path,argv[1],sizeof(shm_path));
		strncpy(ref_path,argv[2],sizeof(ref_path));
	}
	else if (argc==1)
	{
		const char* v=pf->getEnv("shhk_shm_path");
		if (!v)
		{
			ERROR("shhk_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
		v=pf->getEnv("shhk_ref_path");
		if (!v)
		{
			ERROR("shhk_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("shhk_initial shm_path ref_path");
		exit(1);
	}
	DEBUG("shhkinitial %s %s",shm_path,ref_path);
	ShhkReference hk;
	if (hk.initial(ref_path,shm_path))
	{
		DEBUG("shhkinitial ok");
		return 0;
	}
	ERROR("shhkinitial error");
	return 1;
}


