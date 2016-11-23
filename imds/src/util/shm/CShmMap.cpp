/*
 * CShmMap.cpp
 *
 *  Created on: 2016年10月12日
 *      Author: guoyue
 */

#include "string.h"
#include "CShmMap.h"
#include "CChunk.h"
#include "CChunkReader.h"
#include "CStaticAlloctor.h"
#include <iostream>

CShmMap::CShmMap(IAlloctor* ptrAlloctor, IReader *ptrReader,
        shm_offset_t propertyOffset)
        : CShmObj(ptrAlloctor, ptrReader, propertyOffset)
{
    // TODO Auto-generated constructor stub
    CChunkReader *chunkReader = (CChunkReader *) ptrReader;
    CChunk *shmChunk = (CChunk *) chunkReader->getChunk();
    ST_SHM_OBJ_PROPERTY *ptrObjProperty = getObjProperty();
    this->vectorProperty = (ST_SHM_VECTOR_PROPERTY *) shmChunk->getPtr(
            ptrObjProperty->shmObjOffset);
    pageIndex.clear();
    rebuildPageIndex();
    this->position = 0;
    this->ptrItem = NULL;
    this->clearTime = this->vectorProperty->clearTime;
    this->ptrBuff = (char *) malloc(vectorProperty->itemLen);
}

CShmMap::~CShmMap()
{
    // TODO Auto-generated destructor stub
    if (this->ptrBuff)
    {
        free(this->ptrBuff);
        this->ptrBuff = NULL;
    }
}

/**
 * @param [in] position 数据记录下标
 * @return char* 数据记录
 * @bug 2013/5/29 guoyue 添加数据记录下标范围限制
 */
char* CShmMap::at(shm_size_t i)
{
    CChunkReader *chunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) chunkReader->getChunk();

    position = i;
    ptrItem = NULL;

    //guoyue 2013/5/29 添加数据记录下标范围限制
    if(position >= this->getSize())
    {
        return NULL;
    }

    //缺页
    if (position >= (pageIndex.size() * vectorProperty->pageSize))
    {
        if(!rebuildPageIndex())
        {
            return NULL;
        }
    }

    //position从0开始计数
    if (position < this->getSize())
    {
        //计算所在页面
        shm_u32_t pageId = (shm_u32_t) (position / vectorProperty->pageSize);
        ST_SHM_VECTOR_PAGE *page = pageIndex[pageId];
        //计算页面偏移位置
        shm_offset_t pageOffset = (shm_offset_t) (position
                % vectorProperty->pageSize) * vectorProperty->itemLen;
        //返回数据指针
        ptrItem = (char *) ptrShmChunk->getPtr(page->thisPageItem + pageOffset);
        return (ptrItem);
    }
    return NULL;
}

bool CShmMap::push()
{
    if (ptrValue)
    {
        this->getReader()->flush(ptrValue, getItemLength());
        return true;
    }
    return false;
}

bool CShmMap::enter(char* key, char* value)
{
    if (key == NULL || value == NULL)
    {
        return false;
    }

    char *ptrMapInfo = findMapInfo(key);

    //修改旧值
    ST_SHM_MAP_HEAD *ptrMapInfoHead = NULL;
    if (ptrMapInfo)
    {
        ptrMapInfoHead = (ST_SHM_MAP_HEAD *) ptrMapInfo;
        ptrMapInfoHead->isRemoved = 0;
        ptrValue = (char *) (ptrMapInfo + sizeof(ST_SHM_MAP_HEAD));
        memcpy(ptrValue, value, getItemLength());
        this->push();
    }
    else
    {
        if (ptrBuff == NULL)
        {
            ptrBuff = (char *) malloc(vectorProperty->itemLen);
        }
        if (ptrBuff == NULL)
        {
            return false;
        }
        memset(ptrBuff, 0, vectorProperty->itemLen);
        ptrMapInfoHead = (ST_SHM_MAP_HEAD *) ptrBuff;

        strncpy(ptrMapInfoHead->key, key, sizeof(ptrMapInfoHead->key) - 1);
        ptrMapInfoHead->isRemoved = 0;
        ptrValue = ptrBuff + sizeof(ST_SHM_MAP_HEAD);
        memcpy(ptrValue, value, getItemLength());
        push_back(ptrBuff);
    }
    return true;
}

