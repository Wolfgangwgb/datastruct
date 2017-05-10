
#pragma once
#include<vector>
#include<assert.h>
#include<iostream>
using namespace std;

//仿函数
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
//template <class T, template<class> class Compare>//模板的模板参数
template <class T,class Compare = Less<T>>//普通的模板参数
class Heap
{
	typedef vector<T> Vector;
public:
	Heap()
	{}
	Heap(const T array[], size_t size)//创建小堆
	{
		_heap.resize(size);//开辟空间(一次)
		size_t idx = 0;
		for (; idx < size; idx++)
		{
			_heap[idx] = array[idx];
		}
		//找倒数第一个非叶子结点
		int root = (_heap.size() - 2) >> 1;
		for (; root >= 0; root--)
		{
			Adjustdown(root);//从下向上调整
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
		_heap.push_back(data);//插入元素
		if (_heap.size() == 1)
			return;
		int root = _heap.size() - 1;
		Adjustup(root);
	}
	void Remove()
	{
		assert(Empty());
		std::swap(_heap[0], _heap[_heap.size() - 1]);//交换堆顶与堆最后一个元素
		_heap.pop_back();
		Adjustdown(0);//从下向上调整
		
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
	void Adjustdown(int root)//从下向上调整
	{
		Compare com;
		//Compare<T> com;
		size_t child = 2 * root + 1;//左孩子
		size_t parent = root;//双亲
		while (child < _heap.size())
		{
			//右孩子存在的情况下，找左右孩子值最小的
			//if (child + 1 < _heap.size() && _heap[child] > _heap[child + 1])
			if (child + 1 < _heap.size() && com(_heap[child + 1], _heap[child]))//
				child += 1;
			//判断值最小的孩子与双亲结点值得大小
			//if (_heap[child] < _heap[parent])
			if (com(_heap[child], _heap[parent]))
			{
				std::swap(_heap[child], _heap[parent]);
				parent = child;//重新设置双亲
				child = 2 * parent + 1;//重新计算孩子
			}
			else
				return;//不用调整
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
		//堆里保存的是指向结点的指针
		Heap<Node*, HuffLess<Node>> hp;//创建一个空堆
		for (size_t idx = 0; idx < size; ++idx)//按权值创建小堆
		{
			Node* node = new Node(array[idx]);
			hp.Insert(node);
		}//小堆的元素为霍夫曼树的结点
		while (hp.Size() != 1)
		{
			Node* pleft = hp.Top();//取堆顶元素作为左子树
			hp.Remove();//删除
			Node* pright = hp.Top();//取堆顶元素作为右子树
			hp.Remove();
			Node* parent = new Node(pleft->_weight + pright->_weight);//左子树右子树的权值之和创造他们的双亲结点
			//parent指向他的左右孩子
			parent->_pLeft = pleft;
			parent->_pRight = pright;
			hp.Insert(parent);//新创建的树插入堆
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
