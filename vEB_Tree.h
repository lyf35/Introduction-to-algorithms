#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<memory>

int up_root(double x)
{
	return pow(2, ceil(log(x) / log(2.0) / 2.0));
}
int down_root(double x)
{
	return pow(2, floor(log(x) / log(2.0) / 2.0));
}
int high(int x, int u)
{
	return floor(x / down_root(u));
}
int low(int x, int u)
{
	return x%down_root(u);
}
int index(int x, int y, int u)
{
	return x*down_root(u) + y;
}

struct vEB_node //�涨��veb_node����ֵ����-1�ı�������������
{
	int u;
	int min;
	int max;
	std::shared_ptr<vEB_node> summary;
	std::vector<std::shared_ptr<vEB_node>> cluster;
	vEB_node(int size, int minimum = -1, int maximum = -1) :u(size), min(minimum), max(maximum) //�˴�������u������2����
	{
		if (u != 2)
		{
			summary = std::shared_ptr<vEB_node>(new vEB_node(up_root(u)));
			cluster = std::vector<std::shared_ptr<vEB_node>>(up_root(u), nullptr);
			//ע�⣺�˴�������cluster = std::vector<std::shared_ptr<vEB_node>>(up_root(u), std::shared_ptr<vEB_node>(new vEB_node(down_root(u))))�����ĳ�ʼ����ʽ����Ϊ����д���൱���ǽ�vector��ÿ��λ�ö�����һ����ͬ��ָ�롣����veb����ʵ����cluster�е�ÿ��λ���ϴ洢��һ����ͬ����veb���
			for (auto i = 0;i < up_root(u);i++)
			{
				cluster[i] = std::shared_ptr<vEB_node>(new vEB_node(down_root(u)));
			}
		}
	}
};

