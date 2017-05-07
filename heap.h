#pragma once
#include<vector>
#include<iostream>
using namespace std;
template <class T>
class Heap
{
public:
	Heap()
	{}
	Heap(const T array[],size_t size)//����С��
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
		_heap.push_back(data);
		int root = _heap.size()-1;
		Adjustup(root);
	}
	void Remove()
	{
		std::swap(_heap[0],_heap[_heap.size()-1]);
		_heap.pop_back();
		int root = (_heap.size() - 2) >> 1;
		for (; root >= 0; root--)
		{
			Adjustdown(root);//�������ϵ���
		}
	}

protected:
	void Adjustdown(int root)//�������ϵ���
	{
		size_t child = 2 * root + 1;//����
		size_t parent = root;//˫��
		while (child < _heap.size())
		{
			//�Һ��Ӵ��ڵ�����£������Һ���ֵ��С��
			if (child + 1 < _heap.size() && _heap[child] > _heap[child + 1])
				child += 1;
			//�ж�ֵ��С�ĺ�����˫�׽��ֵ�ô�С
			if (_heap[child] < _heap[parent])
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
		int child = root;
		int parent = (child - 1)/2;
		while (parent>=0 && _heap[child] < _heap[parent])
		{
				std::swap(_heap[child], _heap[parent]);
				child = parent;
				parent = (child - 1) / 2;
		}
	}
protected:
	std::vector<T> _heap;
};




void Test()
{
	int array[] = {53,17,78,9,45,65,87,23};
	Heap<int> h(array,sizeof(array)/sizeof(array[0]));
	//h.Remove();
	h.Insert(12);
	h.Insert(1);
	h.Insert(0);
	h.Insert(32);
	h.Remove();
	cout << h.Size() << endl;
}