/**
 * @file    Trd2Reply.h
 * @brief   应答分价成交
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-25
 */

#ifndef TRD2REPLY_H_
#define TRD2REPLY_H_

#include "HttpReply.h"

/**
 * @class Trd2Reply 
 * @brief 应答分价成交
 * @note
 * 处理的URL为/v1/market/trd2/code，价格从高到低，返回按价格分组的成交统计
 */
class Trd2Reply: public HttpReply {
public:
	Trd2Reply();
	virtual ~Trd2Reply();
protected:
	virtual bool parse(const char* script,const char* query);

	int reply(char* buffer,int& len);
	map<int,long,greater<int> > trd2;///<分价成交

};

#endif /* TRD2REPLY_H_ */
