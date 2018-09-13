#include "pch.h"
#include "Fcommand.h"

void attr(char *path)
{
	int tmp_fcb_id = path_to_fcb_id(path, EMPTY_T);
	if (tmp_fcb_id == -1)
		return;
	printf("File name       File size       Create time          Edit time            File type\n%-15s\t%-15d\t", fcb_list[tmp_fcb_id].filename, fcb_list[tmp_fcb_id].file_size);
	print_time(fcb_list[tmp_fcb_id].create_time);
	printf("  ");
	print_time(fcb_list[tmp_fcb_id].edit_time);
	printf("  ");
	switch (fcb_list[tmp_fcb_id].file_type)
	{
	case 0:
		printf("<DIR>\n");
		break;
	case 1:
		printf("<Document>\n");	
		break;
	}
}
