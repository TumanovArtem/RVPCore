#include "Result.h"
#include "Model.h"

extern Model* MyModel;
//����������� 1
Result::Result()
{

}

//����������� 2
Result::Result(string Parametr)
{
	int i;//, BegingNodeIndex, EndNodeIndex;

	InternalItCount = 0;

	Ifactor = 1;

	DataList.reserve(100);

	aDataList.reserve(100);
	bDataList.reserve(100);
	cDataList.reserve(100);

	aDataList.push_back(0);
	bDataList.push_back(0);
	cDataList.push_back(0);

	DataList.push_back(cDataList);
	DataList.push_back(aDataList);
	DataList.push_back(bDataList);

	ParametrName = Parametr;  // ��� ���������

	for (i = Parametr.size(); i > 0; i--)
		if (Parametr[i] == 95) // "_"
		{
			/*if ((Parametr.size() - i > 1) && (int(Parametr[i - 2]) != 32)) //" " ��������� �����
				ParametrType = Parametr[i - 2];*/
			ParametrType += Parametr[i - 1];
			ParametrOwner = stoi(Parametr.substr(i + 1, Parametr.size()));
			i = 0;
		}

	// if (ParametrType == NULL)

	if (ParametrType == "U")
	{
		ParametrCode = 1;

		for (i = 1; i < MyModel->NodeList.size(); i++)
			if (MyModel->NodeList[i]->NodeNum == ParametrOwner)
				BaseParametr = MyModel->NodeList[i]->Ubase;
	}
	else
	{
		if (ParametrType == "I")
		{
			ParametrCode = 2;
			BaseParametr = MyModel->BranchList[ParametrOwner]->Ibase;
			Ifactor = 1000;
		}

		if (ParametrType == "J") //J(t)
		{
			ParametrCode = 5;
			BaseParametr = MyModel->BranchList[ParametrOwner]->Ibase;
			Ifactor = 1000;
		}

		if (ParametrType == "F") //Shunt
		{
			ParametrCode = 6;
			BaseParametr = 1;
		}

		if (ParametrType == "R") //OPN
		{
			ParametrCode = 4;
			BaseParametr = 0.001;
		}

		if (ParametrType == "U�") // ������� ���������� �� �����
		{
			ParametrCode = 7;
			BaseParametr = MyModel->BranchList[ParametrOwner]->Ibase;
		}

		if (ParametrType == "I�") //��� ������ �����
		{
			ParametrCode = 8;
			BaseParametr = MyModel->BranchList[ParametrOwner]->Ibase;
			Ifactor = 1000;
		}

		if (ParametrType == "I�") //��� ����� �����
		{
			ParametrCode = 9;
			BaseParametr = MyModel->BranchList[ParametrOwner]->Ibase;
			Ifactor = 1000;
		}

		if (ParametrType == "V") //����� � �����
		{
			ParametrCode = 3;
			//BaseParametr = 1;
			BaseParametr = MyModel->BranchList[ParametrOwner]->Ibase;
		}

	}
}

//����������
Result::~Result()
{
	aDataList.clear();
	bDataList.clear();
	cDataList.clear();
	DataList.clear();
}
/*
//---------------------------------------------------------------------------
//����� ������������� � ������������ �������� ���������
void Result::FindMaxMin()
{
	int i;
	float MaxArray, MinArray;

	for (i = 1; i < aDataList->Count; i++)
	{
		MaxArray = max(*(float*)aDataList->Items[i], *(float*)bDataList->Items[i]);
		MaxArray = max(MaxArray, *(float*)cDataList->Items[i]);
		Max = max(Max, MaxArray);

		MinArray = min(*(float*)aDataList->Items[i], *(float*)bDataList->Items[i]);
		MinArray = min(MinArray, *(float*)cDataList->Items[i]);
		Min = min(Min, MinArray);
	}
	//     Max = MaxBase * BaseParametr;
	//     Min = MinBase * BaseParametr;
}
*/
//---------------------------------------------------------------------------
//�������� ��������� �� ������
vector<double> Result::GetParametr(int StepN)
{
	vector<double> P;
	int i;
	if (StepN > 0)
		for (i = 0; i < 3; i++)
			P.push_back(DataList[i][StepN]);
	/*else // ��� ������ ���������� ��������� (����� ��������� �� ����� �������)
		for (i = 0; i < 3; i++)
			P[i] = DataList[i][DataList[i].size()];
	*/
	return P;
}
//---------------------------------------------------------------------------


