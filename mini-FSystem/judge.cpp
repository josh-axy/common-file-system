#include "pch.h"
#include "Fcommand.h"

/*Judge users' order*/
void judge(char *op1, char *op2, char *op3)
{
	
	//Create Command
	if (strcmp(op1, "create") == 0)
	{
		if (sys_mounted)
		{
			printf("Please exit first!\n");
		}
		else if (op2[0] == '\0')
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
		if (sys_mounted)
		{
			printf("Please exit first!\n");
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			mount(op2);
	}

	//Copy Command
	else if (strcmp(op1, "cp") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0' || op3[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			cp(op2, op3);
	}

	//Format Command
	else if (strcmp(op1, "fmt") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (strlen(op2) != 0)
			printf("Command syntax is incorrect!\n");
		else
			format();
	}

	//Show Contents Command
	else if (strcmp(op1, "ls") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else
			ls(op2, op3);
	}

	else if (strcmp(op1, "show") == 0)
	{
		int cnt;
		if (op2)
		{
			cnt = (int)(op2 - '0');
		}
		else
		{
			cnt = 10;
		}
		cout << "first:" << sys.freeib_id << " last:" << sys.last_freeib_id << endl;
		for (int i = 1; i <= cnt;)
		{
			//装载空闲信息块头
			fseek(fp, IB_POS(i), SEEK_SET);
			fread(&free_ib_tmp, sizeof(IB_Disk), 1, fp);
			cout << free_ib_tmp.block_id << ' ' << free_ib_tmp.size << ' ' << free_ib_tmp.last_id << ' ' << free_ib_tmp.next_id << endl;
			i++;
		}
	}

	//Enter Contents Command
	else if (strcmp(op1, "cd") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			cd(op2);
	}

	//Delete Command
	else if (strcmp(op1, "del") == 0 || strcmp(op1, "rm") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			del(op2, op3);  //1
	}

	//Show Text Command
	else if (strcmp(op1, "tp") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			readtxt(op2);   //1
	}

	//Paging Show Text Command
	else if (strcmp(op1, "more") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			more(op2);  //1
	}

	//Show File Attribute Command
	else if (strcmp(op1, "attr") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			attr(op2);   //1
	}


	//Create Directory Command
	else if (strcmp(op1, "mkdir") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			make(op2, DIR_T, NULL);   //1
	}

	//Create File Command
	else if (strcmp(op1, "mkf") == 0 || strcmp(op1, "mkfile") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else if (strlen(op3) != 0)
			printf("Command syntax is incorrect!\n");
		else
			make(op2, FILE_T, NULL);   //1
	}

	//Move Command
	else if (strcmp(op1, "mv") == 0 || strcmp(op1, "move") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] == '\0')
			printf("Command syntax is incorrect!\n");
		else
			move(op2, op3);   //1
	}
	//Map Comman
	else if (strcmp(op1, "map") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] != '\0')
			printf("Command syntax is incorrect!\n");
		else
			map();   //1
	}
	
	//Map Comman
	else if (strcmp(op1, "tree") == 0)
	{
	if (!sys_mounted)
	{
		cout << "No FS mounted!" << endl;
		return;
	}
	else if (op3[0] != '\0')
		printf("Command syntax is incorrect!\n");
	else
		tree(op2);   //1
	}
	//Close Command
	else if (strcmp(op1, "exit") == 0)
	{
		if (!sys_mounted)
		{
			cout << "No FS mounted!" << endl;
			return;
		}
		else if (op2[0] != '\0')
			printf("Command syntax is incorrect!\n");
		else if (op3[0] != '\0')
			printf("Command syntax is incorrect!\n");
		else
			exit();   //1
	}

	else
	{
		printf("The input command is incorrect. Please re-enter.\n");
	}
}