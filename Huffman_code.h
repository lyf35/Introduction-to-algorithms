#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<memory>
#include<string>
#include"min_heap.h"
#include"Binarytree.h"
#include"randomized_quicksort.h"

struct Huffman_data //һ�����ݰ��������ַ����Ͷ�Ӧ�ĳ��ָ���
{
	double probability;
	std::string data;
	std::string Huffman_code;
	Huffman_data(double p = 0, std::string d = "", std::string s = "") :probability(p), data(d), Huffman_code(s) {};
};
bool operator<(Huffman_data a, Huffman_data b)
{
	return a.probability < b.probability;
}
bool operator>(Huffman_data a, Huffman_data b)
{
	return a.probability > b.probability;
}
bool operator==(Huffman_data a, Huffman_data b)
{
	return a.probability == b.probability;
}


bool operator<(std::shared_ptr<BinarytreeNode<Huffman_data>> a, std::shared_ptr<BinarytreeNode<Huffman_data>> b)
{
	return a->element < b->element;
}
bool operator>(std::shared_ptr<BinarytreeNode<Huffman_data>> a, std::shared_ptr<BinarytreeNode<Huffman_data>> b)
{
	return a->element > b->element;
}
bool operator==(std::shared_ptr<BinarytreeNode<Huffman_data>> a, std::shared_ptr<BinarytreeNode<Huffman_data>> b)
{
	return a->element == b->element;
}

template<typename T> struct add_Huffman_code //��Huffman���������֮�󣬶���Ӧλ���ϵ�ֵ���ɶ�Ӧ��Huffman����
{
	void operator()(T d)
	{
		if (d->leftchild&&d->rightchild)
		{
			d->leftchild->element.Huffman_code = d->element.Huffman_code + "0";
			d->rightchild->element.Huffman_code = d->element.Huffman_code + "1";
		}
	}
};

Binarytree<Huffman_data> Huffman_tree(std::vector<Huffman_data> a)
{
	std::vector<std::shared_ptr<BinarytreeNode<Huffman_data>>> data;
	for (int i = 0;i < a.size();i++)
	{
		std::shared_ptr<BinarytreeNode<Huffman_data>> temp(new BinarytreeNode<Huffman_data>(NULL, NULL, NULL, a[i], 0, black));
		data.push_back(temp);
	}
	build_min_heap(data);
	while (data.size() > 1)
	{
		auto lc = heap_extract_min(data);
		auto rc = heap_extract_min(data);
		if (lc->element.probability==rc->element.probability&&lc->element.data < rc->element.data) //��������ڵ�ĸ������ʱ�����ַ��ϴ�����Ϊ���������������
		{
			std::swap(lc, rc);
		}
		if (data.size())
		{
			Huffman_data temp(LDBL_MAX);
			Huffman_data temp2(lc->element.probability + rc->element.probability);
			std::shared_ptr<BinarytreeNode<Huffman_data>> newnode(new BinarytreeNode<Huffman_data>(NULL, lc, rc, temp, 0, black));
			std::shared_ptr<BinarytreeNode<Huffman_data>> newnode2(new BinarytreeNode<Huffman_data>(NULL, lc, rc, temp2, 0, black));
			newnode2->pointer_fixed();
			data.push_back(newnode);
			heap_decrease_key(data, data.size() - 1, newnode2);
		}
		else
		{
			Huffman_data temp2(lc->element.probability + rc->element.probability);
			std::shared_ptr<BinarytreeNode<Huffman_data>> newnode2(new BinarytreeNode<Huffman_data>(NULL, lc, rc, temp2, 0, black));
			newnode2->pointer_fixed();
			Binarytree<Huffman_data> tree(newnode2);
			add_Huffman_code<std::shared_ptr<BinarytreeNode<Huffman_data>>> Huffman_code_adder;
			tree.level_traverse(tree.visit_root(), Huffman_code_adder);
			return tree;
		}
	}
}

/*
���Դ��룺
template<typename T> struct traverse
{
	void operator()(T d)
	{
		if (!d->element.data.empty())
		{
			std::cout << d->element.data << "(" << d->element.probability << ")" << "(" << d->element.Huffman_code << ")" << '\t';
		}
	}
};

int main()
{
	traverse<std::shared_ptr<BinarytreeNode<Huffman_data>>> a;
	std::vector<Huffman_data> test{ Huffman_data(45,"a"),Huffman_data(13,"b"),Huffman_data(12,"c"),Huffman_data(16,"d"),Huffman_data(9,"e"),Huffman_data(5,"f") };
	auto result = Huffman_tree(test);
	result.level_traverse(result.visit_root(), a);
	std::cout << std::endl;
	return 0;
}
*/
