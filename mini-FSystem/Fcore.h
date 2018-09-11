#pragma once
#ifndef F_CORE_H
#define F_CORE_H
#include "Fbase.h"
#include "Ftools.h"
/*内核函数定义*/
int cp_s2c(char path1[], char path2[]);
int cp_c2s(char path1[], char path2[]);
int cp_c2c(char path1[], char path2[]);
int new_fcb(int dir_fcb_id, char fcb_type, char *name, FILE* fp_tmp);
int update_fcb(int dir_fcb_id, int fcb_id, char *name, FILE* fp_tmp);
int drop_fcb(int dir_fcb_id, int fcb_id);
int write_ib(int ib_id, int block_size);
void update_sys();
/*内核函数定义*/
#endif // !F_CORE_H
