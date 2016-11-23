/**
 * @file    CShmTypes.h
 * @brief   类型定义头文件
 * @note
 *
 * 类型定义
 *
 * @author  guoyue
 * @version 1.0
 * @date    2013-3-1
 * @bug     无
 * @warning 无
 */
#ifndef CSHMTYPES_H_
#define CSHMTYPES_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CShmError.h"

#ifdef WIN32
#   undef  SHM_OS_WIN32
#   define SHM_OS_WIN32 1
#   ifdef WIN64
#       undef  SHM_64BIT
#       define SHM_64BIT 1
#   elif WIN32
#       undef  SHM_32BIT
#       define SHM_32BIT 1
#   else
#       error "Neither WIN32 nor WIN64 defined!"
#   endif
#else /* posix? */
#   undef  SHM_OS_POSIX
/** 定义SHM_OS_POSIX */
#   define SHM_OS_POSIX 1
#   if defined(__LP64__) || defined(__LP64) || __WORDSIZE==64
#       undef  SHM_64BIT
#       define SHM_64BIT 1
#   else
#       undef  SHM_32BIT
/** 定义SHM_OS_POSIX */
#       define SHM_32BIT 1
#   endif
#endif

/** @name typedefs for 32bit operating systems
 *  @{
 */
#ifdef SHM_32BIT
#   ifdef _MSC_VER
typedef signed __int64 shm_s64_t;
typedef unsigned __int64 shm_u64_t;
#   else
/** 32bit 定义类型shm_s64_t */
typedef signed long long shm_s64_t;
/** 32bit 定义类型shm_u64_t */
typedef unsigned long long shm_u64_t;
#   endif
/** 32bit 定义类型shm_s32_t */
typedef signed int shm_s32_t;
/** 32bit 定义类型shm_u32_t */
typedef unsigned int shm_u32_t;
/** 32bit 定义类型shm_s16_t */
typedef signed short shm_s16_t;
/** 32bit 定义类型shm_u16_t */
typedef unsigned short shm_u16_t;
/** 32bit 定义类型shm_s8_t */
typedef signed char shm_s8_t;
/** 32bit 定义类型shm_u8_t */
typedef unsigned char shm_u8_t;
#endif
/** @}*/ // typedefs for 32bit operating systems
/**
 * typedefs for 64bit operating systems; on Win64,
 * longs do not always have 64bit!
 */
/** @name typedefs for 64bit operating systems
 *  @{
 */
#ifdef SHM_64BIT
#   ifdef _MSC_VER
typedef signed __int64 shm_s64_t;
typedef unsigned __int64 shm_u64_t;
#   else
/** 64bit 定义类型shm_s64_t */
typedef signed long shm_s64_t;
/** 64bit 定义类型shm_u64_t */
typedef unsigned long shm_u64_t;
#   endif
/** 64bit 定义类型shm_s32_t */
typedef signed int shm_s32_t;
/** 64bit 定义类型shm_u32_t */
typedef unsigned int shm_u32_t;
/** 64bit 定义类型shm_s16_t */
typedef signed short shm_s16_t;
/** 64bit 定义类型shm_u16_t */
typedef unsigned short shm_u16_t;
/** 64bit 定义类型shm_s8_t */
typedef signed char shm_s8_t;
/** 64bit 定义类型shm_u8_t */
typedef unsigned char shm_u8_t;
#endif
/** @}*/ // typedefs for 64bit operating systems
/** 定义类型shm_status_t */
typedef int shm_status_t;
/** 定义类型shm_offset_t */
typedef shm_u64_t shm_offset_t;

#ifdef SHM_32BIT
/** 32bit 定义类型shm_size_t */
typedef shm_u32_t shm_size_t;
#endif

#ifdef SHM_64BIT
/** 64bit 定义类型shm_size_t */
typedef shm_u64_t shm_size_t;
#endif

#ifndef __key_t_defined
/** 定义类型__key_t */
typedef shm_s32_t __key_t;
/** 定义类型key_t */
typedef __key_t key_t;
/** 定义__key_t_defined */
#define __key_t_defined
#endif

/** 32bit 无符号整型最大数值*/
#define shm_MAX_U32         (~(shm_u32_t)0)

/** shm_size_t最大值*/
#define shm_MAX_SIZE_T      (~(shm_size_t)0)

/** 判断字符串是否相等 */
#ifndef STREQ
#define STREQ(x,y) (x == y || (x[0] == y[0] && strcmp(x,y) == 0))
#endif

/** 共享内存KEY魔数 */
#ifndef IPCKEY
#define IPCKEY (0x45678923)
#endif

#endif /* CSHMTYPES_H_ */
