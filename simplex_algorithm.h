#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<numeric>
#include<set>
#include<algorithm>

class linear_programming
{
private:
	/*
	��������Ӧ�ĺ��壺
	����N�е�Ԫ�ش���ǻ�����������СΪn
	����B�е�Ԫ�ش��������������СΪm
	����A�еĵ�i�ж�Ӧ��i�����������ȫ��0��������һ�ж�Ӧ�ı����Ƿǻ�������������з�0Ԫ�أ���ô��һ�ж�Ӧ�ڵ�i�������������������Թ滮�ɳ����е�һ��Լ����������һ�е�Ԫ�ض�Ӧ�ǻ���������ϵ����A�Ĺ�ģΪ(n+m)*(n+m)
	����b�е�i��Ԫ�ض�Ӧ��i������������ӦԼ���еĳ������ģΪ��n+m��
	����c�е�i��Ԫ�ض�Ӧ���Ż����ʽ�е�i��������ϵ������ģΪ��n+m��
	��ֵv������Ż����ʽ�еĳ�����
	����result����������Թ滮��һ����н⣬���ȫ������double_max����������Թ滮�Ľ��޽�
	*/
	std::set<int> N;
	std::set<int> B;
	std::vector<std::vector<double>> A;
	std::vector<double> b;
	std::vector<double> c;
	double v;
	std::vector<double> result;

public:
	linear_programming(std::set<int> N_input, std::set<int> B_input, std::vector<std::vector<double>> A_input, std::vector<double> b_input, std::vector<double> c_input) :N(N_input), B(B_input), A(A_input), b(b_input), c(c_input), v(0), result(c_input.size(),0) {};

	linear_programming initialize_simplex() //�����Թ滮���г�ʼ����ʹ��������ڵ������㷨
	{
		double min = LDBL_MAX; 
		int num = 0; //������СԼ��ֵ��Ӧ�����
		for (int k = 0;k < b.size();k++) //ѡȡ����Լ��ֵ�е���Сֵ�Լ����Ӧ�����
		{
			if (b[k] < min)
			{
				min = b[k];
				num = k;
			}
		}
		if (min >= 0) //����������ɳ��Ϳɽ⣬��ֱ�ӷ��ؼ���
		{
			return *this;
		}
		linear_programming aux = build_auxillary_linear_programming(); //������ɽ⣬����Ҫ��⸨�����Թ滮
		aux.pivot(num, A.size());
		aux.simplex(); //�Ը������Թ滮���õ������㷨���
		if (abs(aux.result[aux.result.size() - 1]) <= 1e-10) //�ڸ������Թ滮�����Ž��У��������������ֵΪ0��˵��������Թ滮�п��н⣬���ݸ������Թ滮�Ľ����ԭ���Թ滮�����޸�
		{
			if (aux.B.find(aux.result.size() - 1) != aux.B.end()) //������������ǻ�������������Ҫִ��һ��ת������������������Ϊ�ǻ�������������ѡ������һ���ǻ���������Ϊ���滻��
			{
				aux.pivot(aux.result.size() - 1, *(N.begin()));
			}
			for (int i = 0;i < A.size();i++) //�Ѹ������Թ滮��Լ����������ԭʼ���Թ滮���������еĸ�������ȫ��ȥ��
			{
				for (int j = 0;j < A.size();j++)
				{
					A[i][j] = aux.A[i][j];
				}
				b[i] = aux.b[i];
			}
			B = aux.B; //���������Թ滮�Ļ���������ǻ�������Ҳ���ƹ���
			N = aux.N;
			N.erase(N.find(aux.result.size() - 1));
			for (int i = 0;i < A.size();i++)//��Ҫ��ԭʼ��Ŀ���Ż�����Ҳ��һ�����޸�
			{
				if (c[i] != 0 && B.find(i) != B.end()) //���Ŀ���Ż������д��ڻ�����������Ҫ�����滻��
				{
					for (int j = 0;j < A.size();j++)
					{
						c[j] -= A[i][j] * c[i];
					}
					v += b[i]*c[i];
					c[i] = 0;
				}
			}
		}
		else //�������������ֵ��Ϊ0��˵���������Թ滮�޽⡣��result�����е�����Ԫ�ظ�ֵΪ˫���ȸ���������Сֵ�������޽�
		{
			for (auto i = result.begin();i != result.end();i++)
			{
				*i = LDBL_MIN;
			}
		}
		return *this;
	}

