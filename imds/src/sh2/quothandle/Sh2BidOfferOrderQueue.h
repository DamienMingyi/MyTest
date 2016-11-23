/*
 * Sh2BidOfferOrderQueue.h
 *
 *  Created on: Nov 11, 2016
 *      Author: level2
 */

#ifndef SH2BIDOFFERORDERQUEUE_H_
#define SH2BIDOFFERORDERQUEUE_H_

#include <stdint.h>
#include "../../public/stock2/StockSnap2.h"

struct QUEUE
{
	int32_t orderQueueOperator;
	int32_t orderQueueOperatorEntryID;
	int64_t orderQty;
};

static const int Q_LENGTH = 100;

struct ORDER
{
	int32_t priceLevelOperator;
	int32_t price;
	int64_t orderQty;
	int32_t numOrders;
	int32_t noOrders;
	QUEUE queue[Q_LENGTH];
};

static const int LEVEL_NUM = 20;

static const int ADD = 1;
static const int UPDATE = 2;
static const int DELETE = 3;

static const int FULL_IMAGE = 1;
static const int UPDATED_IMAGE = 2;

static const int BID = 0;
static const int OFFER = 1;

class Sh2BidOfferOrderQueue {
public:
	Sh2BidOfferOrderQueue();
	virtual ~Sh2BidOfferOrderQueue();

	bool sort(STOCKSNAP2* stocksnap2, ORDER order[][LEVEL_NUM], int32_t* noLevel, int imageStatus);
private:
	bool sortFullImage(STOCKSNAP2* stocksnap2, ORDER order[][LEVEL_NUM], int32_t noLevel[]);
	bool sortUpdatedImage(STOCKSNAP2* stocksnap2, ORDER order[][LEVEL_NUM], int32_t noLevel[]);

	bool sortUpdatePrice(LEVELS* levels, const ORDER& order);
	bool sortDeletePrice(LEVELS* levels, const ORDER& order);

	bool sortAddBidPrice(LEVELS* levels, const ORDER& order);
	bool sortAddOfferPrice(LEVELS* levels, const ORDER& order);

	int numLevel(LEVELS* levels);
};

#endif /* SH2BIDOFFERORDERQUEUE_H_ */
