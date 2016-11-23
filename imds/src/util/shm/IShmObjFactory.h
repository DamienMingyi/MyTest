/**
 * @file    IShmObjFactory.h
 * @brief   IShmObjFactory共享内存对象工厂接口头文件
 * @note
 *
 * IShmObjFactory共享内存对象工厂接口。接口包括创建、打开、取得共享内存对象的方法。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef ISHMOBJFACTORY_H_
#define ISHMOBJFACTORY_H_

#include "CShmObj.h"
#include "IAlloctor.h"

/**
 * @class IShmObjFactory IShmObjFactory.h "\source\dce\src\main\util\shm\IShmObjFactory.h"
 * @brief IShmObjFactory共享内存对象工厂接口
 * @note
 *  IShmObjFactory共享内存对象工厂接口。接口包括创建、打开、取得共享内存对象的方法。\n
 */
class IShmObjFactory
{
public:
    /** @brief IShmObjFactory构造函数*/
    IShmObjFactory(IAlloctor *ptrAlloctor);

    /** @brief IShmObjFactory析构函数*/
    virtual ~IShmObjFactory();

    /**
     * @brief 共享内存对象添加到内存对象管理区
     * @param [in] shmObjName 内存对象名
     * @param [in] value 内存对象偏移值
     * @return shm_status_t 操作结果
     * - SUCCEED 成功
     * - ERR_FAILED 失败
     */
    virtual shm_status_t addObjToHt(const char* shmObjName,
            shm_offset_t value) = 0;

    /**
     * @brief 创建共享内存对象
     * @param [in] shmObjName 内存对象名
     * @param [in] itemlen 对象每条数据长度
     * @param [in] capacity 内存对象初始容量
     * @return CShmObj* 共享内存对象指针
     * - NULL 创建失败
     * - 不为NULL 共享内存对象指针
     */
    virtual CShmObj* createShmObj(const char* shmObjName,
            shm_size_t itemlen = 0, shm_size_t capacity = 0) = 0;

    /**
     * @brief 打开共享内存对象
     * @param [in] shmObjName 内存对象名
     * @return CShmObj* 共享内存对象指针
     * - NULL 创建失败
     * - 不为NULL 共享内存对象指针
     */
    virtual CShmObj* openShmObj(const char* shmObjName) = 0;

    /**
     * @brief 取得共享内存对象
     * @param [in] shmObjName 内存对象名
     * @param [in] itemlen 对象每条数据长度
     * @param [in] capacity 内存对象初始容量
     * @return CShmObj* 共享内存对象指针
     * - NULL 创建失败
     * - 不为NULL 共享内存对象指针
     */
    virtual CShmObj* getShmObj(const char* shmObjName, shm_size_t itemlen = 0,
            shm_size_t capacity = 0) =0;
protected:
    IAlloctor* ptrAlloctor; ///<共享内存分配器(IAlloctor)
};

#endif /* ISHMOBJFACTORY_H_ */
