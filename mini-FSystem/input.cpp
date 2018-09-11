#include "pch.h"
#include "Fcommand.h"

/*Input orders!!!*/
int input(char* op1, char* op2, char* op3)
{
	char op[OP_LEN + 1];
	int flag[100];
	int i = 0, j = 0;
	int flag1 = 0;
	memset(flag, 0, sizeof(flag));
	for (i = 0;; i++)
	{
		if (i == OP_LEN)
		{
			if (flag1 == 0)
			{
				printf("The input command character is too long. Please enter the command within %d.\n.", i);
				flag1 = 1;
			}
			i = 0;
		}
		op[i] = getchar();
		if (op[i] == '\n')
		{
			if (flag1 == 1)
				return -1;
			op[i] = '\0';
			break;
		}
	}
	if (op[0] != ' ')
		flag[j++] = 0;
	for (i = 0; op[i] != '\0'; i++)
	{
		if ((op[i] == ' '&&op[i + 1] != ' ') || (op[i] != ' ' && (op[i + 1] == ' ' || op[i + 1] == '\0')))
			flag[j++] = i + 1;
	}
	for (i = flag[0]; i < flag[1]; i++)
		op1[i - flag[0]] = op[i];
	op1[i - flag[0]] = '\0';
	for (i = flag[2]; i < flag[3]; i++)
		op2[i - flag[2]] = op[i];
	op2[i - flag[2]] = '\0';
	for (i = flag[4]; i < flag[5]; i++)
		op3[i - flag[4]] = op[i];
	op3[i - flag[4]] = '\0';
	int len;
	len = strlen(op);
	if ((flag[5] != 0) && flag[5] < len)
	{
		printf("Command syntax is incorrect!\n");
		return -1;
	}
	return 0;
}