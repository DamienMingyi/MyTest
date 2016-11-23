/**
 * @file    CShmHash.h
 * @brief   共享内存哈希表头文件
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
#ifndef CSHMHASH_H_
#define CSHMHASH_H_

#include "IHash.h"

/**
 * @class CShmHash CShmHash.h "\source\dce\src\main\util\shm\CShmHash.h"
 * @brief CShmHash共享内存哈希表操作实现。
 * @note
 * @remarks
 */
class CShmHash: public IHash
{
public:
    /** @brief CShmHash构造函数*/
    CShmHash(ST_SHM_HT *ptrShmDictHt);

    /** @brief CShmHash析构函数*/
    virtual ~CShmHash();
public:

    /** @brief 初始化哈希表*/
    bool create(shm_u32_t size = HASH_MAX_SIZE);

    /** @brief 向哈希表添加数据*/
    bool enter(const char *key, shm_offset_t value);

    /** @brief 向哈希表添加数据*/
    bool enter(const shm_u32_t key, shm_offset_t value);

    /** @brief 从哈希表查找数据*/
    shm_offset_t find(const char *key);

    /** @brief 从哈希表查找数据*/
    shm_offset_t find(const shm_u32_t key);

    /** @brief 列出哈希表数据*/
    bool list(DICT_ENTRY_LIST &dictEntryList);

    /** @brief 取出哈希表容量*/
    shm_u32_t getSize();

    /** @brief 取出哈希表已使用数量*/
    shm_u32_t getUsed();

private:
    ST_SHM_HT *ptrHt; ///<哈希表
};

#endif /* CSHMHASH_H_ */
