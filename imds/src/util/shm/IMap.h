/**
 * @file    IMap.h
 * @brief   IMap接口头文件
 * @note
 *
 * Map容器接口。\n
 *
 * @author  gaoyun
 * @version 1.0
 * @date    2013-3-13
 * @bug     无
 * @warning 无
 */
#ifndef IMAP_H_
#define IMAP_H_

#include "CShmTypes.h"

class IMap
{
public:
    /** @brief IMap构造函数*/
    IMap();

    /** @brief IMap析构函数*/
    virtual ~IMap();
public:

    /** @brief 向Map容器添加记录*/
    virtual bool push() = 0;

    /** @brief 向Map容器添加记录*/
    virtual bool enter(char *key, char *value)=0;

    virtual bool remove(char *key)=0;

    /** @brief 从Map容器按key获取记录*/
    virtual char& operator[](char *key)=0;

    /** @brief 从Map容器取得已存放记录条数*/
    virtual shm_size_t size()=0;

    virtual char *find(char *key) = 0;

    /** @brief 从Map容器按key获取记录*/
    char& getItem(char *key)
    {
        return (*this)[key];
    };
};

#endif /* IMAP_H_ */
