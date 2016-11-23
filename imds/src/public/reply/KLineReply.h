/**
 * @file    KLineReply.h
 * @brief   Ӧ����K������
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-10
 */

#ifndef KLINEREPLY_H_
#define KLINEREPLY_H_

#include "HttpReply.h"
#include "../../util/shm/CShmVector.h"

/**
 * @class KLineReply 
 * @brief Ӧ��K������
 * @note
 * ���г���K�߽ṹ��ͬ
 */
class KLineReply: public HttpReply {
public:
	KLineReply(CShmEnv* shmEnv,const char* prefix);
	virtual ~KLineReply();

	virtual int request(const char* script,const char* query,char* buffer,int& len);

protected:
	virtual int  getKLineString(char* buffer,const char* select,CANDLESTICK& kl);
	virtual void getTodayKLine()=0;
	virtual void getKLine()=0;
	virtual int reply(char* buffer,int& len);
	CShmEnv* shmEnv;
	CShmVector* kvec;
	SNAP_INDEX* si;
	string dftPeriod;
	string prefix;
	vector<CANDLESTICK> kline;
	vector<CANDLESTICK> ksrc;
	int today;
};

#endif /* KLINEREPLY_H_ */
