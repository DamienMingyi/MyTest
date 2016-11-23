/**
 * @file    CChunkReader.cpp
 * @brief   CChunkReader是Chunk类型读写器实现
 * @note
 *
 * CChunkReader是IReader实现类,实现了IReader创建、打开、关闭、移除以及读写接口。\n\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */

#include "CChunkReader.h"

/**
 *  CChunkReader构造函数
 */
CChunkReader::CChunkReader(IChunk* ptrChunk)
{
    // TODO Auto-generated constructor stub
    this->ptrChunk = ptrChunk;
}

/**
 *  CChunkReader析构函数
 */
CChunkReader::~CChunkReader()
{
    // TODO Auto-generated destructor stub
}

/**
 *
 *  @return IChunk*
 *  @note
 *  @remarks
 */
IChunk* CChunkReader::getChunk()
{
    return (this->ptrChunk);
}



