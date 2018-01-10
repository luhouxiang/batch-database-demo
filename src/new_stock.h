#ifndef _KDS_NEWSTOCK_H
#define _KDS_NEWSTOCK_H
# include <json/json.h>
# include <string>
# include <map>
# include <vector>
# include "common/cfg.h"

using std::string;

class FtdsEx;
class NewStock
{
public:
	NewStock(FtdsEx *dbmsconn, const Options& options);
	virtual ~NewStock(){}

	void run_main_proc();

    void Demo();
private:
	string AddTransShell(const string& cmd); // 加入事务处理
    bool _execSQL(const std::string sqlCmd, Json::Value& json_ret);

    string StrInsertHead();
    string StrInsertBody(const string& code, const string& name);
private:
	FtdsEx*	_dbmsconn;
	Options             m_options;  
};

#endif//_KDS_GAME_H