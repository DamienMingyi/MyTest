/**
 * @file    Service.h
 * @brief   �������
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-2-28
 */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <string>
#include <map>
#include <vector>
#include "HttpReply.h"

using namespace std;
/**
 * @class Service 
 * @brief �������
 * @note
 * ֧�ֶ��г��������������
 */
class Service{
public:
	Service();
	virtual ~Service();
	/** @brief Ӧ������*/
	virtual int request(const char* script,const char* query);
	/** @brief ����г���Ӧ��*/
	void addReply(Market* market,HttpReply* reply);
	/** @brief �����г�����*/
	void refresh();
	/** @brief ���Ӧ������*/
	void show();
protected:
	/** @brief ����url����Ӧ�����*/
	IHttpReply* getReply(const char* script);

	map<string,HttpReply*> replys;///<Ӧ��
	vector<Market*> markets;///<�г�

	char* buffer;//���ص��ַ���
	char* bf;
	int buflen;//���صĳ���
};

#endif /* SERVICE_H_ */
