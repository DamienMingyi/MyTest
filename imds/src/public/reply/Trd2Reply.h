/**
 * @file    Trd2Reply.h
 * @brief   Ӧ��ּ۳ɽ�
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-25
 */

#ifndef TRD2REPLY_H_
#define TRD2REPLY_H_

#include "HttpReply.h"

/**
 * @class Trd2Reply 
 * @brief Ӧ��ּ۳ɽ�
 * @note
 * �����URLΪ/v1/market/trd2/code���۸�Ӹߵ��ͣ����ذ��۸����ĳɽ�ͳ��
 */
class Trd2Reply: public HttpReply {
public:
	Trd2Reply();
	virtual ~Trd2Reply();
protected:
	virtual bool parse(const char* script,const char* query);

	int reply(char* buffer,int& len);
	map<int,long,greater<int> > trd2;///<�ּ۳ɽ�

};

#endif /* TRD2REPLY_H_ */
