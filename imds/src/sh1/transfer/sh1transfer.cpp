/*
 * sh1transfer.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: level2
 */

#include "../quothandle/Sh1Process.h"
#include "Sh1RealTimeQuotHandle.h"

int main(int argc, char* argv[])
{
	Profile* pf = Profile::Instance("sh1transfer");
	IDataHandle* sh1 = new Sh1RealTimeQuotHandle();
	if (sh1 == NULL)
	{
		ERROR("create sh1 handle failed!");
		return 1;
	}
	Sh1Process ps(pf, sh1);
	return ps.main(argc, argv);
}


