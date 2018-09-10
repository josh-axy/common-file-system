// mini-FSysten.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <iostream>
#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

using namespace std;


/*宏定义*/
#define BLOCK_SIZE 4096		//每个块的大小
#define BLOCK_NUM 262144	//文件系统中块的数量
#define FCB_SIZE 64			//文件控制块长度
#define FCB_NUM 258048		//控制单元的数量为4032 * 64 = 258048个，隶属于（1，4032），寻找控制块直接访问该块所在数组(0,258047)
#define IB_NUM 258111		//文件信息块数量为258111个，隶属于（4033，262144）
#define OP_LEN 128			//每次输入指令的最大长度
#define MAX_DIR_LEN 256		//最长目录长度
#define FCB_Location 1		//文件控制块起始位置
#define IB_Location 4033	//信息块起始位置
#define ROOT_FCB_ID 0		//根目录FCB编号
#define SUPER_Loacation 0	//系统块位置
/*宏定义*/


/*全局变量*/
FILE *fp;
char mini_path[100] = "D:\\";
int current_fcb_id;
/*全局变量*/

/*指令函数定义*/
int format();
int mount(char arr[]);
int input(char* op1, char* op2, char* op3);
void attr(char *);
void cp(char path1[], char path2[]);
void del(char* path);
void cd(char* path);
void ls(char* path);
void create(char *name);
void judge(char* op1, char* op2, char* op3);
void move(char *path1, char *path2);
int mkdir(char* path);
void help(char * order);
void readtxt(char *);
void more(char *);
/*函数定义*/


/*功能函数定义*/
int current_time();
int path_to_fcb_id(char *path);
int cp_s2c(char path1[], char path2[]);
int cp_c2s(char path1[], char path2[]);
int cp_c2c(char path1[], char path2[]);
int new_fcb(int dir_fcb, char fcb_type, char *name, FILE* fp_tmp);
int drop_fcb(int fcb_id);
/*功能函数定义*/


/*超级块结构*/
typedef struct Super_block {
	int fcb_Location;			//第一个文件控制块的位置
	int ib_location;			//第一个数据块的位置
	int fcb_size;				//fcb大小
	int blk_size;				//块大小
	int superblock_size;		//超级块大小
	int superblock_location;	//超级块位置
	int last_ib;				//最后分配的信息块
	int last_fcb;				//最后分配的文件控制块
	int freefcb_id;				//下一个空闲控制单元位置 (0,258047)
	int rootfcb_id;				//根目录位置
	int freeib_id;				//下一个空闲信息块位置（4033，262144）
	int last_write_time;		//最后一次写入时间
}superBlock;
superBlock sys;

/*控制单元结构*/
typedef struct F_CB {
	int create_time;		//文件创建时间
	int edit_time;			//文件更新时间
	int file_size;			//文件占用块数
	int file_block_id;		//文件头块编号
	int filep[8];			//子目录编号数组
	char filename[13];		//文件名字
	char is_delete;			//删除标记
	char file_type;			//文件类型
}FCB;
FCB fcb_list[FCB_NUM];

/*扩展控制单元结构*/
typedef struct ext_CB {
	int filep[16];			//子目录编号
}EXTB;

/*文件单元结构*/
typedef struct I_B {
	int block_num;			//自己的块号
	int length;				//持续长度
	int last_num;			//前面块头的块号
	int next_num;			//后面块头的块号
}IB;
IB ib_tmp[2];

/***************************************获取时间函数*********************************/
/************************************************************************************/
int current_time()
{		                                         //nowtime:当前系统时间,last_write_time[15]:最后写入时间，用来接收当前系统时间
	int ntime;
	char sTime[20] = { 0 };						//该函数必须包含<time.h><string.h>头文件
	time_t ftime = time(NULL)/* + (8 * 3600)*/;
	ntime = ftime;
	//tm* pTime = gmtime(&ftime);
	//strftime(sTime, 80, "%Y%m%d%H%M%S", pTime);//方法1
	//cout << sTime << endl;
	//strcpy(datetime, sTime);
	return ntime;
}
/************************************************************************************/
/***************************************获取时间函数*********************************/

