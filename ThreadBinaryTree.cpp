#include<iostream>
using namespace std;

enum Threadflag
{
	LINK, THREAD
};

template <class T>
struct ThreadBinaryTreeNode
{
	ThreadBinaryTreeNode(const T& data)
	:_data(data)
	, _pLeft(NULL)
	, _pRight(NULL)
	, _parent(NULL)
	, leftthread(LINK)
	, rightthread(LINK)
	{}
	T _data;
	ThreadBinaryTreeNode<T>* _pLeft;
	ThreadBinaryTreeNode<T>* _pRight;
	ThreadBinaryTreeNode<T>* _parent;
	Threadflag leftthread;
	Threadflag rightthread;
};

template <class T>
class ThreadBinaryTree
{
	typedef ThreadBinaryTreeNode<T> Node;
public:
	ThreadBinaryTree()
		:_pRoot(NULL)
	{}
	//创建线索二叉树
	ThreadBinaryTree(const T array[], size_t size, const T& invalid)
	{
		size_t index = 0;
		_CreateTree(_pRoot, array, size, index, invalid);
	}
	//前序线索化
	void PreThread()
	{
		Node* prev = NULL;
		_PreThread(_pRoot, prev);
	}
	//中序线索化
	void InThread()
	{
		Node* prev = NULL;
		_InThread(_pRoot, prev);
	}
	//后序线索化
	void PostThread()
	{
		Node* prev = NULL;
		_PostThread(_pRoot, prev);
	}
	//前序遍历
	void PreOrder()
	{
		_PreOrder(_pRoot);
	}
	//中序遍历
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	//后序遍历
	void PostOrder()
	{
		_PostOrder(_pRoot);
	}

	~ThreadBinaryTree()
	{
		if (_pRoot)
		{
			delete _pRoot;
			_pRoot = NULL;
		}
	}
private:
	void _CreateTree(Node*& pRoot, const T array[], size_t size, size_t& index, const T& invalid)
	{
		if (index < size && array[index] != invalid)
		{
			pRoot = new Node(array[index]);
			_CreateTree(pRoot->_pLeft, array, size, ++index, invalid);
			if (pRoot->_pLeft)
				pRoot->_pLeft->_parent = pRoot;
			_CreateTree(pRoot->_pRight, array, size, ++index, invalid);
			if (pRoot->_pRight)
				pRoot->_pRight->_parent = pRoot;
		}
	}

	void _PreThread(Node* pRoot, Node*& prev)
	{
		if (NULL == pRoot)
			return;
		if (pRoot)
		{
			//左子树线索化
			if (NULL == pRoot->_pLeft)
			{
				pRoot->_pLeft = prev;
				pRoot->leftthread = THREAD;
			}
			//前一个结点右子树线索化
			if (prev && NULL == prev->_pRight)
			{
				prev->_pRight = pRoot;
				prev->rightthread = THREAD;
			}
			prev = pRoot;//当前访问结点保存
			if (LINK == pRoot->leftthread)//如果左子树没有线索化则执行
				_PreThread(pRoot->_pLeft, prev);
			if (LINK == pRoot->rightthread)//如果右子树没有线索化则执行
				_PreThread(pRoot->_pRight, prev);
		}
	}

	void _InThread(Node* pRoot, Node*& prev)
	{
		if (pRoot)
		{
			//左子树线索化
			_InThread(pRoot->_pLeft, prev);

			//当前结点线索化
			if (NULL == pRoot->_pLeft)//左孩子线索化
			{
				pRoot->_pLeft = prev;
				pRoot->leftthread = THREAD;
			}
			//前一个结点右孩子线索化
			if (prev && NULL == prev->_pRight)
			{
				prev->_pRight = pRoot;
				prev->rightthread = THREAD;
			}
			prev = pRoot;//当前访问结点保存
			if (LINK == pRoot->rightthread)//如果右子树没有线索化则执行
				_InThread(pRoot->_pRight, prev);
		}
	}

