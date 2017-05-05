#include<iostream>
using namespace std;
#include<queue>
#include<stack>


//构建一个结点
template<class T>
struct BinaryTreeNode
{
	BinaryTreeNode(const T& data)
	: _data(data)
	, _pLeft(NULL)
	, _pRight(NULL)
	{}

	T _data;
	BinaryTreeNode<T>* _pLeft;    // 左孩子
	BinaryTreeNode<T>* _pRight;   // 右孩子
};

template<class T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	BinaryTree()  
		: _pRoot(NULL)
	{}

	BinaryTree(const T array[], size_t size, const T& invalid)
	{
		// 创建树
		size_t index = 0;
		_CreateTree(_pRoot, array, size, index, invalid);
	}

	BinaryTree(const BinaryTree<T>& t)//拷贝构造
	{
		_pRoot = _CopyBinaryTree(t._pRoot);
	}
	//重载赋值运算符注意点：1.返回值(该类型的引用)2.参数(该类型的常量引用)3.是不是自己给自己赋值4.释放原来的空间
	BinaryTree<T>& operator=(const BinaryTree<T>& t)
	{
		if (this == &t) //判断是不是自己给自己赋值
			return *this;
		_DestroyTree(_pRoot);//将原来的清空
		_pRoot = _CopyBinaryTree(t._pRoot);//根拷贝构造一样
		return *this;
	}

	// 先序遍历：根左右
	void PreOrder()//递归
	{
		_PreOrder(_pRoot);//进行一层封装，安全
		cout << endl;
	}
	void prepoder()//非递归
	{
		cout << "PreOrder" << endl;
		_preorder(_pRoot);
		cout << endl;
	}

	// 中序遍历：左根右
	void InOrder()//递归
	{
		_InOrder(_pRoot);
		cout << endl;
	}
	void Inorder()//非递归
	{
		cout << "InOrder" << endl;
		_Inorder(_pRoot);
		cout << endl;
	}

	// 后续遍历：左右根
	void PostOrder()//递归
	{
		_PostOrder(_pRoot);
		cout << endl;
	}
	void Postorder()//非递归
	{
		_Postorder(_pRoot);
		cout << endl;
	}
	Node* GetParent(Node* x)//获取双亲
	{
		return _GetParent(_pRoot, x);
	}

	Node* Find(const T& value)//查找值为value的结点
	{
		return _Find(_pRoot, value);
	}

	Node* GetLeftChild(Node* pCur)//获取左孩子
	{
		return pCur->_pLeft;
	}

	Node* GetRightChild(Node* pCur)//获取右孩子
	{
		return pCur->_pRight;
	}

	size_t Height()//高度
	{
		return _Height(_pRoot);
	}

	size_t GetLeefNode()//叶子结点个数
	{
		return _GetLeefNode(_pRoot);
	}

	size_t GetKLevelNode(size_t k)//第k层结点个数
	{
		return _GetKLevelNode(_pRoot, k);
	}
	// 层序遍历
	void LevelOrder()
	{
		queue<Node*> q;//用一个先进先出的队列辅助
		q.push(_pRoot);//头结点入队列
		while (!q.empty())
		{
			_pRoot = q.front();//取队列中的第一个元素为当前头结点
			cout << _pRoot->_data << " ";

			if (_pRoot->_pLeft)//左孩子不为空入队列
				q.push(_pRoot->_pLeft);
			if (_pRoot->_pRight)//右孩子不为空入队列
				q.push(_pRoot->_pRight);
			q.pop();//访问玩他的所有结点将他出队列
		}
	}

	// 求二叉树的镜像：非递归
	void GetBinaryMirror_Nor()
	{
		queue<Node*> q;
		Node* pCur = _pRoot;
		q.push(pCur);
		while (!q.empty())
		{
			pCur = q.front();
			std::swap(pCur->_pLeft, pCur->_pRight);
			q.pop();
			if (pCur->_pLeft)
				q.push(pCur->_pLeft);
			if (pCur->_pRight)
				q.push(pCur->_pRight);
		}
	}

	// 求二叉树的镜像：递归版本
	void GetBinaryMirror()
	{
		return _GetBinaryMirror(_pRoot);
	}

	bool IsCompleteBinaryTree()
	{
		queue<Node*> q;
		bool flag = true;
		Node* pCur = _pRoot;
		q.push(pCur);
		while (!q.empty())
		{
			pCur = q.front();
			if ((flag == false && pCur->_pLeft != NULL) || (flag == false && pCur->_pRight != NULL))
				return false;
			if (pCur->_pLeft && pCur->_pRight)
			{
				q.push(pCur->_pLeft);
				q.push(pCur->_pRight);
			}
			else if (pCur->_pRight)
			{
				return false;
			}
			else if (pCur->_pLeft)
			{
				q.push(pCur->_pLeft);
				flag = false;
			}
			else 
			{
				flag = false;
			}
			q.pop();
		}
		return true;
	}

	Node* PublicAncestor(Node* x,Node* y)
	{
		return _PublicAncestor(_pRoot,x,y);
	}
	~BinaryTree()
	{
		_DestroyTree(_pRoot);
	}
