#pragma once
#include<set>
#include<vector>

template<typename T> std::vector<bool> greedy_set_cover(std::vector<std::set<T>> subset) //����̰���㷨��ÿ��ѡ��δ���ǵ������Ӽ�
{
	std::vector<bool> set_choice(subset.size(), 0);
	bool is_set_exist = 1; //���ڱ���Ӽ����Ƿ���δ���ǵ�
	while (is_set_exist)
	{
		int max_set = 0; //������󼯺ϵı��
		for (int i = 0;i < subset.size();i++) //��ѡ������δ���ǵ����ĵ㼯
		{
			if (subset[i].size()>subset[max_set].size())
			{
				max_set = i;
			}
		}
		set_choice[max_set] = 1;
		for (auto iter = subset[max_set].begin();iter != subset[max_set].end();iter++) //ɾ������ѡ��ĵ�
		{
			for (int i = 0;i < subset.size();i++)
			{
				if (i != max_set)
				{
					subset[i].erase(*iter);
				}
			}
		}
		subset[max_set].clear();
		int cnt = 0;
		for (int i = 0;i < subset.size();i++) //ͳ�Ƽ����пռ��ĸ�����������еĶ�Ϊ�ռ�����Խ���whileѭ��
		{
			if (subset[i].size() == 0)
			{
				cnt++;
			}
		}
		if (cnt == subset.size())
		{
			is_set_exist = 0;
		}
	}
	return set_choice;
}

/*
���Դ��루clrsͼ35-3����
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"greedy_set_cover.h"

int main()
{
	std::vector<std::set<int>> test{ {1,2,3,4,5,6},{5,6,8,9},{1,4,7,10},{2,5,7,8,11},{3,6,9,12},{10,11} };
	auto result = greedy_set_cover(test);
	for (auto i = 0;i < result.size();i++)
	{
		if (result[i])
		{
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
	return 0;
}

*/