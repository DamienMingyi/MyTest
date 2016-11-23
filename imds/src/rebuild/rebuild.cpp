/**
 * @file rebuild.cpp
 * @brief generate the specified reference file from rebuilding data
 *
 * @note
 * 锟斤拷rebuild锟斤拷息锟斤拷锟斤拷同锟斤拷锟缴讹拷应锟侥硷拷
 *
 * @author jiachen@sse.com.cn
 * @author ygao@sse.com.cn
 * @version 1.0
 * @date 2014-1-7
 * @bug     锟睫讹拷锟斤拷锟斤拷    锟睫讹拷锟斤拷     锟睫革拷锟斤拷bug锟斤拷锟斤拷锟睫讹拷原锟斤拷
 * @warning 锟斤拷
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "../public/program/Profile.h"
#include "../public/program/Logger.h"
#include <time.h>

using namespace std;

#define GLENGTH 1024 * 64

/**
 * @author ygao@sse.com.cn
 *
 * @brief generate the request for rebuilding data
 *
 * @param buf store the request for rebuilding data
 * @param method rebuilding method
 * @param category certain data category
 * @param begin the begin position whose default value is 0
 * @param end the end position whose default value is 0
 * @param channel trade channel whose default value is 0
 *
 * @return length of the request information
 *
 */

int getRebuild(char* buf, int method, int category, int begin, int end, int channel)
{
	char sp = 0x01;
	char body[1024];
	int blen = sprintf(body,
			"35=UA1201%c49=RBS%c56=VDE%c34=0%c52=20130926-14:10:00%c10075=%d%c10142=%d%c10073=%d%c10074=%d%c10077=%d%c",
			sp, sp, sp, sp, sp, method, sp, category, sp, begin, sp, end, sp, channel, sp);
	blen = sprintf(buf, "8=STEP.1.0.0%c9=%d%c%s", sp, blen, sp, body);
	int chk = 0;
	for(int i = 0; i < blen; i ++)
	{
		chk += buf[i];
	}
	blen += sprintf(buf + blen, "10=%03d%c", chk % 256, sp);
	return blen;
}

int fileLength(const char* rbPath)
{
	struct stat buf;
	int statValue = stat(rbPath, &buf);
	if (statValue != -1)
		return buf.st_size;
	else
		return -1;
}

void generateFile(char* buffer, int length, const char* refPath)
{
	FILE* fp = NULL;
	char path[256];
	int tag95;
	bool flag[3] = {false, false, false};
	char soh = 0x01;
	char ctag16001[8];
	char ctag16005[8];
	char ctag95[8];
	memset(ctag16001, 0, 8);
	memset(ctag16005, 0, 8);
	memset(ctag95, 0, 8);

	sprintf(ctag16001, "%c16001=", soh);
	sprintf(ctag16005, "%c16005=", soh);
	sprintf(ctag95, "%c95=", soh);
	for (int i = 0; i < length; ++ i)
	{
		int j;
		//.16001=
		if (strncmp(buffer + i, ctag16001, 7) == 0)
		{
			i += 7;
			j = i;
			while (buffer[i ++] != soh);
			char fileName[i - j];
			memset(fileName, 0, i - j);
			memcpy(fileName, buffer + j, i - j - 1);
			memset(path, 0, 256);
			sprintf(path, "%s/%s", refPath, fileName);
			flag[0] = true;
		}
		//.16005=
		if (strncmp(buffer + i, ctag16005, 7) == 0)
		{
			i += 7;
			j = i;
			while (buffer[i ++] != soh);
			char fragmentNo[i - j];
			memset(fragmentNo, 0, i - j);
			memcpy(fragmentNo, buffer + j, i - j - 1);
			int fragNo = atoi(fragmentNo);
			if (fragNo == 1)
				fp = fopen(path, "wb+");
			else
				fp = fopen(path, "ab+");
			if (fp == NULL)
			{
				ERROR("open %s failed!", path);
				exit(1);
			}
			else
				INFO("open %s successful!", path);
			flag[1] = true;

		}
		//.95=
		if (strncmp(buffer + i, ctag95, 4) == 0)
		{
			i += 4;
			j = i;
			while (buffer[i ++] != 0x01);
			char t95[i - j];
			memcpy(t95, buffer + j, i - j - 1);
			t95[i -j - 1] = '\0';
			tag95 = atoi(t95);
			//the following message is tag 96
			i += 3;
			if (fwrite(buffer + i, 1, tag95, fp) != tag95)
			{
				ERROR("write %s failed!", path);
				fclose(fp);
				exit(1) ;
			}
			INFO("writing %s finished!", path);
			fclose(fp);
			flag[2] = true;
		}
	}
	if (flag[0] == false)
	{
		ERROR("missing tag 16001");
		exit(1);
	}
	if (flag[1] == false)
	{
		ERROR("missing tag 16005");
		exit(1);
	}
	if (flag[2] == false)
	{
		ERROR("missing tag 95");
		exit(1);
	}
}

