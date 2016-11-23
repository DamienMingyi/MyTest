/**
 * @file    LineReply.h
 * @brief   Ӧ����������
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-27
 */

#ifndef LINEREPLY_H_
#define LINEREPLY_H_

#include "HttpReply.h"
/**
 * @class LineReply 
 * @brief Ӧ����������
 * @note
 * 
 */
class LineReply: public HttpReply
{
public:
    LineReply();
    virtual ~LineReply();

protected:
    virtual bool parse(const char* script, const char* query);

    virtual int getLineString(char* buffer, const char* select)=0;

    virtual int reply(char* buffer, int& len);

    vector<char*> line; ///<����

    char* curLine; ///<��ǰ���Ƶ�ַ

    int prev_close; ///<���ռ۸�
};

#endif /* LINEREPLY_H_ */
