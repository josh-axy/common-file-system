#include "pch.h"
#include "Fcommand.h"

void cd(char* path)
{
	int tmp_fcb_id = path_to_fcb_id(path, DIR_T);
	int depth;
	char path_list[100][13];

	if (path[0] == '\0')
	{
		tmp_fcb_id = sys.rootfcb_id;
	}
	switch (tmp_fcb_id)
	{
	case -2:
	case -1:
		break;
	default:
		current_fcb_id = tmp_fcb_id;
		for (depth = 0; fcb_list[tmp_fcb_id].filep[0] != -1; depth++)
		{
			strcpy(path_list[depth], fcb_list[tmp_fcb_id].filename);
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[0];
			if (fcb_list[tmp_fcb_id].file_type == EXT_T)
			{
				tmp_fcb_id = fcb_list[tmp_fcb_id].filep[0];
			}
		}
		strcpy(current_path, "/");
		for (int i = depth-1; i >= 0; i--)
		{
			strcat(current_path, path_list[i]);
			if (i)
			{
				strcat(current_path, "/");
			}
		}
		break;
	}
	return;
}