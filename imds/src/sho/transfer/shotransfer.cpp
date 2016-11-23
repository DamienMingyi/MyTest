/*
 * shotransfer.cpp
 *
 *  Created on: Sep 12, 2016
 *      Author: level2
 */

#include "../quothandle/ShoProcess.h"
#include "ShoRealTimeQuotHandle.h"

int main(int argc, char* argv[])
{
	Profile* pf = Profile::Instance("shotransfer");
	IDataHandle* sho = new ShoRealTimeQuotHandle();
	if (sho == NULL)
	{
		ERROR("create sho handle failed!");
		return 1;
	}
	ShoProcess ps(pf, sho);
	return ps.main(argc, argv);
}
