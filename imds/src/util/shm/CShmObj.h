/**
 * @file    CShmObj.h
 * @brief   CShmObj内存对象头文件
 * @note
 *
 * CShmObj内存对象主要用于维护ST_SHM_OBJ_PROPERTY表。
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef CSHMOBJ_H_
#define CSHMOBJ_H_

#include "IAlloctor.h"
#include "IReader.h"

/** @name 共享内存对象类型定义
 *  @{
 */

/** 未定义类型 */
#define SHM_OBJ_NULL	(0)
/** VECTOR类型 */
#define SHM_OBJ_VECTOR	(1)
/** LIST类型 */
#define SHM_OBJ_LIST	(2)
/** MAP类型 */
#define SHM_OBJ_MAP     (3)

/** @}*/ // 共享内存对象类型定义

/** @name 共享内存对象缺省属性值
 *  @{
 */
/** 对象名称字串最大长度 */
#define SHM_NAME_SIZE	(32)
/** @}*/ // 共享内存对象缺省属性值
#pragma pack(push,8)
/**
 * @brief 定义类型 ST_SHM_OBJ_PROPERTY
 * @note ST_SHM_OBJ_PROPERTY的偏移位置保存在内存对象管理区dictEntry中，
 * ST_SHM_OBJ_PROPERTY存放内存对象的公用属性。
 */
typedef struct st_shm_obj_property
{
    shm_u32_t shmObjType;			///<对象类型定义.@see 共享内存对象类型定义
    char shmObjName[SHM_NAME_SIZE];	///<对象名称
    shm_offset_t shmObjOffset;		///<对象在内存中的偏移位置
    shm_size_t occupied;			///<对象占用内存字节数
    volatile shm_size_t size;		///<对象容量
    volatile shm_size_t used;		///<对象中已保存数据条数
} ST_SHM_OBJ_PROPERTY;
#pragma pack(pop)

/**
 * @class CShmObj CShmObj.h "\source\dce\src\main\util\shm\CShmObj.h"
 * @brief CShmObj内存对象
 * @note  CShmObj内存对象主要用于维护ST_SHM_OBJ_PROPERTY。
 * ST_SHM_OBJ_PROPERTY存放内存对象的公用属性。\n
 * 共享内存对象在共享内存中只存放属性和数据,分为三级构成:
 * - 内存对象公用属性，\n
 * - 内存对象私有属性\n
 * - 内存对象数据\n
 * ST_SHM_OBJ_PROPERTY在共享内存中位置记录在objPropertyOffset中，由
 * objPropertyOffset可取到ptrObjProperty, ptrObjProperty中的
 * shmObjOffset记录对象私有属性位置， 由shmObjOffset可取得到vectorProperty
 * 或listProperty。具体是vectorProperty还是listProperty由shmObjType决定。
 * @remarks CShmObj使用共享内存静态分配器(CShmStaticAlloctor)实现共享内存分配
 */
class CShmObj
{
public:
    /**
     * @brief CShmObj构造函数
     */
    CShmObj(IAlloctor* ptrAlloctor, IReader *ptrReader,
            shm_offset_t propertyOffset);

    /** @brief CShmObj析构函数*/
    virtual ~CShmObj();

    /** @brief 取得内存对象类型*/
    virtual shm_u32_t getShmObjType();

    /** @brief 取得内存对象名称*/
    const char* getShmObjName() const;

    /** @brief 取得内存对象保存数据条数*/
    shm_size_t getSize() const;

    /** @brief 取得内存对象容量*/
    shm_size_t getCapacity() const;

    /** @brief 取得内存对象占用字节数*/
    shm_size_t getOccupied() const;
protected:
    /** @brief 取出分配器*/
    IAlloctor* getAlloctor();

    /** @brief 取出读写器*/
    IReader *getReader();

    /** @brief 取出内存对象的公用属性*/
    ST_SHM_OBJ_PROPERTY *getObjProperty();

    /** @brief 设置内存对象已使用量*/
    void setSize(shm_size_t size);

    /** @brief 设置内存对象容量*/
    void setCapacity(shm_size_t capacity);

    /** @brief 内存对象占用字节数增加occupy字节*/
    void incOccupied(shm_size_t occupy);

    /** @brief 设置内存对象已使用量自加一*/
    void incSize();
private:
    IAlloctor *ptrAlloctor;				///<分配器
    IReader *ptrReader;					///<读写器
    shm_offset_t objPropertyOffset;		///<对象属性位置
    ST_SHM_OBJ_PROPERTY *ptrObjProperty;		///<对象属性
};

#endif /* CSHMOBJ_H_ */
