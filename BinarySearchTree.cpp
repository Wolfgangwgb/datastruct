#pragma once
#include<iostream>
using namespace std;

#include<vector>


template <class K,class V>
struct BSTNode
{
	BSTNode(const K& key=K(),const V& value= V())
	:_pLeft(NULL)
	, _pRight(NULL)
	, _parent(NULL)
	, _key(key)
	, _value(value)
	{}
	BSTNode<K, V>* _pLeft;
	BSTNode<K, V>* _pRight;
	BSTNode<K, V>* _parent;
	K _key;
	V _value;
};

template <class K, class V, class Ref, class Ptr>
class BinarySearchTreeIterator
{
	typedef BSTNode<K, V> Node;
	typedef BinarySearchTreeIterator<K, V, Ref, Ptr> self;
	typedef Node* ptr;
public:
	BinarySearchTreeIterator()
		:_pNode(NULL)
	{}
	BinarySearchTreeIterator(const self& s)
		:_pNode(s._pNode)
	{}
	BinarySearchTreeIterator(Node* pNode)
		:_pNode(pNode)
	{}

	self& operator++()
	{
		Increment(_pNode);
		return *this;
	}
	self operator++(int)
	{
		self temp(*this);
		Increment(_pNode);
		return temp;
	}
	self& operator--()
	{
		Decrement(_pNode);
		return *this;
	}
	self operator--(int)
	{
		self temp(*this);
		Decrement(_pNode);
		return *this;
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
	Node*  Increment(Node* pNode)
	{
		//当前结点如果他的右子树存在，找右子树中最左边的结点
		if (pNode->_pRight)
		{
			pNode = pNode->_pRight;
			while (pNode->_pLeft)//注意条件是pNode的左孩子不为空，不是pNode
				pNode = pNode->_pLeft;
		}
		else
		{
			Node* parent = pNode->_parent;
			while (parent->_pRight == pNode)
			{
				pNode = parent;
				parent = parent->_parent;
			}
			if (pNode->_pRight != parent)//????????
				pNode = parent;
		}
		_pNode = pNode;
		return pNode;
	}
	Node* Decrement(Node* pNode)
	{
		if (pNode->_pLeft)
		{//左孩子存在，找左孩子的右孩子中最大的
			pNode = pNode->_pLeft;
			while (pNode->_pRight)
				pNode = pNode->_pRight;
		}
		else
		{
			//左单支的情况
			Node* parent = pNode->_parent;
			while (parent->_pLeft == pNode)
			{
				pNode = parent;
				parent = parent->_parent;
			}
			pNode = parent;
		}
		_pNode = pNode;
		return pNode;
	}
protected:
	Node* _pNode;
};
template <class K,class V>
class BinarySearchTree
{
	typedef BSTNode<K, V> Node;
	typedef BinarySearchTree<K, V> Self;
public:
	typedef BinarySearchTreeIterator<K,V,K&,K*> Iterator;
public:
	BinarySearchTree()
	{
		_pRoot = new Node();
		_pRoot->_pLeft = _pRoot;
		_pRoot->_pRight = _pRoot;
		_pRoot->_parent = NULL;
	}
	Iterator Begin()
	{
		return Iterator(_pRoot->_pLeft);
	}
	Iterator End()
	{
		return Iterator(_pRoot);
	}

	BinarySearchTree(const Self& BST)
	{
		_pRoot = _CopyBinarySearchTree(BST._pRoot);//先序拷贝
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
		while (pCur)//
		{
			if (pCur->_key > key)
				pCur = pCur->_pLeft;
			else if (pCur->_key < key)
				pCur = pCur->_pRight;
			else
				return true;
		}
		return false;
	}
	bool Find(const K& key)
	{
		return _Find(GetRoot(),key);
	}
	///////////////////////////////////
	//插入
	bool Insert_Nor(const K& key, const V& value)
	{
		//树空，直接插入
		//++++
		Node* pRoot = GetRoot();
		if (NULL == pRoot)
		{
			pRoot = new Node(key, value);
			//++++
			_pRoot->_parent = pRoot;
			pRoot->_parent = _pRoot;
			return true;
		}
		//查找要插入的位置
		Node* pCur = pRoot;
		Node* parent = NULL;
		while (pCur)//查找要插入的位置
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
				return false;//该元素存在，
			}
		}
		//要插入的在树中不存在，在寻找停止的位置插入
		pCur = new Node(key,value);
		if (parent->_key > key)
			parent->_pLeft = pCur;
		else
			parent->_pRight = pCur;
		pCur->_parent = parent;

		//++++
		_pRoot->_pLeft = GetMinNode();
		_pRoot->_pRight = GetMaxNode();
		return true;
	}
	bool Insert(const K& key, const V& value)
	{
		return _Insert(GetRoot(),key,value);
	}
	/////////////////////////////////////
	//删除
	bool Remove_Nor(const K& key)
	{
		Node* pRoot = GetRoot();
		if (NULL == pRoot)
			return false;
		Node* pCur = pRoot;
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
				parent = pCur;//要删除的元素存在
				break;
			}
		}
		//pCur为空说明该元素不存在
		if (NULL == pCur)
		{
			return false;
		}
			//右子树
		if (pCur->_pLeft == NULL )
		{
			if (pCur == _pRoot)
			{
				pRoot = pCur->_pRight;
				delete pCur;
			}
			else if (pCur == parent->_pRight)
			{
				parent->_pRight = pCur->_pRight;
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
				pRoot = pCur->_pLeft;
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
		return _Remove(GetRoot(),key);
	}

	void InOrder()//中序
	{
		cout << "InOrder" << endl;
		_InOrder(GetRoot());
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
	Node* ToList()
	{
		Node* pRoot = GetRoot();
		if (NULL == pRoot)
			return NULL;
		//找到最左边的结点
		while (pRoot->_pLeft)
			pRoot = pRoot->_pLeft;

		Node* prev = NULL;
		_ToList(GetRoot(),prev);
		return pRoot;
	}
private:
	void _ToList(Node* pRoot,Node*& prev)
	{
		if (NULL == pRoot)
			return ;
		_ToList(pRoot->_pLeft,prev);

		pRoot->_pLeft = prev;
		if (prev)
			prev->_pRight = pRoot;
		prev = pRoot;

		_ToList(pRoot->_pRight,prev);
	}
	Node*& GetRoot()
	{
		return _pRoot->_parent;
	}
	Node* GetMinNode()
	{
		Node* pCur = GetRoot();
		while (pCur->_pLeft)
			pCur = pCur->_pLeft;
		return pCur;
	}
	Node* GetMaxNode()
	{
		Node* pCur = GetRoot();
		while (pCur->_pRight)
			pCur = pCur->_pRight;
		return pCur;
	}
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
		else
		{
			if (Root->_key > key)
				_Insert(Root->_pLeft, key, value);
			else if (Root->_key < key)
				_Insert(Root->_pRight, key, value);
			else
				return false;
		}
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
void Test3()
{
	int a[] = { 5, 3/*,4*/, 1, 7, 8, 2, 6,9 };
	BinarySearchTree<int, int> b;
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
	{
		b.Insert_Nor(a[idx], a[idx]);
	}
	BSTNode<int,int>* pHead = b.ToList();
	BinarySearchTree<int, int>::Iterator it = b.Begin();
	BinarySearchTree<int, int>::Iterator it1 = b.End();

	while (it != b.End())
	{
		cout << *it << " ";
		++it;
	}

}
int main()
{
	//Test1();
	//Test2();
	Test3();
	return 0;
}