/**
 * @file    CShmList.cpp
 * @brief   CShmList共享内存对象文件
 * @note
 *
 * CShmList是List类型共享内存容器对象，对象每条数据记录长度不预先固定。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     修订日期    修订人     修复的bug或者修订原因
 * - 2013-5-29 guoyue push_back方法添加数据记录长度范围限制
 * - 2014-4-11 guoyue 调整节点链入List时修改指针顺序。
 * - 2014-6-10 guoyue push_back增加返回值
 * - 2014-6-27 guoyue pop增加已弹出记录条数判断，避免push_back时节点入链表时非原子操作。
 * - 2014-7-8  guoyue 存在分配不成功的情况下累计List对象占用内存
 * @warning 无
 */
#include "CShmList.h"
#include "CChunk.h"
#include "CChunkReader.h"
#include "CStaticAlloctor.h"
#include <time.h>

/**
 *  CShmList构造函数
 */
CShmList::CShmList(IAlloctor* ptrAlloctor, IReader *ptrReader,
        shm_offset_t propertyOffset)
        : CShmObj(ptrAlloctor, ptrReader, propertyOffset)
{
    // TODO Auto-generated constructor stub
    CChunkReader *chunkReader = (CChunkReader *) ptrReader;
    CChunk *shmChunk = (CChunk *) chunkReader->getChunk();
    ST_SHM_OBJ_PROPERTY *ptrObjProperty = getObjProperty();
    this->listProperty = (ST_SHM_LIST_PROPERTY *) shmChunk->getPtr(
            ptrObjProperty->shmObjOffset);
    thisItem = NULL;
    lastPopItem = NULL;
    clearTime = this->listProperty->clearTime;
    //2014-6-27 guoyue popCount初始化
    popCount = 0;
    //2014-6-27 end modify
}

/**
 *  CShmList析构函数
 */
CShmList::~CShmList()
{
    // TODO Auto-generated destructor stub
}

/**
 * @param [out] item 输出数据记录
 * @param [out] itemLen 输出数据记录长度
 * @return bool 操作结果
 * - true 操作成功
 * - false 操作失败
 * @bug
 * - 2013-6-4 guoyue 返回false时,同时对item置空
 * - 2014-6-27 guoyue pop增加记录条数判断
 */
bool CShmList::pop(char* item, shm_size_t& itemLen)
{
    if (item == NULL)
    {
        itemLen = 0;
        return false;
    }

    if (empty())
    {
        itemLen = 0;
        lastPopItem = NULL;
        //2014-6-27 guoyue pop增加记录条数判断
        popCount = 0;
        //2014-6-27 end modify
        //2013-6-4 guoyue 返回false时,同时对item置空
        item[0] = 0;
        return false;
    }

    char* ptrValue = NULL;

    if (clearTime != listProperty->clearTime)
    {
        ptrValue = front(itemLen);
        lastPopItem = NULL;
    }

    //2014-6-27 guoyue pop增加记录条数判断
    if (popCount >= this->getSize())
    {
        itemLen = 0;
        item[0] = 0;
        return false;
    }
    //2014-6-27 end modify

    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    if (lastPopItem != NULL)
    {
        shm_offset_t next = lastPopItem->next;
        if (next != 0)
        {
            //2014-6-27 guoyue pop增加记录条数判断
            //lastPopItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(next);
            //itemLen = lastPopItem->itemLen;
            //ptrValue = &lastPopItem->value;
            //memcpy(item, ptrValue, itemLen);

            //保存lastPopItem偏移
            ST_SHM_LIST_ITEM *prePopItem = lastPopItem;
            lastPopItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(next);

            //判断偏移是否正确，不正确时恢复lastPopItem偏移
            if (lastPopItem->previous != ptrShmChunk->getOffset(prePopItem)
                    || lastPopItem->itemLen >= lastPopItem->slotLen
                    || lastPopItem->itemLen <= 0)
            {
                //恢复lastPopItem偏移
                lastPopItem = prePopItem;
                itemLen = 0;
                item[0] = 0;
                return false;
            }
            itemLen = lastPopItem->itemLen;
            ptrValue = &lastPopItem->value;
            memcpy(item, ptrValue, itemLen);

            popCount++;
            //2014-6-27 end modify
            return true;
        }
        itemLen = 0;
        //2013-6-4 guoyue 返回false时,同时对item置空
        item[0] = 0;
        return false;
    }

    lastPopItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
            listProperty->first_item_offset);
    itemLen = lastPopItem->itemLen;
    ptrValue = &lastPopItem->value;
    memcpy(item, ptrValue, itemLen);
    //2014-6-27 guoyue pop增加记录条数判断
    popCount++;
    //2014-6-27 end modify
    return true;
}

