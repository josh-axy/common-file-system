#include "pch.h"
#include "Fcommand.h"

void tree_child(int fcb,int num);
void printtree1(int num);
void printtree3(int num);
int kkkk[FCB_NUM];
int num = -1;
void tree(char* path)
{
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
	if(fcb_list[fcb].filep[0] != -1)
		cout << fcb_list[fcb].filename;
	if (fcb_list[fcb].file_type == DIR_T)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << '/';
	}
	cout << endl;
	if (fcb_list[fcb].filep[2] == -1)
		return;
	else if(fcb_list[fcb].filep[3] == -1)
	{
		printtree1(num);
		kkkk[num] = 1;
		tree_child(fcb_list[fcb].filep[2], num);
	}
	else
	{
		int q;
		for (q = 2; fcb_list[fcb].filep[q + 1] != -1; q++)
		{
			if (q == EXT_CB)
			{
				fcb = fcb_list[fcb].filep[q];
				q = 1;
			}
			printtree3(num);
			kkkk[num] = 0;
			tree_child(fcb_list[fcb].filep[q], num);
		}
		printtree1(num);
		kkkk[num] = 1;
		tree_child(fcb_list[fcb].filep[q], num);
	}
	return;
}

void printtree1(int num)
{
	for (int i = 0; i < num; i++)
	{
		if(i != 0)
			for (int j = 0; j < 2; j++)
				printf(" ");
		if(kkkk[i] == 1)
			printf(" ");
		else
			printf("©¦");
	}
	if(num)
		for (int j = 0; j < 2; j++)
			printf(" ");
	printf("©¸©¤ ");
}

void printtree3(int num)
{
	for (int i = 0; i < num; i++)
	{
		if (i != 0)
			for (int j = 0; j < 2; j++)
				printf(" ");
		if (kkkk[i] == 1)
			printf(" ");
		else
			printf("©¦");
	}
	if (num)
		for (int j = 0; j < 2; j++)
			printf(" ");
	printf("©À©¤ ");
}