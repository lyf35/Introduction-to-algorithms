#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<random>
#include<ctime>
#include"quicksort_increment.h"

//hoare�����㷨������š�����˼·����vec[p]Ϊ���Ԫ�أ���p��r�������м�������ֱ�����˵�������������
template<typename T> int hoare_partition(std::vector<T>& vec, int p, int r)
{
	T x = vec[p];
	int i = p;
	int j = r;
	while (1)
	{
		while (vec[j] > x)
		{
			j--;
		}
		while (vec[i] < x)
		{
			i++;
		}
		if (i < j)
		{
			std::swap(vec[i], vec[j]);
		}
		else
		{
			return j;
		}
	}
}

template<typename T> std::vector<T> hoare_quicksort(std::vector<T>& vec, int p, int r)
{
	if (p < r)
	{
		int q = hoare_partition(vec, p, r);
		hoare_quicksort(vec, p, q - 1);
		hoare_quicksort(vec, q + 1, r);
	}
	return vec;
}

//β�ݹ�������򡣽�ʡ��ջ�ռ�
template<typename T> std::vector<T> tail_recursive_quicksort(std::vector<T>& vec, int p, int r)
{
	while (p < r)
	{
		auto q = partition(vec, p, r);
		tail_recursive_quicksort(vec, p, q - 1);
		p = q + 1;
	}
	return vec;
}

template<typename T> std::vector<T> modified_tail_recursive_quicksort(std::vector<T>& vec, int p, int r)
{
	while (p < r)
	{
		auto q = partition(vec, p, r);
		if (q <= (r - p + 1) / 2)
		{
			modified_tail_recursive_quicksort(vec, q + 1, r);
			r = q - 1;
		}
		else
		{
			modified_tail_recursive_quicksort(vec, p, q - 1);
			p = q + 1;
		}
	}
	return vec;
}


//�����ģ������|�˴�ע�⣬ֻ��Ҫ���������������ɣ�������õ������ڣ��������ҵ�˳��ֻҪÿ�������ڴ���һ������ʹ����Щ�����԰��մ�С�������С������д�����ʱ����Գ������������ص�������д��ֻҪ�����������ص����䣬���Ǳ������Ϊ����������ȣ������ݴ�����һ����С�ص�����ķ�Χ�����Ը��ݾ�����ֵͬ�Ŀ����㷨�����޸�
std::tuple<int,int> section_partition(std::vector<std::tuple<double, double>>& vec, int p, int r)
{
	double x = std::get<0>(vec[r]);
	double y = std::get<1>(vec[r]);
	int i = p - 1;
	int cnt = 0;
	for (auto j = p;j <= r - 1 - cnt;j++)
	{
		if (std::get<0>(vec[j]) < x)//��ȫ���������
		{
			i++;
			std::swap(vec[i], vec[j]);
		}
		else if (std::get<1>(vec[j]) > y)//��ȫ�������Ҳ�
		{
			;
		}
		else//���������ص�������С�ص�����ķ�Χ 
		{
			cnt++;
			x = std::max(std::get<0>(vec[j]), x);
			y = std::min(std::get<1>(vec[j]), y);
			std::swap(vec[j], vec[r - cnt]);
			j--;
		}
	}
	for (auto j = 0;j <= cnt;j++)
	{
		std::swap(vec[i + 1 + j], vec[r - cnt + j]);
	}
	return std::make_tuple(i + 1, i + 1 + cnt);
}

std::tuple<int, int> randomized_section_partition(std::vector<std::tuple<double,double>>& vec, int p, int r)
{
	std::uniform_int_distribution<int> u(p, r);
	static std::default_random_engine e(time(0));
	int i = u(e);
	std::swap(vec[i], vec[r]);
	return section_partition(vec, p, r);
}

std::vector<std::tuple<double, double>> section_sort(std::vector<std::tuple<double, double>>& vec, int p, int r)
{
	if (p < r)
	{
		auto q = section_partition(vec, p, r);
		section_sort(vec, p, std::get<0>(q) - 1);
		section_sort(vec, std::get<1>(q) + 1, r);
	}
	return vec;
}

