/*
 * token.cpp
 *
 *  Created on: 2016��9��8��
 *      Author: Neusoft
 */

#include "../../ngx_auth/token/Token.h"

Token::Token() {
	// TODO Auto-generated constructor stub
}

Token::~Token() {
	// TODO Auto-generated destructor stub
}

bool Token::tokenCheck(char* marketID, const STOKEN stoken, char* sessionID) {
	if(!validityCheck(stoken.tokenNotBefore, stoken.tokenNotAfter))
	{
		ERROR("beyond the validity:[%s - %s]",stoken.tokenNotBefore, stoken.tokenNotAfter);
		return false;
	}
	if(!marketCheck(marketID, stoken.market))
	{
		ERROR("out of authority:[%s]",stoken.market);
		return false;
	}
	if(!signVerify(stoken))
	{
		ERROR("varify sign failed");
		return false;
	}
	if(!generateSessioID(stoken, sessionID))
	{
		ERROR("generateSessioID failed");
		return false;
	}
	return true;
}

bool Token::validityCheck(const char* tokenNotBefore,	const char* tokenNotAfter) {
	char currentDate[32] = {0};
	time_t timeSec = time(NULL);
	struct tm *timeinfo = localtime(&timeSec);
	sprintf(currentDate,"%d-%d-%d %d:%d:%d", timeinfo->tm_year+1900,timeinfo->tm_mon+1,
				timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	if(strcmp(tokenNotBefore, currentDate) <= 0 && strcmp(tokenNotAfter, currentDate) >= 0)
		return true;
	return false;
}

bool Token::marketCheck(const char* queryMarket, const char* allowedmarket) {
	if(!strstr(allowedmarket, queryMarket))
		return false;
	return true;
}

bool Token::signVerify(const STOKEN stoken) {
	return true;
}

bool Token::generateSessioID(const STOKEN stoken,char *sessionID) {
	MD5 md5;
	char md5key[128];
	memset(md5key, 0, sizeof(md5key));
	md5.MD5_String((char *)&stoken, md5key);
	if(md5key[0] == 0)
	{
		return false;
	}
	strncpy(sessionID, md5key, SESSIONLEN);
	return true;
}


