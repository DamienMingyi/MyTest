/**
 * @file    CStaticAlloctor.h
 * @brief	CStaticAlloctor分配器头文件
 * @note
 *
 * IAlloctor分配器类的静态分配策略实现.\n
 * 分配器管理固定大小的资源。每当收到分配请求时，它就移动指针、以返回Chunk。
 * 同时还假定资源一旦被分配，就再也不会被释放。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */
#ifndef CSTATICALLOCTOR_H_
#define CSTATICALLOCTOR_H_

#include "IAlloctor.h"
#include "IChunk.h"

/**
 * @class CStaticAlloctor CStaticAlloctor.h "\source\dce\src\main\util\shm\CStaticAlloctor.h"
 * @brief IAlloctor分配器类的静态分配策略实现
 * @note
 *
 * 分配器管理固定大小的资源。每当收到分配请求时，它就移动指针、以返回Chunk（“大块”）。
 * 同时还假定资源一旦被分配，就再也不会被释放。\n
 * 分配器使用ChunkReader读写器实现对Chunk的操作.
 *
 * @remarks 资源可以是常规内存，共享内存，或者文件。共享内存数据存取使用共享内存作为资源.
 */
class CStaticAlloctor: public IAlloctor
{
public:
    /** @brief CStaticAlloctor构造函数*/
    CStaticAlloctor(IChunk *ptrChunk);

    /** @brief CStaticAlloctor析构函数*/
    virtual ~CStaticAlloctor();

    /** @brief 取得CChunkReader读写器*/
    IChunk* getChunk();

private:
    IChunk *ptrChunk; ///<CChunkReader读写器
};

#endif /* CSTATICALLOCTOR_H_ */
