/**
 * @file    CMmapChunkReader.h
 * @brief   内存映射文件类型Chunk读写器头文件
 * @note
 *
 * CMmapChunkReader是基于内存映射文件的CChunkReader读写器实现。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-5
 * @bug     无
 * @warning 无
 */
#ifndef CMMAPCHUNKREADER_H_
#define CMMAPCHUNKREADER_H_

#include <string>
#include "CChunkReader.h"

#ifndef SHM_OS_POSIX
#include <windows.h>
#endif

using namespace std;

/**
 * @class CMmapChunkReader CMmapChunkReader.h "\source\dce\src\main\util\shm\CMmapChunkReader.h"
 * @brief 内存映射文件类型Chunk读写器.
 * @note
 * CMmapChunkReader是基于内存映射文件的CChunkReader读写器实现。
 * CMmapChunkReader的操作对象是内存映射文件内存块,当调用create方法时向系统申请.\n
 * 内存映射文件共享内存使用按下列顺序进行:
 *  - 根据targetName创建maxSize大小的文件
 *  - 调用mmap把文件映射进内存
 *  - 调用munmap解除映射关系,禁止本进程使用这块共享内存
 *  - 删除文件
 */
class CMmapChunkReader: public CChunkReader
{
public:
    /** @brief CMmapChunkReader构造函数*/
    CMmapChunkReader(IChunk* ptrChunk);
    /** @brief CMmapChunkReader析构函数*/
    virtual ~CMmapChunkReader();

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
    shm_size_t fileLength;
    string shmFileName; ///<共享内存标识文件名
    int shmId; ///<文件描述符
#ifdef SHM_OS_POSIX
    pthread_mutex_t mutex; ///<互斥量
#endif
};

#endif /* CMMAPCHUNKREADER_H_ */
