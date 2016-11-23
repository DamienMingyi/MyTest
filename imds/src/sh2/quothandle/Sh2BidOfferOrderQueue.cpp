/*
 * Sh2BidOfferOrderQueue.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: level2
 */

#include "Sh2BidOfferOrderQueue.h"
#include "../../public/program/Logger.h"

Sh2BidOfferOrderQueue::Sh2BidOfferOrderQueue() {
	// TODO Auto-generated constructor stub

}

Sh2BidOfferOrderQueue::~Sh2BidOfferOrderQueue() {
	// TODO Auto-generated destructor stub
}

bool Sh2BidOfferOrderQueue::sort(STOCKSNAP2* stocksnap2, ORDER order[][LEVEL_NUM], int32_t* noLevel, int imageStatus)
{
	switch(imageStatus)
	{
	case FULL_IMAGE:
		return sortFullImage(stocksnap2, order, noLevel);
	case UPDATED_IMAGE:
		return sortUpdatedImage(stocksnap2, order, noLevel);
	}
	return false;
}

bool Sh2BidOfferOrderQueue::sortFullImage(STOCKSNAP2* stocksnap2, ORDER order[][LEVEL_NUM], int32_t noLevel[])
{
	if (noLevel[0] > 10 || noLevel[1] > 10 || order[0][0].noOrders > 50 || order[1][0].noOrders > 50)
		return false;
	memset(stocksnap2->bid, 0, sizeof(LEVELS) * UPDATE_LEVEL_NUMBER);
	memset(stocksnap2->offer, 0, sizeof(LEVELS) * UPDATE_LEVEL_NUMBER);
	for (int i = 0; i < 2; ++ i)
	{
		for (int l = 0; l < noLevel[i]; ++ l)
		{
			switch(i)
			{
			case BID:
				stocksnap2->bid[l].price = order[i][l].price;
				stocksnap2->bid[l].orderQty = order[i][l].orderQty;
				stocksnap2->bid[l].numOrders = order[i][l].numOrders;
				if (l == 0)
				{
					for (int q = 0; q < order[i][l].noOrders; ++ q)
						stocksnap2->bid[l].order[q] = order[i][l].queue[q].orderQty;
				}
				break;
			case OFFER:
				stocksnap2->offer[l].price = order[i][l].price;
				stocksnap2->offer[l].orderQty = order[i][l].orderQty;
				stocksnap2->offer[l].numOrders = order[i][l].numOrders;
				if (l == 0)
				{
					for (int q = 0; q < order[i][l].noOrders; ++ q)
						stocksnap2->offer[l].order[q] = order[i][l].queue[q].orderQty;
				}
				break;
			default:
				ERROR("bid offer error!");
				return false;
			}
		}
	}
	return true;
}

bool Sh2BidOfferOrderQueue::sortUpdatedImage(STOCKSNAP2* stocksnap2, ORDER order[][LEVEL_NUM], int32_t noLevel[])
{
	if (noLevel[0] > LEVEL_NUM || noLevel[1] > LEVEL_NUM)
		return false;
	for (int i = 0; i < 2; ++ i)
	{
		for (int l = 0; l < noLevel[i]; ++ l)
		{
			switch(i)
			{
			case BID:
				switch(order[i][l].priceLevelOperator)
				{
				case ADD:
					if (!sortAddBidPrice(stocksnap2->bid, order[i][l]))
						return false;
					break;
				case UPDATE:
					if (!sortUpdatePrice(stocksnap2->bid, order[i][l]))
						return false;
					break;
				case DELETE:
					if (!sortDeletePrice(stocksnap2->bid, order[i][l]))
						return false;
					break;
				default:
					ERROR("bid price operation error!");
					return false;
				}
				break;
			case OFFER:
				switch(order[i][l].priceLevelOperator)
				{
				case ADD:
					if (!sortAddOfferPrice(stocksnap2->offer, order[i][l]))
						return false;
					break;
				case UPDATE:
					if (!sortUpdatePrice(stocksnap2->offer, order[i][l]))
						return false;
					break;
				case DELETE:
					if (!sortDeletePrice(stocksnap2->offer, order[i][l]))
						return false;
					break;
				default:
					ERROR("offer price operation error!");
					return false;
				}
				break;
			default:
				ERROR("bid offer error!");
				return false;
			}
		}
	}
	return true;
}

