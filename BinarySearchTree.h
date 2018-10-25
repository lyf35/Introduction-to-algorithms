#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<algorithm>
#include<memory>
#include"Binarytree.h"

template<typename T> class BinarySearchTree :public Binarytree<T>
{
public:
	BinarySearchTree(T elem) :Binarytree(elem) {};
	std::shared_ptr<BinarytreeNode<T>> search_element(T elem)
	{
		auto temp = visit_root();
		while (true)
		{
			if (elem > temp->element)
			{
				if (temp->rightchild)
				{
					temp = temp->rightchild;
				}
				else
				{
					return temp;
				}
			}
			else if(elem < temp->element)
			{
				if (temp->leftchild)
				{
					temp = temp->leftchild;
				}
				else
				{
					return temp;
				}
			}
			else
			{
				if (temp->leftchild&&temp->leftchild->element == elem)
				{
					temp = temp->leftchild;
				}
				else
				{
					return temp;
				}
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> search_element_recursive(std::shared_ptr<BinarytreeNode<T>> node, T elem)
	{
		if (node->element > elem&&node->leftchild)
		{
			search_element_recursive(node->leftchild, elem);
		}
		else if (node->element < elem&&node->rightchild)
		{
			search_element_recursive(node->rightchild, elem);
		}
		else
		{
			return node;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> insert_element(T elem)
	{
		auto temp = search_element(elem);
		std::shared_ptr<BinarytreeNode<T>> newelem(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0));
		if (elem > temp->element)
		{
			insert_rightchild(temp, newelem); //���뺯���Ѿ������˸��¸߶ȵ�����
		}
		else if (elem < temp->element)
		{
			insert_leftchild(temp, newelem);
		}
		else
		{
			auto temp2 = temp->leftchild;
			temp = insert_leftchild(temp, newelem);
			if (temp2)
			{
				temp2 = insert_leftchild(temp, temp2);
			}

		}
		return newelem;
	}
	std::shared_ptr<BinarytreeNode<T>> find_minimum(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (node->leftchild)
		{
			node = node->leftchild;
		}
		return node;
	}
	std::shared_ptr<BinarytreeNode<T>> find_maximum(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (node->rightchild)
		{
			node = node->rightchild;
		}
		return node;
	}
	std::shared_ptr<BinarytreeNode<T>> find_minimum_recursive(std::shared_ptr<BinarytreeNode<T>> node)
	{
		if(node->leftchild)
		{
			node = node->leftchild;
			find_mininum_recursive(node);
		}
		else
		{
			return node;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> find_maximum_recursive(std::shared_ptr<BinarytreeNode<T>> node)
	{
		if (node->rightchild)
		{
			node = node->rightchild;
			find_maximum_recursive(node);
		}
		else
		{
			return node;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> find_successor(T elem)
	{
		auto temp = search_element(elem);
		if (temp->rightchild)
		{
			return find_minimum(temp->rightchild);
		}
		else
		{
			if (temp->parent&&temp == temp->parent->leftchild) //һ��Ҫ��ȷ�ϸ�ָ���Ƿ���ڣ�
			{
				return temp->parent;
			}
			else
			{
				while (temp->parent&&temp == temp->parent->rightchild)
				{
					temp = temp->parent;
				}
				temp = temp->parent;
				return temp;
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> find_predecessor(T elem)
	{
		auto temp = search_element(elem);
		if (temp->leftchild)
		{
			return find_maximum(temp->leftchild);
		}
		else
		{
			if (temp->parent&&temp == temp->parent->rightchild)
			{
				return temp->parent;
			}
			else
			{
				while (temp->parent&&temp == temp->parent->leftchild)
				{
					temp = temp->parent;
				}
				temp = temp->parent;
				return temp;
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> delete_element(T elem) //������������ɾ��Ԫ�س��򣬷���ɾ���ڵ�ĸ��ڵ��Ա���������и߶ȵ����Ȳ���
	{
		auto del = search_element(elem); //��ɾ���ڵ�
		auto node = del->parent;
		if (del->element == elem)
		{
			if ((!del->leftchild) && (!del->rightchild)) //�����Ҷ�ڵ㣬����ֱ��ɾ��
			{
				delete_node(del); //�����ڲ��Ѿ������˸��¸߶ȹ��ܣ������ٸ��¸߶�
			}
			else if (del->leftchild && (!del->rightchild)) //��������ӵ���û���Һ��ӣ���ҪѰ������ǰ��
			{
				auto pred = find_predecessor(elem); //ǰ���ڵ�
				del->element = pred->element; //��ǰ���ڵ��ֵ����del��Ȼ��ɾ�����ǰ���ڵ㼴��
				del = pred; //����del��ֵ
				node = del->parent;
				if (del->leftchild) //���ǰ���������ӣ���ǰ������Ҷ�ڵ㣬��������������򣬴�ʱ���ǰ��һ��û���Һ���
				{
					auto temp = del->leftchild;
					temp->parent = del->parent; //��pred�������滻�Լ���λ�ã�����pred������ָ��ܿ�
					if (del->parent->rightchild == del)
					{
						del->parent->rightchild = temp;
						del->parent = del->rightchild = NULL;
					}
					else
					{
						del->parent->leftchild = temp;
						del->parent = del->leftchild = NULL;
					}
					updateheightabove(temp); //��Ҫ�����¸߶Ȳ���
				}
				else //���ǰ��û�����ӣ���ô������������Ĺ�������һ����Ҷ�ڵ�
				{
					delete_node(del);
				}
			}
			else if (del->rightchild) //����ڵ����Һ��ӣ������Һ�����Ѱ�����ĺ��
			{
				auto succ = find_successor(elem);
				del->element = succ->element; //����Ԫ�ص�ֵ��������delΪsucc�ڵ�
				del = succ;
				node = del->parent;
				if (!del->rightchild) //���������������������û���Һ��ӣ���ض���Ҷ�ڵ㣬ֱ��ɾ������
				{
					delete_node(del);
				}
				else //������Һ��ӣ���Ҫ���Һ��Ӷ�Ӧ������Ϊ�Һ��ӽ��뵽�����̽ڵ���
				{
					auto temp = del->rightchild;
					temp->parent = del->parent;
					if (del->parent->rightchild == del)
					{
						del->parent->rightchild = temp;
						del->parent = succ->rightchild = NULL;
					}
					else
					{
						del->parent->leftchild = temp;
						del->parent = del->leftchild = NULL;
					}
					updateheightabove(temp);
				}
			}
		}
		return node;
	}
	//����delete_element����һ��д����clrs��α����ı������
	void transplant(std::shared_ptr<BinarytreeNode<T>> node1, std::shared_ptr<BinarytreeNode<T>> node2) //��node2�滻node1��Ϊ���ӽڵ�
	{
		if (node1->parent->leftchild == node1)
		{
			node1->parent->leftchild = node2;
			node2->parent = node1->parent;
			node1->parent = NULL;
			updateheightabove(node2);
		}
		else if (node1->parent->rightchild == node1)
		{
			node1->parent->rightchild = node2;
			node2->parent = node1->parent;
			node1->parent = NULL;
			updateheightabove(node2);
		}
	}
	void delete_element_v2(T elem)
	{
		auto temp = search_element(elem);
		if (temp->element == elem)
		{
			if (!temp->leftchild&&temp->rightchild)
			{
				transplant(temp, temp->rightchild);
				return;
			}
			else if (temp->leftchild&&!temp->rightchild)
			{
				transplant(temp, temp->leftchild);
				return;
			}
			else if (temp->leftchild&&temp->rightchild)
			{
				auto succ = find_successor(temp);
				if (succ == temp->rightchild)
				{
					transplant(temp, succ);
				}
				else
				{
					transplant(succ, succ->rightchild);
					temp->element = succ->element;
				}
			}
			else
			{
				delete_node(temp);
			}
		}
	}
};



/*
�����ô��룺
template<typename T> class visit
{
public:
	void operator()(T elem)
	{
		std::cout << elem->element << '(' << elem->height << ')' << '\t';
	}
};

int main()
{
	visit<std::shared_ptr<BinarytreeNode<int>>> myvisit;
	BinarySearchTree<int> test(100);
	test.insert_element(50);
	test.insert_element(70);
	test.insert_element(30);
	test.insert_element(35);
	test.insert_element(30);
	test.insert_element(38);
	test.insert_element(30);
	test.insert_element(25);
	test.insert_element(32);
	test.insert_element(37);
	test.insert_element(39);
	test.insert_element(60);
	test.insert_element(65);
	test.insert_element(69);
	test.insert_element(68);
	test.insert_element(62);
	test.insert_element(600);
	test.insert_element(205);
	test.insert_element(300);
	test.delete_element(25);
	test.level_traverse(test.visit_root(), myvisit);

	std::cout << std::endl;
	test.delete_element(70);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	test.delete_element(50);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	test.inorder_traverse_recursive(test.visit_root(), myvisit);
	std::cout << std::endl;
	return 0;
}
*/
