/**
 * @file    HttpReply.h
 * @brief   http应锟斤拷
 * @note
 * HttpReply锟角伙拷锟斤拷也锟角筹拷锟斤拷锟洁，实锟斤拷锟剿斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷墓锟斤拷埽锟斤拷锟矫伙拷锟绞碉拷志锟斤拷锟揭碉拷锟�
 * 目前只支锟街凤拷锟斤拷JSON锟街凤拷
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-25
 */

#include "HttpReply.h"

const char HttpReply::calcRule[2][32] = {"getFloat", "getHKFloat"};
char HttpReply::calcParams[6][8] = {"0","1","2","3","4","5"};

HttpReply::HttpReply()
{
    market = NULL;
    hTblColDes = NULL;
    total = begin = end = 0;
    status = HttpStatus::OK;
    date = time = 0;
    ftValue[0] = 0;
    dot = 3;
}

HttpReply::~HttpReply()
{
}

const char* HttpReply::getURL()
{
    if (this->market)
        sprintf(this->url, "v1/%s/%s", this->market->getName(), type.c_str());
    else
        this->url[0] = 0;
    return this->url;
}

char* HttpReply::getHKFloat(long v, int scale)
{
    if (scale == 0)
        scale = dot;
    if (scale == 3)
    {
        if (v % 10)
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%03ld", v / 1000, v % 1000);
            else
                sprintf(ftValue, "-%ld.%03ld", -v / 1000, -v % 1000);
        }
        else
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%03ld", v / 1000, v % 1000);
            else
                sprintf(ftValue, "-%ld.%03ld", -v / 1000, -v % 1000);
        }
    }
    else
        sprintf(ftValue, "%ld", v);
    return ftValue;
}

char* HttpReply::getHKFloat(char* colDesc, char *pData)
{
    COL_DESC_ST *pCol = (COL_DESC_ST *) colDesc;

    if (NULL == pCol || NULL == pData)
    {
        return ftValue;
    }

    int scale = 0;
    if (pCol->params)
    {
        scale = atoi(pCol->params);
    }
    switch (pCol->dataType)
    {
        case DATATYPE_CHAR:
        {
            char c = (char) *(pData + pCol->offset);
            return getHKFloat((long) c, scale);
        }
        case DATATYPE_INT32:
        {
            int iNum = (int) *(pData + pCol->offset);
            return getHKFloat((long) iNum, scale);
        }
        case DATATYPE_INT64:
        {
            int64_t iNum64 = (int64_t) *(pData + pCol->offset);
            return getHKFloat((long) iNum64, scale);
        }
        default:
            break;
    }
    return ftValue;
}


char* HttpReply::getFloat(long v, int scale)
{
    if (scale == 0)
        scale = dot;
    if (scale == 3)
    {
        if (v % 10)
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%03ld", v / 1000, v % 1000);
            else
                sprintf(ftValue, "-%ld.%03ld", -v / 1000, -v % 1000);
        }
        else
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%02ld", v / 1000, v % 1000 / 10);
            else
                sprintf(ftValue, "-%ld.%02ld", -v / 1000, -v % 1000 / 10);
        }
    }
    else if (scale == 4)
    {
        if (v % 10)
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%04ld", v / 10000, v % 10000);
            else
                sprintf(ftValue, "-%ld.%04ld", -v / 10000, -v % 10000);
        }
        else
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%03ld", v / 10000, v % 10000 / 10);
            else
                sprintf(ftValue, "-%ld.%03ld", -v / 10000, -v % 10000 / 10);
        }
    }
    else if (scale == 5)
    {
        if (v % 10)
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%05ld", v / 100000, v % 100000);
            else
                sprintf(ftValue, "-%ld.%05ld", -v / 100000, -v % 100000);
        }
        else
        {
            if (v >= 0)
                sprintf(ftValue, "%ld.%04ld", v / 100000, v % 100000 / 10);
            else
                sprintf(ftValue, "-%ld.%04ld", -v / 100000, -v % 100000 / 10);
        }
    }
    else if (scale == 2)
    {
        if (v >= 0)
            sprintf(ftValue, "%ld.%02ld", v / 100, v % 100);
        else
            sprintf(ftValue, "-%ld.%02ld", -v / 100, -v % 100);
    }
    else if (scale == 1)
    {
        if (v >= 0)
            sprintf(ftValue, "%ld.%ld", v / 10, v % 10);
        else
            sprintf(ftValue, "-%ld.%ld", -v / 10, -v % 10);
    }
    else
        sprintf(ftValue, "%ld", v);
    return ftValue;
}

char* HttpReply::getFloat(char* colDesc, char *pData)
{
    COL_DESC_ST *pCol = (COL_DESC_ST *) colDesc;

    if (NULL == pCol || NULL == pData)
    {
        return ftValue;
    }
    int scale = 0;
    if (pCol->params)
    {
        scale = atoi(pCol->params);
    }
    switch (pCol->dataType)
    {
        case DATATYPE_CHAR:
        {
            char c = (char) *(pData + pCol->offset);
            return getFloat((long)c, scale);
        }
        case DATATYPE_INT32:
        {
            int iNum = (int) *(pData + pCol->offset);
            return getFloat((long)iNum, scale);
        }
        case DATATYPE_INT64:
        {
            int64_t iNum64 = (int64_t) *(pData + pCol->offset);
            return getFloat((long)iNum64, scale);
        }
        default:
            break;
    }
    return ftValue;
}

void HttpReply::attach(Market* market)
{
    this->market = market;
}

bool HttpReply::parse(const char* script, const char* query)
{
    parseScript(script);
    parseQuery(query);
    return true;
}

