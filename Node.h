#pragma once
#include <string>
#include <vector>

#include "Phase.h"

using namespace std;
class Node
{
public:

	int i, j,
		NodeNum,	//����� ���� (������������)
		NodeIndex,	//������� ���� � Node List
		PhaseNum;	//���-�� ��� ����

	double	Ubase,	//�������� ���������� ���� (R��� = 1 ��)
			Zwavef,	//������ �������� ������������� ����
			Zwavem;	//����������� �������� ������������� ����

	bool	NodeNextCount,
			External;

	string NodeName;	//������������ ����

	vector<int> BranchesInNode;	//������ ������, �������� � ����
	vector<Phase*> PhaseList;

	void FindNodeU();    //����������� ���������� � ����

	void FindNodeZ();    //����������� ����. ������������� ����.

	void GetUxx(double*); //����� ���������� �������� �����

	void GetUxxPast(double*); //����� ���������� �������� ����� � ������ ����

	vector<double> GetU(); //����� ���������� ����

	void GetIsum(double*); //����� ����� �����

	void GetIsumPast(double*); //����� ����� ����� � ������ ����

	void GetZwave(double*); //����� ���. �������� ������������� ����

	bool UCorrection(); //��������� ���������� � ����

	void IsumCorrection(double*); //��������� c��������� ���� � ����

	void IsumAddition(double*); //��������� c��������� ���� � ����

	void Uxx0Calculation(vector<double>); //���� ���. ������� ������� ���������

	void InitialConditions(); // ���������� ��������� �������

	Node();
	Node(int, double, int, int, int);
	~Node();

	void Show();

};

