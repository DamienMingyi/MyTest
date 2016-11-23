/*
 * FileVector.h
 *
 *  Created on: Mar 13, 2013
 *      Author: level2
 */

#ifndef FILEVECTOR_H_
#define FILEVECTOR_H_

#include "IVector.h"

class FileVector: public IVector
{
public:
    FileVector();
    virtual ~FileVector();

    bool create(const char *fileName, int item_len);
    bool open(const char *fileName);
    void close();
    virtual bool push();
    virtual bool push_back(char *item);
    virtual shm_size_t size();
    virtual char& operator[](shm_size_t i);

private:
    int fid;
    int item_len;
    int cur_pos;
    int vector_size;
    char* pItem;
};

#endif /* FILEVECTOR_H_ */
