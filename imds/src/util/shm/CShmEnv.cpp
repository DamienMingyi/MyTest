/**
 * @file    CShmEnv.cpp
 * @brief   共享内存环境管理文件
 * @note
 *
 * CShmEnv是共享内存数据存取接口。共享内存环境管理主要功能是是管理共享内存环境,向使用者提供获取共享内存对象的方法。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     修订日期    修订人     修复的bug或者修订原因
 * - 2013/5/29 guoyue createShmEnv方法添加数据长度范围限制
 * - 2013/5/29 guoyue getShmList方法添加数据记录范围限制
 * - 2013/5/29 guoyue getShmVector方法添加数据记录范围限制
 * @warning 无
 */
#include "CShmEnv.h"
#include "CMmapChunk.h"
#include "CShmChunk.h"
#include "CShmVectorFactory.h"
#include "CShmListFactory.h"
#include "CShmMapFactory.h"

/**
 *  CShmEnv构造函数
 */
CShmEnv::CShmEnv()
{
    // TODO Auto-generated constructor stub
    this->ptrChunk = NULL;
    //this->envType = ENV_MMAP;
    this->envType = ENV_SHM;
}
/**
 *  CShmEnv构造函数
 */
CShmEnv::CShmEnv(ENV_TYPE envType)
{
    // TODO Auto-generated constructor stub
    this->ptrChunk = NULL;
    this->envType = envType;
}

/**
 *  CShmEnv析构函数
 */
CShmEnv::~CShmEnv()
{
    // TODO Auto-generated destructor stub
    closeShmEnv();
}

/**
 *  实例化资源管理器
 */
IChunk *CShmEnv::getChunkInst()
{
    if(this->ptrChunk)
    {
        return this->ptrChunk;
    }

    switch (this->envType)
    {
        case ENV_SHM:
            this->ptrChunk = new CShmChunk();
            break;
        case ENV_MMAP:
            this->ptrChunk = new CMmapChunk();
            break;
        default:
            break;
    }
    return (this->ptrChunk);
}



/**
 *  openShmEnv方法打开已存在的共享内存环境,并初始化。
 *  @param [in] fileName 共享内存标识文件路径与文件名
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 共享内存环境已存在且共享内存标识文件也存在
 *  @note openShmEnv按参数尝试打开已存在的共享内存环境。
 *  如果共享内存环境存在, 只需取出内存环境管理区使用.
 *  @remarks openShmEnv方法取出内存环境管理区后，进行资源分配管理器初始化。
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmEnv::openShmEnv(const char* fileName)
{
    shm_status_t shmStatus = SUCCEED;

    //参数检查
    if (fileName == NULL)
    {
        return ERR_BADF;
    }

    //资源管理器已实例化
    if (this->ptrChunk)
    {
        return shmStatus;
    }

    //新建资源管理器
    CChunk *chunkInst = (CChunk *)getChunkInst();
    if (!chunkInst)
    {
        return ERR_PARAM;
    }

    //新建读写器
    IReader *ptrChunkReader = chunkInst->buildReader();
    if (NULL == ptrChunkReader)
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return ERR_NOMEM;
    }

    //打开读写器
    shmStatus = ptrChunkReader->open(fileName);
    if (SUCCEED != shmStatus)
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return shmStatus;
    }

    //重新组装管理区
    if(!chunkInst->rebuildChunk())
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return ERR_NOMEM;
    }

    //资源分配管理器初始化
    if (NULL == chunkInst->buildAlloctor())
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return ERR_NOMEM;
    }
    return SUCCEED;
}

shm_size_t CShmEnv::getShmEnvSTSize()
{
    return (sizeof(ST_SHM_ENV));
}

/**
 *  createShmEnv方法首先根据传入参数向系统申请共享内存,在共享内存构造共享内存环境管理区,并初始化。
 *  @param [in] fileName 共享内存标识文件路径与文件名
 *  @param [in] shmSize 申请共享内存大小
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 系统可供申请的共享内存大于shmSize
 *  @note 调用createShmEnv方法时,首先按参数尝试打开已存在的共享内存环境,
 *  如果共享内存环境不存在,才会向系统申请共享内存,并构造共享内存环境管理区,并对管理区初始化.
 *  @remarks createShmEnv方法也进行资源分配管理器初始化
 *  @see 操作返回值可参考CShmError
 *  @bug 2013/5/29 guoyue 添加数据长度范围限制
 */
