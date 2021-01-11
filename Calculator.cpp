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

	/*Определение всех констант. Значения по умолчанию должны быть предусмотрены*/

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

		// Организация заполнение и сортировка списка коммутационных событий
		/*for (i = 0; i < MyModel->EventList.size(); i++) //Очистка списка событий
			delete MyModel->EventList[i];
		MyModel->EventList.clear();*/

		for (i = 1; i < MyModel->NodeList.size(); i++)
			MyModel->NodeList[i]->InitialConditions();

		for (i = 1; i <= MyModel->BranchCount; i++)
			if (MyModel->BranchList[i]->Enabled)
			{
				MyModel->BranchList[i]->FillEvent();  //Заполнение событий
				MyModel->BranchList[i]->FindBaseValue();  //Запись базисного значения тока ветвей
				MyModel->BranchList[i]->InitialConditions(FirstCount); // Заполнение начальных условий
			}

		MyModel->SortEventsList(-1);

		MyModel->ResultList.clear();//Очистка списка результатов

		// Заполнение списка рез.параметров
		for (j = 0; j < MyModel->BlockList.size(); j++)
			for (i = 0; i < MyModel->BlockList[j]->ResBlockList.size(); i++)
			{
				MyModel->ResultList.push_back(new Result("U_1"/*MyModel->BlockList[j]->ResBlockList*/));	//TO DO разобраться с ResBlockList (Block->FillDataFromFile)
				//CWForm->ComboBox1->Items->Add(((Block*)MyModel->BlockList->Items[j])->ResBlockList->Strings[i]);      //Заполнение в форме вывода доп. параметров на график.
			}

		//ToComtrade->Enabled = true; // Пункт меню экспорта в Comtrade

		//-----------------------------Цикл рассчета------------------------------------//
		for (i = 1; i <= StepNum; i++)
		{
			CurrentStep = i;
			CurrentTime = CurrentStep * StepMks * 0.000001; // Время с начала расчета в секундах (необходимо для преобразователя)

			if ((SteadyStateTime != 0) && (SteadyStateTime > CurrentStep * StepMks))
				EmFactor = sqrt((CurrentStep * StepMks + 1) / SteadyStateTime); //Установление режима
			else
			{
				EmFactor = 1;
			}

			//Проверка коммутации фаз
			/*if ((MyModel->EventList.size() > 0) && (MyModel->EventList.size() > SwitchEvent))
				if (1000 * (MyModel->EventList[SwitchEvent])->EventTime / StepMks <= i)
				{
					if (MyModel->EventList[SwitchEvent])->PhaseSwitch())
						SwitchEvent++;
					else
					{
						if (((Event*)MyModel->EventList[SwitchEvent])->EventTime >= CurrentStep * StepMks)
							MyModel->SortEventsList(SwitchEvent); // пересортировка в случае повторной попытки отключения
					}
				}
			*/

			ErrText = MyModel->FindU();

			if (ErrText != "")
				cout << ErrText;	//Переписать на нормальный Try/Catch или что-то такое
				//Warning(ErrText, 1);

		}
		//------------------------------------------------------------------------------//
		FirstCount = false;
	}

	//Вывод результата пока здесь
	int ParNum = 0; //Костыль, должно быть выводимое значение
	vector<double> P;	//Результат расчета
	double GrafTime;	//Время для графика
	for (j = 1; j <= StepNum / ResMultiple ; j++)   //цикл по кол-ву шагов
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