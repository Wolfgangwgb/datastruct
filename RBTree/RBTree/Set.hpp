#include"RBTree.hpp"


template <class K>
class Set
{
	typedef RBTree<K, K> RBtype;
public:
	typename typedef RBTreeIterator<K, K, K&, K*> Iterator;
public:
	Set()
		:_s()
	{}
	Iterator Begin()
	{
		return _s.Begin();
	}
	Iterator End()
	{
		return _s.End();
	}
	Iterator ReBegin()
	{
		return _s.End();
	}

	Iterator ReEnd()
	{
		return _s.Begin();
	}

	size_t Size()
	{
		return _s.Size();
	}

	Iterator Find(const K& key)
	{
		return _s.Find(key);
	}

	bool Empty()const
	{
		return _s.Empty();
	}
	bool Insert_unique(const K& key)
	{
		return _s.Insert(key);
	}

protected:
	RBtype _s;
};

void Test2()
{
	Set<int> s;
	s.Insert_unique(1);
	s.Insert_unique(2);
	s.Insert_unique(3);
	s.Insert_unique(4);
	s.Insert_unique(5);
	s.Insert_unique(6);

	Set<int>::Iterator it = s.Begin();
	Set<int>::Iterator it1 = s.End();

	while (it != it1)
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	it1--;
	cout << *it1 << endl;
	if (s.Empty())
		cout << "Пе" << endl;
	else
		cout << "Тњ" << endl;

	Set<int>::Iterator it2 = s.ReBegin();
	Set<int>::Iterator it3 = s.ReEnd();
	it2--;
	cout << *(it2) << endl;
	cout << *(it3) << endl;
}