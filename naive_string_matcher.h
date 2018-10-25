#pragma once
#include<iostream>
#include<string>
#include<algorithm>

void naive_string_matcher(std::string t, std::string p) //�����ַ���ƥ���㷨����ÿ��ֻ�ƶ�һλ��p����ģʽ�ַ�����t�����ı��ַ���
{
	int p_length = p.size();
	int t_length = t.size();
	for (auto i = 0;i < t_length - p_length + 1;i++)
	{
		for (auto j = 0;j < p_length;j++)
		{
			if (p[j] != t[j + i])
			{
				break;
			}
			if (j == p_length - 1 && p[j] == t[j + i])
			{
				std::cout << "The pattern occurs with shift " << i << std::endl;
			}
		}
	}
}