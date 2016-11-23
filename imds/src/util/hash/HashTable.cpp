/**
 * @file    HashTable.c
 * @brief   哈希表实现
 * @note    哈希表及其操作。\n
 *
 * @author  guoyue
 * @version 1.0
 * @date    2015-12-14
 * @bug     无
 * @warning 无
 */
#include "HashTable.h"

/**
 * @brief 哈希值计算
 * @param [in] key key
 * @param [in] size 哈希表长度
 * @return unsigned 哈希值
 */
static unsigned HTableHash(const char *key, unsigned size)
{
    unsigned long h = 0;
    unsigned long g;

    while (*key)
    {
        h = (h << 4) + *key++;
        if (0 != (g = (h & 0xf0000000)))
        {
            h ^= (g >> 24);
            h ^= g;
        }
    }
    return (h % size);
}

/**
 * @brief 节点链入哈希表
 * @param [in] table 哈希表
 * @param [in] element 节点
 * @return 无
 */
static void HTableLink(HTable_ST *table, HTableInfo_ST *element)
{
    HTableInfo_ST **h = NULL;
    h = table->data + HTableHash(element->key, table->size);
    element->prev = 0;
    if (0 != (element->next = *h))
    {
        (*h)->prev = element;
    }
    *h = element;
    table->used++;
}

/**
 * @brief 分配哈希表节点空间
 * @param [in] table 哈希表
 * @param [in] size 哈希表长度
 * @return 无
 */
static void HTableSize(HTable_ST *table, unsigned size)
{
    HTableInfo_ST **h = NULL;

    size |= 1;

    table->data = h = (HTableInfo_ST **) malloc(size * sizeof(HTableInfo_ST *));
    table->size = size;
    table->used = 0;

    while (0 < size--)
    {
        *h++ = 0;
    }
}

/**
 * @brief 哈希表节点空间扩容
 * @param [in] table 哈希表
 * @return 无
 */
static void HTableGrow(HTable_ST *table)
{
    HTableInfo_ST *ht = NULL;
    HTableInfo_ST *next = NULL;
    unsigned int old_size = table->size;
    HTableInfo_ST **h = table->data;
    HTableInfo_ST **old_entries = h;

    HTableSize(table, 2 * old_size);

    while (0 < old_size--)
    {
        for (ht = *h++; ht; ht = next)
        {
            next = ht->next;
            HTableLink(table, ht);
        }
    }
    free((char *) old_entries);
}

/**
 * @brief 创建哈希表
 * @param [in] size 哈希表长度
 * @param [out] hTable 哈希表
 * @return 无
 */
void HTableCreate(HTable_ST **hTable, int size)
{
    HTable_ST *table = NULL;

    table = (HTable_ST *) malloc(sizeof(HTable_ST));
    if (NULL == table)
    {
        (*hTable) = NULL;
        return;
    }
    memset(table, 0x00, sizeof(HTable_ST));
    HTableSize(table, MAX(size, MINHASH_SIZE));
    (*hTable) = table;
}

/**
 * @brief 哈希表元素入队
 * @param [in] table 哈希表
 * @param [in] key key
 * @param [in] value value
 * @param [out] tableInfo 存放key-value的哈希表节点
 * @return 无
 */
void HTableEnter(HTable_ST *table, const char *key, char *value,
        HTableInfo_ST **tableInfo)
{
    HTableInfo_ST *ht = NULL;

    if (table->used >= table->size)
    {
        HTableGrow(table);
    }
    ht = (HTableInfo_ST *) malloc(sizeof(HTableInfo_ST));
    if (NULL == ht)
    {
        if (NULL != tableInfo)
        {
            (*tableInfo) = NULL;
        }
        return;
    }
#ifdef WIN32
    ht->key = _strdup(key);
#else
    ht->key = strdup(key);
#endif
    ht->value = value;
    HTableLink(table, ht);
    if (NULL != tableInfo)
    {
        (*tableInfo) = ht;
    }
    return;
}

/**
 * @brief 哈希表元素查找
 * @param [in] table 哈希表
 * @param [in] key key
 * @param [out] value 找到的value
 * @return
 *  - TRUE  找到
 *  - FALSE 未找到
 */
BOOL HTableFind(HTable_ST *table, const char *key, char **value)
{
    HTableInfo_ST *ht = NULL;

    if (table)
    {
        for (ht = table->data[HTableHash(key, table->size)]; ht; ht = ht->next)
        {
            if (STREQ(key, ht->key))
            {
                if (NULL != value)
                {
                    (*value) = ht->value;
                }
                return (TRUE);
            }
        }
    }
    if (NULL != value)
    {
        (*value) = NULL;
    }
    return (FALSE);
}

/**
 * @brief 哈希表元素定位
 * @param [in] table 哈希表
 * @param [in] key key
 * @param [out] tableInfo 定位的哈希表节点
 * @return 无
 */
void HTableLocate(HTable_ST *table, const char *key, HTableInfo_ST **tableInfo)
{
    HTableInfo_ST *ht = NULL;

    if (NULL == tableInfo)
    {
        return;
    }

    if (table)
    {
        for (ht = table->data[HTableHash(key, table->size)]; ht; ht = ht->next)
        {
            if (STREQ(key, ht->key))
            {
                (*tableInfo) = ht;
                return;
            }
        }
    }
    (*tableInfo) = NULL;
    return;
}