	void _PostThread(Node* pRoot, Node*& prev)
	{
		if (pRoot)
		{
			//左子树线索化
			_PostThread(pRoot->_pLeft, prev);
			//右子树线索化
			if (LINK == pRoot->rightthread)//判断右子树有没有线索化
				_PostThread(pRoot->_pRight, prev);

			//当前结点线索化
			if (NULL == pRoot->_pLeft)//左孩子线索化
			{
				pRoot->_pLeft = prev;
				pRoot->leftthread = THREAD;
			}
			//前一个结点右孩子线索化
			if (prev && NULL == prev->_pRight)
			{
				prev->_pRight = pRoot;
				prev->rightthread = THREAD;
			}
			prev = pRoot;//当前访问结点保存
		}
	}
	//前序算法：
	//1.树空
	//2.找最左边结点，经过一个点访问一个
	//3.找到后:
	//          1--》右结点存在(作为一个新树处理)   
	//          2--》右结点不存在(取后继，访问后继)
	void _PreOrder(Node* pRoot)
	{
		if (NULL == pRoot)//树空
			return;

		Node* pCur = pRoot;
		while (pCur)
		{
			while (LINK == pCur->leftthread)//找最左边结点
			{
				cout << pCur->_data << " ";
				pCur = pCur->_pLeft;
			}//执行完最后一个结点没有访问
			cout << pCur->_data << " ";//访问最左边结点
			/////////////////////////////////////////
			pCur = pCur->_pRight;
			//将下面所有代码优化为这一句
			//////////////////////////////////////
			/*while (THREAD == pCur->rightthread)//处理后继
			{
			pCur = pCur->_pRight;
			cout << pCur->_data << " ";
			}
			if (LINK == pCur->leftthread)//左新树
			pCur = pCur->_pLeft;
			else
			pCur = pCur->_pRight;//右新树*/
		}
	}
	//中序遍历算法：
	//1.找到最左边的结点
	//2.访问该结点
	//3.判断右子树：(1).右孩子存在（新树处理）(2).右孩子不存在(处理后继)
	void _InOrder(Node* pRoot)
	{
		Node* pCur = pRoot;
		while (pCur)
		{    //1
			while (LINK == pCur->leftthread)
				pCur = pCur->_pLeft;
			//2.
			cout << pCur->_data << " ";
			//3.(2)
			while (THREAD == pCur->rightthread)
			{
				pCur = pCur->_pRight;
				cout << pCur->_data << " ";
			}
			//3.(1)
			pCur = pCur->_pRight;
		}
	}
	//后序算法：
	//1.找到最左边结点
	//2.右子树不存在，处理右后继(这种情况可能处理连续的后继到根结点，但是没有访问根结点)
	//2(1).处理根结点
	//3.右子树存在:(1).右单支情况 (2).把当前结点置为右
	void _PostOrder(Node* pRoot)
	{
		Node* pCur = pRoot;
		Node* prev = NULL;
		while (pCur)
		{
			//1.找左孩子
			if (prev != pCur->_pLeft)//左孩子可能已经访问过了
			{
				while (LINK == pCur->leftthread)
					pCur = pCur->_pLeft;
			}
			//2.右孩子不存在，处理后继
			while (THREAD == pCur->rightthread)
			{
				cout << pCur->_data << " ";
				prev = pCur;
				pCur = pCur->_pRight;
			}
			//2.(1)处理根结点没有右孩子的情况
			if (pCur == pRoot && pCur->_pRight == NULL)
			{
				cout << pCur->_data << "　";
				return;
			}
			//右单支
			while (pCur && prev == pCur->_pRight)
			{
				cout << pCur->_data << " ";
				prev = pCur;
				pCur = pCur->_parent;
			}
			//右存在
			if (pCur && pCur->rightthread == LINK)
				pCur = pCur->_pRight;
		}
	}
private:
	Node* _pRoot;
};
void Test()
{
	char* str1 = "124###35##6";
	char str[] = { '1', '2', '#', '3', '#', '#', '4', '5', '#', '6', '#', '7', '#', '#', '8' };
	//ThreadBinaryTree<char> b(str1, strlen(str1), '#');
	ThreadBinaryTree<char> b(str, sizeof(str) / sizeof(str[0]), '#');
	//b.PreThread();
	//b.PreOrder();
	//b.InThread();
	//b.InOrder();
	b.PostThread();
	b.PostOrder();
}