/*创建fcb项*/
int new_fcb(int dir_fcb_id, char fcb_type, char *name, FILE* fp_tmp)
{
	int free;
	int next;
	int fcb_id_tmp = dir_fcb_id;
	for (int i = 2; fcb_list[fcb_id_tmp].filep[i] != -1; i++)
	{
		if (strcmp(name, fcb_list[fcb_list[fcb_id_tmp].filep[i]].filename) == 0)
		{
			return -1;
		}
	}
	free = sys.freefcb_id;
	next = fcb_list[free].filep[0];
	fcb_list[free].file_type = 0;
	fcb_list[free].create_time = current_time();
	fcb_list[free].edit_time = fcb_list[free].create_time;
	fcb_list[free].is_delete = 0;
	fcb_list[free].filep[0] = dir_fcb_id;
	fcb_list[free].filep[1] = free;
	strcpy(fcb_list[free].filename, name);
	if (fcb_type ==	1)
	{
	}
	return 0;
}

/*释放fcb项*/
int drop_fcb(int fcb_id)
{

}

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
			if (str[i - 1] == '\\')
			{
				return -2;
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
		strcpy(name[k], input_tmp);
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
		if (name[i][0]=='.')
		{
			if (name[i][1]=='.' && name[i][2]=='\0')
			{
				fcb_id = fcb_list[fcb_id].filep[0];
				continue;
			}
			else if(name[i][1]=='\0')
			{
				continue;
			}
		}
		for (j = 2; fcb_list[fcb_id].filep[j] != -1; j++)
		{
			next_id = fcb_list[fcb_id].filep[j];
			if (strcmp(fcb_list[next_id].filename, name[i]) == 0)
			{
				fcb_id = next_id;
			}
		}
		if (next_id == -1)
		{
			fcb_id = -1;
			break;
		}
	}
	return fcb_id;
}

/*Creat 1G space*/
void create(char *name)
{
	int i;
	for (i = 0; name[i] != '\0'; i++)
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= '0' && name[i] <= '9')))
		{
			printf("System name input is not legal!\n");
			return;
		}
	}
	//文件系统所在路径
	mini_path[3] = '\0';
	strcat(mini_path, name);

	/*.DAT并不是一种标准文件。许多软件都使用这个扩展名，但文件含义不同。
		DAT文件，可以按照扩展名来看就是DATA的意思，即数据文件，这类文件并没有进行绝对化的定义。*/
	strcat(mini_path, ".dat");

	//以读/写方式打开或建立一个二进制文件，允许读和写。（为NULL则打不开）
	if ((fp = fopen(mini_path, "wb+")) == NULL)
	{
		printf("System name input is not legal!\n");
		return;
	}
	//开辟1G空间
	fseek(fp, 1024 * 1024 * 1024, 0);
	putw(0, fp);
	fclose(fp);
	//判断文件是否被创建
	if ((fp = fopen(mini_path, "r+")) != NULL)
		printf("\nCreate file success.\n");
	fclose(fp);
	format();
}

/*Format 1G space*/
int format() {
	superBlock block;
	if (strcmp(mini_path, "D:\\") == 0)
	{
		printf("Please create a mini_FS first!\n");
		return -1;
	}
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Format failed.\n");
		return -1;
	}
	rewind(fp);
	fseek(fp, SUPER_Loacation, SEEK_SET);
	//系统块初始化
	block.fcb_Location = FCB_Location;
	block.ib_location = IB_Location;
	block.fcb_size = FCB_SIZE;
	block.blk_size = BLOCK_SIZE;
	block.superblock_size = sizeof(superBlock);
	block.superblock_location = 0;
	block.last_ib = -1;
	block.last_fcb = 0;
	block.freefcb_id = 1;
	block.rootfcb_id = 0;
	block.freeib_id = 0;
	block.last_write_time = current_time();
	fwrite(&block, sizeof(superBlock), 1, fp);

	//空文件头初始化
	IB ib;
	fseek(fp, IB_Location * BLOCK_SIZE, SEEK_SET);
	ib.block_num = 4033;
	ib.length = IB_NUM;
	ib.last_num = -1;		//-1表示此为空文件信息块头
	ib.next_num = -1;		//-1表示此为最后一段空文件信息段
	fwrite(&ib, sizeof(IB), 1, fp);

	//控制块初始化
	FCB fcb;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(fcb.filename, "");			//根目录文件名
	fcb.create_time = current_time();	//创建时间
	fcb.edit_time = fcb.create_time;	//编辑时间
	fcb.file_type = 2;					//空白
	fcb.file_size = 0;					//大小为0
	fcb.is_delete = 1;					//已删除
	fcb.file_block_id = -1;				//文件夹
	for (int i = 0; i < 7; i++)
		fcb.filep[i] = -1;				//暂时不存在子目录
	for (int i = 0; i < FCB_NUM; i++)
	{
		fcb.filep[0] = i + 1;
		fwrite(&fcb, sizeof(FCB), 1, fp);
	}

	//根目录初始化
	FCB root;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(root.filename, "\\");			//根目录文件名
	root.create_time = current_time();		//创建时间
	root.edit_time = root.create_time;		//编辑时间
	root.file_type = 0;						//文件夹
	root.file_size = 0;						//空文件夹，大小为0
	root.is_delete = 0;						//未删除
	root.file_block_id = -1;				//文件夹
	for (int i = 0; i < 7; i++)
		root.filep[i] = -1;					//暂时不存在子目录
	fwrite(&root, sizeof(FCB), 1, fp);
	printf("\nFormat success.\n");
	/*
	FCB rootd;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	fread(&rootd, sizeof(FCB), 1, fp);
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	for (int i = 0; i < FCB_NUM; i++)
	{
		fread(&fcb_list[i], sizeof(FCB), 1, fp);
	}*/
	return 0;
}

