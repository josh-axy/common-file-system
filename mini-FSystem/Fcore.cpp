#include "pch.h"
#include "Fcore.h"

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
int path_to_fcb_id(char *path, int f_type)
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
				if (f_type == DIR_T && fcb_list[next_id].file_type != DIR_T)
				{
					cout << "No such directory." << endl;
					return -1;
				}
				if (f_type == FILE_T && fcb_list[next_id].file_type != FILE_T)
				{
					cout << "No such file." << endl;
					return -1;
				}
				fcb_id = next_id;
				break;
			}
			next_id = fcb_list[fcb_id].filep[j];
		}
		if (next_id == -1)
		{
			if (f_type == DIR_T)
			{
				cout << "No such directory." << endl;
			}
			else if (f_type == FILE_T)
			{
				cout << "No such file." << endl;
			}
			else
			{
				cout << "No such directory or file." << endl;
			}
			return -1;
		}
	}
	return fcb_id;
}

/*创建fcb项*/
int new_fcb(int dir_fcb_id, char fcb_type, char *name, char *file_path)
{
	int free_id;
	int child;
	int fcb_id_tmp = dir_fcb_id;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "New fcb failed." << endl;
		return -2;
	}
	/*查重名*/
	for (child = 2; fcb_list[fcb_id_tmp].filep[child] != -1; child++)
	{
		if (child == EXT_CB)
		{
			fcb_id_tmp = fcb_list[fcb_id_tmp].filep[child];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[fcb_id_tmp].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
	}
	/*分配空白FCB*/
	free_id = sys.freefcb_id;
	fcb_list[fcb_id_tmp].filep[child] = free_id;
	sys.freefcb_id = fcb_list[free_id].filep[0];
	sys.last_write_fcb = free_id;
	fcb_list[fcb_id_tmp].edit_time = current_time();
	/*初始化空白FCB*/
	fcb_list[free_id].file_type = DIR_T;
	fcb_list[free_id].create_time = current_time();
	fcb_list[free_id].edit_time = fcb_list[free_id].create_time;
	fcb_list[free_id].filep[0] = fcb_id_tmp;
	fcb_list[free_id].filep[1] = free_id;
	/*写硬盘*/
	update_fcb(free_id);
	update_fcb(fcb_id_tmp);
	update_sys();
	/*转到空白FCB*/
	fcb_id_tmp = free_id;
	if (child == EXT_CB)
	{
		//变更块类型
		fcb_list[fcb_id_tmp].file_type = EXT_T;
		//分配空白FCB
		free_id = sys.freefcb_id;
		fcb_list[fcb_id_tmp].filep[1] = free_id;//扩展块从下标1开始链接
		sys.freefcb_id = fcb_list[free_id].filep[0];
		sys.last_write_fcb = free_id;
		//初始化空白FCB
		fcb_list[free_id].file_type = DIR_T;
		fcb_list[free_id].create_time = current_time();
		fcb_list[free_id].edit_time = fcb_list[free_id].create_time;
		fcb_list[free_id].filep[0] = fcb_id_tmp;
		fcb_list[free_id].filep[1] = free_id;
		strcpy(fcb_list[free_id].filename, name);
		/*写硬盘*/
		update_fcb(free_id);
		update_fcb(fcb_id_tmp);
		update_sys();
		/*转到空白FCB*/
		fcb_id_tmp = free_id;
	}
	else
	{
		strcpy(fcb_list[fcb_id_tmp].filename, name);
		update_fcb(fcb_id_tmp);
	}
	if (fcb_type == FILE_T)
	{
		fcb_list[fcb_id_tmp].file_type = FILE_T;
		update_fcb(fcb_id_tmp);
	}
	fflush(fp);
	return 0;
}

