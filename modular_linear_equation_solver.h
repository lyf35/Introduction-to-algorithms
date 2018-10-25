#pragma once
#include<iostream>
#include<cmath>
#include<tuple>
#include"Euclid_algorithm.h"

int modular_linear_equation_solver(int a, int b, int n) //���㷽��ax=b(mod n)�Ľ⡣�ɷ��̿ɵ�ax-kn=b����������н⣬��ô�൱��b������a��n��������ϱ�ʾ������н⣬����ֵΪ��С��x���޽��򷵻�-1
{
	auto gcd_an = extended_euclid(a, n); //���b���Ա�gcd(a,n)��������ô˵�������н�
	if (b%std::get<0>(gcd_an) == 0)
	{
		std::cout << "the solution for equation " << a << "x=" << b << "(mod " << n << ") is: ";
		for (int i = 0;i < n / std::get<0>(gcd_an);i++)
		{
			std::cout << (((std::get<1>(gcd_an)*(b / std::get<0>(gcd_an))) % n) + n) % n + i* n / std::get<0>(gcd_an) << " "; //�˴�Ҫע�⣬�п���ȡ����ŵ�����Ǹ���������Ҫ��һЩ����
		}
		std::cout << std::endl;
		return (((std::get<1>(gcd_an)*(b / std::get<0>(gcd_an))) % n) + n) % n;
	}
	else //��֮˵�������޽�
	{
		std::cout << "equation " << a << "x=" << b << "(mod " << n << ") has no solution." << std::endl;
		return -1;
	}
}