#include "Branch.h"
#include "Model.h"

extern Model* MyModel;

Branch::Branch()
{
	BegingNode = 0;
	EndNode = 0;
	BegingNodeIndex = 0;
	EndNodeIndex = 0;
	InternalItCount = 0;
	BlockNum = 1;

	ElementType = 0;
	BranchIndex = 0;
	Enabled = true;
	Virtual = false;

	ReclosingTime = 0;
	CutOffCurrent = 0;

	U0 = 0;
	Umax = 0;
	dU = 0;

	CurrentPhaseState = 7; //��� ���� ABC
	ChangePaseState = false;

	UnomRato = 1; //��������� ����������� ���������� ����� (��� ��������� ���������)
	Ibase = 0;
	PhaseType = 3;

	for (i = 0; i < 3; i++)
	{
		SwitchTime[i] = 0;
		I[i] = 0;
		Iiter[i] = 0;            //���� ����� � ����� ���� �� ���������� ��������
		Ipast[i] = 0;            //���� ����� � ������ ����
		Ub[i] = 0;
		Urab[i] = 0;
		Zrab[i] = 0;
		Grab[i] = 0;
	}

	ArgumentList = { {"", ""} };

}

Branch::~Branch()
{

}

void Branch::FillEvent()
{
	// TO DO
}

void Branch::FindBaseValue()
{
	Ibase = MyModel->NodeList[BegingNodeIndex]->Ubase;
	if (MyModel->NodeList[EndNodeIndex]->Ubase > Ibase)
		Ibase = MyModel->NodeList[EndNodeIndex]->Ubase;
}

void Branch::InitialConditions(bool FirstFill)
{
	for (i = 0; i < 3; i++)
	{
		Ipast[i] = 0;
		Iiter[i] = 0;
		I[i] = 0;
	}
}

void Branch::FindNodeI(int NodeIndex, double J[3])
{

}

// ������� �������� ��������� ������������� �����.
void Branch::GetBranchG(double G[3])
{
	//����������� �������, �������� �����������
}

// ��������� ������ ����������
double Branch::GetVoltageClass()
{
	//�������� �����������
	return 0;
}

// ������ ���������� ���� � ����� � "�������������" ����� � ������.
void Branch::WaveMove()
{
}