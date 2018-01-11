#include "BusinessServer.h"
#include <stdio.h>
#include <iostream>
#include <tr1/functional>
#include "common/ini_reader.h"
# include "common/string_utility.h"

#include "common/log.hpp"

# include "new_stock.h"


BusinessServer::BusinessServer()
{
	m_threadpool = NULL;
	m_dbpools = NULL;
}

BusinessServer::~BusinessServer()
{
	if (m_threadpool != NULL)delete m_threadpool;
	if (m_dbpools != NULL)delete m_dbpools;
	
	m_threadpool = NULL;
	m_dbpools = NULL;
}
	
Dbpools* BusinessServer::GetDbPool() {
	if (!m_dbpools) {
		m_dbpools = new Dbpools();
	}

	return m_dbpools;
}

ThreadPool* BusinessServer::GetThreadPool() {
	if (!m_threadpool) {
		m_threadpool = new ThreadPool();
	}

	return m_threadpool;
}
	
int32_t BusinessServer::LoadOptionsFromIni(const std::string& file_name) {
	
	if ( not JsonReader::instance()->initialize(file_name) ) {
		ERR_LOG("load config file failed");
	}
	
	if ( not JsonReader::instance()->loadConfig(m_options)) {
		ERR_LOG("initialize config failed");
	}

	return 0;
}

int32_t  BusinessServer::Init()
{
	if (GetDbPool() == NULL) {
		
		ERR_LOG("failed to create a dbpools");
		return -1;
	}
		
	if (GetThreadPool() == NULL) {
		
		ERR_LOG("failed to create a threadpool");
		return -1;
	}

	if (m_options._mssql_host.length() < 2) {
		
		ERR_LOG("mssql config host is error");
		return -1;
	}
	
	if (m_options._mssql_user.length() < 1) {
		
		ERR_LOG("mssql config user is error");
		return -1;
	}
	
	if (m_options._mssql_passwd.length() < 1) {
		
		ERR_LOG("mssql config passwd is error");
		return -1;
	}
	
	if (m_options._mssql_db_name.length() < 1) {
		
		ERR_LOG("mssql config dbname is error");
		return -1;
	}
	
	if (m_options._mssql_pool_size < 0 || m_options._mssql_pool_size > 200) {
		
		ERR_LOG("mssql config poolsize is error");
		return -1;
	}
	
	if (m_options._mssql_lg_timeout_s < 1 || m_options._mssql_lg_timeout_s > 15) {
		
		ERR_LOG("mssql config login timeout is error");
		return -1;
	}
	
	if (m_options._mssql_lg_timeout_s < 1 || m_options._mssql_lg_timeout_s > 20) {
		
		ERR_LOG("mssql config read and write timeout is error");
		return -1;
	}
	

	return 0;
}

int32_t BusinessServer::start() {
	if (m_dbpools->init(m_options._mssql_host.c_str(),
		m_options._mssql_user.c_str(), 
		m_options._mssql_passwd.c_str(),
		m_options._mssql_db_name.c_str(),
		m_options._mssql_pool_size,
		m_options._mssql_lg_timeout_s,
		m_options._mssql_rw_timeout_s) != 0) {
		
		ERR_LOG("failed to create a dbpools Init");
		return -1;
	}
	
	if (m_threadpool->Init(1, ThreadPool::PENDING) != 0) {
		
		ERR_LOG("failed to create a threadpool Init");
		return -1;
	}
	
	Fun fun_main = std::tr1::bind(&BusinessServer::run_main_thread, this);
	if (m_threadpool->AddTask(fun_main) != 0) {			
		ERR_LOG("failed to create a fun_main AddTask in");
		return -1;
	}	
			
	return 0;
}

void BusinessServer::stop()
{
	if (m_threadpool != NULL) m_threadpool->Terminate(false);
}

void BusinessServer::Join() {
	m_threadpool->Join();
}
	
void BusinessServer::run_main_thread() {
	RUN_LOG("match_run_main_thread begin...");
	FtdsEx* dbmsconn = m_dbpools->get_db_connect();
	NewStock *newStock = new NewStock(dbmsconn, m_options);
	
	newStock->run_main_proc();
	delete newStock;
	return ;
}