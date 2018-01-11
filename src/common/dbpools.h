/*
 * Tsfree is pleased to support the open source community by making Tsbase available.
 * Copyright (C) 2016 THL A29 Limited, a Tsfree company. All rights reserved.
 * Licensed under the MIT License (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 *
 */

/**
* MSSQL连接池管理类，对物理单元的个体(一个服务器或者更小的级别)进行连接管理
* 这连接池的任意两个连接理论上具有对等的逻辑功能，没有差别
* 为了保证长连接，请在调用接口中实现为静态对象或者全局对象
**/

#ifndef _TS_COMMON_DBPOOLS_H_
#define _TS_COMMON_DBPOOLS_H_

#include <list>
#include "mutex.h"
#include "ftds_ex.h"

#define MAX_DB_HOST_LEN 64
#define MAX_DB_USER_LEN 32
#define MAX_DB_PSWD_LEN 64
#define MAX_DB_NAME_LEN 24

#define DB_CONN_MAX    200   //DB 连接池最大连接数
#define POOL_CONN_DEF  5     //连接池缺省连接数
#define DB_LG_TIMEOUT  2     //DB 连接超时
#define DB_RW_TIMEOUT  2    //读写超时

using namespace std;

class Dbpools
{
public:
	Dbpools();
	Dbpools(uint32_t pool_size, uint32_t lg_time_out, uint32_t rw_time_out);
	~Dbpools();
	
	//初始化所有连接
	int init(const char *host, 
		const char *user, 
		const char *passwd, 
		const char *dbname, 
		uint32_t conn_nums, 
		uint32_t lg_time_out, 
		uint32_t rw_time_out);
	
	FtdsEx* get_db_connect();
	
	int put_db_connect(FtdsEx *dbconnect);
	
private:
	int init(const char *host, const char *user, const char *passwd, const char *dbname, int index); //指定参数初始化连接

private:
	list<FtdsEx*> dbconn_list;
	Mutex db_conn_lock_;                //和连接一一对应保证单个线程使用连接
	uint32_t _conn_nums_;               //连接总数 

	//服务器配置
	char db_host_[MAX_DB_HOST_LEN];
	char db_user_[MAX_DB_USER_LEN];
	char db_pswd_[MAX_DB_PSWD_LEN];
	char db_name_[MAX_DB_NAME_LEN];

	uint32_t lg_time_out_;  //db连接超时
	uint32_t rw_time_out_;  //db读写超时
};



#endif //_TS_COMMON_DBPOOLS_H_
