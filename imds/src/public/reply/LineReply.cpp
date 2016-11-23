/**
 * @file    LineReply.cpp
 * @brief   Ӧ���б�����
 * @note
 * /v1/market/line/code?begin=0&end=-1&select=*
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-27
 */
#include "LineReply.h"

LineReply::LineReply()
{
    prev_close = 0;
    curLine = NULL;
    type = "line";
}

LineReply::~LineReply()
{
}

bool LineReply::parse(const char* script, const char* query)
{
    if (market == NULL)
        return false;

    HttpReply::parse(script, query);

    if (!HttpReply::parseCode())
        return false;

    return true;
}

int LineReply::reply(char* buffer, int& len)
{
    len = sprintf(buffer,
            "{\"code\":\"%s\",\"prev_close\":%s,\"date\":%d,\"time\":%d,\"total\":%d,\"begin\":%d,\"end\":%d,\"line\":[",
            code.c_str(), getFloat(prev_close), date, time, total, begin, end);
    if (total > 0)
    {
        for (unsigned int j = 0; j < line.size(); ++j)
        {
            curLine = line[j];
            if (j > 0)
                len += sprintf(buffer + len, ",[");
            else
                len += sprintf(buffer + len, "[");
            for (unsigned int i = 0; i < paramSelect.size(); ++i)
            {
                if (i > 0)
                    len += sprintf(buffer + len, ",");
                int slen = getLineString(buffer + len, paramSelect[i].c_str());
                if (slen == 0)
                    return HttpReply::replyError(HttpStatus::BadRequest, buffer, len);
                len += slen;
            }
            len += sprintf(buffer + len, "]");
        }
    }
    len += sprintf(buffer + len, "]}");
    return HttpStatus::OK;
}
