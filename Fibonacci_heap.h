#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include"List.h"
#include<vector>
#include<memory>

template<typename T> struct Fibonacci_heap_node
{
	std::shared_ptr<Fibonacci_heap_node<T>> parent; //parent��child������Ϊ��
	std::shared_ptr<Fibonacci_heap_node<T>> child;
	std::shared_ptr<Fibonacci_heap_node<T>> left; //left��right��㲻��Ϊ�գ����û�������ֵܾ�ָ�����Լ�
	std::shared_ptr<Fibonacci_heap_node<T>> right;
	int degree;
	T key;
	bool mark; //ȷ���Ƿ�ִ��cascade_cut
	bool inside; //����ȷ���ý���Ƿ��ڶ�����
	std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>> root_ptr; //���ĳ������ڸ������ڣ���������Ϊָ������
	Fibonacci_heap_node<T>(std::shared_ptr<Fibonacci_heap_node<T>> p, std::shared_ptr<Fibonacci_heap_node<T>> l, std::shared_ptr<Fibonacci_heap_node<T>> r, std::shared_ptr<Fibonacci_heap_node<T>> c, int d, T k, bool stat, bool in, std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>> ptr = NULL) : parent(p), left(l), right(r), child(c), degree(d), key(k), mark(stat), inside(in), root_ptr(ptr) {};
};

