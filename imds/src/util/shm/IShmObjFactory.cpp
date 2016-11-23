/**
 * @file    IShmObjFactory.cpp
 * @brief   IShmObjFactory共享内存对象工厂接口文件
 * @note
 *
 * IShmObjFactory共享内存对象工厂接口。接口包括创建、打开、取得共享内存对象的方法。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */

#include "IShmObjFactory.h"

/**
 *  IShmObjFactory构造函数
 */
IShmObjFactory::IShmObjFactory(IAlloctor *ptrAlloctor)
{
    // TODO Auto-generated constructor stub
    this->ptrAlloctor = ptrAlloctor;
}

/**
 *  IShmObjFactory析构函数
 */
IShmObjFactory::~IShmObjFactory()
{
    // TODO Auto-generated destructor stub
}

