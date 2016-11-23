/*
 * ngxaudit.h
 *
 *  Created on: 2016年11月4日
 *      Author: mds
 */

#ifndef SRC_SERVICE_NGX_SERVICE_NGX_AUDIT_NGXAUDIT_H_
#define SRC_SERVICE_NGX_SERVICE_NGX_AUDIT_NGXAUDIT_H_

#include "../../../util/base64/Base64.h"
#include "../../../util/shm/CShmEnv.h"
#include "../../../public/program/Profile.h"
#include "../../../public/program/Logger.h"
#include "../../../util/jsoncpp/include/json/json.h"
#include "../../ngx_addon/ngx_http_add_auth_module/ngx_auth.h"
#include <string.h>

using namespace std;

class ngxaudit {
public:
	ngxaudit();
	virtual ~ngxaudit();
	bool generateAuditElement(const int cookies,char* encodeToken,
			char* sessionID, const int type, char *value);
	static ngxaudit* getInstant();
private:
	void init();
	bool getvalueFromToken(char* token, const int type, char* value);
	bool getvalueFromSession(char* sessionID, const int type, char* value);
private:
	CShmEnv shmEnv;
	char session_shm_path[256] = {0};
	static ngxaudit* pngxaudit;
};

#endif /* SRC_SERVICE_NGX_SERVICE_NGX_AUDIT_NGXAUDIT_H_ */
