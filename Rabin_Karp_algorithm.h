#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>
#include"modular_exponentiation.h"

void Rabin_Karp_matcher(char* t, char* p) //���á������������˼�룬����ÿһ���ַ���ASCII���ж��ж�Ӧ��intֵ����˿���������һ�ص㽫�ַ���ת��Ϊһ����������ʾ��p��ʾģʽ��t��ʾ��ƥ���ַ���
{
	int q = 4594591; //����ģ�����������
	int t_length = strlen(t);
	int p_length = strlen(p);
	std::vector<int> str_value(t_length - p_length + 1, 0); //���ڱ���ת��Ϊ����֮����ַ���ֵ
	int modular_exp_value = modular_exponentiation(128, p_length - 1, q); //�������λ��Ӧ��ָ����ֵ
	//�����ַ���t��1-|p|λ������Ӧ�������Լ��ַ���p����Ӧ�����֣��ڼ���ʱ�±�ԽС��λ�ö�Ӧ�ڸ�λ
	str_value[0] = int(t[0]);
	int p_value = int(p[0]);
	for (int i = 1;i < p_length;i++)
	{
		str_value[0] *= 128;
		str_value[0] += int(t[i]);
		str_value[0] %= q;
		p_value *= 128;
		p_value += int(p[i]);
		p_value %= q;
	}
	//Ȼ������ַ���t����λ������Ӧ�����֣���ͬʱ���м��
	for (int i = 1;i < t_length - p_length + 1; i++)
	{
		str_value[i] = ((128 * (str_value[i - 1] - modular_exp_value*int(t[i - 1])) + int(t[p_length + i - 1])) + q) % q;
		if (str_value[i] == p_value)
		{
			for (int j = 0;j < p_length;j++)
			{
				if (t[i + j] != p[j])
				{
					break;
				}
				if (j == p_length - 1 && t[i + j] == p[j])
				{
					std::cout << "Pattern occurs with shift " << i << std::endl;
				}
			}
		}
	}
} 