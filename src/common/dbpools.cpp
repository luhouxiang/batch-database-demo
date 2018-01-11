#include "dbpools.h"
#include <time.h>
#include <string.h>
#include "log.hpp"

Dbpools::Dbpools()
{
	_conn_nums_ = POOL_CONN_DEF;
	memset(db_host_, 0x0, sizeof(db_host_));
	memset(db_user_, 0x0, sizeof(db_user_));
	memset(db_pswd_, 0x0, sizeof(db_pswd_));
	memset(db_name_, 0x0, sizeof(db_name_));

	lg_time_out_ = DB_LG_TIMEOUT;
	rw_time_out_ = DB_RW_TIMEOUT;
}

Dbpools::Dbpools(uint32_t pool_size, uint32_t lg_time_out, uint32_t rw_time_out)
{
	_conn_nums_ = pool_size;

	memset(db_host_, 0x0, sizeof(db_host_));
	memset(db_user_, 0x0, sizeof(db_user_));
	memset(db_pswd_, 0x0, sizeof(db_pswd_));
	memset(db_name_, 0x0, sizeof(db_name_));

	lg_time_out_ = lg_time_out;
	rw_time_out_ = rw_time_out;    
}

Dbpools::~Dbpools()
{
	for (uint32_t i = 0; i < _conn_nums_; i++)
	{
		FtdsEx *conn = dbconn_list.front();
		dbconn_list.pop_front();
		if (conn != NULL)
		{
			delete conn;
			conn = NULL;
		}
	}
	// Dbms_conn::exit();
}

int Dbpools::init(const char *host, 
                  const char *user, 
                  const char *passwd, 
                  const char *dbname, 
	uint32_t conn_nums, 
	uint32_t lg_time_out, 
	uint32_t rw_time_out)
{
	
	if (conn_nums == 0) {
		_conn_nums_ = POOL_CONN_DEF;
		
		ERR_LOG("Dbpools get conn_nums config failed,use default conn_nums %d", _conn_nums_);
	} 
	else {
		_conn_nums_ = conn_nums;
	}
	
	//配置的再大，也不能超过框架内部限制的连接数
	if (_conn_nums_ > DB_CONN_MAX) {
		_conn_nums_ = DB_CONN_MAX;
	}
	
	if (host == NULL) {
		ERR_LOG("Dbpools get host failed");
		return -1;
	}
	else {
		strncpy(db_host_, host, sizeof(db_host_) - 1);
	}
	
	if (user == NULL) {
		ERR_LOG("Dbpools get user failed");
		return -1;
	}
	else {
		strncpy(db_user_, user, sizeof(db_user_) - 1);
	}
	
	if (passwd == NULL) {
		ERR_LOG("Dbpools get passwd failed");
		return -1;
	}
	else {
		strncpy(db_pswd_, passwd, sizeof(db_pswd_) - 1);
	}
	
	if (dbname == NULL) {
		ERR_LOG("Dbpools get dbname failed");
		return -1;
	}
	else {
		strncpy(db_name_, dbname, sizeof(db_name_) - 1);
	}
	
	lg_time_out_ = lg_time_out;
	rw_time_out_ = rw_time_out;
	RUN_LOG("Dbpools start to init conn, conn_nums=%d ,lgtimeout=%d ,rwtimeout=%d",_conn_nums_,lg_time_out_,rw_time_out_);
	
	//初始化一池连接
	int ret = 0;
	for (uint32_t index = 0; index < _conn_nums_ && 0 == ret; index++)
	{
		ret = init(db_host_, db_user_, db_pswd_, db_name_, index);  
		if (ret != 0) 
		{
			ERR_LOG("Dbpools connect to server failed");
			return -1;
		}
	}
	return ret;

}

int Dbpools::init(const char *host, const char *user, const char *passwd, const char *dbname, int index)
{
	if (host == NULL || user == NULL || passwd == NULL || 
			dbname == NULL || index >= DB_CONN_MAX || index < 0)
	{
		ERR_LOG("Dbpools init failed, para is invalid");
		return -1;
	}

	FtdsEx *conn = new FtdsEx();
	if (conn == NULL)
	{
		ERR_LOG("Dbpools init index=%d, failed, memory buffer full", index);
		return -1;
	}

	if (conn->Init(host, user, passwd, dbname, lg_time_out_, rw_time_out_) == false) {

		ERR_LOG("Dbpools connect db failed,host=%s, index=%d", host, index);
	}

	dbconn_list.push_back(conn);

	return 0;
}
	
FtdsEx* Dbpools::get_db_connect()  
{
	AutoLocker lock(&db_conn_lock_); 
	if (!dbconn_list.empty())
	{
		FtdsEx *dbconnect = dbconn_list.front();
		dbconn_list.pop_front();
		return dbconnect;
	}
	else {
			ERR_LOG("Dbpools get_db_connect failed, list is empty");
		return NULL;
	}
}

int Dbpools::put_db_connect(FtdsEx *dbconnect)
{
	AutoLocker lock(&db_conn_lock_);
	if (dbconnect != NULL)
	{
		dbconn_list.push_back(dbconnect);			
	}
	else
	{
			ERR_LOG("Dbpools putdbconnect failed, dbconnect is null");
	}
	return 1;
}


