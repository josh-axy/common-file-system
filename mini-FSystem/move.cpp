#include "pch.h"
#include "Fcommand.h"

void move(char *src, char *dst)
{
	char name[13] = { 0 };
	int src_fcb_id, dst_fcb_id;
	int i;
	if (dst[0] == '\0')
	{
		cout << "Please input a valid destination!" << endl;
		return;
	}
	src_fcb_id = path_to_fcb_id(src, EMPTY_T);
	switch (src_fcb_id)
	{
	case -2:
	case -1:
		return;
	default:
		break;
	}
	for (i = strlen(dst) - 1; i >= 0; i--)
	{
		if (dst[i] == '/')
		{
			if (i != 0)
				dst[i] = '\0';
			if (dst[i + 1] == '\0')
			{
				strcpy(name, fcb_list[src_fcb_id].filename);
			}
			break;
		}
	}
	if (i < 0) 
	{
		strcpy(name, dst);
		dst[0] = '\0';
		dst_fcb_id = current_fcb_id;
	}
	else
	{
		if (name[0] == '\0')
			strcpy(name, dst + i + 1);
		dst_fcb_id = path_to_fcb_id(dst, DIR_T);
	}
	switch (dst_fcb_id)
	{
	case -2:
	case -1:
		return;
	default:
		for (int tmp_fcb_id = dst_fcb_id; tmp_fcb_id!=-1;)
		{
			tmp_fcb_id = fcb_list[tmp_fcb_id].filep[0];
			if (tmp_fcb_id == src_fcb_id)
			{
				cout << "Cannot do that operate!" << endl;
				return;
			}
		}
		move_fcb(dst_fcb_id, src_fcb_id, name, NULL);
		break;
	}
	return;
}
