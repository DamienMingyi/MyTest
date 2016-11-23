/*
 * RiskMeasureIndex.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: level2
 */

#include "RiskMeasureIndex.h"
#include <cmath>
#include <cstring>


/**
 * constructor
 * @param [in] p option price
 * @param [in] s underlying stock price
 * @param [in] k exercise price
 * @param [in] t remaining days
 * @param [in] r risk free interest rate
 * @param [in] cp call or put
 */
RiskMeasureIndex::RiskMeasureIndex() {
	// TODO Auto-generated constructor stub
}

RiskMeasureIndex::~RiskMeasureIndex() {
	// TODO Auto-generated destructor stub

}

double RiskMeasureIndex::getDelta(double p, double s, double k, double t, double r, char* cp, double v)
{
	if(cp[0] == 'C')
		return iv.cumulativeDistribution(iv.d1(s, k, t, r, v));
	else if(cp[0] == 'P')
		return -iv.cumulativeDistribution(-iv.d1(s, k, t, r, v));
}

double RiskMeasureIndex::getGamma(double p, double s, double k, double t, double r, char* cp, double v)
{
	return iv.normalDistribution(iv.d1(s, k, t, r, v)) / (s * v * sqrt(t));
}

double RiskMeasureIndex::getTheta(double p, double s, double k, double t, double r, char* cp, double v)
{
	if(cp[0] == 'C')
		return -s * iv.normalDistribution(iv.d1(s, k, t, r, v)) * v / (2 * sqrt(t))
				- r * k * exp(-r * t) * iv.cumulativeDistribution(iv.d2(s, k, t, r, v));
	else if(cp[0] == 'P')
		return -s * iv.normalDistribution(iv.d1(s, k, t, r, v)) * v / (2 * sqrt(t))
				+ r * k * exp(-r * t) * iv.cumulativeDistribution(-iv.d2(s, k, t, r, v));
}

double RiskMeasureIndex::getVega(double p, double s, double k, double t, double r, char* cp, double v)
{
	return s * sqrt(t) * iv.normalDistribution(-iv.d1(s, k, t, r, v));
}

double RiskMeasureIndex::getRho(double p, double s, double k, double t, double r, char* cp, double v)
{
	if(cp[0] == 'C')
		return k * t * exp(-r * t) * iv.cumulativeDistribution(iv.d2(s, k, t, r, v));
	else if(cp[0] == 'P')
		return -k * t * exp(-r * t) * iv.cumulativeDistribution(-iv.d2(s, k, t, r, v));
}

double RiskMeasureIndex::getImpliedVolatility(double p, double s, double k, double t, double r, char* cp)
{
	return iv.impliedVolatility(p, s, k, t, r, cp);
}