	linear_programming pivot(int l, int e)//l��Ӧ����һ��������������ţ�e��Ӧ����һ���ǻ�����������ţ�ͨ��pivot������e��Ϊ����������l��Ϊ�ǻ����������������൱�ڴ�͹�����һ��������ת����һ������
	{
		std::vector<std::vector<double>> A_mod(A.size(), std::vector<double>(A.begin()->size(), 0));
		std::vector<double> b_mod(b.size(), 0);
		std::vector<double> c_mod(c.size(), 0);
		//�ȼ����e��������������Ϊ��������������Ӧ��Լ�����ʽ
		b_mod[e] = b[l] / A[l][e];
		for (auto i = N.begin();i != N.end();i++) 
		{
			if (*i != e)
			{
				A_mod[e][*i] = A[l][*i] / A[l][e];
			}
		}
		A_mod[e][l] = 1 / A[l][e];
		//�ٶ������Լ�������޸ģ����ѵ�e��Ԫ�ض�Ӧ�ı��ʽ���뵽������Լ�������н�����Ԫ
		for (auto i = B.begin();i != B.end();i++)
		{
			if (*i != l)
			{
				b_mod[*i] = b[*i] - A[*i][e] * b_mod[e];
				for (auto j = N.begin();j != N.end();j++)
				{
					if (*j != e)
					{
						A_mod[*i][*j] = A[*i][*j] - A[*i][e] * A_mod[e][*j];
					}
				}
				A_mod[*i][l] = -A[*i][e] * A_mod[e][l];
			}
		}
		//�޸�Ŀ���Ż�����
		v += c[e] * b_mod[e];
		for (auto i = N.begin();i != N.end();i++)
		{
			if (*i != e)
			{
				c_mod[*i] = c[*i] - c[e] * A_mod[e][*i];
			}
		}
		c_mod[l] = -c[e] * A_mod[e][l];
		//�Լ����е�Ԫ�����޸�
		N.erase(N.find(e));
		N.insert(l);
		B.erase(B.find(l));
		B.insert(e);
		A = A_mod;
		b = b_mod;
		c = c_mod;
		return *this;
	}

	linear_programming simplex() //��ⵥ�����㷨�ĳ���ִ���������֮ǰҪ��ִ��һ��initialize_simplex()������ȷ�����Թ滮�Ƿ��п��н�
	{
		if (result[0] == LDBL_MIN)
		{
			std::cout << "infeasible" << std::endl;
			return *this;
		}
		std::vector<double> temp(c.size(), LDBL_MAX); 
		auto i = N.begin();
		while(i != N.end())
		{
			temp = std::vector<double>(c.size(), LDBL_MAX);
			if (c[*i] > 0) //Ѱ�Ҵ��Ż����ʽ��ϵ��Ϊ�����ı���i
			{
				for (auto j = B.begin();j != B.end();j++) //����ÿһ��Լ���������б���i����ȡ�����ֵ
				{
					if (A[*j][*i] > 0)
					{
						temp[*j] = b[*j] / A[*j][*i];
					}
				}
				double min = LDBL_MAX; //���ڱ���Լ��ֵ
				int num = 0; //������СԼ��ֵ��Ӧ�����
				for (auto k = B.begin();k != B.end();k++) //ѡȡ����Լ��ֵ�е���Сֵ�Լ����Ӧ�����
				{
					if (temp[*k] < min)
					{
						min = temp[*k];
						num = *k;
					}
				}
				if (min == LDBL_MAX) //���Լ��ֵ�е���СֵΪ�������ô˵��������Թ滮�޽磬����ֱ�ӷ���
				{
					std::cout << "Unbounded" << std::endl;
					for_each(result.begin(), result.end(), [](double i) {i = LDBL_MAX;});
					return *this;
				}
				else //�����Сֵ�����������ô��ԭ�ɳ�����ת������
				{
					pivot(num, *i);
					i = N.begin(); //����ת��֮��ļ��Ϸ����ı䣬�����Ҫ���¶�i���и�ֵ����ֱ�ӽ�����һ����ѭ��
					continue;
				}
			}
			i++;
		}
		for (int i = 0;i < c.size();i++)
		{
			if (B.find(i) != B.end())
			{
				result[i] = b[i];
			}
			else
			{
				result[i] = 0;
			}
		}
		return *this;
	}

