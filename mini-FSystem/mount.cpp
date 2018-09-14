#include "pch.h"
#include "Fcommand.h"
/*Mount used Mini_FS*/
int mount(char arr[])
{
	fs_path[3] = '\0';
	strcat(fs_path, arr);
	strcat(fs_path, ".dat");
	if ((fp = fopen(fs_path, "rb+")) == NULL)
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
	fseek(fp, IB_POS(sys.freeib_id), SEEK_SET);
	fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
	free_ib_tree.Insert(&free_ib_tmp);
	//建立空闲块平衡二叉树
	for (int i = 0; free_ib_tmp.next_id != 0; i++)
	{
		fseek(fp, IB_POS(free_ib_tmp.next_id), SEEK_SET);
		fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
		free_ib_tree.Insert(&free_ib_tmp);
	}
	current_fcb_id = sys.rootfcb_id;
	strcpy(current_path, "/");
	printf("Mount success.\n");
	cout << "\nFree Ib Map:\n" << endl;
	free_ib_tree.PrintRDL();
	cout << endl;
	sys_mounted = 1;
	fflush(fp);
	return 0;
}