#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<numeric>
#include<algorithm>
#include<iterator>
#include"function.h"

using namespace std;

class matrix
{
	friend vector<vector<double>> operator*(vector<vector<double>> a, vector<vector<double>> b);
	friend vector<vector<double>> unitmatrix(unsigned n);
	friend vector<vector<double>> transposition(vector<vector<double>> a);

private:
	vector<vector<double>> A; //���ԭʼ����Ϊ�˿���ʹ����׵ľ��󶼿��Ա��ֽ⣬������vector���洢
	vector<vector<double>> Q; //�ֱ���QR�ֽ��ľ���
	vector<vector<double>> R;
	vector<vector<double>> L; //�ֱ���LUP�ֽ�֮��ľ���
	vector<vector<double>> U;
	vector<vector<double>> P;

public:
	matrix(const vector<vector<double>> init) :A(init), Q(unitmatrix(init.size())), R(init), L(unitmatrix(init.size())), U(unitmatrix(init.size())), P(unitmatrix(init.size())) {};
	bool QR_decomposition()//����ֵΪ1����ֽ�ɹ�������ֵΪ0����ֽ�ʧ��
	{
		if (A.size() < A.begin()->size())//������������������޷����зֽ⣬ֱ�ӷ���0
		{
			cout << "���������������޷����зֽ⣡" << endl;
			return 0;
		}
		vector<vector<double>> temp;//�洢��ǰ���ĺ�˹�����¾���
		for (auto i = 0;i != min(A.begin()->size(), A.size() - 1);i++)//�����ж��٣���һ������ж��ٴηֽ�
		{
			vector<vector<double>> H(A.size() - i, { vector<double>(A.size() - i,0.0) });
			vector<double> x;
			vector<double> u;
			//�������x
			for (int j = i;j < A.size();j++)
			{
				x.push_back(R[j][i]);
			}
			//�������u
			double x_second_norm = sqrt(inner_product(x.begin(),x.end(),x.begin(),0.0));//����x�Ķ�����
			x[0] = x[0] - x_second_norm;//���ڼ���u��ʱ��ֻ��x�ڵĵ�һ��Ԫ�ر���ȥsigma*v����ֻ��Ҫ��x�ĵ�һ��Ԫ�����仯����
			double xmod_second_norm = sqrt(inner_product(x.begin(), x.end(), x.begin(), 0.0));//����x��ȥsigma*v������������2����
			copy(x.begin(), x.end(), back_inserter(u));
			for_each(u.begin(), u.end(), [xmod_second_norm](double& a) {a = a / xmod_second_norm;});
			//�����˹�����¾����ȼ����˹�����¾���Ԫ��
			for (auto hi = 0;hi < A.size() - i;hi++)
			{
				for (auto hj = 0;hj < A.size() - i;hj++)
				{
					H[hi][hj] = -2 * u[hi] * u[hj];
					if (hi == hj)
					{
						H[hi][hj] += 1.0;
					}
				}
			}
			//���i>0����ֻ��ȡ��R�������Ĳ���Ԫ�أ�����Ҫ�����þ��������չ
			if (i > 0)
			{
				for (auto &iter : H)
				{
					iter.insert(iter.begin(), i, 0.0);
				}
				for (int m = i;m > 0;m--)
				{
					vector<double> temp(H.begin()->size(), 0.0);
					temp[m - 1] = 1;
					H.insert(H.begin(), temp);
				}
			}
			R = H*R;
			Q = H*Q;
		}
		Q = transposition(Q);
		return 1;
	}
	bool LUP_decomposition() //�ڷֽ��ʱ��ѡȡ��Ԫ
	{
		if (A.size() != A.begin()->size())
		{
			std::cout << "���ֽ��������в���ȣ�";
			return 0;
		}
		vector<vector<double>> A_temp = A; 
		int size = A.size();
		for (int i = 0;i < size;i++) //Ϊ��ȷ�����ȣ�ʹ��ѡ��Ԫ�ĸ�˹��ȥ��
		{
			//��ѡȡ��Ԫ
			double col_max = A_temp[i][i]; //���ڱ���ÿһ�д�i��n�����Ԫ��
			int line = i; //�������Ԫ�����ڵ���
			for (int x = i + 1;x < size;x++) 
			{
				if (col_max < A_temp[x][i])
				{
					col_max = A_temp[x][i];
					line = x;
				}
			}
			std::swap(A_temp[i], A_temp[line]); //����Ԫ������i��
			std::swap(P[i], P[line]);
			//ѡȡ����Ԫ֮������LU�ֽ�
			for (int x = i + 1;x < size;x++) //����ÿһ�е�ϵ��������Ӧ���¾�������һ�е�ֵ
			{
				A_temp[x][i] /= A_temp[i][i];
				for (int y = x;y < size;y++)
				{
					A_temp[x][y] -= A_temp[x][i] * A_temp[i][y];
				}
			}
			//ִ������Щ����֮�󣬽���ʱ�����е�ֵд�뵽L��U��
			for (int i = 1;i < size;i++)
			{
				for (int j = 0;j < i;j++)
				{
					L[i][j] = A_temp[i][j];
				}
			}
			for (int i = 0;i < size;i++)
			{
				for (int j = i;j < size;j++)
				{
					U[i][j] = A_temp[i][j];
				}
			}
		}
		return 1;
	}
	bool LU_decomposition() //�ڷֽ��ʱ��ѡȡ��Ԫ���˺����ڵ���ʱ����Ҫȷ��ԭ�����ǶԳ���������
	{
		if (A.size() != A.begin()->size())
		{
			std::cout << "���ֽ��������в���ȣ�";
			return 0;
		}
		vector<vector<double>> A_temp = A;
		int size = A.size();
		for (int i = 0;i < size;i++) 
		{
			//�ȼ���ÿһ�е�ϵ��������Ӧ���¾�������һ�е�ֵ
			for (int x = i + 1;x < size;x++) 
			{
				A_temp[x][i] /= A_temp[i][i];
				for (int y = x;y < size;y++)
				{
					A_temp[x][y] -= A_temp[x][i] * A_temp[i][y];
				}
			}
			//����ʱ�����е�ֵд�뵽L��U��
			for (int i = 1;i < size;i++)
			{
				for (int j = 0;j < i;j++)
				{
					L[i][j] = A_temp[i][j];
				}
			}
			for (int i = 0;i < size;i++)
			{
				for (int j = i;j < size;j++)
				{
					U[i][j] = A_temp[i][j];
				}
			}
		}
		return 1;
	}
	vector<vector<double>> solve_LUP(vector<vector<double>> b) //����LUP�ֽⲢ��ⷽ�̡����д˺���֮ǰҪ���е���LUP�ֽ�ĺ���
	{
		b = P*b;
		vector<vector<double>> Y = b;
		for (int i = 0;i < b.size();i++)
		{
			for (int j = 0;j < i;j++)
			{
				if (L[i][j] != 0)
				{
					Y[i][0] -= Y[j][0] * L[i][j];
				}
			}
		}
		vector<vector<double>> X = Y;
		for (int i = b.size() - 1;i >= 0;i--)
		{
			for (int j = b.size() - 1;j > i;j--)
			{
				if (U[i][j] != 0)
				{
					X[i][0] -= X[j][0] * U[i][j];
				}
			}
			X[i][0] /= U[i][i];
		}
		return X;
	}
	vector<vector<double>> inverse_matrix() //����LUP�ֽ�Ľ����������
	{
		vector<vector<double>> inverse = A;
		for (int i = 0;i < A.size();i++)
		{
			vector<vector<double>> X(A.size(), vector<double>(1, 0));
			X[i][0] = 1;
			X = solve_LUP(X);
			for (int j = 0;j < A.size();j++)
			{
				A[i][j] = X[j][0];
			}
		}
		return A;
	}
	vector<vector<double>> solve_QR(vector<vector<double>> b)//����QR�ֽⲢ��⡣���д˺���֮ǰҪ���е���QR�ֽ�ĺ���
	{
		vector<vector<double>> R_temp(R);//Ϊ��ʹR���󲻱��޸ģ���һ����ʱ�����洢R�Ա������������
		vector<vector<double>> y = transposition(Q)*b;
		vector<vector<double>> X(b.size(), vector<double>(1, 0));
		for (int i = R.begin()->size() - 1;i > 0;i--)
		{
			for (int j = i - 1;j >= 0;j--)
			{
				y[j][0] = y[j][0] - R_temp[j][i] / R_temp[i][i] * y[i][0];
				R_temp[j][i] = 0;
			}
		}
		for (int i = R.begin()->size() - 1;i >= 0;i--)
		{
			X[i][0] = y[i][0] / R[i][i];
		}
		return X;
	}
	void output_QR(ostream& os)//����ֽ����þ���Q��R�ĺ�������������ѡ�����������̨���ı���
	{
		os << "Q����" << endl;
		for (auto i = 0;i < Q.size();i++)
		{
			for (auto j = 0;j < Q.begin()->size();j++)
			{
				os << Q[i][j] << '\t';
			}
			os << endl;
		}
		os << "R����" << endl;
		for (auto i = 0;i < R.size();i++)
		{
			for (auto j = 0;j < R.begin()->size();j++)
			{
				os << R[i][j] << '\t';
			}
			os << endl;
		}
	}
	void output_LUP(ostream& os)//����ֽ����þ���Q��R�ĺ�������������ѡ�����������̨���ı���
	{
		os << "L����" << endl;
		for (auto i = 0;i < L.size();i++)
		{
			for (auto j = 0;j < L.begin()->size();j++)
			{
				os << L[i][j] << '\t';
			}
			os << endl;
		}
		os << "U����" << endl;
		for (auto i = 0;i < U.size();i++)
		{
			for (auto j = 0;j < U.begin()->size();j++)
			{
				os << U[i][j] << '\t';
			}
			os << endl;
		}
		os << "P����" << endl;
		for (auto i = 0;i < P.size();i++)
		{
			for (auto j = 0;j < P.begin()->size();j++)
			{
				os << P[i][j] << '\t';
			}
			os << endl;
		}
	}
};

/*
������������
#include<iostream>
#include<vector>
#include"matrix.h"

using namespace std;

int main()
{
	vector<vector<double>> orig{ {1,-1,0,0,0},{-1,2,-1,0,0},{0,-1,2,-1,0},{0,0,-1,2,-1},{0,0,0,-1,2} };
	vector<vector<double>> in{ { 1 },{ 1 },{ 1 },{ 1 },{ 1 } };
	matrix a(orig);
	a.QR_decomposition();
	auto result_QR = a.solve_QR(in);
	cout << "��QR�ֽ�Ľ���ⷽ�̣�" << endl;
	for (auto i : result_QR)
	{
		cout << i[0] << '\t';
	}
	cout << endl;
	a.output_QR(cout);
	a.LUP_decomposition();
	cout << "��LUP�ֽ�Ľ���������" << endl;
	auto inv = a.inverse_matrix();
	for (auto i = 0;i < inv.size();i++)
	{
		for (auto j = 0;j < inv.begin()->size();j++)
		{
			cout << inv[i][j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
	a.output_LUP(cout);
}
*/
