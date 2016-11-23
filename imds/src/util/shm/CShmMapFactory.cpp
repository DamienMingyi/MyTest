/*
 * CShmMapFactory.cpp
 *
 *  Created on: 2016年10月12日
 *      Author: guoyue
 */

#include "CShmMapFactory.h"
#include "CShmMap.h"
#include "CChunkReader.h"
#include "CChunk.h"
#include "CStaticAlloctor.h"

CShmMapFactory::CShmMapFactory(IAlloctor *ptrAlloctor)
        : IShmObjFactory(ptrAlloctor)
{
    // TODO Auto-generated constructor stub

}

CShmMapFactory::~CShmMapFactory()
{
    // TODO Auto-generated destructor stub
}

shm_status_t CShmMapFactory::addObjToHt(const char* shmObjName,
        shm_offset_t value)
{
    CStaticAlloctor *ptrStaticAlloctor = (CStaticAlloctor *) ptrAlloctor;
    CChunk *ptrChunk = (CChunk *) ptrStaticAlloctor->getChunk();
    IHash *shmDictHt = ptrChunk->getDictHt();

    //对象加入到字典中
    shmDictHt->enter(shmObjName, value);
    return SUCCEED;
}

CShmObj* CShmMapFactory::createShmObj(const char* shmObjName,
        shm_size_t itemlen, shm_size_t capacity)
{
    CStaticAlloctor *ptrStaticAlloctor = (CStaticAlloctor *) ptrAlloctor;
    CChunk *ptrChunk = (CChunk *) ptrStaticAlloctor->getChunk();
    CChunkReader *ptrChunkReader = (CChunkReader *)ptrChunk->getReader();
    IHash *shmDictHt = ptrChunk->getDictHt();

    //2013-5-6 guoyue修改，传入参数检查
    if (shmObjName == NULL || itemlen == 0)
    {
        return NULL;
    }

    if (capacity > (shm_MAX_U32 >> 1))
    {
        return NULL;
    }

    shm_size_t mapInfoLen = itemlen + sizeof(ST_SHM_MAP_HEAD);

    ptrChunkReader->lock();
    //对象字典容量检查
    if (shmDictHt->getSize() <= shmDictHt->getUsed())
    {
        ptrChunkReader->unLock();
        return NULL;
    }

    //2014-6-11 guoyue修改。根据初始容量计算初始页面大小。
    shm_size_t pageSize = 0;
    if (capacity > SHM_PAGE_SIZE)
    {
        //大于缺省最大容量，取缺省最大容量
        pageSize = SHM_PAGE_SIZE;
        if ((capacity % pageSize) != 0)
        {
            capacity = ((capacity / pageSize) + 1) * pageSize;
        }
    }
    else if (capacity == 0)
    {
        //取缺省最大容量
        pageSize = SHM_PAGE_SIZE;
        capacity = pageSize;
    }
    else
    {
        //小于缺省最大容量，按输入取值
        pageSize = capacity;
    }

    //2013-5-6 guoyue修改。计算所需资源算法有误。
    shm_size_t pageCount = (capacity / pageSize) + 1;
    shm_size_t singlePage = (pageSize * mapInfoLen) + sizeof(ST_SHM_VECTOR_PAGE);
    shm_size_t memOccupied = sizeof(ST_SHM_VECTOR_PROPERTY) + (pageCount * singlePage);

    //没有足够资源分配，放弃
    if (ptrChunk->getChunkFree() < memOccupied)
    {
        ptrChunkReader->unLock();
        return NULL;
    }

    //2015-9-23 guoyue 对象属性表前移到共享内存环境管理区
    //初始化对象属性
    //ST_SHM_OBJ_PROPERTY *ptrObjProperty = (ST_SHM_OBJ_PROPERTY *)
    //        ptrStaticAlloctor->calloc(sizeof(ST_SHM_OBJ_PROPERTY));
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrChunk->getPtr(0);
    shm_u32_t nObjIndex = shmDictHt->getUsed();
    ST_SHM_OBJ_PROPERTY *ptrObjZone = (ST_SHM_OBJ_PROPERTY *) ptrENV->shmObjZone;
    ST_SHM_OBJ_PROPERTY *ptrObjProperty = &ptrObjZone[nObjIndex];
    //2015-9-23 guoyue end modify

    ptrObjProperty->shmObjType = SHM_OBJ_MAP;
    strncpy(ptrObjProperty->shmObjName, shmObjName,
            sizeof(ptrObjProperty->shmObjName) - 1);
    ptrObjProperty->used = 0;
    ptrObjProperty->occupied = 0;
    ptrObjProperty->size = 0; //容量==0

    //初始化vector属性
    ST_SHM_VECTOR_PROPERTY * vectorProperty =
            (ST_SHM_VECTOR_PROPERTY *) ptrStaticAlloctor->calloc(
                    sizeof(ST_SHM_VECTOR_PROPERTY));

    ptrObjProperty->shmObjOffset = ptrChunk->getOffset(vectorProperty);
    ptrObjProperty->occupied += sizeof(ST_SHM_VECTOR_PROPERTY);

    //vectorProperty->pageCount = (ptrObjProperty->size / SHM_PAGE_SIZE);
    vectorProperty->itemLen = mapInfoLen;
    vectorProperty->pageSize = pageSize;
    //逐页分配内存
    ST_SHM_VECTOR_PAGE *ptrLastPage = NULL;
    for (shm_u32_t i = 0; i < pageCount; i++)
    {
        //分配页内存
        ST_SHM_VECTOR_PAGE *ptrPage =
                (ST_SHM_VECTOR_PAGE *)ptrStaticAlloctor->calloc(singlePage);
        if (ptrPage)
        {
            //加入空闲节点页面
            ptrObjProperty->occupied += (singlePage);
            ptrObjProperty->size += vectorProperty->pageSize;
            vectorProperty->pageCount = i + 1;
            if (ptrLastPage == NULL)
            {
                vectorProperty->firstPage = ptrChunk->getOffset(ptrPage);
            }
            else
            {
                ptrLastPage->nextPage = ptrChunk->getOffset(ptrPage);
                ptrPage->priorPage = ptrChunk->getOffset(ptrLastPage);
            }
            ptrPage->pageId = i;
            ptrPage->thisPageItem = ptrChunk->getOffset(ptrPage)
                    + sizeof(ST_SHM_VECTOR_PAGE);
            ptrLastPage = ptrPage;
        }
        else
        {
            //未分配到足够内存
            break;
        }
    }

    vectorProperty->usedPageId = 0;
    vectorProperty->usedPageFree = vectorProperty->pageSize;

    //对象已初始化完毕,加到对象管理区
    shm_offset_t objPropertyOffset = ptrChunk->getOffset(ptrObjProperty);
    addObjToHt(shmObjName, objPropertyOffset);
    //shm_offset_t value =

    ptrChunkReader->unLock();
    //根据属性实例化容器
    CShmMap* shmObj = new CShmMap(ptrStaticAlloctor, ptrChunkReader,
            objPropertyOffset);
    return shmObj;
}

CShmObj* CShmMapFactory::openShmObj(const char* shmObjName)
{
    CStaticAlloctor *ptrStaticAlloctor = (CStaticAlloctor *) ptrAlloctor;
    CChunk *ptrChunk = (CChunk *) ptrStaticAlloctor->getChunk();
    CChunkReader *ptrChunkReader = (CChunkReader *) ptrChunk->getReader();
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
    if (SHM_OBJ_MAP != ptrObjProperty->shmObjType)
    {
        //名字能对上,但类型不对
        return NULL;
    }

    //实例化找到的对象返回
    CShmMap* shmObj = new CShmMap(ptrStaticAlloctor, ptrChunkReader,
            objPropertyOffset);
    return shmObj;
}

CShmObj* CShmMapFactory::getShmObj(const char* shmObjName,
        shm_size_t itemlen, shm_size_t capacity)
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
        if (SHM_OBJ_MAP != shmObj->getShmObjType())
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