	linear_programming build_auxillary_linear_programming() //����ԭʼ���Թ滮����һ���������Թ滮������m+nΪ�±����ı��
	{
		std::set<int> N_aux = N;
		std::set<int> B_aux = B;
		std::vector<std::vector<double>> A_aux = A;
		std::vector<double> b_aux = b;
		std::vector<double> c_aux(A.size() + 1, 0);
		N_aux.insert(c.size());
		for (auto i = A_aux.begin();i != A_aux.end();i++)
		{
			i->push_back(0);
		}
		for (auto i = B.begin();i != B.end();i++)
		{
			A_aux[*i][A_aux.begin()->size() - 1] = -1;
		}
		A_aux.push_back(std::vector<double>(A_aux.begin()->size(), 0));
		b_aux.push_back(0);
		c_aux[A.size()] = -1;
		linear_programming aux(N_aux, B_aux, A_aux, b_aux, c_aux);
		return aux;
	}

	void output_result(std::ostream& os)
	{
		for (auto i : result)
		{
			os << i << '\t';
		}
		os << std::endl;
	}
};

/*
���ԣ�
clrs P507��ʽ(29.53)-(29.57)����Ӧ�����Թ滮��
std::set<int> N = { 0,1,2 };
std::set<int> B = { 3,4,5 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0,0 },{ 0,0,0,0,0,0 },{ 0,0,0,0,0,0 },{ 1,1,3,0,0,0 },{ 2,2,5,0,0,0 },{ 4,1,2,0,0,0 } };
std::vector<double> b = { 0,0,0,30,24,36 };
std::vector<double> c = { 3,1,2,0,0,0 };

clrs P524 ��ϰ29.5-5 ��Ӧ���Թ滮��һ������Ŀ��ֵ���Ž�����
std::set<int> N = { 0,1 };
std::set<int> B = { 2,3,4 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 1,-1,0,0,0 },{ -1,-1,0,0,0 },{ -1,4,0,0,0 } };
std::vector<double> b = { 0,0,8,-3,2 };
std::vector<double> c = { 1,3,0,0,0 };

clrs P524 ��ϰ29.5-6 ��Ӧ���Թ滮�޿��н�����
std::set<int> N = { 0,1 };
std::set<int> B = { 2,3,4 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 1,2,0,0,0 },{ -2,-6,0,0,0 },{ 0,1,0,0,0 } };
std::vector<double> b = { 0,0,4,-12,1 };
std::vector<double> c = { 1,-2,0,0,0 };

clrs P524 ��ϰ29.5-7 ��Ӧ���Թ滮�޽�����
std::set<int> N = { 0,1 };
std::set<int> B = { 2,3,4 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ -1,1,0,0,0 },{ -1,-1,0,0,0 },{ -1,4,0,0,0 } };
std::vector<double> b = { 0,0,-1,-3,2 };
std::vector<double> c = { 1,3,0,0,0 };

#include<iostream>
#include<vector>
#include<set>
#include"simplex_algorithm.h"

int main()
{
	//�������ı���ճ���ڴ˴�
	linear_programming test(N, B, A, b, c);
	test.initialize_simplex();
	test.simplex();
	test.output_result(std::cout);
	return 0;
}
*/