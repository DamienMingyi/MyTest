/**
 * @file    Trd1Reply.h
 * @brief   Ӧ����ʳɽ�
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-25
 */

#ifndef TRD1REPLY_H_
#define TRD1REPLY_H_

#include "HttpReply.h"
/**
 * @class Trd1Reply 
 * @brief Ӧ����ʳɽ�
 * @note
 * �����URLΪ/v1/market/trd1/code?begin=-10&end=-1
 */
class Trd1Reply: public HttpReply {
public:
	Trd1Reply();
	virtual ~Trd1Reply();

protected:
	virtual bool  parse(const char* script,const char* query);
	virtual int  getTradeString(char* buffer,char* trd)=0;

	int reply(char* buffer,int& len);
	vector<char*> trd1;///<��ʳɽ�
};

#endif /* TRD1REPLY_H_ */