bool Sh2BidOfferOrderQueue::sortAddBidPrice(LEVELS* levels, const ORDER& order)
{
	int n = numLevel(levels);
	if (order.noOrders > Q_LENGTH || n == LEVEL_NUM)
		return false;
	LEVELS level;
	memset(&level, 0, sizeof(LEVELS));
	level.price = order.price;
	level.orderQty = order.orderQty;
	level.numOrders = order.numOrders;
	for (int i = 0; i < order.noOrders; ++ i)
		level.order[i] = order.queue[i].orderQty;
	if (n == 0)
	{
		memcpy(levels, &level, sizeof(LEVELS));
		return true;
	}
	for (int i = 0; i < n; ++ i)
	{
		if (levels[i].price < order.price)
		{
			memmove(levels + i + 1, levels + i, sizeof(LEVELS) * (n - i));
			memcpy(levels + i, &level, sizeof(LEVELS));
			return true;
		}
	}
	if ((levels + n - 1)->price > order.price)
	{
		memcpy(levels + n, &level, sizeof(LEVELS));
		return true;
	}
	return false;
}

bool Sh2BidOfferOrderQueue::sortAddOfferPrice(LEVELS* levels, const ORDER& order)
{
	int n = numLevel(levels);
	if (order.noOrders > Q_LENGTH || n == LEVEL_NUM)
		return false;
	LEVELS level;
	memset(&level, 0, sizeof(LEVELS));
	level.price = order.price;
	level.orderQty = order.orderQty;
	level.numOrders = order.numOrders;
	for (int i = 0; i < order.noOrders; ++ i)
		level.order[i] = order.queue[i].orderQty;
	if (n == 0)
	{
		memcpy(levels, &level, sizeof(LEVELS));
		return true;
	}
	for (int i = 0; i < n; ++ i)
	{
		if (levels[i].price > order.price)
		{
			memmove(levels + i + 1, levels + i, sizeof(LEVELS) * (n - i));
			memcpy(levels + i, &level, sizeof(LEVELS));
			return true;
		}
	}
	if ((levels + n - 1)->price < order.price)
	{
		memcpy(levels + n, &level, sizeof(LEVELS));
		return true;
	}
	return false;
}



bool Sh2BidOfferOrderQueue::sortUpdatePrice(LEVELS* levels, const ORDER& order)
{
	int n = numLevel(levels);
	if (order.noOrders > Q_LENGTH)
		return false;
	for (int i = 0; i < n; ++ i)
	{
		if (levels[i].price == order.price)
		{
			levels[i].orderQty = order.orderQty;
			levels[i].numOrders = order.numOrders;
			int ord = 0;
			for (int j = 0; j < Q_LENGTH; ++ j)
			{
				if (levels[i].order[j] != 0)
					++ ord;
				else
					break;
			}
			for (int j = 0; j < order.noOrders; ++ j)
			{
				switch(order.queue[j].orderQueueOperator)
				{
				case ADD:
					if (ord >= Q_LENGTH)
						return false;
					levels[i].order[ord ++] = order.queue[j].orderQty;
					break;
				case UPDATE:
					levels[i].order[order.queue[j].orderQueueOperatorEntryID] = order.queue[j].orderQty;
					break;
				case DELETE:
					levels[i].order[order.queue[j].orderQueueOperatorEntryID] = 0;
					break;
				default:
					ERROR("order queue operation error!");
					return false;
				}
			}
			//remove orders which are 0
			int64_t cpyord[Q_LENGTH];
			memcpy(cpyord, levels[i].order, ord * sizeof(int64_t));
			memset(levels[i].order, 0, ord * sizeof(int64_t));
			for (int j = 0, k = 0; j < ord; ++ j)
			{
				if (cpyord[j] != 0)
					levels[i].order[k ++] = cpyord[j];
			}
			return true;
		}
	}
	return false;
}

bool Sh2BidOfferOrderQueue::sortDeletePrice(LEVELS* levels, const ORDER& order)
{
	int n = numLevel(levels);
	if (order.price == 0)
		return true;
	for (int i = 0; i < n; ++ i)
	{
		//delete the price
		if (levels[i].price == order.price)
		{
			memmove(levels + i, levels + i + 1, sizeof(LEVELS) * (n - i - 1));
			memset(levels + n - 1, 0, sizeof(LEVELS));
			return true;
		}
	}
	//
	if (n == 0)
	{
		int num = 0;
		for (int i = 1; i < LEVEL_NUM; ++ i)
		{
			if (levels[i].price != 0)
				++ num;
			else
				break;
		}
		memmove(levels + 1, levels + 2, sizeof(LEVELS));
		memset(levels + num, 0, sizeof(LEVELS));
		return true;
	}
	return false;
}

int Sh2BidOfferOrderQueue::numLevel(LEVELS* levels)
{
	int n = 0;
	for (int i = 0; i < LEVEL_NUM; ++ i)
	{
		if (levels[i].price != 0)
			++ n;
		else
			break;
	}
	return n;
}
