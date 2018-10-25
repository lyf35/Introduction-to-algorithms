#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<complex>

const double pi = 3.14159265358979323846;

std::vector<unsigned int> bit_reverse(unsigned int n)//����һ������Ϊ2^ceil(lg n)�İ�λ�������顣�ú��㷨������֪ʱ�临�Ӷ�ΪO(n)
{
	unsigned bit_size = ceil(log(n)/log(2)); //��n����Ϊ2��������Ҫ����Сָ��
	unsigned int arr_size = pow(2, bit_size); //arr_size����ֵ����10����0��n��0��
	std::vector<unsigned int> rev(arr_size, 0); //������������ֵ
	unsigned rev_num = 0; //�洢������λ��������ֵ
	for (unsigned int i = 1;i < arr_size;i++)
	{
		unsigned temp = 1 << bit_size - 1; //1����n-1��0������������ֵ�����λ
		while ((temp&rev_num) != 0) //�����λ��ʼ�ж����������ĳһλ����1������Ҫ���λ���Ͻ�λ��ֱ������ĳһλΪ0��ֹͣ
		{
			rev_num ^= temp;
			temp >>= 1;
		}
		rev_num ^= temp; 
		rev[i] = rev_num; //����1�������ֵ����������
	}
	return rev;
}

std::vector<std::complex<double>> fast_fourier_transformation(std::vector<std::complex<double>> input) //fft�Ĵ��룬Ĭ��������Զ��������ģ��������ӽ���2��ָ����
{
	const unsigned arr_size = input.size();
	const unsigned output_size = pow(2, ceil(log(arr_size) / log(2)));
	std::vector<std::complex<double>> output(output_size, 0); //�洢fft�Ľ��
	std::vector<unsigned> temp = bit_reverse(arr_size); //�洢λ����֮��Ľ��
	for (unsigned i = 0;i < output_size;i++) //�����������е�Ԫ�ذ���λ����Ĵ���������������
	{
		if (temp[i] < arr_size)
		{
			output[i] = input[temp[i]]; 
		}
	}
	for (unsigned i = 0;i < ceil(log(arr_size) / log(2));i++) //���������С������Ҫ���������Ĳ���
	{
		std::complex<double> omega(cos(2 * pi / pow(2, i + 1)), sin(2 * pi / pow(2, i + 1))); //ÿһ���������ʹ�õ�omega_nֵ
		unsigned num = pow(2, i);//�����i�����������ÿһ�������е�Ԫ�ظ�����һ��
		std::vector<std::complex<double>> omega_m(num, 0);//����������ʹ�õ�omega_n^mֵ
		omega_m[0] = 1; 
		for (unsigned m = 1;m < num;m++)
		{
			omega_m[m] = omega_m[m-1]*omega;
		}
		for (unsigned j = 0;j < output_size;j += pow(2, i + 1))//���������е�Ԫ���ڵ�i���������ʱ���Ա��ֳɵ�����
		{
			for (unsigned k = 0;k < num;k++)//����ÿһ������Ҫ��2^i��ѭ��
			{
				auto temp1 = output[j + k] + omega_m[k] * output[j + k + num];
				auto temp2 = output[j + k] - omega_m[k] * output[j + k + num];
				output[j + k] = temp1;
				output[j + k + num] = temp2;
			}
		}
	}
	return output;
}

std::vector<std::complex<double>> fast_fourier_transformation_recursive(std::vector<std::complex<double>> input) //�ݹ�汾��fft��Ч�ʱȵ����汾Ҫ��
{
	int n = input.size();
	if (n == 1)
	{
		return input;
	}
	std::complex<double> omega_n(cos(2 * pi / n), sin(2 * pi / n));
	std::vector<std::complex<double>> a0, a1, y0, y1, omega(1, (1, 0)), y(n, 0);
	for (auto i = 1;i < n / 2;i++)
	{
		omega.emplace_back(omega[i - 1] * omega_n);
	}
	for (auto i = 0;i < n;i += 2)
	{
		a0.push_back(input[i]);
		a1.push_back(input[i + 1]);
	}
	y0 = fast_fourier_transformation_recursive(a0);
	y1 = fast_fourier_transformation_recursive(a1);
	for (int i = 0;i < n / 2;i++)
	{
		y[i] = y0[i] + omega[i] * y1[i];
		y[i + n / 2] = y0[i] - omega[i] * y1[i];
	}
	return y;
}

