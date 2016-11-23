/*
 * ngx_service.h
 *
 *  Created on: Oct 27, 2015
 *      Author: ygao@sse.com.cn
 */
#ifndef NGX_SERVICE_H_
#define NGX_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

 	 char* request(const char* uri,const char* args,int* status,int* reply_len);

#ifdef __cplusplus
}
#endif


#endif /* NGX_SERVICE_H_ */
