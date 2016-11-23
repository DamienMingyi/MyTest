/*
 * csiptransfer.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: level2
 */

#include "../quothandle/CSIProcess.h"
#include "CSIRealTimeQuotHandle.h"

int main(int argc, char* argv[])
{
	Profile* pf = Profile::Instance("csitransfer");
	IDataHandle* csi = new CSIRealTimeQuotHandle();
	if (csi == NULL)
	{
		ERROR("create csi handle failed!");
		return 1;
	}
	CSIProcess ps(pf, csi);
	return ps.main(argc, argv);
}



