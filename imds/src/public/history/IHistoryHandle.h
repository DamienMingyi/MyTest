/*
 * IHistoryHandle.h
 *
 *  Created on: 2016/9/28/
 *      Author: user
 */

#ifndef SRC_PUBLIC_HISTORY_IHISTORYHANDLE_H_
#define SRC_PUBLIC_HISTORY_IHISTORYHANDLE_H_

#include "../handle/IDataHandle.h"

class IHistoryHandle {
public:
	IHistoryHandle();
	virtual ~IHistoryHandle();
	virtual void setCurHandle();

	static IHistoryHandle *curHandle; //the object callback function uses
	static void Callback(char* data, QuotType qt); //callback function

protected:
	virtual void Transfer(char* data, QuotType qt) = 0;
};

#endif /* SRC_PUBLIC_HISTORY_IHISTORYHANDLE_H_ */
