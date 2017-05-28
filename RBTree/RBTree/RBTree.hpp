#pragma once
#include<iostream>
using namespace std;
enum COLOR
{RED,BLACK};

//红黑树结点结构
template <class K,class V>
struct RBTreeNode
{
	RBTreeNode(const K& key = K(), const V& value = V())
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

//迭代器结构
template <class K,class V,class Ref,class Ptr>
class RBTreeIterator
{
	typedef RBTreeNode<K, V> Node;
	typedef RBTreeIterator<K, V, Ref,Ptr> self;
	typedef Node* Ptr;
public:
	RBTreeIterator()
		:_pNode(NULL)
	{}

	RBTreeIterator(Node* pNode)
		:_pNode(pNode)
	{}
	RBTreeIterator(const self& s)
		:_pNode(s._pNode)
	{}
	self& operator++()
	{
		Increment();
		return *this;
	}
	self operator++(int)
	{
		self temp(*this);
		Increment();
		return temp;
	}
	self& operator--()
	{
		Decrement();
		return *this;
	}
	self operator--(int)
	{
		self temp(*this);
		Decrement();
		return temp;
	}
	Ref& operator*()
	{
		return _pNode->_key;
	}
	Ptr operator->()
	{
		return &(operator*());
	}
	bool operator==(const self& s)
	{
		return _pNode == s._pNode;
	}
	bool operator!=(const self& s)
	{
		return _pNode != s._pNode;
	}
protected:
	void Increment()
	{
		if (_pNode->_pRight)
		{
			_pNode = _pNode->_pRight;
			while (_pNode->_pLeft)
				_pNode = _pNode->_pLeft;
		}
		else
		{
			Node* parent = _pNode->_pParent;
			//右单支
			while (parent->_pRight == _pNode)
			{
				_pNode = parent;
				parent = parent->_pParent;
			}
			if (_pNode->_pRight != parent)//左单支，根结点有右孩子，如果不判断直接跳到结尾
				_pNode = parent;
		}
	}
	void Decrement()
	{
		if (RED == _pNode->_color && _pNode->_pParent->_pParent == _pNode)
		{
			_pNode = _pNode->_pRight;
		}
		else if (_pNode->_pLeft)
		{
			_pNode = _pNode->_pLeft;
			while (_pNode->_pRight)
				_pNode = _pNode->_pRight;
		}
		else
		{
			Node* parent = _pNode->_pParent;
			while (parent->_pLeft == _pNode)
			{
				_pNode = parent;
				parent = parent->_pParent;
			}
			_pNode = parent;
		}
	}
private:
	Node* _pNode;
}; 

//红黑树
template <class K,class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	typedef RBTreeIterator<K, V, K&, K*> Iterator;
public:
	RBTree()
	{
		_pRoot = new Node();
		_pRoot->_pLeft = _pRoot;
		_pRoot->_pRight = _pRoot;
		_pRoot->_pParent = NULL;
		_Size = 0;
	}
	Iterator Begin()
	{
		return Iterator(_pRoot->_pLeft);
	}
	Iterator End()
	{
		return Iterator(_pRoot);
	}
	bool Insert(const K& key = K(), const V& value = V())
	{
		Node*& pRoot = GetRoot();
		//树为空
		if (NULL == pRoot)
		{
			pRoot = new Node(key);
			pRoot->_color = BLACK;
			pRoot->_pParent = _pRoot;
			_pRoot->_pParent = pRoot;
		}
		else
		{
			//查找插入位置
			Node* pCur = pRoot;
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
			pCur = new Node(key, value);
			if (key < parent->_key)
				parent->_pLeft = pCur;
			else
				parent->_pRight = pCur;
			pCur->_pParent = parent;
			//树不空
			//左
			//右
			while (pCur != pRoot && RED == parent->_color)
			{
				Node* Grandfather = parent->_pParent;
				//根据parent是grandfather的左孩子或者右孩子分为两个大的分支
				//每个分支包含三种小情况
				if (parent == Grandfather->_pLeft)
				{
					Node* Uncle = Grandfather->_pRight;
					//根据叔叔结点的情况分离情况3和情况4，5
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
						//情况5经过一次旋转后变为情况4，
						//先处理情况5
						if (pCur == parent->_pRight)
						{
							_RotateL(parent);//分清楚应给从哪个结点开始调整
							std::swap(parent, pCur);
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
							std::swap(parent, pCur);
						}
						//情况4
						parent->_color = BLACK;
						Grandfather->_color = RED;
						_RotateL(Grandfather);
					}
				}
			}
		}
		_Size++;
		pRoot->_color = BLACK;
		_pRoot->_pLeft = GetMinNode();
		_pRoot->_pRight = GetMaxNode();
	}
	void InOrder()
	{
		Node* pRoot = GetRoot();
		cout << "InOrder :";
		_InOrder(pRoot);
		cout << endl;
	}
	bool CheckRBTree()
	{
		Node* pRoot = GetRoot();
		if (pRoot == NULL)
			return true;
		if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight)
		{
			if (RED == pRoot->_color)
				cout << "违反了性质2" << endl;
		}
		//统计一条路径上的黑色结点数
		size_t BlackCount = 0;
		Node* pCur = pRoot;
		while (pCur)
		{
			if (pCur->_color == BLACK)
				BlackCount++;
			pCur = pCur->_pLeft;
		}
		return _CheckRBTree(pRoot, BlackCount, 0);
	}
	Iterator Find(const K& key)
	{
		return Iterator(_Find(key));
	}
	bool Empty()const
	{
		//return _Size == 0;

		if (NULL == _pRoot->_pParent)
			return true;
		else
			return false;
	}
	size_t Size()
	{
		return _Size;
	}

