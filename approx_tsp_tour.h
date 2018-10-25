#pragma once
#include<iostream>
#include"graph.h"

template<typename T, typename W> void approx_tsp_tour(graph<T, W> G) //���Ƶ��������㷨��Ҫ���֮��ľ����������ǲ���ʽ�����ƶ�Ϊ2
{
	auto mst_tree = G.MST_Prim(); //�ȼ���ͼ��һ����С������
	MST_preorder_traversal(mst_tree, 0, -1);
	std::cout << std::endl;
}

template<typename W> void MST_preorder_traversal(std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> M, int i, int p) //MΪMST��Ӧ���ڽӾ���i����ǰ���ڽ�㡣Ϊ�˱���������������ʱ������������㣬����һ������p��ʾ��ǰ�������ĸ���㡣����Ǹ��ڵ㣬���Խ�p����Ϊ-1���������ܷ��ʽ��i��������������MST�Ľ�㰴��ǰ�������˳�����
{
	std::cout << i << " ";
	for (int j = 0;j < M.size();j++)
	{
		if (M[i][j] && j != p)
		{
			MST_preorder_traversal(M, j, i);
		}
	}
}

/*
���Դ��루clrsͼ35-2��
#include<iostream>
#include<vector>
#include"graph.h"
#include"approx_tsp_tour.h"
#include"vector.h"

int main()
{
	graph<point, double> test;
	test.insert_vertex(point(1,4));
	test.insert_vertex(point(1,2));
	test.insert_vertex(point(0,1));
	test.insert_vertex(point(3,4));
	test.insert_vertex(point(4,3));
	test.insert_vertex(point(3,2));
	test.insert_vertex(point(5,2));
	test.insert_vertex(point(2,0));
	for (auto i = 0;i < test.get_size();i++)
	{
		for (auto j = 0;j < i;j++)
		{
			test.insert_edge(i, j, double(), distance(test.get_vertex(i).key, test.get_vertex(j).key));
		}
	}
	approx_tsp_tour(test);
	return 0;
}
*/