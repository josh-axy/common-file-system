#include "pch.h"
#include "Fcommand.h"

/*Show the contents for users*/
void ls(char* path, char* mode)
{
	//ls -l
	int tmp_fcb_id = path_to_fcb_id(path, DIR_T);
	if (mode[0] == '-' && mode[1] == 'l')
	{
		if (tmp_fcb_id == -1)
		{
			return;
		}
		printf("File name       File size       Create time          Edit time            File type\n\n");
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
			printf("%-15s\t%-15d\t", fcb_list[child_id].filename, fcb_list[child_id].file_size);
			print_time(fcb_list[child_id].create_time);
			printf("  ");
			print_time(fcb_list[child_id].edit_time);
			printf("  ");
			switch (fcb_list[child_id].file_type)
			{
			case 0:
				printf("<DIR>\n");
				break;
			case 1:
				printf("<Document>\n");
				break;
			}
			printf("\n");
			if (fcb_list[child_id].file_type == DIR_T)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				//cout << '/';
			}
		}
	}
	//ls
	else if (mode[0] == '\0')
	{
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
				cout << '/';
			}
			cout << endl;
		}
	}
}