/**
 * @file    FileList.h
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

#ifndef FILELIST_H_
#define FILELIST_H_

#include "IList.h"
/**
 * @class FileList FileList.h "\source\dce\src\main\util\shm\FileList.h"
 * @brief FileList.
 * @note
 * FileList主要实现了常见的文件操作\n
 */
class FileList: public IList
{
public:
    FileList();
    virtual ~FileList();
    /** @brief 创建文件*/
    bool create(const char *fileName);
    /** @brief 打开文件*/
    bool open(const char *fileName);
    /** @brief 关闭文件*/
    void close();
    /** @brief 返回文件大小*/
    virtual shm_size_t size()
    {
        return 0;
    }
    /** @brief从文件中读取数据*/
    virtual bool pop(char* item, shm_size_t& itemLen);
    /** @brief写数据到文件*/
    virtual bool push_back(char* value, shm_size_t itemLen);
private:
    int fid;
    unsigned int pos;
};

#endif /* FILELIST_H_ */
