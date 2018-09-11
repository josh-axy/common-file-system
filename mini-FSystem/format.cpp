#include "pch.h"
#include "Fcommand.h"
/*Format 1G space*/
int format() {
	superBlock block;
	if (strcmp(mini_path, "D:\\") == 0)
	{
		printf("Please create a mini_FS first!\n");
		return -1;
	}
	if ((fp = fopen(mini_path, "rb+")) == NULL)
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
	block.last_ib = -1;
	block.last_fcb = 0;
	block.freefcb_id = 1;
	block.rootfcb_id = 0;
	block.freeib_id = 0;
	block.last_write_time = current_time();
	fwrite(&block, sizeof(superBlock), 1, fp);

	//空文件头初始化
	IB ib;
	fseek(fp, IB_Location * BLOCK_SIZE, SEEK_SET);
	ib.block_num = 4033;
	ib.length = IB_NUM;
	ib.last_num = -1;		//-1表示此为空文件信息块头
	ib.next_num = -1;		//-1表示此为最后一段空文件信息段
	fwrite(&ib, sizeof(IB), 1, fp);

	//控制块初始化
	FCB fcb;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(fcb.filename, "");			//根目录文件名
	fcb.create_time = current_time();	//创建时间
	fcb.edit_time = fcb.create_time;	//编辑时间
	fcb.file_type = EMPTY_T;			//空白
	fcb.file_size = 0;					//大小为0
	fcb.is_delete = 1;					//已删除
	fcb.file_block_id = -1;				//文件夹
	for (int i = 0; i < FCB_WIDTH; i++)
		fcb.filep[i] = -1;				//暂时不存在子目录
	for (int i = 0; i < FCB_NUM; i++)
	{
		fcb.filep[0] = i + 1;
		fwrite(&fcb, sizeof(FCB), 1, fp);
	}

	//根目录初始化
	FCB root;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	strcpy(root.filename, "\\");			//根目录文件名
	root.create_time = current_time();		//创建时间
	root.edit_time = root.create_time;		//编辑时间
	root.file_type = DIR_T;					//文件夹
	root.file_size = 0;						//空文件夹，大小为0
	root.is_delete = 0;						//未删除
	root.file_block_id = -1;				//文件夹
	for (int i = 0; i < FCB_WIDTH; i++)
		root.filep[i] = -1;					//暂时不存在子目录
	root.filep[1] = 0;
	fwrite(&root, sizeof(FCB), 1, fp);
	printf("\nFormat success.\n");
	if ((fp = fopen(mini_path, "rb+")) == NULL)
	{
		printf("Mount failed.\n");
		return -1;
	}
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
	fseek(fp, IB_Location * BLOCK_SIZE + sys.freeib_id * BLOCK_SIZE, 0);
	fread(&ib_tmp, sizeof(IB), 1, fp);
	current_fcb_id = sys.rootfcb_id;
	/*
	FCB rootd;
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	fread(&rootd, sizeof(FCB), 1, fp);
	fseek(fp, FCB_Location * BLOCK_SIZE, SEEK_SET);
	for (int i = 0; i < FCB_NUM; i++)
	{
		fread(&fcb_list[i], sizeof(FCB), 1, fp);
	}
	*/
	fclose(fp);
	return 0;
}