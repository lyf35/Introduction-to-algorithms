#pragma once
#include<iostream>
#include<cmath>
#include"vector.h"
#include<vector>
#include<stack>
#include<algorithm>

std::vector<point> graham_scan(std::vector<point> pts)
{
	std::vector<point> convex_point;
	std::sort(pts.begin(), pts.end(), [](point a, point b) {if (a.y < b.y) { return true; } else if (a.y == b.y&&a.x < b.x) { return true; } else { return false; }}); //����y��������еĵ�����������yֵ��ȣ���ȡx��С��ΪС
	auto pivot = *pts.begin(); //�㼯�еĵ�һ����һ����͹���еĵ㣬Ȼ��Ҫ�������Ϊ��Ŧ�㣬��ʣ�µĵ㰴�ռ��ǵ���Դ�С����������Ǵ�Сһ������ȡxֵ��С��Ϊ��С��
	pts.erase(pts.begin());
	std::sort(pts.begin(), pts.end(), [pivot](point a, point b) {if (cross_product(vector(pivot, a), vector(pivot, b)) > 0) { return true; } else if (cross_product(vector(pivot, a), vector(pivot, b)) == 0) { return a.x < b.x; } else { return false; }}); 
	convex_point.push_back(pivot);
	auto i = pts.begin();
	auto pts_top_down = *(i++);
	auto pts_top = *(i++); //�ֱ����ջ��Ԫ�غ�ջ������һ��Ԫ��
	convex_point.push_back(pts_top_down);
	convex_point.push_back(pts_top); //�Ƚ�ǰ����Ԫ������ջ��
	while (i != pts.end())
	{
		while(cross_product(vector(pts_top, pts_top_down), vector(pts_top, *i)) > 0) //��pivot��ʼ��͹����Ӧ�Ķ����Ӧ��һֱ��������ʱ���ߣ��������˳ʱ����������˵���а���ȥ�ıߣ���ô�϶����ԣ���Ҫ����㵯��������ѭ��ֱ������Ҫ��Ϊֹ
		{
			convex_point.pop_back();
			pts_top = *(convex_point.end() - 1);
			pts_top_down = *(convex_point.end() - 2);
		}
		pts_top_down = pts_top;
		pts_top = *i;
		convex_point.push_back(pts_top);
		i++;
	}
	return convex_point;
}

/*
��������������
int main()
{
	std::vector<point> test_pts{ point(0,0),point(1,0),point(7,0),point(7,3),point(7,5),point(6,7),point(1,8),point(1,4),point(2,3),point(3,3),point(4,6),point(4,7) };
	auto result = graham_scan(test_pts);
	for (auto i : result)
	{
		std::cout << i.x << '\t' << i.y << std::endl;
	}
	return 0;
}
*/