/*
 * HistoryLoad.h
 *
 *  Created on: 2016/10/14/
 *      Author: user
 */

#ifndef SRC_PUBLIC_HISTORY_HISTORYLOAD_H_
#define SRC_PUBLIC_HISTORY_HISTORYLOAD_H_

#include "../../util/shm/CShmEnv.h"
#include "HisDataDef.h"
#include "../market/ISnap.h"

typedef map<string, map<uint64_t, boost::shared_ptr<CANDLESTICK> > > SHM_CANDLESTICK_MAP_MAP;
typedef map<string, boost::shared_ptr<CANDLESTICK> > SHM_CANDLESTICK_MAP;

class HistoryLoad {
public:
	HistoryLoad(CShmEnv* shmEnv, char* shmname, char* hispath);
	virtual ~HistoryLoad();

	/** **/
	virtual void load(uint32_t date);

	virtual void loadAll();

	virtual void load(uint32_t date, uint32_t limit);

	virtual void loadAll(uint32_t limit);

	void Clear();

private:
	void Convert2BinaryDay(string& line);
	void Convert2BinaryMinute(string& line, uint32_t limit);

private:
	CShmEnv* m_shmEnv;
	string m_shmName;
	string m_hisPath;
	SHM_CANDLESTICK_MAP_MAP m_mmMinute;  //code--datetime--candlestick
	SHM_CANDLESTICK_MAP m_mDay;
};

#endif /* SRC_PUBLIC_HISTORY_HISTORYLOAD_H_ */
