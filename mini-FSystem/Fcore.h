#pragma once
#ifndef F_CORE_H
#define F_CORE_H
#include "Fbase.h"
#include <queue>
/*内核函数定义*/
void print_time(int timestamp);
int current_time();
void print_time(int timestamp);
int path_to_fcb_id(char *path, int f_type);
int new_fcb(int dir_fcb_id, int fcb_type, char *name, char* file_path);
int move_fcb(int dir_fcb_id, int fcb_id, char *name, char* file_path);
int drop_fcb(int fcb_id, int r_mode);
void write_fcb(int fcb_id);
IB_Disk* write_ib(int f_size, FILE* fp_tmp);
void update_free_ib(IB_Disk* old_ib, IB_Disk* new_ib);
int move_ib(int src_ib_id, int dst_ib_id);
void drop_ib(int fcb_id);
void erase_ib(int ib_id, int size);
IB_Disk* get_free_ib(int ib_size);
IB_Disk* get_ib_info(int ib_id); //获取IB信息
void link_ib(IB_Disk* ib);
int get_ib_content(int ib_id);
void write_sys();
/*内核函数定义*/
#endif // !F_CORE_H
