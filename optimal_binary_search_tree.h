#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

std::vector<std::vector<int>> optimal_binary_search_tree(std::vector<double> p, std::vector<double> q)
{
	std::vector<std::vector<double>> e(q.size() + 1, std::vector<double>(p.size() + 1, 0));
	std::vector<std::vector<double>> w(q.size() + 1, std::vector<double>(p.size() + 1, 0));
	std::vector<std::vector<int>> root(q.size() + 1, std::vector<int>(p.size() + 1, 0));
	for (int i = 1;i <= p.size() + 1;i++)
	{
		e[i][i - 1] = q[i - 1];
		w[i][i - 1] = q[i - 1];
	}
	for (int i = 0;i < p.size();i++) //�Ӽ��Ϊ0��ʼ���𣬴˴������������˷������Ķ�̬�滮�㷨
	{
		for (int m = 1;m <= p.size() - i;m++)
		{
			int min_pos = 0;
			double min = DBL_MAX;
			w[m][m + i] = w[m][m + i - 1] + p[m + i - 1] + q[m + i];
			for (int n = m;n <= m + i;n++)
			{
				if (min >(e[m][n - 1] + e[n + 1][m + i] + w[m][m + i]))
				{
					min = e[m][n - 1] + e[n + 1][m + i] + w[m][m + i];
					min_pos = n;
				}
			}
			e[m][m + i] = min;
			root[m][m + i] = min_pos;
		}
	}
	return root;
}

//clrs 15.5-4 �޸İ�����Ŷ���������
std::vector<std::vector<int>> optimal_binary_search_tree_mod(std::vector<double> p, std::vector<double> q)
{
	std::vector<std::vector<double>> e(q.size() + 1, std::vector<double>(p.size() + 1, 0));
	std::vector<std::vector<double>> w(q.size() + 1, std::vector<double>(p.size() + 1, 0));
	std::vector<std::vector<int>> root(q.size() + 1, std::vector<int>(p.size() + 1, 0));
	for (int i = 1;i <= p.size() + 1;i++)
	{
		e[i][i - 1] = q[i - 1];
		w[i][i - 1] = q[i - 1];
	}
	for (int i = 0;i < p.size();i++) //�Ӽ��Ϊ0��ʼ���𣬴˴������������˷������Ķ�̬�滮�㷨
	{
		for (int m = 1;m <= p.size() - i;m++)
		{
			int min_pos = 0;
			double min = DBL_MAX;
			w[m][m + i] = w[m][m + i - 1] + p[m + i - 1] + q[m + i];
			if (i == 0)  //���������κ�1<=i<j<=n��������㣺root[i][j-1]<=root[i][j]<=root[i+1][j]��������һ���Լ��ټ�����
			{
				root[m][m] = m - 1;
				e[m][m] = e[m][m - 1] + e[m + 1][m] + w[m][m];
			}
			else
			{
				for (int n = root[m][m + i - 1] + 1;n <= root[m + 1][m + i] + 1;n++)
				{
					if (min >(e[m][n - 1] + e[n + 1][m + i] + w[m][m + i]))
					{
						min = e[m][n - 1] + e[n + 1][m + i] + w[m][m + i];
						min_pos = n;
					}
				}
				e[m][m + i] = min;
				root[m][m + i] = min_pos - 1;
			}
		}
	}
	return root;
}