/*
 * ngx_auth_mqws.cpp
 *
 *  Created on: 2016��9��6��
 *      Author: Neusoft
 */

#include "../auth/ngx_auth_mqws.h"

ngx_auth_mqws* ngx_auth_mqws::pInst = 0;

extern "C" int authCheck(const int cookies, const char *requestLine, char* encodeToken, char *sessionID, const char*args)
{
	char request[1024] = {0};
	int i = 0;

	strncpy(request, requestLine, sizeof(request));
	char *market = strtok(request, "/");
	while(market != NULL && ++i < 3)
	{
		market = strtok(NULL, "/");
	}
	if(ngx_auth_mqws::getInstant()->authCheck(cookies, market, encodeToken, sessionID, args))
		return 1;
	else
		return 0;

}

ngx_auth_mqws::ngx_auth_mqws() {
	// TODO Auto-generated constructor stub
	init();
}

ngx_auth_mqws::~ngx_auth_mqws() {
	// TODO Auto-generated destructor stub
}

ngx_auth_mqws *ngx_auth_mqws::getInstant() {
	if(!pInst) {
		pInst = new ngx_auth_mqws();
	}
	return pInst;
}

void ngx_auth_mqws::init() {

	Profile* pf = Profile::Instance("auth");
	const char* v;
	v = pf->getEnv("session_shm_path");
	strncpy(session_shm_path, v, sizeof(session_shm_path));
	v = pf->getEnv("session_shm_size");
	size = atoi(v)*1024LL*1024LL*1024LL;
	INFO("shm_path:[%s],shm_size:%d",session_shm_path,atoi(v));
}

bool ngx_auth_mqws::load()
{
	if(shmEnv.openShmEnv(session_shm_path) == SUCCEED)
	{
		return true;
	}
	else
	{
		if(shmEnv.createShmEnv(session_shm_path, size) == SUCCEED)
		{
			CShmMap *shmMap= shmEnv.getShmMap("sessionMap", sizeof(SESSION), 100);
			if(!shmMap)
			{
				ERROR("create shmMap failed");
				return false;
			}
		}
		else
		{
			ERROR("%s create failed",session_shm_path);
			return false;
		}
	}
   return true;
}

bool ngx_auth_mqws::authCheck(const int cookies, char* market,
		char* encodeToken, char* sessionID, const char* args)
{
	Base64 base64;
	char decode[TOKENLEN] = {0};
	Token token;
	Session session;

	if(cookies)
	{
		int len = strlen(encodeToken);
		if(!len)
		{
			ERROR("size of token is 0");
			return false;
		}
		if(-1 == base64.Decode(encodeToken, decode, len))
		{
			ERROR("decode base64 failed");
			return false;
		}
		INFO("base64 decode success");
		if(!parseToken(decode))
		{
			ERROR("parse token failed");
			return false;
		}
		INFO("parse token success");
		if(!token.tokenCheck(market,stoken,sessionID))
		{
			ERROR("token check failed");
			return false;
		}
		INFO("token check success");
		if(!load())
		{
			ERROR("load %s failed",session_shm_path);
			return false;
		}
		INFO("load %s success",session_shm_path);
		if(!update(sessionID))
		{
			ERROR("update sessionID(%s) failed",sessionID);
			release();
			return false;
		}
		INFO("update sessionID(%s) success",sessionID);
	}
	else
	{
		/*sessionID=4079a99e70e46d437d759a9833495238*/
		char Args[1024] = {0};
		int i = 0;
		char *psessionID = sessionID;

		strncpy(Args, args, sizeof(Args));
		char *ptr = strstr(Args, "sessionID");
		if(!ptr)
		{
			ERROR("args error");
			return false;
		}
		ptr+=10;
		while(i++<32)
		{
			*(psessionID++) = *(ptr++);
		}
		INFO("sessionID = [%s]",sessionID);
		if(!session.checkSession(market, sessionID, session_shm_path))
		{
			ERROR("session check failed");
			return false;
		}
	}
	INFO("pass");
	shmEnv.closeShmEnv();
	return true;
}

