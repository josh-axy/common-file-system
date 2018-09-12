#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/********************************  新建文件夹函数 **********************************/
/***********************************************************************************/
int mkdir(char* path, int mk_type)
{
	/**** 1、指定目录下新建一个目录项，判断是否已存在并提示 ********************/
	/**** 2、新建的同时更新文件控制块，各级父目录的更新时间怎么弄****/

		/**************** 解析输入的路径 **************/
	char name[13] = { 0 };
	int tmp_fcb_id;
	int i;
	for (i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			if (i == strlen(path) - 1)
			{
				cout << "Path ending incorrect. Please re-enter" << endl;
			}
			if (i != 0)
				path[i] = '\0';
			break;
		}
	}
	if (i < 0) {
		strcpy(name, path);
		path[0] = '\0';
		tmp_fcb_id = current_fcb_id;
	}
	else
	{
		strcpy(name, path + i + 1);
		tmp_fcb_id = path_to_fcb_id(path, DIR_T);
	}
	switch (tmp_fcb_id)
	{
	case -2:
	case -1:
		break;
	default:
		new_fcb(tmp_fcb_id, mk_type, name, NULL);
		break;
	}
	return 0;
}