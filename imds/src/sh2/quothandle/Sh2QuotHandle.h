/*
 * Sh2QuotHandle.h
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#ifndef SH2QUOTHANDLE_H_
#define SH2QUOTHANDLE_H_

#include "../../public/program/Logger.h"
#include "../../public/handle/IDataHandle.h"
#include "../../public/stock2/StockSnap2.h"
#include "Sh2BidOfferOrderQueue.h"
#include <map>
#include <cstring>

typedef std::map<std::string, STOCKSNAP2*> StockSnap2List;

class Sh2QuotHandle: public IDataHandle {
public:
	Sh2QuotHandle();
	virtual ~Sh2QuotHandle();

	virtual void handle(Message* msg);

protected:
	virtual void update(char* data, QuotType qt) = 0;

private:
	void handleFAST(FastCodecs::DecoderMessage& dm);
	void handleSTEP(FastCodecs::DecoderMessage& dm);
	void handleTick(FastCodecs::DecoderMessage& dm, int tagCount);
	void handleMarketData(FastCodecs::DecoderMessage& dm, int tagCount);
	void handleIndex(FastCodecs::DecoderMessage& dm, int tagCount);
	void handleVirtualAuctionPrice(FastCodecs::DecoderMessage& dm, int tagCount);
	void handleMarketOverview(FastCodecs::DecoderMessage& dm, int tagCount);
private:
	static const char stepBeginString[11];
	static const char  beginInteger[7];

protected:
	STOCKSNAP2* snap0;
private:
	StockSnap2List stockSnap2List;
	Sh2BidOfferOrderQueue booq;

	static const int SET_NUM = 7;

	int sequence;
	int tag10072;
	int tag10142;
	char tag35[8];
	int date;
	int time;
	int channelIndex[SET_NUM];
};

#endif /* SH2QUOTHANDLE_H_ */
