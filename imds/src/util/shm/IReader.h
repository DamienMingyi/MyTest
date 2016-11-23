/**
 * @file    IReader.h
 * @brief   IReader读写器接口头文件
 * @note
 *
 * IReader读写器接口。读写器接口包括创建、打开、关闭、移除、读、写操作。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */

#ifndef IREADER_H_
#define IREADER_H_

#include "CShmTypes.h"

/**
 * @class IReader IReader.h "\source\dce\src\main\util\shm\IReader.h"
 * @brief IReader读写器接口.
 * @note
 * 读写器包括创建、打开、关闭、移除、读、写操作。\n
 */
class IReader
{
public:
    /** @brief IReader构造函数*/
    IReader();

    /** @brief IReader析构函数*/
    virtual ~IReader();

    /** @brief 新建读写器*/
    virtual shm_status_t create(const char *targetName, shm_size_t maxSize) = 0;

    /** @brief 打开读写器*/
    virtual shm_status_t open(const char *targetName,
            shm_size_t maxSize = 0) = 0;

    /** @brief 关闭读写器*/
    virtual shm_status_t close() = 0;

    /** @brief 关闭读写器，移除读写器对应的资源。*/
    virtual shm_status_t remove() = 0;

    /** @brief 从读写器读出n个字节到应用缓存*/
    virtual shm_status_t read(void *buff, shm_size_t count,
            shm_offset_t offset) = 0;

    /** @brief 从应用缓存写n个字节到读写器*/
    virtual shm_status_t write(const void *buff, shm_size_t count,
            shm_offset_t offset) = 0;

    virtual void flush(void *address, size_t length) = 0;

    /** @brief 共享内存读写器加锁*/
    virtual void lock() = 0;

    /** @brief 共享内存读写器解锁*/
    virtual void unLock() = 0;
};

#endif /* IREADER_H_ */
