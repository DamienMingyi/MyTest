/*
 * token.h
 *
 *  Created on: 2016��9��8��
 *      Author: Neusoft
 */
#include <cstring>
#include "../../../../util/shm/CShmEnv.h"
#include "../../../../util/MD5/MD5.h"
#include "../../../../public/program/Profile.h"
#include "../../../../public/program/Logger.h"
#include "../../../ngx_addon/ngx_http_add_auth_module/ngx_auth.h"
#include "../../ngx_auth/session/Session.h"

#ifndef TOKEN_H_
#define TOKEN_H_

class Token {
public:
	Token();
	virtual ~Token();
	bool tokenCheck(char* marketID, const STOKEN stoken, char* sessionID);
private:
	bool validityCheck(const char* tokenNotBefore, const char* tokenNotAfter);
	bool marketCheck(const char* queryMarket, const char* allowedmarket);
	bool signVerify(const STOKEN stoken);
	bool generateSessioID(const STOKEN stoken, char* sessionID);
};

#endif /* TOKEN_H_ */
