#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

std::tuple<double, std::vector<int>> cut_rod(int length, std::vector<double> price) //�Ե׶��ϵ�˼��
{
	std::vector<double> max_sheet(length + 1, 0);
	std::vector<int> cut_first(length + 1, 0);
	for (int i = 1;i <= length;i++)
	{
		for (int j = 1;j <= i;j++)
		{
			if (max_sheet[i] < price[j] + max_sheet[i - j])
			{
				max_sheet[i] = price[j] + max_sheet[i - j];
				cut_first[i] = j;
			}
		}
	}
	std::vector<int> cut_method;
	int count = length;
	while (count > 0)
	{
		cut_method.push_back(cut_first[count]);
		count -= cut_first[count];
	}
	return std::make_tuple(max_sheet[length], cut_method);
	/*
	���ֻ��max_sheet������ͨ�����µķ����������Ӧ���и����
	std::vector<int> cut_length;
	double count = length;
	while (count > 0)
	{
		int i = 1;
		while (i <= count)
		{
			if (price[i] + max_sheet[count - i] == max_sheet[count])
			{
				cut_length.push_back(i);
				count -= i;
				i = 1;
			}
			i++;
		}
	}
	*/
}

/*
���Դ��룺
std::vector<double> price{ 0,1,5,8,9,10,17,17,20,24,30 };
auto result = cut_rod(7, price);
std::cout << std::get<0>(result) << std::endl;
auto temp = std::get<1>(result);
for_each(temp.begin(), temp.end(), [](double x) {std::cout << x << '\t';});
std::cout << std::endl;
return 0;
*/