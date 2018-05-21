#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include"BinarySearchTree.h"

template<typename T> class AVLTree :public BinarySearchTree<T> //����AVL������Ҫ����ɫ�任��������Ĵ���Ⱥ�����򵥣������ɾ�����������Ե���BST�Ľӿڣ�Ȼ�������߶ȵĵ�������
{
public:
	AVLTree(T elem) :BinarySearchTree(elem) {};
	void balance(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (node)
		{
			auto lc = node->leftchild;
			auto rc = node->rightchild;
			if (find_height(lc) - find_height(rc) > 1) //���������������ĸ߶����ٴ�2
			{
				if (find_height(lc->rightchild) - find_height(lc->leftchild) >= 1)
				{
					left_rotate(lc);
				}
				node = right_rotate(node);
			}
			else if (find_height(rc) - find_height(lc) > 1) //���������������ĸ߶����ٴ�2���൱�������Ǹ�if�ľ������
			{
				if (find_height(rc->leftchild) - find_height(rc->rightchild) >= 1)
				{
					right_rotate(rc);
				}
				node = left_rotate(node);
			}
			node = node->parent;
		}
	}
	void insert_element(T elem)
	{
		auto temp = BinarySearchTree<T>::insert_element(elem);
		balance(temp->parent);
	}
	void delete_element(T elem)
	{
		auto temp = BinarySearchTree<T>::delete_element(elem);
		balance(temp);
	}
};

/*
���Դ��룺
template<typename T> class visit
{
public:
	void operator()(T elem)
	{
		std::cout << elem->element << '(' << elem->height << ')' << '(' << elem->color << ')' << '\t';
	}
};

int main()
{
	visit<std::shared_ptr<BinarytreeNode<int>>> myvisit;
	RBTree<int> test(100);
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
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(25);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(65);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(70);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(50);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(100);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;


	test.delete_element(60);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(68);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(30);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(30);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;
	return 0;
}
*/