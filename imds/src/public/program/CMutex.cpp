/**
 * @file    CMutex.cpp
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
#include "CMutex.h"

/**
 * CMutex构造函数
 */
CMutex::CMutex()
{
    // TODO Auto-generated constructor stub
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_pLock, &attr);
    pthread_mutexattr_destroy(&attr);
}

/**
 * CMutex析构函数
 */
CMutex::~CMutex()
{
    // TODO Auto-generated destructor stub
    pthread_mutex_destroy(&m_pLock);
}

/**
 * @return 空.
 * @bug 修订日期    修订人     修复的bug或者修订原因
 */
void CMutex::lock()
{
    pthread_mutex_lock(&m_pLock);
}

/**
 * @return 空.
 * @bug 修订日期    修订人     修复的bug或者修订原因
 */
void CMutex::unlock()
{
    pthread_mutex_unlock(&m_pLock);
}

/**
 * CScopedMutex构造函数
 */
CScopedMutex::CScopedMutex(CMutex& mutex)
{
    m_pMutex = &mutex;
    m_pMutex->lock();
}

/**
 * CScopedMutex析构函数
 */
CScopedMutex::~CScopedMutex()
{
    m_pMutex->unlock();
}

/**
 * @return 空.
 * @bug 修订日期    修订人     修复的bug或者修订原因
 */
void CScopedMutex::lock()
{
    return m_pMutex->lock();
}

/**
 * @return 空.
 * @bug 修订日期    修订人     修复的bug或者修订原因
 */
void CScopedMutex::unlock()
{
    return m_pMutex->unlock();
}