private:
	//利用先序创建
	void _CreateTree(Node*& pRoot, const T array[], size_t size, size_t& index, const T& invalid)
	{
		if (index < size && array[index] != invalid)
		{
			pRoot = new Node(array[index]);//创建根结点
			_CreateTree(pRoot->_pLeft, array, size, ++index, invalid);//创建左子树
			_CreateTree(pRoot->_pRight, array, size, ++index, invalid);//创建右子树
		}
	}

	Node* _CopyBinaryTree(Node* pRoot)//先序拷贝
	{
		Node* NewNode = NULL;
		if (NULL != pRoot)
		{
			NewNode = new Node(pRoot->_data);
			NewNode->_pLeft =  _CopyBinaryTree(pRoot->_pLeft);
			NewNode->_pRight = _CopyBinaryTree(pRoot->_pRight);
		}
		return NewNode;
	}

	void _DestroyTree(Node* &pRoot)//后序销毁
	{
		if (NULL != pRoot)
		{
			_DestroyTree(pRoot->_pLeft);
			_DestroyTree(pRoot->_pRight);
			delete pRoot;
			pRoot = NULL;
		}
	}
	Node* _GetParent(Node* pRoot, Node* x)
	{
		if (NULL == pRoot || x == pRoot)
			return NULL;
		if (x == pRoot->_pLeft || x == pRoot->_pRight)
			return pRoot;
		Node* pCur = NULL;
		if (pCur = _GetParent(pRoot->_pLeft, x))
			return pCur;
		if (pCur = _GetParent(pRoot->_pRight, x))
			return pCur;
		return NULL;
	}
	Node* _Find(Node* pRoot, const T& value)
	{
		if (NULL == pRoot)
			return NULL;
		if (pRoot->_data == value)
			return pRoot;
		Node* pCur = NULL;
		if (pCur = _Find(pRoot->_pLeft, value))
			return pCur;
		if (pCur = _Find(pRoot->_pRight, value))
			return pCur;
		return NULL;
	}
	size_t _Height(Node* pRoot)
	{
		if (NULL == pRoot)
			return 0;
		if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight)
			return 1;
		size_t LeftHight = _Height(pRoot->_pLeft);
		size_t RightHight = _Height(pRoot->_pRight);
		return  LeftHight>RightHight ? LeftHight + 1 : RightHight+1;
	}

	//size_t GetLeefNode()//叶子结点个数
	size_t _GetLeefNode(Node* pRoot)
	{
		if (NULL == pRoot)
			return 0;
		if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
			return 1;
		size_t Leftcount = _GetLeefNode(pRoot->_pLeft);
		size_t Rightcount = _GetLeefNode(pRoot->_pRight);
		return Leftcount > Rightcount ? Leftcount + 1 : Rightcount + 1;
	}

	//第k层结点个数
	size_t _GetKLevelNode(Node* pRoot, size_t k)
	{
		if (NULL == pRoot || k<1 || k>_Height(pRoot))
			return 0;
		if (k == 1)
			return 1;
		return _GetKLevelNode(pRoot->_pLeft, k - 1) + _GetKLevelNode(pRoot->_pRight,k-1);
	}

	void _PreOrder(Node* pRoot)
	{
		if (NULL != pRoot)
		{
			cout << pRoot->_data << " ";//根
			_PreOrder(pRoot->_pLeft);//左
			_PreOrder(pRoot->_pRight);//右
		}
	}

	void _InOrder(Node* pRoot)
	{
		if (NULL != pRoot)
		{
			_InOrder(pRoot->_pLeft);//左
			cout << pRoot->_data << " ";//根
			_InOrder(pRoot->_pRight);//右
		}
	}

	void _PostOrder(Node* pRoot)
	{
		if (NULL != pRoot)
		{
			_PostOrder(pRoot->_pLeft);//左
			_PostOrder(pRoot->_pRight);//右
			cout << pRoot->_data << " ";//根
		}
	}

	void _preorder(Node* pRoot)
	{
		if (NULL == pRoot)
			return;
		stack<Node*> s;
		s.push(pRoot);//头结点入栈
		while (!s.empty())
		{
			Node* pCur = s.top();//栈顶取元素
			cout << pCur->_data << " ";//访问
			s.pop();//出栈

			if (pCur->_pRight)//不为空保存
				s.push(pCur->_pRight);//保存右孩子
			if (pCur->_pLeft)
				s.push(pCur->_pLeft);//保存左孩子
		}
	}

	void _Inorder(Node* pRoot)
	{
		if (NULL == pRoot)
			return;
		stack<Node*> s;
		Node* pCur = pRoot;
		while (pCur || !s.empty())//循环条件
		{
			//找到最左边的结点
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_pLeft;
			}
			//访问该结点
			pCur = s.top();
			cout << pCur->_data << " ";
			s.pop();//访问完出栈
			/*
			while (NULL == pCur->_pRight && !s.empty())//处理左单支
			{
				pCur = s.top();
				cout << pCur->_data << " ";
				s.pop();
			}*/
			//将他的右孩子作为根结点
			pCur = pCur->_pRight;
		}
	}
	////////////////////////////////////////
	////1.找到最左边的结点
	////2.栈顶取一个元素
	////3.判断当前元素的右孩子是否为空：如果为空，访问当前结点；如果不为空，
	//////判断当前结点的右孩子是不是上一个被访问的结点，如果是，访问该结点，
	//////如果不是就将右孩子作为当前结点
	void _Postorder(Node* pRoot)
	{
		if (NULL == pRoot)
			return;
		stack<Node*> s;
		Node* pCur = pRoot;
		Node* prev = NULL;
		while (pCur || !s.empty())
		{
			while (pCur)//找最左边结点
			{
				s.push(pCur);
				pCur = pCur->_pLeft;
			}

			pCur = s.top();//取栈顶元素

			if (pCur->_pRight == NULL || pCur->_pRight == prev)//当前结点的右孩子为空或者当前结点是上一个被访问的结点
			{
				cout << pCur->_data << " ";
				prev = pCur;
				s.pop();
				pCur = NULL;
			}
			else//右孩子不为空
			{
				pCur = pCur->_pRight;
			}

		}
	}
	//镜像递归
	void _GetBinaryMirror(Node* pRoot)
	{
		//利用前序
		/*if (NULL == pRoot)
			return;
		std::swap(pRoot->_pLeft,pRoot->_pRight);
		_GetBinaryMirror(pRoot->_pLeft);
		_GetBinaryMirror(pRoot->_pRight);*/
		//利用后序
		/*if (NULL == pRoot)
			return;
		_GetBinaryMirror(pRoot->_pLeft);
		_GetBinaryMirror(pRoot->_pRight);
		std::swap(pRoot->_pLeft, pRoot->_pRight);*/
		//中序变为左根左可以
		if (NULL == pRoot)
			return;
		_GetBinaryMirror(pRoot->_pLeft);
		std::swap(pRoot->_pLeft, pRoot->_pRight);
		_GetBinaryMirror(pRoot->_pLeft);
	}
	Node* _PublicAncestor(Node* pRoot, Node* x, Node* y)
	{
		if (NULL == pRoot)
			return NULL;
		if (pRoot == x || pRoot == y)
			return NULL;
		bool isx = false;
		bool isy = false;
		stack<Node*> s;
		Node* pCur = pRoot;
		//Node* root = pRoot;
		Node* pCurx = NULL;
		Node* pCury = NULL;
		while (pCur || !s.empty())
		{
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_pLeft;
			}
			pCur = s.top();
			if (pRoot == pCur)
			{
				if ((isx == true && isy == false) || (isx == false && isy == true))
					return pRoot;
				if (isx == true && isy == true)
				{
					if (_Height(pCurx) > _Height(pCury))
						return _GetParent(pRoot, pCurx);
					else
						return _GetParent(pRoot, pCury);
				}
			}
			if (pCur == x)
			{
				pCurx = pCur;
				isx = true;
			}
			if (pCur == y)
			{
				pCury = pCur;
				isy = true;
			}
			s.pop();//访问完出栈
			pCur = pCur->_pRight;
		}
		if (isx == true && isy == true)
		{
			if (_Height(pCurx) > _Height(pCury))
				return _GetParent(pRoot, pCurx);
			else
				return _GetParent(pRoot, pCury);
		}
	}
