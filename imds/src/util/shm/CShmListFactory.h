/**
 * @file    CShmListFactory.h
 * @brief   CShmListFactory共享内存对象工厂头文件
 * @note
 *
 * CShmListFactory共享内存对象工厂实现对共享内存List对象的创建\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef CSHMLISTFACTORY_H_
#define CSHMLISTFACTORY_H_

#include "IShmObjFactory.h"

/**
 * @class CShmListFactory CShmListFactory.h "\source\dce\src\main\util\shm\CShmListFactory.h"
 * @brief CShmListFactory共享内存对象工厂实现
 * @note CShmListFactory共享内存对象工厂实现对共享内存List对象的创建
 */
class CShmListFactory: public IShmObjFactory
{
public:
    /** @brief CShmListFactory构造函数*/
    CShmListFactory(IAlloctor *ptrAlloctor);
    /** @brief CShmListFactory析构函数*/
    virtual ~CShmListFactory();

    virtual shm_status_t addObjToHt(const char* shmObjName, shm_offset_t value);
    virtual CShmObj* createShmObj(const char* shmObjName,
            shm_size_t itemlen = 0, shm_size_t capacity = 0);

    virtual CShmObj* openShmObj(const char* shmObjName);

    virtual CShmObj* getShmObj(const char* shmObjName, shm_size_t itemlen = 0,
            shm_size_t capacity = 0);
};

#endif /* CSHMLISTFACTORY_H_ */
