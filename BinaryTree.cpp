#include<iostream>
using namespace std;
#include<queue>
#include<stack>


//����һ�����
template<class T>
struct BinaryTreeNode
{
	BinaryTreeNode(const T& data)
	: _data(data)
	, _pLeft(NULL)
	, _pRight(NULL)
	{}

	T _data;
	BinaryTreeNode<T>* _pLeft;    // ����
	BinaryTreeNode<T>* _pRight;   // �Һ���
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
		// ������
		size_t index = 0;
		_CreateTree(_pRoot, array, size, index, invalid);
	}

	BinaryTree(const BinaryTree<T>& t)//��������
	{
		_pRoot = _CopyBinaryTree(t._pRoot);
	}
	//���ظ�ֵ�����ע��㣺1.����ֵ(�����͵�����)2.����(�����͵ĳ�������)3.�ǲ����Լ����Լ���ֵ4.�ͷ�ԭ���Ŀռ�
	BinaryTree<T>& operator=(const BinaryTree<T>& t)
	{
		if (this == &t) //�ж��ǲ����Լ����Լ���ֵ
			return *this;
		_DestroyTree(_pRoot);//��ԭ�������
		_pRoot = _CopyBinaryTree(t._pRoot);//����������һ��
		return *this;
	}

	// ���������������
	void PreOrder()//�ݹ�
	{
		_PreOrder(_pRoot);//����һ���װ����ȫ
		cout << endl;
	}
	void prepoder()//�ǵݹ�
	{
		cout << "PreOrder" << endl;
		_preorder(_pRoot);
		cout << endl;
	}

	// ��������������
	void InOrder()//�ݹ�
	{
		_InOrder(_pRoot);
		cout << endl;
	}
	void Inorder()//�ǵݹ�
	{
		cout << "InOrder" << endl;
		_Inorder(_pRoot);
		cout << endl;
	}

	// �������������Ҹ�
	void PostOrder()//�ݹ�
	{
		_PostOrder(_pRoot);
		cout << endl;
	}
	void Postorder()//�ǵݹ�
	{
		_Postorder(_pRoot);
		cout << endl;
	}
	Node* GetParent(Node* x)//��ȡ˫��
	{
		return _GetParent(_pRoot, x);
	}

	Node* Find(const T& value)//����ֵΪvalue�Ľ��
	{
		return _Find(_pRoot, value);
	}

	Node* GetLeftChild(Node* pCur)//��ȡ����
	{
		return pCur->_pLeft;
	}

	Node* GetRightChild(Node* pCur)//��ȡ�Һ���
	{
		return pCur->_pRight;
	}

	size_t Height()//�߶�
	{
		return _Height(_pRoot);
	}

	size_t GetLeefNode()//Ҷ�ӽ�����
	{
		return _GetLeefNode(_pRoot);
	}

	size_t GetKLevelNode(size_t k)//��k�������
	{
		return _GetKLevelNode(_pRoot, k);
	}
	// �������
	void LevelOrder()
	{
		queue<Node*> q;//��һ���Ƚ��ȳ��Ķ��и���
		q.push(_pRoot);//ͷ��������
		while (!q.empty())
		{
			_pRoot = q.front();//ȡ�����еĵ�һ��Ԫ��Ϊ��ǰͷ���
			cout << _pRoot->_data << " ";

			if (_pRoot->_pLeft)//���Ӳ�Ϊ�������
				q.push(_pRoot->_pLeft);
			if (_pRoot->_pRight)//�Һ��Ӳ�Ϊ�������
				q.push(_pRoot->_pRight);
			q.pop();//�������������н�㽫��������
		}
	}

	// ��������ľ��񣺷ǵݹ�
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

	// ��������ľ��񣺵ݹ�汾
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
	//�������򴴽�
	void _CreateTree(Node*& pRoot, const T array[], size_t size, size_t& index, const T& invalid)
	{
		if (index < size && array[index] != invalid)
		{
			pRoot = new Node(array[index]);//���������
			_CreateTree(pRoot->_pLeft, array, size, ++index, invalid);//����������
			_CreateTree(pRoot->_pRight, array, size, ++index, invalid);//����������
		}
	}

	Node* _CopyBinaryTree(Node* pRoot)//���򿽱�
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

	void _DestroyTree(Node* &pRoot)//��������
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

	//size_t GetLeefNode()//Ҷ�ӽ�����
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

	//��k�������
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
			cout << pRoot->_data << " ";//��
			_PreOrder(pRoot->_pLeft);//��
			_PreOrder(pRoot->_pRight);//��
		}
	}

	void _InOrder(Node* pRoot)
	{
		if (NULL != pRoot)
		{
			_InOrder(pRoot->_pLeft);//��
			cout << pRoot->_data << " ";//��
			_InOrder(pRoot->_pRight);//��
		}
	}

	void _PostOrder(Node* pRoot)
	{
		if (NULL != pRoot)
		{
			_PostOrder(pRoot->_pLeft);//��
			_PostOrder(pRoot->_pRight);//��
			cout << pRoot->_data << " ";//��
		}
	}

	void _preorder(Node* pRoot)
	{
		if (NULL == pRoot)
			return;
		stack<Node*> s;
		s.push(pRoot);//ͷ�����ջ
		while (!s.empty())
		{
			Node* pCur = s.top();//ջ��ȡԪ��
			cout << pCur->_data << " ";//����
			s.pop();//��ջ

			if (pCur->_pRight)//��Ϊ�ձ���
				s.push(pCur->_pRight);//�����Һ���
			if (pCur->_pLeft)
				s.push(pCur->_pLeft);//��������
		}
	}

	void _Inorder(Node* pRoot)
	{
		if (NULL == pRoot)
			return;
		stack<Node*> s;
		Node* pCur = pRoot;
		while (pCur || !s.empty())//ѭ������
		{
			//�ҵ�����ߵĽ��
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_pLeft;
			}
			//���ʸý��
			pCur = s.top();
			cout << pCur->_data << " ";
			s.pop();//�������ջ
			/*
			while (NULL == pCur->_pRight && !s.empty())//������֧
			{
				pCur = s.top();
				cout << pCur->_data << " ";
				s.pop();
			}*/
			//�������Һ�����Ϊ�����
			pCur = pCur->_pRight;
		}
	}
	////////////////////////////////////////
	////1.�ҵ�����ߵĽ��
	////2.ջ��ȡһ��Ԫ��
	////3.�жϵ�ǰԪ�ص��Һ����Ƿ�Ϊ�գ����Ϊ�գ����ʵ�ǰ��㣻�����Ϊ�գ�
	//////�жϵ�ǰ�����Һ����ǲ�����һ�������ʵĽ�㣬����ǣ����ʸý�㣬
	//////������Ǿͽ��Һ�����Ϊ��ǰ���
	void _Postorder(Node* pRoot)
	{
		if (NULL == pRoot)
			return;
		stack<Node*> s;
		Node* pCur = pRoot;
		Node* prev = NULL;
		while (pCur || !s.empty())
		{
			while (pCur)//������߽��
			{
				s.push(pCur);
				pCur = pCur->_pLeft;
			}

			pCur = s.top();//ȡջ��Ԫ��

			if (pCur->_pRight == NULL || pCur->_pRight == prev)//��ǰ�����Һ���Ϊ�ջ��ߵ�ǰ�������һ�������ʵĽ��
			{
				cout << pCur->_data << " ";
				prev = pCur;
				s.pop();
				pCur = NULL;
			}
			else//�Һ��Ӳ�Ϊ��
			{
				pCur = pCur->_pRight;
			}

		}
	}
	//����ݹ�
	void _GetBinaryMirror(Node* pRoot)
	{
		//����ǰ��
		/*if (NULL == pRoot)
			return;
		std::swap(pRoot->_pLeft,pRoot->_pRight);
		_GetBinaryMirror(pRoot->_pLeft);
		_GetBinaryMirror(pRoot->_pRight);*/
		//���ú���
		/*if (NULL == pRoot)
			return;
		_GetBinaryMirror(pRoot->_pLeft);
		_GetBinaryMirror(pRoot->_pRight);
		std::swap(pRoot->_pLeft, pRoot->_pRight);*/
		//�����Ϊ��������
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
			s.pop();//�������ջ
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
	//cout<<"�ǲ�����ȫ��������"<<bt.IsCompleteBinaryTree()<<endl;
	//cout << bt.PublicAncestor(bt.Find('2'), bt.Find('4'))->_data << endl;
}
int main()
{
	//Test();
	//Test1();
	Test2();
	return 0;
}