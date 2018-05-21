#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include"List.h"

 //�������ṹ�Ĵ��룬�����ں���ͨ���̳еķ�ʽ���ɶ�����������������Ƚṹ
enum RBcolor { red, black };
enum orientation { left, right };
template<typename T> struct BinarytreeNode :public std::enable_shared_from_this<BinarytreeNode<T>>
{
	RBcolor color;
	std::shared_ptr<BinarytreeNode<T>> parent;
	std::shared_ptr<BinarytreeNode<T>> leftchild;
	std::shared_ptr<BinarytreeNode<T>> rightchild;
	T element;
	int height;
	void pointer_fixed(orientation x = left) //�����������ϳ�һ������������Ҫ���д˴���
	{
		if (parent)
		{
			if (x == left)
			{
				parent->leftchild = shared_from_this();
			}
			else if (x == right)
			{
				parent->rightchild = shared_from_this();
			}
		}
		if (leftchild)
		{
			leftchild->parent = shared_from_this();
		}
		if (rightchild)
		{
			rightchild->parent = shared_from_this();
		}
	}
	BinarytreeNode(std::shared_ptr<BinarytreeNode<T>> p = NULL, std::shared_ptr<BinarytreeNode<T>> lc = NULL, std::shared_ptr<BinarytreeNode<T>> rc = NULL, T elem = 0, int h = 0, RBcolor c = black) :parent(p), leftchild(lc), rightchild(rc), element(elem), height(h), color(c) {};
};
template<typename U> bool operator<(BinarytreeNode<U> a, BinarytreeNode<U> b)
{
	return a.element < b.element;
}
template<typename U> bool operator>(BinarytreeNode<U> a, BinarytreeNode<U> b)
{
	return a.element > b.element;
}
template<typename U> bool operator==(BinarytreeNode<U> a, BinarytreeNode<U> b)
{
	return a.element == b.element;
}

