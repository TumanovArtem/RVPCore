#pragma once
#include <string>
#include <vector>

using namespace std;
class Result
{
public:
	string  ParametrName;  // ��� ���������
	string  ParametrType;  // ��� ���������
	int  ParametrCode;         // ��� ��������� (1-U,2-I,3-V)
	int  ParametrOwner;        // ����� ����� ��� ����
	int  InternalItCount;      // ���-�� ���������� ��������
	int  Ifactor;
	float BaseParametr;       // �������� ��� ��� ����������
	float Max;                // ������������ �������� ���������
	float Min;                // ����������� �������� ���������

	vector<vector<double>> DataList;   //������ 3-x �������
	vector<double> aDataList;  //������ ������ �����
	vector<double> bDataList;  //������ ������ ����
	vector<double> cDataList;  //������ ������ 0

	Result();
	Result(string);
	~Result();

	vector<double> GetParametr(int);

	//����� ������������� � ������������ �������� ���������
	void FindMaxMin();

};

