/*
 * Sh1Process.h
 *
 *  Created on: Oct 25, 2016
 *      Author: level2
 */

#ifndef SH1PROCESS_H_
#define SH1PROCESS_H_

#include "../../public/process/Process.h"

static void onSignal(int sig)
{
	if (sig == SIGTERM)
	{
		INFO("sh1transfer exit(1)");
		exit(1);
	}
}

class Sh1Process: public Process {
public:
	Sh1Process(Profile* pf, IDataHandle* dh);
	virtual ~Sh1Process();

	virtual int32_t main(int argc, char* argv[]);
};

#endif /* SH1PROCESS_H_ */
