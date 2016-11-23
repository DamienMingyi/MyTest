/*
 * Sh1QuotHandle.h
 *
 *  Created on: Oct 14, 2016
 *      Author: level2
 */

#ifndef SH1QUOTHANDLE_H_
#define SH1QUOTHANDLE_H_

#include "../../public/program/Logger.h"
#include "../../public/handle/IDataHandle.h"
#include "../../public/stock/StockSnap.h"
#include <map>
#include <cstring>

typedef std::map<std::string, STOCKSNAP*> StockSnapList;

class Sh1QuotHandle: public IDataHandle {
public:
	Sh1QuotHandle();
	virtual ~Sh1QuotHandle();

	virtual void handle(Message* msg);
protected:
	virtual void update(char* data, QuotType qt) = 0;

private:
	void handleSTEP(FastCodecs::DecoderMessage& dm);
	void handleEzEI(FastCodecs::DecoderMessage& dm);
	void handleFAST(FastCodecs::DecoderMessage& dm);
	bool checkSequence(int lastSequence, int sequence);
protected:
	STOCKSNAP* snap0;
private:
	static const char stepBeginString[11];
	static const char ezeiBeginString[9];
	static const char  beginInteger[7];
private:
	char securityType[8];
	StockSnapList stockSnpLst;
	char tradingSessionID[8];

	int sequence;
	int tag10072;
	int tag10142;

	int date;
	int time;
};

#endif /* SH1QUOTHANDLE_H_ */
