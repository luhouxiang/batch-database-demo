# ifndef __KDS_MOBILE_STOCK_BATCHUPDATESQL_H__
# define __KDS_MOBILE_STOCK_BATCHUPDATESQL_H__
# include <string>
# include <vector>
using std::string;
using std::vector;
class BatchUpdateSql
{
public:
    BatchUpdateSql(void);
    ~BatchUpdateSql(void);
public:
    void SetHead(const string& tableName, const string& whereName, const string& name) 
    {
        m_columnNames.clear();
        m_columnNames.push_back(name);
        m_tableName = tableName;
        m_whereName = whereName;
    }

    void SetHead(const string& tableName, const string& whereName, const string& name1, const string& name2) 
    {
        m_columnNames.clear();
        m_columnNames.push_back(name1);
        m_columnNames.push_back(name2);
        m_tableName = tableName;
        m_whereName = whereName;
    }

    void addValue(const string& whereValue,const string& columnValue) 
    {
        vector<string> columnValues;
        columnValues.push_back(columnValue);
        m_columnValuesList.push_back(columnValues);
        m_whereValues.push_back(whereValue);
    }

    void addValue(const string& whereValue,const string& columnValue1,const string& columnValue2) 
    {
        vector<string> columnValues;
        columnValues.push_back(columnValue1);
        columnValues.push_back(columnValue2);
        m_columnValuesList.push_back(columnValues);
        m_whereValues.push_back(whereValue);
    }

    virtual string GetSql() 
    { 
        if(m_whereValues.empty())
            return string("");
        string sql_cmd = "update " + m_tableName + " set ";
        vector<string>str_list(m_columnNames.size());
        for (unsigned int i = 0; i < m_columnValuesList.size(); i++)
        {
            vector<string>& strvalues = m_columnValuesList[i];
            for (unsigned int j = 0; j < strvalues.size(); j++)
            {
                string &str = str_list[j];
                if (i == 0)
                {
                    str = m_columnNames[j] + "=CASE " + m_whereName;
                    str += " when '" + m_whereValues[i] + "' then '" + strvalues[j] + "'";
                }
                else
                {
                    str += " when '" + m_whereValues[i] + "' then '" + strvalues[j] + "'";
                }
            }
        }

        for (unsigned int i = 0; i < str_list.size(); i++)
        {
            sql_cmd += str_list[i];
            if(i != str_list.size()-1) 
            {
                sql_cmd += " END,";
            }
            else
            {
                sql_cmd += " END ";
            }
        }

        string strwhere = "where " + m_whereName + " in('" + m_whereValues[0] + "'";
        for (unsigned int i = 1; i < m_whereValues.size(); i++)
        {
            strwhere += ",'" + m_whereValues[i] + "'";
        }
        strwhere += ");";
        sql_cmd += strwhere;
        return sql_cmd;
    }

protected:
    string m_whereName;
    vector<string> m_whereValues;
    vector<string> m_columnNames;
    vector<vector<string> > m_columnValuesList;
    string m_tableName;
};


#endif