/*
 * ngx_access.h
 *
 *  Created on: 2016��9��7��
 *      Author: Neusoft
 */

#ifndef NGX_ACCESS_H_
#define NGX_ACCESS_H_

#define ENCODELEN 2048
#define TOKENLEN  1024
#define SESSIONLEN 32

typedef struct
{
	char tokenNotBefore[32];	/*签发日期*/
	char tokenNotAfter[32];		/*截止日期*/
	char brokerNo[32];				/*券商标识号*/
	char infoNo[32];					/*信息商标识号*/
	char cert[16];						/*证书标识*/
	char clientId[128];       /*客户端唯一码*/
	char userName[128];       /*用户名*/
	char market[64];          /*市场列表*/
	char sign[256];           /*签名*/
}STOKEN;

typedef struct {
	char sessionID[32+1];
	char tokenNotBefore[32];
	char tokenNotAfter[32];
	char brokerNo[32];
	char infoNo[32];
	char clientId[128];
	char userName[128];
	char market[64];
} SESSION;

#ifdef __cplusplus
extern "C" {
#endif
	int authCheck(const int cookies,const char *requestLine, char* encodeToken, char *sessionID, const char* args);
#ifdef __cplusplus
}
#endif



#endif /* NGX_ACCESS_H_ */
