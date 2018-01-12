#include <iostream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "../src/common/string_utility.h"
/******************************************************************************
@源程序名称: test_string_util.cpp                                                
@软件著作权: 
@模块名称  : 
@作者      : luhx
@审核      : 
@版本      : 1.0
@修改日期  : 
@功能说明  : 测试字符串模块
@相关文档  : 
*******************************************************************************/

using namespace std;
class test_string_util : public ::testing::Test
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

TEST_F(test_string_util, to_string)
{
	
	EXPECT_EQ("3", StringUtility::to_string(3));
    EXPECT_EQ("3.12", StringUtility::to_string(3.12));

    EXPECT_EQ("-3", StringUtility::to_string(-3));
    EXPECT_EQ("-3.12", StringUtility::to_string(-3.12));

    EXPECT_EQ("0", StringUtility::to_string(0));
}