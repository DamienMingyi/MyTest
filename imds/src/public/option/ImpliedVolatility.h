/*
 * ImpliedVolatility.h
 *
 *  Created on: Jan 9, 2015
 *      Author: level2
 */

#ifndef IMPLIEDVOLATILITY_H_
#define IMPLIEDVOLATILITY_H_

#define PI 3.14159265359

class ImpliedVolatility {
public:
	ImpliedVolatility();
	virtual ~ImpliedVolatility();

	double impliedVolatility(double p, double s, double k, double t, double r, char* cp);
	double cumulativeDistribution(double x);
	double normalDistribution(double x);
	double d1(double s, double k, double t, double r, double v);
	double d2(double s, double k, double t, double r, double v);
private:
	double fv(double s, double k, double t, double r, double v, char* cp);
};

#endif /* IMPLIEDVOLATILITY_H_ */
