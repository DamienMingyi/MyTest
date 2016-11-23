/**
 * @file    SnapReply.h
 * @brief   Ӧ�����
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-3
 */

#ifndef SNAPREPLY_H_
#define SNAPREPLY_H_

#include "HttpReply.h"
/**
 * @class SnapReply 
 * @brief Ӧ����ʳɽ�
 * @note
 * �����URLΪ/v1/market/snap/code?select=*
 */
class SnapReply: public HttpReply {
public:
	SnapReply();
	virtual ~SnapReply();

protected:
	virtual bool  parse(const char* script,const char* query);
	virtual int  getSnapString(char* buffer,const char* select)=0;

	virtual int reply(char* buffer,int& len);
	char* snap;///<���յ�ַ
};

#endif /* SNAPREPLY_H_ */