class vEB_Tree
{
private:
	std::shared_ptr<vEB_node> root;
public:
	vEB_Tree(int n = 2) :root(new vEB_node(n)) {};
	std::shared_ptr<vEB_node> get_root()
	{
		return root;
	}
	int minimum(std::shared_ptr<vEB_node> n)
	{
		return n->min;
	}
	int maximum(std::shared_ptr<vEB_node> n)
	{
		return n->max;
	}
	bool tree_member(std::shared_ptr<vEB_node> n, int x)
	{
		if (x == n->max || x == n->min)
		{
			return true;
		}
		else if (n->u == 2)
		{
			return false;
		}
		else
		{
			return tree_member(n->cluster[high(x, n->u)], low(x, n->u));
		}
	}
	int predecessor(std::shared_ptr<vEB_node> n, int x) //ע�⣺x��һ���洢��veb����
	{
		if (n->u == 2) //�������
		{
			if (x == 1 && n->min == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else if (n->max != -1 && x > n->max) //������ʱ���뵽ĳ��veb�����У����е�maxֵ��x��С����ômaxֵ��Ϊx��ǰ��
		{
			return n->max;
		}
		else
		{
			int min_low = minimum(n->cluster[high(x, n->u)]); //����x��Ӧ���ڵ��Ǹ�cluster�����ҳ����cluster�е���Сֵ
			if (min_low != -1 && low(x, n->u) > min_low) //���ǰ����xӦ���ڵ��Ǹ�cluster��
			{
				int offset = predecessor(n->cluster[high(x, n->u)], low(x, n->u)); //����x���ڵ�cluster�����ݹ����
				return index(high(x, n->u), offset, n->u); //��high��low�����ֵת��Ϊ��Ӧ������ֵ
			}
			else //x������Ӧ���ڵ��Ǹ�cluster����Ҫ��ǰ�ҵ�һ���µ�cluster
			{
				int pred_cluster = predecessor(n->summary, high(x, n->u));
				if (pred_cluster == -1) //����Ҳ�����ǰ���Ǹ�cluster��Ҳ����˵x���ڵ��Ǹ�cluster֮ǰ������cluster��û��Ԫ�ش洢������
				{
					if (n->min != -1 && x > n->min) //���x�ȵ�ǰ����minֵҪ��˵����ǰ���洢��min����x��ǰ��
					{
						return n->min;
					}
					else //��ǰ���û��x��ǰ��Ԫ��
					{
						return -1;
					}
				}
				else //�ҵ�ǰ����cluster
				{
					int offset = maximum(n->cluster[pred_cluster]); //ֱ����ǰ������������ֵ
					return index(pred_cluster, offset, n->u); //����ǰ��ֵ
				}
			}
		}
	}
	int successor(std::shared_ptr<vEB_node> n, int x)
	{
		if (n->u == 2) //�������
		{
			if (x == 0 && n->max == 1)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if (n->min != -1 && x < n->min) //���x�ȵ�ǰ������Сֵ��ҪС����ֱ�ӷ��ص�ǰ�����Сֵ
		{
			return n->min;
		}
		else
		{
			int max_low = maximum(n->cluster[high(x, n->u)]);
			if (max_low != -1 && low(x, n->u) < max_low) //x�ȵ�ǰcluster�����ֵҪС�����һ������������
			{
				int offset = successor(n->cluster[high(x, n->u)], low(x, n->u));
				return index(high(x, n->u), offset, n->u);
			}
			else //��̲������cluster����Ҫ����ҵ�һ���µ�cluster
			{
				int succ_cluster = successor(n->summary, high(x, n->u));
				if (succ_cluster == -1) //�Ҳ�����̵�cluster
				{
					return -1;
				}
				else
				{
					int offset = minimum(n->cluster[succ_cluster]); //�ں�̵�cluster������Сֵ
					return index(succ_cluster, offset, n->u);
				}
			}
		}
	}
	void empty_insert(std::shared_ptr<vEB_node> n, int x)
	{
		n->max = x;
		n->min = x;
	}
	void insert_element(std::shared_ptr<vEB_node> n, int x)
	{
		if (n->min == -1) //���һ�����Ϊ�գ�ֱ�ӽ�x��Ϊ��Сֵ����
		{
			empty_insert(n, x);
		}
		else
		{
			if (x < n->min) //���x�ȵ�ǰ������Сֵ��ҪС������Ҫ��x�͵�ǰ����Сֵ���滻������ԭ������Сֵ�����ȥ
			{
				int temp = x;
				x = n->min;
				n->min = temp;
			}
			if (n->u > 2) //�����ǰ����uֵ����2����Ҫ�ҵ�x�ĺ���λ�ò���
			{
				if (minimum(n->cluster[high(x, n->u)]) != -1) //���xӦ�ڵĽ���Ƿǿս��
				{
					insert_element(n->cluster[high(x, n->u)], low(x, n->u));
				}
				else //x���ڽ��Ϊ��
				{
					insert_element(n->summary, high(x, n->u));
					empty_insert(n->cluster[high(x, n->u)], low(x, n->u));

				}
			}
			if (x > n->max) //�б�Ҫ�Ļ�����Ҫ���½���е�maxֵ
			{
				n->max = x;
			}
		}
	}
	void delete_element(std::shared_ptr<vEB_node> n, int x)
	{
		if (n->min == n->max) //ĳ�������min=max��˵���ý����ֻ����һ��Ԫ�أ�ֱ��ɾ������
		{
			n->min = -1;
			n->max = -1;
		}
		else if (n->u == 2) //�����min��max����ȣ����ҽ����ֻ��min��max
		{
			if (x == 0)
			{
				n->min = 1;
			}
			else
			{
				n->min = 0;
			}
			n->max = n->min;
		}
		else //����u����2��������ҽ���е�Ԫ�ز�ֹһ��
		{
			if (x == n->min) //���Ҫɾ����Ԫ�ظպ�����Сֵ����Ҫ�ȴӵ�һ��cluster���ҳ�һ��Ԫ���滻����ǰ��minֵ��Ȼ��ɾ����cluster�е��Ǹ���ͬԪ��
			{
				int first_cluster = minimum(n->summary);
				x = index(first_cluster, minimum(n->cluster[first_cluster]), n->u);
				n->min = x;
			}
			delete_element(n->cluster[high(x, n->u)], low(x, n->u));
			//ɾ������֮��Ҫ��һϵ������
			if (minimum(n->cluster[high(x, n->u)]) == -1) //���ɾ����ɺ�x����cluster��գ���Ҫ�޸���Ӧ��summary�е���ֵ
			{
				delete_element(n->summary, high(x, n->u));
				if (x == n->max) //���ɾ����Ԫ��x�պ��ǵ�ǰ�������Ԫ�أ���Ҫ�����ֵ�����¡����ڴ�ʱx���ڵ��Ǹ�cluster�Ѿ���գ������Ҫ����Ѱ��cluster���ҵ����ֵ
				{
					int summary_max = maximum(n->summary);
					if (summary_max == -1)
					{
						n->max = n->min;
					}
					else
					{
						n->max = index(summary_max, maximum(n->cluster[summary_max]), n->u);
					}
				}
			}
			else if (x == n->max) //x����cluster��Ϊ�գ���ʱ���ɾ����ֵ�������ֵ����ô��Ҫ�����ֵ������������ɾ��x֮��x���ڵ�cluster��Ϊ�գ��ʴ�ʱ���ֵ��������
			{
				n->max = index(high(x, n->u), maximum(n->cluster[high(x, n->u)]), n->u);
			}
		}
	}
};

/*
���Դ��룺
	vEB_Tree test(256);
	std::default_random_engine e(std::time(0));
	std::uniform_int_distribution<int> rand_gen(0, 255);
	for (int i = 1;i < 10;i++)
	{
		int t = rand_gen(e);
		std::cout <<"insert:"<< t << std::endl;
		test.insert_element(test.get_root(),t);
	}

	std::cout << "predecessor of 5: " << test.predecessor(test.get_root(), 5) << std::endl;
	std::cout << "successor of 8: " << test.successor(test.get_root(), 8) << std::endl;
	if (test.tree_member(test.get_root(), 9))
	{
		std::cout << "element 9 exists: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;

		test.delete_element(test.get_root(), 9);
		std::cout << "element 9 does not exist: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;
	}
	else
	{
		std::cout << "element 9 does not exist: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;
		
		test.insert_element(test.get_root(), 9);
		std::cout << "element 9 exists: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;

		test.delete_element(test.get_root(), 9);
		std::cout << "element 9 does not exist: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;

	}
	return 0;
*/