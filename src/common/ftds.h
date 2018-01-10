#ifndef __KDS_MOBILE_STOCK_FTDS_H__
#define __KDS_MOBILE_STOCK_FTDS_H__


#include <string>
#include <sybfront.h> //freetds头文件  
#include <sybdb.h> //freetds 

class FTDS
{
  public:
    FTDS();
    virtual ~FTDS();

    bool OpenDB(const char *server, const char *userName, const char *password, 
		const char *DBName,	uint32_t login_timeout, uint32_t rw_timeout);
    void CloseDB();
    bool ExecSQL(const char *strSQL);
    int  GetResult();
    bool BindColumn(int column, int varLen, char *varAddr);
    bool BindColumn(int column, int varLen, int *varAddr);
    bool MoveNextRow();
    int  CountRow();
    bool IsCount();
	bool Cancel();
	bool ErrHandle();
	DBPROCESS* GetDbProcess() const { return m_dbprocess; }

protected:
    DBPROCESS *m_dbprocess;
};

class TdsException : public std::exception
{
//Add by huihai.shen at 2017-04-18 for solve memory leak bug begin
public:
    enum ETdsExceptionType
    {
        TDS_EXCEPT_NONE_ERR = 0,
        TDS_EXCEPT_INIT_ERR,
        TDS_EXCEPT_LOGIN_ERR,
        TDS_EXCEPT_OPEN_ERR,
        TDS_EXCEPT_DBUSE_ERR,
        TDS_EXCEPT_DBCMD_ERR,
        TDS_EXCEPT_DBEXEC_ERR,
        TDS_EXCEPT_GETRESULT_ERR,
        TDS_EXCEPT_BIND_ERR,
        TDS_EXCEPT_BUFFULL_ERR,
        TDS_EXCEPT_NEXTROW_ERR,
        TDS_EXCEPT_UNKOWN_ERR
    };
//Add by huihai.shen at 2017-04-18 for solve memory leak bug end
public:
    //Modify by huihai.shen at 2017-04-18 for solve memory leak bug begin
	//explicit TdsException(const char* msg) {
    explicit TdsException(ETdsExceptionType err_type, const char* msg) {
        except_type = err_type;
    //Modify by huihai.shen at 2017-04-18 for solve memory leak bug end
		errMsg.assign(msg);
	}
	virtual const char* what() const throw() {
		return errMsg.c_str();
	}
	
    //Add by huihai.shen at 2017-04-18 for solve memory leak bug begin
    virtual ETdsExceptionType exceptType() const throw() {
		return except_type;
	}
    //Add by huihai.shen at 2017-04-18 for solve memory leak bug end

	virtual ~TdsException() throw()
	{
		
	}

private:
	std::string errMsg;

    //Add by huihai.shen at 2017-04-18 for solve memory leak bug begin
    ETdsExceptionType except_type;
    //Add by huihai.shen at 2017-04-18 for solve memory leak bug end
};

#endif