/**
 * @param [in] item 数据记录
 */
bool CShmMap::push_back(char* item)
{
    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    //参数检查
    if (item == NULL)
    {
        return false;
    }

    if ((vectorProperty->usedPageId + 1) > pageIndex.size())
    {
        if (!rebuildPageIndex())
        {
            return false;
        }
    }

    ST_SHM_VECTOR_PAGE *page = pageIndex[vectorProperty->usedPageId];
    //thisPageFree等于0时重新分配页
    if (vectorProperty->usedPageFree == 0)
    {
        shm_u32_t thisPage = vectorProperty->usedPageId + 1;

        //容量够,只需分配新页
        if (thisPage < vectorProperty->pageCount)
        {
            vectorProperty->usedPageId++;
            vectorProperty->usedPageFree = vectorProperty->pageSize;
            page = pageIndex[vectorProperty->usedPageId];
        }
        else
        {
            shm_size_t singlePage = (vectorProperty->pageSize
                    * vectorProperty->itemLen) + sizeof(ST_SHM_VECTOR_PAGE);
            //分配新页内存
            CStaticAlloctor *ptrStaticAlloctor =
                    (CStaticAlloctor *) this->getAlloctor();
            if (ptrShmChunk->getChunkFree() < singlePage)
            {
                return false;
            }
            ptrChunkReader->lock();
            ST_SHM_VECTOR_PAGE *ptrPage =
                    (ST_SHM_VECTOR_PAGE *) ptrStaticAlloctor->calloc(
                            singlePage);
            //新页分配后修改对象属性
            if (ptrPage)
            {
                shm_offset_t dataPage = ptrShmChunk->getOffset(ptrPage);
                ptrPage->pageId = vectorProperty->usedPageId + 1;
                ptrPage->priorPage = ptrShmChunk->getOffset(page);
                ptrPage->nextPage = 0;
                ptrPage->thisPageItem = dataPage + sizeof(ST_SHM_VECTOR_PAGE);
                page->nextPage = dataPage;
                vectorProperty->usedPageFree = vectorProperty->pageSize;
                vectorProperty->pageCount++;
                vectorProperty->usedPageId = ptrPage->pageId;
                shm_size_t capacity = this->getCapacity()
                        + vectorProperty->pageSize;
                this->setCapacity(capacity);
                this->incOccupied(singlePage);
                pageIndex.push_back(ptrPage);
                page = ptrPage;
            }
            ptrChunkReader->unLock();

            if (!ptrPage)
            {
                return false;
            }
        }
    }

    //当前页取一条数据空闲空间存数据
    shm_size_t size = this->getSize();
    shm_offset_t pageOffset = (shm_offset_t) (size % vectorProperty->pageSize)
            * vectorProperty->itemLen;
    shm_offset_t offset = page->thisPageItem + pageOffset;
    ptrChunkReader->write(item, vectorProperty->itemLen, offset);
    //当前页可用空间减少1
    vectorProperty->usedPageFree--;
    ptrChunkReader->flush(vectorProperty, sizeof(ST_SHM_VECTOR_PROPERTY));
    //vector长度增加
    this->incSize();
    return true;
}

bool CShmMap::remove(char* key)
{
    char *ptrMapInfo = findMapInfo(key);
    if (ptrMapInfo)
    {
        ST_SHM_MAP_HEAD *ptrMapInfoHead = (ST_SHM_MAP_HEAD *) ptrMapInfo;
        if (!ptrMapInfoHead->isRemoved)
        {
            ptrMapInfoHead->isRemoved = 1;
            this->getReader()->flush(ptrMapInfo, sizeof(ST_SHM_MAP_HEAD));
            return true;
        }
    }
    return false;
}

/**
 * 清除容器数据记录
 */
