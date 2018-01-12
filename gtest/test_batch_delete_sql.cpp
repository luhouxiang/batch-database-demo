#include <iostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../src/common/BatchDeleteSql.h"
# include "../src/common/log.hpp"
/******************************************************************************
@源程序名称:                                               
@软件著作权: 
@模块名称  : 
@作者      : luhx
@审核      : 
@版本      : 1.0
@修改日期  : 
@功能说明  : 
@相关文档  : 
*******************************************************************************/

using namespace std;
class test_batch_delete_sql : public ::testing::Test
{
    virtual void SetUp()
    {
        cout << "SetUp" <<endl ;
    }

    virtual void TearDown()
    {
        cout << "TearDown" << endl;
    }

};

TEST_F(test_batch_delete_sql, to_string)
{
	BatchDeleteSql sql;
    sql.SetHead("TBL_DEMO", "CODE");
    sql.addValue("1");
    sql.addValue("2");

	EXPECT_EQ("delete from TBL_DEMO where CODE in('1','2');", sql.GetSql());
}