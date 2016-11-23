/*
 * ngxaudit.cpp
 *
 *  Created on: 2016年11月4日
 *      Author: mds
 */

#include "ngxaudit.h"
#include "../../ngx_addon/ngx_http_audit_log_module/ngx_service_audit.h"

ngxaudit* ngxaudit::pngxaudit = 0;

extern "C" int generateAuditElement(const int cookies,char* encodeToken,
			char* args, const int type, char *value)
{
	int i=0;
	char* ptr;
	char sessionID[SESSIONLEN+1] = {0};
	if(!cookies)
	{
		ptr = strstr(args,"sessionID");
		if(ptr)
		{
			ptr+=10;
			while(i<32 )
			{
				sessionID[i++] = *(ptr++);
			}
		}else{
			return 0;
		}
	}
	if(ngxaudit::getInstant()->generateAuditElement(cookies, encodeToken, sessionID,type,value))
		return 1;
	else
		return 0;
}

ngxaudit::ngxaudit() {
	// TODO Auto-generated constructor stub
	init();
}

ngxaudit::~ngxaudit() {
	// TODO Auto-generated destructor stub
}


ngxaudit* ngxaudit::getInstant() {
	if(!pngxaudit){
		pngxaudit = new ngxaudit();
	}
	return pngxaudit;
}

void ngxaudit::init() {

	Profile* pf = Profile::Instance("auth");
	const char* v;
	v = pf->getEnv("session_shm_path");
	strncpy(session_shm_path, v, sizeof(session_shm_path));
	INFO("path=[%s]",session_shm_path);

}

bool ngxaudit::generateAuditElement(const int cookies, char* encodeToken,
		char* sessionID, const int type,char* value)
{
	Base64 base64;
	char decode[TOKENLEN] = {0};
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
		INFO("base63 decode success");
		if(!getvalueFromToken(decode, type, value))
		{
			ERROR("get type(%d) value failed",type);
			return false;
		}
		INFO("get type(%d) success,value = %s",type, value);
		return true;
	}
	else
	{
		if(!getvalueFromSession(sessionID, type, value))
			return false;
		return true;
	}
	return true;
}

bool ngxaudit::getvalueFromToken(char* token, const int type, char *value)
{
	string out;
	Json::Reader reader;
	Json::Value  subvalue;

	if(reader.parse(token, subvalue))
	{
		int ret = subvalue["ret"].asInt();
		if(ret)
		{
			ERROR("the return value of token is %d",ret);
			return false;
		}
		Json::Value subsubvalue = subvalue["token"];
		switch (type)
		{
		case 1:
			out = subsubvalue["brokerNo"].asString();
			break;
		case 2:
			out = subsubvalue["infoNo"].asString();
			break;
		case 3:
			out = subsubvalue["clientId"].asString();
			break;
		case 4:
			out = subsubvalue["userName"].asString();
			break;
		default:
			ERROR("type illegal");
			return false;
		}
		strncpy(value, out.c_str(), sizeof(value));
		return true;
	}
	else
	{
		return false;
	}
}

bool ngxaudit::getvalueFromSession(char*sessionID,const int type, char* value)
{
	SESSION *tmpsession;

	if(!strlen(sessionID))
	{
		ERROR("sessionId is null");
		return false;
	}

	if(shmEnv.openShmEnv(session_shm_path) != SUCCEED)
	{
		ERROR("%s open failed", session_shm_path);
		return false;
	}
	CShmMap *shmmap = shmEnv.getShmMap("sessionMap");
	if(shmmap == NULL)
	{
		ERROR("get sessionMap failed");
		return false;
	}

	tmpsession = (SESSION *)shmmap->find(sessionID);
	if(!tmpsession)
	{
		ERROR("not find session(%s)",sessionID);
		return false;
	}

	switch(type)
	{
	case 1:
		strncpy(value, tmpsession->brokerNo, sizeof(value));
		break;
	case 2:
		strncpy(value, tmpsession->infoNo, sizeof(value));
		break;
	case 3:
		strncpy(value, tmpsession->clientId, sizeof(value));
		break;
	case 4:
		strncpy(value, tmpsession->userName, sizeof(value));
		break;
	default:
		ERROR("type illegal");
		return false;
	}
	return true;
}
