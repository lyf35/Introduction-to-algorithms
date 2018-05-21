#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<deque>
#include<memory>
#include<tuple>
#include<string>

template<typename T>struct BTreeNode
{
	std::shared_ptr<BTreeNode<T>> parent;
	std::vector<std::shared_ptr<BTreeNode<T>>> children;
	std::vector<T> element;
	bool leaf;
	BTreeNode(std::shared_ptr<BTreeNode<T>> p, std::vector<std::shared_ptr<BTreeNode<T>>> c, std::vector<T> elem, bool l) :parent(p), children(c), element(elem), leaf(l) {};
};

template<typename T> class BTree
{
private:
	std::shared_ptr<BTreeNode<T>> root;
	unsigned height;
	unsigned degree;
public:
	BTree(unsigned deg = 2) :degree(deg)
	{
		root = std::shared_ptr<BTreeNode<T>>(new BTreeNode<T>(NULL, std::vector<std::shared_ptr<BTreeNode<T>>>(), std::vector<T>(), true));
		height = 0;
	}
	std::tuple<std::shared_ptr<BTreeNode<T>>, int> find_node(T elem)//��������������ڵ��Ǹ�����λ��
	{
		auto node = this->root;
		while (true) //�Ӹ����һֱ����
		{
			for (int i = 0;i < node->element.size();i++)
			{
				if (node->element[i] == elem)
				{
					return std::make_tuple(node, i);
				}
				if (node->element[i] > elem)
				{
					if (node->leaf == true)
					{
						return std::make_tuple(node, i);
					}
					else
					{
						node = node->children[i];
						break;
					}
				}
			}
			if (*(node->element.end() - 1) < elem)
			{
				if (node->leaf == true)
				{
					return std::make_tuple(node, node->element.size());
				}
				else
				{
					node = *(node->children.end() - 1);
				}
			}
		}
	}
	std::tuple<std::shared_ptr<BTreeNode<T>>,int> split_node(std::shared_ptr<BTreeNode<T>> node) //��ĳ������������������ֵ֮һ�Ƿ�����ĸ���㣬��һ�������������������Ԫ�ص�λ��
	{
		if (node->element.size() == 2 * degree - 1)
		{
			std::vector<std::shared_ptr<BTreeNode<T>>> tempvecnode;
			if (node->leaf == false)
			{
				tempvecnode.insert(tempvecnode.end(), node->children.begin() + degree, node->children.end());
			}
			std::vector<T> tempvecelem(node->element.begin() + degree, node->element.end());
			std::shared_ptr<BTreeNode<T>> newnode(new BTreeNode<T>(node->parent, tempvecnode, tempvecelem, node->leaf)); //��node�ĺ�һ�뺢�ӷָ����ѳ����º���
			if (tempvecnode.size() > 0) //ע�⣬��node�ĺ�һ�뺢�ӷֳ�ȥ֮��Ҳ��Ҫ��Ӧ���޸���Щ���ӵĸ��ڵ�
			{
				for (auto i : tempvecnode)
				{
					i->parent = newnode;
				}
			}
			std::shared_ptr<BTreeNode<T>> node_p; //��ǰ���ĸ����
			if (node->parent) //�����ǰҪ���ѵĽ�㲻�Ǹ���㣬����ֱ�Ӵ���������Ҫ������һ�������Ϊ����㣬���Ҹ߶�+1
			{
				node_p = node->parent;
			}
			else
			{
				std::vector<std::shared_ptr<BTreeNode<T>>> tempvecnode2{node};
				std::vector<T> tempvecelem2;
				node_p = std::shared_ptr<BTreeNode<T>>(new BTreeNode<T>(NULL, tempvecnode2, tempvecelem2, false));
				root = node_p;
				node->parent = node_p;
				newnode->parent = node_p;
				height++;
			}
			for (auto i = node_p->element.begin();i <= node_p->element.end();i++) //��������м���Ǹ�Ԫ�ز��뵽�������ȥ���������º���newnode
			{
				if (i == node_p->element.end() || (*i) > node->element[degree - 1]) //�˴���Ҫע�����������Ҷ�ʱ�����
				{
					i = node_p->element.insert(i, node->element[degree - 1]); //ע����һ����������֮ǰ�ı���i�Ѿ�ʧЧ����˱������¸�ֵ��
					node_p->children.insert(node_p->children.begin() + (i - node_p->element.begin()) + 1, newnode);
					node->element.erase(node->element.begin() + degree - 1, node->element.end());
					if (node->leaf==false) //ע�⣺�����ѵĽ�������Ҷ��㣬��ʱû�к���
					{
						node->children.erase(node->children.begin() + degree, node->children.end());
					}
					int rank = i - node_p->element.begin();
					return std::make_tuple(node_p, rank); 
				}
			}
		}
		else
		{
			return std::make_tuple(node, 0);
		}
	}
	std::shared_ptr<BTreeNode<T>> BTree_insert(T elem) //��ĳ��Ԫ�ز��뵽B����
	{
		auto node = this->root;
		while (true) //�Ӹ����һֱ����
		{
			if (node->element.size() == 2 * degree - 1) //��鵱ǰ����Ƿ���Ҫ����
			{
				auto temp = split_node(node);
				node = std::get<0>(temp);
				if (node->element[std::get<1>(temp)] > elem) //���ݴ�С��ϵ�������������ĸ����ӿ�ʼ����
				{
					node = node->children[std::get<1>(temp)];
				}
				else
				{
					node = node->children[std::get<1>(temp) + 1];
				}
			}
			if (node->leaf == false) //�������Ҷ��㣬��Ѱ����Ҫ��һ�������������Ǹ�����
			{
				for (int i = 0;i < node->element.size();i++)
				{
					if (node->element[i] > elem)
					{
						node = node->children[i];
						break;
					}
				}
				if (*(node->element.end() - 1) < elem)
				{
					node = *(node->children.end() - 1);
				}
			}
			else if (node->leaf == true) //�����Ҷ�ڵ㣬���ں��ʵ�λ�ò��룬������
			{
				int i = 0;
				while (i < node->element.size() && node->element[i] < elem)
				{
					i++;
				}
				node->element.insert(node->element.begin() + i, elem);
				return node;
			}
		}
	}
	std::shared_ptr<BTreeNode<T>> merge_node(std::shared_ptr<BTreeNode<T>> node1, std::shared_ptr<BTreeNode<T>> node2, int m)//���ĺϲ�������m����node1��node2�ڸ�����У��м������Ǹ�Ԫ�ص��ȣ���node2λ��node1�ұ�
	{
		auto node_p = node1->parent;
		node1->element.push_back(node_p->element[m]);
		node1->element.insert(node1->element.end(), node2->element.begin(), node2->element.end());
		if (node1->leaf == false)
		{
			node1->children.insert(node1->children.end(), node2->children.begin(), node2->children.end());
			for (auto i : node1->children) //Ҫ�޸�node2���к��ӵĸ���ָ�룬ʹ��ָ��node1
			{
				i->parent = node1;
			}
		}
		node_p->element.erase(node_p->element.begin() + m); //ɾȥ���ڵ��е���ӦԪ��
		node_p->children.erase(node_p->children.begin() + m + 1); //node2��node1�ұߣ�ɾ��node2
		if (node_p->element.size() == 0) //����ϲ����֮���������������
		{
			node1->parent = NULL;
			root = node1;
			height--;
		}
		node2->parent = NULL;
		if (height == 0)
		{
			node1->leaf = true;
		}
		return node1;
	}
	void delete_element(T elem) //ɾ��ĳ��Ԫ��
	{
		auto node = root;
		int rank = -1;
		while (rank < 0) //�Ӹ����һֱ���£���Ѱ��Ԫ�����ڽ���Լ���λ��
		{
			if (node != root&&node->element.size() < degree) //�ڲ��ҵ�ǰ�ڵ�ʱ������ڵ��е�Ԫ�ظ������٣�Ҫ�ȵ������Ա�֤��������ɾ������˳������
			{
				node = delete_fix(node);
			}
			for (int i = 0;i < node->element.size();i++)
			{
				if (node->element[i] == elem) //���ҳɹ���rank�����ɾ��Ԫ���ڵ�ǰ����е�λ��
				{
					rank = i;
					break;
				}
				if (node->element[i] > elem)
				{
					if (node->leaf == true)
					{
						return;
					}
					else
					{
						node = node->children[i];
						break;
					}
				}
			}
			if (*(node->element.end() - 1) < elem)
			{
				if (node->leaf == true)
				{
					return;
				}
				else
				{
					node = *(node->children.end() - 1);
				}
			}
		}
		while (node->leaf == false) //���Ԫ�ز���Ҷ����У�����Ҫ���������
		{
			auto pred = node->children[rank];
			auto succ = node->children[rank + 1];
			if (pred->element.size() >= degree)//���ǰ���ӽڵ��Ԫ�ظ����������Ķȣ��򽫴�ɾ��Ԫ����ǰһ���ӽڵ��е����һ��Ԫ��������
			{
				node->element[rank] = *(pred->element.end() - 1);
				*(pred->element.end() - 1) = elem;
				rank = pred->element.size() - 1;
				node = pred;
			}
			else if (succ->element.size() >= degree) //������ӽڵ��Ԫ�ظ����Ƿ�������Ķȣ�����ǣ��򽫴�ɾ��Ԫ�������ӽڵ�ĵ�һ��Ԫ��������
			{
				node->element[rank] = *(succ->element.begin());
				*(succ->element.begin()) = elem;
				rank = 0;
				node = succ;
			}
			else //���ǰ���ͺ�������ڵ��е�Ԫ�ظ��������٣����Ⱥϲ���ɾ������ֱ�ӽ���
			{
				auto temp = merge_node(pred, succ, rank);
				temp->element.erase(temp->element.begin() + degree - 1);
				return;
			}
		}
		node->element.erase(node->element.begin() + rank); //��ʱ����ɾ��Ԫ���Ѿ���ΪҶ����е�Ԫ�أ�����ֱ��ɾ��
		if (node != root&&node->element.size() < degree - 1) //���ڷǸ��ڵ���˵�����½�ʱ���Ѿ���֤���ڵ��Ԫ�ظ����㹻�����������������������ϴ�������˿���ֱ������
		{
			delete_fix(node);
		}
	}
	std::shared_ptr<BTreeNode<T>> delete_fix(std::shared_ptr<BTreeNode<T>> node)
	{
		auto node_p = node->parent;
		T tempelem = node->element[0];
		int r = 0;
		while (r < node_p->element.size()&&node_p->element[r] < tempelem)
		{
			r++;
		}
		if (r == 0) //node�����ǵ�һ�����ӵ����
		{
			auto next = node_p->children[r + 1];
			if (next->element.size() >= degree) //���Ҳ��ֵܽ�һ���ؼ��ֹ�����ͬʱҲҪ�ǵðѺ��ӽڵ�ת�ƹ���
			{
				node->element.push_back(node_p->element[r]);
				node_p->element[r] = *(next->element.begin());
				next->element.erase(next->element.begin());
				if (node->children.size() > 0)
				{
					node->children.push_back(next->children[0]);
					next->children.erase(next->children.begin());
				}
			}
			else //���ֵܽ���Ԫ�������Ѿ��ﵽ��Сֵ���޷��ٽ����ֻ�ܺϲ�
			{
				return merge_node(node, next, r);
			}
		}
		else
		{
			auto prev = node_p->children[r - 1];
			if (prev->element.size() >= degree) //�������ֵܽ�һ���ؼ��ֹ���
			{
				node->element.insert(node->element.begin(), node_p->element[r - 1]);
				node_p->element[r - 1] = *(prev->element.end() - 1);
				prev->element.pop_back();
				if (node->children.size() > 0)
				{
					node->children.insert(node->children.begin(), prev->children.end() - 1, prev->children.end());
					prev->children.pop_back();
				}
			}
			else if (r < node_p->element.size()) //���Ҳ��ֵܽ�һ���ؼ��ֹ���
			{
				auto next = node_p->children[r + 1];
				if (next->element.size() >= degree)
				{
					node->element.push_back(node_p->element[r]);
					node_p->element[r] = *(next->element.begin());
					next->element.erase(next->element.begin());
					if (node->children.size() > 0)
					{
						node->children.push_back(next->children[0]);
						next->children.erase(next->children.begin());
					}
				}
				else //�����ֵܽ���Ԫ�������Ѿ��ﵽ��Сֵ���޷��ٽ����ֻ��ѡ������/���ֵܺϲ�
				{
					return merge_node(prev, node, r - 1);
				}
			}
			else //û�����ֵܣ�ֻ��ѡ�������ֵܺϲ�
			{
				return merge_node(prev, node, r - 1);
			}
		}
		return node;
	}
	template<typename U> void level_traverse(U op) //��α�������
	{
		std::deque<std::shared_ptr<BTreeNode<T>>> layer1{ root };
		std::deque<std::shared_ptr<BTreeNode<T>>> layer2;
		bool is_layer1 = true;
		while (layer1.size() > 0 || layer2.size() > 0)
		{
			if (is_layer1)
			{
				for (auto i = layer1.begin();i != layer1.end();i++)
				{
					op(*i);
					if ((*i)->leaf == false)
					{
						layer2.insert(layer2.end(), (*i)->children.begin(), (*i)->children.end());
					}
				}
				layer1.clear();
				std::cout << std::endl;
				is_layer1 = false;
			}
			else
			{
				for (auto i = layer2.begin();i != layer2.end();i++)
				{
					op(*i);
					if ((*i)->leaf == false)
					{
						layer1.insert(layer1.end(), (*i)->children.begin(), (*i)->children.end());
					}
				}
				layer2.clear();
				std::cout << std::endl;
				is_layer1 = true;
			}
		}	
	}
};

/*
�����ô��룺
template<typename T> struct visit
{
	void operator()(T obj)
	{
		for (auto i : obj->element)
		{
			std::cout << i << " ";
		}
		std::cout << "|";
	}
};

int main()
{
	visit<std::shared_ptr<BTreeNode<int>>> myvisit;
	BTree<int> test(4);
	for (int i = 0;i < 50;i++)
	{
		test.BTree_insert(i);
		test.level_traverse(myvisit);
		std::cout << std::endl;
	}
	for (int i = 0;i < 15;i ++)
	{
		static std::uniform_int_distribution<int> eng(0,29);
		static std::default_random_engine e(time(0));
		int m = eng(e);
		std::cout << "delete" << m << std::endl;
		test.delete_element(m);
		test.level_traverse(myvisit);
		std::cout << std::endl;
	}
	return 0;
}
*/