/*Mount used Mini_FS*/
int mount(char arr[])
{
	mini_path[3] = '\0';
	strcat(mini_path, arr);
	strcat(mini_path, ".dat");
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Mount failed.\n");
		return -1;
	}
	//装载系统块
	fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
	fread(&sys, sizeof(superBlock), 1, fp);
	//装载控制单元
	//rewind(fp);
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	for (int i = 0; i < FCB_NUM; i++)
	{
		fread(&fcb_list[i], sizeof(FCB), 1, fp);
	}
	//装载空闲信息块头
	fseek(fp, IB_Location * BLOCK_SIZE + sys.freeib_id * BLOCK_SIZE, 0);
	fread(&ib_tmp, sizeof(IB), 1, fp);

	printf("Mount success.\n");
	fclose(fp);
	return 0;
}

/*Input orders!!!*/
int input(char* op1, char* op2, char* op3)
{
	char op[OP_LEN + 1];
	int flag[100];
	int i = 0, j = 0;
	int flag1 = 0;
	memset(flag, 0, sizeof(flag));
	for (i = 0;; i++)
	{
		if (i == OP_LEN)
		{
			if (flag1 == 0)
			{
				printf("The input command character is too long. Please enter the command within %d.\n.", i);
				flag1 = 1;
			}
			i = 0;
		}
		op[i] = getchar();
		if (op[i] == '\n')
		{
			if (flag1 == 1)
				return -1;
			op[i] = '\0';
			break;
		}
	}
	if (op[0] != ' ')
		flag[j++] = 0;
	for (i = 0; op[i] != '\0'; i++)
	{
		if ((op[i] == ' '&&op[i + 1] != ' ') || (op[i] != ' ' && (op[i + 1] == ' ' || op[i + 1] == '\0')))
			flag[j++] = i + 1;
	}
	for (i = flag[0]; i < flag[1]; i++)
		op1[i - flag[0]] = op[i];
	op1[i - flag[0]] = '\0';
	for (i = flag[2]; i < flag[3]; i++)
		op2[i - flag[2]] = op[i];
	op2[i - flag[2]] = '\0';
	for (i = flag[4]; i < flag[5]; i++)
		op3[i - flag[4]] = op[i];
	op3[i - flag[4]] = '\0';
	int len;
	len = strlen(op);
	if ((flag[5] != 0) && flag[5] < len)
	{
		printf("Command syntax is incorrect!\n");
		return -1;
	}
	return 0;
}