protected:

	Node* _Find(const K key)
	{
		Node* pRoot = GetRoot();
		if (NULL == pRoot)
			return NULL;
		while (key != pRoot->_key)
		{
			 if (key < pRoot->_key)
				pRoot = pRoot->_pLeft;
			else
				pRoot = pRoot->_pRight;
		}
		return pRoot;
	}
	Node*& GetRoot() //???&
	{
		return _pRoot->_pParent;
	}
	Node* GetMinNode()
	{
		Node* pRoot = GetRoot();
		if (NULL == pRoot)
			return _pRoot;////???
		while (pRoot->_pLeft)
			pRoot = pRoot->_pLeft;
		return pRoot;
	}
	Node* GetMaxNode()
	{
		Node* pRoot = GetRoot();
		if (NULL == pRoot)
			return _pRoot;//???
		while (pRoot->_pRight)
			pRoot = pRoot->_pRight;
		return pRoot;
	}
	void _RotateL(Node* parent)
	{
		Node* pRoot = GetRoot();
		Node* pSubR = parent->_pRight;
		Node* pSubL = pSubR->_pLeft;

		parent->_pRight = pSubL;
		if (pSubL)
			pSubL->_pParent = parent;
		pSubR->_pLeft = parent;
		Node* pparent = parent->_pParent;
		parent->_pParent = pSubR;
		pSubR->_pParent = pparent;
		if (_pRoot == pparent)//更改，原来pparent ==  NULL
		{
			pRoot = pSubR;
			_pRoot->_pParent = pRoot;
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
		Node* pRoot = GetRoot();
		Node* pSubL = parent->_pLeft;
		Node* pSubR = pSubL->_pRight;

		parent->_pLeft = pSubR;
		if (pSubR)
			pSubR->_pParent = parent;
		pSubL->_pRight = parent;
		Node* pparent = parent->_pParent;
		parent->_pParent = pSubL;
		pSubL->_pParent = pparent;

		
		if (_pRoot == pparent)//更改，原来pparent ==  NULL
		{
			pRoot = pSubL;
			_pRoot->_pParent = pRoot;//更改
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
	size_t _Size;
};




void Test()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> b;
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		b.Insert(a[idx]);
	b.InOrder();
	//b.CheckRBTree();
	RBTree<int, int>::Iterator it = b.Begin();
	RBTree<int, int>::Iterator it1 = b.End();

	while (it != b.End())
	{
		cout << *it << " ";
		++it;
	}
	RBTree<int, int>::Iterator it2 = b.Find(11);
	cout << b.Size() << endl;
	it1--;
	cout << *it1 << endl;
}