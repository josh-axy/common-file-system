#include "pch.h"
#include "Fcommand.h"

/***********************************************************************************/
/************************  复制函数主函数 ******************************************/
/***********************************************************************************/
void cp(char path1[], char path2[])
{
	int dir_fcb_id;
	FILE* fp_tmp;
	IB_Disk* ib;
	char *buf;
	int f_len;
	if (path2[2] != '\\')
	{
		make(path2, FILE_T, path1);
	}
	else
	{
		dir_fcb_id = path_to_fcb_id(path1, FILE_T);
		switch (dir_fcb_id)
		{
		case -2:
		case -1:
			return;
		default:
			break;
		}
		ib = get_ib_info(fcb_list[dir_fcb_id].file_block_id);
		//以读/写方式打开或建立一个文本文件，允许读和写。（为NULL则打不开）
		if ((fp_tmp = fopen(path2, "wb+")) == NULL)
		{
			printf("Name illegal!\n");
			return;
		}
		buf = new char[ib->size];
		fseek(fp, IB_POS(ib->block_id), SEEK_SET);
		fread(buf, ib->size, 1, fp);
		fwrite(buf, sizeof(buf), 1, fp_tmp);
		fclose(fp_tmp);
		delete ib;
	}
	return;
}
