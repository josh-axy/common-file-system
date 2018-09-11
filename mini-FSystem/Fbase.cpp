#include "pch.h"
#include "Fbase.h"
/*全局变量*/
FILE *fp;
char mini_path[100] = "D:\\";
int current_fcb_id;
int sys_mounted = 0;
superBlock sys;
FCB fcb_list[FCB_NUM];
IB ib_tmp[2];