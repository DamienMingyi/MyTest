/**
 * @file    CShmVectorFactory.h
 * @brief   CShmVectorFactory共享内存对象工厂头文件
 * @note
 *
 * CShmVectorFactory共享内存对象工厂实现对共享内存Vector对象的创建\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug
 * 2013-5-6 guoyue createShmObj方法分配资源前对缺省容量需要资源检查算法有误，只检查了是否够分配一页。
 * @warning 无
 */
#ifndef CSHMVECTORFACTORY_H_
#define CSHMVECTORFACTORY_H_

#include "IShmObjFactory.h"

/**
 * @class CShmVectorFactory CShmVectorFactory.h "\source\dce\src\main\util\shm\CShmVectorFactory.h"
 * @brief CShmVectorFactory共享内存对象工厂实现
 * @note CShmVectorFactory共享内存对象工厂实现对共享内存Vector对象的创建
 */
class CShmVectorFactory: public IShmObjFactory
{
public:
    /** @brief CShmVectorFactory构造函数*/
    CShmVectorFactory(IAlloctor *ptrAlloctor);
    /** @brief CShmVectorFactory析构函数*/
    virtual ~CShmVectorFactory();

    virtual shm_status_t addObjToHt(const char* shmObjName, shm_offset_t value);
    virtual CShmObj* createShmObj(const char* shmObjName,
            shm_size_t itemlen = 0, shm_size_t capacity = 0);

    virtual CShmObj* openShmObj(const char* shmObjName);

    virtual CShmObj* getShmObj(const char* shmObjName, shm_size_t itemlen = 0,
            shm_size_t capacity = 0);
};

#endif /* CSHMVECTORFACTORY_H_ */
