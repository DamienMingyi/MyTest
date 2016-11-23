/**
 * @file    IChunk.h
 * @brief   IChunk接口头文件
 * @note	IChunk接口。
 *
 * Chunk表示预先分配好的某种资源。例如常规内存，共享内存，或者文件。
 * Chunk类型资源被创建后长度即固定。使用时实际是从Chunk中划出指定长度的未使用资源供消费者使用。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-4
 * @bug     无
 * @warning 无
 */
#ifndef ICHUNK_H_
#define ICHUNK_H_

#include "CShmTypes.h"

/**
 * @class IChunk IChunk.h "\source\dce\src\main\util\shm\IChunk.h"
 * @brief IChunk接口
 * @note  IChunk接口定义。
 * Chunk表示预先分配好的某种资源。例如常规内存，共享内存，或者文件。
 * Chunk类型资源被创建后长度即固定。使用时实际是从Chunk中划出指定长度的未使用资源供消费者使用。\n
 */
class IChunk
{
public:
    /** @brief IChunk构造函数*/
    IChunk();

    /** @brief IChunk析构函数*/
    virtual ~IChunk();

    /** @brief 根据传入的相对于基地址的偏移位置取得指针*/
    virtual void* getPtr(shm_offset_t offset) = 0;

    /** @brief 根据传入的指针计算相对于基地址的偏移位置*/
    virtual shm_offset_t getOffset(void* ptr) = 0;

    /** @brief 取出整个Chunk长度*/
    virtual shm_size_t getChunkSize() = 0;

    /** @brief 取出Chunk已使用长度*/
    virtual shm_size_t getChunkUsed() = 0;

    /** @brief 取出Chunk剩余可使用长度*/
    virtual shm_size_t getChunkFree() = 0;

    /** @brief 设置Chunk已使用长度*/
    virtual void setChunkUsed(shm_size_t chunkUsed) = 0;

    /** @brief 设置整个Chunk长度*/
    virtual void setChunkSize(shm_size_t chunkSize) = 0;

    /** @brief 设置keyId*/
    virtual void setKeyId(key_t keyId) = 0;
};

#endif /* ICHUNK_H_ */
