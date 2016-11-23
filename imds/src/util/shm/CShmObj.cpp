/**
 * @file    CShmObj.cpp
 * @brief   CShmObj内存对象文件
 * @note
 *
 * CShmObj内存对象主要用于维护ST_SHM_OBJ_PROPERTY表。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug
 * - 2014-6-30 guoyue incSize自加原子操作
 * - 2014-9-22 guoyue 32BIT incSize 自加原子操作
 * @warning 无
 */

#include "CShmObj.h"
#include "CChunk.h"
#include "CChunkReader.h"

/**
 * CShmObj构造函数
 * @param [in] ptrAlloctor 资源分配器
 * @param [in] ptrReader 读写器
 * @param [in] propertyOffset 内存对象公有属性偏移
 */
CShmObj::CShmObj(IAlloctor* ptrAlloctor, IReader *ptrReader,
        shm_offset_t propertyOffset)
{
    // TODO Auto-generated constructor stub
    this->ptrAlloctor = ptrAlloctor;
    this->ptrReader = ptrReader;
    this->objPropertyOffset = propertyOffset;

    CChunkReader *chunkReader = (CChunkReader *) ptrReader;
    CChunk *shmChunk = (CChunk *) chunkReader->getChunk();
    this->ptrObjProperty = (ST_SHM_OBJ_PROPERTY *) shmChunk->getPtr(
            propertyOffset);
}

/**
 *  CShmObj析构函数
 */
CShmObj::~CShmObj()
{
    // TODO Auto-generated destructor stub
}
/**
 * @return IAlloctor* 分配器
 */
IAlloctor* CShmObj::getAlloctor()
{
    return ptrAlloctor;
}
/**
 * @return IReader* 读写器
 */
IReader* CShmObj::getReader()
{
    return ptrReader;
}

/**
 * @return ST_SHM_OBJ_PROPERTY * 内存对象公用属性
 */
ST_SHM_OBJ_PROPERTY *CShmObj::getObjProperty()
{
    return ptrObjProperty;
}

/**
 * @return shm_size_t 内存对象保存数据条数
 */
shm_size_t CShmObj::getSize() const
{
    if (ptrObjProperty)
    {
        return ptrObjProperty->used;
    }
    return 0;
}

/**
 * @param [in] size 内存对象已使用量
 */
void CShmObj::setSize(shm_size_t size)
{
    if (ptrObjProperty)
    {
        (*(shm_size_t *) &(ptrObjProperty->used)) = size;
    }
}

/**
 * @remarks (*(unsigned long *)&jiffiers)++将volatile的jiffiers转换成一般的内存变量，
 * 避免了用寄存器做cache，从而保证了jiffiers加一操作的原子性。\n
 * Linux一开始也是直接用jiffiers++，到后来的版本才改成(*(unsigned long *)&jiffiers)++。
 * @bug
 * - 2014-6-30 guoyue 添加自加原子操作
 */
void CShmObj::incSize()
{
#ifdef SHM_NOT_ATOMIC
    if (ptrObjProperty)
    {
        (*(shm_size_t *) &(ptrObjProperty->used))++;
    }
#else
#ifdef SHM_32BIT
    //2014-9-22 guoyue 32BIT自加原子操作
    asm volatile("lock;" "incl %0"
             : "+m" (ptrObjProperty->used));
    //2014-9-22 end modify
#else
    //2014-6-30 guoyue 自加原子操作
    asm volatile("lock;" "incq %0"
                 : "=m" (ptrObjProperty->used)
                 : "m" (ptrObjProperty->used));
    //2014-6-30 end modify
#endif
#endif
    this->ptrReader->flush(ptrObjProperty, sizeof(ST_SHM_OBJ_PROPERTY));
}

/**
 * @param [in] occupy 内存占用字节数增加occupy字节
 */
void CShmObj::incOccupied(shm_size_t occupy)
{
    if (ptrObjProperty)
    {
        ptrObjProperty->occupied += occupy;
    }
}

/**
 * @return shm_size_t 内存对象容量
 */
shm_size_t CShmObj::getCapacity() const
{
    if (ptrObjProperty)
    {
        return ptrObjProperty->size;
    }
    return 0;
}

/**
 * @return shm_size_t 内存对象占用字节数
 */
shm_size_t CShmObj::getOccupied() const
{
    if (ptrObjProperty)
    {
        return ptrObjProperty->occupied;
    }
    return 0;
}

/**
 * @param [in] capacity 内存对象容量
 */
void CShmObj::setCapacity(shm_size_t capacity)
{
    if (ptrObjProperty)
    {
        (*(shm_size_t *) &(ptrObjProperty->size)) = capacity;
    }
}

/**
 * @return char*  内存对象名称
 * - 字串 内存对象名称
 * - NULL 失败
 */
const char* CShmObj::getShmObjName() const
{
    if (ptrObjProperty)
    {
        return ptrObjProperty->shmObjName;
    }
    return NULL;
}

/**
 * @return shm_u32_t 内存对象类型
 * - 0 SHM_OBJ_NULL
 * - 1 SHM_OBJ_VECTOR
 * - 2 SHM_OBJ_LIST
 */
shm_u32_t CShmObj::getShmObjType()
{
    if (ptrObjProperty)
    {
        return ptrObjProperty->shmObjType;
    }
    return SHM_OBJ_NULL;
}
