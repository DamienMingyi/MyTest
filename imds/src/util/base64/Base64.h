/*
 * Base64.h
 *
 *  Created on: 2013-5-30
 *      Author: level2
 */

#ifndef BASE64_H_
#define BASE64_H_

class Base64 {
public:
	Base64();
	virtual ~Base64();
	static unsigned long Decode(const char *_rEncoded, char *_rRawData,unsigned long ulInputLen);
	static void Encode(const char *_rRawData, char *_rEncoded, unsigned long SrcLen);
private:
	static char getOffset(char ch);
};

#endif /* BASE64_H_ */