/**
 * @param [out] itemLen 输出数据记录长度
 * @return char* 第一条数据记录
 * - NULL 容器中无数据
 */
char* CShmList::front(shm_size_t& itemLen)
{
    if (empty())
    {
        itemLen = 0;
        thisItem = NULL;
        return NULL;
    }

    if (clearTime != listProperty->clearTime)
    {
        clearTime = listProperty->clearTime;
    }

    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    shm_offset_t first = listProperty->first_item_offset;
    if (first != 0)
    {
        thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(first);
        itemLen = thisItem->itemLen;
        return (&thisItem->value);
    }
    itemLen = 0;
    return NULL;
}

/**
 * @param [out] itemLen 输出数据记录长度
 * @return 
 * - char* 最后一条数据记录
 * - NULL 容器中无数据
 * @bug
 * - 2014-6-27 guoyue pop增加记录条数判断
 * - 2014-6-30 guoyue 增加判断，避免获取last_item_offset时正在修改
 */
char* CShmList::back(shm_size_t& itemLen)
{
    if (empty())
    {
        itemLen = 0;
        thisItem = NULL;
        return NULL;
    }

    //2014-6-30 guoyue 增加判断，只有一条数据时取front
    if (this->getSize() == 1)
    {
        return front(itemLen);
    }
    //2014-6-30 end modify

    if (clearTime != listProperty->clearTime)
    {
        clearTime = listProperty->clearTime;
    }

    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    //2014-6-30 guoyue 避免获取last_item_offset时正在修改
    //shm_offset_t last = listProperty->last_item_offset;
    //if (last != 0)
    //{
    //    thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(last);
    //    itemLen = thisItem->itemLen;
    //    return (&thisItem->value);
    //}
    //itemLen = 0;
    //return NULL;
    //大于一条数据时，判断取得的last_item_offset是否完整
    while (true)
    {
        shm_offset_t last = listProperty->last_item_offset;
        if (last != 0)
        {
            thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(last);

            ST_SHM_LIST_ITEM *preItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
                    thisItem->previous);
            //前一节点的下一节点应等于自己，否则表明取值有误。需要重取
            if (preItem->next != last)
            {
                continue;
            }
            itemLen = thisItem->itemLen;
            return (&thisItem->value);
        }
        itemLen = 0;
        return NULL;
    }
    //2014-6-30 end modify
}

/**
 * @note 移动到上一条记录，并取出数据。
 * @param [out] itemLen 输出数据记录长度
 * @return char* 上一条数据记录
 * - NULL 容器中无数据或当前数据是第一条数据
 */
char* CShmList::previous(shm_size_t& itemLen)
{
    if (empty())
    {
        itemLen = 0;
        thisItem = NULL;
        return NULL;
    }

    if (clearTime != listProperty->clearTime)
    {
        return front(itemLen);
    }

    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    if (thisItem != NULL)
    {
        shm_offset_t previous = thisItem->previous;
        if (previous != 0)
        {
            thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(previous);
            itemLen = thisItem->itemLen;
            return (&thisItem->value);
        }
        itemLen = 0;
        return NULL;
    }

    thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
            listProperty->first_item_offset);
    itemLen = thisItem->itemLen;
    return (&thisItem->value);
}

/**
 * @note 移动到下一条记录，并取出数据。
 * @param [out] itemLen 输出数据记录长度
 * @return char* 下一条数据记录
 * - NULL 容器中无数据或当前数据是最后一条数据
 */
char* CShmList::next(shm_size_t& itemLen)
{
    if (empty())
    {
        itemLen = 0;
        thisItem = NULL;
        return NULL;
    }

    if (clearTime != listProperty->clearTime)
    {
        return front(itemLen);
    }

    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    if (thisItem != NULL)
    {
        //2014-6-30 guoyue 增加判断，避免获取thisItem->next时正在修改
        if (ptrShmChunk->getOffset(thisItem) != listProperty->last_item_offset)
        {
            shm_offset_t next = thisItem->next;
            if (next != 0)
            {
                thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(next);
                itemLen = thisItem->itemLen;
                return (&thisItem->value);
            }
        }
        //2014-6-30 guoyue end modified
        itemLen = 0;
        return NULL;
    }

    thisItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
            listProperty->first_item_offset);
    itemLen = thisItem->itemLen;
    return (&thisItem->value);
}

/**
 * @return bool 容器是否为空
 * - true 容器为空
 * - false 容器不为空
 */
bool CShmList::empty()
{
    if (this->getSize() > 0)
    {
        return false;
    }
    return true;
}

/**
 * 清除List容器数据记录
 */
