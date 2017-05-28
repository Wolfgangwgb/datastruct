#include"RBTree.hpp"



template <class K,class V>
class Map
{
	typedef RBTree<K, V> RBType;
public:
	typename typedef RBTreeIterator<K, V, K&, K*> Iterator;
	//typename �ñ�����֪��Iterator��һ�����Ͷ��ǳ�Ա����
	//����ָ��ģ������(����)�еķǶ�������(dependent names)�������������Ǳ�������
public:
	Map()
		:_t()
	{}

	Iterator Begin()
	{
		return _t.Begin();
	}
	Iterator End()
	{
		return _t.End();
	}
	Iterator ReBegin()
	{
		return _t.End();
	}
	Iterator ReEnd()
	{
		return _t.Begin();
	}
	size_t Size()
	{
		return _t.Size();
	}

	bool Insert(const K& key)
	{
		return _t.Insert(key);
	}

	Iterator Find(const K& key)
	{
		return _t.Find(key);
	}
protected:
	RBType _t;//�ײ����ݽṹ�Ǻ����
};

void Test1()
{
	Map<int, int> m;
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		m.Insert(a[idx]);
	Map<int, int>::Iterator it = m.Begin();
	Map<int, int>::Iterator it1 = m.End();
	while (it != it1)
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;
	it1--;
	cout << *it1 << endl;

	cout << m.Size() << endl;

	Map<int, int>::Iterator it2 = m.ReBegin();
	Map<int, int>::Iterator it3 = m.ReEnd();
	while (it2 != it3)
	{
		it2--;
		cout << *it2 << " ";
	}

}