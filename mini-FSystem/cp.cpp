#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/************************  复制函数主函数 ******************************************/
/***********************************************************************************/
void cp(char path1[], char path2[])
{
	int len1, len2;
	len1 = strlen(path1);
	len2 = strlen(path2);
	if (len1 < 3 || len2 < 3)
	{
		printf("The input command is incorrect. Please re-enter\n");
		return;
	}
	/*
	if (!((path1[len1] == '.' || path1[len1 - 3] == '.' || path1[len1 - 4] == '.') && (path2[len2 - 2] == '.' || path2[len2 - 3] == '.' || path2[len2 - 4] == '.')))
	{
		printf("输入指令格式有错，请核对后重新输入！\n");
		return;
	}*/
	if (path1[1] == ':'&&path1[2] == '\\')
	{
		if (path2[0] == '\\')
		{
			cp_s2c(path1, path2);
		}
		else
		{
			printf("The input command is incorrect. Please re-enter\n");
			return;
		}
	}
	else if (path2[1] == ':'&&path2[2] == '\\')
	{
		if (path1[0] != '\\')
		{
			cp_c2s(path1, path2);
		}
		else
		{
			printf("The input command is incorrect. Please re-enter\n");
			return;
		}
	}
	else if (path1[1] == ':'&&path1[2] == '\\'&&path2[1] == ':'&&path2[2] == '\\')
	{
		printf("The input command is incorrect. Please re-enter\n");
		return;
	}
	else if (path1[0] == '\\' && path2[0] == '\\')
	{
		cp_c2c(path1, path2);
	}
	return;
}

int cp_s2c(char path1[], char path2[])
{
	return 0;
}
int cp_c2s(char path1[], char path2[])
{
	return 0;
}
int cp_c2c(char path1[], char path2[])
{

	return 0;
}
