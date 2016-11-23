/*
 * Sh2Process.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#include "Sh2Process.h"

Sh2Process::Sh2Process(Profile* pf, IDataHandle* dh): Process(pf, dh) {
	// TODO Auto-generated constructor stub

}

Sh2Process::~Sh2Process() {
	// TODO Auto-generated destructor stub
	if (dh != NULL)
		delete dh;
}

int32_t Sh2Process::main(int argc, char* argv[])
{
	char templatePath[256];
	memset(templatePath, 0, sizeof(templatePath));
	char localIP[256];
	memset(localIP, 0, sizeof(localIP));
	char ip[256];
	memset(ip, 0, sizeof(ip));
	int port = 0;
	char dataPath[256];
	memset(dataPath, 0, sizeof(dataPath));
	char shmPath[256];
	memset(shmPath, 0, sizeof(shmPath));
	char msgType[256];
	memset(msgType, 0, sizeof(msgType));
	char upstream[256];
	memset(upstream, 0, sizeof(upstream));

	const char* v;
	IStream* stream = NULL;

	if (argc == 8)
	{
		strncpy(templatePath, argv[1], sizeof(templatePath));
		strncpy(ip, argv[2], sizeof(ip));
		port = atoi(argv[3]);
		strncpy(localIP, argv[4], sizeof(localIP));
		strncpy(shmPath, argv[5], sizeof(shmPath));
		strncpy(msgType, argv[6], sizeof(msgType));
		strncpy(upstream,  argv[7], sizeof(msgType));
	}
	else if (argc == 7)
	{
		strncpy(templatePath, argv[1], sizeof(templatePath));
		strncpy(ip, argv[2], sizeof(ip));
		port = atoi(argv[3]);
		strncpy(shmPath, argv[4], sizeof(shmPath));
		strncpy(msgType, argv[5], sizeof(msgType));
		strncpy(upstream,  argv[6], sizeof(msgType));
	}
	else if (argc == 6)
	{
		strncpy(templatePath, argv[1], sizeof(templatePath));
		strncpy(dataPath, argv[2], sizeof(dataPath));
		strncpy(shmPath, argv[3], sizeof(shmPath));
		strncpy(msgType, argv[4], sizeof(msgType));
		strncpy(upstream ,argv[5], sizeof(upstream));
	}
	else if (argc == 2 && strcmp("kill", argv[1]) == 0)
	{
		v = pf->setPid(false, true);
		if (v)
		{
			kill(atoi(v), SIGTERM);
			INFO("sh2transfer(%s) be killed", v);
			return 0;
		}
		else
		{
			INFO("sh2tranfer not run");
			return 1;
		}
	}
	else if (argc == 1)
	{
		v = pf->getEnv("sh2_template_path");
		if (!v)
		{
			ERROR("sh2_template_path not found");
			return 1;
		}
		strcpy(templatePath, v);

		v = pf->getEnv("udp_local_ip");
		if (!v)
		{
			v = pf->getEnv("tcp_ip");
			if (v)
			{
				strcpy(ip, v);
				v = pf->getEnv("port");
				if (!v)
				{
					ERROR("port not found");
					return 1;
				}
				port=atoi(v);
			}
			else
			{
				v = pf->getEnv("sh2_data_path");
				if (!v)
				{
					ERROR("sh2_data_path not found");
					return 1;
				}
				strcpy(dataPath, v);
			}
		}
		else
		{
			strcpy(localIP, v);
			v = pf->getEnv("udp_ip");
			if (!v)
			{
				ERROR("udp_ip not found");
				return 1;
			}
			strcpy(ip, v);
			v = pf->getEnv("port");
			if (!v)
			{
				ERROR("port not found");
				return 1;
			}
			port = atoi(v);
		}

		v = pf->getEnv("sh2_shm_path");
		if (!v)
		{
			ERROR("sh2_shm_path not found");
			return 1;
		}
		strcpy(shmPath, v);

		v = pf->getEnv("sh2_msg_type");
		if (!v)
		{
			ERROR("sh2_msg_type not found");
			return 1;
		}
		strcpy(msgType, v);

		v = pf->getEnv("sh2_upstream_type");
		if (!v)
		{
			ERROR("sh2_upstream_type not found");
			return 1;
		}
		strcpy(upstream, v);
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("sh2transfer tcp: sse_template_path ip port shm_path msg_type upstream");
		ERROR("sh2transfer file: sse_template_path rawdata_path shm_path msg_type upstream");
		ERROR("sh2transfer udp: sse_template_path localip ip port shm_path msg_type upstream");
		return 1;
	}
	if (!pf->setPid(true,true))
	{
		ERROR("sh2transfer already run");
		return 1;
	}

	if (port && localIP[0] == '\0')
	{
		INFO("sh2transfer %s %s %d %s %s %s", templatePath, ip, port, shmPath, msgType, upstream);
		stream = new TCPSocketStream(ip, port);
	}
	else if (localIP[0] == '\0')
	{
		INFO("sh2transfer %s %s %s %s %s", templatePath, dataPath, shmPath, msgType, upstream);
		stream = new FileStream(dataPath);
	}
	else if (port && localIP[0] != '\0')
	{
		INFO("sh2transfer %s %s %s %d %s %s %s", templatePath, localIP, ip, port, shmPath, msgType, upstream);
		stream = new UDPSocketStream(ip, port, localIP);
	}
	MessageReader msgReader;
	ProtocolFactory pFactory;
	PROTOCOL_CONFIG config;

	config.msgType = msgType;

	config.templateName = templatePath;
	config.upstream = upstream;

	IProtocol* protocol = pFactory.createProtocol(&config);
	if (protocol == NULL)
	{
		ERROR("create protocol failed");
		return 1;
	}
	signal(SIGALRM, onSignal);
	signal(SIGTERM, onSignal);
	if (!dh->load(shmPath))
	{
		ERROR("load shared memory %s failed!", shmPath);
		return 1;
	}
	while (true)
	{
		alarm(15);
		if (msgReader.read(protocol, stream))
		{
			alarm(0);
			dh->handle(msgReader.getMessage());
		}
		else
		{
			if (port == 0)
				break;
			else
			{
				WARN("Time out");
				stream->closeStream();
				msgReader.close();
				sleep(5);
			}
		}
	}
	delete stream;
	pf->setPid(false);
	INFO("sh2transfer exit(0)");
	return 0;
}
