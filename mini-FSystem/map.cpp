#include "pch.h"
#include "Fcommand.h"

void map()
{

	/*IB_Disk* ib_tmp;
	IB_Disk* ib_map;
	int flag = 1;
	ib_tmp = get_ib_info(sys.freeib_id);
	ib_map = get_ib_info(ib_tmp->next_id);
	if (ib_tmp->block_id == 1 && ib_tmp->size == 258111)
	{
		printf("No Files In IB\n");
		return;
	}
	if (ib_tmp->block_id != 1)
	{
		printf("File number %d: begin from block 1\n",flag);
		printf("File number %d: block length is  %d\n\n", flag, ib_tmp->block_id - 1);
		flag++;
	}
	while (true)
	{
		if (ib_map->next_id == 0 || ib_tmp->next_id == 0)
		{
			break;
		}
		int lastfull_head,lastfull_length;
		lastfull_head = ib_map->block_id - ib_tmp->block_id;
		lastfull_length = ib_map->block_id - lastfull_head; 
		printf("File number %d: begin from block %d\n", flag,lastfull_head);
		printf("File number %d: block length is  %d\n\n", flag, lastfull_length);
		flag++;
		ib_tmp = ib_map;
		ib_map = get_ib_info(ib_tmp->next_id);
	}
	if (ib_map->block_id + ib_map->size - 1 != 258111 && ib_tmp->block_id + ib_tmp->size - 1 != 258111)
	{
		printf("File number %d: begin from block %d\n", flag, ib_map->block_id + ib_map->size);
		printf("File number %d: block length is  %d\n\n", flag, 258111 - ib_map->block_id + ib_map->size + 1);
	}
	cout << "Search Finished!" << endl;
	return;
	*/
}