/**
 * @file    Profile.cpp
 * @brief   处理配置文件和环境变量
 * @note
 *
 * 单例模式，全局唯一的实例，可以获得运行目录，并在运行目录下的conf目录中获得配置参数
 * 同时设置全局唯一的日志实例
 *
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-13
 * @bug     无
 * @warning 无
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
 * @brief 得到运行目录绝对路径
 * @return const char* 绝对路径
 * @note 通过进程的信息，得到程序所在的绝对路径
 */
const char* Profile::getHome()
{
	pid_t pid=getpid();
	char self[256];
	sprintf(self,"/proc/%d/exe",pid);
	int len=sizeof(home_path)-1;
	//不包含终止符，需要添加‘\0’,过长内容会被截断
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
 * @brief 设置日志
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
 *  @brief 根据参数名称得到配置
 *  @param [in] name 参数名称
 *  @return const char* NULL表示未找到
 *  @note
 *  先在程序的配置文件中找，如果找不到，再到系统全局的配置文件中找
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
 *  @brief 写入或读取PID文件
 *  @param [in] isWrite true 写入PID文件 false 读出pid
 *  @param [in] isOnly true 要求唯一运行 
 *  @return const char* NULL表示写入失败或读失败，否则返回pid值
 *  @note
 *  可以通过文件锁保证唯一运行
 *  读出pid后会删除pid文件
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
