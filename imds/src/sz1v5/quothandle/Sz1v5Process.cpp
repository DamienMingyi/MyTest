/*
 * Sz1v5Process.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */

#include "Sz1v5Process.h"
#include "../../public/protocol/SZV5Protocol.h"

Sz1v5Process::Sz1v5Process(Profile* pf, IDataHandle* dh): Process(pf, dh) {
	// TODO Auto-generated constructor stub

}

Sz1v5Process::~Sz1v5Process() {
	// TODO Auto-generated destructor stub
	if (dh != NULL)
		delete dh;
}

int32_t Sz1v5Process::main(int argc, char* argv[])
{
	char ip[256];
	char localIP[256];
	memset(localIP, 0, 256);
	memset(ip, 0, 256);

	int32_t port = 0;
	char data_path[256];
	memset(data_path, 0, sizeof(data_path));
	char shm_path[256];
	memset(shm_path, 0, sizeof(shm_path));
	char msgType[256];
	memset(msgType, 0, sizeof(msgType));
	char upstream[256];
	memset(upstream, 0, sizeof(upstream));

	char password[16];
	memset(password, 0, sizeof(password));
	char senderCompID[20];
	memset(senderCompID, 0, sizeof(senderCompID));
	char targetCompID[20];
	memset(targetCompID, 0, sizeof(targetCompID));
	int32_t heartBeat = 0;
	char defaultApplVerID[32];
	memset(defaultApplVerID, 0, sizeof(defaultApplVerID));

	const char* v;

	IStream* stream = NULL;

	if (argc == 11)
	{
		strncpy(ip, argv[1], sizeof(ip));
		strncpy(localIP, argv[2], sizeof(localIP));
		port = atoi(argv[3]);
		strncpy(shm_path, argv[4], sizeof(shm_path));
		strncpy(msgType, argv[5], sizeof(msgType));
		strncpy(password, argv[6], sizeof(password));
		strncpy(senderCompID, argv[7], sizeof(senderCompID));
		strncpy(targetCompID, argv[8], sizeof(targetCompID));
		strncpy(defaultApplVerID, argv[9], sizeof(defaultApplVerID));
		strncpy(upstream, argv[10], sizeof(upstream));
	}
	else if (argc == 11)
	{
		strncpy(ip, argv[1], sizeof(ip));
		port = atoi(argv[2]);
		strncpy(shm_path, argv[3], sizeof(shm_path));
		strncpy(msgType, argv[4], sizeof(msgType));
		strncpy(password, argv[5], sizeof(password));
		strncpy(senderCompID, argv[6], sizeof(senderCompID));
		strncpy(targetCompID, argv[7], sizeof(targetCompID));
		heartBeat = atoi(argv[8]);
		strncpy(defaultApplVerID, argv[9], sizeof(defaultApplVerID));
		strncpy(upstream, argv[10], sizeof(upstream));
	}
	else if (argc ==9)
	{
		strncpy(data_path, argv[1], sizeof(data_path));
		strncpy(shm_path, argv[2], sizeof(shm_path));
		strncpy(msgType, argv[3], sizeof(msgType));
		strncpy(password, argv[4], sizeof(password));
		strncpy(senderCompID, argv[5], sizeof(senderCompID));
		strncpy(targetCompID, argv[6], sizeof(targetCompID));
		strncpy(defaultApplVerID, argv[7], sizeof(defaultApplVerID));
		strncpy(upstream, argv[8], sizeof(upstream));
	}
	else if (argc == 2 && strcmp(argv[1], "kill") == 0)
	{
		v = pf->setPid(false, true);
		if (v)
		{
			kill(atoi(v), SIGTERM);
			INFO("sz1v5transfer (%s) be killed", v);
			return 0;
		}
		else
		{
			INFO("sz1v5transfer not run");
			return 1;
		}
	}
	else if (argc == 1)
	{
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
				port = atoi(v);
			}
			else
			{
				v = pf->getEnv("sz1_data_path");
				if (!v)
				{
					ERROR("sz1_data_path not found");
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
				ERROR("port not foudn");
				return 1;
			}
			port = atoi(v);
		}
		v = pf->getEnv("sz1_shm_path");
		if (!v)
		{
			ERROR("sz1 not found");
			return 1;
		}
		strcpy(shm_path, v);

		v = pf->getEnv("password");
		if (!v)
		{
			ERROR("password not found");
			return 1;
		}
		strcpy(password, v);

		v = pf->getEnv("sz1_msg_type");
		if (!v)
		{
			ERROR("sz1_msg_type not found");
			return 1;
		}
		strcpy(msgType, v);

		v = pf->getEnv("senderCompID");
		if (!v)
		{
			ERROR("senderCompID not found");
			return 1;
		}
		strcpy(senderCompID, v);

		v = pf->getEnv("targetCompID");
		if (!v)
		{
			ERROR("targetCompID not found");
			return 1;
		}
		strcpy(targetCompID, v);

		v = pf->getEnv("heart_beat");
		if (!v)
		{
			ERROR("heat_beat not found");
			return 1;
		}
		heartBeat = atoi(v);

		v = pf->getEnv("defaultApplVerID");
		if (!v)
		{
			ERROR("defaultApplVerID not found");
			return 1;
		}
		strcpy(defaultApplVerID, v);

		v = pf->getEnv("sz1_upstream_type");
		if (!v)
		{
			ERROR("sz1_upstream_type not found");
			return 1;
		}
		strcpy(upstream, v);
	}
	else
	{
		ERROR("argument error! usage:");
		ERROR("sz1v5transfer tcp: ip port shm_path msg_type password senderCompID targetCompID heart_beat defaultApplVerID upstream");
		ERROR("sz1v5transfer file: rawdata_path msg_type password senderCompID targetCompID defaultApplVerID upstream");
		ERROR("sz1v5transfer udp: ip localip port shm_path msg_type password senderCompID targetCompID defaultApplVerID upstream");
	}

	if (!pf->setPid(true,true))
	{
		ERROR("shotransfer already run");
		return 1;
	}

	if (port && localIP[0] == '\0')
	{
		INFO("shotransfer %s %d %s %s %s %s %s %d %s %s", ip, port, shm_path, msgType, password, senderCompID,
				targetCompID, heartBeat, defaultApplVerID, upstream);
		stream = new TCPSocketStream(ip, port);
	}
	else if (localIP[0] == '\0')
	{
		INFO("shotransfer %s %s %s %s %s %s %s %s", data_path, shm_path, msgType, password, senderCompID,
				targetCompID, defaultApplVerID, upstream);
		stream = new FileStream(data_path);
	}
	else if (port && localIP[0] != '\0')
	{
		INFO("shotransfer %s %s %d %s %s %s %s %s %s %s", localIP, ip, port, shm_path, msgType, password, senderCompID,
				targetCompID, defaultApplVerID, upstream);
		stream = new UDPSocketStream(ip, port, localIP);
	}
	if (stream == NULL)
	{
		ERROR("create stream failed!");
		return 1;
	}

	MessageReader msgReader;
	ProtocolFactory pFactory;
	PROTOCOL_CONFIG config;
	config.msgType = msgType;
	config.upstream = upstream;
	config.defaultApplVerID = defaultApplVerID;
	config.heartBtInt = heartBeat;
	config.password = password;
	config.senderCompID = senderCompID;
	config.targetCompID = targetCompID;

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

	char hbt[1024];
	memset(hbt, 0, sizeof(hbt));

	SZV5Protocol szv5protocol(msgType, senderCompID, targetCompID, heartBeat, password, defaultApplVerID);
	int32_t hbtLength = szv5protocol.getHeartBeat(hbt);
	time_t t0 = time(NULL);
	time_t t1 = time(NULL);
	while (true)
	{
		alarm(15);
		if (msgReader.read(protocol, stream))
		{
			alarm(0);
			dh->handle(msgReader.getMessage());

			t1 = time(NULL);
			if (difftime(t1, t0) >= heartBeat)
			{
				stream->writeStream(hbt, hbtLength);
				t0 = time(NULL);
			}
		}
		else
		{
			if (port == 0)
				break;
			else
			{
				WARN("Time out");
				stream->closeStream();
				sleep(5);
			}
		}
	}
	delete stream;
	pf->setPid(false);
	INFO("sz1v5transfer exit(0)");
	return 0;
}
