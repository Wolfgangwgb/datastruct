#include<iostream>
#include<stack>
using namespace std;

#include<boost/scoped_ptr>

int main()
{

	stack<int> st;
	stack<int> st1;
	st.push(1);//��ջ
	st.push(2);
	st.push(3);
	st1.push(1);//��ջ
	st1.push(2);
	st1.push(3);
	st1.push(4);
	st.swap(st1);//��������ջ������
	
	st.emplace();
	st1.emplace();
	if (st == st1)//  == / != / < / > / >= / <=
	{
		cout << "�����������" << endl;
	}
	if (st < st1)
	{
		cout << "st1��" << endl;
	}
	cout << st.size() << endl;//ջ��Ԫ�صĸ���
	cout << st.top() << endl;//����ջ��Ԫ��
	st.pop();//��ջ

	st.top() = 12;//�ı�ջ��Ԫ��ֵ
	st.push(11);
	st.push(22);
	cout << st.top() << endl;
	while (!st.empty())//�ж�ջ�Ƿ�Ϊ�գ���-->true,�ǿ�-->false
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
// �ػ���ȫ�������, �ṩoperator==��<�����򹹽������������
// �����ϸ�ڼ�<stl_pair.h>�е�˵��
	friend bool operator== (const stack&, const stack&);
	friend bool operator< (const stack&, const stack&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Container c;   // ���������ʵ��ά��������
public:
	// �ж�stack�Ƿ�Ϊ��
	bool empty() const { return c.empty(); }
							
	// stack��Ԫ�ظ���
	size_type size() const { return c.size(); }
							
	// ����ջ��Ԫ��, ע�����ﷵ�ص�������!!!
	reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
							
	// ��ջ��׷����Ԫ��
	void push(const value_type& x) { c.push_back(x); }
							
	// �Ƴ�ջ��Ԫ��, ע�ⲻ����Ԫ�ص�����,
	// �ܶ��ѧ������ô�����ʱ��������Ϊpop()����ͬʱ�᷵��ջ��Ԫ�ص�����
	void pop() { c.pop_back(); }
};

 // �ж�����stack�Ƿ����, ��Ҫ�������ڲ�ά�������Ƿ����
 // x.c == y.c������������ص�operator ==
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
