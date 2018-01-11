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

#ifndef _TS_FCGISERVER_H_
#define _TS_FCGISERVER_H_

#include "common/thread_pool.h"
#include "common/dbpools.h"
#include "Options.h"
#include "common/cfg.h"

class BusinessServer
{
public:
	BusinessServer();
	~BusinessServer();
	
	/// @brief 自动解析ini文件，并设置到options，如果调用需要在Init前调用
	/// @param file_name 配置文件 如 "../cfg/yeston.ini"
	/// @return 0 成功
	/// @return <0 失败
	int32_t LoadOptionsFromIni(const std::string& file_name);
	
	/// @brief 初始化接口，若使用ini配置，请先调用 @see LoadOptionsFromIni
	/// @return 0 成功
	/// @return <0 失败
	int32_t Init();
	
	/// @brief 启动线程池
	/// @return 0 成功
	/// @return <0 失败
	int32_t start();
	
	/// @brief 停止线程池
	void stop();

	void Join();
	
private:
	/// @brief 获取ThreadPool线程池
	/// @return 非NULL 成功
	/// @return NULL 失败
	ThreadPool* GetThreadPool();
	
	/// @brief 获取DbPool数据库连接池
	/// @return 非NULL 成功
	/// @return NULL 失败
	Dbpools* GetDbPool();
	
	/// @brief 线程池工作函数
	void run_main_thread();

private:
	Options             m_options;
	Dbpools*            m_dbpools;
	ThreadPool*         m_threadpool;
	
	typedef std::tr1::function<void()> Fun;
};



#endif  //_TS_FCGISERVER_H_

