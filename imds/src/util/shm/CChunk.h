/**
 * @file    CChunk.h
 * @brief   Chunk类型接口头文件
 * @note
 *
 * CChunk是IChunk子类, CChunk中包括了一个资源读写器、 对象字典、资源分配器。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2014-3-18
 * @bug     无
 * @warning 无
 */

#ifndef CCHUNK_H_
#define CCHUNK_H_

#include "IChunk.h"
#include "IHash.h"
#include "IAlloctor.h"
#include "IReader.h"
#include "CShmObj.h"

/** @name 缺省值宏定义
 *  @{
 */
/** 管理区标识最大长度 */
#define SHM_IDTLEN      (16)
/** 管理区版本号最大长度 */
#define SHM_VERLEN      (8)
/** 管理区创建日期最大长度 */
#define SHM_DEFLEN      (16)

/** 管理区缺省标识 */
#define SHM_IDT         "Level-2 DCE"
/** 管理区缺省版本号*/
#define SHM_VER         "2.0.0"
/** @}*/ // 共享内存对象类型定义

#pragma pack(push,8)
/**
 * @brief 共享内存环境管理区结构体(Structure of share memory environment. )
 * @note ST_SHM_ENV保存在共享内存起始位置,用于共享内存环境总体管理。
 * 环境管理区中主要包括共享内存长度和已使用空间记录，使用Hash表记录共享内存对象索引。
 */
typedef struct st_shm_env
{
    /** @brief 共享内存环境管理区标识*/
    char shmIdentity[SHM_IDTLEN];
    /** @brief 共享内存环境管理区版本号*/
    char version[SHM_VERLEN];
    /** @brief 共享内存环境管理区创建日期。samples:"20130305093001"*/
    char createDate[SHM_DEFLEN];
    /** @brief 共享内存key值，用于校验*/
    key_t keyId;
    /** @brief 共享内存长度，以字节计算。*/
    volatile shm_size_t shmSize;
    /** @brief 共享内存已使用空间，以字节计算。*/
    volatile shm_size_t shmUsed;
    /** @brief 共享内存对象字典表*/
    ST_SHM_HT shmDictHt;
    //2015-9-23 guoyue 对象属性表前移到共享内存环境管理区
    /** @brief 共享内存对象属性表占用空间*/
    char shmObjZone[HASH_MAX_SIZE * sizeof(ST_SHM_OBJ_PROPERTY)];
    //2015-9-23 guoyue end modify
    /** @brief 共享内存环境管理区占用空间，以字节计算。*/
    shm_size_t shmEnvZoneLen;
    /** @brief 共享内存环境管理区结束位置，管理区之后为数据区*/
    char shmDataZone;
} ST_SHM_ENV;
#pragma pack(pop)
/**
 * @class CChunk CChunk.h "\source\dce\src\main\util\shm\CChunk.h"
 * @brief CChunk.
 * @note
 * CChunk通过方法：getReader、getDictHt、getAlloctor，分别取出资源读写器、取出对象字典、取出资源分配器\n
 */
class CChunk: public IChunk
{
public:
    CChunk();
    virtual ~CChunk();
public:

    /** @brief 取出资源读写器*/
    virtual IReader *getReader();

    /** @brief 取出对象字典*/
    virtual IHash *getDictHt();

    /** @brief 取出资源分配器*/
    virtual IAlloctor *getAlloctor();

    /** @brief 设置keyId*/
    virtual void setKeyId(key_t keyId);

    /** @brief 构造读写器*/
    virtual IReader *buildReader() = 0;

    /** @brief 构造管理区*/
    virtual IHash *buildChunk(shm_size_t shmSize) = 0;

    /** @brief 重建管理区*/
    virtual IHash *rebuildChunk() = 0;

    /** @brief 构造资源分配器*/
    virtual IAlloctor *buildAlloctor() = 0;
protected:
    /** @brief 初始化共享内存环境管理区*/
    ST_SHM_ENV *buildEnv(shm_size_t shmSize);
    /** @brief 检查共享内存环境管理区标识*/
    bool checkIdentity();
protected:
    IHash *ptrDictHt;               ///<共享内存对象字典
    IAlloctor* ptrAlloctor;         ///<资源分配管理器
    IReader *ptrReader;             ///<读写器
    key_t key_id;
};

#endif /* CCHUNK_H_ */