private:
	BinaryTreeNode<T>* _pRoot;
};



void Test()
{
	char* ptr = "124###35##6";
	BinaryTree<char> bt(ptr, strlen(ptr), '#');
	BinaryTree<char> bt1(bt);

	BinaryTree<char> bt2;
	bt2.operator=(bt);
	bt2.PreOrder();
	cout << endl;
	bt2.InOrder();
	cout << endl;
	bt2.PostOrder();
	cout << endl;
	bt2.LevelOrder();
	/*
	bt.PreOrder();
	//bt1.PreOrder();
	cout << endl;
	bt.InOrder();
	cout << endl;
	bt.PostOrder();
	cout << endl;
	bt.LevelOrder();*/
}

void Test1()
{
	char* ptr = "124###35##6";
	BinaryTree<char> bt(ptr, strlen(ptr), '#');
	//bt.PreOrder();
	//bt.prepoder();
	//bt.InOrder();
	//bt.Inorder();
	bt.PostOrder();
	bt.Postorder();
	//cout << bt.GetParent(bt.Find('1'))->_data<< endl;
	//cout << bt.GetLeftChild(bt.Find('1'))->_data << endl;
	//cout << bt.GetRightChild(bt.Find('1'))->_data << endl;
	//cout << bt.Height() << endl;
	//cout << bt.GetLeefNode() << endl;
	//cout << bt.GetKLevelNode(2) << endl;
	}

void Test2()
{
	char* ptr = "124###35##6";
	//char* ptr = "124##5##36###";
	//char* ptr = "124##5##3#6##";
	BinaryTree<char> bt(ptr, strlen(ptr), '#');
	//bt.GetBinaryMirror_Nor();
	//bt.GetBinaryMirror();
	//bt.PreOrder();
	//cout<<"是不是完全二叉树："<<bt.IsCompleteBinaryTree()<<endl;
	//cout << bt.PublicAncestor(bt.Find('2'), bt.Find('4'))->_data << endl;
}
int main()
{
	//Test();
	//Test1();
	Test2();
	return 0;
}