#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>> matrix_chain_order(std::vector<int> size_table) //size_table�еĵ�i-1�͵�i��Ԫ����ɾ���i
{
	std::vector<std::vector<int>> part_pos(size_table.size(), std::vector<int>(size_table.size(), 0)); //�洢�������зָ�����λ�ã�������һλ�ý�Ԫ�ط�Ϊ������㣬�������һ��Ԫ��һֱ�����Ϊһ�飬��һ��Ԫ�ص����Ԫ��Ϊ��һ�飩
	std::vector<std::vector<int>> cal_multi(size_table.size(), std::vector<int>(size_table.size(), 0)); //�洢����i~j����������С���㲽�裨�����Ǿ���
	for (int i = 1;i < size_table.size();i++) //i��������ֽ�Ϊ��СΪ2~n��������
	{
		for (int j = 1;j < size_table.size() - i;j++) //�������ӵ�j��Ԫ�ؿ�ʼ��������
		{
			int min = INT_MAX;
			for (int k = j;k < j + i;k++) //�������ֿ������λ��
			{
				if (min > cal_multi[j][k] + size_table[j - 1] * size_table[k] * size_table[j + i] + cal_multi[k + 1][j + i]) //�˴��ļ��㹫ʽҪע���м䲿�֣��м䲿�ֵļ��㹫ʽҪ������������Ĵ�С��ȷ��Ӧ����
				{
					min = cal_multi[j][k] + size_table[j - 1] * size_table[k] * size_table[j + i] + cal_multi[k + 1][j + i];
					cal_multi[j][j + i] = min;
					part_pos[j][j + i] = k;
				}
			}
		}
	}
	return std::make_tuple(cal_multi, part_pos);
}

std::vector<std::vector<double>> operator*(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	std::vector<std::vector<double>> c(a.size(), std::vector<double>(b.begin()->size(), 0));
	for (int i = 0;i < c.size();i++)
	{
		for (int j = 0;j < c.begin()->size();j++)
		{
			for (int k = 0;k < a.begin()->size();k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return c;
}

std::vector<std::vector<double>> matrix_chain_multiply(std::vector<std::vector<std::vector<double>>> matrix_seq, std::vector<std::vector<int>> part_pos, int m, int n)
{
	if (m + 1 == n)
	{
		return matrix_seq[m - 1]*matrix_seq[m];
	}
	else if (m == n)
	{
		return matrix_seq[m - 1];
	}
	else
	{
		return matrix_chain_multiply(matrix_seq, part_pos, m, part_pos[m][n])*matrix_chain_multiply(matrix_seq, part_pos, part_pos[m][n] + 1, n);
	}
}

/*
���Դ��룺
	std::vector<int> matrixsize = { 30,35,15,5,10,20,25 };
	auto result = matrix_chain_order(matrixsize);
	auto m = std::get<0>(result);
	auto n = std::get<1>(result);
	for (auto i : m)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	for (auto i : n)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
*/