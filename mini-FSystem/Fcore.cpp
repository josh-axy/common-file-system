#include "pch.h"
#include "Fcore.h"

/***************************************获取时间函数*********************************/
/************************************************************************************/
int current_time()
{
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

/***************************************打印时间函数*********************************/
/************************************************************************************/
void print_time(int timestamp)
{
	int ntime;
	char sTime[20] = { 0 };						//该函数必须包含<time.h><string.h>头文件
	time_t ftime = timestamp + (8 * 3600);
	ntime = (int)ftime;
	tm* pTime = gmtime(&ftime);
	strftime(sTime, 80, "%Y-%m-%d %H:%M:%S", pTime);//方法1
	cout << sTime;
	return;
}
/************************************************************************************/

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
		if (str[i] == '/')
		{
			if (i && str[i - 1] == '/')
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
			::memset(input_tmp, 0, sizeof(input_tmp));
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
				fcb_id = fcb_list[fcb_id].filep[EXT_CB];
				j = 1;
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

/*创建fcb项并写入IB*/
int new_fcb(int dir_fcb_id, int fcb_type, char *name, char *src_f_path)
{
	int free_id;
	int child;
	int tmp_fcb_id = dir_fcb_id;
	int f_fcb_id;
	int f_len;
	IB_Disk* file_info = NULL;
	FILE* fp_tmp;
	if (fp == NULL)
	{
		cout << "New fcb failed." << endl;
		return -2;
	}
	//分配IB
	if (fcb_type == FILE_T)
	{
		if (src_f_path)
		{
			//获取文件字节数
			if (src_f_path[1] == ':' && src_f_path[2] == '\\')
			{
				if (src_f_path[3] == '\\')
				{
					cout << "Path format error." << endl;
					return -4;
				}
				if ((fp_tmp = fopen(src_f_path, "rb+")) == NULL)
				{
					cout << "File doesn't exist." << endl;
					return -4;
				}
				fseek(fp_tmp, 0, SEEK_END);
				f_len = ftell(fp_tmp);
				fseek(fp_tmp, 0, SEEK_SET);
			}
			else
			{
				f_fcb_id = path_to_fcb_id(src_f_path, FILE_T);
				switch (f_fcb_id)
				{
				case -2:
				case -1:
					cout << "File doesn't exist." << endl;
					return -4;
				default:
					f_len = fcb_list[f_fcb_id].file_size;
					break;
				}
				if ((fp_tmp = fopen(fs_path, "rb+")) == NULL)
				{
					cout << "File doesn't exist." << endl;
					return -4;
				}
				fseek(fp_tmp, IB_POS(fcb_list[f_fcb_id].file_block_id), SEEK_SET);
			}
		}
		else
		{
			f_len = BLOCK_SIZE - sizeof(IB_Disk);
			fp_tmp = NULL;
		}
		file_info = write_ib(f_len, fp_tmp);
		if (file_info == NULL)
		{
			return -3;
		}
	}
	/*查重名*/
	for (child = 2; fcb_list[tmp_fcb_id].filep[child] != -1; child++)
	{
		if (child == EXT_CB)
		{
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[EXT_CB];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[tmp_fcb_id].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
	}
	/*分配空白FCB*/
	free_id = sys.freefcb_id;
	fcb_list[tmp_fcb_id].filep[child] = free_id;
	sys.freefcb_id = fcb_list[free_id].filep[0];
	sys.last_write_fcb = free_id;
	/*初始化空白FCB*/
	fcb_list[free_id].file_type = fcb_type;
	fcb_list[free_id].create_time = current_time();
	fcb_list[free_id].filep[0] = tmp_fcb_id;
	fcb_list[free_id].filep[1] = free_id;
	/*写硬盘*/
	write_fcb(free_id);
	write_fcb(tmp_fcb_id);
	write_sys();
	/*转到空白FCB*/
	tmp_fcb_id = free_id;
	if (child == EXT_CB)
	{
		//变更块类型
		fcb_list[tmp_fcb_id].file_type = EXT_T;
		//分配空白FCB
		free_id = sys.freefcb_id;
		fcb_list[tmp_fcb_id].filep[1] = free_id;//扩展块从下标1开始链接
		sys.freefcb_id = fcb_list[free_id].filep[0];
		sys.last_write_fcb = free_id;
		//初始化空白FCB
		fcb_list[free_id].file_type = fcb_type;
		fcb_list[free_id].create_time = current_time();
		fcb_list[free_id].filep[0] = tmp_fcb_id;
		fcb_list[free_id].filep[1] = free_id;
		/*写硬盘*/
		write_fcb(tmp_fcb_id);
		write_sys();
		/*转到空白FCB*/
		tmp_fcb_id = free_id;
	}
	if (file_info != NULL)
	{
		fcb_list[tmp_fcb_id].file_block_id = file_info->block_id;
		fcb_list[tmp_fcb_id].file_size = f_len;
		sys.last_write_ib = file_info->block_id;
		write_sys();
		delete file_info;
	}
	strcpy(fcb_list[tmp_fcb_id].filename, name);
	write_fcb(tmp_fcb_id);
	fflush(fp);
	return 0;
}

/*移动FCB*/
int move_fcb(int dir_fcb_id, int fcb_id, char *name, char* file_path)
{
	int child;
	int free_fcb_id;
	int tmp_fcb_id = dir_fcb_id;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int same_origin = 0;;
	int drop;
	if (fp == NULL)
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
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[EXT_CB];
			child = 1;
		}
		if (strcmp(name, fcb_list[fcb_list[tmp_fcb_id].filep[child]].filename) == 0)
		{
			cout << "Name duplicated. Please re-enter." << endl;
			return -1;
		}
		if (fcb_id == fcb_list[tmp_fcb_id].filep[child])
		{
			same_origin = 1;
		}
	}
	if (child == EXT_CB)
	{
		//分配空白FCB
		free_fcb_id = sys.freefcb_id;
		fcb_list[tmp_fcb_id].filep[EXT_CB] = free_fcb_id;//链接extCB
		sys.freefcb_id = fcb_list[free_fcb_id].filep[0];
		sys.last_write_fcb = free_fcb_id;
		//初始化空白FCB
		fcb_list[free_fcb_id].file_type = EXT_T;
		fcb_list[free_fcb_id].create_time = current_time();
		fcb_list[free_fcb_id].edit_time = fcb_list[free_fcb_id].create_time;
		fcb_list[free_fcb_id].filep[0] = tmp_fcb_id;
		fcb_list[free_fcb_id].filep[1] = free_fcb_id;
		/*写硬盘*/
		write_fcb(free_fcb_id);
		write_fcb(tmp_fcb_id);
		write_sys();
		/*转到空白扩展FCB*/
		tmp_fcb_id = free_fcb_id;
		child = 1;
	}
	/*变更路径*/
	//fcb_list[fcb_id].filep[0] = 0;
	if (!same_origin)
	{
		/*链接新路径*/
		fcb_list[tmp_fcb_id].filep[child] = fcb_id;
		fcb_list[fcb_id].filep[0] = dir_fcb_id;
		write_fcb(tmp_fcb_id);
		write_fcb(fcb_id);
		/*回溯父目录节点*/
		while (fcb_list[origin_fcb_id].file_type == EXT_CB)
		{
			origin_fcb_id = fcb_list[origin_fcb_id].filep[0];
		}
		/*删除原链接*/
		for (origin_child = 2; fcb_list[origin_fcb_id].filep[origin_child] != -1; origin_child++)
		{
			if (origin_child == EXT_CB)
			{
				origin_fcb_id = fcb_list[origin_fcb_id].filep[EXT_CB];
				origin_child = 1;
			}
			if (fcb_list[origin_fcb_id].filep[origin_child] == fcb_id)
			{
				//向前移动filep
				for (drop = origin_child; fcb_list[origin_fcb_id].filep[drop] != -1; drop++)
				{
					if (drop == EXT_CB)
					{
						fcb_list[origin_fcb_id].filep[EXT_CB - 1] = fcb_list[fcb_list[origin_fcb_id].filep[EXT_CB]].filep[1];
						write_fcb(origin_fcb_id);
						origin_fcb_id = fcb_list[origin_fcb_id].filep[EXT_CB];
						drop = 1;
						/*子文件父节点重定向*/
						fcb_list[fcb_list[origin_fcb_id].filep[1]].filep[0] = fcb_list[origin_fcb_id].filep[0];
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
					write_fcb(fcb_list[origin_fcb_id].filep[0]);
					//释放extCB
					fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
					write_fcb(origin_fcb_id);
					sys.freefcb_id = origin_fcb_id;
					write_sys();
				}
				else
				{
					write_fcb(origin_fcb_id);
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
	write_fcb(fcb_id);
	return 0;
}

/*释放fcb项*/
int drop_fcb(int fcb_id, int r_mode)
{
	int child;
	int origin_fcb_id = fcb_list[fcb_id].filep[0];
	int origin_child;
	int drop;
	int next_fcb_id;
	if (fp == NULL)
	{
		cout << "Drop FCB failed." << endl;
		return -2;
	}
	//递归调用
	if (fcb_list[fcb_id].filep[2]!=-1)
	{
		if (r_mode == TRUE)
		{
			next_fcb_id = fcb_id;
			for (int i = 2; fcb_list[next_fcb_id].filep[i] != -1; i++)
			{
				if (i == EXT_CB)
				{
					next_fcb_id = fcb_list[next_fcb_id].filep[EXT_CB];
				}
				drop_fcb(fcb_list[next_fcb_id].filep[i], TRUE);
			}
		}
		else
		{
			cout << "FCB not empty. Drop fcb failed!" << endl;
			return -1;
		}
	}
	if (fcb_list[fcb_id].file_type == FILE_T)
	{
		drop_ib(fcb_id);
	}
	/*FCB置空标识并链接*/
	fcb_list[fcb_id].file_type = EMPTY_T;
	fcb_list[fcb_id].filep[0] = -1;
	fcb_list[sys.last_freefcb_id].filep[0] = fcb_id;
	/*写磁盘*/
	write_fcb(sys.last_freefcb_id);
	write_fcb(fcb_id);
	sys.last_freefcb_id = fcb_id;
	write_sys();
	/*回溯父目录节点*/
	while (fcb_list[origin_fcb_id].file_type == EXT_CB)
	{
		origin_fcb_id = fcb_list[origin_fcb_id].filep[0];
	}
	/*删除原链接*/
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
					fcb_list[origin_fcb_id].filep[EXT_CB - 1] = fcb_list[fcb_list[origin_fcb_id].filep[EXT_CB]].filep[1];
					/*写入FCB头块*/
					write_fcb(origin_fcb_id);
					origin_fcb_id = fcb_list[origin_fcb_id].filep[EXT_CB];
					drop = 1;
					/*子文件父节点重定向*/
					fcb_list[fcb_list[origin_fcb_id].filep[1]].filep[0] = fcb_list[origin_fcb_id].filep[0];
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
				write_fcb(fcb_list[origin_fcb_id].filep[0]);
				//释放extCB
				fcb_list[origin_fcb_id].filep[0] = sys.freefcb_id;
				write_fcb(origin_fcb_id);
				sys.freefcb_id = origin_fcb_id;
				write_sys();
			}
			else
			{
				/*写入FCB*/
				write_fcb(origin_fcb_id);
			}
			break;
		}
	}
	return 0;
}

/*更新FCB*/
void write_fcb(int update_fcb_id)
{
	if (fp == NULL)
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

/*获取空白IB*/
IB_Disk* get_free_ib(int ib_size)
{
	IB_Disk* ib = new IB_Disk;
	if (fp == NULL)
	{
		printf("No system mounted. Get free ib failed.\n");
		return NULL;
	}
	ib->block_id = -1;
	ib->size = ib_size;
	ib = free_ib_tree.Search(ib);
	return ib;
}

/*获取IB信息*/
IB_Disk* get_ib_info(int ib_id)
{
	IB_Disk* p_ib = new IB_Disk;
	if (fp == NULL)
	{
		printf("Read IB failed.\n");
		return 0;
	}
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fread(p_ib, sizeof(IB_Disk), 1, fp);

	return p_ib;
}

/*获取IB内容*/
int get_ib_content(int ib_id)
{
	return 0;
}

/*写入IB*/
IB_Disk* write_ib(int f_size, FILE* fp_tmp)
{
	int ib_id;
	char *buffer;
	int blk_size;
	int rest_size;
	int i;
	IB_Disk* p_old_ib;
	IB_Disk* p_new_ib = new IB_Disk;
	IB_Disk* ib_tmp = new IB_Disk;
	IB_AVLNode* ib_avl_tmp;

	if (fp == NULL)
	{
		printf("Write IB failed.\n");
		return NULL;
	}
	//将字节数转化为块数
	blk_size = (f_size + sizeof(IB_Disk)) / BLOCK_SIZE;
	if ((f_size + sizeof(IB_Disk)) % BLOCK_SIZE)
	{
		blk_size++;
	}
	rest_size = 0;
	/*检查IB空间*/
	p_old_ib = get_free_ib(blk_size);
	while (p_old_ib == NULL)
	{
		if (blk_size == 0 || rest_size > blk_size)
		{
			cout << "Disk full. Delete something to create." << endl;
			return NULL;
		}
		blk_size--;
		rest_size++;
		p_old_ib = get_free_ib(blk_size);
	}

	/*开始写*/
	ib_id = p_old_ib->block_id;
	ib_tmp->block_id = ib_id;
	ib_tmp->size = blk_size;
	ib_tmp->last_id = -1;
	ib_tmp->next_id = -1;
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fwrite(ib_tmp, sizeof(IB_Disk), 1, fp);
	if (fp_tmp == NULL)
	{
		buffer = new char[f_size];
		memset(buffer, 0, f_size * sizeof(char));
		fwrite(buffer, f_size, 1, fp);
		delete buffer;
	}
	else
	{
		fwrite(fp_tmp, f_size, 1, fp);
	}
	fflush(fp);
	/*更新IB块*/
	if (p_old_ib->size > blk_size)
	{
		p_new_ib->block_id = p_old_ib->block_id + blk_size;
		p_new_ib->size = p_old_ib->size - blk_size;
		p_new_ib->next_id = p_old_ib->next_id;
		p_new_ib->last_id = p_old_ib->last_id;
		/*更新IB树节点*/
		update_free_ib(p_old_ib, p_new_ib);
	}
	/*当前空闲IB段用光*/
	else
	{
		free_ib_tree.Delete(p_old_ib);
		if (p_old_ib->last_id != 0)
		{
			p_new_ib = get_ib_info(p_old_ib->last_id);
			p_new_ib->next_id = p_old_ib->next_id;
			ib_avl_tmp = free_ib_tree.Search(p_new_ib->block_id, p_new_ib->size);
			ib_avl_tmp->SetValue(*p_new_ib);
			fseek(fp, IB_POS(p_new_ib->block_id), SEEK_SET);
			fwrite(p_new_ib, sizeof(IB_Disk), 1, fp);
			fflush(fp);
		}
		else
		{
			sys.freeib_id = p_old_ib->next_id;
			write_sys();
		}
		if (p_old_ib->next_id != 0)
		{
			p_new_ib = get_ib_info(p_old_ib->next_id);
			p_new_ib->last_id = p_old_ib->last_id;
			ib_avl_tmp = free_ib_tree.Search(p_new_ib->block_id, p_new_ib->size);
			ib_avl_tmp->SetValue(*p_new_ib);
			fseek(fp, IB_POS(p_new_ib->block_id), SEEK_SET);
			fwrite(p_new_ib, sizeof(IB_Disk), 1, fp);
			fflush(fp);
		}
		else
		{
			sys.last_freeib_id = p_old_ib->last_id;
			write_sys();
		}
	}
	delete p_old_ib;
	delete p_new_ib;
	cout << "first:" << sys.freeib_id << " last:" << sys.last_freeib_id << endl;
	for (int i = 1; i <= 20; i++)
	{
		//装载空闲信息块头
		fseek(fp, IB_POS(i), SEEK_SET);
		fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
		cout << free_ib_tmp.block_id << ' ' << free_ib_tmp.size << ' ' << free_ib_tmp.last_id << ' ' << free_ib_tmp.next_id << endl;
	}
	return ib_tmp;
}

/*移动IB*/
int move_ib(int src_ib_id, int dst_ib_id)
{
	return 0;
}

/*释放IB*/
void drop_ib(int fcb_id)
{
	IB_Disk* ib_tmp = new IB_Disk;
	ib_tmp->block_id = fcb_list[fcb_id].file_block_id;
	ib_tmp = get_ib_info(ib_tmp->block_id);
	ib_tmp->last_id = 0;
	ib_tmp->next_id = 0;
	erase_ib(ib_tmp->block_id, 1);
	update_free_ib(NULL, ib_tmp);
	delete ib_tmp;
	return;
}

/*擦除IB*/
void erase_ib(int ib_id, int size)
{
	char *buffer = new char[size*BLOCK_SIZE];
	if (fp == NULL)
	{
		printf("Update IB failed.\n");
		return;
	}
	memset(buffer, 0, size * BLOCK_SIZE * sizeof(char));
	fseek(fp, IB_POS(ib_id), SEEK_SET);
	fwrite(buffer, size * BLOCK_SIZE, 1, fp);
	fflush(fp);
	delete buffer;
	return;
}

/*更新空闲IB树与磁盘*/
void update_free_ib(IB_Disk* old_ib, IB_Disk* new_ib)
{
	IB_Disk *ib_tmp;
	IB_AVLNode *ib_avl_tmp;
	if (fp == NULL)
	{
		printf("Update IB failed.\n");
		return;
	}
	if (old_ib == NULL)
	{
		link_ib(new_ib);
	}
	else
	{
		fseek(fp, IB_POS(new_ib->block_id), SEEK_SET);
		fwrite(new_ib, sizeof(IB_Disk), 1, fp);
		fflush(fp);
		free_ib_tree.Delete(old_ib);
	}
	free_ib_tree.Insert(new_ib);
	if (new_ib->last_id != 0)
	{
		ib_tmp = get_ib_info(new_ib->last_id);
		ib_tmp->next_id = new_ib->block_id;
		fseek(fp, IB_POS(ib_tmp->block_id), SEEK_SET);
		fwrite(ib_tmp, sizeof(IB_Disk), 1, fp);
		fflush(fp);
		ib_avl_tmp = free_ib_tree.Search(ib_tmp->block_id, ib_tmp->size);
		ib_avl_tmp->SetValue(*ib_tmp);
		delete ib_tmp;
	}
	else
	{
		sys.freeib_id = new_ib->block_id;
		write_sys();
	}
	if (new_ib->next_id != 0)
	{
		ib_tmp = get_ib_info(new_ib->next_id);
		ib_tmp->last_id = new_ib->block_id;
		fseek(fp, IB_POS(ib_tmp->block_id), SEEK_SET);
		fwrite(ib_tmp, sizeof(IB_Disk), 1, fp);
		fflush(fp);
		ib_avl_tmp = free_ib_tree.Search(ib_tmp->block_id, ib_tmp->size);
		ib_avl_tmp->SetValue(*ib_tmp);
		delete ib_tmp;
	}
	else
	{
		sys.last_freeib_id = new_ib->block_id;
		write_sys();
	}
	return;
}

/*连接或合并相邻空IB*/
void link_ib(IB_Disk* new_ib)
{
	IB_AVLNode* op_ib;
	IB_Disk* free_ib_disk;
	IB_AVLNode* free_ib = new IB_AVLNode;
	IB_AVLNode* ib_avl_tmp;
	queue<IB_AVLNode*> path;
	IB_AVLNode* id_left = NULL;
	IB_AVLNode* id_right = NULL;

	op_ib = id_tree.GetRoot();
	path.push(op_ib);
	while (!path.empty())
	{
		op_ib = path.front();
		path.pop();
		if (op_ib == NULL)
			continue;
		free_ib_disk = get_ib_info(op_ib->GetId());
		free_ib->SetValue(*free_ib_disk);
		//右侧空白块
		if (new_ib->block_id < free_ib->GetId())
		{
			path.push(op_ib->lchild());
			new_ib->next_id = free_ib->GetId();
			if (new_ib->block_id + new_ib->size == free_ib->GetId())
			{
				new_ib->next_id = free_ib->Next();
				new_ib->size += free_ib->GetSize();
				id_right = new IB_AVLNode(*free_ib);
				erase_ib(free_ib->GetId(), 1);
			}
		}
		//左侧空白块
		else
		{
			path.push(op_ib->rchild());
			new_ib->last_id = free_ib->GetId();
			if (free_ib->GetId() + free_ib->GetSize() == new_ib->block_id)
			{
				new_ib->last_id = free_ib->Last();
				new_ib->block_id = free_ib->GetId();
				new_ib->size += free_ib->GetSize();
				id_left = new IB_AVLNode(*free_ib);
				erase_ib(free_ib->GetId(), 1);
			}
		}
	}
	if (id_right)
	{
		free_ib_tree.Delete(id_right);
		delete id_right;
	}
	if (id_left)
	{
		free_ib_tree.Delete(id_left);
		delete id_left;
	}
	fseek(fp, IB_POS(new_ib->block_id), SEEK_SET);
	fwrite(new_ib, sizeof(IB_Disk), 1, fp);
	fflush(fp);
	delete free_ib;
	return;
}

/*更新系统块*/
void write_sys()
{
	if ((fp = fopen(fs_path, "rb+")) == NULL)
	{
		printf("Write system failed.\n");
		return;
	}
	sys.last_write_time = current_time();
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fwrite(&sys, sizeof(superBlock), 1, fp);
	fflush(fp);
	return;
}
