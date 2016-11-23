/**
 * @file    HttpReply.h
 * @brief   http应锟斤拷
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-24
 */

#ifndef HTTPREPLY_H_
#define HTTPREPLY_H_

#include "IHttpReply.h"
#include "../market/Market.h"
//#include "../market/Market2.h"
#include "../../util/hash/HashTable.h"
#include "../../util/jsoncpp/include/json/json.h"

#include <map>
#include <string>
#include <vector>
using namespace std;

/**
 * @class HttpReply 
 * @brief http应锟斤拷
 * @note
 * HttpReply锟角伙拷锟斤拷也锟角筹拷锟斤拷锟洁，实锟斤拷锟斤拷通锟矫的癸拷锟斤拷
 */
class HttpReply: public IHttpReply
{
public:
    HttpReply();
    virtual ~HttpReply();

    /** @brief 锟斤拷锟斤拷锟叫筹拷*/
    virtual void attach(Market* market);
    /** @brief 锟矫碉拷锟斤拷锟皆达拷锟斤拷锟�URL*/
    const char* getURL();
protected:
    /** @brief 锟斤拷锟斤拷路锟斤拷锟酵诧拷锟斤拷*/
    virtual bool parse(const char* script, const char* query);
    /** @brief 锟斤拷锟斤拷锟斤拷锟斤拷*/
    void parseQuery(const char* query);
    /** @brief 锟斤拷锟斤拷路锟斤拷*/
    void parseScript(const char* script);
    /** @brief 锟斤拷锟斤拷锟斤拷票锟斤拷锟斤拷*/
    virtual bool parseCode(int pos = -1);
    /** @brief 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷*/
    virtual bool parseList();
    /** @brief 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�*/
    virtual bool parseOrder();
    /** @brief 锟斤拷锟斤拷锟斤拷始锟酵斤拷锟斤拷锟斤拷锟斤拷*/
    virtual bool parseBeginEnd(int dftBegin, int dftEnd);
    /** @brief 锟斤拷锟斤拷选锟斤拷锟斤拷锟斤拷锟�*/
    virtual bool parseSelect(char* pres = NULL);
    /** @brief 锟斤拷锟斤拷锟斤拷锟节诧拷锟斤拷*/
    virtual bool parsePeriod(const char* dftPeriod);
    /** @brief 应锟斤拷*/
    virtual int reply(char* buffer, int& len);
    /** @brief 锟斤拷锟截达拷锟斤拷*/
    virtual int replyError(int status, char* buffer, int& len);

    virtual void registerCol();
    virtual int getString(char* buffer, const char* select, char *pData);
    /** @brief 锟街斤拷锟斤拷锟�*/
    void split(char sp, const char* source, vector<string>& vecSplit);
    /** @brief 锟斤拷锟斤拷小锟斤拷位锟斤拷锟截革拷锟斤拷锟斤拷锟街凤拷锟斤拷*/
    char* getFloat(long v, int sc = 0);

    char* getFloat(char* colDesc, char *pData);

    char* getHKFloat(long v, int sc = 0);

    char* getHKFloat(char* colDesc, char *pData);

    static const char calcRule[2][32];
    static char calcParams[6][8];

    map<string, string> paramQuery; ///<锟斤拷锟斤拷
    vector<string> paramScript; ///<路锟斤拷
    vector<string> paramSelect; ///<选锟斤拷

    HTable_ST *hTblColDes;
    Market* market; ///<锟叫筹拷
    Json::FastWriter fast_writer;
    Json::Value root;

    string type; ///<锟斤拷锟斤拷

    string script; ///<路锟斤拷
    string code; ///<锟斤拷锟斤拷
    string select; ///<选锟斤拷
    string order; ///<锟斤拷锟斤拷
    string listType; ///<锟斤拷锟斤拷锟斤拷锟斤拷
    string listSubType; ///<锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
    string period; ///<锟斤拷锟斤拷

    int total; ///<锟斤拷锟斤拷
    int begin; ///<锟斤拷始
    int end; ///<锟斤拷锟斤拷
    int status; ///<锟斤拷锟斤拷状态
    int date; ///<锟斤拷锟斤拷锟斤拷锟斤拷
    int time; ///<锟斤拷锟斤拷时锟斤拷

    char url[36]; ///<锟斤拷锟皆达拷锟斤拷锟�URL
    char ftValue[36]; ///<锟斤拷锟斤拷锟斤拷锟街凤拷锟斤拷
    int dot; ///<小锟斤拷位锟斤拷
};

#endif /* HTTPREPLY_H_ */
