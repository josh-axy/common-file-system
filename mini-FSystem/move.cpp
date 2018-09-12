#include "pch.h"
#include "Fcommand.h"

void move(char *src, char *dst)
{
	char name[13] = { 0 };
	int src_fcb_id, dst_fcb_id;
	int i;
	src_fcb_id = path_to_fcb_id(src);
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
		if (dst[i] == '\\')
		{
			if (i != 0)
				dst[i] = '\0';
			break;
		}
	}
	if (i < 0) {
		strcpy(name, dst);
		dst[0] = '\0';
		dst_fcb_id = current_fcb_id;
	}
	else
	{
		strcpy(name, dst + i + 1);
		dst_fcb_id = path_to_fcb_id(dst);
	}
	switch (dst_fcb_id)
	{
	case -2:
	case -1:
		return;
	default:
		update_fcb(dst_fcb_id, src_fcb_id, name, NULL);
		break;
	}
	return;
}
