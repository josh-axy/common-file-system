#include "pch.h"
#include "Fcommand.h"

void cd(char* path)
{
	int tmp_fcb_id = path_to_fcb_id(path);
	switch (tmp_fcb_id)
	{
	case -2:
	case -1:
		break;
	default:
		current_fcb_id = tmp_fcb_id;
		break;
	}
	return;
}