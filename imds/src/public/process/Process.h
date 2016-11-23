/*
 * Process.h
 *
 *  Created on: Oct 24, 2016
 *      Author: level2
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <signal.h>
#include "../handle/IDataHandle.h"
#include "../program/Profile.h"
#include "../stream/FileStream.h"
#include "../stream/TCPSocketStream.h"
#include "../stream/UDPSocketStream.h"
#include "../messagereader/MessageReader.h"
#include "../protocolfactory/ProtocolFactory.h"

class Process {
public:
	Process(Profile* pf, IDataHandle* dh);
	virtual ~Process();

	virtual int32_t main(int argc, char* argv[]) = 0;
protected:
	Profile* pf;
	IDataHandle* dh;
};

#endif /* PROCESS_H_ */
