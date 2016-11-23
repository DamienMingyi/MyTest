/*
 * ShoProcess.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: level2
 */

#include "ShoProcess.h"

ShoProcess::ShoProcess(Profile* pf, IDataHandle* dh): Process(pf, dh) {
	// TODO Auto-generated constructor stub

}

ShoProcess::~ShoProcess() {
	// TODO Auto-generated destructor stub
	if (dh != NULL)
		delete dh;
}

int32_t ShoProcess::main(int argc, char* argv[])
{
	char sse_template_path[256];
	char ip[256];
	char localIP[256];
	memset(localIP, 0, 256);
	memset(ip, 0, 256);

	int port = 0;
	char data_path[256];
	memset(data_path, 0, sizeof(data_path));
	char shm_path[256];
	memset(shm_path, 0, sizeof(shm_path));
	int mode = 0;
	char securityType[256];
	memset(securityType, 0, sizeof(securityType));
	char msgType[256];
	memset(msgType, 0, sizeof(msgType));
	char upstream[256];
	memset(upstream, 0, sizeof(upstream));
	const char* v;

	IStream* stream = NULL;

	if (argc == 10)
	{
		strncpy(sse_template_path, argv[1], sizeof(sse_template_path));
		strncpy(ip, argv[2], sizeof(ip));
		strncpy(localIP, argv[3], sizeof(localIP));
		port = atoi(argv[4]);
		strncpy(shm_path, argv[5], sizeof(shm_path));
		mode = atoi(argv[6]);
		strncpy(securityType, argv[7], sizeof(securityType));
		strncpy(msgType, argv[8], sizeof(msgType));
		strncpy(upstream, argv[9], sizeof(upstream));
	}
	if (argc == 9)
	{
		strncpy(sse_template_path, argv[1], sizeof(sse_template_path));
		strncpy(ip, argv[2], sizeof(ip));
		port=atoi(argv[3]);
		strncpy(shm_path, argv[4], sizeof(shm_path));
		mode = atoi(argv[5]);
		strncpy(securityType, argv[6], sizeof(securityType));
		strncpy(msgType, argv[7], sizeof(msgType));
		strncpy(upstream, argv[8], sizeof(upstream));
	}
	else if (argc == 8)
	{
		strncpy(sse_template_path, argv[1], sizeof(sse_template_path));
		strncpy(data_path, argv[2], sizeof(data_path));
		strncpy(shm_path, argv[3], sizeof(shm_path));
		mode = atoi(argv[4]);
		strncpy(securityType, argv[5], sizeof(securityType));
		strncpy(msgType, argv[6], sizeof(msgType));
		strncpy(upstream, argv[7], sizeof(upstream));
	}
	else if (argc == 2 && strcmp(argv[1], "kill") == 0)
	{
		v = pf->setPid(false, true);
		if (v)
		{
			kill(atoi(v), SIGTERM);
			INFO("shotransfer(%s) be killed", v);
			return 0;
		}
		else
		{
			INFO("shotransfer not run");
			return 1;
		}
	}
	else if (argc == 1)
	{
		v = pf->getEnv("sho_template_path");
		if (!v)
		{
			ERROR("sho_template_path not found");
			return 1;
		}
		strcpy(sse_template_path, v);

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
				v = pf->getEnv("sho_data_path");
				if (!v)
				{
					ERROR("sho_data_path not found");
					return 1;
				}
				strcpy(data_path, v);
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

		v = pf->getEnv("sho_shm_path");
		if (!v)
		{
			ERROR("sho_shm_path not found");
			return 1;
		}
		strcpy(shm_path, v);

		v = pf->getEnv("sho_mode");
		if(!v)
		{
			ERROR("sho_mode not found");
			return 1;
		}
		mode = atoi(v);

		v = pf->getEnv("sho_security_type");
		if (!v)
		{
			ERROR("sho_security_type not found");
			return 1;
		}
		strcpy(securityType, v);

		v = pf->getEnv("sho_msg_type");
		if (!v)
		{
			ERROR("sho_msg_type not found");
			return 1;
		}
		strcpy(msgType, v);

		v = pf->getEnv("sho_upstream_type");
		if (!v)
		{
			ERROR("sho_upstream_type not found");
			return 1;
		}
		strcpy(upstream, v);
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("shotransfer tcp: sse_template_path ip port shm_path mode security_type msg_type upstream");
		ERROR("shotransfer file: sse_template_path rawdata_path shm_path mode security_type msg_type upstream");
		ERROR("shotransfer udp: sse_template_path ip localip port shm_path mode security_type msg_type upstream");
		return 1;
	}

	if (!pf->setPid(true,true))
	{
		ERROR("shotransfer already run");
		return 1;
	}

	if (port && localIP[0] == '\0')
	{
		INFO("shotransfer %s %s %d %s %d %s %s %s", sse_template_path, ip, port, shm_path,
				mode, securityType, msgType, upstream);
		stream = new TCPSocketStream(ip, port);
	}
	else if (localIP[0] == '\0')
	{
		INFO("shotransfer %s %s %s %d %s %s %s", sse_template_path, data_path, shm_path,
				mode, securityType, msgType, upstream);
		stream = new FileStream(data_path);
	}
	else if (port && localIP[0] != '\0')
	{
		INFO("shotransfer %s %s %s %d %s %d %s %s %s", sse_template_path, localIP, ip, port,
				shm_path, mode, securityType, msgType, upstream);
		stream = new UDPSocketStream(ip, port, localIP);
	}

	MessageReader msgReader;
	ProtocolFactory pFactory;
	PROTOCOL_CONFIG config;
	config.mode = mode;
	config.msgType = msgType;
	config.securityType = securityType;
	config.templateName = sse_template_path;
	config.upstream = upstream;

	IProtocol* protocol = pFactory.createProtocol(&config);
	if (protocol == NULL)
	{
		ERROR("create protocol failed");
		return 1;
	}

	signal(SIGALRM, onSignal);
	signal(SIGTERM, onSignal);
	if (!dh->load(shm_path))
	{
		ERROR("load shared memory %s failed!", shm_path);
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
	INFO("shotransfer exit(0)");
	return 0;
}
