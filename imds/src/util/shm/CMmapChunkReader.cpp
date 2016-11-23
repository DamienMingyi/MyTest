/**
 * @file    CMmapChunkReader.cpp
 * @brief   内存映射文件类型Chunk读写器实现
 * @note
 *
 * CMmapChunkReader是基于内存映射文件的CChunkReader读写器。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "CMmapChunkReader.h"
#include "CMmapChunk.h"

#ifdef SHM_OS_POSIX
#include <sys/ipc.h>
#include <sys/mman.h>
#endif

/**
 *  CMmapChunkReader构造函数
 */
CMmapChunkReader::CMmapChunkReader(IChunk* ptrChunk)
        : CChunkReader(ptrChunk)
{
    // TODO Auto-generated constructor stub
    fileLength = 0;
    shmId = 0;
#ifdef SHM_OS_POSIX
    pthread_mutex_init(&mutex,NULL);
#endif
}

/**
 *  CMmapChunkReader析构函数
 */
CMmapChunkReader::~CMmapChunkReader()
{
    // TODO Auto-generated destructor stub
}

/**
 *  create方法首先根据传入参数向系统申请共享内存。申请成功后构造出一个Chunk并把共享内存放入其内，作为读写器的操作对象。
 *  @param [in] targetName 共享内存标识文件路径与文件名
 *  @param [in] maxSize 申请共享内存大小
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 系统可供申请的共享内存大于maxSize；targetName是真正存在的文件.如果不存在,create会创建。
 *  @note create方法申请共享内存顺序如下:
 *  - 根据targetName创建maxSize大小的文件
 *  - 调用mmap把文件映射进内存
 *  @remarks
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CMmapChunkReader::create(const char* targetName,
        shm_size_t maxSize)
{

    //参数检查
    if (NULL == targetName || 0 == maxSize)
    {
        return ERR_PARAM;
    }

    CMmapChunk *pChunk = (CMmapChunk *) this->getChunk();

    if (NULL == pChunk)
    {
        return ERR_INVAL;
    }

    //检查是否Chunk可用
    if (NULL != pChunk->getPtr(0))
    {
        return ERR_EXIST;
    }

    //新建共享内存标识文件
    if (!createShmFile(targetName))
    {
        return ERR_FAILED;
    }

    if(-1 == ::lseek(shmId, maxSize-1, SEEK_SET))
    {
        ::close(shmId);
        shmId = 0;
        return ERR_NOSPC;
    }
    ::write(shmId,"\0",1);
    ::lseek(shmId, 0, SEEK_SET);

    shmFileName = targetName;
    this->fileLength = maxSize;

    void *ptr = NULL;
#ifdef SHM_OS_POSIX
    //共享内存映射到本进程空间
    //使用共享内存标识文件计算keyId
    key_t keyId = ftok(targetName, IPCKEY);
    if (-1 == keyId)
    {
    	 ::close(shmId);
    	  shmId = 0;
        return ERR_FAILED;
    }
    pChunk->setKeyId(keyId);

#ifdef SHM_64BIT
    ptr = mmap64(NULL, this->fileLength, PROT_READ | PROT_WRITE, MAP_SHARED, shmId, 0);
#else
    ptr = mmap(NULL, this->fileLength, PROT_READ | PROT_WRITE, MAP_SHARED, shmId, 0);
#endif

    if (MAP_FAILED ==  ptr)
    {
        ::close(shmId);
        shmId = 0;
        return ERR_INVAL;
    }
#else
    if (NULL ==  ptr)
    {
        ::close(shmId);
        shmId = 0;
        return ERR_INVAL;
    }
#endif
    //构造CShmChunk，供CShmStaticAlloctor从Chunk中分配内存时使用
    //CShmChunk *pChunk = new CShmChunk(ptr);
    pChunk->setBasePtr(ptr);
    pChunk->setChunkSize(maxSize);
    pChunk->setChunkUsed(0);
    //setChunk(pChunk);
    return SUCCEED;
}

/**
 *  open方法打开系统已事先创建好的共享内存。打开后构造出一个Chunk并把共享内存放入其内，作为读写器的操作对象。
 *  @param [in] targetName 共享内存标识文件路径与文件名
 *  @param [in] maxSize 申请共享内存大小，设置为0表示打开的共享内存空间与创建时同样大小
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e targetName是已创建好实际存在的文件。否则open调用时返回失败。
 *  @note open方法打开已存在的共享内存顺序如下:
 *  - 打开已存在的文件
 *  - 调用mmap把文件映射进内存
 *  @remarks
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CMmapChunkReader::open(const char* targetName, shm_size_t maxSize)
{
    if (NULL == targetName)
    {
        return ERR_PARAM;
    }

    CMmapChunk *pChunk = (CMmapChunk *) this->getChunk();
    if (NULL == pChunk)
    {
        return ERR_INVAL;
    }

    //检查是否Chunk可用
    if (NULL != pChunk->getPtr(0))
    {
        return ERR_EXIST;
    }

    //打开共享内存标识文件
    if (!openShmFile(targetName))
    {
        return ERR_NOENT;
    }

    void *ptr = NULL;
#ifdef SHM_OS_POSIX
    //使用共享内存标识文件计算keyId
    key_t keyId = ftok(targetName, IPCKEY);
    if (-1 == keyId)
    {
    	::close(shmId);
    	shmId = 0;
        return ERR_FAILED;
    }
    pChunk->setKeyId(keyId);
#ifdef SHM_64BIT
    ptr = mmap64(NULL, fileLength, PROT_READ | PROT_WRITE, MAP_SHARED, shmId, 0);
#else
    ptr = mmap(NULL, fileLength, PROT_READ | PROT_WRITE, MAP_SHARED, shmId, 0);
#endif
    if (ptr == MAP_FAILED )
    {
        ::close(shmId);
        shmId = 0;
        return ERR_INVAL;
    }
#else
    if (ptr == NULL)
    {
        ::close(shmId);
        shmId = 0;
        return ERR_INVAL;
    }
#endif

    shmFileName = targetName;

    if (pChunk == NULL)
    {
        close();
        return ERR_NOMEM;
    }
    //把共享内存装进去
    pChunk->setBasePtr(ptr);

    return SUCCEED;
}

/**
 *  close方法关闭读写器. 关闭过程首先脱接共享内存共享内存, 销毁共享内存对象字典表管理器, 最后销毁Chunk。
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 共享内存已创建或打开。
 *  @note close脱接共享内存顺序如下:
 *  - 判断指向共享内存的指针是否有效
 *  - 调用munmap解除映射关系,禁止本进程使用这块共享内存
 *  @remarks 关闭读写器后,共享内存不会被移除,仍存在于系统中. targetName文件也不会被移除.
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CMmapChunkReader::close()
{
    CMmapChunk *pChunk = (CMmapChunk *) getChunk();
    if (NULL != pChunk)
    {
        if (pChunk->getPtr(0))
        {
#ifdef SHM_OS_POSIX
            msync( pChunk->getPtr(0), this->fileLength, MS_ASYNC);
            //断开内存映射
            if (-1 == munmap(pChunk->getPtr(0), this->fileLength))
            {
                return ERR_FAILED;
            }
#endif
            pChunk->setBasePtr(NULL);
            if (shmId)
            {
                ::close(shmId);
                shmId = 0;
            }
        }
    }

   return SUCCEED;
}

/**
 *  remove方法除了关闭读写器,还会删除共享内存与共享内存标识文件。
 *  @return shm_status_t 操作返回值.
 *  - 返回值为0时表示操作成功
 *  - 返回值为负值时表示操作失败
 *  @pre \e 共享内存存在于系统且targetName文件存在。
 *  @note 移除共享内存顺序如下:
 *  - 调用close关闭读写器
 *  - 删除文件
 *  @remarks
 *  - 进程之间通过映射同一个普通文件实现共享.删除文件相当于删除了共享内存。
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CMmapChunkReader::remove()
{
    //先关闭
    close();

    if ( 0 == this->fileLength)
    {
        return SUCCEED;
    }
    ::remove(shmFileName.c_str());
    shmFileName.clear();
    this->fileLength = 0;
    return SUCCEED;
}

/**
 *  @param [out] buff 应用缓存
 *  @param [in] count 读出数据长度，以字节计算。
 *  @param [in] offset 读出数据相对于共享内存首地址的偏移。
 *  @return shm_status_t 操作返回值.
 *  - 返回值为SUCCEED时表示操作成功
 *  - 返回值为ERR_FAILED时表示操作失败
 *  @pre \e targetName是已创建好实际存在的文件。否则open调用时返回失败。
 *  @note
 *  @remarks
 */
