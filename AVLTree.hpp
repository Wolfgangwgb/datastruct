#pragma once
#include<iostream>
using namespace std;

template <class K,class V>
struct AVLTreeNode
{
	AVLTreeNode(const K& key,const V& value = V())
	:_pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _bf(0)
	, _key(key)
	, _value(value)
	{}
	AVLTreeNode<K, V>* _pLeft;
	AVLTreeNode<K, V>*_pRight;
	AVLTreeNode<K, V>*_pParent;
	K _key;
	V _value;
	int _bf;
};


template <class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K,V> Node;
public:
	AVLTree()
		:_pRoot(NULL)
	{}
	bool Insert(const K& key, const V& value=V())
	{
		//空树
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		//树不为空，寻找插入的位置
		Node* pCur = _pRoot;
		Node* parent = NULL;
		while (pCur)
		{
			if (key < pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (key > pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else
			{
				return false;
			}
		}
		//插入位置找到
		pCur = new Node(key,value);
		if (parent->_key > pCur->_key)
		{
			parent->_pLeft = pCur;
			pCur->_pParent = parent;
		}
		else
		{
			parent->_pRight = pCur;
			pCur->_pParent = parent;
		}
		//更新平衡因子

		while (parent)
		{
			//1.判断是双亲的左孩子(--)或者右孩子(++)
			if (pCur == parent->_pLeft)//放在循环里面
				parent->_bf--;
			else
				parent->_bf++;
			//2.如果parent的_bf等于0，就不在向上调整
			if (parent->_bf == 0)
				return true;
			//3.向上调整
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				pCur = parent;
				parent = parent->_pParent;
			}
			else
			{
				if (parent->_bf == 2)
				{
					//1.左单旋
					//2.先右后左
					if (pCur->_bf == 1)
						_RotateL(parent);
					else
						_RotateRL(parent);
				}
				else
				{
					//1.右单旋
					//2.先左后右
					if (pCur->_bf == -1)
						_RotateR(parent);
					else
						_RotateLR(parent);
				}
				break;///????
			}
		}
	}

	void InOrder()
	{
		cout << "InOrder" << endl;
		_InOrder(_pRoot);
		cout << endl;
	}
	size_t Height()
	{
		return _Height(_pRoot);
	}

	bool IsBalanceTree()
	{
		return _IsBlanceTree(_pRoot);
	}

	bool Remove(const K& key)//删除
	{
		if (NULL == _pRoot)
			return false;
		Node* pCur = _pRoot;
		Node* parent = NULL;
		while (pCur)
		{
			if (key < pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (key>pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else
			{
				//parent = pCur;
				break;
			}
		}
		if (NULL == pCur)
			return false;
		
		if (pCur->_pLeft == NULL || pCur->_pRight == NULL)
		{
			if (pCur == _pRoot)
			{
				_pRoot = pCur->_pRight;
				delete pCur;
			}
			else if (pCur == parent->_pRight)
			{
				if (pCur->_pRight)
				{
					parent->_pRight = pCur->_pRight; parent->_bf--;
				}
				else
				{
					parent->_pRight = pCur->_pLeft; parent->_bf--;
				}
				delete pCur;

			}
			else if (pCur == parent->_pLeft)
			{
				if (pCur->_pRight)
				{
					parent->_pLeft = pCur->_pRight; parent->_bf++;
				}
				else
				{
					parent->_pLeft = pCur->_pLeft; parent->_bf++;
				}
				delete pCur;
			}

		}
		else
		{
			//在右子树中找中序遍历的第一个节点
			Node* prev = pCur;

			pCur = pCur->_pRight;
			while (pCur->_pLeft)
			{
				prev = pCur;
				pCur = pCur->_pLeft;
			}
			if (prev->_pRight == pCur)
			{
				prev->_pRight = pCur->_pRight;
				delete pCur;
			}
			else
			{
				parent->_key = pCur->_key;
				parent->_value = pCur->_value;
				delete pCur;
			}
			while (parent)
			{
				//1.判断是双亲的左孩子(--)或者右孩子(++)
				if (pCur == parent->_pLeft)//放在循环里面
					parent->_bf--;
				else
					parent->_bf++;
				//2.如果parent的_bf等于0，就不在向上调整
				if (parent->_bf == 0)
					return true;
				//3.向上调整
				else if (parent->_bf == -1 || parent->_bf == 1)
				{
					pCur = parent;
					parent = parent->_pParent;
				}
				else
				{
					if (parent->_bf == 2)
					{
						//1.左单旋
						//2.先右后左
						if (pCur->_bf == 1)
							_RotateL(parent);
						else
							_RotateRL(parent);
					}
					else
					{
						//1.右单旋
						//2.先左后右
						if (pCur->_bf == -1)
							_RotateR(parent);
						else
							_RotateLR(parent);
					}
					break;///????
				}
			}
		}
	}
protected:
	void _RotateL(Node* parent)
	{
		Node* pSubR = parent->_pRight;
		Node* pSubL = pSubR->_pLeft;

		parent->_pRight = pSubL;
		if (pSubL)//不存在的那个结点
			pSubL->_pParent = parent;

		pSubR->_pLeft = parent;
		Node* ppParent = parent->_pParent;
		parent->_pParent = pSubR;
		pSubR->_pParent = ppParent;

		if (NULL == ppParent)
		{
			_pRoot = pSubR;
		}
		else
		{
			if (parent == ppParent->_pLeft)
				ppParent->_pLeft = pSubR;
			else
				ppParent->_pRight = pSubR;
		}
		parent->_bf = 0;
		pSubR->_bf = 0;
	}
	void _RotateRL(Node* parent)
	{
		_RotateR(parent->_pRight);
		_RotateL(parent);
		//更改平衡因子
		_TurnBlance(_pRoot);
	}
	void _RotateR(Node* parent)
	{
		Node* pSubL = parent->_pLeft;
		Node* pSubR = pSubL->_pRight;

		parent->_pLeft = pSubR;
		if (pSubR)//右子树可能不存在
			pSubR->_pParent = parent;

		pSubL->_pRight = parent;
		Node* ppParent = parent->_pParent;
		parent->_pParent = pSubL;
		pSubL->_pParent = ppParent;

		if (NULL == ppParent)
		{
			_pRoot = pSubL;
		}
		else
		{
			if (ppParent->_pLeft == parent)
				ppParent->_pLeft = pSubL;
			else
				ppParent->_pRight = pSubL;
		}
		parent->_bf = 0;
		pSubL->_bf = 0;
	}
	void _RotateLR(Node* parent)
	{
		_RotateL(parent->_pLeft);
		_RotateR(parent);
		//更新平衡因子
		_TurnBlance(_pRoot);
	}

	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}

	size_t _Height(Node* pRoot)
	{
		if (NULL == pRoot)
			return 0;
		if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
			return 1;
		size_t left = _Height(pRoot->_pLeft);
		size_t right = _Height(pRoot->_pRight);
		return left > right ? left + 1 : right + 1;
	}
	void _TurnBlance(Node* pRoot)
	{
		if (pRoot)
		{
			size_t left = _Height(pRoot->_pLeft);
			size_t right = _Height(pRoot->_pRight);
			if (right - left != pRoot->_bf)
			{
				pRoot->_bf = right - left;
			}
			_TurnBlance(pRoot->_pLeft);
			_TurnBlance(pRoot->_pRight);
		}
	}
	/////面试题
	bool _IsBlanceTree(Node* pRoot)
	{
		//树空
		if (NULL == pRoot)
			return true;
		//求左右子树高度
		size_t left = _Height(pRoot->_pLeft);
		size_t right = _Height(pRoot->_pRight);
		//右子树高度-左子树高度不等于当前结点的平衡因子或者当前结点的平衡因子绝对值大于1，就不是AVL树
		if (right - left != pRoot->_bf || abs(pRoot->_bf) > 1)
			return false;
		//&&都为真才为真，当左右子树都是平衡二叉树该树就是AVL树
		return _IsBlanceTree(pRoot->_pLeft) && _IsBlanceTree(pRoot->_pRight);
	}
protected:
	Node* _pRoot;
};


void TestAVL()
{
	int array[] = {16, 3, 7, 11, 9, 26, 18, 14, 15};
	//int array[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> v;
	for (size_t idx = 0; idx < sizeof(array) / sizeof(array[0]); ++idx)
		v.Insert(array[idx]);
	v.InOrder();
	if (v.IsBalanceTree())
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
	v.Remove(26);
	if (v.IsBalanceTree())
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
	v.InOrder();
}

