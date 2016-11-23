/*
 * Sh1HistoryQuotHandle.cpp
 *
 *  Created on: 2016年10月26日
 *      Author: user
 */

#include "Sh1HistoryQuotHandle.h"
#include <sys/stat.h>
#include <dirent.h>

Sh1HistoryQuotHandle::Sh1HistoryQuotHandle(const char* path, uint32_t date)
	: m_path(path), m_date(date), m_flag(false) {
	// TODO Auto-generated constructor stub
	InitEnv();
}

Sh1HistoryQuotHandle::~Sh1HistoryQuotHandle() {
	// TODO Auto-generated destructor stub
	m_snapStream.close();
	m_dayStream.close();
	m_minStream.close();

	if (snap0 != NULL) {
		delete snap0;
		snap0 = NULL;
	}
}

void Sh1HistoryQuotHandle::InitEnv() {
	ostringstream oss;
	oss << m_path << "/" << m_date;
	string datePath(oss.str());
	if (access(datePath.c_str(), F_OK) < 0) {
		//create
		if (mkdir(datePath.c_str(), 0755) < 0) {
			ERROR("mkdir %s failed!", datePath.c_str());
			return;
		}
	}

	string filePath = datePath + "/Snapshot.csv";
	m_snapStream.open(filePath.c_str(), ios::out | ios::ate);
	if (!m_snapStream.is_open()) {
		ERROR("open %s failed!", filePath.c_str());
		return;
	}

	filePath = datePath + "/Day.csv";
	m_dayStream.open(filePath.c_str(), ios::out | ios::ate);
	if (!m_dayStream.is_open()) {
		ERROR("open %s failed!", filePath.c_str());
		return;
	}

	filePath = datePath + "/Minute.csv";
	m_minStream.open(filePath.c_str(), ios::out | ios::ate);
	if (!m_minStream.is_open()) {
		ERROR("open %s failed!", filePath.c_str());
		return;
	}

	try {
		snap0 = new STOCKSNAP();
	}
	catch (const bad_alloc& e) {
		ERROR("create snap0 failed!");
		return;
	}
}

void Sh1HistoryQuotHandle::update(char* data, QuotType qt) {
	assert(data != NULL);
	STOCKSNAP oSnap = *(STOCKSNAP *) data;
	string sCode(oSnap.securityID);

	if ( (strncmp(oSnap.tradingSessionID, "T10", 3) == 0) && oSnap.lastPx >0) {
		boost::shared_ptr<HIS_SNAP_L1> sp = boost::make_shared<HIS_SNAP_L1>();
		sp->uiDateTime = (uint64_t) oSnap.date * 1000000 + oSnap.time / 100;
		sp->uiPreClosePx = oSnap.prevClosePx;
		sp->uiOpenPx = oSnap.openPx;
		sp->uiHighPx = oSnap.highPx;
		sp->uiLowPx = oSnap.lowPx;
		sp->uiLastPx = oSnap.lastPx;
		sp->uiVolume = oSnap.volume;
		sp->uiAmount = oSnap.amount;
		for (int i = 0; i < MAX_LEVEL_SIZE; ++i) {
			sp->arrBidPx[i] = oSnap.bid[i].price;
			sp->arrBidSize[i] = oSnap.bid[i].size;
			sp->arrOfferPx[i] = oSnap.offer[i].price;
			sp->arrOfferSize[i] = oSnap.offer[i].size;
		}
		sp->uiNumTrades = oSnap.numTrades;
		sp->uiIOPV = oSnap.iopv;
		sp->uiNAV = oSnap.nav;
		strcpy(sp->strPhaseCode, oSnap.tradingPhaseCode);
		if (m_mvSnap[sCode].empty()
				|| memcmp( (char*) m_mvSnap[sCode].back().get() + sizeof(uint64_t),
						(char*) sp.get() + sizeof(uint64_t),
						sizeof(HIS_SNAP_L1) - sizeof(uint64_t)) != 0) {
			m_mvSnap[sCode].push_back(sp);
			StoreSnap(sCode, sp);
		}
	}

	if (!m_flag && (strncmp(oSnap.tradingSessionID, "E11", 3) == 0)) {
		for (SH1_SNAP_MAP_VECTOR::const_iterator cit = m_mvSnap.begin(); cit != m_mvSnap.end(); ++cit) {
			if (atoi(cit->first.c_str()) <= 999) {
				DayCandle(cit->first);
				StoreDay(cit->first);
				MinuteCandle(cit->first);
				StoreMinute(cit->first);
			}
		}

		m_flag = true;
	}
}

