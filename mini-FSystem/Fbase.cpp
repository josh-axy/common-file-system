#include "pch.h"
#include "Fbase.h"

/*全局变量*/
FILE *fp;
superBlock sys;
FCB fcb_list[FCB_NUM];
IB_Disk free_ib_tmp;
IB_AVLTree free_ib_tree;
IB_AVLTree id_tree;
int current_fcb_id;
int sys_mounted = 0;
char* ib_tmp;
char fs_path[100] = "D:\\";
char current_path[128] = { '\0' };

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
IB_AVLNode::IB_AVLNode() :p_lchild_(NULL), p_rchild_(NULL), p_parent_(NULL), height_(1) {}

IB_AVLNode::IB_AVLNode(const int id) : p_lchild_(NULL), p_rchild_(NULL), p_parent_(NULL), height_(1), size_(id) {}

IB_AVLNode::IB_AVLNode(const int id, const int size, const int last_id, const int next_id) : p_lchild_(NULL), p_rchild_(NULL), p_parent_(NULL), height_(1), size_(size), IB_id_(id), last_id_(last_id), next_id_(next_id){}

IB_AVLNode::IB_AVLNode(const IB_AVLNode & ib) : p_lchild_(NULL), p_rchild_(NULL), p_parent_(NULL), height_(1), size_(ib.size_), IB_id_(ib.IB_id_), last_id_(ib.last_id_), next_id_(ib.next_id_) {}

IB_AVLNode::IB_AVLNode(const IB_Disk & ib) : p_lchild_(NULL), p_rchild_(NULL), p_parent_(NULL), height_(1), size_(ib.size), IB_id_(ib.block_id), last_id_(ib.last_id), next_id_(ib.next_id) {}

void IB_AVLNode::SetValue(const int id, const int size)
{
	IB_id_ = id;
	size_ = size;
}

void IB_AVLNode::SetValue(const IB_Disk & ib)
{
	IB_id_ = ib.block_id;
	size_ = ib.size;
	last_id_ = ib.last_id;
	next_id_ = ib.next_id;
}

void IB_AVLNode::Print()
{
	cout << "id:" << IB_id_ << ", size:" << size_ << endl;
}

int IB_AVLNode::GetSize() const
{
	return size_;
}


int IB_AVLNode::GetId() const
{
	return IB_id_;
}

int IB_AVLNode::Last() const
{
	return last_id_;
}

int IB_AVLNode::Next() const
{
	return next_id_;
}

IB_AVLNode* IB_AVLNode::lchild()
{
	return p_lchild_;
}

IB_AVLNode* IB_AVLNode::rchild()
{
	return p_rchild_;
}

