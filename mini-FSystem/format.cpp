#include "pch.h"
#include "Fcommand.h"
/*Format 1G space*/
int format() {
	superBlock block;
	if (strcmp(fs_path, "D:\\") == 0)
	{
		printf("Please create a mini_FS first!\n");
		return -1;
	}
	if ((fp = fopen(fs_path, "rb+")) == NULL)
	{
		printf("Format failed.\n");
		return -1;
	}
	rewind(fp);
	fseek(fp, SUPER_Loacation, SEEK_SET);
	//系统块初始化
	block.fcb_Location = FCB_Location;
	block.ib_location = IB_Location;
	block.fcb_size = FCB_SIZE;
	block.blk_size = BLOCK_SIZE;
	block.superblock_size = sizeof(superBlock);
	block.superblock_location = 0;
	block.last_write_ib = -1;
	block.last_write_fcb = 0;
	block.freefcb_id = 1;
	block.rootfcb_id = 0;
	block.freeib_id = 0;
	block.last_freefcb_id = FCB_NUM - 1;
	block.last_freeib_id = 0;
	block.last_write_time = current_time();
	fwrite(&block, sizeof(superBlock), 1, fp);
	fflush(fp);

	//空文件头初始化
	IB_Disk ib;
	fseek(fp, IB_Location * BLOCK_SIZE, SEEK_SET);
	ib.block_id = 4033;
	ib.size = IB_NUM;
	ib.last_free_ib = -1;		//-1表示此为空文件信息块头
	ib.next_free_ib = -1;		//-1表示此为最后一段空文件信息段
	fwrite(&ib, sizeof(IB_Disk), 1, fp);
	fflush(fp);

	//控制块初始化
	FCB fcb;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(fcb.filename, "");			//根目录文件名
	fcb.create_time = current_time();	//创建时间
	fcb.edit_time = fcb.create_time;	//编辑时间
	fcb.file_type = EMPTY_T;			//空白
	fcb.file_size = 0;					//大小为0
	fcb.file_block_id = -1;				//文件夹
	for (int i = 0; i < FCB_WIDTH; i++)
		fcb.filep[i] = -1;				//暂时不存在子目录
	for (int i = 0; i < FCB_NUM - 1; i++)
	{
		fcb.filep[0] = i + 1;
		fwrite(&fcb, sizeof(FCB), 1, fp);
	}
	//初始化末位空闲FCB
	fcb.filep[0] = -1;
	fwrite(&fcb, sizeof(FCB), 1, fp);
	fflush(fp);
	//根目录初始化
	FCB root;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(root.filename, "/");			//根目录文件名
	root.create_time = current_time();		//创建时间
	root.edit_time = root.create_time;		//编辑时间
	root.file_type = DIR_T;					//文件夹
	root.file_size = 0;						//空文件夹，大小为0
	root.file_block_id = -1;				//文件夹
	for (int i = 0; i < FCB_WIDTH; i++)
		root.filep[i] = -1;					//暂时不存在子目录
	root.filep[1] = 0;
	fwrite(&root, sizeof(FCB), 1, fp);
	fflush(fp);
	printf("\nFormat success.\n");
	if (fp == NULL)
	{
		printf("Mount failed.\n");
		return -1;
	}
	if (sys_mounted == 1)
	{
		//装载系统块
		fseek(fp, SUPER_Loacation * BLOCK_SIZE, SEEK_SET);
		fread(&sys, sizeof(superBlock), 1, fp);
		//装载控制单元
		fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
		for (int i = 0; i < FCB_NUM; i++)
		{
			fread(&fcb_list[i], sizeof(FCB), 1, fp);
		}
		//装载空闲信息块头
		fseek(fp, IB_POS(sys.freeib_id), 0);
		fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
		free_ib_tree.Insert(free_ib_tmp);
		//建立空闲块平衡二叉树
		for (int i = 0; free_ib_tmp.next_free_ib != -1; i++)
		{
			fseek(fp, IB_POS(free_ib_tmp.next_free_ib), 0);
			fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
			free_ib_tree.Insert(free_ib_tmp);
		}
		current_fcb_id = sys.rootfcb_id;
		strcpy(current_path, "/");
	}
	fflush(fp);
	return 0;
}