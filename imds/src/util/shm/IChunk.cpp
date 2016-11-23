/**
 * @file    IChunk.h
 * @brief   IChunk接口文件
 * @note	IChunk接口。
 *
 * Chunk表示预先分配好的某种资源。例如常规内存，共享内存，或者文件。
 * Chunk类型资源被创建后长度即固定。使用时实际是从Chunk中划出指定长度的未使用资源供消费者使用。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-4
 * @bug     无
 * @warning 无
 */

#include "IChunk.h"

/**
 *  IChunk构造函数
 */
IChunk::IChunk()
{
    // TODO Auto-generated constructor stub

}

/**
 *  IChunk析构函数
 */
IChunk::~IChunk()
{
    // TODO Auto-generated destructor stub
}

