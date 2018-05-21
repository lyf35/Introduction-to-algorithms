#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include"BinarySearchTree.h"

template<typename T> class RBTree :public BinarySearchTree<T>  //�������һ������Ķ���������
{
public:
	RBTree(T elem) :BinarySearchTree(elem) {};
	std::shared_ptr<BinarytreeNode<T>> insert_element(T elem)
	{
		auto temp = search_element(elem);
		std::shared_ptr<BinarytreeNode<T>> newelem = std::shared_ptr<BinarytreeNode<T>>(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0, red));
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
		insert_fixup(newelem);
		return newelem;
	}
	void insert_fixup(std::shared_ptr<BinarytreeNode<T>> node) //����ڵ�Ϊ��ɫ���ܻ��ƻ�����������ʣ������������ɫ�ڵ���������˶������������Ҫ���¶��϶Ժ����������
	{
		/*����Ϊ6���������6������ɷ�Ϊ�ԳƵ������֡�һ���ǲ���ڵ�λ���游�ڵ����������������ڵ㣨�游�ڵ����������Ǻ�ɫ���ڶ����ǲ���ڵ�Ϊ������λ���游�ڵ���������������ڵ�Ϊ��ɫ���������ǲ���ڵ�Ϊ������λ���游�ڵ������������ڵ�Ϊ��ɫ��������������൱��������������ľ�����ˣ������¶��ϵ�ʱ����Ҫ�����6������ֱ���*/
		while (true) 
		{
			auto p = node->parent; //���ڵ�
			if (p->color == red&&node->color == red) //����������ɫ�ڵ㣬������ͻ
			{
				auto g = p->parent; //�游�ڵ�
				auto u = (g->leftchild == p) ? g->rightchild : g->leftchild; //��ڵ�
				if (u&&u->color == red) //����ڵ����ڵ㣨�游�ڵ����������Ǻ�ɫ�������ڵ����ڵ��Ϊ��ɫ��ͬʱ���游�ڵ��Ϊ��ɫ��
				{
					u->color = p->color = black;
					g->color = red;
					node = g; //�����游�ڵ��Ǻ�ɫ�����ܷ�����ͻ������Ҫ��һ������
					if (node == visit_root())//Ҳ�����游�ڵ��Ǹ��ڵ㣬�������ںڼ��ɣ�Ȼ��ֱ���˳�ѭ����������ǣ���ôѭ����Ҫ���游�ڵ����������ȥ
					{
						node->color = black;
						break; 
					}
				}
				else //��ڵ��Ǻ�ɫ����ʱ��Ҫ����ת������������ת�����������ĸ��ڵ���Ϊ��ɫ�������ӽڵ���Ϊ��ɫ
				{
					if (p == g->leftchild) //���ڵ�������
					{
						if (node == p->leftchild) //�ýڵ��Ǹ��ڵ�����ӣ���ʱ��Ҫ�Ը��ڵ�Ϊ֧������������
						{
							right_rotate(g); //����֮��p��Ϊ�������ĸ��ڵ�
							p->color = black;
							p->leftchild->color = p->rightchild->color = red;
						}
						else //�ýڵ��Ǹ��ڵ�����ӣ���Ҫ���Ը��ڵ�Ϊ֧��������������Ȼ����gΪ֧������������
						{
							left_rotate(p);
							right_rotate(g);
							node->color = black;
							node->leftchild->color = node->rightchild->color = red;
						}
					}
					else //���ڵ�������
					{
						if (node == p->rightchild) //�ýڵ��Ǹ��ڵ�����ӣ��Ը��ڵ�Ϊ֧����������������
						{
							left_rotate(g);
							p->color = black;
							p->leftchild->color = p->rightchild->color = red;
						}
						else //�ýڵ��Ǹ��ڵ�����ӣ����Ը��ڵ�Ϊ֧��������������gΪ֧����������������
						{
							right_rotate(p);
							left_rotate(g);
							node->color = black;
							node->leftchild->color = node->rightchild->color = red;
						}
					}
					break; //��һ������֮������ڵ�ֱ��Ϊ��ɫ��һ������������Ҫ�󣬹ʿ���ֱ���˳�ѭ��
				}
			}
			else
			{
				break;
			}
		}
	}
	void delete_element(T elem) //���������BST��ɾ���������ƣ���Ϊ֮����Ҫ����ɾ���Ľڵ�����ڵ㻹���ҽڵ��ȡ��ز��������޷�ͨ��BSTɾ�������ķ���ֵȷ��ɾ���ڵ������ӻ����ӣ�����BSTɾ�������Ļ�������дɾ������
	{
		auto del = search_element(elem); //��ɾ���ڵ�
		auto node = del->parent; //��¼�Ƿ�Ҫ����������ṹ�Ľڵ㣬��ɾ��Ԫ�صĸ��ڵ�
		orientation orit; //���ڽڵ�ɾ�����֮��֮ǰλ��delλ�õ��½ڵ������null�������һ��ֵ��ȷ�������������ӻ�������
		if (del->element == elem)
		{
			if ((!del->leftchild) && (!del->rightchild)) //�����Ҷ�ڵ㣬����ֱ��ɾ��
			{
				orit = (node->leftchild == del) ? left : right;
				delete_node(del); //�����ڲ��Ѿ������˸��¸߶ȹ��ܣ������ٸ��¸߶�
			}
			else if (del->leftchild && (!del->rightchild)) //��������ӵ���û���Һ��ӣ���ҪѰ������ǰ��
			{
				auto pred = find_predecessor(elem); //ǰ���ڵ�
				del->element = pred->element; //��ǰ���ڵ��ֵ����del��Ȼ��ɾ�����ǰ���ڵ㼴��
				del = pred; //����del��ֵ
				node = del->parent;
				orit = (node->leftchild == del) ? left : right;
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
				orit = (node->leftchild == del) ? left : right;
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
			if (del->color == black&&node) //���ɾ��Ԫ�ص���ɫΪ��ɫ����ô��Ҫ�Ժ�����Ľṹ������
			{
				delete_fixup(node, orit);
			}
		}
	}
	void delete_fixup(std::shared_ptr<BinarytreeNode<T>> node, oritation orit) //�ı�ṹ֮��Ҫ�ı�߶�
	{
		while (node)
		{
			auto lc = node->leftchild;
			auto rc = node->rightchild;
			if (orit == left) //֮ǰ��ɾ���Ľڵ���node���
			{
				if (lc&&lc->color == red) //�����ɾ���ڵ����ڱ���ɫ�ڵ���棬ֱ������Ϊ��ɫ����
				{
					lc->color = black;
					break;
				}
				else //����ɾ���ڵ㱻��ɫ�ڵ��������
				{
					if (rc->color == red) //���rc�Ǻ�ɫ�ڵ㣬��Ҫ�������������ı���ɫ��Ȼ�������node����
					{
						left_rotate(node); //��ʱnode����lc�ĸ��ڵ㣬����node�ĸ��ڵ��Ѿ������rc��lc�Բ�����������Ҫ�������Ҫ��lc����ѭ��
						node->color = red;
						rc->color = black;
					}
					else //rc�Ǻ�ɫ�ڵ㣬����lc��rc�ĸ߶�С1����ʱrc�����Һ��ӱ�Ȼ���Է��ʣ���ʹ��NULL������ʱ���ж�rc�������ӵ���ɫ��
					{
						if (!(rc->leftchild&&rc->leftchild->color == red) && !(rc->rightchild&&rc->rightchild->color == red)) //���Ҷ��Ǻ�ɫ
						{
							rc->color = red;
							orit = (node == node->parent->leftchild) ? left : right; //node�ڵ�����Ӧ����������߶��½�1�������һ��ѭ����Ҫ�滻node�ڵ�Ϊ���ĸ��ڵ�
							if (node == visit_root()) //�����ʱnode�Ǹ��ڵ���ôȾ��ֱ���˳�ѭ�����ɣ�������ǣ���ô��Ҫ��һ������
							{
								node->color = black;
								break;
							}
							node = node->parent;
						}
						else //��������һ���Ǻ�ɫ
						{
							if ((rc->leftchild&&rc->leftchild->color == red) && !(rc->rightchild&&rc->rightchild->color == red)) //���������Һڵ��������Ҫ��һ����������Ȼ���ٴ���
							{
								right_rotate(rc); 
								rc = node->rightchild;
								rc->color = black; //��ʱ��rclc��Ϊnode��rc����Ҫ����Ⱦ��
							}
							rc->rightchild->color = black; //�Һ�������ֱ��������������node��rc����ɫ����
							std::swap(node->color, rc->color);
							left_rotate(node);
							break; //����������޸�������ֱ���˳�
						}
					}
				}
			}
			else //��ɾ���ڵ���node�ұߣ��൱������if�ľ����������if�����е����ҵ�������
			{
				if (rc&&rc->color == red) //�����ɾ���ڵ����ڱ���ɫ�ڵ���棬ֱ������Ϊ��ɫ����
				{
					rc->color = black;
					break;
				}
				else //����ɾ���ڵ㱻��ɫ�ڵ��������
				{
					if (lc->color == red) //���lc�Ǻ�ɫ�ڵ㣬��Ҫ�������������ı���ɫ��Ȼ�������node����
					{
						right_rotate(node); //��ʱnode����rc�ĸ��ڵ㣬����node�ĸ��ڵ��Ѿ������lc��lc��Ӧ�������Բ�����������Ҫ�������Ҫ��lc����ѭ������������lc�Ǻ�ɫ�������ѭ��һ��һ�����˳�
						node->color = red;
						lc->color = black;
					}
					else //lc�Ǻ�ɫ�ڵ㣬����rc��lc�ĸ߶�С1����ʱlc�����Һ��ӱ�Ȼ���Է��ʣ���ʹ��NULL������ʱ���ж�lc�������ӵ���ɫ��
					{
						if (!(lc->leftchild&&lc->leftchild->color == red) && !(lc->rightchild&&lc->rightchild->color == red)) //���Ҷ��Ǻ�ɫ
						{
							lc->color = red;
							orit = (node == node->parent->leftchild) ? left : right; //node�ڵ�����Ӧ����������߶��½�1�������һ��ѭ����Ҫ�滻node�ڵ�Ϊ���ĸ��ڵ�
							if (node == visit_root()) //�����ʱnode�Ǹ��ڵ���ôȾ��ֱ���˳�ѭ�����ɣ�������ǣ���ô��Ҫ��һ������
							{
								node->color = black;
								break;
							}
							node = node->parent;
						}
						else //��������һ���Ǻ�ɫ
						{
							if ((lc->rightchild&&lc->rightchild->color == red) && !(lc->leftchild&&lc->leftchild->color == red)) //������Һ���ڵ��������Ҫ��һ����������Ȼ���ٴ���
							{
								left_rotate(rc);
								lc = node->leftchild;
								lc->color = black; //��ʱ��lcrc��Ϊnode��lc����Ҫ����Ⱦ��
							}
							lc->leftchild->color = black; //���������ֱ��������������node��lc����ɫ����
							std::swap(node->color, lc->color);
							right_rotate(node);
							break; //����������޸�������ֱ���˳�
						}
					}
				}
			}
		}
	}
};


/*
������Ĳ��Դ��루Ҳ��������AVL������
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