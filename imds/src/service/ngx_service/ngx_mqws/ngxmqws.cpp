/*
 * ngxmqws.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: ygao@sse.com.cn
 */
#include "stdio.h"
#include "string.h"
#include <sys/types.h>
#include <unistd.h>

#include "ngxmqws.h"
#include "../../ngx_addon/ngx_http_mqws_module/ngx_service.h"

ngx_mqws* ngx_mqws::pInst = 0;

extern "C" char* request(const char* uri, const char* args, int* status, int* reply_len)
{
    return ngx_mqws::getInstant()->request(uri, args, status, reply_len);
}

ngx_mqws::ngx_mqws()
        : Service()
{
    init();
}

ngx_mqws::~ngx_mqws()
{

}

ngx_mqws* ngx_mqws::getInstant()
{
    if (!pInst)
        pInst = new ngx_mqws();
    return pInst;
}

bool ngx_mqws::init()
{
    Profile* pf = Profile::Instance("mqws");
    Market* shomarket = NULL;
    Market* sz1market = NULL;
    Market* sh1fstmarket = NULL;
    Market* sh2market = NULL;
    Market* shfimarket = NULL;
    Market* csipmarket = NULL;
    Market* hkpmarket = NULL;

    tmBegin = time(NULL);
    const char* v;

    v = pf->getEnv("sh1_shm_path");
    if (v)
        sh1fstmarket = createMarket("sh1", v, new StockSnap());
    if (sh1fstmarket)
    {
        addReply(sh1fstmarket, new StockSnapReply());
        addReply(sh1fstmarket, new StockListReply());
        addReply(sh1fstmarket, new StockLineReply());
        addReply(sh1fstmarket, new StockTrd2Reply());
        addReply(sh1fstmarket, new StockTrd1Reply());
        addReply(sh1fstmarket, new StockMonitorReply());
        addReply(sh1fstmarket, new StockStatusReply());
    }
    /*
     v = pf->getEnv("sh1fst_his_shm_path");
     if (v && sh1fstmarket)
     {
     CShmEnv* sh1fsthis = createShm(v);
     if (sh1fsthis)
     {
     addReply(sh1fstmarket, new DayKReply(sh1fsthis, "sh1"));
     addReply(sh1fstmarket, new MinKReply(sh1fsthis, "sh1"));
     }
     }*/

    v = pf->getEnv("sho_shm_path");
    if (v)
        shomarket = createMarket("sho", v, new OptionSnap());
    if (shomarket)
    {
        addReply(shomarket, new OptionSnapReply());
        addReply(shomarket, new OptionListReply());
        addReply(shomarket, new OptionLineReply());
        addReply(shomarket, new OptionTrd2Reply());
        addReply(shomarket, new OptionTrd1Reply());
        addReply(shomarket, new OptionMonitorReply());
        addReply(shomarket, new OptionStatusReply());
    }
    /*
     v = pf->getEnv("sh2_his_shm_path");
     if (v && sh2market)
     {
     CShmEnv* sh2his = createShm(v);
     if (sh2his)
     {
     addReply(sh2market, new DayKReply(sh2his, "sh2"));
     addReply(sh2market, new MinKReply(sh2his, "sh2"));
     }
     }

     v=pf->getEnv("sho_his_shm_path");
     if (v && shomarket)
     {
     CShmEnv* shohis=createShm(v);
     if (shohis)
     {
     addReply(shomarket,new DayKReply(shohis,"sho"));
     addReply(shomarket,new MinKReply(shohis,"sho"));
     }
     }
     */
    v = pf->getEnv("sz1_shm_path");
    if (v)
        sz1market = createMarket("sz1", v, new SZSnap());
    if (sz1market)
    {
        addReply(sz1market, new SzStockSnapReply());
        addReply(sz1market, new SzStockLineReply());
        addReply(sz1market, new SzStockListReply());
        addReply(sz1market, new SzStockMonitorReply());
        addReply(sz1market, new SzStockTrd1Reply());
        addReply(sz1market, new SzStockTrd2Reply());
        addReply(sz1market, new SzStockStatusReply());
    }
    /*
     v = pf->getEnv("sz1_his_shm_path");
     if(v && sz1market)
     {
     CShmEnv* sz1his=createShm(v);
     if (sz1his)
     {
     addReply(sz1market,new DayKReply(sz1his,"sz1"));
     addReply(sz1market,new MinKReply(sz1his,"sz1"));
     }
     }
     */

    v = pf->getEnv("sh2_shm_path");
    if (v)
        sh2market = createMarket("sh2", v, new StockSnap2());
    if (sh2market)
    {
        addReply(sh2market, new Stock2SnapReply());
        addReply(sh2market, new Stock2ListReply());
        addReply(sh2market, new Stock2LineReply());
        addReply(sh2market, new Stock2Trd0Reply());
        addReply(sh2market, new Stock2Trd2Reply());
        addReply(sh2market, new Stock2Trd1Reply());
    }
    /*
     v = pf->getEnv("sh2_his_shm_path");
     if (v && sh2market)
     {
     CShmEnv* sh2his = createShm(v);
     if (sh2his)
     {
     addReply(sh2market, new DayKReply(sh2his, "sh2"));
     addReply(sh2market, new MinKReply(sh2his, "sh2"));
     }
     }
     */
    /*
    v = pf->getEnv("shfi_shm_path");
    if (v)
        shfimarket = createMarket("shfi", v, new FixedIncomeSnap());
    if (shfimarket)
    {
        addReply(shfimarket, new FixIncomeSnapReply());
        addReply(shfimarket, new FixIncomeListReply());
        addReply(shfimarket, new FixIncomeLineReply());
        addReply(shfimarket, new FixIncomeTrd0Reply());
        addReply(shfimarket, new FixIncomeTrd1Reply());
        addReply(shfimarket, new FixIncomeTrd2Reply());
        addReply(shfimarket, new FixIncomeMonitorReply());
        addReply(shfimarket, new FixIncomeStatusReply());
    }
    */
    /*
     v = pf->getEnv("shfi_his_shm_path");
     if (v && shfimarket)
     {
     CShmEnv* shfihis = createShm(v);
     if (shfihis)
     {
     addReply(shfimarket, new DayKReply(shfihis, "shfi"));
     addReply(shfimarket, new MinKReply(shfihis, "shfi"));
     }
     }
     */
    v = pf->getEnv("csi_shm_path");
    if (v)
        csipmarket = createMarket("csi", v, new IndexPressSnap());
    if (csipmarket)
    {
        addReply(csipmarket, new IndexPressSnapReply());
        addReply(csipmarket, new IndexPressListReply());
        addReply(csipmarket, new IndexPressLineReply());
        addReply(csipmarket, new IndexPressMonitorReply());
        addReply(csipmarket, new IndexPressStatusReply());
    }
    /*
     v = pf->getEnv("csip_his_shm_path");
     if (v && csipmarket)
     {
     CShmEnv* csiphis = createShm(v);
     if (csiphis)
     {
     addReply(csipmarket, new DayKReply(csiphis, "csip"));
     addReply(csipmarket, new MinKReply(csiphis, "csip"));
     }
     }
     */

    v = pf->getEnv("shhk_shm_path");
    if (v)
        hkpmarket = createMarket("shhk", v, new HKPressSnap());
    if (hkpmarket)
    {
        addReply(hkpmarket, new HKPressSnapReply());
        addReply(hkpmarket, new HKPressListReply());
        addReply(hkpmarket, new HKPressLineReply());
        addReply(hkpmarket, new HKPressTrd1Reply());
        addReply(hkpmarket, new HKPressTrd2Reply());
        addReply(hkpmarket, new HKPressMonitorReply());
        addReply(hkpmarket, new HKPressStatusReply());
    }

    /*
     v = pf->getEnv("hkp_his_shm_path");
     if (v && hkpmarket)
     {
     CShmEnv* hkphis = createShm(v);
     if (hkphis)
     {
     addReply(hkpmarket, new DayKReply(hkphis, "hkp"));
     addReply(hkpmarket, new MinKReply(hkphis, "hkp"));
     }
     }*/
    return true;
}

