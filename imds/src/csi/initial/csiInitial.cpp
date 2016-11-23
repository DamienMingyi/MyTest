/*
 * csipInitial.cpp
 *
 *  Created on: Sep 1, 2015
 *      Author: level2
 */

#include <stdio.h>

#include "../../public/program/Profile.h"
#include "../../public/program/Logger.h"
#include "CsiReference.h"

int main(int argc,char* argv[])
{
	Profile* pf=Profile::Instance("csiinitial");
	char shm_path[256];
	char ref_path[256];
	if (argc==3)
	{
		strncpy(shm_path,argv[1],sizeof(shm_path));
		strncpy(ref_path,argv[2],sizeof(ref_path));
	}
	else if (argc==1)
	{
		const char* v=pf->getEnv("csi_shm_path");
		if (!v)
		{
			ERROR("csi_shm_path not found");
			exit(1);
		}
		strncpy(shm_path,v,sizeof(shm_path));
		v=pf->getEnv("csi_ref_path");
		if (!v)
		{
			ERROR("csi_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("csi_initial shm_path ref_path");
		exit(1);
	}
	DEBUG("csiinitial %s %s",shm_path,ref_path);
	CsiReference csi;
	if (csi.initial(ref_path,shm_path))
	{
		DEBUG("csiinitial ok");
		return 0;
	}
	ERROR("csiinitial error");
	return 1;
}



