#include "pch.h"
#include "Fcommand.h"

/*Show the contents for users*/
void ls(char* path, char* mode)
{
	int tmp_fcb_id = path_to_fcb_id(path, DIR_T);
	if (tmp_fcb_id == -1)
	{
		return;
	}
	for (int i = 2; fcb_list[tmp_fcb_id].filep[i] != -1; i++)
	{
		int child_id;
		if (i == EXT_CB)
		{
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[i];
			i = 1;
		}
		child_id = fcb_list[tmp_fcb_id].filep[i];
		if (fcb_list[child_id].file_type == DIR_T)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		cout << fcb_list[child_id].filename;
		if (fcb_list[child_id].file_type == DIR_T)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		if (fcb_list[child_id].file_type == DIR_T)
		{
			cout << '/';
		}
		cout << endl;
	}
}