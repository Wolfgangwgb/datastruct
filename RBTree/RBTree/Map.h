#include"RBTree.hpp"



template <class K,class V>
class Map
{
public:
	Map()
		:_RB(NULL)
	{}
	
protected:
	RBTree<K, V>* _RB;
};