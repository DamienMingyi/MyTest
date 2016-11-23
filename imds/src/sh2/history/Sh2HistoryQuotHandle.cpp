/*
 * Sh2HistoryQuotHandle.cpp
 *
 *  Created on: 2016年11月2日
 *      Author: user
 */

#include "Sh2HistoryQuotHandle.h"
#include <sys/stat.h>
#include <dirent.h>

Sh2HistoryQuotHandle::Sh2HistoryQuotHandle(const char* path, uint32_t date)
        : m_path(path), m_date(date), m_codeNum(0), m_flag(false) {
        // TODO Auto-generated constructor stub
        InitEnv();
}

Sh2HistoryQuotHandle::~Sh2HistoryQuotHandle() {
        // TODO Auto-generated destructor stub
        m_snapStream.close();
        m_tickStream.close();
        m_aucStream.close();
        m_dayStream.close();
        m_minStream.close();

        if (snap0 != NULL) {
                delete snap0;
                snap0 = NULL;
        }
}

void Sh2HistoryQuotHandle::InitEnv() {
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

        filePath = datePath + "/Tick.csv";
        m_tickStream.open(filePath.c_str(), ios::out | ios::ate);
        if (!m_tickStream.is_open()) {
                ERROR("open %s failed!", filePath.c_str());
                return;
        }

        filePath = datePath + "/Auction.csv";
        m_aucStream.open(filePath.c_str(), ios::out | ios::ate);
        if (!m_aucStream.is_open()) {
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
                snap0 = new STOCKSNAP2();
        }
        catch (const bad_alloc& e) {
                ERROR("create snap0 failed!");
                return;
        }
}

