#pragma once
#include<iostream>
#include<cmath>
#include<random>
#include<ctime>
#include"modular_exponentiation.h"

bool witness(int a, int n) //Ѱ��nΪ�����ġ�֤�ݡ����ж�a�Ƿ�Ϊ��֤�ݡ�
{
	int t = 1;
	int u = n - 1;
	while (u % 2 == 0)
	{
		u /= 2;
		t++;
	}
	int x = modular_exponentiation(a, u, n);
	for (int i = 1;i <= t;i++)
	{
		int temp = (x*x) % n;
		if (temp == 1 && x != 1 && x != n - 1) //���n����������ôx^2=1 (mod n) �Ľ���ƽ���ģ�����1
		{
			return true;
		}
		x = temp;
	}
	if (x != 1) //���n�����������ݷ�����һ���� a^n-1 (mod n)=1
	{
		return true;
	}
	return false;
}

bool miller_rabin(int n, int s) //miller-rabin�������Եĳ���n��������Ե�����s������ԵĴ���������ֵΪ1���������0��������
{
	std::uniform_int_distribution<int> rand(1, n - 1);
	std::default_random_engine e(time(0));
	for (int j = 1;j <= s; j++)
	{
		int a = rand(e);
		if (witness(a, n))
		{
			return 1;
		}
	}
	return 0;
}