/**
 * @file    CMutex.h
 * @brief   线程互斥类。
 * @note
 *
 * 线程互斥锁、范围锁实现, 不依赖boost库
 *
 * @author  guoyue
 * @version 1.0
 * @copyright
 * @date    2015-6-15
 * @bug     修订日期    修订人     修复的bug或者修订原因
 * @warning 无
 */
#ifndef UTIL_THREAD_CMUTEX_H_
#define UTIL_THREAD_CMUTEX_H_

#include <stdlib.h>
#include <pthread.h>

/**
 * @class CMutex CMutex.h "/dce/src/main/util/thread/CMutex.h"
 * @brief pthread互斥锁。
 * @note  基于pthread互斥锁。功能与CWnLock类似。
 * @remarks
 * @bug   修订日期    修订人     修复的bug或者修订原因
 * @warning
 */
class CMutex
{
public:
    /** @brief CMutex构造函数*/
    CMutex();
    /** @brief CMutex析构函数*/
    ~CMutex();
public:
    /** @brief 加锁*/
    void lock();
    /** @brief 解锁*/
    void unlock();
protected:
    pthread_mutex_t m_pLock;    ///<pthread_mutex_t互斥锁实例
};

/**
 * @class CScopedMutex CMutex.h "/dce/src/main/util/thread/CMutex.h"
 * @brief pthread范围锁。
 * @note  使用CMutex实现的scoped_lock。
 * @remarks
 * @bug   修订日期    修订人     修复的bug或者修订原因
 * @warning
 */
class CScopedMutex: public CMutex
{
public:
    /** @brief CScopedMutex构造函数*/
    CScopedMutex(CMutex& mutex);
    /** @brief CScopedMutex析构函数*/
    virtual ~CScopedMutex();
public:
    /** @brief 加锁*/
    virtual void lock();
    /** @brief 解锁*/
    virtual void unlock();
private:
    CMutex* m_pMutex;  ///<互斥锁引用对象
};

#endif /* UTIL_THREAD_CMUTEX_H_ */
