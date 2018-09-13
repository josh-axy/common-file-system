#pragma once
/*指令函数定义*/
#include "Fbase.h"
#include "Fcore.h"
int format();
int mount(char arr[]);
int input(char* op1, char* op2, char* op3);
void attr(char *);
void cp(char path1[], char path2[]);
int cp_s2c(char path1[], char path2[]);
int cp_c2s(char path1[], char path2[]);
int cp_c2c(char path1[], char path2[]);
void del(char* path, char* mode);
void cd(char* path);
void ls(char* path, char* mode);
void create(char *name);
void judge(char* op1, char* op2, char* op3);
void move(char *src, char *dst);
int mkdir(char* path, int mk_type);
void help(char * order);
void readtxt(char *);
void more(char *);
void tree(char *path);
void exit();
void map();
using namespace std;
/*指令函数定义*/