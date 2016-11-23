/**
 * @file    Profile.cpp
 * @brief   ���������ļ��ͻ�������
 * @note
 *
 * ����ģʽ��ȫ��Ψһ��ʵ�������Ի������Ŀ¼����������Ŀ¼�µ�confĿ¼�л�����ò���
 * ͬʱ����ȫ��Ψһ����־ʵ��
 *
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-13
 * @bug     ��
 * @warning ��
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include "Profile.h"
#include "Logger.h"

Profile* Profile::prof=NULL;

Profile::Profile(const char* program_name)
{
	strncpy(this->program_name,program_name,sizeof(this->program_name)-1);
	this->envvalue[0]=0;
	getHome();
	getLogger();
}

Profile::~Profile()
{
}

Profile* Profile::Instance()
{
	return Profile::prof;
}

Profile* Profile::Instance(const char* program_name)
{
	if (!Profile::prof)
		Profile::prof=new Profile(program_name);
	return Profile::prof;
}


 /**
 * @brief �õ�����Ŀ¼����·��
 * @return const char* ����·��
 * @note ͨ�����̵���Ϣ���õ��������ڵľ���·��
 */
const char* Profile::getHome()
{
	pid_t pid=getpid();
	char self[256];
	sprintf(self,"/proc/%d/exe",pid);
	int len=sizeof(home_path)-1;
	//��������ֹ������Ҫ��ӡ�\0��,�������ݻᱻ�ض�
	len=readlink(self,home_path,len);
	int i=0;
	int pos=0;
	while (i<len)
	{
		if (home_path[i]=='/')
			pos=i;
		if (home_path[i]==' ')
			break;
		++i;
	}
	if (pos>0)
	{
		home_path[pos]=0;
		return home_path;
	}
	return NULL;
}
 /**
 * @brief ������־
 * @return void
 */
void Profile::getLogger()
{
	const char* log_path=getEnv("log_path");
	if (log_path!=NULL)
	{
		char path[256];
		strncpy(path,log_path,sizeof(path));
		const char* log_level=getEnv("log_level");
		if (log_level!=NULL)
			Logger::Instance(path,program_name,atoi(log_level));
		else
			Logger::Instance(path,program_name,Logger::Log_Info);
	}
	else
	{
		Logger::Instance();
	}
}
/** 
 *  @brief ���ݲ������Ƶõ�����
 *  @param [in] name ��������
 *  @return const char* NULL��ʾδ�ҵ�
 *  @note
 *  ���ڳ���������ļ����ң�����Ҳ������ٵ�ϵͳȫ�ֵ������ļ�����
 */
const char* Profile::getEnv(const char* name)
{
	char path[256];
	sprintf(path,"%s/conf/%s.conf",home_path,program_name);
	if (getValue(path,name))
		return envvalue;
	sprintf(path,"%s/conf/system.conf",home_path);
	if (getValue(path,name))
		return envvalue;
	return NULL;
}
/** 
 *  @brief д����ȡPID�ļ�
 *  @param [in] isWrite true д��PID�ļ� false ����pid
 *  @param [in] isOnly true Ҫ��Ψһ���� 
 *  @return const char* NULL��ʾд��ʧ�ܻ��ʧ�ܣ����򷵻�pidֵ
 *  @note
 *  ����ͨ���ļ�����֤Ψһ����
 *  ����pid���ɾ��pid�ļ�
 */
const char* Profile::setPid(bool isWrite,bool isOnly)
{
	char path[256];
	if (isWrite)
	{
		pid_t pid=getpid();
		sprintf(envvalue,"pid=%d",pid);
		sprintf(path,"%s/conf/%s.pid",home_path,program_name);
		int fid=open(path,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
		if (fid<0)
			return NULL;
		if (isOnly && flock(fid,LOCK_EX|LOCK_NB)!=0)
			return NULL;
		ftruncate(fid,0);
		write(fid,envvalue,strlen(envvalue));
		if (!isOnly)
			close(fid);
		return envvalue;
	}
	else
	{
		sprintf(path,"%s/conf/%s.pid",home_path,program_name);
		if (getValue(path,"pid"))
		{
			unlink(path);
			return envvalue;
		}
	}
	return NULL;
}

bool Profile::getValue(const char* path,const char* name)
{
	FILE* fp;
	fp=fopen(path,"r");
	int slen=strlen(name);
	char value[256];
	if (fp!=NULL)
	{
		while (fgets(value,256,fp))
		{
			if (strlen(value)>slen && memcmp(value,name,slen)==0)
			{
				slen=sprintf(envvalue,"%s",value+slen+1);
				if (slen>1 && envvalue[slen-1]=='\n')
					envvalue[--slen]=0;
				if (slen>1 && envvalue[slen-1]=='\r')
					envvalue[--slen]=0;
				fclose(fp);
				return true;
			}
		}
		fclose(fp);
	}
	return false;
}
