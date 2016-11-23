
/**
 * @file    FileList.cpp
 * @brief   共享内存类型FileList接口头文件
 * @note
 *
 * FileList是IList子类,实现了IList接口功能\n
 *
 * @author  level2
 * @version 1.0
 * @date    2013-3-14
 * @bug     无
 * @warning 无
 */

#include "FileList.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
/**
 *  FileList构造函数
 */
FileList::FileList()
{
    // TODO Auto-generated constructor stub
    this->fid = -1;
    this->pos = 0;
}

/**
 *  FileList析构函数
 */
FileList::~FileList()
{
    // TODO Auto-generated destructor stub
}

/**
 *  @param [in] fileName 文件名
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool FileList::create(const char *fileName)
{
#ifdef SHM_OS_POSIX
    fid = ::open(fileName, O_RDWR | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
#else
    fid = ::open(fileName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
#endif
    if (fid < 0)
        return false;
    shm_size_t len = 0;
    write(fid, &len, sizeof(shm_size_t));
    return true;
}

/**
 *  @param [in] fileName 文件名
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool FileList::open(const char *fileName)
{
    fid = ::open(fileName, O_RDWR);
    if (fid < 0)
        return false;
    return true;
}

/**
 *  @return 操作结果
 */
void FileList::close()
{
    if (this->fid > 0)
        ::close(this->fid);
}

/**
 *  @param [in] item 文件中读出的数据， itemLen  文件件中读出数据的长度
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool FileList::pop(char* item, shm_size_t& itemLen)
{
    shm_size_t len = 0;
    lseek(fid, 0, SEEK_SET);
    read(fid, &len, sizeof(shm_size_t));
    if (len - this->pos > sizeof(shm_size_t))
    {
        lseek(fid, this->pos + sizeof(shm_size_t), SEEK_SET);
        read(fid, &itemLen, sizeof(shm_size_t));
        read(fid, item, itemLen);
        this->pos += sizeof(shm_size_t);
        this->pos += itemLen;
        return true;
    }
    return false;
}


/**
 *  @param [in] value 写入文件的数据， itemLen  写入文年的数据长度
 *  @return 操作结果
 *  - true 成功
 *  - false 失败
 */
bool FileList::push_back(char* value, shm_size_t itemLen)
{
    shm_size_t len = 0;
    lseek(fid, 0, SEEK_SET);
    read(fid, &len, sizeof(shm_size_t));
    lseek(fid, len + sizeof(shm_size_t), SEEK_SET);
    write(fid, &itemLen, sizeof(itemLen));
    write(fid, value, itemLen);
    len += sizeof(shm_size_t);
    len += itemLen;
    lseek(fid, 0, SEEK_SET);
    write(fid, &len, sizeof(shm_size_t));
    return true;
}
