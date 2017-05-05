#pragma once
template<class T>
struct ListNode
{
	ListNode(const T& data = T())
	: _prev(0)
	, _next(0)
	, _data(data)
	{}

	ListNode<T>* _prev;
	ListNode<T>* _next;
	T _data;
};


template<class T, class Ref, class Ptr>
class __ListIterator__
{
	typedef __ListIterator__<T, T&, T*> Iterator;
	typedef __ListIterator__<T, const T&, const T*> ConstIterator;
	typedef __ListIterator__<T, Ref, Ptr> Self;
	typedef ListNode<T>* LinkType;
	typedef Ref Reference;
	typedef Ptr Pointer;

public:
	__ListIterator__(LinkType x = 0)
		:_node(x)
	{}
	__ListIterator__(const Iterator& x)
		:_node(x._node)
	{}
	bool operator==(const Iterator& x)
	{
		return _node == x._node;
	}
	bool operator!=(const Iterator& x)
	{
		return _node != x._node;
	}
	Reference operator*()
	{
		return (*_node)._data;
	}
	Pointer operator->()
	{
		return &(operator*())
	}
	Self& operator++()
	{
		_node = (LinkType)(*_node)._next;
		return *this;
	}
	Self operator++(int)
	{
		Self tmp = *this;
		++*this;
		return tmp;
	}
	Self& operator--()
	{
		_node = (LinkType)(*_node)._prev;
		return *this;
	}
	Self operator--(int)
	{
		Self tmp = *this;
		--*this;
		return tmp;
	}
public:
	LinkType _node;
};


template<class T>
class List
{
public:
	typedef ListNode<T> Node;
	typedef T ValueType;
	typedef ValueType& Reference;
	typedef const ValueType& ConstReference;
	typedef ValueType* Pointer;
	typedef const ValueType* ConstPointer;
	typedef Node* LinkType;
	typedef size_t SizeType;
	typedef __ListIterator__<T, T&, T*> Iterator;
	typedef __ListIterator__<T, const T&, const T*> ConstIterator;

public:
	List()
	{
		EmptyInit();
	}
	List(SizeType n, const T& data)
	{
		EmptyInit();
		for (SizeType i = 0; i < n; ++i)
		{
			PushBack(data);
		}
	}
	List(const List<T>& l)
	{
		EmptyInit();
		Iterator it = l._node->_next;
		for (SizeType idx = 0; idx<l.Size(); ++idx)
		{
			PushBack(it._node->_data);
			it++;
		}
	}

	~List()
	{
		/*Iterator it = End();
		--it;
		while(it!=_node)
		{
		delete it._node;
		--it;
		}*/
		Clear();

		delete _node;
	}
	List<T>& operator= (const List<T>& l)
	{

	}
	////////////////////////////////////////////
	Iterator Begin()
	{
		return _node->_next;
	}
	ConstIterator Begin()const
	{
		return _node->_next;
	}
	Iterator End()
	{
		return _node->_prev;
	}
	ConstIterator End()const
	{
		return _node->_prev;
	}
	bool Empty()
	{
		return _node == _node->_next;
	}
	SizeType Size()const
	{
		Iterator it = _node->_next;
		SizeType count = 0;
		while (it != _node->_prev)
		{
			++count;
			it++;
		}
		return count;
	}
	SizeType MaxSize()const
	{
		return SizeType(-1);
	}
	Reference Front()
	{
		return *(Begin());
	}
	ConstReference Front()const
	{
		return *(Begin());
	}
	Reference Back()
	{
		return *(--End());
	}
	ConstReference Back()const
	{
		return *(--End());
	}
	Iterator Insert(Iterator pos, const T& x = T())//任意位置插入
	{
		LinkType temp = new Node(x);
		temp->_next = pos._node;
		temp->_prev = pos._node->_prev;
		pos._node->_prev->_next = temp;
		pos._node->_prev = temp;
		return temp;
	}
	void PushFront(const T& x)
	{
		Insert(Begin(), x);
	}

	void PushBack(const T& x)
	{
		Iterator it = End();
		Insert(it, x);
	}

	Iterator Erase(Iterator pos)//任意位置删除
	{
		Iterator pRemNode = pos._node->_next;
		pos._node->_prev->_next = pos._node->_next;
		pos._node->_next->_prev = pos._node->_prev;

		pos._node->_next = pos._node->_prev = NULL;
		delete pos._node;
		return pRemNode;
	}
	void PopFront()//头删
	{
		Erase(Begin());
	}
	void PopBack()//尾删
	{
		Erase(--End());
	}
	void ReSize(SizeType n, const T& data)//改变list的大小
	{
		if (Size() >= n)
		{
			SizeType size1 = Size() - n;
			while (size1--)
			{
				PopBack();
			}
		}
		else
		{
			SizeType size2 = n - Size();
			while (size2--)
			{
				PushBack(data);
			}
		}
	}
	void Assign(SizeType n, const T& data)//给list赋值
	{
		Iterator pTemp = _node->_next;
		if (Size() >= n)//n小于等于原来list的元素个数
		{
			for (size_t idx = 0; idx<n; ++idx)
			{
				pTemp._node->_data = data;
				++pTemp;
			}
			for (size_t idx = 0; idx<Size() - n; ++idx)
			{
				PopBack();
			}
		}
		else
		{
			pTemp = _node->_next;
			size_t size = Size();
			for (size_t idx = 0; idx<size; ++idx)
			{
				pTemp._node->_data = data;
				++pTemp;
			}
			for (size_t idx = 0; idx<n - size; ++idx)
			{
				PushBack(data);
			}
		}
	}
	void Clear()//删除所有元素
	{
		Iterator it = --End();
		while (it != _node)
		{
			it = Erase(it);
		}
		_node->_next = _node;
		_node->_prev = _node;
	}
private:
	void EmptyInit()
	{
		_node = new Node;
		_node->_data = 0;
		_node->_next = _node;
		_node->_prev = _node;
	}
protected:
	LinkType _node;
};


template<class T, class Con = List<T> >
class Queue
{
public:
	typedef size_t SizeType;
	typedef T ValueType;
public:
	Queue()
	{}
	void Push(const ValueType data)
	{
		_con.PushBack(data);
	}
	void Pop()
	{
		_con.PopFront();
	}
	bool Empty()
	{
		return _con.Empty();
	}
	SizeType Size()const
	{
		return _con.Size();
	}
	ValueType& Front()
	{
		return _con.Front();
	}
	const ValueType& Front()const
	{
		return _con.Front();
	}
	ValueType& Back()
	{
		return _con.Back();
	}
	const ValueType& Back()const
	{
		return _con.Back();
	}
private:
	Con _con;
};