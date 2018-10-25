#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<omp.h>

template<typename T> int binary_search(T x, std::vector<T> A, int p, int r) //������A��p��r���±�֮������x
{
	int low = p;
	int high = p < r + 1 ? r + 1 : p;
	while (low < high)
	{
		int mid = (low + high) / 2;
		if (x <= A[mid])
		{
			high = mid;
		}
		else
		{
			low = mid + 1;
		}
	}
	return high;
}

template<typename T> void parallel_merge(std::vector<T> A, int p1, int r1, int p2, int r2, std::vector<T>& B, int p3) //p1,r1;p2,r2�ֱ��Ӧ����Ҫ�鲢��λ�ã�p3��Ӧ�鲢֮�������Ҫ�洢��λ�ã�AΪԭ���飬BΪ�鲢֮�������
{
	int n1 = r1 - p1 + 1;
	int n2 = r2 - p2 + 1;
	if (n1 < n2) //��֤�鲢ʱn1>=n2
	{
		std::swap(p1, p2);
		std::swap(r1, r2);
		std::swap(n1, n2);
	}
	if (n1 == 0)
	{
		return;
	}
	else
	{
		int q1 = (p1 + r1) / 2;
		int q2 = binary_search(A[q1], A, p2, r2);//����һ���鲢λ��������p1~r1����λ����p2~r2������Ӧ��λ��
		int q3 = p3 + (q1 - p1) + (q2 - p2); //����鲢֮��p1~r1��λ����Ӧ���ڵ�λ��
		B[q3] = A[q1]; //�Ƚ���λ�����ƹ���
#pragma omp parallel sections //ʹ�ò����㷨����ԭ����A�е�����������й鲢�����洢������B��
		{
#pragma omp section
		{
			parallel_merge(A, p1, q1 - 1, p2, q2 - 1, B, p3);
		}
#pragma omp section
		{
			parallel_merge(A, q1 + 1, r1, q2, r2, B, q3 + 1);
		}
		}
	}
}

template<typename T> void parallel_merge_sort(std::vector<T> A, int p, int r, std::vector<T>& B, int s) //A��Ӧ�ڼ������鲢��������飬p��r��ʾ�鲢��λ�ã�B��ʾ����洢�ľ���s��ʾ�鲢�����B�п�ʼ��λ��
{
	int n = r - p + 1;
	if (n == 1)
	{
		B[s] = A[p];
	}
	else
	{
		std::vector<T> C(n);
		int q = (p + r) / 2;
		int q_pos = q - p + 1;
#pragma omp parallel sections
		{
#pragma omp section
		{
			parallel_merge_sort(A, p, q, C, 0);
		}
#pragma omp section
		{
			parallel_merge_sort(A, q + 1, r, C, q_pos);
		}
		}
		parallel_merge(C, 0, q_pos - 1, q_pos, n - 1, B, s);
	}
}

/*
���Դ��룺���ʹ����ͨ��merge_sort�������������ģԽ��Ч�ʱȶ��̵߳�mergesort�ߵ�ҲԽ�࣬��������Ϊ���̵߳�mergesortҪ���ϴ���������/��������
#include<iostream>
#include<vector>
#include"parallel_merge_sort.h"
#include"merge_sort.h"
#include<random>
#include<ctime>

int main()
{
	double start, end;
	std::uniform_real_distribution<double> rand;
	std::default_random_engine e(time(0));
	std::vector<double> input, output(10000, 0.0);
	for (int i = 0;i <10000 ;i++)
	{
		input.push_back(rand(e));
	}
	start = clock();
	parallel_merge_sort(input, 0, 9999, output, 0);
	//merge_sort(input);
	end = clock();
	std::cout << end - start << std::endl;
	return 0;
}
*/