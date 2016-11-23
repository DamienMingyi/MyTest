/**
 * @file    CShmChunk.cpp
 * @brief   CShmChunk基于共享内存实现IChunk接口
 * @note
 *
 * CShmChunk是基于共享内存的IChunk实现类。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#include "CShmChunk.h"
#include "CShmHash.h"
#include "CShmChunkReader.h"
#include "CShmStaticAlloctor.h"

/**
 *  CShmChunk构造函数
 */
CShmChunk::CShmChunk()
{
    // TODO Auto-generated constructor stub
    ptrBase = NULL;
}

/**
 *  CShmChunk析构函数
 */
CShmChunk::~CShmChunk()
{
    // TODO Auto-generated destructor stub
    if (ptrAlloctor)
    {
        delete ptrAlloctor;
        ptrAlloctor = NULL;
    }

    if (ptrDictHt)
    {
        delete ptrDictHt;
        ptrDictHt = NULL;
    }

    if (ptrReader)
    {
        ptrReader->close();
        delete ptrReader;
        ptrReader = NULL;
    }
    ptrBase = NULL;
}

/**
 *  @param [in] offset 偏移量
 *  @return void* 共享内存基地址偏移offset字节后的指针\n
 *  @note 偏移量offset为0时,返回的是共享内存基地址
 *  @remarks
 *  - 如果共享内存基地址指针未初始化,返回NULL。
 *  - 如果偏移量超过Chunk长度,返回NULL。
 */
void* CShmChunk::getPtr(shm_offset_t offset)
{
    //参数检查
    if (!ptrBase || offset >= getChunkSize())
    {
        return NULL;
    }
    return ((void *) (ptrBase + offset));
}

/**
 *  @param [in] ptr 传入指针
 *  @return shm_offset_t 偏移位置\n
 *  @note
 *  @remarks
 *  - ptr为空,返回0。
 *  - 传入指针与共享内存基地址相同,返回0
 */
shm_offset_t CShmChunk::getOffset(void* ptr)
{
    //参数检查
    if (ptr)
    {
        return ((char *) ptr - ptrBase);
    }

    return 0;
}

/**
 *  @param [in] chunkUsed 已使用长度
 *  @note
 *  @remarks
 */
void CShmChunk::setChunkUsed(shm_size_t chunkUsed)
{
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrBase;

    //参数检查
    if (ptrENV)
    {
        (*(shm_size_t *) &(ptrENV->shmUsed)) = chunkUsed;
    }
}

/**
 *  @param [in] chunkSize 共享内存Chunk长度
 *  @note
 *  @remarks
 */
void CShmChunk::setChunkSize(shm_size_t chunkSize)
{
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrBase;

    //参数检查
    if (ptrENV)
    {
        (*(shm_size_t *) &(ptrENV->shmSize)) = chunkSize;
    }
}

/**
 *  @return 共享内存Chunk长度
 *  @note
 *  @remarks
 */
shm_size_t CShmChunk::getChunkSize()
{
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrBase;

    if (ptrENV)
    {
        return ptrENV->shmSize;
    }

    return 0;
}

/**
 *  @return 共享内存Chunk已使用长度
 *  @note
 *  @remarks
 */
shm_size_t CShmChunk::getChunkUsed()
{
    //参数检查
    if (ptrBase)
    {
        ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrBase;
        return ptrENV->shmUsed;
    }

    return 0;
}

/**
 *  @return 共享内存Chunk剩余可使用长度
 *  @note
 *  @remarks
 */
shm_size_t CShmChunk::getChunkFree()
{
    //参数检查
    if (ptrBase)
    {
        ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrBase;
        return (ptrENV->shmSize - ptrENV->shmUsed);
    }

    return 0;
}


void CShmChunk::setBasePtr(void *ptrShm)
{
    if (NULL == ptrShm)
    {
        ptrBase = NULL;
        return;
    }
    ptrBase = (char *) ptrShm;
}

IReader *CShmChunk::buildReader()
{
    if (ptrReader)
    {
        return ptrReader;
    }
    ptrReader = new CShmChunkReader(this);
    return ptrReader;
}

IHash *CShmChunk::buildChunk(shm_size_t shmSize)
{
    ST_SHM_ENV *ptrENV = buildEnv(shmSize);
    if (ptrENV == NULL)
    {
        return NULL;
    }

    //构造共享内存管理区共享内存对象字典操作对象
    CShmHash *shmDictHt = new CShmHash(&ptrENV->shmDictHt);

    if (shmDictHt == NULL)
    {
        return NULL;
    }

    //字典操作对象初始化
    shmDictHt->create();

    //使字典操作对象由Chunk管理
    this->ptrDictHt = shmDictHt;
    return (this->ptrDictHt);
}

IHash *CShmChunk::rebuildChunk()
{
    //字典操作对象由Chunk管理
    if(this->ptrDictHt)
    {
        return this->ptrDictHt;
    }

    //共享内存管理区处于共享内存起始位置
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) this->getPtr(0);

    if(ptrENV == NULL)
    {
        return NULL;
    }

    //共享内存环境管理区标识校验
    if(!checkIdentity())
    {
        return NULL;
    }
    //构造共享内存管理区共享内存对象字典操作对象
    this->ptrDictHt = new CShmHash(&ptrENV->shmDictHt);

    return (this->ptrDictHt);
}

IAlloctor *CShmChunk::buildAlloctor()
{
    if (this->ptrAlloctor)
    {
        return this->ptrAlloctor;
    }
    this->ptrAlloctor = new CShmStaticAlloctor(this);

    return (this->ptrAlloctor);
}
