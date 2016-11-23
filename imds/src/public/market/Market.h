/**
 * @file    Market.h
 * @brief   ����г�����
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-20
 * @bug     ��
 * @warning ��
 */

#ifndef MARKET_H_
#define MARKET_H_

#include <string>
#include <map>
#include "ISnapShot.h"
#include "../../util/shm/CShmEnv.h"
#include "../../util/shm/CShmVector.h"
#include <vector>


typedef struct
{
	char* snap_item;
	CShmVector* lineVector;
	CShmVector* tradeVector;
	CShmVector* trade2Vector;
}
SNAP_INDEX;

class Market {
public:
	Market(const char* name,CShmEnv* shmEnv,ISnapShot* snap);
	virtual ~Market();
	bool load(int snap_size,int line_size,int trade_size, int trade2_size = 0);
	void refresh();
	int getSnapCount();
	char* getSnap(int pos);
	char* getSnap(const char* code);
	void initSnap(int pos,char* item);
	void update(char* snap_item);
	void updateTick(char* snap_item);
	SNAP_INDEX* getSnapIndex(const char* code);
	ISnapShot* getISnap();
	const char* getName();
protected:
	char* getItem(CShmVector* shmVector,int pos);
protected:
	char name[32];
	CShmEnv* shmEnv;
	CShmVector* snapVector;
	ISnapShot* snap;
	char* tmSnap;
	map<string,SNAP_INDEX> finder;
	int date;
	int init_time;
	int line_size;
	int trade_size;
	int trade2_size;
	SNAP_INDEX snap_idx;
};

Market* createMarket(const char* name,const char* shm_path,ISnapShot* ISnap);

int getDatetime(int line_time);
int getLineTime(int time);
int getHKDateTime(int lineTime);
int getHKLineTime(int time);
#endif /* MARKET_H_ */
