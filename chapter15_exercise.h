#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

//clrs exercise15.4-6
int find_pos(double elem, std::vector<int> pos, std::vector<double> a, int m, int n) //�������溯��longest_increment_sequence�Ķ��ַ���������
{
	while (n != m)
	{
		if (elem < a[pos[(m + n) / 2]])
		{
			n = (m + n) / 2;
		}
		else if (elem == a[pos[(m + n) / 2]])
		{
			return pos[(m + n) / 2];
		}
		else
		{
			m = (m + n) / 2 + 1;
		}
	}
	if (elem < a[pos[n]]) //��󷵻�pos�����в�����elem�����Ԫ��
	{
		return pos[n - 1];
	}
	else
	{
		return pos[m];
	}
}

std::vector<int> longest_increment_sequence(std::vector<double> a)
{
	std::vector<int> result(1,1); //�����ÿ��λ��Ԫ��Ϊ�������������е����һ��Ԫ��ʱ�����������еĳ���
	std::vector<int> element{ -1,0 }; //��ŵ��������еĳ���Ϊiʱ������Ӧ���Ǹ���С��Ԫ�ص�λ��
	for (int i = 1;i < a.size();i++)
	{
		int max = 0;
		if (find_pos(a[i], element, a, 1, element.size() - 1) >= 0)
		{
			max = result[find_pos(a[i], element, a, 1, element.size() - 1)] + 1; //��a[i]��Ϊ��β��������еĳ���
		}
		else
		{
			max = 1;
		}
		result.push_back(max);
		if (element.size() <= max) //���element�Ĵ�СС�ڻ����max��Ҳ����˵Ŀǰ������еĳ��ȱ�maxС������Ҫ��i����element��
		{
			element.push_back(i);
		}
		else //���element[max]��һλ������Ӧa�е�Ԫ�صĴ�С��a[i]����߶�����ȣ���ô��Ҫ��element�����Ӧλ�õ�Ԫ�����޸�
		{
			if (a[element[max]] >= a[i])
			{
				element[max] = i;
			}
		}
	}
	return result;
}
/*
���Դ��룺
int main()
{
	std::vector<double> a;
	uniform_int_distribution<int> rand(0, 10);
	default_random_engine e(time(0));
	for (int i = 0;i < 10;i++)
	{
		a.push_back(rand(e));
	}
	for (auto i : a)
	{
		std::cout << i << '\t';
	}
	std::cout << std::endl;
	auto c = longest_increment_sequence(a);
	for (auto i : c)
	{
		std::cout << i << '\t';
	}
	std::cout << std::endl;
	return 0;
}
*/