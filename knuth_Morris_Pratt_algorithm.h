#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>

std::vector<int> compute_prefix_function(std::string a) //����ǰ׺����pi[i]=k������ĳ��λ�����ַ������԰��մӺ���ǰ��˳����ĳ��ǰ׺��ƥ�䣬��a[i-k]~a[i]=a[0]~a[k]������һǰ׺�����ϸ�С�ڵ�ǰλ�õĳ���
{
	std::vector<int> prefix_pos(a.size(), 0);
	int temp = -1; //��¼ƥ��ǰ׺�ַ����ĳ��ȣ�-1����û��ƥ���κ�ǰ׺�ַ���
	prefix_pos[0] = temp;
	for (auto i = 1;i < a.size();i++)
	{
		while (a[temp + 1] != a[i] && temp>=0) //����һ���У�temp��¼������һ��λ�õ�ǰ׺�ַ�����Ӧλ�á����ǰ׺�ַ�������һλ���޷��뵱ǰλ��ƥ�䣬����Ҫ��ת����һλ��ǰ׺�ַ�����ǰ׺�ַ���������� 
		{
			temp = prefix_pos[temp];
		}
		if (a[temp + 1] == a[i]) //whileѭ����������һ���൱�����ж���Ϊ��ԭ�������ת
		{
			temp++;
		}
		prefix_pos[i] = temp;
	}
	return prefix_pos;
}

void kmp_matcher(std::string t, std::string p) //��ǰ׺����������ƥ��
{
	std::vector<int> prefix_func = compute_prefix_function(p);
	int pos = -1;
	for (size_t i = 0; i < t.length(); i++)
	{
		while (t[i] != p[pos+1] && pos >= 0) //ע��pos�Ա��������һλ��p��ƥ��λ�ã�i��Ӧ���ǵ�ǰλ�á�����޷�ƥ������Ҫ��ת
		{
			pos = prefix_func[pos];
		}
		if (t[i] == p[pos+1])
		{
			pos++;
		}
		if (pos == p.size() - 1)
		{
			std::cout << "Pattern occurs at shift " << i - p.size() + 1 << std::endl;
			pos = prefix_func[pos]; //��ǰλ���Ѿ�ƥ����ɣ�����ڱȽ���һ���ַ���֮ǰ��������ת����һλ�õ�ǰ׺
		}
	} 
}