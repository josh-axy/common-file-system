#pragma once
#ifndef F_BASE_H
#define F_BASE_H
#include "pch.h"
#include <iostream>
#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

/*超级块结构*/
typedef struct Super_block {
	int fcb_Location;			//第一个文件控制块的位置
	int ib_location;			//第一个数据块的位置
	int fcb_size;				//fcb大小
	int blk_size;				//块大小
	int superblock_size;		//超级块大小
	int superblock_location;	//超级块位置
	int last_write_ib;			//最后写入的信息块
	int last_write_fcb;			//最后写入的文件控制块
	int freefcb_id;				//下一个空闲控制块位置 (0,258047)
	int freeib_id;				//下一个空闲信息块位置（4033，262144）
	int last_freefcb_id;		//最后一个空闲控制块位置
	int last_freeib_id;			//最后一个空闲信息块位置
	int rootfcb_id;				//根目录位置
	int last_write_time;		//最后一次写入时间
}superBlock;

/*控制单元结构*/
typedef struct F_CB {
	int create_time;		//文件创建时间
	int edit_time;			//文件更新时间
	int file_size;			//文件占用块数
	int file_block_id;		//文件头块编号
	int filep[FCB_WIDTH];			//子目录编号数组
	char filename[14];		//文件名字
	char file_type;			//文件类型
}FCB;

/*扩展控制单元结构*/
typedef struct ext_CB {
	int filep[16];			//子目录编号
}extCB;

/*文件单元结构*/
typedef struct FREE_I_B {
	int block_id;			//自己的块号
	int length;				//持续长度
	int last_block;			//前面块头的块号
	int next_block;			//后面块头的块号
	int left;				//更小块
	int right;				//更大块
}FreeIB;

typedef struct FULL_I_B {
	int block_id;
	int lenght;
	int next_block;
}FullIB;

/*全局变量*/
extern FILE *fp;
extern char mini_path[100];
extern int current_fcb_id;
extern int sys_mounted;
extern char current_path[128];
extern superBlock sys;
extern FCB fcb_list[FCB_NUM];
extern FreeIB ib_tmp[2];
/*全局变量*/

using namespace std;

#endif // !F_BASE_H


