/*
 * Sh2Reference.h
 *
 *  Created on: Apr 8, 2014
 *      Author: level2
 */

#ifndef SH2REFERENCE_H_
#define SH2REFERENCE_H_

#include "../../public/market/Market.h"
#include "../../public/stock2/StockSnap2.h"
#include "../../public/initial/IInitial.h"
#include "../../public/static/Staticparse.h"

using namespace std;
class Sh2Reference:public IInitial {
public:
	Sh2Reference();
	virtual ~Sh2Reference();
	bool initial(const char* referencePath, const char* shm);
private:
	bool fjyStaticParse(char *path);
	bool mktdt00StaticParse(char *path);
	bool plateFileParse(char *path,const char flag);
	void getmGnpz(char * conceptcode,char* securities);
private:
	vector<STOCKSNAP2> vStock;
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

#endif /* SH2REFERENCE_H_ */
