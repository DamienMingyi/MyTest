/*
 * session.h
 *
 *  Created on: 2016��9��8��
 *      Author: Neusoft
 */
#include "../../../../util/shm/CShmEnv.h"
#include "../../../../public/program/Profile.h"
#include "../../../../public/program/Logger.h"
#ifndef SESSION_H_
#define SESSION_H_
#include "../../../ngx_addon/ngx_http_add_auth_module/ngx_auth.h"

class Session {
public:
	Session();
	virtual ~Session();
	bool checkSession(const char* market,char* sessionID, char* shm_path);
private:
	bool loadSession(char* shm_path, char* sessionID);
	bool validityCheck(const char* tokenNotBefore, const char* tokenNotAfter);
	bool marketCheck(const char* queryMarket, const char* allowedmarket);
private:
	CShmEnv shmEnv;
	SESSION *session;
};

#endif /* SESSION_H_ */
