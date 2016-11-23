/**
 * @file    IList.h
 * @brief   IList接口头文件
 * @note
 *
 * List容器接口。\n
 *
 * @author  gaoyun
 * @version 1.0
 * @date    2013-3-13
 * @bug     无
 * @warning 无
 */
#ifndef ILIST_H_
#define ILIST_H_

#include "CShmTypes.h"

/**
 * @class IList IList.h "\source\dce\src\main\util\shm\IList.h"
 * @brief List容器接口.
 * @note
 * List容器包括向后添加数据、取得数据等操作。\n
 */
class IList
{
public:
    /** @brief IList构造函数*/
    IList()
    {
    }
    ;

    /** @brief IList析构函数*/
    virtual ~IList()
    {
    }
    ;

    /** @brief 从List取得已存放记录条数*/
    virtual shm_size_t size()=0;

    /** @brief 取得List中记录*/
    virtual bool pop(char* item, shm_size_t& itemLen)=0;

    /** @brief 向List最后位置添加记录*/
    virtual bool push_back(char* value, shm_size_t itemLen)=0;
};

#endif /* ILIST_H_ */
