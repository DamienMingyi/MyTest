/*
 * CShmMap.h
 *
 *  Created on: 2016年10月12日
 *      Author: guoyue
 */

#ifndef CSHMMAP_H_
#define CSHMMAP_H_

#include "CShmObj.h"
#include "IMap.h"
#include "CShmVector.h"
#include <map>
#include <string>

using namespace std;
/** @name CShmMap缺省值
 *  @{
 */
/** key长度缺省值 */
#define SHM_MAP_KEY_SIZE   (64)
/** @}*/ // CShmMap缺省值
#pragma pack(push,8)

/**
 * @brief 定义类型ST_SHM_MAP_HEAD
 * @note
 *
 * @remarks
 *
 */
typedef struct st_shm_map_head
{
    shm_s32_t isRemoved;
    char key[SHM_MAP_KEY_SIZE]; ///<Key,
    //char value[itemLen];
}ST_SHM_MAP_HEAD;

#pragma pack(pop)

class CShmMap: public CShmObj, public IMap
{
public:
    CShmMap(IAlloctor* ptrAlloctor, IReader *ptrReader,
            shm_offset_t propertyOffset);
    virtual ~CShmMap();
public:
    /**
     * @brief 从容器取得已存放记录条数
     * @return shm_size_t 容器数据记录条数
     */
    shm_size_t size();

    /** @brief 向Map容器添加记录*/
    bool push();

    /** @brief 向Map容器添加记录*/
    bool enter(char *key, char *value);

    bool remove(char *key);

    char *find(char *key);

    /** @brief 清除容器数据记录 */
    void clear();

    /** @brief 从Map容器按key获取记录*/
    char& operator[](char *key);

    /** @brief 从Map容器取得数据记录长度 */
    shm_size_t getItemLength();
private:
    /** @brief 从内部Vector容器按下标取得数据记录*/
    char* at(shm_size_t i);
    /** @brief 向容器最后添加一条数据*/
    bool push_back(char* item);
    /** @brief 重建页面数据索引 */
    bool rebuildPageIndex();
    bool rebuildKeyValueMap();
    char *findMapInfo(char *key);
private:
    ST_SHM_VECTOR_PROPERTY *vectorProperty;     ///<共享内存对象Vector的私有属性
    vector<ST_SHM_VECTOR_PAGE *> pageIndex;     ///<页面索引表
    map<string, char*> mapIndex;                ///<key与对应值偏移量对应表
    time_t clearTime;                           ///<数据清除时间
    shm_size_t position;                        ///<下标
    char *ptrItem;                              ///<指向下标数据记录
    char *ptrValue;
    char *ptrBuff;
};

#endif /* CSHMMAP_H_ */
