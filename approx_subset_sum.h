#pragma once
#include<iostream>
#include<vector>
#include<set>

std::set<int> merge_lists(std::set<int> L1, int x)
{
	std::set<int> merged_list = L1; //�����Ȱ�L1�е�Ԫ��ֱ�Ӹ��Ƶ��ϲ��ļ�����
	for (auto i = L1.begin();i != L1.end();i++)
	{
		merged_list.insert(*i + x);
	}
	return merged_list;
}

void delete_elem(std::set<int> &L, int t) //ɾ�������д���t��Ԫ��
{
	auto i = L.begin();
	for (;i != L.end();i++)
	{
		if (*i > t)
		{
			break;
		}
	}
	L.erase(i, L.end());
}

int exact_subset_sum(std::vector<int> input, int sum) //Ѱ���Ƿ���ں͸պ�Ϊt��һ���Ӽ�����������ڣ��򷵻�С��t�����ֵ
{
	std::set<int> P{ 0 }; //��1-i��Ԫ�ؿ�����ɵĺ�����ɵļ���
	for (auto i = 0;i < input.size();i++)
	{
		auto temp = P;
		P = merge_lists(P, input[i]);
		delete_elem(P, sum);
	}
	int max = 0;
	for (auto i = P.begin();i != P.end();i++)
	{
		if (*i > max)
		{
			max = *i;
		}
	}
	return max;
}

std::set<int> trim(std::set<int> L, double delta) //���������õ�deltaֵ���Լ���������y ~ y*(1+delta)������ֵ����y����
{
	std::set<int> L_trim;
	auto iter = L.begin();
	int last = *iter;
	L_trim.insert(*iter);
	while (iter != L.end())
	{
		if ((double)*iter > (double)last*(1 + delta))
		{
			L_trim.insert(*iter);
			last = *iter;
		}
		iter++;
	}
	return L_trim;
}

int approx_subset_sum(std::vector<int> input, int sum, double approx) //һ��Ѱ���Ӽ�Ԫ�غͲ�����sum�Ľ����㷨��approx+1Ϊ���ƶ�
{
	std::set<int> P{ 0 }; //��1-i��Ԫ�ؿ�����ɵĺ�����ɵļ���
	for (auto i = 0;i < input.size();i++)
	{
		auto temp = P;
		P = merge_lists(P, input[i]);
		delete_elem(P, sum);
		double delta = approx / (2 * input.size());
		P = trim(P, delta);
	}
	int max = 0;
	for (auto i = P.begin();i != P.end();i++)
	{
		if (*i > max)
		{
			max = *i;
		}
	}
	return max;
}

/*
���Դ��룺����clrs P666��ʾ�����ԣ�
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"approx_subset_sum.h"

int main()
{
	std::vector<int> test{ 104,102,201,101 };
	std::cout << exact_subset_sum(test,308) << std::endl;
	std::cout << approx_subset_sum(test, 308, 0.4) << std::endl;
	return 0;
}
*/