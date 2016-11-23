/**
 * @file    Logger.cpp
 * @brief   ��־
 * @note
 *
 * ����ģʽ��ȫ��Ψһ����־ʵ��
 *
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-13
 * @bug     ��
 * @warning ��
 */


#include "Logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Profile.h"
#include "CMutex.h"

char Logger::filename[256];
int Logger::level;
char Logger::localIP[256];
char Logger::remoteIP[256];
int Logger::port=0;
CMutex g_logMutex;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Instance()
{
	Logger::filename[0]=0;
	Logger::level=Logger::Log_Debug;
}

void Logger::Instance(const char* name)
{
	sprintf(Logger::filename,"%s/log/%s.log",Profile::Instance()->getHome(),name);
	Logger::level=Logger::Log_Info;
}

void Logger::Instance(const char* path, const char* name, int level)
{
    memset(Logger::filename, 0, sizeof(Logger::filename));
    int i = 0;
    if (path != 0 && strlen(path) != 0)
        i = sprintf(Logger::filename, "%s/", path);
    if (name != 0 && strlen(name) != 0)
        sprintf(Logger::filename + i, "%s.log", name);
    if (level < Log_Debug || level > Log_Error)
        level = Log_Debug;
    else
    	Logger::level = level;
}

void Logger::Instance(const char* path_name, int level)
{
    memset(Logger::filename, 0, sizeof(Logger::filename));
    if (path_name != 0 && strlen(path_name) != 0)
        strncpy(Logger::filename, path_name, 256);
    if (level < Log_Debug || level > Log_Error)
    	Logger::level = Log_Debug;
    else
    	Logger::level = level;
}
void Logger::Log(const char* srcname, int lineno, int level, const char* fmt, ...)
{
#ifndef Debug
    if (level < Logger::getLevel())
        return;
#endif
    va_list ap;
    char msg[2000];
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);
    CScopedMutex scoped_lock(g_logMutex);
    return Logger::log(srcname, lineno, level, msg);
}

void Logger::log(const char* srcname, int lineno, int level, char* msg)
{
    char buf[2048];
    char stime[36];
    time_t logtime;
    time(&logtime);
    strftime(stime, 36, "%Y-%m-%d %H:%M:%S", localtime(&logtime));
    sprintf(buf, "%s [%s#%d] %s -- %s\r\n", stime, srcname, lineno,
            LogLevelName[level], msg);

#ifdef Debug
    printf("%s",buf);
#else
    if (Logger::filename[0] == 0)
    {
        printf("%s", buf);
        return;
    }
#endif


    int flog = 0;
    if ((flog = open(Logger::filename, O_WRONLY | O_APPEND | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0)
    {
        return;
    }
    write(flog, buf, strlen(buf));
    int length = lseek(flog, 0, SEEK_END);
    close(flog);

    if (Logger::port>0 && level>Logger::Log_Info)
    	remoteLog(level,msg);

    if (length > 512 * 1024)
    {
        char old[256];
        char oldder[256];
        for (int i = 3; i >= 0; i--)
        {
            sprintf(old, "%s.%d", Logger::filename, i);
            if (access(old, F_OK) == 0)
            {
                sprintf(oldder, "%s.%d", Logger::filename, i + 1);
                rename(old, oldder);
            }
        }
        rename(Logger::filename, old);
    }
}
void Logger::setRemote(const char* localIP,const char* remoteIP,int port)
{
	strncpy(Logger::localIP,localIP,255);
	strncpy(Logger::remoteIP,remoteIP,255);
	Logger::port=port;
}

void Logger::remoteLog(int level,char* msg)
{
	int udp_socket=0;
	struct sockaddr_in localAddr,serverAddr;
	memset(&localAddr,0,sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(Logger::port);
	localAddr.sin_addr.s_addr = inet_addr(Logger::localIP);

	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(Logger::port);
	serverAddr.sin_addr.s_addr = inet_addr(Logger::remoteIP);
	udp_socket=socket(AF_INET,SOCK_DGRAM,0);

	if (udp_socket>0)
	{
	    int nREUSEADDR = 1;
	    if(setsockopt( udp_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nREUSEADDR, sizeof(int)) == 0)
	    {
            if (bind(udp_socket,(struct sockaddr*)&localAddr,sizeof(localAddr))==0)
            {
                char buf[2048];
                int buflen=sprintf(buf,"[{type:\"1\",name:\"%s\",value:\"%s--%s\",produceTime:\"\"}]",\
                		Logger::filename,LogLevelName[level],msg);
                buflen=sendto(udp_socket,buf,buflen,0,(sockaddr*)&serverAddr,sizeof(serverAddr));
            }
            close(udp_socket);
	    }
	}
}


