# include "ftds_ex.h"
# include "log.hpp"
# include <string>
using namespace std;

FtdsEx::FtdsEx()
{
}


FtdsEx::~FtdsEx()
{
}

bool FtdsEx::Init(const char* db_server,
				  const char* db_user,
				  const char* db_pswd, 
				  const char *db_name,
				  uint32_t login_timeout,
				  uint32_t rw_timeout
				  )
{
	_server_name = db_server;
	_db_name = db_name;
	_user_name = db_user;
	_pass_word = db_pswd;
	_login_timeout = login_timeout;
	_rw_timeout = rw_timeout;
	return OpenDB(_server_name.c_str(), _user_name.c_str(), _pass_word.c_str(), 
		_db_name.c_str(), _login_timeout, _rw_timeout);
}

bool FtdsEx::OpenDB(const char *server, const char *userName, const char *password, 
					const char *DBName, uint32_t login_timeout, uint32_t rw_timeout)
{
	try
	{
		return FTDS::OpenDB(server, userName, password, DBName, login_timeout, rw_timeout);
	}
	catch (TdsException& e)
	{
		ERR_LOG("OpenDB error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("OpenDB error!!!");
		return false;
	}
}

void FtdsEx::CloseDB()
{
	try
	{
		return FTDS::CloseDB();
	}
	catch (TdsException& e)
	{
		ERR_LOG("CloseDB error!!!, %s", e.what());
		return;
	}
	catch (...)
	{
		ERR_LOG("CloseDB error!!!");
		return;
	}
}

bool FtdsEx::ExecSQL(const char *strSQL)
{	
	try
	{
		if (!ReOpen())
			throw TdsException(TdsException::TDS_EXCEPT_OPEN_ERR, "execDML(): error handle => FAIL");
		return FTDS::ExecSQL(strSQL);
	}
	catch (TdsException& e)
	{
		ERR_LOG("ExecSQL error !!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("ExecSQL error!!!");
		return false;
	}
}

int  FtdsEx::GetResult()
{
	try
	{
		return FTDS::GetResult();
	}
	catch (TdsException& e)
	{
		ERR_LOG("GetResult error!!!, %s", e.what());
		return -1;
	}
	catch (...)
	{
		ERR_LOG("GetResult error!!!");
		return -1;
	}
}

bool FtdsEx::BindColumn(int column, int varLen, char *varAddr)
{
	try
	{
		return FTDS::BindColumn(column, varLen, varAddr);
	}
	catch (TdsException& e)
	{
		ERR_LOG("BindColumn error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("BindColumn error!!!");
		return false;
	}
}
bool FtdsEx::BindColumn(int column, int varLen, int *varAddr)
{
	try
	{
		return FTDS::BindColumn(column, varLen, varAddr);
	}
	catch (TdsException& e)
	{
		ERR_LOG("BindColumn error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("BindColumn error!!!");
		return false;
	}
}
bool FtdsEx::MoveNextRow()
{
	try
	{
		return FTDS::MoveNextRow();
	}
	catch (TdsException& e)
	{
		ERR_LOG("MoveNextRow error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("MoveNextRow error!!!");
		return false;
	}
}
int  FtdsEx::CountRow()
{
	try
	{
		return FTDS::CountRow();
	}
	catch (TdsException& e)
	{
		ERR_LOG("CountRow error!!!, %s", e.what());
		return -1;
	}
	catch (...)
	{
		ERR_LOG("CountRow error!!!");
		return -1;
	}
}

bool FtdsEx::IsCount()
{
	try
	{
		return FTDS::IsCount();
	}
	catch (TdsException& e)
	{
		ERR_LOG("IsCount error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("IsCount error!!!");
		return false;
	}
}
bool FtdsEx::Cancel()
{
	try
	{
		return FTDS::Cancel();
	}
	catch (TdsException& e)
	{
		ERR_LOG("Cancel error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("Cancel error!!!");
		return false;
	}
}

bool FtdsEx::ReOpen()
{
	try
	{
		if (!ErrHandle())
			return true;
		return OpenDB(_server_name.c_str(), _user_name.c_str(), _pass_word.c_str(), 
			_db_name.c_str(), _login_timeout, _rw_timeout);
	}
	catch (TdsException& e)
	{
		ERR_LOG("ReOpen error!!!, %s", e.what());
		return false;
	}
	catch (...)
	{
		ERR_LOG("ReOpen error!!!");
		return false;
	}
}
	
struct FTDS_COL
{
    FTDS_COL()
    {
        buffer = NULL;
        type = 0;
        size = 0;
        status = 0;
    }
    char* buffer;
    int type;
    int size;
    int status;
};

void ClearDatas(vector<FTDS_COL *> &col_values, vector<string> &col_keys)
{
    for (unsigned int i = 0; i < col_values.size(); i++)
    {
        FTDS_COL* pcol = col_values[i];
        delete[] pcol->buffer;
        delete pcol;
    }
    col_keys.clear();
    col_values.clear();
}

int FtdsEx::CountCol() {
	return int(dbnumcols(m_dbprocess));
}

bool FtdsEx::ExecSQL(const char *sql, Json::Value& json_ret)
{
	// LOG(INFO)<<"数据库操作开始";
    json_ret = Json::Value();
	string msg = "";

	if (!ExecSQL(sql))
		return false;
	int erc, rowCode;
	int recordSet = 0;
	bool ret_flag = true;
	while ( (ret_flag) && (( erc = GetResult() ) != 0) ) {
		if ( 1 != erc) {
			ret_flag = false;
			break;
		}
		struct COL {
			char *name;
			char *buffer;
			int type, size, status;
			COL(){
				name = NULL;
				buffer = NULL;
				type = 0;
				size = 0;
				status = 0;
			}
		} *columns, *pcol;

		int ncols = CountCol();
		columns = (struct COL *)calloc(ncols, sizeof(struct COL));
		if (NULL == columns) {
			ERR_LOG("call calloc() failed");
			ret_flag = false;
			break;
		}

		for (pcol = columns; pcol - columns < ncols; pcol++) {
			int c = pcol - columns + 1;

			pcol->name = dbcolname(m_dbprocess, c); 
			pcol->type = dbcoltype(m_dbprocess, c); 
			pcol->size = dbcollen(m_dbprocess, c); 

			if (SYBCHAR != pcol->type) {
				pcol->size = dbwillconvert(pcol->type, SYBCHAR);
				if (pcol->size == TRUE) {
					pcol->size = 255;
				}
			}

			if ((pcol->buffer = (char *)calloc(1, pcol->size + 1)) == NULL) {
				ERR_LOG("call calloc() failed");
				ret_flag = false;
				break;
			}

			// erc = dbbind(m_dbprocess, c, NTBSTRINGBIND, pcol->size+1, (BYTE*)pcol->buffer);
			bool bbind = BindColumn(c, pcol->size+1,pcol->buffer);
			if (!bbind) {
				ERR_LOG("call dbbind() failed");
				ret_flag = false;
				break;
			}
		}

		if ( not ret_flag ) {
			for (pcol=columns; pcol - columns < ncols; pcol++) {
				free(pcol->buffer);
			}
			free(columns);
		}
		else {
			while (( rowCode = MoveNextRow() ) != 0) {
				if ( 1 != rowCode) {
					ret_flag = false;
					break;
				}
				Json::Value json_item;
				for (pcol = columns; pcol - columns < ncols; pcol++) {
					char *buffer = pcol->buffer;
					char *name = pcol->name;   
					json_item[name] = buffer;
				}   				
				json_ret[recordSet].append(json_item);
			}
			for (pcol=columns; pcol - columns < ncols; pcol++) {
				free(pcol->buffer);
			}
			free(columns);
			recordSet++;
		}
	}
	if ( not ret_flag )
		CloseDB();
	return ret_flag;

}