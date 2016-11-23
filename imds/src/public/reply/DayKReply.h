/**
 * @file    DayKReply.h
 * @brief   应答日K线行情
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-10
 */

#ifndef DAYKREPLY_H_
#define DAYKREPLY_H_

#include "KLineReply.h"
#include "../../util/shm/CShmEnv.h"
/**
 * @class DayKReply 
 * @brief 应答日K线行情
 * @note
 * 支持day,week,month,year和自定义天数
 */
class DayKReply: public KLineReply {
public:
	DayKReply(CShmEnv* shmEnv,const char* prefix);
	virtual ~DayKReply();

protected:
	virtual void getKLine();
	virtual void getTodayKLine();
};

#endif /* DAYKREPLY_H_ */
