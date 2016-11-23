/**
 * @file    CShmChunkReader.cpp
 * @brief   共享内存类型Chunk读写器实现
 * @note
 *
 * CShmChunkReader是基于共享内存的CChunkReader读写器。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <unistd.h>

#include "CShmChunkReader.h"
#include "CShmChunk.h"

#ifdef SHM_OS_POSIX
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

/**
 *  CShmChunkReader构造函数
 */
CShmChunkReader::CShmChunkReader(IChunk* ptrChunk)
        : CChunkReader(ptrChunk)
{
    // TODO Auto-generated constructor stub
    //mutex = NULL;
    file = NULL;
    fileNo = 0;
#ifdef SHM_OS_POSIX
    shmId = 0;
    pthread_mutex_init(&mutex, NULL);
#else
    handle = NULL;
#endif
}

/**
 *  CShmChunkReader析构函数
 */
CShmChunkReader::~CShmChunkReader()
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
 *  - 调用ftok产生key值
 *  - 调用shmget开辟一块共享内存
 *  - 调用shmat把共享内存映射到本应用
 *  @remarks
 *  create使用ftok产生key_t值.ftok将一个已存在的路径名和一个整数标识符转换成一个key_t
 *  值。 key_t的生成是以一个已存在的文件作为输入，并不是简单的字符串散列函数，必须真正存在某个文
 *  件， 才能将其位置传入ftok。\n
 *  ftok组合三个值来产生key：
 *  - targetName所在的文件系统的信息。
 *  - 文件在文件系统内的索引节点号。
 *  - IPCKEY的低序8位。
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmChunkReader::create(const char* targetName, shm_size_t maxSize)
{

    //参数检查
    if (NULL == targetName || 0 == maxSize)
    {
        return ERR_PARAM;
    }

    //CShmChunk *chunk = (CShmChunk *)getChunk();
    CShmChunk *pChunk = (CShmChunk *)this->getChunk();

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

#ifdef SHM_OS_POSIX
    //使用共享内存标识文件计算keyId
    key_t keyId = ftok(targetName, IPCKEY);
    if (-1 == keyId)
    {
        fclose(file);
        file = NULL;
        return ERR_FAILED;
    }
    pChunk->setKeyId(keyId);
    //取得shmId
    shmId = shmget(keyId, maxSize, 0666 | IPC_CREAT | IPC_EXCL);
    if (-1 == shmId)
    {
        fclose(file);
        file = NULL;
        return ERR_NOSPC;
    }

    shmFileName = targetName;

    //共享内存映射到本进程空间
    void *ptr = shmat(shmId, NULL, 0);

    if (-1 == *((int*) ptr))
    {
        fclose(file);
        file = NULL;
        return ERR_NOSPC;
    }
#else
    //本机是 SHM_OS_WIN
    if (handle == NULL)
    {
        handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                0, maxSize, targetName);
        if (handle == NULL)
        {
            fclose(file);
            file = NULL;
            return ERR_NOSPC;
        }
    }
    void *ptr = (void *) MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (ptr == NULL)
    {
        //UnmapViewOfFile(ptr);
        CloseHandle(handle);
        handle = NULL;
        return ERR_NOSPC;
    }
#endif

    fileNo = fileno(file);

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
 *  - 调用ftok产生key值
 *  - 调用shmget取得共享内存共享内存id
 *  - 调用shmat把共享内存映射到本应用
 *  @remarks
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmChunkReader::open(const char* targetName, shm_size_t maxSize)
{
    if (NULL == targetName)
    {
        return ERR_PARAM;
    }

    CShmChunk *pChunk = (CShmChunk *)this->getChunk();
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

#ifdef SHM_OS_POSIX
    //使用共享内存标识文件计算keyId
    key_t keyId = ftok(targetName, IPCKEY);

    if (-1 == keyId)
    {
        fclose(file);
        file = NULL;
        return ERR_FAILED;
    }
    pChunk->setKeyId(keyId);
    //取得shmId
    shmId = shmget(keyId, maxSize, 0666);
    if (-1 == shmId)
    {
        fclose(file);
        file = NULL;
        return ERR_INVAL;
    }
    else
    {
        shmFileName = targetName;
    }

    //共享内存映射到本进程空间
    void *ptr = shmat(shmId, NULL, 0);
    if (-1 == *((int*) ptr))
    {
        fclose(file);
        file = NULL;
        return ERR_NOSPC;
    }

#else
    //for test
    handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, targetName);
    if (handle == NULL)
    {
        fclose(file);
        file = NULL;
        return ERR_INVAL;
    }
    void *ptr = (void *) MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (ptr == NULL)
    {
        CloseHandle(handle);
        handle = NULL;
        fclose(file);
        file = NULL;
        return ERR_NOSPC;
    }