void CShmList::clear()
{
    //保存第一个节点偏移位置
    shm_offset_t thisOffset = listProperty->first_item_offset;

    //置空
    this->setSize(0);
    listProperty->first_item_offset = 0;
    listProperty->last_item_offset = 0;
    listProperty->clearTime = time(NULL);

    //无可用节点返回
    if (thisOffset == 0)
    {
        return;
    }

    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();
    ptrChunkReader->lock();

    //取出第一个节点
    ST_SHM_LIST_ITEM *ptrItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
            thisOffset);
    //从一个节点开始遍历.不是真正释放资源,而是逐个链到空闲空间表中
    while (ptrItem)
    {

        shm_offset_t next = ptrItem->next;

        ptrItem->next = 0;
        ptrItem->previous = 0;
        ptrItem->itemLen = 0;

        shm_u32_t slotLen = ptrItem->slotLen;
        shm_u32_t slotindex = 0;
        //计算该放到哪个空闲槽中
        slotLen = rad2nAlloc(slotLen, slotindex);
        slotindex--;

        if (listProperty->shmSlotHt[slotindex] == 0)
        {
            //空闲槽中还没有空闲节点,直接加入
            listProperty->shmSlotHt[slotindex] = thisOffset;
        }
        else
        {
            //空闲槽中已有空闲节点,链到头节点
            shm_offset_t nextFree = listProperty->shmSlotHt[slotindex];
            ST_SHM_LIST_ITEM *ptrNextItem =
                    (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(nextFree);
            ptrNextItem->previous = thisOffset;
            ptrItem->next = nextFree;
            listProperty->shmSlotHt[slotindex] = thisOffset;
        }

        //取下个节点
        thisOffset = next;
        if (thisOffset)
        {
            //还有下个节点
            ptrItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(thisOffset);
        }
        else
        {
            //已经是最后一个节点
            ptrItem = NULL;
        }
    }

    ptrChunkReader->unLock();
    return;
}

/**
 * @param [in] value 数据记录内容
 * @param [in] itemLen 数据记录内容长度
 * @return bool 添加数据是否成功
 * - true 成功
 * - false 失败
 * @bug 2013-5-29 guoyue 添加数据记录长度范围限制
 * @bug 2014-4-11 guoyue 调整节点链入List时修改指针顺序。
 * @bug 2014-6-10 guoyue push_back增加返回值
 * @bug 2014-7-8  guoyue 存在分配不成功的情况下累计List对象占用内存
 */
