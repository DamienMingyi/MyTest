/**
 * @file    MinKReply.h
 * @brief   应答分钟K线
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-10
 */

#ifndef MINKREPLY_H_
#define MINKREPLY_H_

#include "KLineReply.h"
#include "../../util/shm/CShmEnv.h"

class MinKReply: public KLineReply {
public:
	MinKReply(CShmEnv* shmEnv,const char* prefix);
	virtual ~MinKReply();

protected:
	/** @brief 得到当日分钟K线*/
	virtual void getTodayKLine();
	/** @brief 得到所有分钟线*/
	void getKLine();
	/** @brief 得到前几日分钟线*/
	void getBefore(int before,int date);
	/** @brief 得到指定日分钟线*/
	void getKLinebyDate(int date);
};

#endif /* MINKREPLY_H_ */