#endif
    //this->maxSize = ptrENV->shmSize;
    //this->mutex = &ptrENV->shmLock;
    fileNo = fileno(file);

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
 *  - 调用 shmdt禁止本进程使用这块共享内存
 *  @remarks 关闭读写器后,共享内存不会被移除,仍存在于系统中. targetName文件也不会被移除.
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmChunkReader::close()
{
    CShmChunk *pChunk = (CShmChunk *) getChunk();
    if (NULL != pChunk)
    {
        if (pChunk->getPtr(0))
        {
#ifdef SHM_OS_POSIX
            //断开内存映射
            if (-1 == shmdt(pChunk->getPtr(0)))
            {
                return ERR_FAILED;
            }
            shmId = -1;
#else
            if (handle)
            {
                UnmapViewOfFile(pChunk->getPtr(0));
                CloseHandle(handle);
                handle = NULL;
            }
#endif
        }
        pChunk->setBasePtr(NULL);
        if (file)
        {
            fclose(file);
            file = NULL;
            fileNo = 0;
        }
    }

    /*
     if(mutex)
     {
     mutex = NULL;
     }*/
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
 *  - 重新调用ftok产生key值
 *  - 重新调用shmget取得共享内存共享内存id
 *  - 调用shmctl删除共享内存
 *  @remarks
 *  - 删除共享内存成功后，targetName文件也会被移除。
 *  - 如果方法执行失败,也可用命令行ipcrm删除共享内存.
 *  @see 操作返回值可参考CShmError
 */
shm_status_t CShmChunkReader::remove()
{
    CShmChunk *pChunk = (CShmChunk *) getChunk();
    shm_size_t lastMaxSize = pChunk->getChunkSize();
    string fileName = shmFileName;

    //先关闭
    close();

    if (fileName.empty() || 0 == lastMaxSize)
    {
        return SUCCEED;
    }

    //确保文件存在
    createShmFile(fileName.c_str());
    fclose(file);
    file = NULL;

#ifdef SHM_OS_POSIX
    //重新调用ftok产生key值
    key_t keyId = ftok(fileName.c_str(), IPCKEY);

    if (-1 == keyId)
    {
        ::remove(fileName.c_str());
        fileName.clear();
        return SUCCEED;
    }

    //重新调用shmget取得共享内存共享内存id
    shmId = shmget(keyId, lastMaxSize, 0666);

    if (-1 == shmId)
    {
        ::remove(fileName.c_str());
        fileName.clear();
        return SUCCEED;
    }

    //调用shmctl删除共享内存
    int result = shmctl(shmId, IPC_RMID, 0);
    if (-1 == result)
    {
        ::remove(fileName.c_str());
        fileName.clear();

        return ERR_FAILED;
    }

    shmId = -1;
#else
    handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, fileName.c_str());
    if (handle != NULL)
    {
        void *ptr = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (ptr)
        {
            UnmapViewOfFile(ptr);
        }
        CloseHandle(handle);
        handle = NULL;
    }
#endif
    //清除文件
    ::remove(fileName.c_str());
    fileName.clear();
    shmFileName.clear();
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
shm_status_t CShmChunkReader::read(void* buff, shm_size_t count,
        shm_offset_t offset)
{
    CShmChunk *pChunk = (CShmChunk *) getChunk();
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
shm_status_t CShmChunkReader::write(const void* buff, shm_size_t count,
        shm_offset_t offset)
{
    CShmChunk *pChunk = (CShmChunk *) getChunk();
    if (pChunk)
    {
        char *ptr = (char *) pChunk->getPtr(offset);
        //写入内容
        memcpy(ptr, buff, count);
        return SUCCEED;
    }
    return ERR_FAILED;
}

void CShmChunkReader::flush(void *address, size_t length)
{
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
bool CShmChunkReader::createShmFile(const char *targetName)
{
    if (NULL != file)
    {
        return true;
    }

    file = fopen(targetName, "w+");

    if (NULL != file)
    {
        return true;
    }

    return false;
}

/**
 *  @param [in] targetName 文件的pathname
 *  @return
 *  - true 成功
 *  - false 失败
 *  @note 以可读写方式打开文件。
 *  @remarks 如果文件不存在，返回失败，不创建文件。
 */
bool CShmChunkReader::openShmFile(const char *targetName)
{
    if (NULL != file)
    {
        return true;
    }

    file = fopen(targetName, "r+");

    if (NULL != file)
    {
        return true;
    }

    return false;
}

void CShmChunkReader::lock()
{
#ifdef SHM_OS_POSIX
    if (this->fileNo)
    {
        //加锁
        //2015-10-14 guoyue修改。线程安全
        pthread_mutex_lock(&mutex);
        //2015-10-14 end of modify
        int count = 0;
        while(flock(fileNo, LOCK_EX) != 0 && count < 3)
        {
            usleep(100);
            count ++;
        }
    }
#endif
    return;
}

void CShmChunkReader::unLock()
{
#ifdef SHM_OS_POSIX
    if (this->fileNo)
    {
        //解锁
        flock(fileNo, LOCK_UN);
        //2015-10-14 guoyue修改。线程安全
        pthread_mutex_unlock(&mutex);
        //2015-10-14 end of modify
    }
#endif
    return;
}

