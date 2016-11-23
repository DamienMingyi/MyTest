/*
 * sz1v5transfer.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: level2
 */


#include "Sz1v5RealTimeQuotHandle.h"
#include "../quothandle/Sz1v5Process.h"

int main(int argc, char* argv[])
{
	Profile* pf = Profile::Instance("sz1v5transfer");
	IDataHandle* sz1v5 = new Sz1v5RealTimeQuotHandle();
	if (sz1v5 == NULL)
	{
		ERROR("create sz1v5 handle failed!");
		return 1;
	}
	Sz1v5Process ps(pf, sz1v5);
	return ps.main(argc, argv);
}