void Sh2HistoryQuotHandle::update(char* data, QuotType qt) {
        assert(data != NULL);

        STOCKSNAP2* pSnap = reinterpret_cast<STOCKSNAP2*>(data);
        string sCode(pSnap->securityID);
        if (sCode == "000000")
                return;
        if (m_mIsClose.find(sCode) == m_mIsClose.end())
                m_mIsClose[sCode] = false;

        if (qt == shanghaiLevel2Auction && pSnap->vap.price > 0) {
                boost::shared_ptr<HIS_AUCTION> sp = boost::make_shared<HIS_AUCTION>();
                sp->uiDateTime = (uint64_t) pSnap->date * 1000000 + pSnap->time / 100;
                sp->uiPrice = pSnap->vap.price;
                sp->uiVirtualAuctionQty = pSnap->vap.virtualAuctionQty;
                sp->uiLeaveQty = pSnap->vap.leaveQty;
                strcpy(sp->strSide, pSnap->vap.side);
                StoreAuction(sCode, sp);
        }
        else if (qt == shanghaiLevel2Tick) {
                boost::shared_ptr<HIS_TICK> sp = boost::make_shared<HIS_TICK>();
                sp->uiDateTime = (uint64_t) pSnap->date * 100000000 + pSnap->tradeTime;
                sp->uiPrice = pSnap->tradePrice;
                sp->uiVolume = pSnap->tradeQty;
                sp->uiAmount = pSnap->tradeMoney;
                sp->uiBuyNo = pSnap->tradeBuyNo;
                sp->uiSellNo = pSnap->tradeSellNo;
                StoreTick(sCode, sp);
        }
        else if (qt == shanghaiLevel2Snap && pSnap->time >= 9250000 
                        && !m_mIsClose[sCode] && pSnap->openPx > 0) {
                boost::shared_ptr<HIS_SNAP_L2> sp = boost::make_shared<HIS_SNAP_L2>();
                sp->uiDateTime = (uint64_t) pSnap->date * 1000000 + pSnap->time / 100;
                sp->uiPreClosePx = pSnap->prevClosePx;
                sp->uiOpenPx = pSnap->openPx;
                sp->uiHighPx = pSnap->highPx;
                sp->uiLowPx = pSnap->lowPx;
                sp->uiLastPx = pSnap->lastPx;
                sp->uiNumTrades = pSnap->numTrades;
                strcpy(sp->strInstrumentStatus, pSnap->instrumentStatus);
                sp->uiTotalVolumeTrade = pSnap->volume;
                sp->uiTotalValueTrade = pSnap->amount;
                sp->uiTotalBidQty = pSnap->totalBidQty;
                sp->uiTotalOfferQty = pSnap->totalOfferQty;
                sp->uiWeightedAvgBidPx = pSnap->weightedAvgBidPx;
                sp->uiWeightedAvgOfferPx = pSnap->weightAvgOfferPx;
                sp->uiWithdrawBuyNumber = pSnap->withdrawBuyNumber;
                sp->uiWithdrawBuyAmount = pSnap->withdrawBuyAmount;
                sp->uiWithdrawBuyMoney = pSnap->withdrawBuyMoney;
                sp->uiWithdrawSellNumber = pSnap->withdrawSellNumber;
                sp->uiWithdrawSellAmount = pSnap->withdrawSellAmount;
                sp->uiWithdrawSellMoney = pSnap->withdrawSellMoney;
                sp->uiTotalBidNumber = pSnap->totalBidNumber;
                sp->uiTotalOfferNumber = pSnap->totalOfferNumber;
                sp->uiBidTradeMaxDuration = pSnap->bidTradeMaxDuration;
                sp->uiOfferTradeMaxDuration = pSnap->offerTradeMaxDuration;
                sp->uiNumBidOrders = pSnap->numBidOrders;
                sp->uiNumOfferOrders = pSnap->numOfferOrders;
                for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                        sp->arrBidPrice[i] = pSnap->bid[i].price;
                        sp->arrBidOrderQty[i] = pSnap->bid[i].orderQty;
                        sp->arrBidNumOrders[i] = pSnap->bid[i].numOrders;
                        sp->arrOfferPrice[i] = pSnap->offer[i].price;
                        sp->arrOfferOrderQty[i] = pSnap->offer[i].orderQty;
                        sp->arrOfferNumOrders[i] = pSnap->offer[i].numOrders;
                }
                for (int i = 0; i < LEVEL2_ORDER_SIZE; ++i) {
                        sp->arrBidOrders[i] = pSnap->bid[0].order[i];
                        sp->arrOfferOrders[i] = pSnap->offer[0].order[i];
                }
                sp->uiIOPV = pSnap->iopv;
                sp->uiETFBuyNumber = pSnap->ETFBuyNumber;
                sp->uiETFBuyAmount = pSnap->ETFBuyAmount;
                sp->uiETFBuyMoney = pSnap->ETFBuyMoney;
                sp->uiETFSellNumber = pSnap->ETFSellNumber;
                sp->uiETFSellAmount = pSnap->ETFSellAmount;
                sp->uiETFSellMoney = pSnap->ETFSellMoney;
                if (m_mvSnap[sCode].empty() || memcpy((char*)m_mvSnap[sCode].back().get() + sizeof(uint64_t),
                                        (char*)sp.get() + sizeof(uint64_t),
                                        sizeof(HIS_SNAP_L2) - sizeof(uint64_t)) != 0) {
                        m_mvSnap[sCode].push_back(sp);
                        StoreSnap(sCode, sp);
                }

                if (strcmp(sp->strInstrumentStatus, "CLOSE") == 0) {
                        m_mIsClose[sCode] = true;
                        ++m_codeNum;
                }
        }

        if (!m_flag && m_mvSnap.size() == m_codeNum && !m_mvSnap.empty()) {
                for (SH2_SNAP_MAP_VECTOR::const_iterator cit = m_mvSnap.begin(); cit != m_mvSnap.end(); ++cit) {
                        DayCandle(cit->first);
                        StoreDay(cit->first);
                        MinuteCandle(cit->first);
                        StoreMinute(cit->first);
                }
                m_flag = true;
        }
}