/*移动FCB*/
int move_fcb(int dir_fcb_id, int fcb_id, char *name, char* file_path)
{
	int child;
	int tmp_fcb_id = dir_fcb_id;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int drop;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "Update fcb failed." << endl;
		return -2;
	}
	if (name[0] == '\0')
	{
		cout << "Empty name. Please re-enter" << endl;
	}
	/*查重名并寻找新filep位置*/
	for (child = 2; fcb_list[tmp_fcb_id].filep[child] != -1; child++)
	{
		if (child == EXT_CB)
		{
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[child];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[tmp_fcb_id].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
	}
	/*变更路径*/
	//fcb_list[fcb_id].filep[0] = 0;
	if (tmp_fcb_id != fcb_list[fcb_id].filep[0])
	{
		/*链接新路径*/
		fcb_list[tmp_fcb_id].filep[child] = fcb_id;
		fcb_list[fcb_id].filep[0] = dir_fcb_id;
		update_fcb(tmp_fcb_id);
		update_fcb(fcb_id);
		/*删除原有路径*/
		for (origin_child = 2; fcb_list[origin_fcb_id].filep[origin_child] != -1; origin_child++)
		{
			if (origin_child == EXT_CB)
			{
				origin_fcb_id = fcb_list[origin_fcb_id].filep[origin_child];
				origin_child = 1;
			}
			if (fcb_list[origin_fcb_id].filep[origin_child] == fcb_id)
			{
				//向前移动filep
				for (drop = origin_child; fcb_list[origin_fcb_id].filep[drop] != -1; drop++)
				{
					if (drop == EXT_CB)
					{
						origin_fcb_id = fcb_list[origin_fcb_id].filep[drop];
						fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[drop - 1] == fcb_list[origin_fcb_id].filep[1];
						update_fcb(origin_fcb_id);
						drop = 1;
					}
					fcb_list[origin_fcb_id].filep[drop] = fcb_list[origin_fcb_id].filep[drop + 1];
					if (fcb_list[origin_fcb_id].filep[drop] == -1)
					{
						break;
					}
				}
				if (drop == 1)
				{
					//断开extCB链接
					fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[EXT_CB] = -1;
					update_fcb(fcb_list[origin_fcb_id].filep[0]);
					//释放extCB
					fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
					update_fcb(origin_fcb_id);
					sys.freefcb_id = origin_fcb_id;
					update_sys();
				}
				else
				{
					update_fcb(origin_fcb_id);
				}
				break;
			}
		}

	}
	fcb_list[fcb_id].edit_time = current_time();
	strcpy(fcb_list[fcb_id].filename, name);
	if (fcb_list[fcb_id].file_type == FILE_T)
	{

	}
	/*写磁盘*/
	update_fcb(fcb_id);
	return 0;
}

/*释放fcb项*/
int drop_fcb(int fcb_id, int r_mode)
{
	int child;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int drop;
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		cout << "Drop FCB failed." << endl;
		return -2;
	}
	if (fcb_list[fcb_id].filep[2]!=-1)
	{
		if (r_mode == TRUE)
		{
			for (int i = 2; fcb_list[fcb_id].filep[i] != -1; i++)
			{
				drop_fcb(fcb_list[fcb_id].filep[i], TRUE);
			}
		}
		else
		{
			cout << "FCB not empty. Drop fcb failed!" << endl;
			return -1;
		}
	}

	/*FCB置空标识*/
	fcb_list[fcb_id].file_type = EMPTY_T;
	fcb_list[fcb_id].filep[0] = -1;
	fcb_list[sys.last_freefcb_id].filep[0] = fcb_id;
	/*写磁盘*/
	update_fcb(sys.last_freefcb_id);
	update_fcb(fcb_id);
	sys.last_freefcb_id = fcb_id;
	update_sys();
	/*删除原有链接*/
	for (origin_child = 2; fcb_list[origin_fcb_id].filep[origin_child] != -1; origin_child++)
	{
		if (origin_child == EXT_CB)
		{
			origin_fcb_id = fcb_list[origin_fcb_id].filep[origin_child];
			origin_child = 1;
		}
		if (fcb_list[origin_fcb_id].filep[origin_child] == fcb_id)
		{
			//向前移动filep
			for (drop = origin_child; fcb_list[origin_fcb_id].filep[drop] != -1; drop++)
			{
				if (drop == EXT_CB)
				{
					fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[drop - 1] == fcb_list[origin_fcb_id].filep[1];
					/*写入FCB头块*/
					update_fcb(origin_fcb_id);
					origin_fcb_id = fcb_list[origin_fcb_id].filep[drop];
					drop = 1;
				}
				fcb_list[origin_fcb_id].filep[drop] = fcb_list[origin_fcb_id].filep[drop + 1];
				if (fcb_list[origin_fcb_id].filep[drop] == -1)
				{
					break;
				}
			}
			//空extCB
			if (drop == 1)
			{
				//断开extCB链接
				fcb_list[fcb_list[origin_fcb_id].filep[0]].filep[EXT_CB] = -1;
				update_fcb(fcb_list[origin_fcb_id].filep[0]);
				//释放extCB
				fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
				update_fcb(origin_fcb_id);
				sys.freefcb_id = origin_fcb_id;
				update_sys();
			}
			else
			{
				/*写入FCB*/
				update_fcb(origin_fcb_id);
			}
			break;
		}
	}
	return 0;
}

/*更新FCB*/
void update_fcb(int update_fcb_id)
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update FCB failed.\n");
		return;
	}
	fcb_list[update_fcb_id].edit_time = current_time();
	fseek(fp, FCB_POS(update_fcb_id), SEEK_SET);
	fwrite(&fcb_list[update_fcb_id], sizeof(FCB), 1, fp);
	fflush(fp);
	return;
}

int write_ib(int ib_id, int ib_size, FILE* fp_tmp)
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update system failed.\n");
		return 0;
	}
	sys.last_write_time = current_time();
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return 0;
}
int move_ib(int ib_id, FILE* fp_tmp)
{
	return 0;
}
int erase_ib(int ib_id)
{
	return 0;
}

/*更新系统块*/
void update_sys()
{
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Update system failed.\n");
		return;
	}
	sys.last_write_time = current_time();
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return;
}
