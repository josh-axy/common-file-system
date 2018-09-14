#include "pch.h"
#include "Fcommand.h"

void exit()
{
	current_fcb_id = 0;
	sys_mounted = 0;
	fclose(fp);
	memset(&sys, 0, sizeof(sys));
	memset(fcb_list, 0, sizeof(fcb_list));
	memset(&free_ib_tmp, 0, sizeof(free_ib_tmp));
	strcpy(fs_path,"D:\\");
	strcpy(current_path, "");
	free_ib_tree.Clear();
	id_tree.Clear();
	cout << "Bye." << endl;
}