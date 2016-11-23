/*
 * FastCodecs.h
 *
 *  Created on: Jan 22, 2015
 *      Author: level2
 */

#ifndef FASTCODECS_H_
#define FASTCODECS_H_

#include <string.h>
#include <assert.h>
#include <vector>

#include "EncoderMessage.h"

using namespace std;

namespace FastCodecs
{
	class FastTemplate;
	class IDirector;

	class FastCodecs
	{
	public:
		FastCodecs();
		virtual ~FastCodecs();

	public:
		bool loadTemplate(const char* tmplFileName);
		bool parse(const char* Template, size_t nBytes = (size_t)-1);
		bool setTemplate(uint32_t templId);
		bool encode(EncoderMessage &em, uchar_t* buf, size_t &length, uint32_t tmplId = 0);
		int decode(uchar_t* buf, size_t length, vector<DecoderMessage> &vecDm);
		const unsigned char* getBuffer(size_t &length);
		void printEm(EncoderMessage &out_);
	private:
		IDirector* ptrEncoderDirector;
		FastTemplate* ptrTemplate;
		static const char L2Fast_VERSION[256];
	};
}
#endif

