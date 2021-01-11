#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

class Branch
{
public:

	Branch();
	~Branch();
	
	int i,					//������
		PhaseOn,			//���-�� ���������� ���
		BegingNode,			//����� ���������� ���� �����
		EndNode,			//����� ��������� ���� �����
		BegingNodeIndex,	//������ ���������� ���� � ������ �����
		EndNodeIndex,		//������ ��������� ���� � ������ �����
		InternalItCount,	//���-�� �������� ������ ���� (���, ����)

		BranchIndex,       //����� �����
		ElementType,       //��� �������� �����

		CurrentPhaseState, //������� ��������� ��� (0-7 0-AllOff 1-A 2-B 3-AB 4-C 5-AC 6-BC 7-ABC)
		SwitchTime[3],     //����� ���������� ���
		//��������� �������� ���������� �����������
		U0,             //��������� ����������
		Umax,           //������������ ����������
		dU,             //�������� ��������� ����������
		//         ShuntingTime,   //������ ������������ R
		ReclosingTime,		//t��� ���������� ���, ����� ��������� ������� �������� ��� �������� dU
		CutOffCurrent,		//���������� ���-�� ����, ����������� ������������
		PhaseType,
		BlockNum;        //����� �����


	double  UnomRato,		//��������� ����������� ���������� ����� (��� ��������� ���������)
			Ibase,			//�������� ���
			I[3],			//���� ����� � ����� ����
			Iiter[3],		//���� ����� � ����� ���� �� ���������� ��������
			Ipast[3],		//���� ����� � ������ ����
			Ub[3],			//������� ���������� �� �����
			Urab[3],
			Zrab[3],		// ����������� �������� ������������� �����
			Grab[3];		// ����������� �������� ������������ �����

	bool    Enabled,        //������� ������� ����� � �������
			Virtual,
			ChangePaseState;//������� ��������� ��������� �����

	string BranchName; //��� �����

	map<string, string> ArgumentList;

	virtual void FindNodeI(int, double *);

	// ��������� ������ ����������
	virtual double GetVoltageClass();

	// ������� �������� ��������� ������������� �����.
	virtual void GetBranchG(double*);

	//���������� �������
	void FillEvent();

	// ������ ���������� ���� � ����� � "�������������" ����� � ������.
	virtual void WaveMove();

	//������ ��������� �������� ���� �����
	virtual void FindBaseValue();

	//���������� ������� ���������� ���������� ����
	virtual void InitialConditions(bool);
};