template<typename T> class Binarytree
{
private:
	std::shared_ptr<BinarytreeNode<T>> root;
public:
	Binarytree(std::shared_ptr<BinarytreeNode<T>> node) :root(node) {};
	Binarytree(T elem)
	{
		root = std::shared_ptr<BinarytreeNode<T>>(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0));
	}
	std::shared_ptr<BinarytreeNode<T>> visit_root()
	{
		return root;
	}
	void updateheightabove(std::shared_ptr<BinarytreeNode<T>> node) //��������
	{
		while (node)
		{
			int lh = node->leftchild ? node->leftchild->height : -1;
			int rh = node->rightchild ? node->rightchild->height : -1;
			int i = lh > rh ? lh : rh;
			node->height = ++i;
			node = node->parent;
		}
	}
	int find_height(std::shared_ptr<BinarytreeNode<T>> node)
	{
		if (node)
		{
			return node->height;
		}
		else
		{
			return -1;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> insert_leftchild(std::shared_ptr<BinarytreeNode<T>> node, std::shared_ptr<BinarytreeNode<T>> lc)
	{
		node->leftchild = lc;
		lc->parent = node;
		updateheightabove(lc);
		return lc;
	}
	std::shared_ptr<BinarytreeNode<T>> insert_rightchild(std::shared_ptr<BinarytreeNode<T>> node, std::shared_ptr<BinarytreeNode<T>> rc)
	{
		node->rightchild = rc;
		rc->parent = node;
		updateheightabove(rc);
		return rc;
	}
	std::shared_ptr<BinarytreeNode<T>> find_leftchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->leftchild;
	}
	std::shared_ptr<BinarytreeNode<T>> find_rightchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->rightchild;
	}
	std::shared_ptr<BinarytreeNode<T>> find_parent(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->parent;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_leftchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->leftchild;
		node->leftchild = NULL;
		if (temp)
		{
			temp->parent = NULL;
			updateheightabove(node->rightchild);
		}
		else if (!node->rightchild)
		{
			updateheightabove(node);
		}
		return temp;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_rightchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->rightchild;
		node->rightchild = NULL;
		if (temp)
		{
			temp->parent = NULL;
			updateheightabove(node->leftchild);
		}
		else if(!node->leftchild)
		{
			updateheightabove(node);
		}
		return temp;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_node(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->parent;
		if (node != visit_root() && temp->leftchild==node)
		{
			temp->leftchild = NULL;
			node->parent = NULL;
			updateheightabove(temp);
		}
		if (node != visit_root() && temp->rightchild==node)
		{
			temp->rightchild = NULL;
			node->parent = NULL;
			updateheightabove(temp);
		}
		return node;
	}
	//�ݹ���ʽ�����ֱ���
	template<typename U> void preorder_traverse_recursive (std::shared_ptr<BinarytreeNode<T>> node, U op)//U���������⺯�������Ǿ��У������������
	{
		op(node);
		if (node->leftchild.get())
		{
			preorder_traverse_recursive(node->leftchild, op);
		}
		if (node->rightchild.get())
		{
			preorder_traverse_recursive(node->rightchild, op);
		}
	}
	template<typename U> void inorder_traverse_recursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		if (node->leftchild.get())
		{
			inorder_traverse_recursive(node->leftchild, op);
		}
		op(node);
		if (node->rightchild.get())
		{
			inorder_traverse_recursive(node->rightchild, op);
		}
	}
	template<typename U> void postorder_traverse_recursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		if (node->leftchild.get())
		{
			postorder_traverse_recursive(node->leftchild, op);
		}
		if (node->rightchild.get())
		{
			postorder_traverse_recursive(node->rightchild, op);
		}
		op(node);
	}
	//�����Ƿǵݹ���ʽ�����ֱ���
	template<typename U> void preorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container; //ֻʹ��list��insert_end��delete_end��������ʵ�൱������ʹ��ջ������ͬ��
		container.insert_end(NULL);
		while (node)
		{
			op(node);
			if (find_rightchild(node).get())
			{
				container.insert_end(find_rightchild(node));
			}
			if (!(node = find_leftchild(node)).get())
			{
				node = container.delete_end();
			}
		}
	}
	void leftmost_trace(std::shared_ptr<BinarytreeNode<T>> node, List<std::shared_ptr<BinarytreeNode<T>>>& container)
	{
		while (node)
		{
			container.insert_end(node);
			node = find_leftchild(node);
		}
	}
	template<typename U> void inorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_end(NULL);
		leftmost_trace(node, container);
		while (true) //һ��Ҫע�⣬һ��һֱ�������������������֧·Ҫ��֤������������ֻ����һ�飡
		{
			node = container.delete_end();
			if (node != NULL)
			{
				op(node);
			}
			else
			{
				break;
			}
			if (find_rightchild(node))
			{
				leftmost_trace(find_rightchild(node), container);
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> leftmost_trace_v2(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (find_leftchild(node))
		{
			node = find_leftchild(node);
		}
		return node;
	}
	template<typename U> void inorder_traverse_nonrecursive_v2(std::shared_ptr<BinarytreeNode<T>> node, U op) //��ʹ��ջ��Ϊ��ʱ�洢�ĵݹ�
	{
		node = leftmost_trace_v2(node); //���������������ӽڵ㿪ʼ����
		while(node) //ֻҪnode���ǿս���һֱѭ����ȥ
		{
			op(node);
			if (find_rightchild(node)) //���node������������ֱ������������������ڵ㿪ʼ����
			{
				node = leftmost_trace_v2(find_rightchild(node)); 
			}
			else //���nodeû������������ô��Ҫ����
			{
				while (find_parent(node)->rightchild == node)
				{
					node = find_parent(node); //���node���Һ��ӣ�Ҫһֱ����׷��
					if (node == visit_root())
					{
						return;
					}
				}
				node = find_parent(node); //�˳���һ��whileѭ����ʱ��node�Ѿ������ӣ���ʱ���nodeһ�����ʹ������������׷��һ��
			}
		}
	}
	void insert_container(std::shared_ptr<BinarytreeNode<T>> p, List<std::shared_ptr<BinarytreeNode<T>>>& container)
	{
		while(p)
		{
			if (find_leftchild(p))
			{
				if (find_rightchild(p))
				{
					container.insert_end(find_rightchild(p));
				}
				container.insert_end(p = find_leftchild(p));
			}
			else
			{
				container.insert_end(p = find_rightchild(p));
			}
		}
		container.delete_end();
	}
	template<typename U> void postorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_end(node);
		while (container.get_size())
		{
			if (container.get_end()->element != node->parent) //��ջ���ڵ㲻�ǵ�ǰ���ĸ��ף�����Ҫ��ջ���ڵ�����λ����������Ҳ����˵����ʱջ���ڵ����ҽڵ㣬����ǰ�ڵ���������ڵ�
			{
				insert_container(container.get_end()->element, container);
			}
			node = container.delete_end();
			op(node);
		}
	}
	template<typename U> void level_traverse(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_begin(node);
		while (container.get_beg() != container.get_tail())
		{
			node = container.delete_end();
			op(node);
			if (node->leftchild)
			{
				container.insert_begin(node->leftchild);
			}
			if (node->rightchild)
			{
				container.insert_begin(node->rightchild);
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> left_rotate(std::shared_ptr<BinarytreeNode<T>> x) //xΪ���ڵ㣬��x������������ʹx��Ϊ���ӽڵ㣬x���Һ���y����Ϊ���ڵ㣬����y�����ӽ���x���Һ��Ӵ�
	{
		auto y = x->rightchild; //y�滻x��λ��
		if (y == NULL) //���y��Ҷ�ڵ�������Ĳ��費���ٽ�����ȥ
		{
			return NULL;
		}
		auto y_lc = y->leftchild; //y�����ӣ�Ҫ���뵽x���Һ���
		if (x == root)
		{
			root = y;
			root->parent = NULL;
		}
		else if (x->parent->leftchild == x)
		{
			x->parent->leftchild = y;
			y->parent = x->parent;
		}
		else if (x->parent->rightchild == x)
		{
			x->parent->rightchild = y;
			y->parent = x->parent;
		}
		x->parent = y;
		y->leftchild = x;
		x->rightchild = y_lc;
		if (y_lc)
		{
			y_lc->parent = x;
		}
		updateheightabove(y->rightchild);
		updateheightabove(x);
		return x->parent;
	}
	std::shared_ptr<BinarytreeNode<T>> right_rotate(std::shared_ptr<BinarytreeNode<T>> x) //xΪ���ڵ㣬��x������������ʹx��Ϊ�Һ��ӽڵ㣬x������y����Ϊ���ڵ㣬����y���Һ��ӽ���x�����Ӵ����൱��left-rotate�ľ������
	{
		auto y = x->leftchild; //yҪ�滻x��λ��
		if (y == NULL)
		{
			return NULL;
		}
		auto y_rc = y->rightchild; //y���Һ��ӣ�Ҫ���뵽x������
		if (x == root)
		{
			root = y;
			root->parent = NULL;
		}
		else if (x->parent->leftchild == x)
		{
			x->parent->leftchild = y;
			y->parent = x->parent;
		}
		else if (x->parent->rightchild == x)
		{
			x->parent->rightchild = y;
			y->parent = x->parent;
		}
		x->parent = y;
		y->rightchild = x;
		x->leftchild = y_rc;
		if (y_rc)
		{
			y_rc->parent = x;
		}
		updateheightabove(y->leftchild);
		updateheightabove(x);
		return x->parent;
	}
};

/*
����ʹ�õ����������룺
Binarytree<int> test(1);
auto temp = test.insert_leftchild(test.visit_root(), std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 2)));
auto temp2 = test.insert_rightchild(test.visit_root(), std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 3)));
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 4)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 5)));
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 6)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 7)));
temp = test.find_leftchild(temp);
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 8)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 9)));
temp = test.find_rightchild(test.find_parent(temp));
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 10)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 11)));
temp2 = test.find_leftchild(temp2);
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 12)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 13)));
temp2 = test.find_rightchild(test.find_parent(temp2));
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 14)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 15)));
visit<int> my_visit;
test.preorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.preorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.inorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.inorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.inorder_traverse_nonrecursive_v2(test.visit_root(), my_visit);
test.postorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.postorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.level_traverse(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

*/