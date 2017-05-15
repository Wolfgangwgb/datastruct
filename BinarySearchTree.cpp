#pragma once
#include<iostream>
using namespace std;


template <class K,class V>
struct BSTNode
{
	BSTNode(const K& key,const V& value)
	:_pLeft(NULL)
	, _pRight(NULL)
	, _key(key)
	, _value(value)
	{}
	BSTNode<K, V>* _pLeft;
	BSTNode<K, V>* _pRight;
	K _key;
	V _value;
};


template <class K,class V>
class BinarySearchTree
{
	typedef BSTNode<K, V> Node;
	typedef BinarySearchTree<K, V> Self;

public:
	BinarySearchTree()
		:_pRoot(NULL)
	{}

	BinarySearchTree(const Self& BST)
	{
		_pRoot = _CopyBinarySearchTree(BST._pRoot);
	}
	Self& operator=(const Self& BST)
	{
		if (this == &BST)
			return *this;
		delete _pRoot;
		_pRoot = _CopyBinarySearchTree(BST._pRoot);;
		return *this;
	}
	///////////////////////////////////////
	//查找
	bool Find_Nor(const K& key)
	{
		//树为空
		if (NULL == _pRoot)
			return false;
		//树不空
		Node* pCur = _pRoot;
		while (pCur)
		{
			if (pCur->_key > key)
			{
				pCur = pCur->_pLeft;
			}
			else if (pCur->_key < key)
			{
				pCur = pCur->_pRight;
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	bool Find(const K& key)
	{
		return _Find(_pRoot,key);
	}
	///////////////////////////////////
	//插入
	bool Insert_Nor(const K& key, const V& value)
	{
		//树空，直接插入
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key,value);
			return true;
		}
		//查找要插入的元素
		Node* pCur = _pRoot;
		Node* parent = NULL;
		while (pCur)
		{
			if (pCur->_key > key)
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (pCur->_key < key)
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else
			{
				return false;
			}
		}
		//要插入的在树中不存在，在寻找停止的位置插入
		if (NULL == pCur)
		{
			if (parent->_key > key)
			{
				parent->_pLeft = new Node(key,value);
			}
			else
			{
				parent->_pRight = new Node(key,value);
			}
		}
		return true;
	}
	bool Insert(const K& key, const V& value)
	{
		return _Insert(_pRoot,key,value);
	}
	/////////////////////////////////////
	//删除
	bool Remove_Nor(const K& key)
	{
		if (NULL == _pRoot)
			return false;
		Node* pCur = _pRoot;
		Node* parent = NULL;
		while (pCur)
		{
			if (pCur->_key > key)
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (pCur->_key < key)
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else /*if (pCur->_key == key)*/
			{
				parent = pCur;
				break;
			}
		}
		if (NULL == pCur)
		{
			return false;
		}
			//右子树
		if (pCur->_pLeft == NULL )
		{
			if (pCur == _pRoot)
			{
				_pRoot = pCur->_pRight;
				delete pCur;
			}
			else if (pCur == parent->_pRight)
			{
				parent->_pRight = pCur;
				delete pCur;
			}
			else if (pCur == parent->_pLeft)
			{
				parent->_pLeft = pCur->_pRight;
				delete pCur;
			}
		}
		//左子树
		else if (pCur->_pRight == NULL )
		{
			if (pCur == _pRoot)
			{
				_pRoot = pCur->_pLeft;
				delete pCur;
			}
			else if (pCur == parent->_pLeft)
			{
				parent->_pLeft = pCur->_pLeft;
				delete pCur;
			}
			else if (pCur == parent->_pRight)
			{
				parent->_pRight = pCur->_pLeft;
				delete pCur;
			}
		}
		//左右子树
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
		}
		return true;
	}
	bool Remove(const K& key)
	{
		return _Remove(_pRoot,key);
	}

	void InOrder()
	{
		cout << "InOrder" << endl;
		_InOrder(_pRoot);
		cout << endl;
	}

	~BinarySearchTree()
	{
		if (_pRoot)
		{
			delete _pRoot;
			_pRoot = NULL;
		}
	}
private:
	Node* _CopyBinarySearchTree(Node* Root)
	{
		Node* NewNode = NULL;
		if (Root)
		{
			NewNode = new Node(Root->_key,Root->_value);
			NewNode->_pLeft = _CopyBinarySearchTree(Root->_pLeft);
			NewNode->_pRight = _CopyBinarySearchTree(Root->_pRight);
		}
		return NewNode;
	}
	bool _Find(Node* Root, const K& key)
	{
		if (Root && Root->_key == key)
		{
			return true;
		}
		else if (NULL == Root )
		{
			return false;
		}
		if (Root->_key > key)
			_Find(Root->_pLeft,key);
		if (Root->_key < key)
			_Find(Root->_pRight, key);
	}
	bool _Insert(Node*& Root,const K& key,const V& value)
	{
		if (NULL == Root)
		{
			Root = new Node(key,value);
			return true;
		}
		if (Root->_key == key && Root->_value == value)
		{
			return false;
		}
		if (Root->_key > key)
			_Insert(Root->_pLeft,key,value);
		if (Root->_key < key)
			_Insert(Root->_pRight,key,value);
		return false;
	}
	bool _Remove(Node*& Root, const K& key)
	{
		if (NULL == Root)
			return false;
		else
		{
			if (NULL == Root->_pRight && Root->_key == key)
			{
				Root = Root->_pLeft; return true;
			}
			if (NULL == Root->_pLeft&& Root->_key == key)
			{
				Root = Root->_pRight; return true;
			}
			if (Root->_pLeft && Root->_pRight&& Root->_key == key)
			{
				Root = Root->_pRight; return true;
			}
		}
		if (Root->_key > key)
			_Remove(Root->_pLeft,key);
		if (Root->_key < key)
			_Remove(Root->_pRight,key);
	}
	void _InOrder(Node* Root)
	{
		if (Root)
		{
			_InOrder(Root->_pLeft);
			cout << Root->_key << " ";
			_InOrder(Root->_pRight);
		}
	}
private:
	Node* _pRoot;
};

void Test1()
{
	int a[] = { 5, 3/*,4*/, 1, 7, 8, 2, 6, 0, 9 };
	BinarySearchTree<int, int> b;
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
	{
		b.Insert_Nor(a[idx],a[idx]);
	}

	/*
	if (b.Find_Nor(0))
		cout << "存在" << endl;
	else
		cout << "不存在" << endl;
	if (b.Find_Nor(4))
		cout << "存在" << endl;
	else
		cout << "不存在" << endl;
		*/
	//b.Remove_Nor(2);
	//b.Remove_Nor(4);
	//b.Remove_Nor(7);
	b.Remove_Nor(5);
}
void Test2()
{
	int a[] = { 5, 3/*,4*/, 1, 7, 8, 2, 6, 0, 9 };
	BinarySearchTree<int, int> b;
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
	{
		b.Insert(a[idx], a[idx]);
	}
	BinarySearchTree<int, int> b2(b);
	BinarySearchTree<int, int> b3;
	b3 = b2;
	b.InOrder();
	b2.InOrder();
	b3.InOrder();
	//b.Remove(2);
	//b.Remove(3);
	//b.Remove(8);
	//if (b.Find(7) == true)
	//	cout << "存在" << endl;
	//else
	//	cout << "不存在" << endl;
	//
	//if (b.Find(4)== true)
	//	cout << "存在" << endl;
	//else
	//	cout << "不存在" << endl;
}
int main()
{
	//Test1();
	Test2();
	return 0;
}