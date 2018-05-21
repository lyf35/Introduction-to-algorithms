#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

std::vector<std::vector<int>> solve_longest_common_subsequence(std::string a, std::string b)
{
	std::vector<std::vector<int>> result(a.size() + 1, std::vector<int>(b.size() + 1, 0));
	for (int i = 1;i <= a.size();i++)
	{
		for (int j = 1;j <= b.size();j++)
		{
			if (a[i - 1] == b[j - 1])
			{
				result[i][j] = result[i - 1][j - 1] + 1;
			}
			else
			{
				result[i][j] = result[i - 1][j] > result[i][j - 1] ? result[i - 1][j] : result[i][j - 1];
			}
		}
	}
	return result;
}

void print_longest_common_subsequence(std::vector<std::vector<int>> lcs_table, std::string a)
{
	std::string result;
	int j = lcs_table.begin()->size() - 1;
	int i = lcs_table.size() - 1;
	while (lcs_table[i][j] > 0)
	{
		if (lcs_table[i][j] == lcs_table[i - 1][j - 1] + 1 && lcs_table[i][j] > lcs_table[i][j - 1] && lcs_table[i][j] > lcs_table[i - 1][j])
		{
			result += a[i - 1]; //ע��lcs_table�Ǵ�1��ʼ����string��Ӧ��0λ��Ԫ�ص�
			i--;
			j--;
		}
		else if (lcs_table[i][j] == lcs_table[i - 1][j])//�����Ψһ���˴�������������
		{
			i--;
		}
		else if (lcs_table[i][j] == lcs_table[i][j - 1])//�������������ɹ�
		{
			j--;
		}
	}
	for (int i = result.size() - 1;i >= 0;i--)
	{
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
}