void Sh1HistoryQuotHandle::StoreSnap(string code, boost::shared_ptr<HIS_SNAP_L1> &sp) {
	//write
	m_snapStream << code << "," << sp->uiDateTime << ","
			<< boost::format("%.3f") % ((float)sp->uiPreClosePx / 1000) << ","
			<< boost::format("%.3f") % ((float)sp->uiOpenPx / 1000) << ","
			<< boost::format("%.3f") % ((float)sp->uiHighPx / 1000) << ","
			<< boost::format("%.3f") % ((float)sp->uiLowPx / 1000) << ","
			<< boost::format("%.3f") % ((float)sp->uiLastPx / 1000) << ","
			<< sp->uiVolume / 100 << ","
			<< boost::format("%.2f") % ((float)sp->uiAmount) << ",";
	for (int i = 0; i < MAX_LEVEL_SIZE; ++i) {
		m_snapStream << boost::format("%.3f") % ((float)sp->arrBidPx[i] / 1000) << ",";
	}
	for (int i = 0; i < MAX_LEVEL_SIZE; ++i) {
		m_snapStream << sp->arrBidSize[i] / 100 << ",";
	}
	for (int i = 0; i < MAX_LEVEL_SIZE; ++i) {
		m_snapStream << boost::format("%.3f") % ((float)sp->arrOfferPx[i] / 1000) << ",";
	}
	for (int i = 0; i < MAX_LEVEL_SIZE; ++i) {
		m_snapStream << sp->arrOfferSize[i] / 100 << ",";
	}
	m_snapStream << sp->uiNumTrades << ","
			<< boost::format("%.3f") % ((float)sp->uiIOPV / 1000) << ","
			<< boost::format("%.3f") % ((float)sp->uiNAV / 1000) << ","
			<< sp->strPhaseCode << endl;
}

void Sh1HistoryQuotHandle::DayCandle(string code) {
	if (m_mDay.find(code) == m_mDay.end()) {
		m_mDay[code] = boost::make_shared<HIS_CANDLESTICK>();
		m_mDay[code]->uiDateTime = m_date;
		m_mDay[code]->uiPreClosePx = m_mvSnap[code].back()->uiPreClosePx;
		m_mDay[code]->uiOpenPx = m_mvSnap[code].back()->uiOpenPx;
		m_mDay[code]->uiHighPx = m_mvSnap[code].back()->uiHighPx;
		m_mDay[code]->uiLowPx = m_mvSnap[code].back()->uiLowPx;
		m_mDay[code]->uiLastPx = m_mvSnap[code].back()->uiLastPx;
		m_mDay[code]->uiVolume = m_mvSnap[code].back()->uiVolume;
		m_mDay[code]->uiAmount = m_mvSnap[code].back()->uiAmount;
		m_mDay[code]->iMF = m_mvSnap[code].back()->uiIOPV;
	}
}

