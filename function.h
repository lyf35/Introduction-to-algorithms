#pragma once
#include<iostream>
#include<vector>

using namespace std;

vector<vector<double>> operator*(vector<vector<double>> a, vector<vector<double>> b)//��������˵ĺ���
{
	vector<vector<double>> temp;
	vector<double> line;
	double numtemp = 0.0;
	if (a.begin()->size() != b.size())//�˺���߾����������������ұ߾��������
	{
		cout << "�������޷����˷����㣡" << endl;
		return temp;
	}
	for (auto i = 0;i != a.size();i++)//���վ������������a���������
	{
		for (auto j = 0;j != b.begin()->size();j++)//���վ������������b���������
		{
			for (auto m = 0; m != a.begin()->size();m++)//a����ÿ�е�Ԫ����b����ÿ�е�Ԫ�����
			{
				numtemp += a[i][m] * b[m][j];
			}
			line.push_back(numtemp);
			numtemp = 0.0;
		}
		temp.push_back(line);
		line.clear();
	}
	return temp;
}

vector<vector<double>> unitmatrix(unsigned n)//��λ����
{
	vector<vector<double>> matrix(n, { vector<double>(n,0.0) });
	for (unsigned i = 0;i < n;i++)
	{
		matrix[i][i] = 1;
	}
	return matrix;
}

vector<vector<double>> transposition(vector<vector<double>> a)//����ת�ú���
{
	unsigned line = a.size(), column = a.begin()->size();
	vector<vector<double>> temp(column, { vector<double>(line,0.0) });
	for (auto i = 0;i != a.size();i++)
	{
		for (auto j = 0;j != a.begin()->size();j++)
		{
			temp[j][i] = a[i][j];
		}
	}
	return temp;
}

vector<vector<double>> operator+(vector<vector<double>> a, vector<vector<double>> b)//����������ӵĺ���
{
	vector<vector<double>> temp(a.size(), { vector<double>(a.begin()->size(),0.0) });
	if (a.size() != b.size() || a.begin()->size() != b.begin()->size())
	{
		cout << "���������޷���ӣ�" << endl;
		return temp;
	}
	for (auto i = 0;i != a.size();i++)
	{
		for (auto j = 0;j != a.begin()->size();j++)
		{
			temp[i][j] = a[i][j]+b[i][j];
		}
	}
	return temp;
}

vector<vector<double>> operator-(vector<vector<double>> a, vector<vector<double>> b)//������������ĺ���
{
	vector<vector<double>> temp(a.size(), { vector<double>(a.begin()->size(),0.0) });
	if (a.size() != b.size() || a.begin()->size() != b.begin()->size())
	{
		cout << "���������޷������" << endl;
		return temp;
	}
	for (auto i = 0;i != a.size();i++)
	{
		for (auto j = 0;j != a.begin()->size();j++)
		{
			temp[i][j] = a[i][j] - b[i][j];
		}
	}
	return temp;
}

vector<vector<double>> operator*(double a, vector<vector<double>> b)//һ��������һ������ĺ���
{
	vector<vector<double>> temp(b.size(), { vector<double>(b.begin()->size(),0.0) });
	for (auto i = 0;i != b.size();i++)
	{
		for (auto j = 0;j != b.begin()->size();j++)
		{
			temp[i][j] = a*b[i][j];
		}
	}
	return temp;
}

vector<double> operator+(vector<double> a, vector <double> b) //��������ӵ������
{
	vector<double> temp;
	if (a.size() != b.size())
	{
		cout << "������������ȣ��޷����" << endl;
		return temp;
	}
	for (int i = 0;i < a.size();i++)
	{
		temp.push_back(a[i] + b[i]);
	}
	return temp;
}

vector<double> operator*(double a, vector <double> b) //һ��������һ��������˵������
{
	vector<double> temp;
	for (int i = 0;i < b.size();i++)
	{
		temp.push_back(a*b[i]);
	}
	return temp;
}

vector<double> operator/(vector <double> b, double a) //һ����������һ�������������
{
	vector<double> temp;
	for (int i = 0;i < b.size();i++)
	{
		temp.push_back(b[i] / a);
	}
	return temp;
}