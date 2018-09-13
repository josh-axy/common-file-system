#include "pch.h"
#include "Fcommand.h"

void tree_child(int fcb,int num);
void printtree1(int num);
void printtree2(int num);
void printtree3(int num);
typedef struct FLAG {
	int flag;
	int kkkk;
}FLAG;
FLAG flag[FCB_NUM];
int iflag = 0;
int num = -1;
void tree(char* path)
{
	flag[iflag].flag = 0;
	int tmp_fcb_id = path_to_fcb_id(path, DIR_T);
	if (tmp_fcb_id == -1)
		return;
	tree_child(tmp_fcb_id,num);
}

void tree_child(int fcb,int num)
{
	num++;
	if (fcb_list[fcb].file_type == DIR_T)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	if(num)
		cout << fcb_list[fcb].filename;
	if (fcb_list[fcb].file_type == DIR_T)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << '/';
	}
	cout << endl;
	iflag++;
	flag[iflag].flag = /*strlen(fcb_list[fcb].filename)*/3;
	if (fcb_list[fcb].filep[2] == -1)
		return;
	else if(fcb_list[fcb].filep[3] == -1)
	{
		printtree1(num);
		flag[num].kkkk = 1;
		tree_child(fcb_list[fcb].filep[2], num);
	}
	else
	{
		int q;
		for (q = 2; fcb_list[fcb].filep[q + 1] != -1; q++)
		{
			//flag += 1;
			//tree_child(fcb_list[fcb].filep[i],flag);
			int child_id;
			if (q == EXT_CB)
			{
				fcb = fcb_list[fcb].filep[q];
				q = 1;
			}
			child_id = fcb_list[fcb].filep[q];
			
			printtree3(num);
			flag[num].kkkk = 0;
			tree_child(fcb_list[fcb].filep[q], num);
		}
		printtree1(num);
		flag[num].kkkk = 1;
		tree_child(fcb_list[fcb].filep[q], num);
	}
	return;
}

void printtree1(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < flag[i].flag; j++)
			printf(" ");
		if(flag[i].kkkk == 1)
			printf(" ");
		else
			printf("©¦");
	}
	for (int j = 0; j < flag[num].flag; j++)
		printf(" ");
	printf("©¸©¤ ");
}

void printtree3(int num)
{
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < flag[i].flag; j++)
			printf(" ");
		if (flag[i].kkkk == 1)
			printf(" ");
		else
			printf("©¦");
	}
	for (int j = 0; j < flag[num].flag; j++)
		printf(" ");
	printf("©À©¤ ");
}