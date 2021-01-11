#pragma once
#include "Model.h"
#include <iostream>

using namespace std;

class Calculator
{
public:
	Calculator();
	~Calculator();

	//TO DO ���������� ����������. ����������� ����� ����� 

	unsigned int i, j;          // unsigned int	32 bits	0  to  4,294,967,295
						   // int            32 bits	-2,147,483,648  to  2,147,483,647
	int k, m, ActivRow;        // ���� ������

	bool FirstCount;         // ������� ��������������� �������


	int CountTime,          // ����� ����� �������
		SteadyStateTime,    // ����� ������������ ������
		CountTimeNum,
		OldStepNum,         // ���-�� ����� ��� �������� �������
		SwitchEvent,        // ���������� �������������� ������������
		NewFloatCount;      // ������� ���-�� new float

	string EntrancePath, // ���������� ��� ������ � ������
		ResultPath,   // ���������� ��� ������ � ������
		DatFileName,  // ���� � ����� ������
		ResFileName,  // ���� � ����� �����������
		Message,      // ��������� ���������
		OpenFilePath, // ��� ��������� �����
		OpenFileName, // ������������ ��������� �����
		ErrText,
		LastOpendFile[4]; // ��� � ���� ���������� ��������� �����

	void Calculate();

};

