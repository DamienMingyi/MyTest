/*
 * Sz1v5Process.h
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */

#ifndef SZ1V5PROCESS_H_
#define SZ1V5PROCESS_H_

#include "../../public/process/Process.h"

static void onSignal(int sig)
{
	if (sig == SIGTERM)
	{
		INFO("sz1v5transfer exit(1)");
		exit(1);
	}
}

class Sz1v5Process: public Process {
public:
	Sz1v5Process(Profile* pf, IDataHandle* dh);
	virtual ~Sz1v5Process();

	virtual int32_t main(int argc, char* argv[]);
};

#endif /* SZ1V5PROCESS_H_ */
