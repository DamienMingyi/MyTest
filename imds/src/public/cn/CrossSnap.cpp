/*
 * CrossSnap.cpp
 *
 *  Created on: 2016年10月21日
 *      Author: mds
 */

#include "../cn/CrossSnap.h"

CrossSnap::CrossSnap() {
	// TODO Auto-generated constructor stub
	snap = NULL;
}

CrossSnap::~CrossSnap() {
	// TODO Auto-generated destructor stub
}


bool CrossSnap::getValue(int id,char* v)
{
	if (snap == NULL)	return false;

	switch (id)
	{
	case SNAP_SECURITY_ID:
		strncpy(v, snap->securityID, sizeof(snap->securityID));
		return true;
	default:
		return false;
	}
}

bool CrossSnap::getValue(int id,int& v)
{
	if (id >= 0 && snap == NULL)
		return false;
	switch (id)
	{
	case SNAP_SIZE:
		v = sizeof(SNAP);
		return true;
	case SNAP_DATE:
		v = snap->date;
		return true;
	case SNAP_INIT_TIME:
		if (strcmp(snap->securityID, "000000") == 0)
		{
			v = snap->time;
			return true;
		}
		return false;
	case SNAP_TOTAL:
		if (strcmp(snap->securityID,"000000") == 0)
		{
			v = snap->amount;
			return true;
		}
		return false;
	case SNAP_POS:
		v = snap->pos;
		return true;
	case SNAP_LINE_TIME:
		v = snap->lineTime;
		return true;
	default:
		return false;
	}
}
