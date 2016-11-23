/**
 * @file    MinKReply.h
 * @brief   Ӧ�����K��
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
	/** @brief �õ����շ���K��*/
	virtual void getTodayKLine();
	/** @brief �õ����з�����*/
	void getKLine();
	/** @brief �õ�ǰ���շ�����*/
	void getBefore(int before,int date);
	/** @brief �õ�ָ���շ�����*/
	void getKLinebyDate(int date);
};

#endif /* MINKREPLY_H_ */
