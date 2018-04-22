#pragma once
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>
#include"chapter10.h"

template<typename T> class BinarySearchTree :public Binarytree<T>
{
public:
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
			else if (elem < temp->element)
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
				return temp;
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
		std::shared_ptr<BinarytreeNode<T>> newelem = new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0);
		if (elem > temp->element)
		{
			insert_rightchild(temp, newelem);
			updateheightabove(newelem);
		}
		else
		{
			insert_leftchild(temp, newelem);
			updateheightabove(newelem);
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
			if (temp == temp->parent->leftchild)
			{
				return temp->parent;
			}
			else
			{
				while (temp == temp->parent->rightchild)
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
			return find_minimum(temp->leftchild);
		}
		else
		{
			if (temp == temp->parent->rightchild)
			{
				return temp->parent;
			}
			else
			{
				while (temp == temp->parent->leftchild)
				{
					temp = temp->parent;
				}
				temp = temp->parent;
				return temp;
			}
		}
	}
	void delete_element(T elem)
	{
		auto temp = search_element(elem);
		if (temp->element == elem)
		{
			if (!temp->leftchild&&!temp->rightchild) //�����Ҷ�ڵ��ֱ��ɾ������
			{
				delete_node(temp); //�����ڲ��Ѿ������˸��¸߶ȹ��ܣ������ٽ��в���
				return;
			}
			else if (temp->leftchild&&!temp->rightchild) //��������ӵ���û���Һ��ӣ���ҪѰ������ǰ��
			{
				auto pred = find_predecessor(elem);
				temp->element = pred->element;
				if (pred->leftchild) //���ǰ���������ӣ���ǰ������Ҷ�ڵ㣬��������������򣬴�ʱ���ǰ��һ��û���Һ���
				{
					auto temp2 = pred->leftchild;
					temp2->parent = pred->parent; //��pred�������滻�Լ���λ�ã�����pred������ָ��ܿ�
					pred->parent->leftchild = temp2;
					pred->parent = pred->leftchild= NULL;
					updateheightabove(temp2); //��Ҫ�����¸߶Ȳ���
				}
				else //���ǰ��û�����ӣ���ô������������Ĺ�������һ����Ҷ�ڵ�
				{
					delete_node(pred);
				}
				return;
			}
			else if (temp->rightchild) //����ڵ����Һ��ӣ������Һ�����Ѱ�����ĺ��
			{
				auto succ = find_successor(elem);
				temp->element = succ->element;
				if (!succ->rightchild) //���������������������û���Һ��ӣ���ض���Ҷ�ڵ㣬ֱ��ɾ������
				{
					delete_node(succ);
				}
				else //������Һ��ӣ���Ҫ���Һ��Ӷ�Ӧ������Ϊ�Һ��ӽ��뵽�����̽ڵ���
				{
					auto temp2 = succ->rightchild;
					temp2->parent = succ->parent;
					succ->parent->rightchild = temp2;
					succ->parent = succ->rightchild = NULL;
					updateheightabove(temp2);
				}
				return;
			}
		}
	}
	/*
	����delete_element����һ��д����clrs��α����ı������
	void transplant(std::shared_ptr<BinarytreeNode<T>> node1, std::shared_ptr<BinarytreeNode<T>> node2) //��node2�滻node1��Ϊ���ӽڵ�
	{
		if (node1->parent->leftchild == node1)
		{
			node1->parent->leftchild = node2;
			node2->parent = node1->parent;
			node1->parent = NULL;
			updateheightabove(node2);
		}
		else if(node1->parent->rightchild == node1)
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
	*/

};
