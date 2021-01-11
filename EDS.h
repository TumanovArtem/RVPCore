#pragma once
#include "Branch.h"
#include "Model.h"
#include "Calculator.h"
#include <istream>
#include <string>
#include <vector>

class EDS : public Branch //����� ������������� �����������-�����������
{
public:

	EDS(int BranchCount, vector<string> DataString);
	~EDS();

	//������ ������ �� �����
	void FillData(vector<string> line);


	//�������� ������ ����������
	void FillArgumentList();

	//������ ������ � �������
	void WriteToTable(int BranchCount);

	//��������� ������
	void ChaingeData();

	// ������� ���������� ������������ ��� ����������� ���������� � ����
	void FindNodeI(int NodeIndex, double*);

	// ������� ��������� ��������� ������������� ����.
	void GetBranchG(double*);

	//�������� �������� �������� ����������
	double GetVoltageClass();

	//���������� ��������� �������
	void InitialConditions(bool);

private:

	int  PhaseType;        //���-�� ���
	double VoltageClass,   //����� ����������
		X,              //���������� ������������� ������ ������������������
		X0,             //���������� ������������� ������� ������������������
		R,              //�������� ���������������� ������������� ������ ������������������
		R0,             //�������� ���������������� ������������� ������� ������������������
		Rg,             //�������� ������������ ������������� ������ ������������������
		Rg0,            //�������� ������������ ������������� ������� ������������������
		Em,             //��������� �.�.�. (� �.�.)
		InitialAngle,   //��������� ���� ���� � (��.����.)
		InitAngleRad,
		ShuntingTime,   //����� ������������ R
		E[3];           //��� �� ������� ����

};
