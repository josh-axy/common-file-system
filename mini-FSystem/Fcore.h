#pragma once
#ifndef F_CORE_H
#define F_CORE_H
#include "Fbase.h"
/*内核函数定义*/
void print_time(int timestamp);
int current_time();
int path_to_fcb_id(char *path, int f_type);
int cp_s2c(char path1[], char path2[]);
int cp_c2s(char path1[], char path2[]);
int cp_c2c(char path1[], char path2[]);
int new_fcb(int dir_fcb_id, char fcb_type, char *name, char* file_path);
int move_fcb(int dir_fcb_id, int fcb_id, char *name, char* file_path);
int drop_fcb(int fcb_id, int r_mode);
void update_fcb(int fcb_id);
int get_free_ib(int ib_size);
int write_ib(int ib_id, int ib_size, FILE* fp_tmp);
int move_ib(int src_ib_id, int dst_ib_id);
int erase_ib(int ib_id, int ib_size);
void update_sys();
/*内核函数定义*/
#endif // !F_CORE_H