CShmEnv* ngx_mqws::createShm(const char* name)
{
    CShmEnv* shmEnv = new CShmEnv;
    if (shmEnv->openShmEnv(name) != SUCCEED)
    {
        ERROR("openShmEnv(%s) error", name);
        return NULL;
    }
    return shmEnv;
}
void ngx_mqws::parse(string query, string& callbackValue,
        string::size_type& callbackPosition, string::size_type& andPosition)
{
    callbackPosition = query.find("callback=");
    if (callbackPosition == string::npos)
        return;
    andPosition = query.find('&');
    if (andPosition == string::npos)
        callbackValue = query.substr(9, query.length() - 1);
    else
        callbackValue = query.substr(9, andPosition - 9);
}

char* ngx_mqws::request(const char* uri, const char* args, int* status, int* reply_len)
{
    time_t tmNow = time(NULL);
    if (tmNow - tmBegin >= 3)
    {
        tmBegin = tmNow;
        refresh();
    }

    IHttpReply* httpReply = getReply(uri);
    if (httpReply == NULL)
    {
        *status = HttpStatus::ServiceUnavailable;
        return NULL;
        //*reply_len=sprintf(buffer,"%s %d %d :%s %s",Profile::Instance()->getHome(),getppid(),replys.size(),uri,args);
        //*status=200;
        //return buffer;
    }
    string callbackValue;
    string::size_type callbackPosition;
    string::size_type andPosition;
    if (args[0] != 0)
    {
        string strQuery(args);
        parse(strQuery, callbackValue, callbackPosition, andPosition);
    }
    //json
    if (args[0] == 0 || callbackPosition == string::npos)
    {
        *status = httpReply->request(uri, args, buffer, buflen);
        if (*status == HttpStatus::OK)
        {
            *reply_len = buflen;
            return buffer;
        }
    }
    //jsonp
    else
    {
        *status =
                (andPosition == string::npos) ?
                        httpReply->request(uri, NULL, buffer, buflen) :
                        httpReply->request(uri, args + 10 + callbackValue.length(),
                                buffer, buflen);
        //encapsulate
        size_t cblength = callbackValue.length();
        memset(bf, 0, 8 * 1024 * 1024);
        memcpy(bf, buffer, buflen);
        memcpy(buffer, callbackValue.c_str(), cblength);
        memcpy(buffer + cblength + 1, bf, buflen);
        buffer[cblength] = '(';
        buflen += cblength + 2;
        buffer[buflen - 1] = ')';
        if (*status == HttpStatus::OK)
        {
            *reply_len = buflen;
            return buffer;
        }
    }
    return NULL;
}

/*
 int main(int argc,char* argv[])
 {
 int status=0,len=0;
 char* buf=request("/v1/sz1/list/exchange/all","select=code,price_limit_setting_c",&status,&len);
 if (buf == NULL)
 printf("%d\n", status);
 //printf("%s\n", buf);
 return 0;
 }
 */