void Sh2HistoryQuotHandle::StoreSnap(string code, boost::shared_ptr<HIS_SNAP_L2> &sp) {
        //write
        m_snapStream << code << "," << sp->uiDateTime << ","
                        << boost::format("%.3f") % ((float)sp->uiPreClosePx / 1000) << ","
                        << boost::format("%.3f") % ((float)sp->uiOpenPx / 1000) << ","
                        << boost::format("%.3f") % ((float)sp->uiHighPx / 1000) << ","
                        << boost::format("%.3f") % ((float)sp->uiLowPx / 1000) << ","
                        << boost::format("%.3f") % ((float)sp->uiLastPx / 1000) << ","
                        << sp->uiTotalVolumeTrade / 100 << ","
                        << boost::format("%.2f") % ((float)sp->uiTotalValueTrade / 100000) << ","
                        << sp->strInstrumentStatus << ",";

        for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                m_snapStream << boost::format("%.3f") % ((float)sp->arrBidPrice[i] / 1000) << ",";
        }
        for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                m_snapStream << sp->arrBidOrderQty[i] / 100 << ",";
        }
        for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                m_snapStream << sp->arrBidNumOrders[i] << ",";
        }
        for (int i = 0; i < LEVEL2_ORDER_SIZE; ++i) {
                m_snapStream << sp->arrBidOrders[i] / 100 << ",";
        }
        for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                m_snapStream << boost::format("%.3f") % ((float)sp->arrOfferPrice[i] / 1000) << ",";
        }
        for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                m_snapStream << sp->arrOfferOrderQty[i] / 100 << ",";
        }
        for (int i = 0; i < MAX_LEVEL2_SIZE; ++i) {
                m_snapStream << sp->arrOfferNumOrders[i] << ",";
        }
        for (int i = 0; i < LEVEL2_ORDER_SIZE; ++i) {
                m_snapStream << sp->arrOfferOrders[i] / 100 << ",";
        }

        m_snapStream << sp->uiNumTrades << ","
                        << boost::format("%.3f") % ((float)sp->uiIOPV / 1000) << ","
                        << sp->uiTotalBidQty / 100 << ","
                        << sp->uiTotalOfferQty / 100 << ","
                        << boost::format("%.3f") % ((float)sp->uiWeightedAvgBidPx / 1000) << ","
                        << boost::format("%.3f") % ((float)sp->uiWeightedAvgOfferPx / 1000) << ","
                        << sp->uiTotalBidNumber << ","        << sp->uiTotalOfferNumber << ","
                        << sp->uiBidTradeMaxDuration << ","        << sp->uiOfferTradeMaxDuration << ","
                        << sp->uiNumBidOrders << "," << sp->uiNumOfferOrders << ","
                        << sp->uiWithdrawBuyNumber << ","
                        << sp->uiWithdrawBuyAmount / 100 << ","
                        << boost::format("%.2f") % ((float)sp->uiWithdrawBuyMoney / 100000) << ","
                        << sp->uiWithdrawSellNumber << ","
                        << sp->uiWithdrawSellAmount / 100 << ","
                        << boost::format("%.2f") % ((float)sp->uiWithdrawSellMoney / 100000) << ","
                        << sp->uiETFBuyNumber << ","
                        << sp->uiETFBuyAmount / 100 << ","
                        << boost::format("%.2f") % ((float)sp->uiETFBuyMoney / 100000) << ","
                        << sp->uiETFSellNumber << ","
                        << sp->uiETFSellAmount / 100 << ","
                        << boost::format("%.2f") % ((float)sp->uiETFSellMoney / 100000) << endl;
}

void Sh2HistoryQuotHandle::StoreTick(string code, boost::shared_ptr<HIS_TICK> &sp) {
        //write
        m_tickStream << code << "," << sp->uiDateTime << ","
                        << boost::format("%.3f") % ((float)sp->uiPrice / 1000) << ","
                        << sp->uiVolume / 100 << ","
                        << boost::format("%.2f") % ((float)sp->uiAmount / 100000) << ","
                        << sp->uiBuyNo << "," << sp->uiSellNo << endl;
}

void Sh2HistoryQuotHandle::StoreAuction(string code, boost::shared_ptr<HIS_AUCTION> &sp) {
        //write
        m_aucStream << code << "," << sp->uiDateTime << ","
                        << boost::format("%.3f") % ((float)sp->uiPrice / 1000) << ","
                        << sp->uiVirtualAuctionQty / 100 << ","
                        << sp->uiLeaveQty / 100 << ","
                        << sp->strSide << endl;
}

