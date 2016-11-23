/*
 * RiskMeasureIndex.h
 *
 *  Created on: Apr 14, 2015
 *      Author: level2
 */

#ifndef RISKMEASUREINDEX_H_
#define RISKMEASUREINDEX_H_

#include "ImpliedVolatility.h"

class RiskMeasureIndex {
public:
	RiskMeasureIndex();
	virtual ~RiskMeasureIndex();

	double getDelta(double p, double s, double k, double t, double r, char* cp, double v);
	double getGamma(double p, double s, double k, double t, double r, char* cp, double v);
	double getTheta(double p, double s, double k, double t, double r, char* cp, double v);
	double getVega(double p, double s, double k, double t, double r, char* cp, double v);
	double getRho(double p, double s, double k, double t, double r, char* cp, double v);
	double getImpliedVolatility(double p, double s, double k, double t, double r, char* cp);
private:

	ImpliedVolatility iv;
};

#endif /* RISKMEASUREINDEX_H_ */
