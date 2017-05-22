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
		//����
		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		//����Ϊ�գ�Ѱ�Ҳ����λ��
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
		//����λ���ҵ�
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
		//����ƽ������

		while (parent)
		{
			//1.�ж���˫�׵�����(--)�����Һ���(++)
			if (pCur == parent->_pLeft)//����ѭ������
				parent->_bf--;
			else
				parent->_bf++;
			//2.���parent��_bf����0���Ͳ������ϵ���
			if (parent->_bf == 0)
				return true;
			//3.���ϵ���
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				pCur = parent;
				parent = parent->_pParent;
			}
			else
			{
				if (parent->_bf == 2)
				{
					//1.����
					//2.���Һ���
					if (pCur->_bf == 1)
						_RotateL(parent);
					else
						_RotateRL(parent);
				}
				else
				{
					//1.�ҵ���
					//2.�������
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

	bool Remove(const K& key)//ɾ��
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
			//��������������������ĵ�һ���ڵ�
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
				//1.�ж���˫�׵�����(--)�����Һ���(++)
				if (pCur == parent->_pLeft)//����ѭ������
					parent->_bf--;
				else
					parent->_bf++;
				//2.���parent��_bf����0���Ͳ������ϵ���
				if (parent->_bf == 0)
					return true;
				//3.���ϵ���
				else if (parent->_bf == -1 || parent->_bf == 1)
				{
					pCur = parent;
					parent = parent->_pParent;
				}
				else
				{
					if (parent->_bf == 2)
					{
						//1.����
						//2.���Һ���
						if (pCur->_bf == 1)
							_RotateL(parent);
						else
							_RotateRL(parent);
					}
					else
					{
						//1.�ҵ���
						//2.�������
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
		if (pSubL)//�����ڵ��Ǹ����
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
		//����ƽ������
		_TurnBlance(_pRoot);
	}
	void _RotateR(Node* parent)
	{
		Node* pSubL = parent->_pLeft;
		Node* pSubR = pSubL->_pRight;

		parent->_pLeft = pSubR;
		if (pSubR)//���������ܲ�����
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
		//����ƽ������
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
	/////������
	bool _IsBlanceTree(Node* pRoot)
	{
		//����
		if (NULL == pRoot)
			return true;
		//�����������߶�
		size_t left = _Height(pRoot->_pLeft);
		size_t right = _Height(pRoot->_pRight);
		//�������߶�-�������߶Ȳ����ڵ�ǰ����ƽ�����ӻ��ߵ�ǰ����ƽ�����Ӿ���ֵ����1���Ͳ���AVL��
		if (right - left != pRoot->_bf || abs(pRoot->_bf) > 1)
			return false;
		//&&��Ϊ���Ϊ�棬��������������ƽ���������������AVL��
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

