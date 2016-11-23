/*
 * HKPReference.h
 *
 *  Created on: Sep 15, 2015
 *      Author: level2
 */

#ifndef HKPREFERENCE_H_
#define HKPREFERENCE_H_

#include "../../public/market/Market.h"
#include "../../public/hkpress/HKPressSnap.h"
#include "../../public/initial/IInitial.h"
#include <string>
#include <vector>

#define REFF04_FIELD 19

class ShhkReference: public IInitial {
public:
	ShhkReference();
	virtual ~ShhkReference();
	bool initial(const char* referencePath, const char* shm);
	int getStockNum();
private:
	bool reff04StaticParse(char* ref_path);
	bool mktdt04StaticParse(char* ref_path);
	bool split(const std::string& record);
	std::string trim(const std::string str);
private:
	int32_t totalNum;
	std::vector<HKPSNAP> vHKP;
	map<string,MKTDT04> mMktdt04;
};

#endif /* HKPREFERENCE_H_ */
