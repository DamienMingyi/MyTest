/**
 * @file StockSnap.h
 *
 * @brief ��Ʊ������սṹ��
 *
 * @note
 * ���¹�Ʊ���գ���ʣ�������
 *
 * @author ygao@sse.com.cn
 * @author jiachen@sse.com.cn
 *
 * @version 1.0
 * @date 2014-2-21
 * @bug
 * @warning
 */
#ifndef STOCKSNAP_H_
#define STOCKSNAP_H_

#include "../market/Market.h"
#include "../market/ISnapShot.h"
#include "Sh1Snap.h"

class StockSnap: public ISnapShot {
public:
	StockSnap();
	virtual ~StockSnap();

	virtual void attach(char* snap)
	{
		this->snap = reinterpret_cast<STOCKSNAP*>(snap);
	}

	virtual bool getValue(int id, char* v);
	virtual bool getValue(int id, int& v);
	virtual void update();
	virtual void lineUpdate();
	virtual void tradeUpdate();
	virtual char* getLine();
	virtual char* getTrade();

	virtual void getDayCandlestick(CANDLESTICK* candlestick);
	virtual void getMinCandlestick(char* min, CANDLESTICK* candlestick);
	virtual char* append(int position, char* item, unsigned long sz);
private:
	STOCKSNAP* snap;
	STOCKLINE ln;
};

#endif /* STOCKSNAP_H_ */
