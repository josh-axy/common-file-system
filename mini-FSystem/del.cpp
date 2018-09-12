#include "pch.h"
#include "Fcommand.h"

void del(char* path, char* mode)
{
	char name[13] = { 0 };
	int drop_fcb_id;
	int i;
	int r_mode = 0;
	if (path[0] == '\0')
	{
		cout << "Please input a valid destination!" << endl;
		return;
	}
	drop_fcb_id = path_to_fcb_id(path, EMPTY_T);
	switch (drop_fcb_id)
	{
	case -2:
	case -1:
		return;
	default:
		if (strcmp(mode, "-r") == 0)
		{
			r_mode = 1;
		}
		drop_fcb(drop_fcb_id, r_mode);
		break;
	}
}