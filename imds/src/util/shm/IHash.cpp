/**
 * @file    IHash.cpp
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

#include "IHash.h"

/**
 *  IHash构造函数
 */
IHash::IHash()
{
    // TODO Auto-generated constructor stub

}

/**
 *  IHash析构函数
 */
IHash::~IHash()
{
    // TODO Auto-generated destructor stub
}

/**
 *  @param [in] key 哈希Key
 *  @return shm_size_t 计算出的哈希值
 */
shm_size_t IHash::hash(const char *key)
{
    shm_u32_t h = 0;
    shm_u32_t g = 0;

    if (key == NULL)
    {
        return 0;
    }

    //计算哈希值
    for (int i = 0; i <= HASH_KEY_SIZE; i++)
    {
        if (*key)
        {
            h = (h << 4) + *key++;
            if (0 != (g = (h & 0xf0000000)))
            {
                h ^= (g >> 24);
                h ^= g;
            }

        }
        else
        {
            break;
        }
    }
    return (h % HASH_MAX_SIZE);
}

/**
 *  @param [in] key 哈希Key
 *  @return shm_size_t 计算出的哈希值
 */
shm_size_t IHash::hash(const shm_u32_t key)
{
    if (key == 0)
    {
        return 0;
    }

    return (key % HASH_MAX_SIZE);
}
