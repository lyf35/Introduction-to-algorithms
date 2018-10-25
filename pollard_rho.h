#pragma once
#include<iostream>
#include<cmath>
#include<random>
#include<ctime>
#include<set>
#include"Euclid_algorithm.h"

std::set<int> pollard_rho(int n) //���������ӷֽ�ĳ���
{
	std::set<int> result;
	std::uniform_int_distribution<int> rand(0, n - 1);
	std::default_random_engine e(time(0));
	int i = 1;
	int x = rand(e);
	int y = x;
	int k = 2;
	int temp = result.size();
	int count = 10000;//���õ�ѭ��1000��ʱ��set��Ԫ�ظ�����û�仯����ֹͣ���������������Ҳ�޷���֤�����ȫ��ȷ����Ҫ�����м���
	while (true)
	{
		i++;
		x = (x*x - 1) % n;
		int d = euclid(abs(y - x), n);
		if (d != 1 && d != n)
		{
			result.insert(d);
			if (result.size() == temp) //�ж�result���Ƿ��������Ԫ��
			{
				count--;
				if (count == 0)
				{
					return result;
				}
			}
			else
			{
				temp = result.size();
			}
		}
		if (i == k)
		{
			y = x;
			k *= 2;
		}
	}
}