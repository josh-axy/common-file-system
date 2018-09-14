#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/************************  复制函数主函数 ******************************************/
/***********************************************************************************/
void cp(char path1[], char path2[])
{
	int len1, len2;
	int dir_fcb_id;
	len1 = strlen(path1);
	len2 = strlen(path2);
	if (len1 < 3 && len2 < 3)
	{
		printf("Path format incorrect. Please re-enter.\n");
		return;
	}
	if (path2[2] != '\\')
	{
		make(path2, FILE_T, path1);
	}
	else
	{

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
