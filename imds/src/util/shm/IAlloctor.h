/**
 * @file    IAlloctor.h
 * @brief	IAlloctor分配器接口头文件
 * @note
 *
 * IAlloctor分配器类的接口类。分配器包括资源分配、资源释放、资源移除等操作。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */
#ifndef IALLOCTOR_H_
#define IALLOCTOR_H_

#include "CShmTypes.h"

/**
 * @class IAlloctor IAlloctor.h "\source\dce\src\main\util\shm\IAlloctor.h"
 * @brief IAlloctor分配器接口定义
 * @note 分配器(IAlloctor)用于提供一种资源管理机制。分配器资源管理策略分为几类：
 * - 资源静态分配策略(StaticAlloctor)\n
 * 分配器管理固定大小的资源。每当收到分配请求时，它就移动指针、以返回Chunk（“大块”）。同时还假定资源一旦被分配，
 * 就再也不会被释放。
 * - 资源动态分配策略(DynamicAlloctor)\n
 * 为c++ new和delete操作符提供包装的分配器。也就是在内部使用new和delete操作符，以满足动态资源请求。
 * - 资源缓存分配策略(CachedAlloctor)\n
 * 该分配器预先分配资源池，其中含有特定数目和大小的资源Chunk。这些Chunk在内部空闲表（free list）中进行维护，
 * 并在收到资源请求(malloc())时被返回。当应用调用free()时，Chunk被归还到内部空闲表、而不是OS中。
 *
 * @remarks 共享内存数据存取使用资源静态分配策略
 */
class IAlloctor
{
public:
    /** @brief IAlloctor构造函数*/
    IAlloctor();

    /** @brief IAlloctor析构函数*/
    virtual ~IAlloctor();

    /** @brief 分配nbytes字节资源*/
    virtual char *malloc(shm_size_t nbytes) = 0;

    /** @brief 分配nbytes字节资源, 分配后对资源初始化*/
    virtual char *calloc(shm_size_t nbytes, char initial_value = '\0') = 0;

    /** @brief 分配n_elem个元素, 每个元素长度为elem_size, 分配后对资源初始化*/
    virtual char *calloc(shm_size_t n_elem, shm_size_t elem_size,
            char initial_value = '\0') = 0;

    /** @brief 释放指针所指向的资源*/
    virtual void free(char *ptr) = 0;

    /** @brief 释放所有资源*/
    virtual shm_status_t realse(void) = 0;
};

#endif /* IALLOCTOR_H_ */
