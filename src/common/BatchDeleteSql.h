# ifndef __KDS_MOBILE_STOCK_BATCHDELETESQL_H__
# define __KDS_MOBILE_STOCK_BATCHDELETESQL_H__
# include <string>
# include <vector>
using namespace std;

class BatchDeleteSql
{
private:
    string m_tableName;
    string m_whereName;
    vector<string> m_whereValues;
public:
    BatchDeleteSql(void);
    ~BatchDeleteSql(void);

    void SetHead(const string& tableName,const string& whereName) {
        m_whereValues.clear();
        m_tableName = tableName;
        m_whereName = whereName;
    }

    void addValue(const string& whereValue) 
    {
        m_whereValues.push_back(whereValue);
    }

    virtual string GetSql() 
    { 
        if(m_whereValues.empty())
            return string("");

        string sql_cmd = "delete from " + m_tableName;
        sql_cmd = sql_cmd +  " where " + m_whereName + " in('" + m_whereValues[0] + "'";

        for (unsigned int i = 1; i < m_whereValues.size(); i++)
        {
            sql_cmd += ",'" + m_whereValues[i] + "'";
        }
        sql_cmd += ");";
        return sql_cmd;
    }
};


#endif
