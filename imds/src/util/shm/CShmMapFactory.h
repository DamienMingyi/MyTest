/*
 * CShmMapFactory.h
 *
 *  Created on: 2016年10月12日
 *      Author: guoyue
 */

#ifndef CSHMMAPFACTORY_H_
#define CSHMMAPFACTORY_H_

#include "IShmObjFactory.h"

class CShmMapFactory: public IShmObjFactory
{
public:
    CShmMapFactory(IAlloctor *ptrAlloctor);
    virtual ~CShmMapFactory();

    virtual shm_status_t addObjToHt(const char* shmObjName, shm_offset_t value);
    virtual CShmObj* createShmObj(const char* shmObjName,
            shm_size_t itemlen = 0, shm_size_t capacity = 0);

    virtual CShmObj* openShmObj(const char* shmObjName);

    virtual CShmObj* getShmObj(const char* shmObjName, shm_size_t itemlen = 0,
            shm_size_t capacity = 0);
};

#endif /* CSHMMAPFACTORY_H_ */
