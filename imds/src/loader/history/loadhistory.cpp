/*
 * loadhistory.cpp
 *
 *  Created on: 2016/10/16/
 *      Author: user
 */

#include "../../public/history/HistoryLoad.h"
#include "../../public/program/Profile.h"

int main(int argc, char* argv[]) {
	char market_name[64];
	memset(market_name, 0, 64);
	char program[64];
	memset(program, 0, 64);
	char shm_path[256];
	memset(shm_path, 0, 256);
	char his_path[256];
	memset(his_path, 0, 256);
	int max_mink = 0;
	Profile* pf = NULL;
	const char* v = NULL;
	int date = 0;
	bool isAll = false;

	if (argc == 2 || argc == 3) {
		strcpy(market_name, argv[1]);
		sprintf(program, "%stransfer", market_name);
		pf = Profile::Instance(program);
		sprintf(program, "%s_his_shm_path", market_name);
		v = pf->getEnv(program);
		if (v == NULL) {
			//ERROR("%s not found", program);
			exit(1);
		}
		strcpy(shm_path, v);

		sprintf(program, "%s_his_path", market_name);
		v = pf->getEnv(program);
		if (v == NULL) {
			//ERROR("%s not found", program);
			exit(1);
		}
		strcpy(his_path, v);

		sprintf(program, "%s_max_mink", market_name);
		v = pf->getEnv(program);
		if (v == NULL) {
			//ERROR("%s not found", program);
			exit(1);
		}
		max_mink = atoi(v);

		if (argc == 2) {
			//current day
			time_t t;
			time(&t);
			struct tm *pt = localtime(&t);
			char chdate[9];
			snprintf(chdate, 9, "%4.4d%2.2d%2.2d", pt->tm_year + 1900, pt->tm_mon + 1,
					pt->tm_mday);
			date = atoi(chdate);
		}
		else {
			if (strcmp(argv[2], "all") == 0) {
				isAll = true;
			}
			else {
				date = atoi(argv[2]);
			}
		}
	}
	else {
		//ERROR("usage: ./historyload market_name <date | all>");
		exit(1);
	}

	CShmEnv* shmEnv = new CShmEnv();
	if (shmEnv->openShmEnv(shm_path) != SUCCEED) {
		//ERROR("openShmEnv(%s) error", shm_path);
		exit(1);
	}

	HistoryLoad* his = new HistoryLoad(shmEnv, market_name, his_path);
	if (his == NULL) {
		//ERROR("create protocol factory failed!");
		exit(1);
	}

	if (isAll) {
		his->Clear(); //clear all first
		his->loadAll();
		his->loadAll(max_mink);
	}
	else {
		his->load(date);
		his->load(date, max_mink);
	}

	return 0;
}


