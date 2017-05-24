#pragma once
#include<iostream>
using namespace std;
enum COLOR
{
	RED,
	BLACK
};
template <class K,class V>
struct RBTreeNode
{
	RBTreeNode(const K& key, const V& value = V())
	:_pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _key(key)
	, _value(value)
	, _color(RED)
	{}
	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>*_pRight;
	RBTreeNode<K, V>*_pParent;
	K _key;
	V _value;
	COLOR _color;
};

template <class K,class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value = V())
	{
		//树为空
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			_pRoot->_color = BLACK;
			return true;
		}
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
				return false;
			}
		}
		pCur = new Node(key,value);
		if (key < parent->_key)
			parent->_pLeft = pCur;
		else
			parent->_pRight = pCur;
		pCur->_pParent = parent;
		//树不空
		//左
		//右
		while (pCur != _pRoot && RED == parent->_color)
		{
			Node* Grandfather = parent->_pParent;
			if (parent == Grandfather->_pLeft)
			{
				Node* Uncle = Grandfather->_pRight;
				//情况3
				if (Uncle && RED == Uncle->_color)
				{
					parent->_color = BLACK;
					Uncle->_color = BLACK;
					Grandfather->_color = RED;

					pCur = Grandfather;
					parent = Grandfather->_pParent;
				}
				else
				{
					//先处理情况5
					if (pCur == parent->_pRight)
					{
						_RotateL(parent);
						std::swap(parent,pCur);
					}
					//情况4
					parent->_color = BLACK;
					Grandfather->_color = RED;
					_RotateR(Grandfather);
				}
			}
			else
			{
				Node* Uncle = Grandfather->_pLeft;
				//情况3
				if (Uncle && RED == Uncle->_color)
				{
					parent->_color = BLACK;
					Uncle->_color = BLACK;
					Grandfather->_color = RED;

					pCur = Grandfather;
					parent = Grandfather->_pParent;
				}
				else
				{
					//情况5
					if (pCur == parent->_pLeft)
					{
						_RotateR(parent);
						std::swap(parent,pCur);
					}
					//情况4
					parent->_color = BLACK;
					Grandfather->_color = RED;
					_RotateL(Grandfather);
				}
			}
		}
		_pRoot->_color = BLACK;
	}
	void InOrder()
	{
		cout << "InOrder :";
		_InOrder(_pRoot);
		cout << endl;
	}
	bool CheckRBTree()
	{
		if (_pRoot == NULL)
			return true;
		if (NULL == _pRoot->_pLeft && NULL == _pRoot->_pRight)
		{
			if (RED == _pRoot->_color)
				cout << "违反了性质2" << endl;
		}

		size_t BlackCount = 0;
		Node* pCur = _pRoot;
		while (pCur)
		{
			if (pCur->_color == BLACK)
				BlackCount++;
			pCur = pCur->_pLeft;
		}
		return _CheckRBTree(_pRoot,BlackCount,0);
	}
protected:
	void _RotateL(Node* parent)
	{
		Node* pSubR = parent->_pRight;
		Node* pSubL = pSubR->_pLeft;

		parent->_pRight = pSubL;
		if (pSubL)
			pSubL->_pParent = parent;
		pSubR->_pLeft = parent;
		Node* pparent = parent->_pParent;
		parent->_pParent = pSubR;
		pSubR->_pParent = pparent;
		if (NULL == pparent)
		{
			_pRoot = pSubR;
		}
		else
		{
			if (parent == pparent->_pLeft)
				pparent->_pLeft = pSubR;
			else
				pparent->_pRight = pSubR;
		}
	}
	void _RotateR(Node* parent)
	{
		Node* pSubL = parent->_pLeft;
		Node* pSubR = pSubL->_pRight;

		parent->_pLeft = pSubR;
		if (pSubR)
			pSubR->_pParent = parent;
		pSubL->_pRight = parent;
		Node* pparent = parent->_pParent;
		parent->_pParent = pSubL;
		pSubL->_pParent = pparent;

		if (NULL == pparent)
		{
			_pRoot = pSubL;
		}
		else
		{
			if (parent == pparent->_pLeft)
				pparent->_pLeft = pSubL;
			else
				pparent->_pRight = pSubL;
		}
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

	bool _CheckRBTree(Node* pRoot, size_t blackcout, size_t k)
	{
		if (pRoot == NULL)
			return true;
		if (pRoot->_color == BLACK)
			k++;
		if (pRoot->_color == RED)
		{
			Node* pCur = pRoot->_pLeft;
			if (pCur && pCur->_color == RED)
			{
				cout << "违反了性质3" << endl;
				return false;
			}
		}
		if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
		{
			if (blackcout != k)
			{
				cout << "违反了性质4" << endl;
				return false;
			}
		}
		return _CheckRBTree(pRoot->_pLeft, blackcout, k) && _CheckRBTree(pRoot->_pRight,blackcout,k);
	}
protected:
	Node* _pRoot;
};




void Test()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> b;
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		b.Insert(a[idx]);
	b.InOrder();
	b.CheckRBTree();
}