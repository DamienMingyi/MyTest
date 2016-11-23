/*
 * Sh1fstReference.h
 *
 *  Created on: Jun 3, 2015
 *      Author: level2
 */

#ifndef SH1FSTREFERENCE_H_
#define SH1FSTREFERENCE_H_

#include "../../public/market/Market.h"
#include "../../public/stock/StockSnap.h"
#include "../../public/initial/IInitial.h"
#include "../../public/static/Staticparse.h"
#include <string>

using namespace std;

class Sh1Reference: public IInitial{
public:
	Sh1Reference();
	virtual ~Sh1Reference();
	bool initial(const char* referencePath, const char* shm);
private:
	bool fjyStaticParse(char *path);
	bool mktdt00StaticParse(char *path);
	bool plateFileParse(char *path,const char flag);
	void getmGnpz(char * conceptcode,char* securities);
private:
	vector<STOCKSNAP> vStock;
	map<string, CPXX> mCpxx;
	map<string, GZLX> mGzlx;
	map<string, DBP> mDbp;
	map<string,BASIC_INFO> mBasicInfo;
	map<string,CAPITAL_DATA> mCapitalData;
	map<string,FINANCIAL_DIGEST> mFinancialDigest;
	map<string,FINANCIAL_INDEX> mFinancialIndex;
	map<string,WEIGHT_DATA> mWeightData;
	map<string, string> mgnpz;
};

#endif /* SH1FSTREFERENCE_H_ */
