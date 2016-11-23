/*
 * FileVector.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: level2
 */

#include "FileVector.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

FileVector::FileVector()
{
    // TODO Auto-generated constructor stub
    this->fid = -1;
    this->vector_size = 0;
    this->cur_pos = 0;
    this->pItem = 0;
    this->item_len = 0;
}

FileVector::~FileVector()
{
    // TODO Auto-generated destructor stub
    if (this->pItem != 0)
        delete pItem;
}
void FileVector::close()
{
    if (fid > 0)
        ::close(fid);
    if (this->pItem != 0)
        delete pItem;
    this->pItem = NULL;
    this->fid = -1;
    this->vector_size = 0;
    this->cur_pos = 0;
    this->pItem = 0;
    this->item_len = 0;
}
bool FileVector::create(const char *fileName, int item_len)
{
    if (item_len < 0)
        return false;
#ifdef SHM_OS_POSIX
    fid = ::open(fileName, O_RDWR | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
#else
    fid = ::open(fileName, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
#endif

    if (fid < 0)
        return false;

    this->item_len = item_len;
    this->pItem = new char[item_len];

    lseek(fid, 0, SEEK_SET);
    write(fid, &this->vector_size, sizeof(this->vector_size));
    write(fid, &this->item_len, sizeof(this->item_len));

    return true;
}

bool FileVector::open(const char *fileName)
{
    fid = ::open(fileName, O_RDWR);
    if (fid < 0)
        return false;

    lseek(fid, 0, SEEK_SET);
    read(fid, &this->vector_size, sizeof(this->vector_size));
    read(fid, &this->item_len, sizeof(this->item_len));

    if (this->pItem == 0)
        this->pItem = new char[item_len];
    return true;
}
bool FileVector::push()
{
    lseek(fid, sizeof(int) * 2 + this->cur_pos * this->item_len, SEEK_SET);
    write(fid, this->pItem, this->item_len);
    return true;
}

bool FileVector::push_back(char *item)
{
    lseek(fid, sizeof(int) * 2 + this->vector_size * this->item_len, SEEK_SET);
    write(fid, item, this->item_len);
    this->vector_size++;
    lseek(fid, 0, SEEK_SET);
    write(fid, &this->vector_size, sizeof(this->vector_size));
    return true;
}

char& FileVector::operator[](shm_size_t i)
{
    cur_pos = i;
    lseek(fid, sizeof(int) * 2 + i * this->item_len, SEEK_SET);
    read(fid, pItem, this->item_len);
    return *pItem;
}

shm_size_t FileVector::size()
{
    lseek(fid, 0, SEEK_SET);
    read(fid, &this->vector_size, sizeof(this->vector_size));
    return this->vector_size;
}
