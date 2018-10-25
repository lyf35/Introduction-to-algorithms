#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<omp.h>

//һ�����е�ѡ���㷨������clrs9.3��������һ�������е�i�������i=1~n��
template<typename T> size_t partition(std::vector<T>& vec, int p, int r, T x) //�޸Ĺ��Ļ����㷨����x��ֵΪpivot���������p~rλ�ý��л���
{
	int i = p - 1;
	for (auto j = p;j <= r;j++)
	{
		if (vec[j] <= x)
		{
			i++;
			std::swap(vec[i], vec[j]);
		}
	}
	return i + 1; //����ֵΪpartition֮�����x�ĵ�һ������λ��
}

template<typename T> T find_median(std::vector<T> input, int i, int j) //Ѱ��i~j�е���λ����ʹ�ò������򼴿ɡ����⣬���j-i+1Ϊż��������ÿ�η�����λ���н�С��
{
	if (i == j)
	{
		return input[i];
	}
	for (int k = i + 1;k <= j;k++)
	{
		T temp = input[k];
		int m = k - 1;
		while (m >=i&&input[m] > temp)
		{
			input[m + 1] = input[m];
			m--;
		}
		input[m + 1] = temp;
	}
	return input[floor((i + j) / 2.0)];
}

template<typename T> T parallel_select(std::vector<T> input, int i, int j, int r)//����input�е�rС��Ԫ�أ�input��û���ź���
{
	int n = j - i + 1;
	if (j == i)
	{
		return input[i];
	}
	else
	{
		int size = ceil(double(n) / 5.0) ;
		std::vector<T> M(size);
#pragma omp parallel for
		for (int k = 0;k < size;k++)
		{
			M[k] = find_median(input, k * 5 + i, i + (k + 1) * 5 - 1 >= n - 1 + i ? n - 1 + i : i + (k + 1) * 5 - 1);
		}
		T x = parallel_select(M, 0, size - 1, size / 2); //Ѱ��M�е���λ��
		int k = partition(input, i, j, x); //����x�Ĵ�С����ԭ���������֣�����ֵΪi,j�д���x�ĵ�һ������λ�ã���������xֵ����������������������
		if (k == r)
		{
			return x;
		}
		else
		{
			if (k > r)
			{
				return parallel_select(input, i, k - 1, r);
			}
			else
			{
				return parallel_select(input, k, j, r);
			}
		}
	}
}


/*
���Դ��룺
#include<iostream>
#include<vector>
#include"parallel_select.h"
#include<random>
#include<ctime>

int main()
{
	std::vector<double> input;
	std::uniform_real_distribution<double> rand;
	std::default_random_engine e(time(0));
	for (int i = 0;i <10000;i++)
	{
		input.push_back(rand(e));
	}
	auto i = parallel_select(input, 0, 9999, 100);
	std::cout << i << std::endl;
	return 0;
}
*/