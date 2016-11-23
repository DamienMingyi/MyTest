/*
 * Sh1HistorySave.cpp
 *
 *  Created on: 2016年11月2日
 *      Author: user
 */

#include "Sh2HistoryQuotHandle.h"
#include "../../public/stream/FileStream.h"
#include "../../public/protocolfactory/ProtocolFactory.h"
#include "../../public/messagereader/MessageReader.h"
#include "../../public/program/Profile.h"

int main(int argc, char* argv[])
{
	Profile* pf=Profile::Instance("sh2transfer");

	char rawdata_path[256];
	memset(rawdata_path, 0, 256);
	char his_path[256];
	memset(his_path, 0, 256);
	char template_path[256];
	memset(template_path, 0, 256);
	char msgType[256];
	memset(msgType, 0, 256);
	char upstream[256];
	memset(upstream, 0, 256);
	const char* v = NULL;

	if (argc == 1) {
		v = pf->getEnv("sh2_data_path");
		if (v == NULL) {
			ERROR("sh2_data_path not found");
			exit(1);
		}
		strcpy(rawdata_path, v);

		v = pf->getEnv("sh2_his_path");
		if (v == NULL) {
			ERROR("sh2_his_path not found");
			exit(1);
		}
		strcpy(his_path, v);
	}
	else if (argc == 2) {
		strcpy(rawdata_path, argv[1]);

		v = pf->getEnv("sh2_his_path");
		if (v == NULL) {
			ERROR("sh2_his_path not found");
			exit(1);
		}
		strcpy(his_path, v);
	}
	else if (argc == 3) {
		strcpy(rawdata_path, argv[1]);
		strcpy(his_path, argv[2]);
	}
	else {
		ERROR("usage: ./Sh2HistorySave <raw_data_path> <his_file_path>");
		exit(1);
	}

	v = pf->getEnv("sh2_template_path");
	if (v == NULL) {
		ERROR("sh2_template_path not found");
		exit(1);
	}
	strcpy(template_path, v);

	v = pf->getEnv("sh2_msg_type");
	if (v == NULL) {
		ERROR("sh2_msg_type not found");
		exit(1);
	}
	strcpy(msgType, v);

	v = pf->getEnv("sh2_upstream_type");
	if (v == NULL) {
		ERROR("sh2_upstream_type not found");
		exit(1);
	}
	strcpy(upstream, v);

	//get the date
	string strRawPath(rawdata_path);
	int pos1 = strRawPath.rfind('_');
	int pos2 = strRawPath.rfind('.');
	if (pos1 == string::npos || pos2 == string::npos) {
		ERROR("The rawdata is not correct!");
		return -1;
	}
	string strDate = strRawPath.substr(pos1 + 1, pos2 - pos1 -1);
	uint32_t date = atoi(strDate.c_str());

	IDataHandle* sh2History = new Sh2HistoryQuotHandle(his_path, date);
	if (sh2History == NULL) {
		ERROR("create object failed!");
		return -1;
	}

	IStream *stream = new FileStream(rawdata_path);
	if (stream == NULL) {
		ERROR("create object failed!");
		return -1;
	}

	ProtocolFactory* pFactory = new ProtocolFactory();
	if (pFactory == NULL)
	{
		ERROR("create protocol factory failed!");
		return -1;
	}
	PROTOCOL_CONFIG config;
	config.msgType = msgType;
	config.templateName = template_path;
	config.upstream = upstream;

	IProtocol* protocol = pFactory->createProtocol(&config);
	if (protocol == NULL)
	{
		ERROR("create protocol failed");
		return -1;
	}

	MessageReader msgReader;
	while (msgReader.read(protocol, stream)) {
		sh2History->handle(msgReader.getMessage());
	}

	return 0;
}
