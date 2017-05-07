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
	//��������������
	ThreadBinaryTree(const T array[], size_t size, const T& invalid)
	{
		size_t index = 0;
		_CreateTree(_pRoot, array, size, index, invalid);
	}
	//ǰ��������
	void PreThread()
	{
		Node* prev = NULL;
		_PreThread(_pRoot, prev);
	}
	//����������
	void InThread()
	{
		Node* prev = NULL;
		_InThread(_pRoot, prev);
	}
	//����������
	void PostThread()
	{
		Node* prev = NULL;
		_PostThread(_pRoot, prev);
	}
	//ǰ�����
	void PreOrder()
	{
		_PreOrder(_pRoot);
	}
	//�������
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	//�������
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
			//������������
			if (NULL == pRoot->_pLeft)
			{
				pRoot->_pLeft = prev;
				pRoot->leftthread = THREAD;
			}
			//ǰһ�����������������
			if (prev && NULL == prev->_pRight)
			{
				prev->_pRight = pRoot;
				prev->rightthread = THREAD;
			}
			prev = pRoot;//��ǰ���ʽ�㱣��
			if (LINK == pRoot->leftthread)//���������û����������ִ��
				_PreThread(pRoot->_pLeft, prev);
			if (LINK == pRoot->rightthread)//���������û����������ִ��
				_PreThread(pRoot->_pRight, prev);
		}
	}

	void _InThread(Node* pRoot, Node*& prev)
	{
		if (pRoot)
		{
			//������������
			_InThread(pRoot->_pLeft, prev);

			//��ǰ���������
			if (NULL == pRoot->_pLeft)//����������
			{
				pRoot->_pLeft = prev;
				pRoot->leftthread = THREAD;
			}
			//ǰһ������Һ���������
			if (prev && NULL == prev->_pRight)
			{
				prev->_pRight = pRoot;
				prev->rightthread = THREAD;
			}
			prev = pRoot;//��ǰ���ʽ�㱣��
			if (LINK == pRoot->rightthread)//���������û����������ִ��
				_InThread(pRoot->_pRight, prev);
		}
	}

	void _PostThread(Node* pRoot, Node*& prev)
	{
		if (pRoot)
		{
			//������������
			_PostThread(pRoot->_pLeft, prev);
			//������������
			if (LINK == pRoot->rightthread)//�ж���������û��������
				_PostThread(pRoot->_pRight, prev);

			//��ǰ���������
			if (NULL == pRoot->_pLeft)//����������
			{
				pRoot->_pLeft = prev;
				pRoot->leftthread = THREAD;
			}
			//ǰһ������Һ���������
			if (prev && NULL == prev->_pRight)
			{
				prev->_pRight = pRoot;
				prev->rightthread = THREAD;
			}
			prev = pRoot;//��ǰ���ʽ�㱣��
		}
	}
	//ǰ���㷨��
	//1.����
	//2.������߽�㣬����һ�������һ��
	//3.�ҵ���:
	//          1--���ҽ�����(��Ϊһ����������)   
	//          2--���ҽ�㲻����(ȡ��̣����ʺ��)
	void _PreOrder(Node* pRoot)
	{
		if (NULL == pRoot)//����
			return;

		Node* pCur = pRoot;
		while (pCur)
		{
			while (LINK == pCur->leftthread)//������߽��
			{
				cout << pCur->_data << " ";
				pCur = pCur->_pLeft;
			}//ִ�������һ�����û�з���
			cout << pCur->_data << " ";//��������߽��
			/////////////////////////////////////////
			pCur = pCur->_pRight;
			//���������д����Ż�Ϊ��һ��
			//////////////////////////////////////
			/*while (THREAD == pCur->rightthread)//������
			{
			pCur = pCur->_pRight;
			cout << pCur->_data << " ";
			}
			if (LINK == pCur->leftthread)//������
			pCur = pCur->_pLeft;
			else
			pCur = pCur->_pRight;//������*/
		}
	}
	//��������㷨��
	//1.�ҵ�����ߵĽ��
	//2.���ʸý��
	//3.�ж���������(1).�Һ��Ӵ��ڣ���������(2).�Һ��Ӳ�����(������)
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
	//�����㷨��
	//1.�ҵ�����߽��
	//2.�����������ڣ������Һ��(����������ܴ��������ĺ�̵�����㣬����û�з��ʸ����)
	//2(1).��������
	//3.����������:(1).�ҵ�֧��� (2).�ѵ�ǰ�����Ϊ��
	void _PostOrder(Node* pRoot)
	{
		Node* pCur = pRoot;
		Node* prev = NULL;
		while (pCur)
		{
			//1.������
			if (prev != pCur->_pLeft)//���ӿ����Ѿ����ʹ���
			{
				while (LINK == pCur->leftthread)
					pCur = pCur->_pLeft;
			}
			//2.�Һ��Ӳ����ڣ�������
			while (THREAD == pCur->rightthread)
			{
				cout << pCur->_data << " ";
				prev = pCur;
				pCur = pCur->_pRight;
			}
			//2.(1)��������û���Һ��ӵ����
			if (pCur == pRoot && pCur->_pRight == NULL)
			{
				cout << pCur->_data << "��";
				return;
			}
			//�ҵ�֧
			while (pCur && prev == pCur->_pRight)
			{
				cout << pCur->_data << " ";
				prev = pCur;
				pCur = pCur->_parent;
			}
			//�Ҵ���
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