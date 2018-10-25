#pragma once
#include<iostream>
#include<cmath>
#include<vector>

int modular_exponentiation(int a, int b, int n) //����a^b(mod n)
{
	int d = 1;
	int s = a;
	int bit = 1;
	for (int i = 0;i <= floor(log2(b));i++) //�������Ҫѭ��b�Զ����Ʊ�ʾ��λ���Ρ�ѭ������λ���ӵ�λ����λ��˳�����
	{
		if (b&bit) //����b�Ķ����Ʊ�ʾ�У�ĳһλ�ϵ���ֵΪ1����ʱ��Ҫ����Ӧ�Ĳ���
		{
			d = (s*d) % n;
		}
		bit <<= 1;
		s = (s*s) % n; //��Ӧ��b�ĵ�i+1λ����a��2^i+1�η�
	}
	return d;
}