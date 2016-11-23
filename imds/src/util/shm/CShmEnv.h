/**
 * @file    CShmEnv.h
 * @brief   共享内存环境管理头文件
 * @note
 *
 * CShmEnv是共享内存数据存取接口。共享内存环境管理主要功能是是管理共享内存环境,向使用者提供获取共享内存对象的方法。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef CSHMENV_H_
#define CSHMENV_H_

#include <map>
#include <vector>
#include <string>

#include "CShmTypes.h"
#include "CShmList.h"
#include "CShmVector.h"
#include "CShmMap.h"
#include "IChunk.h"

using namespace std;
/** @name 共享内存环境管理信息查询结构类型定义
 *  @{
 */

/**
 * @brief 定义类型 ENV_TYPE
 * @note 环境类型
 */
typedef enum Env_Type
 {
    ENV_SHM = 0,
    ENV_MMAP,
 }ENV_TYPE;

/**
 * @brief 定义类型 ST_SHM_OBJ_INFO
 * @note 共享内存对象查询信息
 */
typedef struct st_shm_obj_info
{
    string shmObjName;		///<共享对象名称
    shm_u32_t shmObjType;	///<共享对象类型定义.\n 0:未定义类型,1:VECTOR类型,2:LIST类型,3:MAP类型
    shm_size_t capacity;	///<共享对象容量
    shm_size_t size;		///<共享对象中已保存数据条数
    shm_size_t occupied;	///<共享对象占用内存字节数
} ST_SHM_OBJ_INFO;

/** 定义类型SHM_OBJ_INFO_LIST */
typedef vector<ST_SHM_OBJ_INFO> SHM_OBJ_INFO_LIST;

/**
 * @brief 定义类型 ST_SHM_ENV_INFO
 * @note 共享内存环境管理区查询信息
 */
typedef struct st_shm_env_info
{
    string identity;		///<共享内存环境管理区标识
    string version;			///<共享内存环境管理区版本号
    string createDate;		///<共享内存环境管理区创建日期。@par 数据格式实例:"20130305093001"
    key_t keyId;
    shm_size_t envZoneLen;	///<共享内存环境管理区占用空间，以字节计算。
    shm_size_t envCapacity; ///<共享内存长度，以字节计算。
    shm_size_t envSize;		///<共享内存已使用空间，以字节计算。
    shm_size_t envFree;		///<共享内存剩余可使用长度,以字节计算。
    shm_u32_t objsCapacity; ///<共享内存环境可供存储内存对象数量
    shm_u32_t objsSize;		///<共享内存环境已存在内存对象数量
} ST_SHM_ENV_INFO;
/** @}*/ // 共享内存环境管理信息查询结构类型定义
/**
 * @class CShmEnv CShmEnv.h "\source\dce\src\main\util\shm\CShmEnv.h"
 * @brief CShmEnv共享内存环境管理。
 * @note  共享内存环境管理是共享内存数据存取接口, 主要功能是向使用者提供获取共享内存对象的方法。
 * 共享内存环境管理中方法分为三类:
 * - 共享内存环境管理\n
 * - 提供获取共享内存对象的方法\n
 * - 共享内存环境管理信息查询方法\n
 * @remarks  CShmEnv内部维护了共享内存对象映射表(CShmObjMap)。\n
 * 获取共享内存对象时,首先从映射表中查询,\n
 * 如果映射表中无此对象,从共享内存环境管理区中查找。\n
 * 如果共享内存环境管理区中也无此对象,且传入参数的数据长度(itemlen)不为0,CShmEnv才会在共享内存中新建对象.\n
 * 从共享内存环境管理区中取回或新建的内存对象会自动添加到共享内存对象映射表中。
 * @warning  创建与销毁共享内存对象都由CShmEnv完成。\n
 * 应用能够获取共享内存对象并使用，但不能自行创建与销毁共享内存对象。
 */
class CShmEnv
{
public:
    /** @brief CShmEnv构造函数*/
    CShmEnv();
    /** @brief CShmEnv构造函数*/
    CShmEnv(ENV_TYPE envType);
    /** @brief CShmEnv析构函数*/
    virtual ~CShmEnv();

    /** @brief 创建共享内存管理环境结构体长度*/
    shm_size_t getShmEnvSTSize();
    /** @brief 创建共享内存管理环境*/
    shm_status_t createShmEnv(const char *fileName, shm_size_t shmSize);
    /** @brief 打开共享内存管理环境*/
    shm_status_t openShmEnv(const char *fileName);
    /** @brief 关闭共享内存管理环境*/
    shm_status_t closeShmEnv(void);
    /** @brief 关闭共享内存管理环境,清除使用资源*/
    shm_status_t releaseShmEnv(void);

    /** @brief 获取List类型共享内存对象*/
    CShmList *getShmList(const char* shmObjName, shm_size_t itemlen = 0);

    /** @brief 获取Vector类型共享内存对象*/
    CShmVector *getShmVector(const char* shmObjName, shm_size_t itemlen = 0,
            shm_size_t capacity = 0);

    /** @brief 获取Map类型共享内存对象*/
    CShmMap *getShmMap(const char* shmObjName, shm_size_t itemlen = 0,
            shm_size_t capacity = 0);

    /** @brief 查询共享内存管理环境信息*/
    bool getEnvInfo(ST_SHM_ENV_INFO &envInfo);
    /** @brief 查询共享内存对象信息*/
    bool getShmObjInfo(SHM_OBJ_INFO_LIST &objInfoList);

private:
    /** @brief 共享内存对象映射表中查找对象*/
    CShmObj* findShmObj(const char* shmObjName);
    /** @brief 释放共享内存对象映射表资源*/
    void clearShmObj();
    /** @brief 向共享内存对象映射表添加对象*/
    void addShmToMap(const char* shmObjName, CShmObj *shmObj);

    /** @brief 实例化资源管理器*/
    IChunk *getChunkInst();
private:
    IChunk* ptrChunk; ///<资源管理器
    /** 定义类型CShmObjMap */
    typedef map<string, CShmObj *> CShmObjMap;
    CShmObjMap shmObjMap; ///<共享内存对象映射表
    ENV_TYPE envType;
};

#endif /* CSHMENV_H_ */
