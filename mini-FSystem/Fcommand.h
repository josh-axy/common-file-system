#pragma once
/*指令函数定义*/
#include "Fbase.h"
#include "Ftools.h"
#include "Fcore.h"
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
void move(char *src, char *dst);
int mkdir(char* path, int mk_type);
void help(char * order);
void readtxt(char *);
void more(char *);
using namespace std;
/*指令函数定义*/