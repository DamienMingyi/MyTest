/**
 * @file    CMmapStaticAlloctor.cpp
 * @brief	CMmapStaticAlloctor分配器实现
 * @note
 *
 * 使用静态分配策略的共享内存分配器实现.
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */
#include "CMmapStaticAlloctor.h"
#include "CMmapChunk.h"

using namespace std;

/**
 * CMmapStaticAlloctor构造函数
 */
CMmapStaticAlloctor::CMmapStaticAlloctor(IChunk *ptrChunk)
        : CStaticAlloctor(ptrChunk)
{
    // TODO Auto-generated constructor stub

}
/**
 *  CMmapStaticAlloctor析构函数
 */
CMmapStaticAlloctor::~CMmapStaticAlloctor()
{
    // TODO Auto-generated destructor stub
}

char* CMmapStaticAlloctor::malloc(shm_size_t nbytes)
{

    CMmapChunk *ptrChunk = (CMmapChunk *)getChunk();

    if (NULL == ptrChunk || nbytes == 0)
    {
        return NULL;
    }
    shm_size_t used = ptrChunk->getChunkUsed();
    //检查内存是否够分配
    if ((used + nbytes) >= ptrChunk->getChunkSize())
    {
        //内存不够
        return NULL;
    }
    //计算指针
    char* ptr = (char*) ptrChunk->getPtr(used);
    //分配内存.从ptr后的nbytes被分配
    ptrChunk->setChunkUsed(used + nbytes);

    return ptr;
}

char* CMmapStaticAlloctor::calloc(shm_size_t nbytes, char initial_value)
{
    char* ptr = this->malloc(nbytes);
    if (ptr)
    {
        memset(ptr, initial_value, nbytes);
        return ptr;
    }
    return NULL;
}

char* CMmapStaticAlloctor::calloc(shm_size_t n_elem, shm_size_t elem_size,
        char initial_value)
{
    return this->calloc((n_elem * elem_size), initial_value);
}

void CMmapStaticAlloctor::free(char* ptr)
{
    //不实现
    return;
}

shm_status_t CMmapStaticAlloctor::realse(void)
{
    //不实现
    return 0;
}
