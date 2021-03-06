#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

std::vector<std::vector<double>> operator+(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	if (a.begin()->size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The two matrix can not add." << std::endl;
		return { { 0 } };
	}
	else
	{
		for (int i = 0;i < a.size();i++)
		{
			for (int j = 0;j < a.begin()->size();j++)
			{
				a[i][j] += b[i][j];
			}
		}
		return a;
	}
}
std::vector<std::vector<double>> operator-(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	if (a.begin()->size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The two matrix can not subtract." << std::endl;
		return{ { 0 } };
	}
	else
	{
		for (int i = 0;i < a.size();i++)
		{
			for (int j = 0;j < a.begin()->size();j++)
			{
				a[i][j] -= b[i][j];
			}
		}
		return a;
	}
}
std::vector<std::vector<double>> merge(std::vector<std::vector<double>> c11, std::vector<std::vector<double>> c12, std::vector<std::vector<double>> c21, std::vector<std::vector<double>> c22)
{
	std::vector<std::vector<double>> c(c11.size() * 2, std::vector<double>(c11.size() * 2, 0));
	for (int i = 0;i < c11.size();i++)
	{
		for (int j = 0;j < c11.size();j++)
		{
			c[i][j]= c11[i][j];
			c[i + c11.size()][j + c11.size()]= c22[i][j];
			c[i + c11.size()][j] = c21[i][j];
			c[i][j + c11.size()] = c12[i][j];
		}
	}
	return c;
}
std::vector<std::vector<double>> Strassen_algorithm(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b) //需要保证a和b的大小都是2的幂指数
{
	if (a.size() != a.begin()->size() || b.size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The size of the martix can not match the algorithm." << std::endl;
		return{ {0} };
	}
	else
	{
		if (a.size() == 1 && b.size() == 1)
		{
			return{ {a[0][0] * b[0][0]} };
		}
		else
		{
			int size = a.size() / 2;
			std::vector<std::vector<double>> a11(size, std::vector<double>(size));
			std::vector<std::vector<double>> b11(size, std::vector<double>(size));
			std::vector<std::vector<double>> a22(size, std::vector<double>(size));
			std::vector<std::vector<double>> b22(size, std::vector<double>(size));
			std::vector<std::vector<double>> s1(size, std::vector<double>(size));
			std::vector<std::vector<double>> s2(size, std::vector<double>(size));
			std::vector<std::vector<double>> s3(size, std::vector<double>(size));
			std::vector<std::vector<double>> s4(size, std::vector<double>(size));
			std::vector<std::vector<double>> s5(size, std::vector<double>(size));
			std::vector<std::vector<double>> s6(size, std::vector<double>(size));
			std::vector<std::vector<double>> s7(size, std::vector<double>(size));
			std::vector<std::vector<double>> s8(size, std::vector<double>(size));
			std::vector<std::vector<double>> s9(size, std::vector<double>(size));
			std::vector<std::vector<double>> s10(size, std::vector<double>(size));
			for (int i = 0;i < size;i++)
			{
				for (int j = 0;j < size;j++)
				{
					a11[i][j] = a[i][j];
					a22[i][j] = a[i + size][j + size];
					b11[i][j] = b[i][j];
					b22[i][j] = b[i + size][j + size];
					s1[i][j] = b[i][j + size] - b[i + size][j + size];
					s2[i][j] = a[i][j] + a[i][j + size];
					s3[i][j] = a[i + size][j] + a[i + size][j + size];
					s4[i][j] = b[i + size][j] - b[i][j];
					s5[i][j] = a[i][j] + a[i + size][j + size];
					s6[i][j] = b[i][j] + b[i + size][j + size];
					s7[i][j] = a[i][j + size] - a[i + size][j + size];
					s8[i][j] = b[i + size][j] + b[i + size][j + size];
					s9[i][j] = a[i][j] - a[i + size][j];
					s10[i][j] = b[i][j] + b[i][j + size];
				}
			}
			auto p1 = Strassen_algorithm(a11, s1);
			auto p2 = Strassen_algorithm(s2, b22);
			auto p3 = Strassen_algorithm(s3, b11);
			auto p4 = Strassen_algorithm(a22, s4);
			auto p5 = Strassen_algorithm(s5, s6);
			auto p6 = Strassen_algorithm(s7, s8);
			auto p7 = Strassen_algorithm(s9, s10);
			auto c11 = p5 + p4 - p2 + p6;
			auto c12 = p1 + p2;
			auto c21 = p3 + p4;
			auto c22 = p5 + p1 - p3 - p7;
			return merge(c11, c12, c21, c22);
		}
	}
}