void CShmMap::clear()
{
    vectorProperty->usedPageId = 0;
    vectorProperty->usedPageFree = vectorProperty->pageSize;
    this->setSize(0);
    vectorProperty->clearTime = time(NULL);
    mapIndex.clear();
}

char* CShmMap::find(char* key)
{
    ptrValue = NULL;
    char *ptrMapInfo = findMapInfo(key);
    if (ptrMapInfo)
    {
        ST_SHM_MAP_HEAD *ptrMapInfoHead = (ST_SHM_MAP_HEAD *) ptrMapInfo;
        if (!ptrMapInfoHead->isRemoved)
        {
            ptrValue = (char *) (ptrMapInfo + sizeof(ST_SHM_MAP_HEAD));
        }
    }
    return ptrValue;
}

char *CShmMap::findMapInfo(char *key)
{
    if (key == NULL)
    {
        return NULL;
    }

    if (clearTime != vectorProperty->clearTime)
    {
        clearTime = vectorProperty->clearTime;
        mapIndex.clear();
    }

    if (mapIndex.size() < this->getSize())
    {
        rebuildKeyValueMap();
    }

    char *ptrMapInfo = NULL;
    map<string, char*>::iterator iter;
    iter = mapIndex.find(key);
    if (iter != mapIndex.end())
    {
        ptrMapInfo = iter->second;
    }

    return ptrMapInfo;
}

shm_size_t CShmMap::size()
{
    return this->getSize();
}

char& CShmMap::operator[](char *key)
{
    return (*find(key));
}

/**
 * @return shm_size_t 数据记录长度
 */
shm_size_t CShmMap::getItemLength()
{
    if (vectorProperty)
    {
        return (vectorProperty->itemLen - sizeof(ST_SHM_MAP_HEAD));
    }
    return 0;
}

bool CShmMap::rebuildKeyValueMap()
{
    if (this->getSize() == 0)
    {
        if (!mapIndex.empty())
        {
            mapIndex.clear();
        }
        return true;
    }

    //shm_size_t mapTableSize = mapIndex.size();//this->getSize();
    for (shm_size_t i = mapIndex.size(); i < this->getSize(); i++)
    {
        char *ptrMapInfo = this->at(i);
        if (ptrMapInfo == NULL)
        {
            return false;
        }
        ST_SHM_MAP_HEAD *ptrMapInfoHead = (ST_SHM_MAP_HEAD *)ptrMapInfo;
        mapIndex[ptrMapInfoHead->key] = ptrMapInfo;
    }
    return true;
}

/**
 * @return bool 重建索引是否成功
 * - true 成功
 * - false 失败
 */
bool CShmMap::rebuildPageIndex()
{
    ST_SHM_VECTOR_PAGE *page = NULL;
    CChunkReader *chunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) chunkReader->getChunk();
    shm_size_t pageCount = pageIndex.size();

    //不需要重建索引
    if (pageCount == vectorProperty->pageCount)
    {
        return true;
    }

    //首次建索引，先载入第一页
    if (pageCount == 0)
    {
        if(vectorProperty->firstPage == 0)
        {
            return false;
        }
        page = (ST_SHM_VECTOR_PAGE *)ptrShmChunk->getPtr(vectorProperty->firstPage);
        pageIndex.push_back(page);
        pageCount++;
    }

    //载入所有页
    if(pageCount < vectorProperty->pageCount)
    {
        chunkReader->lock();
        //从pageIndex中取出最后一页
        page = pageIndex[pageCount-1];
        //逐页载入索引
        while(page->nextPage)
        {
            ST_SHM_VECTOR_PAGE *nextPage = NULL;
            nextPage = (ST_SHM_VECTOR_PAGE *)ptrShmChunk->getPtr(page->nextPage);
            pageIndex.push_back(nextPage);
            page = nextPage;
        }
        chunkReader->unLock();
    }

    //建索引发生错误
    if(pageIndex.size() > vectorProperty->pageCount)
    {
        return false;
    }
    return true;
}