bool ngx_auth_mqws::update(char *sessionID) {
	SESSION session;
	CShmMap *shmmap = shmEnv.getShmMap("sessionMap");
	if(shmmap == NULL)
	{
		ERROR("getshmmap failed");
		return false;
	}

	strncpy(session.tokenNotBefore, stoken.tokenNotBefore, sizeof(session.tokenNotBefore));
	strncpy(session.tokenNotAfter, stoken.tokenNotAfter, sizeof(session.tokenNotAfter));
	strncpy(session.brokerNo, stoken.brokerNo, sizeof(session.brokerNo));
	strncpy(session.infoNo, stoken.infoNo, sizeof(session.infoNo));
	strncpy(session.clientId, stoken.clientId, sizeof(session.clientId));
	strncpy(session.userName, stoken.userName, sizeof(session.userName));
	strncpy(session.market, stoken.market, sizeof(session.market));
	strncpy(session.sessionID, sessionID, sizeof(session.sessionID));

	if(!shmmap->enter(session.sessionID, (char *)&session))
	{
		ERROR("enter session failed");
		return false;
	}
	return true;
}

void ngx_auth_mqws::release()
{
	shmEnv.releaseShmEnv();
}

bool ngx_auth_mqws::parseToken(char* decodeToken)
{
	string out;
	int i=0;
	Json::Reader reader;
	Json::Value  subvalue;

	memset(&stoken, 0x00, sizeof(STOKEN));

	if(reader.parse(decodeToken, subvalue))
	{
		int ret = subvalue["ret"].asInt();
		if(ret)
		{
			ERROR("the return value of token is %d",ret);
			return false;
		}
		Json::Value subsubvalue = subvalue["token"];
		out = subsubvalue["tokenNotBefore"].asString();
		strncpy(stoken.tokenNotBefore, out.c_str(), sizeof(stoken.tokenNotBefore)-1);

		out = subsubvalue["tokenNotAfter"].asString();
		strncpy(stoken.tokenNotAfter, out.c_str(), sizeof(stoken.tokenNotAfter)-1);

		out = subsubvalue["brokerNo"].asString();
		strncpy(stoken.brokerNo, out.c_str(), sizeof(stoken.brokerNo)-1);

		out = subsubvalue["infoNo"].asString();
		strncpy(stoken.infoNo, out.c_str(), sizeof(stoken.infoNo)-1);

		out = subsubvalue["cert"].asString();
		strncpy(stoken.cert, out.c_str(), sizeof(stoken.cert)-1);

		out = subsubvalue["clientId"].asString();
		strncpy(stoken.clientId, out.c_str(), sizeof(stoken.clientId)-1);

		out = subsubvalue["userName"].asString();
		strncpy(stoken.userName, out.c_str(), sizeof(stoken.userName)-1);

		Json::Value array = subsubvalue["market"];
		for(unsigned int i=0; i < array.size(); i++)
		{
			out = array[i]["marketId"].asString();
			strncpy(stoken.market+strlen(stoken.market), out.c_str(), sizeof(stoken.market)-1);
			if(i<array.size()-1) strcat(stoken.market,",");
		}
		out  = subsubvalue["sign"].asString();
		strncpy(stoken.sign, out.c_str(), sizeof(stoken.sign)-1);
	}
	else
	{
		return false;
	}

	INFO("tokenNotBefore = [%s]",stoken.tokenNotBefore);
	INFO("tokenNotAfter = [%s]",stoken.tokenNotAfter);
	INFO("brokerNo = [%s]",stoken.brokerNo);
	INFO("infoNo = [%s]",stoken.infoNo);
	INFO("clientId = [%s]",stoken.clientId);
	INFO("userName = [%s]",stoken.userName);
	INFO("market = [%s]",stoken.market);
	INFO("sign = [%s]",stoken.sign);

	return true;
}
