/*
 * Base64.cpp
 *
 *  Created on: 2013-5-30
 *      Author: level2
 */

#include "Base64.h"
#include <string.h>

const char MAGIC_STR[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char DECODE_LIST[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
        59, 60, 61, -1, -1, -1, 64, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26,
        27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
        47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

typedef struct stInBlock
{
    char c1;
    char c2;
    char c3;
} Block3;

typedef struct stOutBlock
{
    char c1;
    char c2;
    char c3;
    char c4;
} Block4;

Base64::Base64()
{
    // TODO Auto-generated constructor stub

}

Base64::~Base64()
{
    // TODO Auto-generated destructor stub
}

char Base64::getOffset(char ch)
{
    if ((ch >= 'A') && (ch <= 'Z'))
        return ch - 'A';

    if ((ch >= 'a') && (ch <= 'z'))
        return ch - 'a' + 26;

    if ((ch >= '0') && (ch <= '9'))
        return ch - '0' + 52;

    switch (ch)
    {
        case '+':
            return 62;
        case '/':
            return 63;
        case '=':
            return 64;
        default:
            return -1;
    }
}

unsigned long Base64::Decode(const char *_rEncoded, char *_rRawData,
        unsigned long ulInputLen)
{
    //unsigned long ulInputLen = strlen(_rEncoded);

    if (ulInputLen % 4 != 0)
    {
        return -1;
    }

    unsigned long ulOutputLen = 0; // it will record the really used ulOutputLen gradually

    const Block4 * pIn = (Block4 *) _rEncoded;
    Block3 * pOut = (Block3 *) _rRawData;

    while (ulInputLen > 0)
    {
        unsigned long ulNewOutput = 3;
        char c[4];
        int i;

        c[0] = DECODE_LIST[pIn->c1];
        c[1] = DECODE_LIST[pIn->c2];
        c[2] = DECODE_LIST[pIn->c3];
        c[3] = DECODE_LIST[pIn->c4];

        // Check the pattern
        if ((-1 == c[0]) || (-1 == c[1]) || (-1 == c[2]) || (-1 == c[3]))
        {
            return -1;
        }

        // c[i] == 64 when input is '='
        if (64 == c[0] || 64 == c[1])
        {
            return -1;
        }
        else if (64 == c[2])
        {
            if (64 == c[3])
            {
                // 'XX==' pattern
                ulNewOutput = 1;
            }
            else
            {
                return -1;
            }
        }
        else if (64 == c[3])
        {
            // 'XXX=' pattern
            ulNewOutput = 2;
        }

        // Now ulNewOutput can be 1, 2 or 3.
        ulOutputLen += ulNewOutput;
        switch (ulNewOutput)
        {
            case 1:
                // 'XX==', 1 output char
                pOut->c1 = ((c[0] & 0x3F) << 2) | ((c[1] >> 4) & 0x03);

                ulInputLen = 0;
                break;
            case 2:
                // 'XXX=', 2 output char
                pOut->c1 = ((c[0] & 0x3F) << 2) | ((c[1] >> 4) & 0x03);
                pOut->c2 = ((c[1] & 0x0F) << 4) | ((c[2] >> 2) & 0x0F);

                ulInputLen = 0;
                break;
            default:
                // 'XXXX', 3 output char
                pOut->c1 = ((c[0] & 0x3F) << 2) | ((c[1] >> 4) & 0x03);
                pOut->c2 = ((c[1] & 0x0F) << 4) | ((c[2] >> 2) & 0x0F);
                pOut->c3 = ((c[2] & 0x03) << 6) | (c[3] & 0x3F);

                ulInputLen -= 4;
                break;
        }
        pOut++;
        pIn++;
    }

    return ulOutputLen;
}

void Base64::Encode(const char *_rRawData, char *_rEncoded, unsigned long SrcLen)
{
    if ((_rEncoded == 0) || (_rRawData == 0))
    {
        return;
    }

    unsigned long ulSrcLen = SrcLen;

    const Block3 * pIn = (Block3 *) _rRawData;
    Block4 * pOut = (Block4 *) _rEncoded;

    while (ulSrcLen > 0)
    {
        switch (ulSrcLen)
        {
            case 1:
                pOut->c1 = MAGIC_STR[(pIn->c1 >> 2) & 0x3F];
                pOut->c2 = MAGIC_STR[(pIn->c1 & 0x03) << 4];
                pOut->c3 = '=';
                pOut->c4 = '=';

                ulSrcLen = 0;
                break;
            case 2:
                pOut->c1 = MAGIC_STR[(pIn->c1 >> 2) & 0x3F];
                pOut->c2 = MAGIC_STR[((pIn->c1 & 0x03) << 4) | ((pIn->c2 >> 4) & 0x0F)];
                pOut->c3 = MAGIC_STR[(pIn->c2 & 0x0F) << 2];
                pOut->c4 = '=';

                ulSrcLen = 0;
                break;
            default:
                pOut->c1 = MAGIC_STR[(pIn->c1 >> 2) & 0x3F];
                pOut->c2 = MAGIC_STR[((pIn->c1 & 0x03) << 4) | ((pIn->c2 >> 4) & 0x0F)];
                pOut->c3 = MAGIC_STR[((pIn->c2 & 0x0F) << 2) | ((pIn->c3 >> 6) & 0x03)];
                pOut->c4 = MAGIC_STR[pIn->c3 & 0x3F];

                pIn++;
                pOut++;
                ulSrcLen -= 3;

                break;
        }

    }	// end of while

    return;
}
