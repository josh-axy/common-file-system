#include "pch.h"
#include "Fcommand.h"

void exit()
{
	current_fcb_id = 0;
	sys_mounted = 0;
	fclose(fp);
	memset(&sys, 0, sizeof(sys));
	memset(fcb_list, 0, sizeof(fcb_list));
	memset(ib_tmp, 0, sizeof(ib_tmp));
	strcmp(mini_path,"D:\\");
	strcpy(current_path, "");
	cout << "Bye." << endl;
}