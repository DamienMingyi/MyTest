/*
 * ngxmqws.h
 *
 *  Created on: Oct 28, 2015
 *      Author: ygao@sse.com.cn
 */

#ifndef NGXMQWS_H_
#define NGXMQWS_H_

#include "../../../util/shm/CShmEnv.h"
#include "../../../public/market/Market.h"
#include "../../../public/program/Profile.h"
#include "../../../public/program/Logger.h"

#include "../../../public/reply/Service.h"
#include "../../../public/reply/DayKReply.h"
#include "../../../public/reply/MinKReply.h"


#include "../../../public/stock/StockSnap.h"
#include "../../stock/StockTrd2Reply.h"
#include "../../stock/StockTrd1Reply.h"
#include "../../stock/StockSnapReply.h"
#include "../../stock/StockLineReply.h"
#include "../../stock/StockListReply.h"
#include "../../stock/StockMonitorReply.h"
#include "../../stock/StockStatusReply.h"

#include "../../../public/option/OptionSnap.h"
#include "../../option/OptionLineReply.h"
#include "../../option/OptionListReply.h"
#include "../../option/OptionTrd2Reply.h"
#include "../../option/OptionTrd1Reply.h"
#include "../../option/OptionSnapReply.h"
#include "../../option/OptionMonitorReply.h"
#include "../../option/OptionStatusReply.h"

#include "../../../public/stock2/StockSnap2.h"
#include "../../stock2/Stock2Trd2Reply.h"
#include "../../stock2/Stock2Trd1Reply.h"
#include "../../stock2/Stock2SnapReply.h"
#include "../../stock2/Stock2LineReply.h"
#include "../../stock2/Stock2ListReply.h"
#include "../../stock2/Stock2Trd0Reply.h"

/*
#include "../../../public/fixincome/FixIncomeSnap.h"
#include "../../fixincome/FixIncomeTrd2Reply.h"
#include "../../fixincome/FixIncomeTrd1Reply.h"
#include "../../fixincome/FixIncomeSnapReply.h"
#include "../../fixincome/FixIncomeLineReply.h"
#include "../../fixincome/FixIncomeListReply.h"
#include "../../fixincome/FixIncomeTrd0Reply.h"
#include "../../fixincome/FixIncomeMonitorReply.h"
#include "../../fixincome/FixIncomeStatusReply.h"
*/

#include "../../../public/indexpress/IndexPressSnap.h"
#include "../../indexpress/IndexPressSnapReply.h"
#include "../../indexpress/IndexPressLineReply.h"
#include "../../indexpress/IndexPressListReply.h"
#include "../../indexpress/IndexPressMonitorReply.h"
#include "../../indexpress/IndexPressStatusReply.h"


#include "../../../public/hkpress/HKPressSnap.h"
#include "../../hkpress/HKPressSnapReply.h"
#include "../../hkpress/HKPressLineReply.h"
#include "../../hkpress/HKPressListReply.h"
#include "../../hkpress/HKPressTrd2Reply.h"
#include "../../hkpress/HKPressTrd1Reply.h"
#include "../../hkpress/HKPressMonitorReply.h"
#include "../../hkpress/HKPressStatusReply.h"

#include "../../../public/szstock/SZSnap.h"
#include "../../szstock/SzStockLineReply.h"
#include "../../szstock/SzStockSnapReply.h"
#include "../../szstock/SzStockListReply.h"
#include "../../szstock/SzStockMonitorReply.h"
#include "../../szstock/SzStockSnapReply.h"
#include "../../szstock/SzStockStatusReply.h"
#include "../../szstock/SzStockTrd1Reply.h"
#include "../../szstock/SzStockTrd2Reply.h"

#include <time.h>
#include <string>
using namespace std;

class ngx_mqws : public Service {
public:
	ngx_mqws();
	virtual ~ngx_mqws();

	static ngx_mqws* getInstant();
	char* request(const char* uri,const char* args,int* status,int* reply_len);
private:
	bool init();
	void parse(string query, string& callbackValue, string::size_type& callbackPosition, string::size_type& andPosition);
	CShmEnv* createShm(const char* name);
	time_t tmBegin;

	static ngx_mqws* pInst;
};

#endif /* NGXMQWS_H_ */
