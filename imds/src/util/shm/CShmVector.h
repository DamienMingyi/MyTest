/**
 * @file    CShmVector.h
 * @brief   CShmVector共享内存对象头文件
 * @note
 *
 * CShmVector是Vector类型共享内存容器对象，对象数据记录长度在创建时固定。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef CSHMVECTOR_H_
#define CSHMVECTOR_H_

#include "CShmObj.h"
#include "IVector.h"
#include <vector>

using namespace std;
/** @name CShmVector缺省值
 *  @{
 */
/** 每页容量缺省值 */
#define SHM_PAGE_SIZE	(4096)
/** 最大页数缺省值 */
#define SHM_MAX_PAGE	(8192)
/** @}*/ // CShmVector缺省值
#pragma pack(push,8)

/**
 * @brief 定义类型ST_SHM_VECTOR_PROPERTY
 * @note ST_SHM_VECTOR_PROPERTY的偏移位置保存在ST_SHM_OBJ_PROPERTY中，
 * ST_SHM_VECTOR_PROPERTY存放共享内存对象Vector的私有属性。
 * @remarks usedPageId记录push_back操作当前页；usedPageFree记录当前使用页剩余空间。
 * push_back时当usedPageFree为0会分派新页，为0且(usedPageId+1)==pageCount分配页内存。
 * 分配页内存成功后，容器容量会增加。
 */
typedef struct st_shm_vector_property
{
    shm_u32_t pageCount;               ///<页数
    shm_offset_t firstPage;            ///<第一页偏移地址
    shm_u32_t pageSize;                ///<每一页大小
    shm_size_t itemLen;				   ///<每条记录长度
    shm_u32_t usedPageId;              ///<当前使用页索引。
    shm_u32_t usedPageFree;            ///<最后一页剩余可存储数据条数。取值范围：0..pageSize。
    time_t clearTime;                  ///<数据清除时间
} ST_SHM_VECTOR_PROPERTY;

/**
 * @brief 定义类型ST_SHM_VECTOR_PAGE
 * @note ST_SHM_VECTOR_PROPERTY中firstPage指向第一页数据页偏移位置，
 * ST_SHM_VECTOR_PAGE存放每一数据页的本页索引号，前一数据页偏移，下一数据页偏移，本页第一条数据偏移。
 * @remarks ShmVector的数据按数据页存放，数据页构成
 *
 */
typedef struct st_shm_vector_page
{
    shm_u32_t pageId;                       ///<本页索引号,
    shm_offset_t priorPage;                 ///<前一页偏移
    shm_offset_t nextPage;                  ///<下一页偏移
    shm_offset_t thisPageItem;              ///<本页第一条数据偏移
}ST_SHM_VECTOR_PAGE;

#pragma pack(pop)

/**
 * @class CShmVector CShmVector.h "\source\dce\src\main\util\shm\CShmVector.h"
 * @brief CShmVector共享内存类型容器对象
 * @note CShmVector是数据记录长度在创建时固定的共享内存对象容器。
 * 对象中的数据记录按页保存，每页空间预先申请。
 * - 添加记录时，根据ST_SHM_VECTOR_PROPERTY中usedPageId指示直接找到页面，
 * 再对当前size取模得到在页面中的偏移位置存入即可。
 * - 查找记录时，根据数据记录下标整除页面长度得到页面索引，再对记录下标取模得到在页面中的偏移位置，
 * 根据页面索引与页面偏移位置即可取出数据记录
 * @remarks
 * - Vector容量大小的为： pageSize * pageCount
 * - Vector容量在缺页时按每页长度动态增长，直到共享内存耗尽或达到最大容量值。
 * - Vector容器不实现删除数据记录功能，但支持数据清除方法。
 * - 执行数据清除方法，只是清除数据记录内容，不会回收共享内存，即Vector内存对象的容量不会改变。
 */
class CShmVector: public CShmObj, public IVector
{
public:

    /** @brief CShmVector构造函数*/
    CShmVector(IAlloctor* ptrAlloctor, IReader *ptrReader,
            shm_offset_t propertyOffset);

    /** @brief CShmObj析构函数*/
    virtual ~CShmVector();

    /**
     * @brief 从Vector容器取得已存放记录条数
     * @return shm_size_t 容器数据记录条数
     */
    shm_size_t size()
    {
        return getSize();
    }
    ;

    /** @brief 从Vector容器按下标取得数据记录*/
    char* at(shm_size_t i);

    /** @brief 从Vector容器取得第一条数据记录 */
    char* front();

    /** @brief 从Vector容器取得最后一条数据记录 */
    char* back();

    /** @brief 清除Vector容器数据记录 */
    void clear();

    /** @brief 判断容器是否为空*/
    bool empty();

    /** @brief 向容器最后添加一条数据*/
    bool push_back(char* item);

    bool push();

    /**
     * @brief 从Vector容器按下标取得数据记录引用
     * @param [in] i 数据记录下标
     * @return char& 数据记录引用
     */
    char& operator[](shm_size_t i)
    {
        return (*at(i));
    }
    ;

    /** @brief 从Vector容器取得数据记录长度 */
    shm_size_t getItemLength();
private:
    /** @brief 重建页面数据索引 */
    bool rebuildIndex();
private:
    ST_SHM_VECTOR_PROPERTY *vectorProperty;				///<共享内存对象Vector的私有属性
    typedef vector<ST_SHM_VECTOR_PAGE *> PAGE_INDEX;
    PAGE_INDEX pageIndex;                               ///<页面索引表
    shm_size_t  position;                               ///<下标
    char *ptrItem;                                      ///<指向下标数据记录
    time_t clearTime;                                   ///<数据清除时间
};

#endif /* CSHMVECTOR_H_ */
