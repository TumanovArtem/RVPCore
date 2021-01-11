#pragma once
#include "Branch.h"
#include "Channel.h"

#include <string>
#include <vector>

class Line : public Branch
{
public:
	Line();
	Line(int BranchCount, vector<string> DataString);

	~Line();

	vector<Channel*> ChannelList;

	double VoltageClass,      //����� ����������
		Length,            //����� �����
		L,                 //������������� ������ ������������������
		L0,                //������������� ������� ������������������
		C,                 //������� ������ ������������������
		C0,                //������� ������� ������������������
		R,                 //�������� ������������� ������ ������������������
		R0,                //�������� ������������� ������� ������������������
		InitVoltage[3],    //��������� ���������� ���
		BegingWave[3],     //����� ���������� �� ���� ������ � ����� ����� (��� ���������� ���� � �����)
		EndWave[3],
		//�������� ���������
		WaveImpendance,   //�������� ������������� ������ ������������������
		WaveImpendance0,  //�������� ������������� ������� ������������������
		WaveLength,       //�������� ����� �����
		WaveLength0;      //��������� �������� ���� ������
						   //� ������� ������������������

//     int  PhaseType;        //���-�� ���
	bool PBayModel,        //������������� ����� �-��������
		SingleBay;        //������� ���������� ����� �� �������

		 //������ ������ �� ����� EnterForm
	void FillDataFromForm();

	//������ ������
	void FillData(vector<string> DataString);

	//�������� ������ ����������
	void FillArgumentList();

	//����� ������� ����
	void InitialConditions(bool FirstFill);

	// �������� ������� ������ �������� � ������� ������������������
	void ChannelCreate();

	// ������� ���������� ������������ ��� ����������� ���������� � ����
	void FindNodeI(int NodeIndex, double*);

	// ������� �������� ��������� ������������� ����.
	void GetBranchG(double*);

	// ������ ���������� ���� � �����
	void WaveMove();

	// ����� ����� �� ������
	void GetBranchV(double*);

	//�������� �������� �������� ����������
	double GetVoltageClass();
};

