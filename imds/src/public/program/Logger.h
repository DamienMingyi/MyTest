/**
 * @file    Logger.h
 * @brief   »’÷æ
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-13
 */

#ifndef LOGGER_H_
#define LOGGER_H_

static const char* LogLevelName[]={"Debug","Info","Warn","Error"};

#define     DEBUG(...)      Logger::Log(__FILE__, __LINE__, Logger::Log_Debug, __VA_ARGS__)
#define     INFO(...)       Logger::Log(__FILE__, __LINE__, Logger::Log_Info, __VA_ARGS__)
#define		WARN(...)		Logger::Log(__FILE__, __LINE__, Logger::Log_Warn, __VA_ARGS__)
#define		ERROR(...)		Logger::Log(__FILE__, __LINE__, Logger::Log_Error,__VA_ARGS__)

class Logger {
public:
	static void Instance();
	static void Instance(const char* name);
	static void Instance(const char* path_name,int level);
	static void Instance(const char* path,const char* name,int level);
	static void setRemote(const char* localIP,const char* remoteIP,int port);
	static void Log(const char* srcname,int lineno,int level,const char* fmt,...);
public:
	static const int Log_Debug=0;
	static const int Log_Info=1;
	static const int Log_Warn=2;
	static const int Log_Error=3;
private:
	static 	char filename[256];
	static 	int level;
	static  char localIP[256];
	static  char remoteIP[256];
	static  int  port;
	static void log(const char* srcname,int lineno,int level,char* msg);
	static void remoteLog(int level,char* msg);
	static int getLevel() { return level;}
private:
	Logger();
	virtual ~Logger();
};

#endif /* LOGGER_H_ */
