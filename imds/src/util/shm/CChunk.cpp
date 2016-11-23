/**
 * @file    CChunk.cpp
 * @brief   CChunk是IChunk的扩展
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


#include "CChunk.h"
/**
 *  CChunk构造函数
 */
CChunk::CChunk()
{
    // TODO Auto-generated constructor stub
    this->ptrReader = NULL;
    this->ptrDictHt = NULL;
    this->ptrAlloctor = NULL;
    this->key_id = 0;
}
/**
 *  CChunk析构函数
 */
CChunk::~CChunk()
{
    // TODO Auto-generated destructor stub
}

/**
 *  @return 资源读写器
 *  @note
 *  @remarks
 */
IReader *CChunk::getReader()
{
    return this->ptrReader;
}

/**
 *  @return 内存对象字典
 *  @note
 *  @remarks
 */
IHash *CChunk::getDictHt()
{
    return this->ptrDictHt;
}

/**
 *  @return 资源分配器
 *  @note
 *  @remarks
 */
IAlloctor *CChunk::getAlloctor()
{
    return this->ptrAlloctor;
}

void CChunk::setKeyId(key_t keyId)
{
    this->key_id = keyId;
}

/**
 *  @param [in] shmSize Chunk长度
 *  @return 初始化后的内存环境管理区指针\n
 *  @note
 *  @remarks
 */
ST_SHM_ENV* CChunk::buildEnv(shm_size_t shmSize)
{
    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) this->getPtr(0);
    if (ptrENV == NULL)
    {
        return NULL;
    }
    memset(ptrENV, 0, sizeof(ST_SHM_ENV));
    strcpy((char *) ptrENV->shmIdentity, SHM_IDT);
    strcpy((char *) ptrENV->version, SHM_VER);
    time_t tNow = time(NULL);
    struct tm* pTm = localtime(&tNow);
    sprintf((char *) ptrENV->createDate, "%04d%02d%02d%02d%02d%02d",
            pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
            pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
    ptrENV->keyId = this->key_id;
    ptrENV->shmEnvZoneLen = sizeof(ST_SHM_ENV);
    ptrENV->shmDataZone = '*';
    ptrENV->shmSize = shmSize;
    ptrENV->shmUsed = sizeof(ST_SHM_ENV);
    return ptrENV;
}

/**
 *  @return 共享内存环境管理区标识校验结果
 *  - true 通过校验\n
 *  - false 未通过校验
 *  @note
 *  @remarks
 */
bool CChunk::checkIdentity()
{
    char identity[SHM_IDTLEN] = SHM_IDT;
    char version[SHM_VERLEN] = SHM_VER;

    ST_SHM_ENV *ptrENV = (ST_SHM_ENV *) this->getPtr(0);

    if (ptrENV == NULL)
    {
        return false;
    }

    //检查共享内存管理区标识
    if (STREQ(ptrENV->shmIdentity, identity) && STREQ(ptrENV->version, version)
            && ptrENV->keyId == this->key_id)
    {
        return true;
    }
    return false;
}
