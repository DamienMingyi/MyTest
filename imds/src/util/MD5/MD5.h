/******************************************************************
**版权所有
**文件名:		qpsmain.cpp
**版本:		0.1
**创建人:		ygao@sse.com.cn
**创建日期:	2013-2-27
**功能描述:	对字符串或文件求MD5的值
**修订记录：
**修订日期	修订人	修复的bug或修订原因
**-----------------------------------------------------------------
*******************************************************************/
#ifndef MD5_H_
#define MD5_H_

class MD5 {
public:
	MD5();
	~MD5();

	void MD5_Key(char* buff, int buffLen, char* md5key);
	void MD5_String(char* msg,char* md5key);
	void MD5_File(const char* filename,char* md5key);
private:
	unsigned int state[4];					/* state (ABCD) */
	unsigned int count[2];					/* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];       /* input buffer */
	unsigned char PADDING[64];		/* What? */

private:
	void Init ();
	void Transform (unsigned  int state[4], unsigned char block[64]);
	void Update (unsigned char *input, unsigned int inputLen);
	void Final (unsigned char digest[16]);
	//void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned  int *input,unsigned int len);
	void Decode (unsigned  int *output, unsigned char *input, unsigned int len);
	//void MD5_memset (unsigned char* output,int value,unsigned int len);
};

#endif /* MD5_H_ */
