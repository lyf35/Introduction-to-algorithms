#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>
#include<map>
#include"knuth_Morris_Pratt_algorithm.h"

std::vector<std::map<char,int>> compute_transition_function(std::string t,std::string p) //���������Զ�����ת�ƺ�����ת�ƺ�����ֵk������һ�αȽ�ʱ��p�ĵ�k���ַ����бȽϣ�ǰ��k-1���ַ����Ѿ�ƥ��ɹ�
{
	std::string b = t;
	sort(b.begin(), b.end());
	auto c = unique(b.begin(), b.end());
	b.erase(c, b.end()); //����ƥ���ַ�������ȥ��
	std::vector<std::map<char, int>> automaton(p.size() + 1);
	std::vector<int> prefix_func = compute_prefix_function(p); //����ǰ׺����
	for (auto i = 0;i <= p.size();i++)
	{
		for (auto k : b)
		{
			if (i < p.size() && k == p[i]) //���k�뵱ǰλ�õ���ĸ��ͬ����ת�Ƶ���һ���ַ�
			{
				automaton[i][k] = i + 1;
			}
			else //���k�뵱ǰλ�õ���ĸ��ͬ������k�Ѿ����ƥ�䣬����Ҫ����ǰ׺�������ж���תλ��
			{
				int temp = i - 1; //Ĭ��0-i-1λ�����Ѿ�ƥ�䣬iλ���޷�ƥ�䣬�����ҪѰ��i-1��ǰ׺����
				while (temp >= 0 && k != p[temp + 1]) //���i-1λ�õ�ǰ׺���޷�ƥ�䣬���ظ�Ѱ��
				{
					temp = prefix_func[temp];
				}
				if (k == p[temp + 1]) //���ƥ�䵽��˵����һλ�ÿ������i��λ�ý���ƥ��
				{
					temp++;
				}
				automaton[i][k] = temp + 1; //������һ��Ӧ���бȽϵ�λ��
			}
		}
	}
	return automaton;
}

void finite_automaton_matcher(std::string t, std::string p) //ʹ�������Զ�������ƥ��
{
	auto automaton = compute_transition_function(t,p);
	int a = 0;
	for (auto i = 0;i < t.size();i++)
	{
		a = automaton[a][t[i]];
		if (a == p.size())
		{
			std::cout << "Pattern occurs at shift " << i - a + 1 << std::endl;
		}
	}
}