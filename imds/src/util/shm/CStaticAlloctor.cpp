/**
 * @file    CStaticAlloctor.cpp
 * @brief	CStaticAlloctor分配器文件
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

#include "CStaticAlloctor.h"

/**
 *  CStaticAlloctor构造函数
 */
CStaticAlloctor::CStaticAlloctor(IChunk *ptrChunk)
{
    // TODO Auto-generated constructor stub
    this->ptrChunk = ptrChunk;
}

/**
 *  CStaticAlloctor析构函数
 */
CStaticAlloctor::~CStaticAlloctor()
{
    // TODO Auto-generated destructor stub
}

/**
 *  取出CStaticAlloctor中保存的CChunkReader
 *  @return CChunkReader*
 *  @note
 *  @remarks
 */
IChunk* CStaticAlloctor::getChunk()
{
    return ptrChunk;
}
