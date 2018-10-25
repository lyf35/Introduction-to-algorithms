#pragma once
#include<iostream>
#include<cmath>
#include<tuple>

int euclid(int a, int b) //ŷ������㷨������շת����������������Լ��
{
	if (b == 0)
	{
		return a;
	}
	else
	{
		return euclid(b, a%b);
	}
}

std::tuple<int, int, int> extended_euclid(int a, int b) //��չ���ŷ������㷨������ֵ�е�һ��Ϊ���Լ�����ڶ����͵���������ֵ��Ϊax+by=gcd(a,b)��x��y��ֵ
{
	if (b == 0)
	{
		return std::make_tuple(a, 1, 0); //�ݹ�Ļ�������
	}
	else
	{
		std::tuple<int, int, int> temp = extended_euclid(b, a%b); //��һ������ֵΪgcd(a,b)������������ֵΪbx+(a%b)y=gcd(a,b)�е�x��y
		std::tuple<int, int, int> temp2 = std::make_tuple(std::get<0>(temp), std::get<2>(temp),std::get<1>(temp)-std::get<2>(temp)*floor(a/b)); //����temp�ķ���ֵ�����ƶ�ax'+by'=gcd(a,b)�е�x'��y'������ʽ�������ɽ�ã�x'=y��y'=x-y*floor(a/b)
		return temp2;
	}
}

/*
����������������99��78��gcd�Լ�ax+by=gcd(a,b)�ı��ʽ
int main()
{
	std::cout << euclid(99, 78) << std::endl;
	auto i = extended_euclid(99, 78);
	std::cout << std::get<0>(i) << '\t' << std::get<1>(i) << '\t' << std::get<2>(i) << std::endl;
	return 0;
}
*/