/**
 * @brief 哈希表元素删除
 * @param [in] table 哈希表
 * @param [in] key key
 * @param [in] free_fn 释放节点内存回调函数
 * @return 无
 */
void HtableDelete(HTable_ST *table, const char *key, void (*free_fn)(char *))
{
    if (table)
    {
        HTableInfo_ST *ht = NULL;
        HTableInfo_ST **h = table->data + HTableHash(key, table->size);

        for (ht = *h; ht; ht = ht->next)
        {
            if (STREQ(key, ht->key))
            {
                if (ht->next)
                {
                    ht->next->prev = ht->prev;
                }
                if (ht->prev)
                {
                    ht->prev->next = ht->next;
                }
                else
                {
                    *h = ht->next;
                }

                table->used--;
                free(ht->key);
                if (free_fn && ht->value)
                {
                    (*free_fn)(ht->value);
                }
                free((char *) ht);
                return;
            }
        }
    }
}

/**
 * @brief 哈希表释放
 * @param [in] table 哈希表
 * @param [in] free_fn 释放节点内存回调函数
 * @return 无
 */
void HTableFree(HTable_ST *table, void (*free_fn)(char *))
{
    unsigned int i = 0;
    HTableInfo_ST *ht = NULL;
    HTableInfo_ST *next = NULL;
    HTableInfo_ST **h = NULL;

    if (table)
    {
        i = table->size;
        h = table->data;
        while (0 < i--)
        {
            for (ht = *h++; ht; ht = next)
            {
                next = ht->next;
                free(ht->key);
                if (free_fn && ht->value)
                {
                    (*free_fn)(ht->value);
                }
                free((char *) ht);
            }
        }
        free((char *) table->data);
        table->data = 0;
        free((char *) table);
    }
}

/**
 * @brief 列出哈希表元素
 * @param [in] table 哈希表
 * @param [out] tableInfo 哈希表元素列表
 * @return 无
 */
void HTableList(HTable_ST *table, HTableInfo_ST ***tableInfo)
{
    unsigned int i = 0;
    int count = 0;

    HTableInfo_ST **list = NULL;
    HTableInfo_ST *tableData = NULL;

    if (NULL == tableInfo)
    {
        return;
    }

    if (table)
    {
        list = (HTableInfo_ST **) malloc(sizeof(*list) * (table->used + 1));
        if (NULL == list)
        {
            (*tableInfo) = NULL;
            return;
        }
        for (i = 0; i < table->size; i++)
        {
            for (tableData = table->data[i]; tableData != 0; tableData =
                    tableData->next)
            {
                list[count++] = tableData;
            }
        }
    }
    else
    {
        list = (HTableInfo_ST **) malloc(sizeof(*list));
    }
    list[count] = 0;
    (*tableInfo) = list;
    return;
}

/**
 * @brief 对象列缺省字串格式化输出
 * @param [in] buff： destination buffer
 * @param [in] bufflen： size of buffer
 * @param [in] colDesc： col description struct
 * @param [in] pData: object's base address
 * @param [out] rc： return err code
 * @return
 * - > 0 return the number of characters printed
 * - == 0 *rc == -1, param eror.
 * - == 0 *rc == -2, unkown data type eror.
 */
int default_format_func(char *buff, size_t bufflen, char* colDesc, char *pData, int *rc)
{
    int nPrint = 0;
    COL_DESC_ST *pCol = (COL_DESC_ST *) colDesc;

    if (NULL == buff || 0 >= bufflen || NULL == pData)
    {
        if (rc)
        {
            (*rc) = -1; ///param eror
        }
        return nPrint;
    }

    if (NULL == colDesc)
    {
        char *ptrStr = (char *) pData;
        nPrint = snprintf(buff, bufflen, "\"%s\"", ptrStr);
        return nPrint;
    }

    switch (pCol->dataType)
    {
        case DATATYPE_CHAR:
        {
            char c = (char) (*(pData + pCol->offset));
            nPrint = snprintf(buff, bufflen, "%c", c);
        }
            break;
        case DATATYPE_CHARP:
        {
            char **ptrCh = (char **) (pData + pCol->offset);
            nPrint = snprintf(buff, bufflen, "\"%s\"", *ptrCh);
        }
            break;
        case DATATYPE_STR:
        {
            char *ptrStr = (char *) (pData + pCol->offset);
            nPrint = snprintf(buff, bufflen, "\"%s\"", ptrStr);
        }
            break;
        case DATATYPE_BOOL:
        {
            bool b = (bool) (*(pData + pCol->offset));
            if (b)
                nPrint = snprintf(buff, bufflen, "true");
            else
                nPrint = snprintf(buff, bufflen, "false");
        }
            break;
        case DATATYPE_INT32:
        {
            int iNum = (int) *(pData + pCol->offset);
            nPrint = snprintf(buff, bufflen, "%d", iNum);
        }
            break;
        case DATATYPE_INT64:
        {
            int64_t iNum64 = (int64_t) *(pData + pCol->offset);
            nPrint = snprintf(buff, bufflen, "%ld", iNum64);
        }
            break;
        case DATATYPE_DOUBLE:
        {
            double dNum = (double) *(pData + pCol->offset);
            nPrint = snprintf(buff, bufflen, "%g", dNum);
        }
            break;
        default:
            if (rc)
            {
                (*rc) = -2; ///unkown data type
            }
            break;
    }
    return nPrint;
}

