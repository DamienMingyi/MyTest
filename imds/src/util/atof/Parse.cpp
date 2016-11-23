/*
 * Parse.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: level2
 */

#include "Parse.h"

Parse::Parse():mantissa_(0), exponent_(0) {
	// TODO Auto-generated constructor stub

}

Parse::~Parse() {
	// TODO Auto-generated destructor stub
}

void Parse::normalize()
{
	  while(mantissa_ != 0 && mantissa_ % 10 == 0 && exponent_ < 128)
	  {
	    mantissa_ /= 10;
	    exponent_ += 1;
	  }
	  if(exponent_ > 128)
	  {
	    if(true)
	    {
	 //     throw OverflowError("[ERR R1]Decimal Exponent overflow.");
	    	throw 1;
	    }
	    while(exponent_ > 128)
	    {
	      mantissa_ *= 10;
	      exponent_ -= 1;
	    }
	  }
	  if(exponent_ < -128)
	  {
	    if(true)
	    {
	 //     throw OverflowError("[ERR R1]Decimal Exponent undeflow.");
	    	throw 2;
	    }
	    while(exponent_ < -128)
	    {
	      mantissa_ /= 10;
	      exponent_ += 1;
	    }
	  }
}

void Parse::parse(const std::string & value)
{
	  std::string str = value;
	  boost::algorithm::trim(str);
	  size_t dotPos = str.find(".");
	  std::string wholeString = str.substr(0,dotPos);
	  std::string fracString;
	  if(dotPos != std::string::npos)
	  {
	    fracString = str.substr(dotPos+1);
	  }
	  std::string mantissaString = wholeString+fracString;
	  exponent_ = -exponent_t(fracString.size());

	  bool overflow = false;

	  // VC8 stringstream is truncating a large string instead of throwing
	  // the exception, so we'll always fall back on the overflow code.
	#if defined _MSC_VER && _MSC_VER < 1500
	  overflow = true;
	#else
	  try {
	    mantissa_ = boost::lexical_cast<mantissa_t>(mantissaString);
	  }
	  catch (std::exception &)
	  {
	    overflow = true;
	  }
	#endif

	  if (overflow && true)
	  {
	    size_t pos = mantissaString.find_last_not_of ("0");
	    exponent_ += exponent_t(mantissaString.length () - pos - 1);
	    mantissaString = mantissaString.substr (0, pos + 1);
	    mantissa_ = boost::lexical_cast<mantissa_t>(mantissaString);
	  }

	  if(true)
	  {
	    normalize();
	  }
}

long Parse::getMantissa()
{
	return static_cast<long>(mantissa_);
}

int Parse::getExponent()
{
	return static_cast<int>(exponent_);
}
