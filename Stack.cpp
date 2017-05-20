#include<iostream>
#include<stack>
using namespace std;

#include<boost/scoped_ptr>

int main()
{

	stack<int> st;
	stack<int> st1;
	st.push(1);//入栈
	st.push(2);
	st.push(3);
	st1.push(1);//入栈
	st1.push(2);
	st1.push(3);
	st1.push(4);
	st.swap(st1);//交换两个栈的数据
	
	st.emplace();
	st1.emplace();
	if (st == st1)//  == / != / < / > / >= / <=
	{
		cout << "两个容器相等" << endl;
	}
	if (st < st1)
	{
		cout << "st1大" << endl;
	}
	cout << st.size() << endl;//栈中元素的个数
	cout << st.top() << endl;//返回栈顶元素
	st.pop();//出栈

	st.top() = 12;//改变栈顶元素值
	st.push(11);
	st.push(22);
	cout << st.top() << endl;
	while (!st.empty())//判断栈是否为空，空-->true,非空-->false
	{
		cout << st.top() << endl;
		st.pop();
	}
	return 0;
}



/*
template <class T, class Container = deque<T> >
class stack
{
// 特化的全局运算符, 提供operator==和<重载则构建出所有运算符
// 其具体细节见<stl_pair.h>中的说明
	friend bool operator== (const stack&, const stack&);
	friend bool operator< (const stack&, const stack&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Container c;   // 这个是我们实际维护的容器
public:
	// 判断stack是否为空
	bool empty() const { return c.empty(); }
							
	// stack中元素个数
	size_type size() const { return c.size(); }
							
	// 返回栈顶元素, 注意这里返回的是引用!!!
	reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
							
	// 在栈顶追加新元素
	void push(const value_type& x) { c.push_back(x); }
							
	// 移除栈顶元素, 注意不返回元素的引用,
	// 很多初学者随机用此容器时经常误认为pop()操作同时会返回栈顶元素的引用
	void pop() { c.pop_back(); }
};

 // 判断两个stack是否相等, 就要测试其内部维护容器是否相等
 // x.c == y.c会调用容器重载的operator ==
 template <class T, class Sequence>
 bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
 {
   return x.c == y.c;
 }

 template <class T, class Sequence>
 bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
 {
   return x.c < y.c;
 }
 */
