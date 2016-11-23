/*
 * Parse.h
 *
 *  Created on: Oct 13, 2015
 *      Author: level2
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>

/// @brief Signed 16 bit integer. AKA: char on some, but not all platforms.
typedef boost::int64_t int64;
/// @brief 64 bit signed integer
typedef boost::int64_t int64;
/// @brief the mantissa portion of a Decimal
typedef int64 mantissa_t;
/// @brief the exponent portion of a Decimal
typedef int64 exponent_t;

class Parse {
public:
	Parse();
	virtual ~Parse();

	void normalize();
	void parse(const std::string & value);

	long getMantissa();
	int getExponent();

private:
	mantissa_t mantissa_;
	exponent_t exponent_;
};

#endif /* PARSE_H_ */