shm_status_t CShmEnv::createShmEnv(const char* fileName, shm_size_t shmSize)
{
    shm_status_t shmStatus = SUCCEED;
    //参数检查
    if (fileName == NULL)
    {
        return ERR_BADF;
    }

    //guoyue 2013/5/29 添加数据长度范围限制
    if (shmSize > (shm_MAX_SIZE_T >> 1))
    {
        return ERR_PARAM;
    }

    //检查期望分配的资源是否小到不能容下管理区
    if (shmSize < getShmEnvSTSize())
    {
        return ERR_NOSPC;
    }

#ifdef SHM_64BIT
    shm_size_t maxSize = (1024 * 1024 * 1024 * 256L);
#else
    shm_size_t maxSize = (1024 * 1024 * 1024);
#endif

    if (shmSize > maxSize)
    {
        return ERR_NOSPC;
    }
    //资源管理器已实例化
    if (this->ptrChunk)
    {
        return shmStatus;
    }

    //如果共享内存已经存在,直接打开返回
    shmStatus = openShmEnv(fileName);
    if (SUCCEED == shmStatus)
    {
        return shmStatus;
    }

    //新建资源管理器
    CChunk *chunkInst = (CChunk *)getChunkInst();
    if (!chunkInst)
    {
        return ERR_PARAM;
    }

    //新建读写器
    IReader *ptrChunkReader = chunkInst->buildReader();
    if (NULL == ptrChunkReader)
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return ERR_NOMEM;
    }

    //申请创建资源
    shmStatus = ptrChunkReader->create(fileName, shmSize);
    if (SUCCEED != shmStatus)
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return shmStatus;
    }

    //开始构造共享内存管理区
    if(!chunkInst->buildChunk(shmSize))
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return ERR_NOMEM;
    }

    //资源分配管理器初始化
    if (NULL == chunkInst->buildAlloctor())
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
        return ERR_NOMEM;
    }

    return SUCCEED;
}

/**
 *  closeShmEnv方法关闭共享内存环境,释放共享内存对象映射表资源，释放资源分配管理器。
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 共享内存环境已打开
 *  @note
 *  @remarks
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmEnv::closeShmEnv(void)
{
    shm_status_t shmStatus = SUCCEED;

    //如果资源管理器已经释放,不需要再做更多操作
    if (this->ptrChunk == NULL)
    {
        return shmStatus;
    }

    //释放内存对象映射表资源
    clearShmObj();

    CChunk *chunkInst = (CChunk *)getChunkInst();
    IReader *ptrReader = chunkInst->getReader();

    //关闭读写器
    if (ptrReader)
    {
        shmStatus = ptrReader->close();
    }

    //释放资源管理器
    if (chunkInst)
    {
        delete this->ptrChunk;
        this->ptrChunk = NULL;
    }
    return shmStatus;
}

/**
 *  releaseShmEnv方法关闭共享内存环境,释放共享内存,释放共享内存对象映射表资源,释放资源分配管理器。
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 共享内存环境已打开
 *  @note
 *  @remarks
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmEnv::releaseShmEnv(void)
{
    shm_status_t shmStatus = SUCCEED;

        //如果资源管理器已经释放,不需要再做更多操作
        if (this->ptrChunk == NULL)
        {
            return shmStatus;
        }

        //释放内存对象映射表资源
        clearShmObj();

        CChunk *chunkInst = (CChunk *)getChunkInst();
        IReader *ptrReader = chunkInst->getReader();

        //关闭读写器
        if (ptrReader)
        {
            shmStatus = ptrReader->remove();
        }

        //释放资源管理器
        if (chunkInst)
        {
            delete this->ptrChunk;
            this->ptrChunk = NULL;
        }
        return shmStatus;
}

/**
 *  getShmList方法按名称优先查找已存在对象,对象不存在且itemlen为0时新增List对象。
 *  @param [in] shmObjName List对象名
 *  @param [in] itemlen List对象每条数据缺省长度
 *  @return CShmList* 已存在或新建的List类型共享内存对象
 *  @pre \e 共享内存环境已打开
 *  @note 获取共享内存对象时,首先从映射表中查询,\n
 * 如果映射表中无此对象,使用CShmListFactory从共享内存环境管理区中查找。\n
 * 如果共享内存环境管理区中也无此对象,且传入参数的数据长度(itemlen)不为0,CShmListFactory在共享内存中新建对象.\n
 * 从共享内存环境管理区中取回或新建的内存对象会自动添加到共享内存对象映射表中。
 *  @remarks
 *  - 记录数据长度(itemlen)为0时,只查找已存在List,不会创建List.
 *  - 如果存在同名的内存对象,但类型不是List,返回值为空
 *  @bug 2013/5/29 guoyue 添加数据记录范围限制
 */
