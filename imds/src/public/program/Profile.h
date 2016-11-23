/**
 * @file    Profile.h
 * @brief   处理配置文件和环境变量
 * @author  ygao@sse.com.cn
 * @version 1.0
 * @date    2014-3-13
 */

#ifndef PROFILE_H_
#define PROFILE_H_

/**
 * @class Profile 
 * @brief 处理配置文件和环境变量
 */
class Profile {
public:
	/** @brief 得到配置类实例*/
	static Profile* Instance();
	static Profile* Instance(const char* program_name);
	/** @brief 得到运行目录绝对路径*/
	const char* getHome();
	/** @brief 根据参数名称得到配置*/
	const char* getEnv(const char* name);
	/** @brief 写入或读取PID文件*/
	const char* setPid(bool isWrite,bool isOnly=false);
private:
	Profile(const char* program_name);
	virtual ~Profile();

	bool getValue(const char* path,const char* name);
	void getLogger();
private:
	static Profile* prof;
	char program_name[256];
	char home_path[256];
	char envvalue[256];
};

#endif /* PROFILE_H_ */