IB_AVLNode* IB_AVLNode::parent()
{
	return p_parent_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
IB_AVLTree::IB_AVLTree()
{
	//root_ = new IB_AVLNode();
	root_ = 0;
}


IB_AVLTree::~IB_AVLTree()
{
	Clear();
}

IB_AVLNode* IB_AVLTree::GetRoot()
{
	return root_;
}

void IB_AVLTree::Insert(IB_Disk* ib)
{
	Insert(ib->block_id, ib->size, ib->last_id, ib->next_id);
	id_tree.Insert(ib->block_id);
}

void IB_AVLTree::Insert(IB_AVLNode* ib)
{
	Insert(ib->IB_id_, ib->size_, ib->last_id_, ib->next_id_);
	id_tree.Insert(ib->IB_id_);
}

void IB_AVLTree::Insert(const int id)
{
	Insert(id, id, 0, 0);
}

void IB_AVLTree::Insert(const int id, const int size, const int last_id, const int next_id)
{
	IB_AVLNode *p_parent = 0;
	IB_AVLNode *p_new_node = root_;

	//此段寻找插入点;
	while (p_new_node != NULL)
	{
		p_parent = p_new_node;
		if (id == p_parent->IB_id_)
		{
			cout << "IB_id duplicate. Please delete first." << endl;
			return;
		}
		if (size < p_parent->size_ || size == p_parent->size_ && id < p_parent->IB_id_)
		{
			p_new_node = p_parent->p_lchild_;
		}
		else if (size > p_parent->size_ || size == p_parent->size_ && id > p_parent->IB_id_)
		{
			p_new_node = p_parent->p_rchild_;
		}
	}

	//此段程序插入结点;
	p_new_node = new IB_AVLNode(id, size, last_id, next_id);
	if (root_ == NULL)
	{
		root_ = p_new_node;
		return;
	}
	else if (size < p_parent->size_ || size == p_parent->size_ && id < p_parent->IB_id_)  //新结点值小，插在左边;
	{
		p_parent->p_lchild_ = p_new_node;
		p_new_node->p_parent_ = p_parent;
	}
	else
	{
		p_parent->p_rchild_ = p_new_node;
		p_new_node->p_parent_ = p_parent;
	}

	InsertAdjust(p_parent);
}

void IB_AVLTree::Delete(IB_AVLNode* ib)
{
	Delete(ib->IB_id_, ib->size_);
	id_tree.Delete(ib->IB_id_);
}

void IB_AVLTree::Delete(IB_Disk* ib)
{
	Delete(ib->block_id, ib->size);
	id_tree.Delete(ib->block_id);
}

void IB_AVLTree::Delete(const int id)
{
	Delete(id, id);
}

void IB_AVLTree::Delete(const int id, const int size)
{
	IB_AVLNode *p_delete_position = Search(id, size);
	//IB_AVLNode *p_best_replacement = 0;

	if (p_delete_position)
	{
		IB_AVLNode *p_best_replacement = FindBestReplacement(p_delete_position);
		IB_AVLNode* p_parent = 0;
		if (p_best_replacement != 0) //存在替代结点;
		{
			p_delete_position->size_ = p_best_replacement->size_;
			p_delete_position->IB_id_ = p_best_replacement->IB_id_;
			p_parent = p_best_replacement->p_parent_;
			if (p_parent->p_lchild_ == p_best_replacement)
			{
				p_parent->p_lchild_ = 0;
			}
			else
			{
				p_parent->p_rchild_ = 0;
			}
			delete p_best_replacement;

			DeleteAdjust(p_parent);
		} //if(p_best_replacement != 0)
		else//p_best_replacement == 0,叶子结点;
		{
			if (p_delete_position->p_parent_ == NULL)
			{
				root_ = NULL;
			}
			else
			{
				p_parent = p_delete_position->p_parent_;
				if (p_parent->p_lchild_ == p_delete_position)
				{
					p_parent->p_lchild_ = NULL;
				}
				else
				{
					p_parent->p_rchild_ = NULL;
				}
				UpdateHeight(p_parent);
			}
			delete p_delete_position;
		}
	}// if(p_delete_position != 0)

}

IB_Disk* IB_AVLTree::Search(IB_Disk* ib)
{
	IB_AVLNode *p_node = Search(ib->block_id, ib->size);
	ib->block_id = p_node->IB_id_;
	ib->last_id = p_node->last_id_;
	ib->next_id = p_node->next_id_;
	ib->size = p_node->size_;
	return ib;
}

IB_AVLNode* IB_AVLTree::Search(IB_AVLNode* ib)
{
	IB_AVLNode *p_node = Search(ib->IB_id_, ib->size_);
	return p_node;
}

IB_AVLNode* IB_AVLTree::Search(const int id, const int size)
{
	IB_AVLNode *p_node = root_;
	IB_AVLNode *best_node = NULL;
	while (p_node)
	{
		if (size == p_node->size_ && id == p_node->IB_id_)
		{
			return p_node;
		}
		if (size <= p_node->size_ && id == -1)
		{
			if (p_node->p_lchild_ == NULL || p_node->p_lchild_->size_ < size)
			{
				best_node = p_node;
			}
		}
		if (size < p_node->size_ || size == p_node->size_ && id < p_node->IB_id_)
		{
			p_node = p_node->p_lchild_;
		}
		else
		{
			p_node = p_node->p_rchild_;
		}
	}
	if (best_node)
	{
		return best_node;
	}
	return NULL;
}


void IB_AVLTree::PrintDLR()
{
	//if (root_ == 0)  //空树;
	//{
	//	return;
	//}

	stack<IB_AVLNode*> path;
	IB_AVLNode *p_node = root_;
	path.push(root_);

	while (!path.empty())
	{
		p_node = path.top();
		path.pop();
		if (p_node != 0) //非空结点输出，且左右孩子进栈;
		{
			cout << "blk_id:" << p_node->IB_id_ << ", blk_size:" << p_node->size_ << endl;
			path.push(p_node->p_rchild_);
			path.push(p_node->p_lchild_);
		}
	}
}


void IB_AVLTree::PrintLDR()
{
	PrintLDR(root_);
}

void IB_AVLTree::PrintLDR(IB_AVLNode* p_node)
{
	if (p_node == 0)
	{
		return;
	}

	PrintLDR(p_node->p_lchild_);
	cout << "blk_id:" << p_node->IB_id_ << ", blk_size:" << p_node->size_ << endl;
	PrintLDR(p_node->p_rchild_);
}

void IB_AVLTree::PrintRDL()
{
	PrintRDL(root_);
}

void IB_AVLTree::PrintRDL(IB_AVLNode* p_node)
{
	if (p_node == 0)
	{
		return;
	}

	PrintRDL(p_node->p_rchild_);
	cout << "blk_id:" << p_node->IB_id_ << ", blk_size:" << p_node->size_ << endl;
	PrintRDL(p_node->p_lchild_);
}

void IB_AVLTree::Clear()
{
	if (root_ == 0)
	{
		return;
	}
	stack<IB_AVLNode*> path;
	IB_AVLNode *p_node = root_;
	path.push(root_);
	root_ = 0;

	while (!path.empty())
	{
		p_node = path.top();
		path.pop();
		if (p_node != 0) //非空结点输出，且左右孩子进栈;
		{
			//cout << "    " << p_node->size_;
			path.push(p_node->p_rchild_);
			path.push(p_node->p_lchild_);
			delete p_node;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
//                            从此往下为私有函数																//
//                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IB_AVLTree::Rotate(IB_AVLNode *p_node)
{
	if (p_node == 0)
	{
		return;
	}
	if (GetBalance(p_node) < 0)
	{
		if (GetBalance(p_node->p_rchild_) <= 0)
		{
			LL(p_node);
		}
		else
		{
			RR(p_node->p_rchild_);
			LL(p_node);
		}
	}
	else
	{
		if (GetBalance(p_node->p_lchild_) >= 0)
		{
			RR(p_node);
		}
		else
		{
			LL(p_node->p_lchild_);
			RR(p_node);
		}
	}
}


void IB_AVLTree::LL(IB_AVLNode *p_old_subtree_root)
{
	if (p_old_subtree_root == 0)
	{
		return;
	}
	IB_AVLNode* p_new_subtree_root = p_old_subtree_root->p_rchild_;

	//这段程序调整位置关系;
	IB_AVLNode *p_parent = p_old_subtree_root->p_parent_;
	//此段把新子树与整棵树连接;
	if (p_parent == 0)
	{
		root_ = p_new_subtree_root;
		p_new_subtree_root->p_parent_ = 0;
	}
	else
	{
		if (p_parent->p_lchild_ == p_old_subtree_root)
		{
			p_parent->p_lchild_ = p_new_subtree_root;
			p_new_subtree_root->p_parent_ = p_parent;
		}
		else
		{
			p_parent->p_rchild_ = p_new_subtree_root;
			p_new_subtree_root->p_parent_ = p_parent;
		}
	}

	p_old_subtree_root->p_rchild_ = p_new_subtree_root->p_lchild_;
	if (p_old_subtree_root->p_rchild_ != 0)
	{
		p_old_subtree_root->p_rchild_->p_parent_ = p_old_subtree_root;
	}

	p_new_subtree_root->p_lchild_ = p_old_subtree_root;
	p_old_subtree_root->p_parent_ = p_new_subtree_root;

	//这一段更新height值;
	UpdateHeight(p_old_subtree_root);
	UpdateHeight(p_new_subtree_root);
}



void IB_AVLTree::RR(IB_AVLNode *p_old_subtree_root)
{
	if (p_old_subtree_root == 0)
	{
		return;
	}
	IB_AVLNode* p_new_subtree_root = p_old_subtree_root->p_lchild_;

	//这段程序调整位置关系;
	IB_AVLNode *p_parent = p_old_subtree_root->p_parent_;
	//此段把新子树与整棵树连接;
	if (p_parent == 0)
	{
		root_ = p_new_subtree_root;
		p_new_subtree_root->p_parent_ = 0;
	}
	else
	{
		if (p_parent->p_lchild_ == p_old_subtree_root)
		{
			p_parent->p_lchild_ = p_new_subtree_root;
			p_new_subtree_root->p_parent_ = p_parent;
		}
		else
		{
			p_parent->p_rchild_ = p_new_subtree_root;
			p_new_subtree_root->p_parent_ = p_parent;
		}
	}

	p_old_subtree_root->p_lchild_ = p_new_subtree_root->p_rchild_;
	if (p_old_subtree_root->p_lchild_ != 0)
	{
		p_old_subtree_root->p_lchild_->p_parent_ = p_old_subtree_root;
	}

	p_new_subtree_root->p_rchild_ = p_old_subtree_root;
	p_old_subtree_root->p_parent_ = p_new_subtree_root;

	//这一段更新balance值;
	UpdateHeight(p_old_subtree_root);
	UpdateHeight(p_new_subtree_root);

}


//插入结点引起的不平衡只需调整最小不平衡子树即可;
void IB_AVLTree::InsertAdjust(IB_AVLNode* p_parent_new_node)
{
	if (p_parent_new_node == 0)
	{
		return;
	}
	IB_AVLNode* p_root_min_unbalance_subtree = FindMinUnbalanceSubtree(p_parent_new_node);

	if (p_root_min_unbalance_subtree != 0)
	{
		Rotate(p_root_min_unbalance_subtree);
	}
}


void IB_AVLTree::DeleteAdjust(IB_AVLNode* p_parent_deleted_node)
{
	if (p_parent_deleted_node == 0)
	{
		return;
	}
	IB_AVLNode* p_parent = p_parent_deleted_node;
	IB_AVLNode* p_node = FindMinUnbalanceSubtree(p_parent);
	while (p_node != 0)
	{
		p_parent = p_node->p_parent_;
		Rotate(p_node);
		p_node = FindMinUnbalanceSubtree(p_parent);
	}
}


int IB_AVLTree::GetBalance(IB_AVLNode* p_node)
{
	if (p_node == 0)
	{
		return 0;
	}
	if (p_node->p_lchild_ == 0 || p_node->p_rchild_ == 0)
	{
		if (p_node->p_lchild_ == 0)
		{
			if (p_node->p_rchild_ == 0)
			{
				return 0;
			}
			return -p_node->p_rchild_->height_;
		}
		else
		{
			return p_node->p_lchild_->height_;
		}
	}
	else
	{
		return p_node->p_lchild_->height_ - p_node->p_rchild_->height_;
	}
}


void IB_AVLTree::UpdateHeight(IB_AVLNode* p_node)
{
	if (p_node == 0)
	{
		return;
	}

	int balance_value = GetBalance(p_node);
	if (p_node->p_lchild_ != 0 || p_node->p_rchild_ != 0)
	{
		if (balance_value > 0)
		{
			p_node->height_ = p_node->p_lchild_->height_ + 1;
		}
		else
		{
			p_node->height_ = p_node->p_rchild_->height_ + 1;
		}
	}
	else
	{
		p_node->height_ = 1;
	}
}


//参数为空会返回0值;
IB_AVLNode* IB_AVLTree::FindMinUnbalanceSubtree(IB_AVLNode *p_node)
{
	if (p_node == 0)
	{
		return 0;
	}
	IB_AVLNode* p_parent = p_node;  //总是指向p_node的父结点; 此变量为效率而存在;
	bool is_grow = 1;
	while (p_parent != 0 && is_grow)
	{
		int old_height = p_parent->height_;
		UpdateHeight(p_parent);

		if (GetBalance(p_parent) == 2 || GetBalance(p_parent) == -2)  //找到最小不平衡子树;
		{
			return p_parent;
		}

		if (old_height == p_parent->height_)
		{
			is_grow = 0;
		}
		p_node = p_parent;
		p_parent = p_parent->p_parent_;
	}

	return 0; //没有找到符合要求的结点;
}


IB_AVLNode* IB_AVLTree::FindBestReplacement(IB_AVLNode* p_node)
{
	if (p_node == 0)
	{
		return 0;
	}
	IB_AVLNode* p_replacement = 0;
	if (p_node == 0)
	{
		return 0;
	}
	else if (GetBalance(p_node) > 0)
	{
		p_replacement = p_node->p_lchild_;
		while (p_replacement && p_replacement->p_rchild_)
		{
			p_replacement = p_replacement->p_rchild_;
		}
	}
	else
	{
		p_replacement = p_node->p_rchild_;
		while (p_replacement && p_replacement->p_lchild_)
		{
			p_replacement = p_replacement->p_lchild_;
		}
	}

	return p_replacement;
}