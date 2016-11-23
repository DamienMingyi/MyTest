/*
 * CSIQuotHandle.h
 *
 *  Created on: Nov 15, 2016
 *      Author: level2
 */

#ifndef CSIQUOTHANDLE_H_
#define CSIQUOTHANDLE_H_

#include "../../public/program/Logger.h"
#include "../../public/handle/IDataHandle.h"
#include "../../public/indexpress/IndexPressSnap.h"
#include "../../util/atof/Parse.h"
#include <map>
#include <cstring>

typedef std::map<std::string, INDEXSNAP*> IndexSnapList;

class CSIQuotHandle: public IDataHandle {
public:
	CSIQuotHandle();
	virtual ~CSIQuotHandle();

	virtual void handle(Message* msg);
protected:
	virtual void update(char* data, QuotType qt) = 0;

private:
	void handleSTEP(FastCodecs::DecoderMessage& dm);
	void handleEzEI(FastCodecs::DecoderMessage& dm);
	void handleFAST(FastCodecs::DecoderMessage& dm);
	bool checkSequence(int lastSequence, int sequence);
protected:
	INDEXSNAP* snap0;
private:
	static const char stepBeginString[11];
	static const char ezeiBeginString[9];
	static const char beginInteger[7];
	static const char indexInfo[3];
private:
	char securityType[8];
	IndexSnapList indexSnpLst;

	int sequence;
	int tag10072;
	int tag10142;

	int date;
	int time;
	Parse parse;
};

#endif /* CSIQUOTHANDLE_H_ */
