/*
 * SettlePrice.h
 *
 *  Created on: Jun 23, 2015
 *      Author: level2
 */

#ifndef SETTLEPRICE_H_
#define SETTLEPRICE_H_

#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "../../public/option/OptionSnap.h"
#include "../../util/shm/CShmEnv.h"
#include <fstream>
#include "../../public/program/Logger.h"
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "../../util/atof/Parse.h"
#include "../../public/market/Market.h"

class SettlePrice {
public:
	SettlePrice();
	virtual ~SettlePrice();

	bool close(char* close_ref_path, char* shm_path);
private:
	void getSettlePrice(char* path);
private:
	int date;						///<write reference file date
	int closeTime;					///<write reference file time
	std::map<std::string, int> settleMap;
	Parse ps;
};

#endif /* SETTLEPRICE_H_ */
