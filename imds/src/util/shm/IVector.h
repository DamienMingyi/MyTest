/**
 * @file    IVector.h
 * @brief   IVector接口头文件
 * @note
 *
 * Vector容器接口。\n
 *
 * @author  gaoyun
 * @version 1.0
 * @date    2013-3-13
 * @bug     无
 * @warning 无
 */
#ifndef IVECTOR_H_
#define IVECTOR_H_

#include "CShmTypes.h"

/**
 * @class IVector IVector.h "\source\dce\src\main\util\shm\IVector.h"
 * @brief Vector容器接口.
 * @note
 * Vector容器包括添加数据、向后添加数据、按下标取数据、得到容器记录条数等操作。\n
 */
class IVector
{
public:
    /** @brief IVector构造函数*/
    IVector()
    {
    }

    /** @brief IVector析构函数*/
    virtual ~IVector()
    {
    }
public:
    /** @brief 向Vector容器添加记录*/
    virtual bool push() = 0;

    /** @brief 向Vector容器最后位置添加记录*/
    virtual bool push_back(char *item)=0;

    /** @brief 从Vector容器按下标获取记录*/
    virtual char& operator[](shm_size_t i)=0;

    /** @brief 从Vector容器取得已存放记录条数*/
    virtual shm_size_t size()=0;

    /** @brief 从Vector容器按下标获取记录*/
    char& getItem(shm_size_t i)
    {
        return (*this)[i];
    }
    ;
};

#endif /* IVECTOR_H_ */