shm_status_t CMmapChunkReader::read(void* buff, shm_size_t count,
        shm_offset_t offset)
{
    CMmapChunk *pChunk = (CMmapChunk *) getChunk();
    if (pChunk)
    {
        char *ptr = (char *) pChunk->getPtr(offset);
        //读出内容
        memcpy(buff, ptr, count);
        return SUCCEED;
    }
    return ERR_FAILED;
}

/**
 *  @param [out] buff 应用缓存
 *  @param [in] count 写出数据长度，以字节计算。
 *  @param [in] offset 写出数据相对于共享内存首地址的偏移。
 *  @return shm_status_t 操作返回值.
 *  - 返回值为SUCCEED时表示操作成功
 *  - 返回值为ERR_FAILED时表示操作失败
 *  @pre \e targetName是已创建好实际存在的文件。否则open调用时返回失败。
 *  @note
 *  @remarks
 */
shm_status_t CMmapChunkReader::write(const void* buff, shm_size_t count,
        shm_offset_t offset)
{
    CMmapChunk *pChunk = (CMmapChunk *) getChunk();
    if (pChunk)
    {
        char *ptr = (char *) pChunk->getPtr(offset);
        //写入内容
        memcpy(ptr, buff, count);
#ifdef SHM_OS_POSIX
        msync( ptr, count, MS_ASYNC);
#endif
        return SUCCEED;
    }
    return ERR_FAILED;
}