void Sh1HistoryQuotHandle::MinuteCandle(string code) {
    uint64_t preVolume = 0;
    uint64_t preAmount = 0;
    uint64_t preHighPx = 0;
    uint64_t preLowPx = 0;

    for (vector<boost::shared_ptr<HIS_SNAP_L1> >::const_iterator const_it =
    		m_mvSnap[code].begin(); const_it != m_mvSnap[code].end(); ++const_it) {
    	int uiTime = (*const_it)->uiDateTime % 1000000;
    	if (uiTime < 92500)
    		continue;

    	if (m_mvMinute[code].size() - 1 != getLineTime(uiTime)) {
    		if (m_mvMinute[code].size() <= getLineTime(uiTime)) {
    			while (m_mvMinute[code].size() < getLineTime(uiTime)) {
    				// no data, fill the blank
    				boost::shared_ptr<HIS_CANDLESTICK> sp = boost::make_shared<HIS_CANDLESTICK>();
    				sp->uiDateTime = (uint64_t) m_date * 1000000
    						+ getDatetime(m_mvMinute[code].size());
    				sp->uiPreClosePx = (*const_it)->uiPreClosePx;
    				sp->uiOpenPx = sp->uiHighPx = sp->uiLowPx = sp->uiLastPx
    						= m_mvMinute[code].empty() ? sp->uiPreClosePx : m_mvMinute[code].back()->uiLastPx;
    				if (m_mvMinute[code].empty())
    					preHighPx = preLowPx = sp->uiOpenPx;
    				sp->uiVolume = sp->uiAmount = sp->iMF = 0;
    				m_mvMinute[code].push_back(sp);
    			}
    			if (m_mvMinute[code].size() == getLineTime(uiTime)) {
    				// the first snap in the minute
    				boost::shared_ptr<HIS_CANDLESTICK> sp = boost::make_shared<HIS_CANDLESTICK>();
    				sp->uiDateTime = (uint64_t) m_date * 1000000
    						+ getDatetime(m_mvMinute[code].size());
    				sp->uiPreClosePx = (*const_it)->uiPreClosePx;
    				sp->uiOpenPx = sp->uiLastPx = (*const_it)->uiLastPx > 0
    						? (*const_it)->uiLastPx : (*const_it)->uiPreClosePx;
    				if (m_mvMinute[code].empty())
    					preHighPx = preLowPx = sp->uiOpenPx;

                    if ((*const_it)->uiHighPx > preHighPx) {
                            sp->uiHighPx = preHighPx = (*const_it)->uiHighPx;
                    }
                    else {
                            sp->uiHighPx = sp->uiOpenPx;
                    }

                    if ((*const_it)->uiLowPx < preLowPx) {
                            sp->uiLowPx = preLowPx = (*const_it)->uiLowPx;
                    }
                    else {
                            sp->uiLowPx = sp->uiOpenPx;
                    }

                    sp->uiVolume = (*const_it)->uiVolume - preVolume;
                    preVolume = (*const_it)->uiVolume;
                    sp->uiAmount = (*const_it)->uiAmount - preAmount;
                    preAmount = (*const_it)->uiAmount;
                    sp->iMF = (*const_it)->uiIOPV;
                    m_mvMinute[code].push_back(sp);
    			}
    		}
    		else {
    			//ERROR
    		}
    	}
    	else {
    		//the subsequent snaps in the minute
            if ((*const_it)->uiHighPx > preHighPx) {
            	m_mvMinute[code].back()->uiHighPx = (*const_it)->uiHighPx;
                    preHighPx = (*const_it)->uiHighPx;
            }
            else if ((*const_it)->uiLastPx > m_mvMinute[code].back()->uiHighPx) {
            	m_mvMinute[code].back()->uiHighPx = (*const_it)->uiLastPx;
            }

            if ((*const_it)->uiLowPx < preLowPx) {
            	m_mvMinute[code].back()->uiLowPx = (*const_it)->uiLowPx;
                    preLowPx = (*const_it)->uiLowPx;
            }
            else if ((*const_it)->uiLastPx < m_mvMinute[code].back()->uiLowPx) {
            	m_mvMinute[code].back()->uiLowPx = (*const_it)->uiLastPx;
            }

            m_mvMinute[code].back()->uiLastPx = (*const_it)->uiLastPx;
            m_mvMinute[code].back()->uiVolume +=
                            (*const_it)->uiVolume - preVolume;
            preVolume = (*const_it)->uiVolume;
            m_mvMinute[code].back()->uiAmount +=
                            (*const_it)->uiAmount - preAmount;
            preAmount = (*const_it)->uiAmount;
            m_mvMinute[code].back()->iMF = (*const_it)->uiIOPV;
    	}
    }

    if (m_mvMinute[code].size() != 241) {
            int diff = 241 - m_mvMinute[code].size();
            while (diff-- > 0) {
            	m_mvMinute[code].push_back(m_mvMinute[code].back());
            }
    }
}

void Sh1HistoryQuotHandle::StoreDay(string code) {
	//write
	m_dayStream << code << "," << m_mDay[code]->uiDateTime << ","
			<< boost::format("%.3f") % ((float) m_mDay[code]->uiPreClosePx / 1000) << ","
			<< boost::format("%.3f") % ((float) m_mDay[code]->uiOpenPx / 1000) << ","
			<< boost::format("%.3f") % ((float) m_mDay[code]->uiHighPx / 1000) << ","
			<< boost::format("%.3f") % ((float) m_mDay[code]->uiLowPx / 1000) << ","
			<< boost::format("%.3f") % ((float) m_mDay[code]->uiLastPx / 1000) << ","
			<< m_mDay[code]->uiVolume / 100 << ","
			<< boost::format("%.2f") % ((float) m_mDay[code]->uiAmount) << ","
			<< boost::format("%.3f") % ((float) m_mDay[code]->iMF / 1000) << endl;

}

void Sh1HistoryQuotHandle::StoreMinute(string code) {
	//write
	for (vector<boost::shared_ptr<HIS_CANDLESTICK> >::const_iterator cit =
			m_mvMinute[code].begin(); cit != m_mvMinute[code].end(); ++cit) {
		m_minStream << code << "," << (*cit)->uiDateTime << ","
				<< boost::format("%.3f") % ((float) (*cit)->uiPreClosePx / 1000) << ","
				<< boost::format("%.3f") % ((float) (*cit)->uiOpenPx / 1000) << ","
				<< boost::format("%.3f") % ((float) (*cit)->uiHighPx / 1000) << ","
				<< boost::format("%.3f") % ((float) (*cit)->uiLowPx / 1000) << ","
				<< boost::format("%.3f") % ((float) (*cit)->uiLastPx / 1000) << ","
				<< (*cit)->uiVolume / 100 << ","
				<< boost::format("%.2f") % ((float) (*cit)->uiAmount) << ","
				<< boost::format("%.3f") % ((float) (*cit)->iMF / 1000) << endl;
	}
}
