/*
 * EncoderMessage.h
 *
 *  Created on: 2014-12-16
 *      Author: level2
 */

#ifndef ENCODERMESSAGE_H_
#define ENCODERMESSAGE_H_

namespace FastCodecs
{
#ifdef WIN32
#   undef  _OS_WIN32
#   define _OS_WIN32 1
#   ifdef WIN64
#       undef  _OS_64BIT
#       define _OS_64BIT 1
#   elif WIN32
#       undef  _OS_32BIT
#       define _OS_32BIT 1
#   else
#       error "Neither WIN32 nor WIN64 defined!"
#   endif
#else /* posix? */
#   undef  _OS_POSIX
    /** 定义_OS_POSIX */
#   define _OS_POSIX 1
#   if defined(__LP64__) || defined(__LP64) || __WORDSIZE==64
#       undef  _OS_64BIT
#       define _OS_64BIT 1
#   else
#       undef  _OS_32BIT
    /** 定义_OS_POSIX */
#       define _OS_32BIT 1
#   endif
#endif

#ifdef _OS_32BIT
#ifdef _MSC_VER
    typedef signed __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#else
    /** 32bit 定义类型int64 */
    typedef signed long long int64_t;
    /** 32bit 定义类型uint64 */
    typedef unsigned long long uint64_t;
#endif
    /** 32bit 定义类型int32 */
    typedef signed int int32_t;
    /** 32bit 定义类型uint32 */
    typedef unsigned int uint32_t;
    /** 32bit 定义类型int16 */
    typedef signed short int16_t;
    /** 32bit 定义类型uint16 */
    typedef unsigned short uint16_t;
    /** 32bit 定义类型int8 */
    typedef signed char int8_t;
    /** 32bit 定义类型uint8 */
    typedef unsigned char uint8;
#endif

#ifdef _OS_64BIT
#ifdef _MSC_VER
    typedef signed __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#else
    /** 64bit 定义类型int64 */
    typedef signed long long int64_t;
    /** 64bit 定义类型uint64 */
    typedef unsigned long long uint64_t;
#endif
    /** 64bit 定义类型int32 */
    typedef signed int int32_t;
    /** 64bit 定义类型uint32 */
    typedef unsigned int uint32_t;
    /** 64bit 定义类型int16 */
    typedef signed short int16_t;
    /** 64bit 定义类型uint16 */
    typedef unsigned short uint16_t;
    /** 64bit 定义类型int8 */
    typedef signed char int8_t;
    /** 64bit 定义类型uint8 */
    typedef unsigned char uint8_t;
#endif

    static const char STOPBIT = 0x80;
    static const int FAST_TAG_NUM = 64;

#ifndef NULL
#define NULL 0
#endif

    typedef unsigned char uchar_t;

    typedef enum
    {
        Undefined = 1,
        Null = 2,
        Integer = 4,
        uInteger = 8,
        aciString = 16,
        uniString = 32,
        decimal = 64,
    } L2ValueType;

    class Value
    {
    public:
        Value();
        virtual ~Value();
    public:
        void setValue(const int32_t v);
        void setValue(const uint32_t v);
        void setValue(const int64_t v);
        void setValue(const uint64_t v);
        void setValue(const char* str);
        void setValue(const char* str, int len);
        //void setValue(const double v);
        void setValue(const int64_t mantissa, const int32_t exponent);
        void setUndefined();
        void setNull();
        void setNullInteger();
        void setNullACIString();
        void setNullUNIString();
        bool isUndefined();
        bool isNull();

        bool isType(const L2ValueType vt);

        template<typename INT_Type>
        bool getInt(INT_Type& v)
        {
            if ((vt & Undefined) | (vt & Null))
                return false;
            if (vt & Integer)
            {
                v = INT_Type(intVal);
                return true;
            }
            if (vt & uInteger)
            {
                v = INT_Type(uintVal);
                return true;
            }
            return false;
        }

        bool getString(char* str);
        bool getDouble(double &v);
        bool getDecimalInt(int64_t &v, int mulExp = 0);

        bool operator==(Value rhs);
        bool operator!=(Value rhs);
    private:
        static const int StringSize = 64;
        int vt;
        int64_t intVal;
        uint64_t uintVal;
        int32_t exponent;
        char stringVal[StringSize];
    };

    class FieldValue: public Value
    {
    public:
        FieldValue()
        {
            id = decimal = 0;
        }
        ~FieldValue()
        {
        }
    public:
        void setID(int id)
        {
            this->id = id;
        }
        void setDecimal(int decimal)
        {
            this->decimal = decimal;
        }
        int getID()
        {
            return id;
        }
        int getDecimal()
        {
            return decimal;
        }
    private:
        int id;
        int decimal;
    };

    class EncoderMessage
    {
    public:
        EncoderMessage();
        EncoderMessage(const EncoderMessage& rhs); //拷贝构造
        virtual ~EncoderMessage();

        EncoderMessage& operator=(const EncoderMessage& rhs); //拷贝赋值
        FieldValue& operator[](int i);

        const FieldValue* getField() const;
        int getFieldLength() const;
    private:
        FieldValue* field;
        int field_length;
    };

    typedef EncoderMessage DecoderMessage;
} /* namespace FastCodecs */
#endif /* ENCODERMESSAGE_H_ */
