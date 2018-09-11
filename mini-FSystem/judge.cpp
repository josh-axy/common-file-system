#include "pch.h"

/*Judge users' order*/
void judge(char *op1, char *op2, char *op3)
{
	//Create Command
	if (strcmp(op1, "create") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			create(op2);
	}

	//Show Help Command
	else if (strcmp(op1, "help") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		help(op2);  //0
	}

	//Mount Command
	else if (strcmp(op1, "mount") == 0)
	{

		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			mount(op2);
	}

	else if (!sys_mounted)
	{
		cout << "No FS mounted!" << endl;
		return;
	}

	//Copy Command
	else if (strcmp(op1, "cp") == 0)
	{
		if (op2[0] == '\0' || op3[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			cp(op2, op3);
	}

	//Format Command
	else if (strcmp(op1, "fmt") == 0)
	{

		if (strlen(op2) != 0)
			printf("Command syntax is incorrect!\n");
		else
			format();
	}

	//Show Contents Command
	else if (strcmp(op1, "ls") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			ls(op2);
	}

	//Enter Contents Command
	else if (strcmp(op1, "cd") == 0)
	{
		if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			cd(op2);
	}

	//Delete Command
	else if (strcmp(op1, "dl") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			del(op2);  //1
	}

	//Show Text Command
	else if (strcmp(op1, "tp") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			readtxt(op2);   //1
	}

	//Paging Show Text Command
	else if (strcmp(op1, "more") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			more(op2);  //1
	}

	//Show File Attribute Command
	else if (strcmp(op1, "attr") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			attr(op2);   //1
	}


	//Create Directory Command
	else if (strcmp(op1, "mkdir") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			mkdir(op2, DIR_T);   //1
	}

	//Create File Command
	else if (strcmp(op1, "mkfile") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			mkdir(op2, FILE_T);   //1
	}

	//Move Command
	else if (strcmp(op1, "mv") == 0)
	{
		if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			move(op2, op3);   //1
	}

	else
	{
		printf("The input command is incorrect. Please re-enter.\n");
	}
}