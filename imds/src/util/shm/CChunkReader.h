/**
 * @file    CChunkReader.h
 * @brief   Chunk类型读写器头文件
 * @note
 *
 * CChunkReader是IReader实现类,实现了IReader创建、打开、关闭、移除以及读写接口。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */

#ifndef CCHUNKREADER_H_
#define CCHUNKREADER_H_

#include "IReader.h"
#include "IChunk.h"

/**
 * @class CChunkReader CChunkReader.h "\source\dce\src\main\util\shm\CChunkReader.h"
 * @brief CChunkReader读写器.
 * @note
 * CChunkReader通过create方法新建一个资源长度固定的Chunk，在Chunk范围内执行读写操作。\n
 */
class CChunkReader: public IReader
{
public:
    /** @brief CChunkReader构造函数*/
    CChunkReader(IChunk* ptrChunk);

    /** @brief CChunkReader析构函数*/
    virtual ~CChunkReader();

    /** @brief 取出CChunkReader中保存的IChunk*/
    IChunk* getChunk();

private:
    IChunk *ptrChunk;	///<IChunk对象
};

#endif /* CCHUNKREADER_H_ */