CShmList* CShmEnv::getShmList(const char* shmObjName, shm_size_t itemlen)
{
    //参数检查
    if (shmObjName == NULL)
    {
        return NULL;
    }
    //guoyue 2013/5/29 添加数据记录范围限制
    if (itemlen >= shm_MAX_U32 )
    {
        return NULL;
    }

    //查找已打开过的对象
    CShmObj* shmObj = findShmObj(shmObjName);
    //找到名字相同的对象了
    if (shmObj)
    {
        //检查类型对不对
        if (SHM_OBJ_LIST != shmObj->getShmObjType())
        {
            return NULL;
        }
        return (CShmList*) shmObj;
    }

    //分配器有效性检查
    if (this->ptrChunk == NULL)
    {
        return NULL;
    }
    CChunk *chunkInst = (CChunk *)getChunkInst();
    CShmListFactory shmListFactory(chunkInst->getAlloctor());
    //使用List工厂取出对象
    shmObj = shmListFactory.getShmObj(shmObjName, itemlen);

    //取出的对象加到对象映射表,以后再使用时就可直接取出
    if (NULL != shmObj)
    {
        addShmToMap(shmObjName, shmObj);
    }

    return (CShmList*) shmObj;

}

/**
 *  getShmVector方法按名称优查找已存在对象,对象不存在且itemlen为0时新增Vector对象。
 *  @param [in] shmObjName Vector对象名
 *  @param [in] itemlen Vector对象每条数据长度
 *  @param [in] capacity Vector对象缺省容量,为0时使用系统缺省容量
 *  @return CShmVector* 已存在或新建的Vector类型共享内存对象
 *  @pre \e 共享内存环境已打开
 *  @note 获取共享内存对象时,首先从映射表中查询,\n
 * 如果映射表中无此对象,使用CShmVectorFactory从共享内存环境管理区中查找。\n
 * 如果共享内存环境管理区中也无此对象,且传入参数的记录数据长度(itemlen)不为0,CShmVectorFactory在共享内存中新建对象.\n
 * 从共享内存环境管理区中取回或新建的内存对象会自动添加到共享内存对象映射表中。
 *  @remarks
 *  - 记录数据长度(itemlen)为0时,只查找已存在Vector,不会创建Vector.
 *  - 如果存在同名的内存对象,但类型不是Vector,返回值为空
 *  - 如果存在同名的Vector内存对象,由于不新建对象,传入的参数不做处理, 返回Vector的属性(例如itemlen)是已存在的Vector属性.
 *  @bug 2013/5/29 guoyue 添加数据记录范围限制
 */
CShmVector* CShmEnv::getShmVector(const char* shmObjName, shm_size_t itemlen,
        shm_size_t capacity)
{
    //guoyue 2013/5/29 添加数据记录范围限制
    //参数检查
    //if (shmObjName == NULL)
    if (shmObjName == NULL || itemlen >= shm_MAX_U32
            || capacity > (shm_MAX_U32 >> 1))
    {
        return NULL;
    }
    //查找已打开过的对象
    CShmObj* shmObj = findShmObj(shmObjName);
    //找到名字相同的对象了
    if (shmObj)
    {
        //检查类型对不对
        if (SHM_OBJ_VECTOR != shmObj->getShmObjType())
        {
            return NULL;
        }
        return (CShmVector*) shmObj;
    }

    //分配器有效性检查
    if (this->ptrChunk == NULL)
    {
        return NULL;
    }
    CChunk *chunkInst = (CChunk *) getChunkInst();

    CShmVectorFactory shmVectorFactory(chunkInst->getAlloctor());
    //使用工厂取出对象
    shmObj = shmVectorFactory.getShmObj(shmObjName, itemlen, capacity);

    //取出的对象加到对象映射表,以后再使用时就可直接取出
    if (NULL != shmObj)
    {
        addShmToMap(shmObjName, shmObj);
        return (CShmVector*) shmObj;
    }
    return NULL;
}


CShmMap* CShmEnv::getShmMap(const char* shmObjName, shm_size_t itemlen,
        shm_size_t capacity)
{
    //guoyue 2013/5/29 添加数据记录范围限制
    //参数检查
    //if (shmObjName == NULL)
    if (shmObjName == NULL || itemlen >= shm_MAX_U32
            || capacity > (shm_MAX_U32 >> 1))
    {
        return NULL;
    }
    //查找已打开过的对象
    CShmObj* shmObj = findShmObj(shmObjName);
    //找到名字相同的对象了
    if (shmObj)
    {
        //检查类型对不对
        if (SHM_OBJ_MAP != shmObj->getShmObjType())
        {
            return NULL;
        }
        return (CShmMap*) shmObj;
    }

    //分配器有效性检查
    if (this->ptrChunk == NULL)
    {
        return NULL;
    }
    CChunk *chunkInst = (CChunk *) getChunkInst();

    CShmMapFactory shmMapFactory(chunkInst->getAlloctor());
    //使用工厂取出对象
    shmObj = shmMapFactory.getShmObj(shmObjName, itemlen, capacity);

    //取出的对象加到对象映射表,以后再使用时就可直接取出
    if (NULL != shmObj)
    {
        addShmToMap(shmObjName, shmObj);
        return (CShmMap*) shmObj;
    }
    return NULL;
}