void CMmapChunkReader::flush(void *address, size_t length)
{
    CMmapChunk *pChunk = (CMmapChunk *) getChunk();
    if (pChunk)
    {
        if (pChunk->getOffset((char *)address + length) < pChunk->getChunkSize()
                && pChunk->getOffset(address) > 0)
        {
#ifdef SHM_OS_POSIX
            msync( address, length, MS_ASYNC);
#endif
        }
    }
    return;
}

/**
 *  @param [in] targetName 文件的pathname
 *  @return
 *  - true 成功
 *  - false 失败
 *  @note 以可读写方式打开文件。
 *  @remarks 如果文件不存在，创建文件
 */
bool CMmapChunkReader::createShmFile(const char *targetName)
{
    if (shmId > 0)
    {
        return true;
    }

    //file = fopen(targetName, "r+");

#ifdef SHM_OS_POSIX
    shmId = ::open(targetName, O_RDWR | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
#else
    shmId = ::open(targetName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
#endif

    if (-1 == shmId)
    {
        return false;
    }

    return true;
}

/**
 *  @param [in] targetName 文件的pathname
 *  @return
 *  - true 成功
 *  - false 失败
 *  @note 以可读写方式打开文件。
 *  @remarks 如果文件不存在，返回失败，不创建文件。
 */
bool CMmapChunkReader::openShmFile(const char *targetName)
{
    if (shmId > 0)
    {
        return true;
    }

#ifdef SHM_OS_POSIX
    shmId = ::open(targetName, O_RDWR | O_EXCL ,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
#else
    shmId = ::open(targetName, O_RDWR | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR);
#endif

    if (-1 == shmId)
    {
        return false;
    }

    struct stat filestat;
    if((fstat(shmId, &filestat)) != 0)
    {
        ::close(shmId);
        shmId = 0;
        return false;
    }
    this->fileLength = filestat.st_size;

    return true;
}

void CMmapChunkReader::lock()
{
#ifdef SHM_OS_POSIX
    if (this->shmId)
    {
        //加锁
        //2015-10-14 guoyue修改。线程安全
        pthread_mutex_lock(&mutex);
        //2015-10-14 end of modify
        int count = 0;
        while (flock(shmId, LOCK_EX) != 0 && count < 3)
        {
            usleep(100);
            count ++;
        }
    }
#endif
    return;
}

void CMmapChunkReader::unLock()
{
    /*
     if(this->mutex)
     {
     pthread_mutex_unlock(mutex);
     }*/
#ifdef SHM_OS_POSIX
    if (this->shmId)
    {
        //解锁
        flock(shmId, LOCK_UN);
        //2015-10-14 guoyue修改。线程安全
        pthread_mutex_unlock(&mutex);
        //2015-10-14 end of modify
    }
#endif
    return;
}

