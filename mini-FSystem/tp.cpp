#include "pch.h"
#include "Fcommand.h"

void readtxt(char* path)
{
	FILE* fp_tmp;
	int fcb_id = path_to_fcb_id(path, FILE_T);
	switch (fcb_id)
	{
	case -2:
	case -1:
		return;
	default:
		break;
	}
	char* buffer = new char[fcb_list[fcb_id].file_size];
	if (fp == NULL)
	{
		return;
	}
	fseek(fp, IB_POS(fcb_list[fcb_id].file_block_id) + sizeof(IB_Disk), SEEK_SET);
	fread(buffer, fcb_list[fcb_id].file_size, 1, fp);
	//fclose(fp_tmp);
	cout << buffer << endl;
	delete buffer;
	return;
}