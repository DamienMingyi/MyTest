/*
 * Message.h
 *
 *  Created on: Oct 10, 2016
 *      Author: level2
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "../fast/FastCodecs.h"
#include <vector>

typedef std::vector<FastCodecs::DecoderMessage> vecDM;

class Message {
public:
	Message();
	virtual ~Message();

	void clear();
public:
	vecDM vectorDecoderMessage;
	char* bf;
};

#endif /* MESSAGE_H_ */
