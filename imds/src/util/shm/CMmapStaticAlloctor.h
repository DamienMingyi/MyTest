/**
 * @file    CMmapStaticAlloctor.h
 * @brief	CMmapStaticAlloctor分配器头文件
 * @note
 *
 * 使用静态分配策略的共享内存分配器.
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */
#ifndef CMMAPSTATICALLOCTOR_H_
#define CMMAPSTATICALLOCTOR_H_

#include "CStaticAlloctor.h"

/**
 * @class CMmapStaticAlloctor CMmapStaticAlloctor.h "\source\dce\src\main\util\shm\CMmapStaticAlloctor.h"
 * @brief 使用静态分配策略的共享内存分配器实现
 * @note CMmapStaticAlloctor以共享内存作为分配资源
 */
class CMmapStaticAlloctor: public CStaticAlloctor
{
public:
    /**
     * @brief CMmapStaticAlloctor构造函数
     * @param [in] ptrChunkReader 分配器使用的读写器
     */
    CMmapStaticAlloctor(IChunk *ptrChunk);

    /** @brief CMmapStaticAlloctor析构函数*/
    virtual ~CMmapStaticAlloctor();

    /**
     * @brief 分配nbytes字节共享内存
     * @param [in] nbytes 分配nbytes字节数
     * @return char* 分配的共享内存
     */
    virtual char *malloc(shm_size_t nbytes);

    /**
     * @brief 分配nbytes字节共享内存, 分配后对资源初始化
     * @param [in] nbytes 分配nbytes字节数
     * @param [in] initial_value 初始化值
     * @return char* 分配的共享内存
     */
    virtual char *calloc(shm_size_t nbytes, char initial_value = '\0');

    /**
     * @brief 分配n_elem个元素, 每个元素长度为elem_size, 对分配共享内存初始化
     * @param [in] n_elem 分配n_elem个元素
     * @param [in] elem_size 每个元素长度
     * @param [in] initial_value 初始化值
     * @return char* 分配的共享内存
     */
    virtual char *calloc(shm_size_t n_elem, shm_size_t elem_size,
            char initial_value = '\0');

    /** @brief 释放ptr指针所指向的资源,对共享内存不实现*/
    virtual void free(char *ptr);

    /** @brief 释放所有资源,对共享内存不实现*/
    virtual shm_status_t realse(void);
};

#endif /* CMMAPSTATICALLOCTOR_H_ */
