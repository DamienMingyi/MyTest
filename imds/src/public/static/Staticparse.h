/*
 * Staticparse.h
 *
 *  Created on: 2016年11月10日
 *      Author: mds
 */

#ifndef SRC_PUBLIC_STATIC_STATICPARSE_H_
#define SRC_PUBLIC_STATIC_STATICPARSE_H_
#include <map>
#include <string>
#include <fstream>
#include <time.h>
#include <sys/stat.h>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "../../public/program/Logger.h"
#include "../../util/atof/Parse.h"
#include "../stock/StockSnap.h"
using namespace std;

class Staticparse {
public:
	Staticparse();
	virtual ~Staticparse();
	bool cpxxStaticParse(const char* refpath, map<string, CPXX> &mCpxx);
	bool gzlxStaticParse(const char* refpath, map<string, GZLX> &mGzlx);
	bool dbpStaticParse(const char* refpath, map<string, DBP> &mDbp);
	bool zqjcxxStaticParse(const char* refpath, map<string, BASIC_INFO> &mBasicInfo);
	bool gbsjStaticParse(char *refpath, map<string, CAPITAL_DATA> &mCapitalData);
	bool cwzyStaticParse(char *refpath, map<string, FINANCIAL_DIGEST> &mFinancialDigest);
	bool cwzbStaticParse(char *refpath, map<string, FINANCIAL_INDEX> &mFinancialIndex);
	bool qxsjStaticParse(char *refpath, map<string, WEIGHT_DATA> &mWeightData);
private:
	Parse ps;
	int32_t date;
	int32_t getFileBufferLength(const char* fileName);
};

#endif /* SRC_PUBLIC_STATIC_STATICPARSE_H_ */