/**
 *  @param [in] shmObjName 内存对象对象名
 *  @return CShmObj* 查找到的共享内存对象
 */
CShmObj* CShmEnv::findShmObj(const char* shmObjName)
{
    CShmObjMap::iterator iter;
    CShmObj *shmObj = NULL;
    //查找共享内存对象
    iter = shmObjMap.find(shmObjName);
    if (shmObjMap.end() != iter)
    {
        shmObj = iter->second;
    }
    return shmObj;
}

/**
 *  释放共享内存对象映射表资源
 */
void CShmEnv::clearShmObj()
{
    CShmObj *shmObj = NULL;
    CShmObjMap::iterator iter;
    //逐个清理共享内存对象
    for (iter = shmObjMap.begin(); iter != shmObjMap.end(); iter++)
    {
        shmObj = iter->second;
        if (shmObj)
        {
            delete shmObj;
            shmObj = NULL;
        }
    }
    shmObjMap.clear();
}

/**
 *  @param [in] shmObjName 内存对象对象名称
 *  @param [in] shmObj 内存对象对象
 */
void CShmEnv::addShmToMap(const char* shmObjName, CShmObj *shmObj)
{
    shmObjMap.insert(pair<string, CShmObj *>(shmObjName, shmObj));
}

/**
 *  @param [out] envInfo 共享内存管理环境信息
 *  @return 查询执行情况
 *  - true 查询成功
 *  - false 查询失败
 *  @pre \e 共享内存环境已打开
 */
bool CShmEnv::getEnvInfo(ST_SHM_ENV_INFO &envInfo)
{
    //参数检查
    if (this->ptrChunk == NULL)
    {
        return false;
    }
    CChunk *ptrShmChunk = (CShmChunk *) this->ptrChunk;
    //取出对象字典
    IHash *shmDictHt = ptrShmChunk->getDictHt();
    //取出管理区
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) ptrShmChunk->getPtr(0);

    //取回共享内存管理环境信息
    envInfo.identity = string(ptrENV->shmIdentity);
    envInfo.version = string(ptrENV->version);
    envInfo.createDate = string(ptrENV->createDate);
    envInfo.keyId = ptrENV->keyId;
    envInfo.envZoneLen = ptrENV->shmEnvZoneLen;
    envInfo.envCapacity = ptrShmChunk->getChunkSize();
    envInfo.envSize = ptrShmChunk->getChunkUsed();
    envInfo.envFree = ptrShmChunk->getChunkFree();
    envInfo.objsSize = shmDictHt->getUsed();
    envInfo.objsCapacity = shmDictHt->getSize();
    return true;
}

/**
 *  @param [out] objInfoList 共享内存对象信息列表
 *  @return 查询执行情况
 *  - true 查询成功
 *  - false 查询失败
 *  @pre \e 共享内存环境已打开
 */
bool CShmEnv::getShmObjInfo(SHM_OBJ_INFO_LIST &objInfoList)
{
    //信息列表清理下先
    objInfoList.clear();
    //参数检查
    if (this->ptrChunk == NULL)
    {
        return false;
    }

    CChunk *ptrShmChunk = (CChunk *) this->ptrChunk;
    //取出对象字典
    IHash *shmDictHt = ptrShmChunk->getDictHt();

    //查字典
    DICT_ENTRY_LIST dictEntryList;
    if (!shmDictHt->list(dictEntryList))
    {
        return false;
    }

    //对象查询信息放入列表
    for (unsigned int i = 0; i < dictEntryList.size(); i++)
    {
        ST_SHM_OBJ_INFO objInfo;
        objInfo.shmObjName = string(dictEntryList[i].key);
        shm_offset_t shmObjOffset = dictEntryList[i].value;
        ST_SHM_OBJ_PROPERTY *ptrObjProperty =
                (ST_SHM_OBJ_PROPERTY *) ptrShmChunk->getPtr(shmObjOffset);
        objInfo.shmObjType = ptrObjProperty->shmObjType;
        objInfo.capacity = ptrObjProperty->size;
        objInfo.size = ptrObjProperty->used;
        objInfo.occupied = ptrObjProperty->occupied;
        objInfoList.push_back(objInfo);
    }

    return true;
}
