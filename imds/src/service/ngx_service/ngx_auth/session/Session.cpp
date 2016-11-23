/*
 * session.cpp
 *
 *  Created on: 2016��9��8��
 *      Author: Neusoft
 */

#include "../../ngx_auth/session/Session.h"

Session::Session() {
	// TODO Auto-generated constructor stub
	session = NULL;
}

Session::~Session() {
	// TODO Auto-generated destructor stub
}

bool Session::checkSession(const char* market, char* sessionID, char* shm_path) {

	if(!loadSession(shm_path, sessionID))
		return false;
	if(!validityCheck(session->tokenNotBefore, session->tokenNotAfter))
	{
		ERROR("beyond the validity:[%s - %s]",session->tokenNotBefore, session->tokenNotAfter);
		return false;
	}
	if(!marketCheck(market, session->market))
	{
		ERROR("out of authority:[%s]",session->market);
		return false;
	}
	return true;
}

bool Session::loadSession(char* shm_path,char* sessionID) {
	if(shmEnv.openShmEnv(shm_path) != SUCCEED)
	{
		ERROR("%s open failed",shm_path);
		return false;
	}
	CShmMap *shmmap = shmEnv.getShmMap("sessionMap");
	if(shmmap == NULL)
	{
		ERROR("get shmmap failed");
		return false;
	}
	session = (SESSION *)shmmap->find(sessionID);
	if(session == NULL)
	{
		return false;
	}
	INFO("brokerNo = [%s]",session->brokerNo);
	INFO("clientId = [%s]",session->clientId);
	INFO("infoNo = [%s]",session->infoNo);
	INFO("market = [%s]",session->market);
	INFO("sessionID = [%s]",session->sessionID);
	INFO("tokenNotAfter = [%s]",session->tokenNotAfter);
	INFO("tokenNotBefore = [%s]",session->tokenNotBefore);
	INFO("userName = [%s]",session->userName);
	return true;
}

bool Session::validityCheck(const char* tokenNotBefore, const char* tokenNotAfter) {
	char currentDate[32] = {0};
	time_t timeSec = time(NULL);
	struct tm *timeinfo = localtime(&timeSec);
	sprintf(currentDate,"%d-%d-%d %d:%d:%d", timeinfo->tm_year+1900,timeinfo->tm_mon+1,
				timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	if(strcmp(tokenNotBefore, currentDate) <= 0 && strcmp(tokenNotAfter, currentDate) >= 0)
		return true;
	return false;
}

bool Session::marketCheck(const char* queryMarket, const char* allowedmarket) {
	if(!strstr(allowedmarket, queryMarket))
		return false;
	return true;
}
