/*
 * Message.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: level2
 */

#include "Message.h"

Message::Message(): bf(NULL) {
	// TODO Auto-generated constructor stub

}

Message::~Message() {
	// TODO Auto-generated destructor stub
}

void Message::clear()
{
	this->vectorDecoderMessage.clear();
	this->bf = NULL;
}
