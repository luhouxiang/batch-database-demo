# include <stdio.h>
# include <stdlib.h>


//// 显示版本号
////// versionStr svn版本
////// nameStr 中文名称

void ShowVersion(const char* versionStr, const char* nameStr)
{
	if(versionStr == NULL || nameStr == NULL)
	{
		printf("警告！本副本在编译时使用的源代码没有纳入源代码版本控制系统。\n");
	}
	else
	{
		printf("%s v%s 版(%s,%s)\n", nameStr, versionStr, __DATE__, __TIME__); 
	}
}
