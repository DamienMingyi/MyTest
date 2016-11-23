/**
 * @file    IHttpReply.h
 * @brief   httpӦ��ӿ�
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-24
 */

#ifndef IHTTPREPLY_H_
#define IHTTPREPLY_H_

typedef struct
{
    static const int OK = 200;
    static const int BadRequest = 400;
    static const int NotFound = 404;
    static const int InternalServerError = 500;
    static const int ServiceUnavailable = 503;
} HttpStatus;

class IHttpReply
{
public:
    IHttpReply()
    {
    }
    ;
    virtual ~IHttpReply()
    {
    }
    ;

    virtual int request(const char* script, const char* query, char* reply,
            int& len)=0;
};
#endif /* IHTTPREPLY_H_ */
