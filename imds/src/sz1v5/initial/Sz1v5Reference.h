/*
 * Sz1v5Reference.h
 *
 *  Created on: May 31, 2016
 *      Author: level2
 */

#ifndef SZ1V5REFERENCE_H_
#define SZ1V5REFERENCE_H_

#include <stdint.h>
#include <map>
#include <string>
#include "../../util/xml/tinyxml2.h"
#include "../../public/szstock/SZSnap.h"
#include <boost/algorithm/string.hpp>
#include "../../public/initial/IInitial.h"
#include "../../public/static/Staticparse.h"
#include <fstream>

typedef std::map<std::string, SZSNAP> MSZSNAP;

class Sz1v5Reference:public IInitial {
public:
	Sz1v5Reference();
	virtual ~Sz1v5Reference();

	bool initial(const char* referencePath, const char* shm);

	bool readXML(char* securities, char* indexInfo, char* stat, char* cashAuctionParams);
	bool parseStaticFile(char* zqjcxx, char* gbsj, char* cwzy, char* cwzb, char* qxsj);
private:
	bool parse(char* file);
	void nodeParse(tinyxml2::XMLElement *node, uint32_t depth, uint32_t width);
	std::string trim(const std::string str);
	bool plateFileParse(char *path,const char flag);
	void getmGnpz(char * conceptcode,char* securities);
private:
	MSZSNAP mszsnap;
	std::string code;
	uint32_t setting;
	map<string,BASIC_INFO> mBasicInfo;
	map<string,CAPITAL_DATA> mCapitalData;
	map<string,FINANCIAL_DIGEST> mFinancialDigest;
	map<string,FINANCIAL_INDEX> mFinancialIndex;
	map<string,WEIGHT_DATA> mWeightData;
	map<string, string> mgnpz;
};

#endif /* SZ1V5REFERENCE_H_ */
