#pragma once
#ifndef F_BASE_H
#define F_BASE_H
#include "pch.h"
#include <iostream>
#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stack>

using namespace std;

/*超级块结构*/
typedef struct Super_block {
	int fcb_Location;			//第一个文件控制块的位置
	int ib_location;			//第一个数据块的位置
	int fcb_size;				//fcb大小
	int blk_size;				//块大小
	int superblock_size;		//超级块大小
	int superblock_location;	//超级块位置
	int last_write_ib;			//最后写入的信息块
	int last_write_fcb;			//最后写入的文件控制块
	int freefcb_id;				//下一个空闲控制块位置 (0,258047)
	int freeib_id;				//下一个空闲信息块位置（4033，262144）
	int last_freefcb_id;		//最后一个空闲控制块位置
	int last_freeib_id;			//最后一个空闲信息块位置
	int rootfcb_id;				//根目录位置
	int last_write_time;		//最后一次写入时间
}superBlock;

/*控制单元结构*/
typedef struct F_CB {
	int create_time;		//文件创建时间
	int edit_time;			//文件更新时间
	int file_size;			//文件占用字节数
	int file_block_id;		//文件头块编号
	int filep[FCB_WIDTH];	//子目录编号数组
	char filename[14];		//文件名字
	char file_type;			//文件类型
}FCB;

/*扩展控制单元结构*/
typedef struct ext_CB {
	int filep[16];			//子目录编号
}extCB;

/*文件单元结构*/
typedef struct I_B {
	int block_id;			//自己的块号
	int size;				//持续长度
	int last_id;			//前面块头的块号
	int next_id;			//后面块头的块号
}IB_Disk;

class IB_AVLNode {
	friend class IB_AVLTree;
private:
	int IB_id_;
	int size_;
	int last_id_;
	int next_id_;
	IB_AVLNode* p_lchild_;
	IB_AVLNode* p_rchild_;
	IB_AVLNode* p_parent_;
	int height_;
public:
	IB_AVLNode();  //默认构造函数，不用;
	IB_AVLNode(const int id);	//以ID为排序值
	IB_AVLNode(const int id, const int size, const int last_id, const int next_id);  //以size为排序值，对所有成员变量赋初值;
	IB_AVLNode(const IB_AVLNode & ib);
	IB_AVLNode(const IB_Disk & ib);
	void SetValue(const int id, const int size);
	void SetValue(const IB_Disk & ib);
	void Print();
	int GetSize() const;
	int GetId() const;
	int Last() const;
	int Next() const;
	IB_AVLNode* lchild();
	IB_AVLNode* rchild();
	IB_AVLNode* parent();
};


class IB_AVLTree {
private:
	IB_AVLNode *root_;

public:
	IB_AVLTree();    //给root赋初值;

	~IB_AVLTree();   //调用Clear()清理树，释放内存;

	void Insert(const int id);		//插入ID
	void Insert(const int id, const int size, const int last_id, const int next_id);      //插入一个结点;
	void Insert(IB_Disk* ib);							//插入一个free ib;
	void Insert(IB_AVLNode* ib);

	void Delete(const int id);		//删除ID
	void Delete(const int id, const int size);      //删除IB_id为id的结点，若没有则不删除;
	void Delete(IB_Disk* ib);
	void Delete(IB_AVLNode* ib);

	IB_AVLNode* GetRoot();
	IB_AVLNode* Search(const int id, const int size);  //查找值为size的结点;
	IB_AVLNode* Search(IB_AVLNode* ib);
	IB_Disk* Search(IB_Disk* ib);

	void PrintDLR();     //先根遍历打印整棵树;

	void PrintLDR();     //中根遍历打印树; 此函数打印出有序的数据，可供测试;

	void PrintRDL();     //后根遍历打印树

	void Clear();        //对树清空，删除所有结点;

private:
	//根据情况判断并调用四种具体的旋转算法其中一种;
	void Rotate(IB_AVLNode *p_node);

	//向左旋转一次,用于树失去平衡后调整;
	//更新子树的height值;
	void LL(IB_AVLNode *p_node);

	//向右旋转一次,用于树失去平衡后调整;
	//更新子树的height值;
	void RR(IB_AVLNode *p_node);

	//插入结点引起的旋转调整;
	//p_parent是新插入结点的父结点,此结点height值未更新;
	void InsertAdjust(IB_AVLNode* p_parent_new_node);

	//删除结点引起的调整;
	//p_parent_deleted_node是被删除结点的父结点;此结点height值未更新;
	void DeleteAdjust(IB_AVLNode* p_parent_deleted_node);

	//计算并返回p_node所指向结点的balance值;
	int GetBalance(IB_AVLNode* p_node);

	//更新p_node指向结点的height值;
	void UpdateHeight(IB_AVLNode* p_node);

	//删除结点时找到被删除结点的中序前驱或后继，用于替代被删除结点;
	IB_AVLNode* FindBestReplacement(IB_AVLNode* p_node);

	//找到最小不平衡子树并返回其根结点,p_node的height值必须为未更新的;
	IB_AVLNode* FindMinUnbalanceSubtree(IB_AVLNode *p_node);

	//中序打印结点值;
	void PrintLDR(IB_AVLNode* p_node);

	//后序打印结点值;
	void PrintRDL(IB_AVLNode* p_node);     //后根遍历打印树
};

/*全局变量*/
extern FILE *fp;
extern superBlock sys;
extern FCB fcb_list[FCB_NUM];
extern IB_AVLTree free_ib_tree;
extern IB_AVLTree id_tree;
extern IB_Disk free_ib_tmp;
extern int current_fcb_id;
extern int sys_mounted;
extern char* ib_tmp;
extern char fs_path[100];
extern char current_path[128];
/*全局变量*/

#endif // !F_BASE_H


