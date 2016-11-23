/*
 * CSIProcess.h
 *
 *  Created on: Nov 15, 2016
 *      Author: level2
 */

#ifndef CSIPROCESS_H_
#define CSIPROCESS_H_

#include "../../public/process/Process.h"

static void onSignal(int sig)
{
	if (sig == SIGTERM)
	{
		INFO("csitransfer exit(1)");
		exit(1);
	}
}

class CSIProcess: public Process {
public:
	CSIProcess(Profile* pf, IDataHandle* dh);
	virtual ~CSIProcess();

	virtual int32_t main(int argc, char* argv[]);
};

#endif /* CSIPROCESS_H_ */
