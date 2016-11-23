/**
 * @file    HashTable.h
 * @brief   哈希表头文件
 * @note    哈希表及其操作。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2015-12-14
 * @bug     无
 * @warning 无
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef UTILITY_HASH_HASHTABLE_H_
#define UTILITY_HASH_HASHTABLE_H_

//#if defined (__cplusplus) || defined (c_plusplus)
//extern "C"
//{
//#endif

/**
 * 取哈希表缺省最小长度
 */
#define MINHASH_SIZE           (73)

#ifndef STREQ
#define STREQ(x,y) (x == y || (x[0] == y[0] && strcmp(x,y) == 0))
#endif

/**
 * 取最小值定义
 */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/**
 * 取最大值定义
 */
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/**
 * BOOL 类型定义
 */
#ifndef  BOOL
#define BOOL                        int
#endif

#ifndef  TRUE
#define TRUE                        (1)
#endif

#ifndef  FALSE
#define FALSE                       (0)
#endif

#ifndef  OFFSET
#define OFFSET(s, m)   ((size_t) &((s *) 0)->m)
#endif

/**
 *  哈希表节点类型
 */
typedef struct HTableInfoTag
{
    char *key;                  ///< lookup key
    char *value;                ///< associated value
    struct HTableInfoTag *next; ///< colliding entry
    struct HTableInfoTag *prev; ///< colliding entry
} HTableInfo_ST;

/**
 *  Structure of one hash table.
 */
typedef struct HTableTag
{
    unsigned int size;          ///< length of entries array
    unsigned int used;          ///< number of entries in table
    HTableInfo_ST **data;       ///< entries array, auto-resized
} HTable_ST;

typedef enum Data_Type
{
    DATATYPE_CHAR = 0,  ///<char
    DATATYPE_CHARP,     ///<char*
    DATATYPE_STR,       ///<char str[n]
    DATATYPE_BOOL,      ///<bool
    DATATYPE_INT32,     ///<int
    DATATYPE_INT64,     ///<int64_t
    DATATYPE_DOUBLE,    ///<float,double
    DATATYPE_STRUCT,    ///<struct
    DATATYPE_CONTAINER  ///<stl
} DATA_TYPE_ENUM;

/** @brief 对象缺省字串格式化输出回调函数*/
typedef int (*ObjectFormatFunc)(char*, size_t, char* , char*, int*);

/** @brief 对象缺省字串格式化输出回调函数*/
int default_format_func(char *buff, size_t bufflen, char* colDesc, char *pData, int *rc = NULL);

//int calc_format_func(char *buff, size_t bufflen, char* colDescWithRule, char *pData, int *rc = NULL);

/*
 * 对象序列化
 * param:
 *     buff           :  destination buffer
 *     bufflen        :  size of buffer
 *     retCalc        :  typing format handler
 *     pData          :  object's address
 *     rc             :  err code
 * return:
 *     return the number of characters printed
*/
//int object_snprintf(char *buff, size_t bufflen, char* calcRule, void *pData, int *rc=NULL);

/**
 * @brief 类型 定义COL_DESC_ST
 * @note 列类型描述
 */
typedef struct ColDescriptionStruct
{
    const char* key;        ///< 成员名称
    DATA_TYPE_ENUM dataType;///< 类型名称
    size_t offset;          ///< 成员偏移
    int plevel;             ///< 指针层级 ,0:非指针或char*;1:有1层指针或char**;n:有n层指针或char*(*n*)
    const char* structName; ///< 结构名称 ,dataType=DATATYPE_STRUCT 时表示类型名称 ; dataType=DATATYPE_CONTAINER 时表示容器内元素的名称
    ObjectFormatFunc formatFunc; ///< 对象格式化打印函数(dataType=DATATYPE_CONTAINER 时有效)
    const char* calcRule;   ///<计算规则
    char* params;           ///<参数
} COL_DESC_ST;

/**
 * @brief 类型 定义CALC_RULE_PARAM_ST
 * @note 参数类型描述
 */
typedef struct CalcRuleParam
{
    int argc;
    char **argv;
}CALC_PARAM_ST;

/**
 * @brief 类型 定义SCHEMA_DESC_ST
 * @note 表类型描述
 */
typedef struct SchemaDescStruct
{
    const char* name;       ///< 结构体名称
    COL_DESC_ST* stDesc;    ///< 表
    size_t nColCount;       ///< 表中列的个数
} SCHEMA_DESC_ST;

/** @brief 哈希表创建*/
void HTableCreate(HTable_ST **hTable, int size = MINHASH_SIZE);

/** @brief 哈希表元素入队*/
void HTableEnter(HTable_ST *table, const char *key, char *value,
        HTableInfo_ST **tableInfo);

/** @brief 哈希表元素查找*/
BOOL HTableFind(HTable_ST *table, const char *key, char **value);

/** @brief 哈希表元素定位*/
void HTableLocate(HTable_ST *table, const char *key, HTableInfo_ST **tableInfo);

/** @brief 哈希表元素列表*/
void HTableList(HTable_ST *table, HTableInfo_ST ***tableInfo);

/** @brief 哈希表元素删除*/
void HtableDelete(HTable_ST *table, const char *key, void (*free_fn)(char *));

/** @brief 哈希表释放*/
void HTableFree(HTable_ST *table, void (*free_fn)(char *));

//#if defined (__cplusplus) || defined (c_plusplus)
//}
//#endif /* __cplusplus */

#endif /* UTILITY_HASH_HASHTABLE_H_ */
