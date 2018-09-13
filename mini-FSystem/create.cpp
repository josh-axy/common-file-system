#include "pch.h"
#include "Fcommand.h"
/*Creat 1G space*/
void create(char *name)
{
	int i;
	for (i = 0; name[i] != '\0'; i++)
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= '0' && name[i] <= '9')))
		{
			printf("System name input is not legal!\n");
			return;
		}
	}
	//文件系统所在路径
	fs_path[3] = '\0';
	strcat(fs_path, name);

	/*.DAT并不是一种标准文件。许多软件都使用这个扩展名，但文件含义不同。
		DAT文件，可以按照扩展名来看就是DATA的意思，即数据文件，这类文件并没有进行绝对化的定义。*/
	strcat(fs_path, ".dat");

	//以读/写方式打开或建立一个二进制文件，允许读和写。（为NULL则打不开）
	if ((fp = fopen(fs_path, "wb+")) == NULL)
	{
		printf("System name input is not legal!\n");
		return;
	}
	//开辟1G空间
	fseek(fp, 1024 * 1024 * 1024, 0);
	putw(0, fp);
	fclose(fp);
	//判断文件是否被创建
	if ((fp = fopen(fs_path, "r+")) != NULL)
		printf("\nCreate file success.\n");
	fclose(fp);
	format();
}