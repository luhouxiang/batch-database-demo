# include "new_stock.h"
# include <string>
# include <vector>
# include <string.h>
# include "common/log.hpp"

# include "common/ftds_ex.h"
# include "common/string_utility.h"

# include "common/BatchUpdateSql.h"
# include "common/BatchInsertSql.h"
# include "common/BatchDeleteSql.h"

using namespace std;

NewStock::NewStock(FtdsEx *dbmsconn, const Options& options)
{
	m_options = options;
	_dbmsconn = dbmsconn;
}

string NewStock::StrInsertHead()
{
    char sqlbuf[512] = {0};
    sprintf(sqlbuf, "insert into TBL_DEMO([CODE],[NAME])values");
    return sqlbuf;
}

string NewStock::StrInsertBody(const string& code, const string& name)
{
    char sqlbuf[512] = {0};
    sprintf(sqlbuf, "('%s','%s')", code.c_str(), name.c_str());
    return sqlbuf;
}

void NewStock::Demo()
{
    BatchInsertSql batch_insert;
    BatchUpdateSql batch_update;
    BatchDeleteSql batch_delete;

    batch_insert.SetHead(StrInsertHead());
    batch_update.SetHead("TBL_DEMO","CODE","NAME");
    batch_delete.SetHead("TBL_DEMO","CODE");

    for (int i = 0; i < 3; i++)
    {
        batch_insert.addBody(StrInsertBody(StringUtility::to_string(i), 
            "name_" + StringUtility::to_string(i)));
        batch_update.addValue(StringUtility::to_string(i), "myname_" + StringUtility::to_string(i));
        batch_delete.addValue(StringUtility::to_string(i));
    }
    batch_insert.addBody(StrInsertBody(StringUtility::to_string(100), 
        "name_" + StringUtility::to_string(100)));
    string cmd = batch_insert.GetSql() + batch_update.GetSql() + batch_delete.GetSql();
    cmd = AddTransShell(cmd);
    RUN_LOG("sql: %s", cmd.c_str());
    Json::Value json_ret;
    if(_execSQL(cmd, json_ret)) {
        RUN_LOG("执行成功");
    }
    
}

bool NewStock::_execSQL(const std::string sqlCmd, Json::Value& json_ret)
{
    if (_dbmsconn == NULL) {
        json_ret[0][0]["errorcode"] = "-1";
        json_ret[0][0]["errormsg"] = "操作不成功";
        json_ret[1] = "";
        ERR_LOG("数据库连接对象为空。sql: %s", sqlCmd.c_str());
        return false;
    }
    if (!_dbmsconn->ExecSQL( sqlCmd.c_str(), json_ret)) {
        // LOG(ERROR) << "db exec failed:" << _dbmsconn->get_err_msg();
        json_ret[0][0]["errorcode"] = "-1";
        json_ret[0][0]["errormsg"] = "操作不成功";
        json_ret[1] = "";
        ERR_LOG("执行存储过程失败。sql: %s", sqlCmd.c_str());
        return false;
    }
    return true;
}

void NewStock::run_main_proc()
{
	RUN_LOG("begin run_main_proc. please wait...");

 	while(1) {
        sleep(3);
        Demo();
        return;
	}
}

string NewStock::AddTransShell(const string& cmd)
{
	string trans_head = "begin try begin tran; ";
	string trans_body = "commit tran end Try begin catch rollback tran end catch";
	return trans_head + cmd + trans_body;
}