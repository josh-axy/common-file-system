#include "pch.h"
#include "Fcommand.h"

/*Show help to users*/
void help(char * order)
{
	char ch;
	FILE* fp_tmp;
	if ((fp_tmp = fopen("D:\\help.txt", "rb+")) == NULL)
	{
		printf("Open help file error\n");
		getchar();
		exit(1);
	}
	if (order[0] == '\0')
	{
		ch = fgetc(fp_tmp);
		while (ch != EOF && ch != '1')
		{
			if (ch >= 'A'&&ch <= 'Z')
				ch = ch + 'a' - 'A';
			putchar(ch);
			ch = fgetc(fp_tmp);
		}
	}
	else
	{
		char info[300];
		while (1)
		{
			fscanf(fp_tmp, "%s", info);
			if ((strcmp("end", info)) == 0)
				break;
			if ((strcmp(order, info)) == 0)
			{
				while (1)
				{
					fgets(info, 300, fp_tmp);
					if (info[0] == '1')
						break;
					printf("%s", info);
				}
				return;
			}
		}
		printf("The input command is incorrect. Please re-enter!\n");
	}
	fclose(fp_tmp);
}