/**
 * @file    DayKReply.h
 * @brief   Ӧ����K������
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
 * @brief Ӧ����K������
 * @note
 * ֧��day,week,month,year���Զ�������
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