void Sh2HistoryQuotHandle::DayCandle(string code) {
        if (m_mDay.find(code) == m_mDay.end()) {
                m_mDay[code] = boost::make_shared<HIS_CANDLESTICK>();
                m_mDay[code]->uiDateTime = m_date;
                m_mDay[code]->uiPreClosePx = m_mvSnap[code].back()->uiPreClosePx;
                m_mDay[code]->uiOpenPx = m_mvSnap[code].back()->uiOpenPx;
                m_mDay[code]->uiHighPx = m_mvSnap[code].back()->uiHighPx;
                m_mDay[code]->uiLowPx = m_mvSnap[code].back()->uiLowPx;
                m_mDay[code]->uiLastPx = m_mvSnap[code].back()->uiLastPx;
                m_mDay[code]->uiVolume = m_mvSnap[code].back()->uiTotalVolumeTrade;
                m_mDay[code]->uiAmount = m_mvSnap[code].back()->uiTotalValueTrade;
                m_mDay[code]->iMF = m_mvSnap[code].back()->uiIOPV;
        }
}

void Sh2HistoryQuotHandle::MinuteCandle(string code) {
    uint64_t preVolume = 0;
    uint64_t preAmount = 0;
    uint64_t preHighPx = 0;
    uint64_t preLowPx = 0;

    for (vector<boost::shared_ptr<HIS_SNAP_L2> >::const_iterator const_it =
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

                    sp->uiVolume = (*const_it)->uiTotalVolumeTrade - preVolume;
                    preVolume = (*const_it)->uiTotalVolumeTrade;
                    sp->uiAmount = (*const_it)->uiTotalValueTrade - preAmount;
                    preAmount = (*const_it)->uiTotalValueTrade;
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
                            (*const_it)->uiTotalVolumeTrade - preVolume;
            preVolume = (*const_it)->uiTotalVolumeTrade;
            m_mvMinute[code].back()->uiAmount +=
                            (*const_it)->uiTotalValueTrade - preAmount;
            preAmount = (*const_it)->uiTotalValueTrade;
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

void Sh2HistoryQuotHandle::StoreDay(string code) {
        //write
        m_dayStream << code << "," << m_mDay[code]->uiDateTime << ","
                        << boost::format("%.3f") % ((float)m_mDay[code]->uiPreClosePx / 1000) << ","
                        << boost::format("%.3f") % ((float)m_mDay[code]->uiOpenPx / 1000) << ","
                        << boost::format("%.3f") % ((float)m_mDay[code]->uiHighPx / 1000) << ","
                        << boost::format("%.3f") % ((float)m_mDay[code]->uiLowPx / 1000) << ","
                        << boost::format("%.3f") % ((float)m_mDay[code]->uiLastPx / 1000) << ","
                        << m_mDay[code]->uiVolume / 100 << ","
                        << boost::format("%.2f") % ((float)m_mDay[code]->uiAmount / 100000) << ","
                        << boost::format("%.3f") % ((float) m_mDay[code]->iMF / 1000) << endl;
}

void Sh2HistoryQuotHandle::StoreMinute(string code) {
        //write
        for (vector<boost::shared_ptr<HIS_CANDLESTICK> >::const_iterator cit =
                        m_mvMinute[code].begin(); cit != m_mvMinute[code].end(); ++cit) {
                m_minStream << code << "," << (*cit)->uiDateTime << ","
                                << boost::format("%.3f") % ((float)(*cit)->uiPreClosePx / 1000) << ","
                                << boost::format("%.3f") % ((float)(*cit)->uiOpenPx / 1000) << ","
                                << boost::format("%.3f") % ((float)(*cit)->uiHighPx / 1000) << ","
                                << boost::format("%.3f") % ((float)(*cit)->uiLowPx / 1000) << ","
                                << boost::format("%.3f") % ((float)(*cit)->uiLastPx / 1000) << ","
                                << (*cit)->uiVolume / 100 << ","
                                << boost::format("%.2f") % ((float)(*cit)->uiAmount / 100000) << ","
                                << boost::format("%.3f") % ((float) (*cit)->iMF / 1000) << endl;
        }
}
