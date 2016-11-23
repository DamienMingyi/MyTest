/**
 * @file    Profile.h
 * @brief   ���������ļ��ͻ�������
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-13
 */

#ifndef PROFILE_H_
#define PROFILE_H_

/**
 * @class Profile 
 * @brief ���������ļ��ͻ�������
 */
class Profile {
public:
	/** @brief �õ�������ʵ��*/
	static Profile* Instance();
	static Profile* Instance(const char* program_name);
	/** @brief �õ�����Ŀ¼����·��*/
	const char* getHome();
	/** @brief ���ݲ������Ƶõ�����*/
	const char* getEnv(const char* name);
	/** @brief д����ȡPID�ļ�*/
	const char* setPid(bool isWrite,bool isOnly=false);
private:
	Profile(const char* program_name);
	virtual ~Profile();

	bool getValue(const char* path,const char* name);
	void getLogger();
private:
	static Profile* prof;
	char program_name[256];
	char home_path[256];
	char envvalue[256];
};

#endif /* PROFILE_H_ */
