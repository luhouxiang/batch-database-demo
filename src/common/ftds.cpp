#include <stdio.h>
#include "ftds.h"
#include "log.hpp"

FTDS::FTDS():m_dbprocess(FAIL)
{

}


FTDS::~FTDS()
{ 
	CloseDB();
}


bool FTDS::OpenDB(const char *server, const char *userName, const char *password, 
				  const char *DBName, uint32_t login_timeout, uint32_t rw_timeout)
{
	if (dbinit() == FAIL)
	{
		throw TdsException(TdsException::TDS_EXCEPT_INIT_ERR,
			"FTDS::OpenDB(): dbinit => FAIL");
	}

    LOGINREC *loginrec = dblogin(); 
    if(loginrec == NULL)
    {
        throw TdsException(TdsException::TDS_EXCEPT_LOGIN_ERR, 
            "FTDS::OpenDB(): dblogin => FAIL");
    }
    DBSETLUSER(loginrec, userName);         
    DBSETLPWD(loginrec, password);  

    dbsetlogintime(login_timeout);
    dbsettime(rw_timeout);

    m_dbprocess = dbopen(loginrec, server);
	dbloginfree(loginrec);
    if(m_dbprocess == FAIL)  
    {  
        throw TdsException(TdsException::TDS_EXCEPT_OPEN_ERR, 
            "FTDS::OpenDB(): dbopen => FAIL");
    }  

    if(dbuse(m_dbprocess, DBName) == FAIL)  
    {
		CloseDB();
        throw TdsException(TdsException::TDS_EXCEPT_DBUSE_ERR, 
            "FTDS::OpenDB(): dbuse => FAIL");
    }
    return true;  
}


void FTDS::CloseDB()
{
	RUN_LOG("begin dbclose, %x", m_dbprocess);
	if ((m_dbprocess != FAIL))
	{
		dbfreebuf(m_dbprocess);
		dbclose(m_dbprocess);	
		m_dbprocess = FAIL;
		dbexit();
	}
    return;  
}

bool FTDS::ExecSQL(const char *strSQL){
	if (m_dbprocess == FAIL )
	{
		throw TdsException(TdsException::TDS_EXCEPT_DBEXEC_ERR, 
            "execDML(): error handle => FAIL");
	}

	if (dbdead(m_dbprocess))
	{
		CloseDB();
		throw TdsException(TdsException::TDS_EXCEPT_DBEXEC_ERR, 
            "execDML(): error dbdead => FAIL");
	}

	if (dbcmd(m_dbprocess, strSQL) != SUCCEED) {
        CloseDB();
   		throw TdsException(TdsException::TDS_EXCEPT_DBCMD_ERR, 
            "execDML(): dbcmd => FAIL");
	}

	if (dbsqlexec(m_dbprocess) != SUCCEED) {
		CloseDB();
		throw TdsException(TdsException::TDS_EXCEPT_DBEXEC_ERR, 
            "execDML(): dbsqlexec => FAIL");
	}
	return true;
}


int FTDS::GetResult()
{
    DBINT result_code = dbresults(m_dbprocess);
    if(result_code == SUCCEED)
    {
       return 1;
    }
    else if(result_code == NO_MORE_RESULTS)
    {
       return 0;
    }
    else //if(result_code == FAIL)
    {
	   CloseDB();
	   throw TdsException(TdsException::TDS_EXCEPT_GETRESULT_ERR,
		   "execDML(): dbcancel => FAIL");
    }
}

bool FTDS::BindColumn(int column, int varLen, char *varAddr)
{
    if(dbbind(m_dbprocess, column, NTBSTRINGBIND, (DBINT)varLen, (BYTE*)varAddr) == SUCCEED)
    {
    	return true;   
    }
    else
    {
        CloseDB();
        throw TdsException(TdsException::TDS_EXCEPT_BIND_ERR, 
           "BindColumn(): dbbind => FAIL");
    }
}


bool FTDS::BindColumn(int column, int varLen, int *varAddr)
{
    if(dbbind(m_dbprocess, column, INTBIND, (DBINT)varLen, (BYTE*)varAddr) == SUCCEED)
    {
    	return true;   
    }
    else
    {
        CloseDB();
        throw TdsException(TdsException::TDS_EXCEPT_BIND_ERR, 
           "BindColumn(): dbbind => FAIL");
    }
}

int FTDS::MoveNextRow(){
	int row_code = dbnextrow(m_dbprocess);
	if (row_code == NO_MORE_ROWS) {
		return 0;
	}else if (row_code == REG_ROW) {
		return 1;
	}else if (row_code == BUF_FULL) {
        CloseDB();
        throw TdsException(TdsException::TDS_EXCEPT_BUFFULL_ERR, 
            "nextRow(): dbnextrow => BUF_FULL");
	}else {	
        CloseDB();
        throw TdsException(TdsException::TDS_EXCEPT_NEXTROW_ERR, 
            "nextRow(): dbnextrow => FAIL");
	}
}


int FTDS::CountRow()
{
    return int(DBCOUNT(m_dbprocess));
}


bool FTDS::IsCount()
{
    if(dbiscount(m_dbprocess) == TRUE)
    {
        return true;
    }
    else
    {
        return false;
    } 
}

bool FTDS::Cancel(){
	dbcancel(m_dbprocess);
	return true;
}

bool FTDS::ErrHandle()
{
    return (FAIL == m_dbprocess);
}
