/**
 * @file    CShmHash.cpp
 * @brief   共享内存哈希表实现
 * @note
 *
 * CShmHash是共享内存哈希表及其操作实现。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */

#include "CShmHash.h"

/**
 *  CShmHash构造函数
 */
CShmHash::CShmHash(ST_SHM_HT *ptrShmDictHt)
{
    ptrHt = ptrShmDictHt;
}

/**
 *  CShmHash析构函数
 */
CShmHash::~CShmHash()
{
    // TODO Auto-generated destructor stub
}



/**
 *  @return shm_size_t 哈希表容量
 */
shm_u32_t CShmHash::getSize()
{
    if (ptrHt)
    {
        return ptrHt->size;
    }
    return 0;
}

/**
 *  @return shm_size_t 哈希表已用量
 */
shm_u32_t CShmHash::getUsed()
{
    if (ptrHt)
    {
        return ptrHt->used;
    }
    return 0;
}

/**
 *  @param [in] size 哈希表容量
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool CShmHash::create(shm_u32_t size)
{
    if (ptrHt)
    {
        //至少有一个节点
        ptrHt->size = (size | 1);
        //最多HASH_MAX_SIZE个节点
        if (ptrHt->size > HASH_MAX_SIZE)
        {
            ptrHt->size = HASH_MAX_SIZE;
        }
        ptrHt->used = 0;
        //初始化节点
        for (shm_u32_t i = 0; i < ptrHt->size; i++)
        {
            ptrHt->dictEntryIndex[i] = HASH_INVALID;
            memset(&ptrHt->dictEntry[i], 0, sizeof(ST_SHM_DICTENTRY));
            ptrHt->dictEntry[i].prev = HASH_INVALID;
            ptrHt->dictEntry[i].next = HASH_INVALID;
        }
        return true;
    }
    return false;
}

/**
 *  @param [in] key 哈希表key值
 *  @param [in] value 与key关联的数值
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool CShmHash::enter(const shm_u32_t key, shm_offset_t value)
{
    //参数检查
    if (!ptrHt)
    {
        return false;
    }

    //检查空间是否够加入新节点
    if ((ptrHt->used) > (ptrHt->size))
    {
        return false;
    }

    shm_s32_t index = ptrHt->used;
    //分配一个节点
    ST_SHM_DICTENTRY *dictEntry = &ptrHt->dictEntry[index];
    memcpy(dictEntry->key, &key, sizeof(key));
    dictEntry->value = value;
    shm_size_t hVal = hash(key);
    shm_s32_t oldIndex = ptrHt->dictEntryIndex[hVal];

    //链入表中
    dictEntry->prev = HASH_INVALID;
    dictEntry->next = oldIndex;

    if (oldIndex != HASH_INVALID)
    {
        ptrHt->dictEntry[oldIndex].prev = index;
    }

    ptrHt->dictEntryIndex[hVal] = index;
    ptrHt->used++;

    return true;
}

/**
 *  @param [in] key 哈希表key值
 *  @param [in] value 与key关联的数值
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool CShmHash::enter(const char *key, shm_offset_t value)
{
    //参数检查
    if (!ptrHt)
    {
        return false;
    }

    //检查空间是否够加入新节点
    if ((ptrHt->used) > (ptrHt->size))
    {
        return false;
    }

    shm_s32_t index = ptrHt->used;
    //allocating a node
    ST_SHM_DICTENTRY *dictEntry = &ptrHt->dictEntry[index];
    strncpy(dictEntry->key, key, sizeof(dictEntry->key) - 1);
    dictEntry->value = value;
    shm_size_t hVal = hash(dictEntry->key);
    shm_s32_t oldIndex = ptrHt->dictEntryIndex[hVal];

    dictEntry->prev = HASH_INVALID;
    dictEntry->next = oldIndex;

    if (oldIndex != HASH_INVALID)
    {
        ptrHt->dictEntry[oldIndex].prev = index;
    }

    ptrHt->dictEntryIndex[hVal] = index;
    ptrHt->used++;

    return true;
}

/**
 *  @param [in] key 哈希表key值
 *  @return shm_offset_t 与key关联的数值
 *  - 0 未找到
 *  - 大于0 查找结果
 */
shm_offset_t CShmHash::find(const char *key)
{
    if (!ptrHt)
    {
        return 0;
    }
    shm_size_t hVal = hash(key);
    shm_s32_t index = ptrHt->dictEntryIndex[hVal];
    if (HASH_INVALID == index)
    {
        return 0;
    }

    while (index != HASH_INVALID)
    {
        ST_SHM_DICTENTRY *dictEntry = &ptrHt->dictEntry[index];
        if (STREQ(key, dictEntry->key))
        {
            return dictEntry->value;
        }
        index = dictEntry->next;
    }
    return 0;
}

/**
 *  @param [in] key 哈希表key值
 *  @return shm_offset_t 与key关联的数值
 *  - 0 未找到
 *  - 大于0 查找结果
 */
shm_offset_t CShmHash::find(const shm_u32_t key)
{
    if (!ptrHt)
    {
        return 0;
    }
    shm_size_t hVal = hash(key);
    shm_s32_t index = ptrHt->dictEntryIndex[hVal];
    if (HASH_INVALID == index)
    {
        return 0;
    }

    while (index != HASH_INVALID)
    {
        ST_SHM_DICTENTRY *dictEntry = &ptrHt->dictEntry[index];
        shm_u32_t dictKey = 0;
        memcpy(&dictKey, dictEntry->key, sizeof(dictKey));
        if (key == dictKey)
        {
            return dictEntry->value;
        }
        index = dictEntry->next;
    }
    return 0;
}

/**
 *  @param [out] dictEntryList 哈希表所有数据列表
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool CShmHash::list(DICT_ENTRY_LIST &dictEntryList)
{
    dictEntryList.clear();
    if (!ptrHt)
    {
        return false;
    }

    if (!ptrHt->used)
    {
        return true;
    }

    for (shm_u32_t i = 0; i < ptrHt->size; i++)
    {
        shm_s32_t index = ptrHt->dictEntryIndex[i];
        while (index != HASH_INVALID)
        {
            ST_SHM_DICTENTRY dictEntry = ptrHt->dictEntry[index];
            dictEntryList.push_back(dictEntry);
            index = dictEntry.next;
        }
    }
    return true;
}
