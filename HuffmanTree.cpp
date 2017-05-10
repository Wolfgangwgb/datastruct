
#pragma once
#include<vector>
#include<assert.h>
#include<iostream>
using namespace std;

//�º���
template <class T>
struct Less
{
	bool operator()(const T&  left, const T&  right)
	{
		return left < right;
	}
};

template <class T>
struct Greater
{
	bool operator()(const T& left, const T& right)
	{
		return left > right;
	}
};

template <class T>
struct HuffLess
{
	bool operator()(const T* left, const T* right)
	{
		return left->_weight < right->_weight;
	}
};
//template <class T, template<class> class Compare>//ģ���ģ�����
template <class T,class Compare = Less<T>>//��ͨ��ģ�����
class Heap
{
	typedef vector<T> Vector;
public:
	Heap()
	{}
	Heap(const T array[], size_t size)//����С��
	{
		_heap.resize(size);//���ٿռ�(һ��)
		size_t idx = 0;
		for (; idx < size; idx++)
		{
			_heap[idx] = array[idx];
		}
		//�ҵ�����һ����Ҷ�ӽ��
		int root = (_heap.size() - 2) >> 1;
		for (; root >= 0; root--)
		{
			Adjustdown(root);//�������ϵ���
		}
	}
	size_t Size()const
	{
		return _heap.size();
	}
	bool Empty()const
	{
		return _heap.empty();
	}
	void Insert(const T& data)
	{
		_heap.push_back(data);//����Ԫ��
		if (_heap.size() == 1)
			return;
		int root = _heap.size() - 1;
		Adjustup(root);
	}
	void Remove()
	{
		assert(Empty());
		std::swap(_heap[0], _heap[_heap.size() - 1]);//�����Ѷ�������һ��Ԫ��
		_heap.pop_back();
		Adjustdown(0);//�������ϵ���
		
	}

	const T& Top()
	{
		return _heap[0];
	}
	/*
	void HeapSort()
	{
		while (!Empty())
		{
			cout << _heap[0] << " ";
			//swap(_heap[0],_heap[_heap.size()-1]);
			Remove();
		}
	}*/
private:
	void Adjustdown(int root)//�������ϵ���
	{
		Compare com;
		//Compare<T> com;
		size_t child = 2 * root + 1;//����
		size_t parent = root;//˫��
		while (child < _heap.size())
		{
			//�Һ��Ӵ��ڵ�����£������Һ���ֵ��С��
			//if (child + 1 < _heap.size() && _heap[child] > _heap[child + 1])
			if (child + 1 < _heap.size() && com(_heap[child + 1], _heap[child]))//
				child += 1;
			//�ж�ֵ��С�ĺ�����˫�׽��ֵ�ô�С
			//if (_heap[child] < _heap[parent])
			if (com(_heap[child], _heap[parent]))
			{
				std::swap(_heap[child], _heap[parent]);
				parent = child;//��������˫��
				child = 2 * parent + 1;//���¼��㺢��
			}
			else
				return;//���õ���
		}
	}

	void Adjustup(int root)
	{
		Compare com;
		//Compare<T> com;
		int child = root;
		int parent = (child - 1) >> 1;
		while (child != 0)
		{
			//if (_heap[child] < _heap[parent])
			if (com(_heap[child], _heap[parent]))
			{
				std::swap(_heap[child], _heap[parent]);
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
				return;
		}
	}

protected:
	Vector _heap;
};


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const T& weight,const T& data = T())
	: _weight(weight)
	, _pLeft(NULL)
	, _pRight(NULL)
	{}
	T _data;
	T _weight;
	HuffmanTreeNode<T>* _pLeft;
	HuffmanTreeNode<T>* _pRight;
};

template <class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
	:_pRoot(NULL)
	{}
	HuffmanTree(const T array[],size_t size)
	{
		_Create(array,size);
	}
protected:
	void _Create(const T array[], size_t size)
	{
		//���ﱣ�����ָ�����ָ��
		Heap<Node*, HuffLess<Node>> hp;//����һ���ն�
		for (size_t idx = 0; idx < size; ++idx)//��Ȩֵ����С��
		{
			Node* node = new Node(array[idx]);
			hp.Insert(node);
		}//С�ѵ�Ԫ��Ϊ���������Ľ��
		while (hp.Size() != 1)
		{
			Node* pleft = hp.Top();//ȡ�Ѷ�Ԫ����Ϊ������
			hp.Remove();//ɾ��
			Node* pright = hp.Top();//ȡ�Ѷ�Ԫ����Ϊ������
			hp.Remove();
			Node* parent = new Node(pleft->_weight + pright->_weight);//��������������Ȩֵ֮�ʹ������ǵ�˫�׽��
			//parentָ���������Һ���
			parent->_pLeft = pleft;
			parent->_pRight = pright;
			hp.Insert(parent);//�´������������
		}
		_pRoot = hp.Top();
	}
private:
	Node* _pRoot;
};

void Test()
{
	int array[] = {3,1,7,5};
	HuffmanTree<int> h(array, sizeof(array) / sizeof(array[0]));
}

int main()
{
	Test();
	return 0;
}
