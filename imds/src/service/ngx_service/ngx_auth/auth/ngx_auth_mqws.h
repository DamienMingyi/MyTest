/*
 * ngx_access_mqws.h
 *
 *  Created on: 2016��9��6��
 *      Author: Neusoft
 */

#ifndef NGX_ACCESS_MQWS_H_
#define NGX_ACCESS_MQWS_H_

#include <cstring>
#include <iostream>
#include "../../../../public/program/Profile.h"
#include "../../../../public/program/Logger.h"
#include "../../../../util/shm/CShmEnv.h"
#include "../../../../util/base64/Base64.h"
#include "../../../../util/jsoncpp/include/json/json.h"
#include "../../../ngx_addon/ngx_http_add_auth_module/ngx_auth.h"
#include "../../ngx_auth/session/Session.h"
#include "../../ngx_auth/token/Token.h"

class ngx_auth_mqws {
public:
	ngx_auth_mqws();
	virtual ~ngx_auth_mqws();
	bool authCheck(const int cookies,char *market, char* encodeToken, char *sessionID, const char* args);
	static ngx_auth_mqws* getInstant();
private:
	void init();
	bool load();
	bool update(char *sessionID);
	void release();
	bool parseToken(char *decodeToken);
private:
	CShmEnv shmEnv;
	STOKEN  stoken;
	shm_size_t size;
	char session_shm_path[256] = {0};
	static  ngx_auth_mqws* pInst;
};

#endif /* NGX_ACCESS_MQWS_H_ */
