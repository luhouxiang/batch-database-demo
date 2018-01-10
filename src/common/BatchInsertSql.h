# ifndef __KDS_MOBILE_STOCK_BATCHINSERTSQL_H__
# define __KDS_MOBILE_STOCK_BATCHINSERTSQL_H__
# include <string>
using namespace std;

class BatchInsertSql
{
private:
    int m_count;
    string m_strSql;
public:
    BatchInsertSql(void);
    ~BatchInsertSql(void);

    void SetHead(const string head) {
        m_count = 0;
        m_strSql = head;
    }

    virtual void addBody(const string& body) 
    {
        if (m_count == 0)
            m_strSql += body;
        else 
            m_strSql += "," + body;

        m_count++;
    }

    virtual string GetSql() 
    { 
        if(m_count == 0)
            return string("");
        return m_strSql + ";";
    }
};


#endif
