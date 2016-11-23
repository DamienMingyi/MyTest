/*
 * IHistoryHandle.cpp
 *
 *  Created on: 2016/9/28/
 *      Author: user
 */

#include "IHistoryHandle.h"
#include <stdio.h>

IHistoryHandle::IHistoryHandle() {
	// TODO Auto-generated constructor stub

}

IHistoryHandle::~IHistoryHandle() {
	// TODO Auto-generated destructor stub
}

IHistoryHandle* IHistoryHandle::curHandle = NULL;

void IHistoryHandle::Callback(char* data, QuotType qt)
{
	curHandle->Transfer(data, qt);
}

void IHistoryHandle::setCurHandle()
{
	curHandle = this;
}
