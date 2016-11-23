/*
 * sh2transfer.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: level2
 */

#include "../quothandle/Sh2Process.h"
#include "Sh2RealTimeQuotHandle.h"

int main(int argc, char* argv[])
{
	Profile* pf = Profile::Instance("sh2transfer");
	IDataHandle* sh2 = new Sh2RealTimeQuotHandle();
	if (sh2 == NULL)
	{
		ERROR("create sh2 handle failed!");
		return 1;
	}
	Sh2Process ps(pf, sh2);
	return ps.main(argc, argv);
}



