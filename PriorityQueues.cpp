#pragma once
#include<vector>
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

//template <class T,class Compare = Less<T>>//��ͨ��ģ�����
template <class T, template<class> class Compare>//ģ���ģ�����
class Heap
{
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
		if (!Empty())
		{
			std::swap(_heap[0], _heap[_heap.size() - 1]);//�����Ѷ�������һ��Ԫ��
			_heap.pop_back();
			Adjustdown(0);//�������ϵ���
		}
	}
	const T& Top()const
	{
		return _heap[0];
	}

protected:
	void Adjustdown(int root)//�������ϵ���
	{
		//Compare com;
		Compare<T> com;
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
		//Compare com;
		Compare<T> com;
		int child = root;
		int parent = (child - 1) >> 1;
		while (child != 0)
		{
			//if (_heap[child] < _heap[parent])
			if (com(child, parent))
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
	std::vector<T> _heap;
};



template <class T, template <class> class Compare>
class PriorityQueues
{
public:
	PriorityQueues()
	{}

	bool Empty()
	{
		return _hp.Empty();
	}

	size_t Size()
	{
		return _hp.Size();
	}
	void Push(const T& data)
	{
		_hp.Insert(data);
	}
	void Pop()
	{
		_hp.Remove();
	}
	const T& Top()const
	{
		return _hp.Top();
	}

private:
	Heap<T,Compare> _hp;
};