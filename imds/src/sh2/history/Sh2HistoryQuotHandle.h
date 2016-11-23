/*
 * Sh2HistoryQuotHandle.h
 *
 *  Created on: 2016年11月2日
 *      Author: user
 */

#ifndef SRC_SH2_HISTORY_SH2HISTORYQUOTHANDLE_H_
#define SRC_SH2_HISTORY_SH2HISTORYQUOTHANDLE_H_

#include "../quothandle/Sh2QuotHandle.h"
#include "../../public/history/HisDataDef.h"

class Sh2HistoryQuotHandle: public Sh2QuotHandle {
public:
	Sh2HistoryQuotHandle(const char* path, uint32_t date);
	virtual ~Sh2HistoryQuotHandle();
protected:
	virtual void update(char* data, QuotType qt);

private:
	void InitEnv();
	void StoreSnap(string code, boost::shared_ptr<HIS_SNAP_L2> &sp);
	void StoreTick(string code, boost::shared_ptr<HIS_TICK> &sp);
	void StoreAuction(string code, boost::shared_ptr<HIS_AUCTION> &sp);
	void DayCandle(string code);
	void StoreDay(string code);
	void MinuteCandle(string code);
	void StoreMinute(string code);

private:
	string m_path;
	uint32_t m_date;
	uint32_t m_codeNum;
	bool m_flag;
	map<string, bool> m_mIsClose;
	SH2_SNAP_MAP_VECTOR m_mvSnap;
	CANDLESTICK_MAP_VECTOR m_mvMinute;
	CANDLESTICK_MAP m_mDay;
	fstream m_snapStream;
	fstream m_tickStream;
	fstream m_aucStream;
	fstream m_dayStream;
	fstream m_minStream;
};

#endif /* SRC_SH2_HISTORY_SH2HISTORYQUOTHANDLE_H_ */