int main(int argc,char *argv[ ])
{
	Profile* pf=Profile::Instance("rebuild");
	char ip[64];
	int port;
	int method=1;
	int category=3;
	char ref_path[256];
	int	begin = 0;
	int end = 999;
	int channel = 0;
	//check the number of argument and convert them to specified type
	if(argc == 6)
	{
		strncpy(ip,argv[1],sizeof(ip));
		port=atoi(argv[2]);
		method = atoi(argv[3]);
		category = atoi(argv[4]);
		strncpy(ref_path,argv[5],sizeof(ref_path));
	}
	else if(argc == 9)
	{
		strncpy(ip,argv[1],sizeof(ip));
		port=atoi(argv[2]);
		method = atoi(argv[3]);
		category = atoi(argv[4]);
		strncpy(ref_path,argv[5],sizeof(ref_path));
		begin = atoi(argv[6]);
		end = atoi(argv[7]);
		channel = atoi(argv[8]);
	}
	else if (argc==3)
	{
		method = atoi(argv[1]);
		category = atoi(argv[2]);
		const char* v=pf->getEnv("vde_rebuild_ip");
		if (!v)
		{
			ERROR("vde_rebuild_ip not found");
			exit(1);
		}
		strncpy(ip,v,sizeof(ip));
		v=pf->getEnv("vde_rebuild_port");
		if (!v)
		{
			ERROR("vde_rebuild_port not found");
			exit(1);
		}
		port=atoi(v);
		v=pf->getEnv("rebuild_ref_path");
		if (!v)
		{
			ERROR("rebuild_ref_path not found");
			exit(1);
		}
		strncpy(ref_path,v,sizeof(ref_path));
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("rebuild ip port method category path begin end channel");
		exit(1);
	}
	INFO("rebuild %s %d %d %d %s",ip,port,method,category,ref_path);
	//check the rebuild data method
	if(method != 1 && method != 2 && method != 3)
	{
		ERROR("method:%d error!",method);
		exit(1);
	}

	char sbuf[1024];
	struct sockaddr_in serverAddr;
	int clientSocket;
	//check whether the socket is successful
	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ERROR("socket error!");
		exit(1);
	}
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);
	//check whether the connection is successful
	if(connect(clientSocket, (sockaddr*)&serverAddr, sizeof(sockaddr)) < 0)
	{
		ERROR("connect %s:%d error!",ip,port);
		exit(1);
	}
	//generate the request for rebuild data
	int len = getRebuild(sbuf, method, category, begin, end, channel);
	//send the request information
	len = send(clientSocket, sbuf, len, 0);

	char rbFile[256];
	sprintf(rbFile, "%s/%s", ref_path, "rb.dat");
	FILE *fp = fopen(rbFile, "wb+");
	if (fp == NULL)
	{
		ERROR("open file error!");
		exit(1);
	}

	while(true)
	{
		char rbuf[GLENGTH];
		int rlen = recv(clientSocket, rbuf, GLENGTH, 0);
		if(rlen > 0)
		{
			fwrite(rbuf, 1, rlen, fp);
		}
		else if (rlen == 0)
		{
			INFO("connection finished!");
			break;
		}
		else
		{
			ERROR("client socket error!");
			break;
		}
	}
	close(clientSocket);
	fclose(fp);
	int fLength = fileLength(rbFile);
	if (fLength == -1)
	{
		ERROR("get file length error!");
		exit(1);
	}
	if (fLength == 0)
	{
		WARN("file length is 0!");
		exit(1);
	}

	char *buffer = new char[fLength];
	if (buffer == NULL)
	{
		ERROR("new buffer failed!");
		exit(1);
	}

	FILE* fd = fopen(rbFile, "rb+");
	while (fLength != fread(buffer, 1, fLength, fd))
	{
		ERROR("read %s failed", rbFile);
		delete[] buffer;
		buffer = NULL;
		fclose(fd);
		exit(1);
	}
	fclose(fd);
	generateFile(buffer, fLength, ref_path);
	delete[] buffer;
	buffer = NULL;
	return 0;
}
