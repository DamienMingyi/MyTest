/*
 * Sh1HistoryQuotHandle.h
 *
 *  Created on: 2016年10月26日
 *      Author: user
 */

#ifndef SRC_SH1_HISTORY_SH1HISTORYQUOTHANDLE_H_
#define SRC_SH1_HISTORY_SH1HISTORYQUOTHANDLE_H_

#include "../quothandle/Sh1QuotHandle.h"
#include "../../public/history/HisDataDef.h"

class Sh1HistoryQuotHandle: public Sh1QuotHandle {
public:
	Sh1HistoryQuotHandle(const char* path, uint32_t date);
	virtual ~Sh1HistoryQuotHandle();
protected:
	virtual void update(char* data, QuotType qt);

private:
	void InitEnv();
	void StoreSnap(string code, boost::shared_ptr<HIS_SNAP_L1> &sp);
	void DayCandle(string code);
	void StoreDay(string code);
	void MinuteCandle(string code);
	void StoreMinute(string code);

private:
	string m_path;
	uint32_t m_date;
	bool m_flag;
	SH1_SNAP_MAP_VECTOR m_mvSnap;
	CANDLESTICK_MAP_VECTOR m_mvMinute;
	CANDLESTICK_MAP m_mDay;
	fstream m_snapStream;
	fstream m_dayStream;
	fstream m_minStream;
};

#endif /* SRC_SH1_HISTORY_SH1HISTORYQUOTHANDLE_H_ */
