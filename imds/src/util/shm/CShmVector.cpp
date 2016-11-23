/**
 * @file    CShmVector.cpp
 * @brief   CShmVector共享内存对象文件
 * @note
 *
 * CShmVector是Vector类型共享内存容器对象，对象数据记录长度在创建时固定。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     修订日期    修订人     修复的bug或者修订原因
 * - 2013/5/29 guoyue at方法添加数据记录下标范围限制
 * - 2014/6/10 guoyue push_back方法空间已满边界判断错误，应为大于等于
 * - 2014/6/10 guoyue push_back增加返回值
 * @warning 无
 */

#include "CShmVector.h"
#include "CChunk.h"
#include "CChunkReader.h"
#include "CStaticAlloctor.h"

/**
 * @param [in] ptrAlloctor 资源分配器
 * @param [in] ptrReader 读写器
 * @param [in] propertyOffset 内存对象公有属性偏移
 */
CShmVector::CShmVector(IAlloctor* ptrAlloctor, IReader *ptrReader,
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
    rebuildIndex();
    this->position = 0;
    this->ptrItem = NULL;
    this->clearTime = this->vectorProperty->clearTime;
}

/**
 * CShmVector析构函数
 */
CShmVector::~CShmVector()
{
    // TODO Auto-generated destructor stub
}

/**
 * @param [in] position 数据记录下标
 * @return char* 数据记录
 * @bug 2013/5/29 guoyue 添加数据记录下标范围限制
 */
char* CShmVector::at(shm_size_t i)
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
        if(!rebuildIndex())
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

/**
 * @return char* 数据记录
 */
char* CShmVector::front()
{
    if (this->getSize() == 0)
    {
        return NULL;
    }
    return at(0);
}

/**
 * @return char* 数据记录
 */
char* CShmVector::back()
{
    shm_size_t size = this->getSize();
    if (size == 0)
    {
        return NULL;
    }
    return at(size - 1);
}

/**
 * 清除Vector容器数据记录
 */
void CShmVector::clear()
{
    vectorProperty->usedPageId = 0;
    vectorProperty->usedPageFree = vectorProperty->pageSize;
    this->setSize(0);
    vectorProperty->clearTime = time(NULL);
}

/**
 * @return bool 容器是否为空
 * - true 容器为空
 * - false 容器不为空
 */
bool CShmVector::empty()
{
    if (this->getSize() > 0)
    {
        return false;
    }
    return true;
}

bool CShmVector::push()
{
    if (ptrItem)
    {
        this->getReader()->flush(ptrItem, getItemLength());
        return true;
    }
    return false;
}
/**
 * @param [in] item 数据记录
 */
bool CShmVector::push_back(char* item)
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
        if (!rebuildIndex())
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

/**
 * @return shm_size_t 数据记录长度
 */
shm_size_t CShmVector::getItemLength()
{
    if (vectorProperty)
    {
        return vectorProperty->itemLen;
    }
    return 0;
}

/**
 * @return bool 重建索引是否成功
 * - true 成功
 * - false 失败
 */
bool CShmVector::rebuildIndex()
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