template<typename T> class Fibonacci_heap
{
	friend class List<T>;
private:
	List<std::shared_ptr<Fibonacci_heap_node<T>>> root; //����ֱ����root��head��㱣����Сֵ
	int size;
public:
	Fibonacci_heap<T>() : root(circular_link), size(0) {};
	int get_size()
	{
		return size;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> insert(T elem) //��Fibonacci���������Ԫ��
	{
		std::shared_ptr<Fibonacci_heap_node<T>> temp = std::shared_ptr<Fibonacci_heap_node<T>>(new Fibonacci_heap_node<T>(NULL, NULL, NULL, NULL, 0, elem, false, true)); //�½�һ�����
		temp->left = temp;
		temp->right = temp;
		root.insert_end(temp);
		temp->root_ptr = root.get_head()->prev;
		if (root.get_head()->element->key > elem) //������Ҫ�ƶ�headָ��
		{
			root.change_head(root.get_head()->prev);
		}
		size++;
		return temp;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> insert_node(std::shared_ptr<Fibonacci_heap_node<T>> node)
	{
		root.insert_end(node);
		node->root_ptr = root.get_head()->prev;
		if (root.get_head()->element->key > node->key)
		{
			root.change_head(root.get_head()->prev);
		}
		node->left = node; //�ý�������ָ�붼ָ���Լ�
		node->right = node;
		node->parent = NULL;
		size++;
		node->inside = true;
		return node;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> minimum()
	{
		return root.get_head()->element;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> extract_min()
	{
		auto temp = root.get_head(); //����ȡ��Ԫ�ء�ע����ܴ�ʱֻ����һ��Ԫ�أ����ɾ��������Ĳ���Ҫ�������
		size--;
		if (size >= 1)
		{
			auto temp_child = temp->element->child;
			if (temp->element->child)
			{
				auto sig = temp_child; //�����������ж�ѭ�������ı�־
				while (true) //��temp_child�����ֵ�ȫ��������б��У���ͬʱ����parentָ����Ϊ�ա������ֵ�֮���ָ����ѭ���ģ����ֻ��ָ��ѭ��һ�鼴��
				{
					root.insert_end(temp_child);
					temp_child->parent = NULL;
					temp_child->root_ptr = root.get_head()->prev;
					auto i = temp_child;
					temp_child = temp_child->right; //�������Ĳ�����ı�ָ��ָ�������Ҫ��������һλ
					i->left = i; //��������Ľ������Һ��Ӹ�Ϊ��ѭ���ģ�����������ĺϲ�����
					i->right = i;
					if (temp_child == sig)
					{
						break;
					}
				}
			}
			root.change_head(temp->next);
			root.delete_node(temp); //��������ɾȥ��С��㣬������󷵻���
			consolidate(); //�Խ�����ϲ�
			temp->element->root_ptr = NULL; //ע�ⷵ��֮ǰ�ǵ��޸���СԪ�ؽ��ĺ���ָ��ͽ��Ķ�
			temp->element->child = NULL; 
			temp->element->degree = 0;
			temp->element->inside = false;
			return temp->element;
		}
		else
		{
			temp->element->root_ptr = NULL;
			temp->element->inside = false;
			return temp->element;
		}
	}
	void consolidate() //�Զ���ͬ�Ľ�����ϲ���ֱ����������û�ж���ͬ�Ľ�㡣����֮ǰ����������б��ʱ��û���޸���Сֵ��λ�ã������Ҫͬʱ�޸���Сֵ��λ��
	{
		if (root.get_size() <= 1) //���úϲ�
		{
			return;
		}
		auto temp = root.get_head();
		std::vector<std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>>> root_elem(ceil(log(size) / log(1.618) + 1), NULL); //vector�е��±�ʹ���ǰ�洢���Ķ�
		root_elem[root.get_head()->element->degree] = temp;
		temp = temp->next;
		while (temp != root.get_head()) //ѭ��һȦ֮�����
		{
			while (true) //���vector�д洢���뵱ǰ������ͬ�Ľ�㣬��Ҫ����������ϲ������ںϲ�֮�����п����ظ��������Ҫ������ѭ��
			{
				std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>> x = root_elem[temp->element->degree];
				if (x) //vector�д洢�к͵�ǰ���temp��һ���Ľ�㣬��Ҫ���ϲ�
				{
					root_elem[temp->element->degree] = NULL;
					if (x->element->key < temp->element->key) //ÿ�β���������ǽ�x��Ϊ���Ӳ��뵽λ�ÿ�ǰ��temp����ϣ�Ϊ�˱�����С�����ʣ�������Ҫ������listnode��Ԫ��������
					{
						auto temp2 = temp->element;
						temp->element = x->element;
						x->element = temp2;
					}
					if (!temp->element->child) //����temp��㵱ǰ�Ƿ��к��������в�ͬ�Ĳ��������û�к��ӣ��Ͱ�temp�����Ϊ���Ӳ��ȥ
					{
						temp->element->child = x->element;
					}
					else //����Ѿ��к��ӣ��ͽ�����뵽temp��child�������
					{
						x->element->right = temp->element->child;
						x->element->left = temp->element->child->left;
						temp->element->child->left->right = x->element; //ע������������˳��
						temp->element->child->left = x->element;
					}
					x->element->root_ptr = NULL;
					x->element->parent = temp->element;
					temp->element->degree++;
				}
				else //���vector��û����temp������ͬ�Ľ�㣬�Ͱ�temp���뵽vector�У���ֱ������ѭ��
				{
					root_elem[temp->element->degree] = temp; //����ǰԪ�ش���vector����ָ����һԪ��
					temp = temp->next;
					break;
				}
			}
		}
		root = List<std::shared_ptr<Fibonacci_heap_node<T>>>(circular_link); //������ʹ�������е�Ԫ�أ��ٶ�list��������
		for (auto i : root_elem) 
		{
			if (i)
			{
				auto temp = root.insert_end(i->element);
				temp->element->parent = NULL;
				temp->element->root_ptr = root.get_head()->prev;
				if (temp->element->key < root.get_head()->element->key)
				{
					root.change_head(temp);
				}
			}
		}
	}
	void decrease_key(std::shared_ptr<Fibonacci_heap_node<T>> p, T newval) //����ĳ������keyֵ
	{
		p->key = newval;
		if (!(p->parent)) //���û�и���㣬��ô������һ��λ�ڸ�������
		{
			if (p->key < root.get_head()->element->key) //�������keyС�ڵ�ǰ����������С��key����ô��Ҫ����ָ��p����Ӧ���Ǹ������head
			{
				root.change_head(p->root_ptr);
			}
		}
		else if (p->key < p->parent->key) //�����������С�����ʣ��Ͱ��������ĺ���ֱ���е������������
		{
			cut(p->parent, p);
		}
	}
	void cut(std::shared_ptr<Fibonacci_heap_node<T>> x, std::shared_ptr<Fibonacci_heap_node<T>> y) //x�Ǹ���㣬y���ӽ�㣬��y��x�����������������������
	{
		if (y&&x->child == y) //���x�ĺ���ָ��պ���y������Ҫ�������޸�
		{
			if (y->left != y)
			{
				x->child = y->left;
			}
			else //�൱��xֻ��һ������
			{
				x->child = NULL;
			}
		}
		y->left->right = y->right;
		y->right->left = y->left;
		y->left = y;
		y->right = y;
		y->parent = NULL;
		root.insert_end(y);
		y->root_ptr = root.get_head()->prev;
		x->degree--;
		if (y->key < root.get_head()->element->key)
		{
			root.change_head(root.get_head()->prev);
		}
		if (!x->root_ptr&&x->mark == true)
		{
			cascade_cut(x); //���֮ǰ�Ѿ�ʧȥһ�����ӵĻ�����ʱʧȥ�ڶ������ӣ���Ҫ���ô˺���������
		}
		else
		{
			x->mark = true;
		}
	}
	void cascade_cut(std::shared_ptr<Fibonacci_heap_node<T>> x) //x�Ѿ�ʧȥ�������ӣ�ɾȥx�������汻��ǵ�Ԫ�ز��������������
	{
		while (x->parent) //һֱ������ֱ������ĳ������mark���Ϊfalse����������cut�����жϣ����������
		{
			x->mark = false;
			cut(x->parent, x); //ֱ�ӵ���cut�������ɣ����x_p�ı��Ϊtrue��cut�������Զ��ݹ����
		}
	}
	void union_heap(Fibonacci_heap<T> f2) //������Fibonacci�Ѻϲ�����
	{
		auto iter = f2.root.get_head()->next;
		while (iter != f2.root.get_head())
		{
			insert_node(iter->element);
			iter = iter->next;
		}
		auto mark = insert_node(iter->element);
		if (mark->key < root.get_head()->element->key)
		{
			std::swap(root.get_head()->element, mark);
		}
		size += f2.size;
	}
	template<typename U> void traverse(U op) //��쳲��������еĽ����һ�α���
	{
		auto temp = root.get_head();
		traverse_node(temp->element, op);
		temp = temp->next;
		std::cout << std::endl;
		while (temp != root.get_head())
		{
			traverse_node(temp->element, op);
			std::cout << std::endl;
			temp = temp->next;
		}
	}
	template<typename U> void traverse_node(std::shared_ptr<Fibonacci_heap_node<T>> node, U op)
	{
		std::vector<std::shared_ptr<Fibonacci_heap_node<T>>> nodevec1{ node };
		std::vector<std::shared_ptr<Fibonacci_heap_node<T>>> nodevec2;
		bool vec1 = true;
		while (nodevec1.size() || nodevec2.size())
		{
			if (vec1)
			{
				for (auto i : nodevec1)
				{
					op(i);
					auto temp_child = i->child;
					auto sig = temp_child; //�����������ж�ѭ�������ı�־
					if (temp_child)
					{
						while (true) //��temp_child�����ֵ�ȫ��������б��У���ͬʱ����parentָ����Ϊ�ա������ֵ�֮���ָ����ѭ���ģ����ֻ��ָ��ѭ��һ�鼴��
						{
							nodevec2.push_back(temp_child);
							temp_child = temp_child->right; //�������Ĳ�����ı�ָ��ָ�������Ҫ��������һλ
							if (temp_child == sig)
							{
								break;
							}
						}
					}
				}
				nodevec1.clear();
				std::cout << std::endl;
				vec1 = false;
			}
			else
			{
				for (auto i : nodevec2)
				{
					op(i);
					auto temp_child = i->child;
					auto sig = temp_child; //�����������ж�ѭ�������ı�־
					if (temp_child)
					{
						while (true) //��temp_child�����ֵ�ȫ��������б��У���ͬʱ����parentָ����Ϊ�ա������ֵ�֮���ָ����ѭ���ģ����ֻ��ָ��ѭ��һ�鼴��
						{
							nodevec1.push_back(temp_child);
							temp_child = temp_child->right; //�������Ĳ�����ı�ָ��ָ�������Ҫ��������һλ
							if (temp_child == sig)
							{
								break;
							}
						}
					}
				}
				nodevec2.clear();
				std::cout << std::endl;
				vec1 = true;
			}
		}
	}
};

/*
���Դ��룺
template<typename T> struct visit
{
	void operator()(T obj)
	{
		std::cout << obj->key << " ";
		std::cout << "|";
	}
};

int main()
{
	visit<std::shared_ptr<Fibonacci_heap_node<int>>> myvisit;
	Fibonacci_heap<int> test;
	for (int i = 0;i < 5;i++)
	{
		test.insert(i);
	}
	Fibonacci_heap<int> test2;
	for (int i = 55;i < 60;i++)
	{
		test2.insert(i);
	}
	test.union_heap(test2);
	std::shared_ptr<Fibonacci_heap_node<int>> temp = std::shared_ptr<Fibonacci_heap_node<int>>(new Fibonacci_heap_node<int>(NULL, NULL, NULL, NULL, 0, 1000, false)); //�½�һ�����
	temp->left = temp;
	temp->right = temp;
	test.insert_node(temp);
	for (int i = 0;i < 5;i ++)
	{
		test.traverse(myvisit);
		std::cout << std::endl;
		std::cout << test.minimum()->key << std::endl;
		test.extract_min();
		std::cout << std::endl;
	}
	test.decrease_key(temp, 0);
	test.traverse(myvisit);
	std::cout << std::endl;
	return 0;
}
*/