/*
 * ShoQuotHandle.h
 *
 *  Created on: Sep 14, 2016
 *      Author: level2
 */

#ifndef SHOQUOTHANDLE_H_
#define SHOQUOTHANDLE_H_

#include "../../public/program/Logger.h"
#include "../../public/handle/IDataHandle.h"
#include "../../public/option/OptionSnap.h"
#include <map>
#include <cstring>
typedef std::map<std::string, int64_t> StockPx;
typedef std::map<std::string, SHOSNAP*> OptionSnapList;

class ShoQuotHandle: public IDataHandle {
public:
	ShoQuotHandle();
	virtual ~ShoQuotHandle();

	virtual void handle(Message* msg);
protected:
	virtual void update(char* data, QuotType qt) = 0;

private:
	void handleSTEP(FastCodecs::DecoderMessage& dm);
	void handleEzEI(FastCodecs::DecoderMessage& dm);
	void handleFAST(FastCodecs::DecoderMessage& dm);
	void handleOptionFAST(FastCodecs::DecoderMessage& dm);
	void handleStockFAST(FastCodecs::DecoderMessage& dm);
	bool checkSequence(int lastSequence, int sequence);
protected:
	SHOSNAP* snap0;
private:
	static const char stepBeginString[11];
	static const char ezeiBeginString[9];
	static const char  beginInteger[7];
private:
	char securityType[8];
	StockPx stockpx;
	OptionSnapList optionSnpLst;
	char tradingSessionID[8];
	int sequence;
	int tag10072;
	int tag10142;
	int date;
	int time;
};

#endif /* SHOQUOTHANDLE_H_ */
