/*
 * Sz1v5QuotHandle.h
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */

#ifndef SZ1V5QUOTHANDLE_H_
#define SZ1V5QUOTHANDLE_H_

#include "../../public/program/Logger.h"
#include "../../public/handle/IDataHandle.h"
#include "../../public/szstock/SZSnap.h"
#include <map>
#include <cstring>

typedef std::map<std::string, SZSNAP*> SzSnapList;

class Sz1v5QuotHandle: public IDataHandle {
public:
	Sz1v5QuotHandle();
	virtual ~Sz1v5QuotHandle();


	virtual void handle(Message* msg);
protected:
	virtual void update(char* data, QuotType qt) = 0;
protected:
	SZSNAP* snap0;
private:
	void handleSTEP(FastCodecs::DecoderMessage& dm);
	void handleSZ1V5(char* data);
	void handleLogon(char* buf, uint32_t bodyLength);
	void handleLogout(char* buf, uint32_t bodyLength);


	void handleStatistic(char* buf, uint32_t bodyLength);
	void handleStatus(char* buf, uint32_t bodyLength);
	void handleAuction(char* buf, uint32_t bodyLength);

	void handleIndex(char* buf, uint32_t bodyLength);
	void handleTradeVolume(char* buf, uint32_t bodyLength);
	uint32_t handleSnap(char* buf, SZSNAP** szsnap);
private:
	static const char  beginInteger[7];

private:
	SzSnapList szSnpLst;
	char securityType[8];
	int sequence;
	int tag10072;
	int tag10142;

};

static inline uint64_t swap64(uint64_t in);
uint64_t ntohll(uint64_t value);
uint64_t htonll(uint64_t value);

#endif /* SZ1V5QUOTHANDLE_H_ */
