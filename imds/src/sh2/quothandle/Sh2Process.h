/*
 * Sh2Process.h
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#ifndef SH2PROCESS_H_
#define SH2PROCESS_H_

#include "../../public/process/Process.h"

static void onSignal(int sig)
{
	if (sig == SIGTERM)
	{
		INFO("sh2transfer exit(1)");
		exit(1);
	}
}

class Sh2Process: public Process {
public:
	Sh2Process(Profile* pf, IDataHandle* dh);
	virtual ~Sh2Process();

	virtual int32_t main(int argc, char* argv[]);
};

#endif /* SH2PROCESS_H_ */
