#include "pch.h"
#include "Fbase.h"
#include "Ftools.h"

using namespace std;

/***************************************获取时间函数*********************************/
/************************************************************************************/
int current_time()
{		                                         //nowtime:当前系统时间,last_write_time[15]:最后写入时间，用来接收当前系统时间
	int ntime;
	char sTime[20] = { 0 };						//该函数必须包含<time.h><string.h>头文件
	time_t ftime = time(NULL)/* + (8 * 3600)*/;
	ntime = (int)ftime;
	//tm* pTime = gmtime(&ftime);
	//strftime(sTime, 80, "%Y%m%d%H%M%S", pTime);//方法1
	//cout << sTime << endl;
	//strcpy(datetime, sTime);
	return ntime;
}
/************************************************************************************/
/***************************************获取时间函数*********************************/

/*路径 -> fcb编号*/
int path_to_fcb_id(char *path)
{
	char input_tmp[13] = { 0 };
	char name[100][13] = { 0 };
	char str[100];
	int len, abs = 0;
	int fcb_id = 0, next_id;
	int i = 0, j = 0, k = 0;

	strcpy(str, path);
	len = strlen(path);
	//拆分路径
	for (i = 0, j = 0, k = 0; i < len; i++)
	{
		if (str[i] == '\\')
		{
			if (i && str[i - 1] == '\\')
			{
				cout << "Path format incorrect. Please re-enter." << endl;
				return -2;
			}
			if (i == len - 1)
			{
				input_tmp[j] = '\0';
			}
			if (i == 0)
			{
				abs = 1;
			}
			else
			{
				strcpy(name[k++], input_tmp);
			}
			memset(input_tmp, 0, sizeof(input_tmp));
			j = 0;
		}
		else
		{
			input_tmp[j++] = str[i];
		}

	}
	if (j != 0)
	{
		strcpy(name[k++], input_tmp);
	}
	//起始路径定位
	if (abs)
	{
		fcb_id = sys.rootfcb_id;
	}
	else
	{
		fcb_id = current_fcb_id;
	}
	//启动搜索
	for (i = 0; i < k; i++)
	{
		if (name[i][0] == '.')
		{
			if (name[i][1] == '.' && name[i][2] == '\0')
			{
				fcb_id = fcb_list[fcb_id].filep[0];
				continue;
			}
			else if (name[i][1] == '\0')
			{
				continue;
			}
		}

		for (j = 2, next_id = fcb_list[fcb_id].filep[j]; next_id != -1; j++)
		{
			if (j == EXT_CB)
			{
				fcb_id = fcb_list[fcb_id].filep[j];
				j = 2;
			}
			if (strcmp(fcb_list[next_id].filename, name[i]) == 0)
			{
				fcb_id = next_id;
				break;
			}
			next_id = fcb_list[fcb_id].filep[j];
		}
		if (next_id == -1)
		{
			cout << "No such directory." << endl;
			return -1;
		}
	}
	return fcb_id;
}