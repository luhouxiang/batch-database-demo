#ifndef __KDS_MOBILE_STOCK_FTDSEX_H__
#define __KDS_MOBILE_STOCK_FTDSEX_H__
# include <string>
# include "ftds.h"
# include "json/json.h"
using std::string;

class FtdsEx: public FTDS
{
public:
	FtdsEx();
	~FtdsEx();
public:
	bool Init(const char* db_server,
		const char* db_user,
		const char* db_pswd, 
		const char *db_name,
		uint32_t login_timeout,
		uint32_t rw_timeout
		);
	bool ExecSQL(const char *strSQL, Json::Value& json_ret);
private:
	int CountCol();
	bool OpenDB(const char *server, const char *userName, 
		const char *password, const char *DBName, uint32_t login_timeout,
		uint32_t rw_timeout);
	void CloseDB();
	bool ExecSQL(const char *strSQL);
	int  GetResult();
	bool BindColumn(int column, int varLen, char *varAddr);
	bool BindColumn(int column, int varLen, int *varAddr);
	bool MoveNextRow();
	int  CountRow();
	bool IsCount();
	bool Cancel();
	bool ReOpen();
	

private:
	string _db_name;
	string _user_name;
	string _pass_word;
	string _server_name;
	uint32_t _login_timeout;
	uint32_t _rw_timeout;
};

#endif
