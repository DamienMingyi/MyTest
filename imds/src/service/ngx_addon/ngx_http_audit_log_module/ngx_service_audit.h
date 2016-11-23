/*
 * ngx_service_audit.h
 *
 *  Created on: 2016年11月4日
 *      Author: mds
 */

#ifndef SRC_SERVICE_NGX_ADDON_NGX_HTTP_AUDIT_LOG_MODULE_NGX_SERVICE_AUDIT_H_
#define SRC_SERVICE_NGX_ADDON_NGX_HTTP_AUDIT_LOG_MODULE_NGX_SERVICE_AUDIT_H_

#define ENCODELEN 2048
#define BROKERNOLEN 32
#define INFONOLEN 32
#define CLINETIDLEN 128
#define USERNAMELEN 128
#ifdef __cplusplus
extern "C" {
#endif

 	 int generateAuditElement(const int cookies,char* encodeToken,
 			char* args, const int type, char *value);

#ifdef __cplusplus
}
#endif


#endif /* SRC_SERVICE_NGX_ADDON_NGX_HTTP_AUDIT_LOG_MODULE_NGX_SERVICE_AUDIT_H_ */