std::vector<std::complex<double>> inverse_fast_fourier_transformation(std::vector<std::complex<double>> input) //��fft�Ĵ��룬Ĭ��������Զ��������ģ��������ӽ���2��ָ����
{
	const unsigned arr_size = input.size();
	const unsigned output_size = pow(2, ceil(log(arr_size) / log(2)));
	std::vector<std::complex<double>> output(output_size, 0); //�洢ifft�Ľ��
	std::vector<unsigned> temp = bit_reverse(arr_size); //�洢λ����֮��Ľ��
	for (unsigned i = 0;i < output_size;i++) //�����������е�Ԫ�ذ���λ����Ĵ���������������
	{
		if (temp[i] < arr_size)
		{
			output[i] = input[temp[i]];
		}
	}
	for (unsigned i = 0;i < ceil(log(arr_size) / log(2));i++) //���������С������Ҫ���������Ĳ���
	{
		std::complex<double> omega(cos(2 * pi / pow(2, i + 1)), -sin(2 * pi / pow(2, i + 1))); //ÿһ���������ʹ�õ�-omega_nֵ
		unsigned num = pow(2, i);//�����i�����������ÿһ�������е�Ԫ�ظ�����һ��
		std::vector<std::complex<double>> omega_m(num, 0);//����������ʹ�õ�-omega_n^mֵ
		omega_m[0] = 1;
		for (unsigned m = 1;m < num;m++)
		{
			omega_m[m] = omega_m[m - 1] * omega;
		}
		for (unsigned j = 0;j < output_size;j += pow(2, i + 1))//���������е�Ԫ���ڵ�i���������ʱ���Ա��ֳɵ�����
		{
			for (unsigned k = 0;k < num;k++)//����ÿһ������Ҫ��2^i��ѭ��
			{
				auto temp1 = output[j + k] + omega_m[k] * output[j + k + num];
				auto temp2 = output[j + k] - omega_m[k] * output[j + k + num];
				output[j + k] = temp1;
				output[j + k + num] = temp2;
			}
		}
	}
	for (unsigned i = 0;i < output_size;i++)
	{
		output[i] /= output_size;
	}
	return output;
}

/*
�����ô��루clrs��ϰ��30.3-1����
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<random>
#include<complex>
#include"fast_fourier_transformation.h"

int main()
{
	std::vector<std::complex<double>> test{ std::complex<double>(0.0,0.0),std::complex<double>(2.0,0.0),std::complex<double>(3.0,0.0),std::complex<double>(-1.0,0.0),std::complex<double>(4.0,0.0),std::complex<double>(5.0,0.0),std::complex<double>(7.0,0.0),std::complex<double>(9.0,0.0) };
	auto result = fast_fourier_transformation(test);
	for (auto i : result)
	{
		std::cout << i << std::endl;
	}
	std::vector<std::complex<double>> test2{ std::complex<double>(29, 0),std::complex<double>(0.949747, -13.1924),std::complex<double>(-6, -1),std::complex<double>(-8.94975, -5.19239),std::complex<double>(-1, 0),std::complex<double>(-8.94975, 5.19239),std::complex<double>(-6, 1),std::complex<double>(0.949747, 13.1924) };
	std::cout << std::endl;
	auto result2 = inverse_fast_fourier_transformation(test2);
	for (auto i : result2)
	{
		std::cout << i << std::endl;
	}
	return 0;
}
*/