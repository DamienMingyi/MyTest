/*
 * shminitial.cpp
 *
 *  Created on: Jan 15, 2014
 *      Author: level2
 */
#include <stdio.h>
#include "../util/shm/CShmEnv.h"

int main(int argc,char* argv[])
{
	if (argc<3)
	{
		printf("argument error! usage:\r\n");
		printf("shmcreater create name size(G)\r\n");
		printf("shmcreater release name\r\n");
		printf("shmcreater show name");
		exit(1);
	}

	CShmEnv shmEnv;
	if (strcmp(argv[1],"create")==0)
	{
		if (argc<4)
		{
			printf("argument error! usage:\r\n");
			printf("shmcreater create name size(G)\r\n");
			exit(1);
		}
		if (shmEnv.openShmEnv(argv[2])==SUCCEED)
		{
			printf("%s already open\r\n",argv[2]);
		}
		else
		{
			shm_size_t size=atoi(argv[3])*1024LL*1024LL*1024LL;
			if (shmEnv.createShmEnv(argv[2],size)==SUCCEED)
			{
				printf("%s create succeed\r\n",argv[2]);
			}
			else
			{
				printf("%s create fail\r\n",argv[2]);
				exit(1);
			}
		}
	}
	else if (strcmp(argv[1],"release")==0)
	{
		if (shmEnv.openShmEnv(argv[2])!=SUCCEED)
		{
			printf("%s open fail\r\n",argv[2]);
			exit(1);
		}
		if (shmEnv.releaseShmEnv()==SUCCEED)
		{
			printf("%s release succeed\r\n",argv[2]);
		}
		else
		{
			printf("%s release fail\r\n",argv[2]);
			exit(1);
		}
	}
	else if (strcmp(argv[1],"show")==0)
	{
		if (shmEnv.openShmEnv(argv[2])!=SUCCEED)
		{
			printf("%s open fail\r\n",argv[2]);
			exit(1);
		}
		ST_SHM_ENV_INFO envInfo;
		if (!shmEnv.getEnvInfo(envInfo))
		{
			printf("can not getEnvInfo\r\n");
			exit(1);
		}
		SHM_OBJ_INFO_LIST objInfoList;
		if (!shmEnv.getShmObjInfo(objInfoList))
		{
			printf("can not getShmObjInfo\r\n");
			exit(1);
		}
		printf("shm:%s\r\nversion:%s\r\ncreate_date:%s\r\n",argv[2],envInfo.version.c_str(),envInfo.createDate.c_str());
		printf("shm_size=%ld shm_used_size=%ld used_ratio=%ld%% object_count=%d\r\n",envInfo.envCapacity,envInfo.envSize,envInfo.envSize*100/envInfo.envCapacity,objInfoList.size());

		for (unsigned int i=0;i<objInfoList.size();i++)
		{
			if (objInfoList[i].shmObjType==SHM_OBJ_LIST)
				printf("name:%s type:list occupied:%ld capacity:%ld size:%ld\r\n",\
						objInfoList[i].shmObjName.c_str(), objInfoList[i].occupied,objInfoList[i].capacity,objInfoList[i].size);
			else
				printf("name:%s type:vector occupied:%ld capacity:%ld  size:%ld\r\n",\
						objInfoList[i].shmObjName.c_str(), objInfoList[i].occupied,objInfoList[i].capacity,objInfoList[i].size);
		}
		printf("shm_size=%ld shm_used_size=%ld used_ratio=%ld%% object_count=%d\r\n",envInfo.envCapacity,envInfo.envSize,envInfo.envSize*100/envInfo.envCapacity,objInfoList.size());
	}
	else
	{
		printf("argument error! usage:\r\n");
		printf("shmcreater create name size(G)\r\n");
		printf("shmcreater release name\r\n");
		exit(1);
	}
	return 0;
}