/*Judge users' order*/
void judge(char *op1, char *op2, char *op3)
{
	//Create Command
	if (strcmp(op1, "create") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			create(op2);
	}

	//Mount Command
	else if (strcmp(op1, "mount") == 0)
	{

		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			mount(op2);
	}

	//Copy Command
	else if (strcmp(op1, "cp") == 0)
	{
		if (op2[0] == '\0' || op3[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			cp(op2, op3);
	}

	//Format Command
	else if (strcmp(op1, "fmt") == 0)
	{

		if (strlen(op2) != 0)
			printf("Command syntax is incorrect!\n");
		else
			format();
	}

	//Show Contents Command
	else if (strcmp(op1, "ls") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			ls(op2);
	}

	//Enter Contents Command
	else if (strcmp(op1, "cd") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			cd(op2);
	}

	//Delete Command
	else if (strcmp(op1, "dl") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			del(op2);  //1
	}

	//Show Text Command
	else if (strcmp(op1, "tp") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			readtxt(op2);   //1
	}

	//Paging Show Text Command
	else if (strcmp(op1, "more") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			more(op2);  //1
	}

	//Show File Attribute Command
	else if (strcmp(op1, "attr") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			attr(op2);   //1
	}

	//Show Help Command
	else if (strcmp(op1, "help") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		help(op2);  //0
	}

	//Create Content Command
	else if (strcmp(op1, "mkdir") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			mkdir(op2);   //1
	}

	//Move Command
	else if (strcmp(op1, "mv") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			move(op2, op3);   //1
	}

	else
	{
		printf("The input command is incorrect. Please re-enter.\n");
	}
}

void attr(char *)
{

}

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

void del(char* path)
{

}

void cd(char* path)
{

}

/*Show the contents for users*/
void ls(char* path) 
{
	char ch;
	if (path[0] == '\0')
	{
		
	}
}

void move(char *path1, char *path2)
{

}

/***********************************************************************************/
/********************************  新建文件夹函数 **********************************/
/***********************************************************************************/
int mkdir(char* path)
{
	/**** 1、指定目录下新建一个目录项，判断是否已存在并提示 ********************/
	/**** 2、新建的同时更新文件控制块，各级父目录的更新时间怎么弄****/

		/**************** 解析输入的路径 **************/
	FILE* fp_tmp;
	char name[13] = { 0 };
	int now_fcb_id;
	int i;
	if ((fp_tmp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Mount Failed.\n");
		return -1;
	}
	for (i = strlen(path) - 1; i >= 0; i++)
	{
		if (path[i] == '\\' && i != 0)
		{
			path[i] = '0';
			break;
		}
	}
	strcpy(name, path + i);
	now_fcb_id = path_to_fcb_id(path);
	switch (now_fcb_id)
	{
	case -2:
		cout << "Path format error. Please re-enter the path." << endl;
		break;
	case -1:
		cout << "No such directory." << endl;
	default:

		break;
	}
	return 0;
}

/*Show help to users*/
void help(char * order)
{
	char ch;
	if ((fp = fopen("D:\\help.txt", "rb+")) == NULL)
	{
		printf("Open help file error\n");
		getchar();
		exit(1);
	}
	if (order[0] == '\0')
	{
		ch = fgetc(fp);
		while (ch != EOF && ch != '1')
		{
			if (ch >= 'A'&&ch <= 'Z')
				ch = ch + 'a' - 'A';
			putchar(ch);
			ch = fgetc(fp);
		}
		fclose(fp);
	}
	else
	{
		char info[300];
		while (1)
		{
			fscanf(fp, "%s", info);
			if ((strcmp("end", info)) == 0)
				break;
			if ((strcmp(order, info)) == 0)
			{
				while (1)
				{
					fgets(info, 300, fp);
					if (info[0] == '1')
						break;
					printf("%s", info);
				}
				fclose(fp);
				return;
			}
		}
		printf("The input command is incorrect. Please re-enter!\n");
		fclose(fp);
	}
}

void readtxt(char *)
{

}

void more(char *)
{

}

int main()
{
	char op1[OP_LEN];
	char op2[OP_LEN];
	char op3[OP_LEN];
	char sTime[20] = { 0 };
	__int64 ftime = time(NULL) + 8 * 3600;
	tm* pTime;
	pTime = gmtime(&ftime);
	strftime(sTime, 20, "%Y-%m-%d %H:%M:%S", pTime);
	printf("Welcome to Common MFS!\n\nSystem time: UTC+8 %s\n", sTime);
	//cout << current_time() << endl << sizeof(FCB) << endl;
	printf("\nGet help, please enter help!\n");
	//printf("\n\nIf you are familiar with the operation of this system, please ignore the second sentence!\n\n\n");
	printf("\nEnter the command character to operate, enter \"close\" to exit operation\n");
	while (1)
	{
		printf("Mini_FS>");
		if ((input(op1, op2, op3) == -1))
			continue;
		if (strcmp(op1, "close") == 0)
			break;
		judge(op1, op2, op3);
	}
	fclose(fp);
	printf("\nThank you for using COMMON Mini_File_System!\n");
}