bool CShmList::push_back(char* value, shm_size_t itemLen)
{
    CChunkReader *ptrChunkReader = (CChunkReader *) this->getReader();
    CChunk *ptrShmChunk = (CChunk *) ptrChunkReader->getChunk();

    //guoyue 2013/5/29 添加数据记录长度范围限制
    //参数检查
    //if (value == NULL || itemLen == 0)
    if (value == NULL || itemLen == 0 || itemLen >= shm_MAX_U32)
    {
        //2014-6-10 guoyue push_back增加返回值
        //return;
        return false;
        //2014-6-10 end of modify
    }

    shm_u32_t slotindex = 0;
    //计算实际占用空间
    shm_u32_t slotLen = sizeof(ST_SHM_LIST_ITEM) + itemLen;
    //根据实际占用空间计算该在哪个空槽取空闲节点
    slotLen = rad2nAlloc(slotLen, slotindex);
    //空槽索引
    slotindex--;

    //空槽中若无可用空闲节点,先分配若干空闲节点入槽
    if (listProperty->shmSlotHt[slotindex] == 0)
    {
        shm_u32_t iSlotCount = slotCount(slotLen);
        CStaticAlloctor *ptrStaticAlloctor =
                (CStaticAlloctor *) this->getAlloctor();
        //检查内存是否还够分配
        if (ptrShmChunk->getChunkFree() < (iSlotCount * slotLen))
        {
            //2014-6-10 guoyue push_back增加返回值
            //return;
            return false;
            //2014-6-10 end of modify
        }
        ptrChunkReader->lock();
        char *ptrSlotAlloc = ptrStaticAlloctor->calloc(iSlotCount, slotLen,
                '\0');

        //2014-7-8 guoyue 可能存在分配不成功的情况
        //this->incOccupied(iSlotCount * slotLen);
        //2014-7-8 end of modify

        //分配的空闲节点初始化并链入空槽
        if (ptrSlotAlloc)
        {
            //2014-7-8 guoyue 分配成功才增加占用内存
            this->incOccupied(iSlotCount * slotLen);
            //2014-7-8 end of modify
            listProperty->shmSlotHt[slotindex] = ptrShmChunk->getOffset(
                    ptrSlotAlloc);
            ST_SHM_LIST_ITEM *ptrItem[iSlotCount];

            for (shm_u32_t i = 0; i < iSlotCount; i++)
            {
                ptrItem[i] =
                        (ST_SHM_LIST_ITEM *) (ptrSlotAlloc + (i * slotLen));
                ptrItem[i]->slotLen = slotLen;
            }
            for (shm_u32_t i = 0; i < iSlotCount; i++)
            {
                if (i == 0)
                {
                    ptrItem[i]->previous = 0;
                    ptrItem[i]->next = ptrShmChunk->getOffset(ptrItem[i + 1]);
                }
                else if ((i + 1) == iSlotCount)
                {
                    ptrItem[i]->previous = ptrShmChunk->getOffset(
                            ptrItem[i - 1]);
                    ptrItem[i]->next = 0;
                }
                else
                {
                    ptrItem[i]->previous = ptrShmChunk->getOffset(
                            ptrItem[i - 1]);
                    ptrItem[i]->next = ptrShmChunk->getOffset(ptrItem[i + 1]);
                }
            }
            shm_size_t capacity = this->getCapacity() + iSlotCount;
            this->setCapacity(capacity);
        }
        ptrChunkReader->unLock();
        if (!ptrSlotAlloc)
        {
            //2014-6-10 guoyue push_back增加返回值
            //return;
            return false;
            //2014-6-10 end of modify
        }
    }

    //空槽中取一个空闲节点
    shm_offset_t itemOffset = listProperty->shmSlotHt[slotindex];
    ST_SHM_LIST_ITEM *ptrListItem = (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
            itemOffset);
    listProperty->shmSlotHt[slotindex] = ptrListItem->next;
    if (ptrListItem->next != 0)
    {
        ST_SHM_LIST_ITEM *ptrListItemNext =
                (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(ptrListItem->next);
        ptrListItemNext->previous = 0;
    }

    //节点赋值
    //char *ptrValue = &ptrListItem->value;
    ptrListItem->itemLen = itemLen;
    //memcpy(ptrValue, value, itemLen);
    shm_offset_t valueOffset = ptrShmChunk->getOffset(&ptrListItem->value);
    ptrChunkReader->write(value, itemLen, valueOffset);

    //链入List
    if (listProperty->first_item_offset == 0)
    {
        //List还没有节点,first_item_offset与last_item_offset指向新入节点
        //guoyue 2014-4-11 调整节点链入List时修改指针顺序。
        //listProperty->first_item_offset = itemOffset;
        ptrListItem->previous = 0;
        ptrListItem->next = 0;
        //guoyue 2014-4-11 调整节点链入List时修改指针顺序。
        listProperty->first_item_offset = itemOffset;
        listProperty->last_item_offset = itemOffset;
    }
    else
    {
        //List已有节点,链入队尾
        ST_SHM_LIST_ITEM *ptrPreListItem =
                (ST_SHM_LIST_ITEM *) ptrShmChunk->getPtr(
                        listProperty->last_item_offset);
        //guoyue 2014-4-11 调整节点链入List时修改指针顺序。
        //ptrPreListItem->next = itemOffset;
        ptrListItem->previous = listProperty->last_item_offset;
        ptrListItem->next = 0;
        //guoyue 2014-4-11 调整节点链入List时修改指针顺序。
        ptrPreListItem->next = itemOffset;
        listProperty->last_item_offset = itemOffset;
    }
    //队列长度增加1
    this->incSize();
    //2014-6-10 guoyue push_back增加返回值
    //return;
    return true;
    //2014-6-10 end of modify
}

/**
 * @param [in] slotLen 节点长度
 * @param [in] slotAlloc 预置内存数量，以字节计算
 * @return char* 计算应分配的空闲节点数
 * @note slotCount计算预置内存数量最少可分配的节点数，slotLen乘上空闲节点数的数值接近slotAlloc。
 * @remarks 返回的空闲节点数至少为1
 */
shm_u32_t CShmList::slotCount(shm_u32_t slotLen, shm_u32_t slotAlloc)
{
    shm_u32_t iSlotCount = (shm_u32_t) ((slotAlloc / slotLen) | 1);
    return iSlotCount;
}

/**
 * @param [in] len 数据容器所需最小空间
 * @param [out] n 2^n中的n值，slotLen = 2^n 大于等于参数len
 * @return shm_u32_t 容纳数据所需的slotLen长度
 * @remarks List中ST_SHM_LIST_ITEM节点长度slotLen一定为2的n次幂。
 */
shm_u32_t CShmList::rad2nAlloc(shm_u32_t len, shm_u32_t &n)
{
    if (len <= 7)
    {
        n = 3;
        return 8;
    }

    n = 0;
    shm_u32_t value = 1;
    len--;

    while (len >= 1)
    {
        len = len >> 1;
        n++;
    }

    value = value << n;

    return value;
}
