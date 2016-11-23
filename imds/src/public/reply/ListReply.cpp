/**
 * @file    ListReply.cpp
 * @brief   Ӧ���б�����
 * @note
 * /v1/market/list/type/sub_type
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-28
 */

#include "ListReply.h"
#include <algorithm>

bool ListCompare(const ListCmp& l1, const ListCmp& l2)
{
    bool cmp = true;
    if (l1.type == 0 && l2.type == 0)
    {
        if (strcmp(l1.charCmp, l2.charCmp) < 0)
            cmp = false;
    }
    else if (l1.type == 1 && l2.type == 1)
    {
        cmp = (l1.iCmp > l2.iCmp);
    }
    else if (l1.type == 2 && l1.type == 2)
    {
        cmp = (l1.lCmp > l2.lCmp);
    }
    else if (l1.type == 3 && l1.type == 3)
    {
        cmp = (l1.dCmp > l2.dCmp);
    }
    return cmp;
}

bool listCompare(const ListCmp& l1, const ListCmp& l2)
{
    bool cmp = true;
    if (l1.type == 0 && l2.type == 0)
    {
        if (strcmp(l1.charCmp, l2.charCmp) > 0)
            cmp = false;
    }
    else if (l1.type == 1 && l2.type == 1)
    {
        cmp = (l1.iCmp < l2.iCmp);
    }
    else if (l1.type == 2 && l1.type == 2)
    {
        cmp = (l1.lCmp < l2.lCmp);
    }
    else if (l1.type == 3 && l1.type == 3)
    {
        cmp = (l1.dCmp < l2.dCmp);
    }
    return cmp;
}

ListReply::ListReply()
{
    curSnap = NULL;
    type = "list";
}

ListReply::~ListReply()
{
}

bool ListReply::parse(const char* script, const char* query)
{
    if (market == NULL)
        return false;

    HttpReply::parse(script, query);

    HttpReply::parseOrder();

    if (!HttpReply::parseList())
        return false;
    if (listType.compare("self") == 0)
    {
        split('_', listSubType.c_str(), self);
        if (self.empty())
            return false;
        if (!getSelfList())
            return false;
    }
    else
    {
        if (!getTypeList())
            return false;
    }
    if (!order.empty())
    {
        if (list[0].order)
            sort(list.begin(), list.end(), ListCompare);
        else
            sort(list.begin(), list.end(), listCompare);
    }
    total = list.size();
    return parseBeginEnd(0, -1);
}

int ListReply::reply(char* buffer, int& len)
{
    len = sprintf(buffer,
            "{\"date\":%d,\"time\":%d,\"total\":%d,\"begin\":%d,\"end\":%d,\"list\":[",
            date, time, total, begin, end);
    if (total > 0)
    {
        for (unsigned int j = begin; j < list.size() && j < end; ++j)
        {
            curSnap = list[j].snap;
            if (j > begin)
                len += sprintf(buffer + len, ",[");
            else
                len += sprintf(buffer + len, "[");
            for (unsigned int i = 0; i < paramSelect.size(); ++i)
            {
                if (i > 0)
                    len += sprintf(buffer + len, ",");
                int slen = getSnapString(buffer + len, paramSelect[i].c_str());
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

bool ListReply::setOrder(char* colDesc, char *pData, ListCmp& cmp)
{
    COL_DESC_ST *pCol = (COL_DESC_ST *) colDesc;
    if (NULL == pCol || NULL == pData)
    {
        return false;
    }

    switch (pCol->dataType)
    {
        case DATATYPE_CHAR:
        {
            char c = (char) (*(pData + pCol->offset));
            cmp.type = 1;
            cmp.iCmp = (int) c;
        }
            break;
        case DATATYPE_CHARP:
        {
            char **ptrCh = (char **) (pData + pCol->offset);
            cmp.type = 0;
            strncpy(cmp.charCmp, *ptrCh, sizeof(cmp.charCmp));
        }
            break;
        case DATATYPE_STR:
        {
            char *ptrStr = (char *) (pData + pCol->offset);
            cmp.type = 0;
            strncpy(cmp.charCmp, ptrStr, sizeof(cmp.charCmp));
        }
            break;
        case DATATYPE_INT32:
            cmp.type = 1;
            cmp.iCmp = (int) *(pData + pCol->offset);
            break;
        case DATATYPE_INT64:
            cmp.type = 2;
            cmp.lCmp = (int64_t) *(pData + pCol->offset);
            break;
        case DATATYPE_DOUBLE:
            cmp.type = 3;
            cmp.dCmp = (double) *(pData + pCol->offset);
            break;
        default:
            return false;
    }
    return true;
}
