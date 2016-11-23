/**
 * @file    IHash.h
 * @brief   共享内存类型IHash接口头文件
 * @note
 *
 * 定义了：哈希表的通用结构体\n
 * IHash 主要实现了哈希表的常见操作\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-18
 * @bug     无
 * @warning 无
 */

#ifndef IHASH_H_
#define IHASH_H_

#include <vector>
#include "CShmTypes.h"

/** @name 共享内存哈希表缺省属性值
 *  @{
 */
/** 哈希表最大长度,是个质数 */
#define HASH_MAX_SIZE   (40009)//(9973)
/** 哈希表KEY值最大长度 */
#define HASH_KEY_SIZE   (32)
/** 哈希表无效入口值 */
#define HASH_INVALID    (-1)
/** @}*/ // 共享内存哈希表缺省属性值
using namespace std;

#pragma pack(push,8)

/**
 * @brief 结构体ST_SHM_DICTENTRY类型定义
 * @note  哈希表一条数据的信息
 */
typedef struct st_shm_dictEntry
{
    char key[HASH_KEY_SIZE];    ///<查找key
    shm_offset_t value;         ///<与key关联的数值
    shm_s32_t next;             ///<下一条冲突Hash(key)入口
    shm_s32_t prev;             ///<上一条冲突Hash(key)入口
} ST_SHM_DICTENTRY;

/**
 * @brief 结构体ST_SHM_HT类型定义
 * @note  哈希表定义
 */
typedef struct st_shm_ht
{
    shm_u32_t size;             ///<哈希表容量
    shm_u32_t used;             ///<哈希表已使用数量
    shm_s32_t dictEntryIndex[HASH_MAX_SIZE]; ///<入口索引, 指向dictEntry
    ST_SHM_DICTENTRY dictEntry[HASH_MAX_SIZE]; ///<哈希表数据
} ST_SHM_HT;
#pragma pack(pop)

/** 定义类型DICT_ENTRY_LIST*/
typedef vector<ST_SHM_DICTENTRY> DICT_ENTRY_LIST;
/**
 * @class IHash IHash.h "\source\dce\src\main\util\shm\IHash.h"
 * @brief IHash.
 * @note
 * IHash主要实现了哈希表的常用操作\n
 */
class IHash
{
public:
    IHash();
    virtual ~IHash();

public:

    /** @brief 初始化哈希表*/
    virtual bool create(shm_u32_t size = HASH_MAX_SIZE) = 0;

    /** @brief 向哈希表添加数据*/
    virtual bool enter(const char *key, shm_offset_t value) = 0;

    /** @brief 向哈希表添加数据*/
    virtual bool enter(const shm_u32_t key, shm_offset_t value) = 0;

    /** @brief 从哈希表查找数据*/
    virtual shm_offset_t find(const char *key) = 0;

    /** @brief 从哈希表查找数据*/
    virtual shm_offset_t find(const shm_u32_t key) = 0;

    /** @brief 列出哈希表数据*/
    virtual bool list(DICT_ENTRY_LIST &dictEntryList) = 0;

    /** @brief 取出哈希表容量*/
    virtual shm_u32_t getSize() = 0;

    /** @brief 取出哈希表已使用数量*/
    virtual shm_u32_t getUsed() = 0;

protected:
    /** @brief 计算哈希值*/
    shm_size_t hash(const char* key);

    /** @brief 计算哈希值*/
    shm_size_t hash(const shm_u32_t key);
};

#endif /* IHASH_H_ */
