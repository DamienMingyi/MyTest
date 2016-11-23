/*
 * ShoProcess.h
 *
 *  Created on: Oct 24, 2016
 *      Author: level2
 */

#ifndef SHOPROCESS_H_
#define SHOPROCESS_H_

#include "../../public/process/Process.h"

static void onSignal(int sig)
{
	if (sig == SIGTERM)
	{
		INFO("shotransfer exit(1)");
		exit(1);
	}
}

class ShoProcess: public Process {
public:
	ShoProcess(Profile* pf, IDataHandle* dh);
	virtual ~ShoProcess();

	virtual int32_t main(int argc, char* argv[]);
};

#endif /* SHOPROCESS_H_ */
