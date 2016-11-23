/**
 * @file    CShmChunkReader.h
 * @brief   共享内存类型Chunk读写器头文件
 * @note
 *
 * CShmChunkReader是基于共享内存的CChunkReader读写器实现。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef CSHMCHUNKREADER_H_
#define CSHMCHUNKREADER_H_

#include <string>
#include "CChunkReader.h"

#ifndef SHM_OS_POSIX
#include <windows.h>
#endif

using namespace std;

/**
 * @class CShmChunkReader CShmChunkReader.h "\source\dce\src\main\util\shm\CShmChunkReader.h"
 * @brief 共享内存类型Chunk读写器.
 * @note
 * CShmChunkReader是基于共享内存的CChunkReader读写器实现。
 * CShmChunkReader的操作对象是共享内存块,当调用create方法时向系统申请.\n
 * 共享内存使用按下列顺序进行:
 *  - 调用ftok产生key值
 *  - 调用shmget开辟一块共享内存
 *  - 调用shmat把共享内存映射到本应用
 *  - 共享内存写/读操作
 *  - 调用 shmdt禁止本进程使用这块共享内存
 *  - 删除这块共享内存 shmctl()或者命令行ipcrm
 */
class CShmChunkReader: public CChunkReader
{
public:
    /** @brief CShmChunkReader构造函数*/
    CShmChunkReader(IChunk* ptrChunk);
    /** @brief CShmChunkReader析构函数*/
    virtual ~CShmChunkReader();

    /** @brief 新建读写器，创建共享内存Chunk*/
    virtual shm_status_t create(const char* targetName, shm_size_t maxSize);

    /** @brief 打开读写器，创建共享内存Chunk*/
    virtual shm_status_t open(const char* targetName, shm_size_t maxSize = 0);

    /** @brief 关闭读写器，关闭共享内存Chunk*/
    virtual shm_status_t close();

    /** @brief 关闭读写器，移除共享内存Chunk中的资源。*/
    virtual shm_status_t remove();

    /** @brief 从共享内存读写器读出n个字节到应用缓存*/
    virtual shm_status_t read(void* buff, shm_size_t count, shm_offset_t offset);

    /** @brief 从应用缓存写n个字节到共享内存读写器*/
    virtual shm_status_t write(const void* buff, shm_size_t count, shm_offset_t offset);

    virtual void flush(void *address, size_t length);

    /** @brief 共享内存读写器加锁*/
    void lock();
    /** @brief 共享内存读写器解锁*/
    void unLock();
private:
    /** @brief 创建共享内存标识文件*/
    bool createShmFile(const char* targetName);
    /** @brief 打开共享内存标识文件*/
    bool openShmFile(const char* targetName);
private:
    FILE* file; ///<共享内存标识文件指针
    string shmFileName; ///<共享内存标识文件名
    int fileNo; ///<文件描述符
#ifdef SHM_OS_POSIX
    int shmId; ///<共享内存标识符
    pthread_mutex_t mutex; ///<互斥量
#else
    HANDLE handle; ///<共享内存标识符
#endif
};

#endif /* CSHMCHUNKREADER_H_ */
