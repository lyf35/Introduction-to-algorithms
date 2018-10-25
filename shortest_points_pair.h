#pragma once
#include<iostream>
#include<cmath>
#include"vector.h"
#include<utility>
#include<vector>
#include<algorithm>

std::pair<point, point> calculate_shortest_distance(std::vector<point> pts) //���㼯�е�ĸ���С�ڵ�������ʱ��ֱ�����������
{
	if (pts.size() == 2)
	{
		return std::make_pair(pts[0], pts[1]);
	}
	else
	{
		auto i12 = distance(pts[0], pts[1]);
		auto i13 = distance(pts[0], pts[2]);
		auto i23 = distance(pts[1], pts[2]);
		if (i12 < i13)
		{
			if (i13 < i23)
			{
				return std::make_pair(pts[0], pts[1]);
			}
			else
			{
				if (i12 < i23)
				{
					return std::make_pair(pts[0], pts[1]);
				}
				else
				{
					return std::make_pair(pts[1], pts[2]);
				}
			}
		}
		else
		{
			if (i23 < i13)
			{
				return std::make_pair(pts[1], pts[2]);
			}
			else
			{
				return std::make_pair(pts[0], pts[2]);
			}
		}
	}
}

std::pair<point, point> shortest_points_pair(std::vector<point> pts) //����ŷ����þ������ƽ������������ԣ�Ҳ���Ի��������ľ��붨�壬����ҲҪͬʱ�޸ļ���������ʱ����жϾ���ĵ���
{
	sort(pts.begin(), pts.end(), [](point a, point b) {if (a.x < b.x) { return true; } else if (a.x == b.x&&a.y < b.y) { return true; } else { return false; }});//���պ������С������������������������С��ΪС
	int num_size = pts.size() / 2;
	std::vector<point> left(pts.begin(), pts.begin() + num_size), right(pts.begin() + num_size, pts.end());
	std::vector<point> left_sortby_x = left, right_sortby_x = right, left_sortby_y = left, right_sortby_y = right;
	sort(left_sortby_y.begin(), left_sortby_y.end(), [](point a, point b) {if (a.y < b.y) { return true; } else if (a.y == b.y&&a.x < b.x) { return true; } else { return false; }});
	sort(right_sortby_y.begin(), right_sortby_y.end(), [](point a, point b) {if (a.y < b.y) { return true; } else if (a.y == b.y&&a.x < b.x) { return true; } else { return false; }});
	std::pair<point, point> left_pts_pair, right_pts_pair, total_pts_pair;
	if (left.size() <= 3) //����˵ĵ㼯ʹ�÷��β��Դ���
	{
		left_pts_pair = calculate_shortest_distance(left);
	}
	else
	{
		left_pts_pair = shortest_points_pair(left);
	}
	if (right.size() <= 3) //���Ҷ˵㼯���÷��β��Դ���
	{
		right_pts_pair = calculate_shortest_distance(right);
	}
	else
	{
		right_pts_pair = shortest_points_pair(right);

	}
	total_pts_pair = distance(left_pts_pair.first, left_pts_pair.second) > distance(right_pts_pair.first, right_pts_pair.second) ? right_pts_pair : left_pts_pair; //ȡ�������˵���о����С��
	double dist = distance(total_pts_pair.first, total_pts_pair.second); //������С����
	if (distance(*(left_sortby_x.end() - 1), *(right_sortby_x.begin())) >= dist) //������㼯�����ҵ����Ҳ�㼯�����֮��ľ�������ҳ�����С���룬��ô����/�ҵ㼯���Ӵ��϶��������������������㼯���Ҿ���С�ڵ�ǰ��С���룬����ֱ�ӷ���
	{
		return total_pts_pair;
	}
	else
	{
		std::vector<point> le_dist;
		double mid = ((*(left_sortby_x.end() - 1)).x + (*(right_sortby_x.begin())).x) / 2; //�����������㼯�������ߣ�����һ����ѡ���ʱ��ֻ������Щ�������������˲�����dist��Χ�ڵĵ�
		auto i = left_sortby_y.begin();
		auto j = right_sortby_y.begin();
		while (i != left_sortby_y.end() && j != right_sortby_y.end()) //�ù鲢�������ҵ㼯��������������dist�����ڵĵ㰴������������
		{
			if ((*i).x < mid - dist)
			{
				i++;
				break;
			}
			if ((*j).x > mid + dist)
			{
				j++;
				break;
			}
			if ((*i).y < (*j).y)
			{
				le_dist.push_back(*i);
				i++;
			}
			else if ((*i).y == (*j).y && (*i).x < (*j).x)
			{
				le_dist.push_back(*i);
				i++;
			}
			else
			{
				le_dist.push_back(*j);
				j++;
			}
		}
		if (i == left_sortby_y.end())
		{
			while (j != right_sortby_y.end())
			{
				if ((*j).x <= mid + dist)
				{
					le_dist.push_back(*j);
				}
				j++;
			}
		}
		else
		{
			while (i != left_sortby_y.end())
			{
				if ((*i).x >= mid - dist)
				{
					le_dist.push_back(*i);
				}
				i++;
			}
		}
		for (auto ptr = le_dist.begin();ptr != le_dist.end();ptr++) //���м�������Ѱ�ң����ܷ��ҵ������С�ĵ��
		{
			for (int i = 1;i <= 5;i++) //���ڴ˴�������ŷ����þ��붨���µ���С���룬������ֻ��Ҫ����ĳ��������5���㼴��
			{
				if (ptr + i != le_dist.end())
				{
					if (distance(*(ptr), *(ptr + i)) < dist) //����ҵ������С�ĵ�ԣ������dist��total_dis_pair��ֵ
					{
						dist = distance(*(ptr), *(ptr + i));
						total_pts_pair = std::make_pair(*(ptr), *(ptr + i));
					}
				}
				else
				{
					break;
				}
			}
		}
		return total_pts_pair;
	}
}

/*
��������������
int main()
{
	std::vector<point> test_pts{ point(0,0),point(1,2),point(1,3),point(5,0),point(6,5),point(7,4),point(12,0),point(12.5,0),point(13,2),point(13,5),point(13,6.7),point(13,7.1),point(13,-5),point(13,-8),point(13,8),point(13,10),point(15,3),point(15,7),point(16,10),point(15,20),point(17,30) };
	auto i = shortest_points_pair(test_pts);
	std::cout << i.first.x << "," << i.first.y << "  " << i.second.x << "," << i.second.y << std::endl;
	return 0;
}
*/