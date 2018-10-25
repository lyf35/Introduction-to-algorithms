#pragma once
#include<iostream>
#include<cmath>
#include"vector.h"
#include<vector>
#include<tuple>
#include"RedBlackTree.h"
#include<algorithm>

bool operator<(std::tuple<point, int, bool> a, std::tuple<point, int, bool> b) //���߶ζ˵㰴�պ�����������������������ͬ����˵����ȣ����������/�Ҷ˵��Һ�������ͬ����������С������
{
	if (std::get<0>(a).x < std::get<0>(b).x)
	{
		return true;
	}
	else if (std::get<0>(a).x == std::get<0>(b).x&&std::get<2>(a) < std::get<2>(b))
	{
		return true;
	}
	else if (std::get<0>(a).x == std::get<0>(b).x&&std::get<2>(a) == std::get<2>(b) && std::get<0>(a).y < std::get<0>(b).y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool operator==(std::tuple<point, int, bool> a, std::tuple<point, int, bool> b) //������ڵĻ�����tuple��ÿһ�����ݶ����
{
	if (std::get<0>(a).x == std::get<0>(b).x&&std::get<0>(a).y == std::get<0>(b).y&&std::get<1>(a) == std::get<1>(b) && std::get<2>(a) == std::get<2>(b))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool operator>(std::tuple<point, int, bool> a, std::tuple<point, int, bool> b)
{
	return !((a < b)||(a==b));
}


bool any_segment_intersect(std::vector<point> left,std::vector<point> right) //�ֱ�洢�߶�i�����Ҷ˵�
{
	std::vector<std::tuple<point, int, bool>> pts;
	for (auto i = 0;i < left.size();i++)
	{
		pts.push_back(std::make_tuple(left[i], i, 0)); //tuple�ĵ�һ��Ԫ���ǵ㣬�ڶ���Ԫ�ش�����ԭ����Ӧ���߶α�ţ�������Ԫ�ش������Ҷ˵㣨0Ϊ��1Ϊ�ң�
		pts.push_back(std::make_tuple(right[i], i, 1));
	}
	sort(pts.begin(), pts.end()); //���߶ζ˵㰴�պ�����������������������ͬ����˵����ȣ����������/�Ҷ˵��Һ�������ͬ����������С������
	auto i = pts.begin();
	RBTree<std::tuple<point, int, bool>> T(*(i++)); //���ڱ��浱ǰɨ�����ϵ��߶�����
	while (i != pts.end())
	{
		if (std::get<2>(*i) == 0) //����ǰ����ĵ���ĳ���߶ε���˵�
		{
			T.insert_element(*i);
			if ((T.find_successor(*i) && segment_intersect(left[std::get<1>(*i)], right[std::get<1>(*i)], left[std::get<1>(T.find_successor(*i)->element)], right[std::get<1>(T.find_successor(*i)->element)]))
				|| (T.find_predecessor(*i) && segment_intersect(left[std::get<1>(*i)], right[std::get<1>(*i)], left[std::get<1>(T.find_predecessor(*i)->element)], right[std::get<1>(T.find_predecessor(*i)->element)])))
			{
				return true;
			}
		}
		else //����ǰ����ĵ���ĳ���߶ε��Ҷ˵�
		{
			if (T.find_successor(*i) && T.find_predecessor(*i) && segment_intersect(left[std::get<1>(T.find_successor(*i)->element)], right[std::get<1>(T.find_successor(*i)->element)], left[std::get<1>(T.find_predecessor(*i)->element)], right[std::get<1>(T.find_predecessor(*i)->element)]))
			{
				return true;
			}
			T.delete_element(*i);
		}
		i++;
	}
	return false;
}

/*
������������
int main() //�޽���
{
	std::vector<point> test_pts1{ point(0,0),point(1,2),point(1,3),point(5,0),point(6,5),point(7,4) };
	std::vector<point> test_pts2{ point(1,0),point(2,0),point(3,4),point(5,4),point(7,5),point(8,4) };
	std::cout << any_segment_intersect(test_pts1, test_pts2) << std::endl;
	return 0;
}

int main() //�н���
{
	std::vector<point> test_pts1{ point(0,0),point(1,2),point(1,1),point(5,0),point(6,5),point(7,4) };
	std::vector<point> test_pts2{ point(1,0),point(2,0),point(3,4),point(5,4),point(7,5),point(8,4) };
	std::cout << any_segment_intersect(test_pts1, test_pts2) << std::endl;
	return 0;
}
*/