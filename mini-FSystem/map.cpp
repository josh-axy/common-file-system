#include "pch.h"
#include "Fcommand.h"

void map()
{
	IB_Disk* ib_tmp;
	while (true)
	{
		ib_tmp = get_ib_info(sys.freeib_id);
		cout << ib_tmp->block_id << ',' << ib_tmp->size << '\t';
		if (ib_tmp->next_free_ib == -1)
		{
			break;
		}
	}
	cout << endl;
	return;
}