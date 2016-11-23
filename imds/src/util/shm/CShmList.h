/**
 * @file    CShmList.h
 * @brief   CShmList共享内存对象头文件
 * @note
 *
 * CShmList是List类型共享内存容器对象，对象每条数据记录长度不预先固定。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     修订日期    修订人     修复的bug或者修订原因
 * - 2014-4-11 guoyue 调整节点链入List时修改指针顺序。
 * - 2014-6-10 guoyue push_back增加返回值
 * - 2014-6-27 guoyue pop增加已弹出记录条数判断，避免push_back时节点入链表时非原子操作。
 * @warning 无
 */
#ifndef CSHMLIST_H_
#define CSHMLIST_H_

#include "CShmObj.h"
#include "CShmHash.h"
#include "IList.h"

/** @name CShmList缺省值
 *  @{
 */
/** 空槽最小长度 */
#define SHM_MIN_SLOT        (0x100)
/** 空槽个数缺省值 */
#define SHM_SLOT_SIZE		(32)
#define SHM_SLOT_TOTAL_ALLOC (4 * 1024 * 1024)
/** @}*/ // CShmList缺省值
#pragma pack(push,8)

/**
 * @brief 定义类型ST_SHM_LIST_ITEM
 * @note 数据节点，ST_SHM_LIST_ITEM是保存Item数据的容器。
 * @remarks item数据存在value处，数据长度为itemLen。
 * 每个ST_SHM_LIST_ITEM占用空间实际是slotLen，不是sizeof(st_shm_list_item)
 */
typedef struct st_shm_list_item
{
    shm_u32_t slotLen;          ///<每个ST_SHM_LIST_ITEM实际分配空间大小。slotLen = rad2nAlloc(itemLen + sizeof(st_shm_list_itemhead))
    shm_offset_t previous;      ///<前一个Item
    shm_offset_t next;          ///<下一个Item
    shm_size_t itemLen;         ///<Item数据记录实际长度
    char value;                 ///<从value开始保存数据。从value开始计算长度itemLen的内存空间存放Item实际数据。
} ST_SHM_LIST_ITEM;

/**
 * @brief 定义类型ST_SHM_LIST_PROPERTY
 * @note ST_SHM_LIST_PROPERTY的偏移位置保存在ST_SHM_OBJ_PROPERTY中，
 * ST_SHM_LIST_PROPERTY存放共享内存对象List的私有属性。
 * @remarks first_item_offset与last_item_offset指向的是ST_SHM_LIST_ITEM，不是Item数据本身。
 */
typedef struct st_shm_list_property
{
    shm_offset_t shmSlotHt[SHM_SLOT_SIZE];	///<分配空闲空间表, 指向空闲shm_list_item列表
    time_t clearTime;						///<上一次数据清除时间
    volatile shm_offset_t first_item_offset;	///<第一条数据偏移量
    volatile shm_offset_t last_item_offset;		///<最后一条数据偏移量
} ST_SHM_LIST_PROPERTY;

#pragma pack(pop)

/**
 * @class CShmList CShmList.h "\source\dce\src\main\util\shm\CShmList.h"
 * @brief CShmList共享内存类型容器对象
 * @note CShmList是List类型共享内存容器对象，对象每条数据记录长度不预先固定。
 * - 对象中的数据使用双向链表记录,第一个节点偏移位置和最后一条数据偏移量记录在ST_SHM_LIST_PROPERTY中。
 * 向后添加数据时，先根据last_item_offset找到最后一条数据，把数据琏入最后一条数据，再改动last_item_offset指向新添加的数据。
 * - 链表中无数据时，first_item_offset与last_item_offset值为0。
 * - 链表中只有一条数据时，first_item_offset与last_item_offset指向同一个数据节点。
 * @remarks CShmList中每条数据长度不能预先确定，因此CShmList使用的内存分配策略如下:
 * - List把具有相同slotLen尺寸的空闲节点琏成双向链表, 链表第一个节点偏移量记录在shmSlotHt的某一元素中.
 * shmSlotHt记录在ST_SHM_LIST_PROPERTY属性中,是空闲节点索引表.
 * - shmSlotHt以n作为下标,每个下标指向2^n的slotLen空间.指向的节点列表能够容纳的数据长度(itemLen)范围在:
 * (((2^n)+1)- sizeof(st_shm_list_item)) <= itemLen < (((2^(n+1))- sizeof(st_shm_list_item))
 * - 添加记录时，先计算出slotLen，再根据slotLen计算出n。检查shmSlotHt[n]，如果shmSlotHt[n]中无空闲节点，
 * 使用分配器按接近SHM_SLOT_TOTAL_ALLOC的大小申请共享内存，申请后制作成空闲双向链表琏入shmSlotHt[n]，再 从
 * shmSlotHt[n]中取出空闲节点，存入数据后，琏入last_item_offset指向的链表中。
 * - 分配器以接近SHM_SLOT_TOTAL_ALLOC的大小申请共享内存的原因是每次分配预留空间，降低分配器分配内存频率。
 * - List容量动态增长，直到共享内存耗尽。
 * - List也支持clear方法，清除时不释放占用共享内存空间，只是把first_item_offset指向的数据链表根据slotLen值重新琏入shmSlotHt，
 * 琏入结束后，first_item_offset与last_item_offset被重置为0。容器容量不会改变。
 */
class CShmList: public CShmObj, public IList
{
public:
    /**
     * @brief CShmList构造函数
     * @param [in] ptrAlloctor 资源分配器
     * @param [in] ptrReader 读写器
     * @param [in] propertyOffset 内存对象公有属性偏移
     */
    CShmList(IAlloctor* ptrAlloctor, IReader *ptrReader,
            shm_offset_t propertyOffset);

    /** @brief CShmList析构函数*/
    virtual ~CShmList();

    /**
     * @brief 从List容器取得已存放记录条数
     * @return shm_size_t 容器数据记录条数
     */
    shm_size_t size()
    {
        return getSize();
    }
    ;

    /** @brief 从List容器取得第一条数据记录*/
    char *front(shm_size_t& itemLen);

    /** @brief 从List容器取得最后一条数据记录 */
    char *back(shm_size_t& itemLen);

    /** @brief 从List容器取得上一条数据记录 */
    char *previous(shm_size_t& itemLen);

    /** @brief 从List容器取得下一条数据记录 */
    char *next(shm_size_t& itemLen);

    /** @brief 判断容器是否为空*/
    bool empty();

    /** @brief 清除List容器数据记录 */
    void clear();

    /** @brief 向容器最后添加一条数据*/
    bool push_back(char* value, shm_size_t itemLen);

    /** @brief 弹出List中顶条记录*/
    bool pop(char* item, shm_size_t& itemLen);

private:
    /** @brief 计算应分配多少个空闲节点*/
    shm_u32_t slotCount(shm_u32_t slotLen, shm_u32_t slotAlloc =
            SHM_SLOT_TOTAL_ALLOC);
    /** @brief 计算要容纳长度为len的数据，需要的最小slotLen*/
    shm_u32_t rad2nAlloc(shm_u32_t len, shm_u32_t &n);
private:
    ST_SHM_LIST_PROPERTY *listProperty; ///<共享内存对象List的私有属性
    ST_SHM_LIST_ITEM *thisItem;			///<当前记录
    ST_SHM_LIST_ITEM *lastPopItem;		///<上次Pop记录
    time_t clearTime;					///<数据清除时间
    shm_size_t popCount;                ///<已经弹出的记录数
    //char *ptrItem;
    //shm_size_t itemLength;
};

#endif /* CSHMLIST_H_ */
