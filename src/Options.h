#ifndef  _TS_OPTIONS_H_
#define  _TS_OPTIONS_H_

#include <string> 
using namespace std;
/// @brief FastcgiServer的全量参数定义，每个参数均有默认值，用户只需修改自己使用的即可
/// 配置参数赋值后在init生效
struct Options {

	// mssql      
	std::string _mssql_host;        // mssql连接串
	std::string _mssql_user;        // 用户名
	std::string _mssql_passwd;      // 密码
	std::string _mssql_db_name;     // 数据库名
	uint32_t _mssql_pool_size;      // MSDB连接池大小
	uint32_t _mssql_lg_timeout_s;   // 读写超时，单位为秒，默认为2s
	uint32_t _mssql_rw_timeout_s;   // 读写超时，单位为秒，默认为15s

	string _log_path;
	string _log_level;
	
	Options();
};


#endif   //  _TS_OPTIONS_H_