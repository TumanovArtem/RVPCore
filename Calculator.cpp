#include "Calculator.h"
extern Model* MyModel;

extern double Omega;
extern double StepMks;
extern double PI;
extern double Frequency;
extern double Step;
extern double EmFactor;
extern double ResMultiple;
extern double CurrentTime;

extern int CurrentStep;
extern int StepNum;
extern int CountTime;

Calculator::Calculator()
{

	/*����������� ���� ��������. �������� �� ��������� ������ ���� �������������*/

	NewFloatCount = 0;
	SwitchEvent = 0;
	CountTimeNum = 1;
	FirstCount = true;

	ErrText = "";
}

Calculator::~Calculator()
{
	delete MyModel;
}

void Calculator::Calculate()
{
	if (FirstCount)
	{
		MyModel->FillBranchesListInNode();
	}

	if (MyModel->CheckModel()) {
		MyModel->ResStep = 1;
		MyModel->FindZ();

		// ����������� ���������� � ���������� ������ �������������� �������
		/*for (i = 0; i < MyModel->EventList.size(); i++) //������� ������ �������
			delete MyModel->EventList[i];
		MyModel->EventList.clear();*/

		for (i = 1; i < MyModel->NodeList.size(); i++)
			MyModel->NodeList[i]->InitialConditions();

		for (i = 1; i <= MyModel->BranchCount; i++)
			if (MyModel->BranchList[i]->Enabled)
			{
				MyModel->BranchList[i]->FillEvent();  //���������� �������
				MyModel->BranchList[i]->FindBaseValue();  //������ ��������� �������� ���� ������
				MyModel->BranchList[i]->InitialConditions(FirstCount); // ���������� ��������� �������
			}

		MyModel->SortEventsList(-1);

		MyModel->ResultList.clear();//������� ������ �����������

		// ���������� ������ ���.����������
		for (j = 0; j < MyModel->BlockList.size(); j++)
			for (i = 0; i < MyModel->BlockList[j]->ResBlockList.size(); i++)
			{
				MyModel->ResultList.push_back(new Result("U_1"/*MyModel->BlockList[j]->ResBlockList*/));	//TO DO ����������� � ResBlockList (Block->FillDataFromFile)
				//CWForm->ComboBox1->Items->Add(((Block*)MyModel->BlockList->Items[j])->ResBlockList->Strings[i]);      //���������� � ����� ������ ���. ���������� �� ������.
			}

		//ToComtrade->Enabled = true; // ����� ���� �������� � Comtrade

		//-----------------------------���� ��������------------------------------------//
		for (i = 1; i <= StepNum; i++)
		{
			CurrentStep = i;
			CurrentTime = CurrentStep * StepMks * 0.000001; // ����� � ������ ������� � �������� (���������� ��� ���������������)

			if ((SteadyStateTime != 0) && (SteadyStateTime > CurrentStep * StepMks))
				EmFactor = sqrt((CurrentStep * StepMks + 1) / SteadyStateTime); //������������ ������
			else
			{
				EmFactor = 1;
			}

			//�������� ���������� ���
			/*if ((MyModel->EventList.size() > 0) && (MyModel->EventList.size() > SwitchEvent))
				if (1000 * (MyModel->EventList[SwitchEvent])->EventTime / StepMks <= i)
				{
					if (MyModel->EventList[SwitchEvent])->PhaseSwitch())
						SwitchEvent++;
					else
					{
						if (((Event*)MyModel->EventList[SwitchEvent])->EventTime >= CurrentStep * StepMks)
							MyModel->SortEventsList(SwitchEvent); // �������������� � ������ ��������� ������� ����������
					}
				}
			*/

			ErrText = MyModel->FindU();

			if (ErrText != "")
				cout << ErrText;	//���������� �� ���������� Try/Catch ��� ���-�� �����
				//Warning(ErrText, 1);

		}
		//------------------------------------------------------------------------------//
		FirstCount = false;
	}

	//����� ���������� ���� �����
	int ParNum = 0; //�������, ������ ���� ��������� ��������
	vector<double> P;	//��������� �������
	double GrafTime;	//����� ��� �������
	for (j = 1; j <= StepNum / ResMultiple ; j++)   //���� �� ���-�� �����
	{
		GrafTime = j * ResMultiple * Step * 1000 / Omega;
		P = MyModel->ResultList[ParNum]->GetParametr(j);
		cout << GrafTime;
		cout << ' ';
		for (i = 0; i < 3; i++)
		{
			P[i] = P[i] * MyModel->ResultList[ParNum]->Ifactor * MyModel->ResultList[ParNum]->BaseParametr;
			cout << (P[i]);
			cout << ' ';
		}
		cout << endl;
	}
}