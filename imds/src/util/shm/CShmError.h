/**
 * @file    CShmError.h
 * @brief   shm_status_t类型返回值头文件
 * @note
 *
 * shm_status_t类型返回值可能的取值
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-8
 * @bug     无
 * @warning 无
 */
#ifndef CSHMERROR_H_
#define CSHMERROR_H_

#include <errno.h>

#define SUCCEED      (0)    ///< 操作成功
#define ERR_FAILED   (-1)   ///< 操作失败
#define ERR_PARAM    (-2)   ///< 无效参数

#define ERR_INT      (-10)  ///< 调用时中断
#define ERR_AGAIN    (-11)  ///< 没有准备就绪,稍后再试
#define ERR_NXIO     (-12)  ///< 指定设备无效
#define ERR_IO       (-13)  ///< I/O错误

#define ERR_DIR      (-20)  ///< 无效目录
#define ERR_BADF     (-21)  ///< 无效文件
#define ERR_ISDIR    (-22)  ///< 不是文件而是目录
#define ERR_NOENT    (-23)  ///< 指定文件或目录不存在
#define ERR_NODIR    (-24)  ///< 不是目录

#define ERR_NOMEM    (-30)  ///< 内存不足
#define ERR_NOSPC    (-31)  ///< 无可用空间
#define ERR_FAULT    (-32)  ///< 地址错误,属性与传入参数不匹配
#define ERR_OVERFLOW (-33)  ///< 溢出(越界)
#define ERR_ACCESS   (-34)  ///< 禁止访问
#define ERR_BADPTR   (-35)  ///< 无效指针

#define ERR_NTL      (-40)  ///< 长度太长
#define ERR_INVAL    (-41)  ///< 引用对象无效
#define ERR_FBIG     (-42)  ///< 对象数目超出限制
#define ERR_OPENED   (-43)  ///< 打开指定对象错误
#define ERR_ROFS     (-44)  ///< 对象只读
#define ERR_TXTBSY   (-45)  ///< 对象正在使用中
#define ERR_EXIST    (-46)  ///< 对象已存在
#define ERR_SEEK     (-47)  ///< 没有找到指定记录
#define ERR_PERM     (-48)  ///< 操作被禁止

#endif /* CSHMERROR_H_ */
