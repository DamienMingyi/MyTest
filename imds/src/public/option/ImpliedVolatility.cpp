/*
 * ImpliedVolatility.cpp
 *
 *  Created on: Jan 9, 2015
 *      Author: level2
 */

#include "ImpliedVolatility.h"
#include <cmath>
#include <cstring>

ImpliedVolatility::ImpliedVolatility() {
	// TODO Auto-generated constructor stub

}

ImpliedVolatility::~ImpliedVolatility() {
	// TODO Auto-generated destructor stub
}

/**
 * calculate implied volatility
 * @param [in] p option price
 * @param [in] s underlying stock price
 * @param [in] k exercise price
 * @param [in] t remaining days
 * @param [in] r risk free interest rate
 * @param [in] cp call or put
 */
double ImpliedVolatility::impliedVolatility(double p, double s, double k, double t, double r, char* cp)
{
	//step 1
	double v_h = 1.0;
	double v_l = 0.01;
	//step 2
	while(fv(s, k, t, r, v_h, cp) < p)
	{
		v_l = v_h;
		v_h = 2.0 * v_h;
		if(v_h > 20.0)
			return 20.0;
	}
	//step 3
	while(fv(s, k, t, r, v_l, cp) > p)
	{
		v_h = v_l;
		v_l = v_l / 2.0;
		if(v_l < 0.0001)
			return 0.0001;
	}
	//step 4
	double t_p;
	while(fabs((t_p = fv(s, k, t, r, (v_h + v_l) / 2.0, cp)) - p) >= 0.0001)
	{
		if(v_h > 20.0)
			return 20.0;
		if(v_l < 0.0001)
			return 0.0001;
		if(t_p > p)
			v_h = (v_h + v_l) / 2.0;
		else if(t_p < p)
			v_l = (v_h + v_l) / 2.0;
	}
	return (v_h + v_l) / 2.0;
}

double ImpliedVolatility::cumulativeDistribution(double x)
{
	if(x > 6)
		return 1;
	if(x < -6)
		return 0;
	static const double gamma = 0.231641900,
			a1 = 0.319381530,
			a2 = -0.356563782,
			a3  =  1.781477973,
			a4  = -1.821255978,
			a5  =  1.330274429;
	double k = 1.0 / (1 + fabs(x) * gamma);
	double n = k * (a1 + k * (a2 + k * (a3 + k * (a4 + k * a5))));
	n = 1 - normalDistribution(x) * n;
	if(x < 0)
		return 1.0 - n;
	return n;

}

double ImpliedVolatility::normalDistribution(double x)
{
	return exp(-pow(x, 2.0) / 2.0) / sqrt(2 * PI);
}

double ImpliedVolatility::d1(double s, double k, double t, double r, double v)
{
	return (log(s / k) + (r + pow(v, 2.0) / 2.0) * t) / (v * sqrt(t));
}

double ImpliedVolatility::d2(double s, double k, double t, double r, double v)
{
	return (log(s / k) + (r - pow(v, 2.0) / 2.0) * t) / (v * sqrt(t));
}

/**
 * calculate market price
 * @param [in] s underlying stock price
 * @param [in] k exercise price
 * @param [in] t remaining days
 * @param [in] r risk free interest rate
 * @param [in] v volatility
 * @param [in] cp call or put
 */
double ImpliedVolatility::fv(double s, double k, double t, double r, double v, char* cp)
{
	if(cp[0] == 'C')
		return s * cumulativeDistribution(d1(s, k, t, r, v)) - k * exp(-r * t) * cumulativeDistribution(d2(s, k, t, r, v));
	else if(cp[0] == 'P')
		return k * exp(-r * t) * cumulativeDistribution(-d2(s, k, t, r, v)) - s * cumulativeDistribution(-d1(s, k, t, r, v));
}

