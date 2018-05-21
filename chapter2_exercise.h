#pragma once
#include<iostream>
#include<vector>
#include"merge_sort.h"

void find_elements(std::vector<double> vec, double num)//exercise2.3-7����vec��Ѱ��������ӵ���num����ϡ�����֮ǰ�����鲢�����ü����е�Ԫ�ذ�����������
{
	int i = 0;
	int cnt = 0;
	while (i < vec.size() - 1 && vec[i] + vec[i + 1] <= num)//ע��&&�ж������ļ���˳���ұ߲��ַ�����ߵ�ʱ������ȼ���Ӷ�����Խ��
	{
		double temp = num - vec[i];
		auto x = my_binary_search(vec.begin() + i, vec.end(), temp);
		if (x != vec.end())
		{
			if (*x == temp)
			{
				std::cout << "The elements are " << vec[i] << " and " << *x << std::endl;
				cnt++;
			}
		}
		i++;
	}
	if (cnt == 0)
	{
		std::cout << "No element found." << std::endl;
	}
}