void HttpReply::parseQuery(const char* query)
{
    paramQuery.clear();
    if (query != NULL)
    {
        bool norv = true;
        unsigned int vp = 0;
        int qlen = strlen(query);
        char qn[128], qv[1024];
        for (int i = 0; i < qlen; ++i)
        {
            if (norv)
            {
                if (vp >= sizeof(qn) - 2)
                    return;
            }
            else
            {
                if (vp >= sizeof(qv) - 2)
                    return;
            }
            if (query[i] == '&')
            {
                norv = !norv;
                qv[vp] = 0;
                vp = 0;
                paramQuery[qn] = qv;
            }
            else if (query[i] == '=')
            {
                norv = !norv;
                qn[vp] = 0;
                vp = 0;
            }
            else
            {
                if (norv)
                {
                    qn[vp++] = query[i];
                }
                else
                {
                    qv[vp++] = query[i];
                }
            }
        }
        if (qn[0] != 0 && qv[0] != 0)
        {
            qv[vp] = 0;
            paramQuery[qn] = qv;
        }
    }
}
void HttpReply::split(char sp, const char* source, vector<string>& vecSplit)
{
    vecSplit.clear();
    if (source != NULL)
    {
        int qlen = strlen(source);
        char sc[1024];
        int j = 0;
        for (int i = 0; i < qlen; ++i)
        {
            if (j >= sizeof(sc) - 2)
                return;
            if (source[i] == sp)
            {
                if (j > 0)
                {
                    sc[j] = 0;
                    vecSplit.push_back(sc);
                    j = 0;
                }
            }
            else
                sc[j++] = source[i];
        }
        if (j != 0)
        {
            sc[j] = 0;
            vecSplit.push_back(sc);
        }
    }
}

void HttpReply::parseScript(const char* script)
{
    split('/', script, paramScript);
}

bool HttpReply::parseCode(int pos)
{
    if (paramScript.size() + pos <= 0)
    {
        status = HttpStatus::BadRequest;
        return false;
    }
    code = paramScript[paramScript.size() + pos];
    return true;
}

bool HttpReply::parseOrder()
{
    map<string, string>::iterator it;
    it = this->paramQuery.find("order");
    if (it != paramQuery.end())
        this->order = it->second;
    else
        this->order.clear();
    return true;
}
bool HttpReply::parsePeriod(const char* dftPeriod)
{
    map<string, string>::iterator it;
    it = this->paramQuery.find("period");
    if (it != paramQuery.end())
        this->period = it->second;
    else
        this->period = dftPeriod;
    return true;
}
bool HttpReply::parseBeginEnd(int dftBegin, int dftEnd)
{
    map<string, string>::iterator it;
    it = this->paramQuery.find("begin");
    if (it != paramQuery.end())
        begin = atoi(it->second.c_str());
    else
        begin = dftBegin;
    it = this->paramQuery.find("end");
    if (it != paramQuery.end())
        end = atoi(it->second.c_str());
    else
        end = dftEnd;

    if (total == 0)
    {
        begin = 0;
        end = 0;
    }
    else
    {
        if (begin < 0)
        {
            begin = total + begin + 1;
            if (begin < 0)
                begin = 0;
        }
        if (end < 0)
            end = total + end + 1;
        if (begin > total)
            begin = total;
        if (end > total)
            end = total;
        if (end < begin)
            return false;
    }
    return true;
}

bool HttpReply::parseSelect(char* pres)
{
    map<string, string>::iterator it;
    it = this->paramQuery.find("select");
    if (it != paramQuery.end())
        select = it->second;
    else
    {
        if (pres != NULL)
            select = pres;
        else
            select.clear();
    }
    if (select.empty())
        paramSelect.clear();
    else
        split(',', select.c_str(), paramSelect);
    return true;
}

bool HttpReply::parseList()
{
    if (paramScript.size() < 2)
        return false;
    listSubType = paramScript[paramScript.size() - 1];
    listType = paramScript[paramScript.size() - 2];
    return true;
}

int HttpReply::reply(char* buffer, int& len)
{
    return replyError(status, buffer, len);
}

void HttpReply::registerCol()
{
    return;
}

int HttpReply::getString(char* buffer, const char* select, char *pData)
{
    COL_DESC_ST *description = NULL;
    if (this->hTblColDes == NULL)
    {
        return 0;
    }

    if (!HTableFind(this->hTblColDes, select, (char **) &description))
    {
        return 0;
    }

    if (description->formatFunc == default_format_func)
    {
        if (description->calcRule == NULL)
        {
            return (*description->formatFunc)(buffer, 64,
                    (char *) description, pData, NULL);
        }
        else if (STREQ(calcRule[0], description->calcRule))
        {
            return (*description->formatFunc)(buffer, 64,
                    NULL, getFloat((char *) description, pData), NULL);
        }
        else if (STREQ(calcRule[1], description->calcRule))
        {
            return (*description->formatFunc)(buffer, 64,
                    NULL, getHKFloat((char *) description, pData), NULL);
        }
    }

    return 0;
}

int HttpReply::replyError(int http_status, char* buffer, int& len)
{
    switch (http_status)
    {
        case HttpStatus::BadRequest:
            len = sprintf(buffer, "BadRequest");
            break;
        case HttpStatus::NotFound:
            len = sprintf(buffer, "NotFound");
            break;
        case HttpStatus::InternalServerError:
            len = sprintf(buffer, "InternalServerError");
            break;
        case HttpStatus::ServiceUnavailable:
            len = sprintf(buffer, "ServiceUnavailable");
            break;
        default:
            len = sprintf(buffer, "UnknownError");
            break;
    }
    return http_status;
}
