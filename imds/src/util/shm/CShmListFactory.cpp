/**
 * @file    CShmListFactory.cpp
 * @brief   CShmListFactory共享内存对象工厂文件
 * @note
 *
 * CShmListFactory共享内存对象工厂实现对共享内存List对象的创建\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#include <time.h>

#include "CShmListFactory.h"
#include "CShmList.h"
#include "CChunkReader.h"
#include "CChunk.h"
#include "CStaticAlloctor.h"

/**
 *  CShmListFactory构造函数
 */
CShmListFactory::CShmListFactory(IAlloctor *ptrAlloctor)
        : IShmObjFactory(ptrAlloctor)
{
    // TODO Auto-generated constructor stub

}

/**
 *  CShmListFactory析构函数
 */
CShmListFactory::~CShmListFactory()
{
    // TODO Auto-generated destructor stub
}

shm_status_t CShmListFactory::addObjToHt(const char* shmObjName,
        shm_offset_t value)
{
    CStaticAlloctor *ptrStaticAlloctor = (CStaticAlloctor *) ptrAlloctor;
    CChunk *ptrChunk = (CChunk *) ptrStaticAlloctor->getChunk();
    IHash *shmDictHt = ptrChunk->getDictHt();
    //对象加入到字典中
    shmDictHt->enter(shmObjName, value);
    return SUCCEED;
}

/**
 * @bug 2013/6/4 guoyue 添加数据记录长度范围限制
 */
CShmObj* CShmListFactory::createShmObj(const char* shmObjName,
        shm_size_t itemlen, shm_size_t capacity)
{
    CStaticAlloctor *ptrStaticAlloctor = (CStaticAlloctor *) ptrAlloctor;
    CChunk *ptrChunk = (CChunk *) ptrStaticAlloctor->getChunk();
    CChunkReader *ptrChunkReader = (CChunkReader *)ptrChunk->getReader();
    IHash *shmDictHt = ptrChunk->getDictHt();
    //参数检查
    if (shmObjName == NULL)
    {
        return NULL;
    }
    
     ptrChunkReader->lock();
    //对象字典容量检查
    if (shmDictHt->getSize() <= shmDictHt->getUsed())
    {
    	ptrChunkReader->unLock();
        return NULL;
    }

    shm_size_t memOccupied = sizeof(ST_SHM_LIST_PROPERTY);

    //没有足够资源分配，放弃
    //@bug 2013/6/4 guoyue 添加数据记录长度范围限制,不够分配一个元素时也不创建容器
    //if (ptrShmChunk->getChunkFree() < memOccupied)
    if (ptrChunk->getChunkFree() < (memOccupied + itemlen))
    {
        ptrChunkReader->unLock();
        return NULL;
    }

    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrChunk->getPtr(0);
    shm_u32_t nObjIndex = shmDictHt->getUsed();
    ST_SHM_OBJ_PROPERTY *ptrObjZone = (ST_SHM_OBJ_PROPERTY *)ptrENV->shmObjZone;
    ST_SHM_OBJ_PROPERTY *ptrObjProperty = &ptrObjZone[nObjIndex];

    ptrObjProperty->shmObjType = SHM_OBJ_LIST;
    strncpy(ptrObjProperty->shmObjName, shmObjName,
            sizeof(ptrObjProperty->shmObjName) - 1);
    //list size
    ptrObjProperty->used = 0;
    //capacity
    ptrObjProperty->size = 0;

    //2015-9-23 guoyue 对象属性表前移到共享内存环境管理区
    //ptrObjProperty->occupied = sizeof(ST_SHM_OBJ_PROPERTY);
    ptrObjProperty->occupied = 0;
    //2015-9-23 guoyue end modify

    //初始化list属性
    ST_SHM_LIST_PROPERTY * listProperty =
            (ST_SHM_LIST_PROPERTY *) ptrStaticAlloctor->calloc(
                    sizeof(ST_SHM_LIST_PROPERTY));

    ptrObjProperty->shmObjOffset = ptrChunk->getOffset(listProperty);
    ptrObjProperty->occupied = ptrObjProperty->occupied
            + sizeof(ST_SHM_LIST_PROPERTY);

    //所有空槽置空,空槽是指放置空闲节点的槽.
    for (shm_size_t iSlot = 0; iSlot < SHM_SLOT_SIZE; iSlot++)
    {
        listProperty->shmSlotHt[iSlot] = 0;
    }
    listProperty->clearTime = time(NULL);
    listProperty->first_item_offset = 0;
    listProperty->last_item_offset = 0;

    //对象已初始化完毕,加到对象管理区
    shm_offset_t objPropertyOffset = ptrChunk->getOffset(ptrObjProperty);
    addObjToHt(shmObjName, objPropertyOffset);

    ptrChunkReader->unLock();
    //根据属性实例化容器
    CShmList* shmObj = new CShmList(ptrStaticAlloctor, ptrChunkReader,
            objPropertyOffset);
    return shmObj;
}

CShmObj* CShmListFactory::openShmObj(const char* shmObjName)
{
    CStaticAlloctor *ptrStaticAlloctor = (CStaticAlloctor *) ptrAlloctor;
    CChunk *ptrChunk = (CChunk *) ptrStaticAlloctor->getChunk();
    CChunkReader *ptrChunkReader = (CChunkReader *)ptrChunk->getReader();
    IHash *shmDictHt = ptrChunk->getDictHt();

    //参数检查
    if (shmObjName == NULL)
    {
        return NULL;
    }
    ptrChunkReader->lock();
    //共享内存管理区管理区查找对象
    shm_offset_t objPropertyOffset = shmDictHt->find(shmObjName);
    ptrChunkReader->unLock();

    if (0 == objPropertyOffset)
    {
        //没找到
        return NULL;
    }

    ST_SHM_OBJ_PROPERTY *ptrObjProperty =
            (ST_SHM_OBJ_PROPERTY *) ptrChunk->getPtr(objPropertyOffset);
    if (SHM_OBJ_LIST != ptrObjProperty->shmObjType)
    {
        //名字能对上,但类型不对
        return NULL;
    }

    //实例化找到的对象返回
    CShmList* shmObj = new CShmList(ptrStaticAlloctor, ptrChunkReader,
            objPropertyOffset);
    return shmObj;
}

CShmObj* CShmListFactory::getShmObj(const char* shmObjName, shm_size_t itemlen,
        shm_size_t capacity)
{
    //参数检查
    if (shmObjName == NULL)
    {
        return NULL;
    }

    //检查是否对象已存在
    CShmObj* shmObj = openShmObj(shmObjName);

    //对象已存在,返回对象
    if (shmObj != NULL)
    {
        if (SHM_OBJ_LIST != shmObj->getShmObjType())
        {
            return NULL;
        }
        return shmObj;
    }

    //参数检查,itemlen为0只打开已存在对象
    if (0 == itemlen)
    {
        return NULL;
    }

    //创建个新的对象
    shmObj = createShmObj(shmObjName, itemlen, capacity);
    return shmObj;
}
