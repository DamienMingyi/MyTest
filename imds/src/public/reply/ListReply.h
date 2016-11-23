/**
 * @file    ListReply.h
 * @brief   Ӧ���б�����
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-28
 */

#ifndef LISTREPLY_H_
#define LISTREPLY_H_

#include "HttpReply.h"

/** @brief �Ƚϴ�С�Ľṹ��*/
typedef struct
{
    char* snap; ///<������յ�ַ
    bool order; ///<true ����false ����
    int type; ///<�Ƚϵ����� 0 �ַ��� 1 int 2 long 3 double
    char charCmp[80]; ///<�ַ����Ƚ�
    int iCmp; ///<int�Ƚ�
    long lCmp; ///<long�Ƚ�
    double dCmp; ///double�Ƚ�
} ListCmp;
/**
 * @class ListReply 
 * @brief Ӧ���б�����
 * @note
 * 
 */
class ListReply: public HttpReply
{
public:
    ListReply();
    virtual ~ListReply();
protected:
    virtual bool parse(const char* script, const char* query);
    virtual int reply(char* buffer, int& len);
    /** @brief �õ���ѡ���б�*/
    virtual bool getSelfList()=0;
    /** @brief �õ��б�����*/
    virtual bool getTypeList()=0;
    /** @brief �õ��Ƚ���*/
    virtual bool getOrder(ListCmp& cmp)=0;

    virtual bool setOrder(char* colDesc, char *pData, ListCmp& cmp);

    /** @brief �õ�������*/
    virtual int getSnapString(char* buffer, const char* select)=0;
    vector<string> self; ///<��ѡ�ɱ�

    vector<ListCmp> list; ///<�Ƚϵ��б�

    char* curSnap; ///<��ǰ����
};

#endif /* LISTREPLY_H_ */
