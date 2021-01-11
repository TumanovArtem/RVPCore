#include "Model.h"

#include <iostream>
#include <fstream>

using namespace std;

extern double Omega;
extern double Step;
extern int StepNum;
extern int CurrentStep;
extern double ResMultiple;

extern Calculator* Calc;

extern vector<double> abc_AB0(vector<double> abc);
extern vector<double> AB0_abc(vector<double> AB0);

Model::Model(vector<string> data)
{
	BlockCount = 0;
	BranchCount = 0;
	NodeCount = 0;
	CountGroupsNum = 1;
	ResStep = 1;
	GrafStep = 0;

	BlockList.reserve(100);
	BranchList.reserve(100);;
	NodeList.reserve(100);
	EventList.reserve(20);
	ResultList.reserve(100);
	IterationCount.reserve(1000);	//Переписать как простой int array?

	Load_Data(data);
}

void Model::Load_Data(vector<string> data)
{
	//Переменные для парсинга строк
	vector<string> line;
	line.reserve(20);
	string Parametr, 
			delimiter;		//Разделитель
	size_t pos = 0; //Позиция разделителя

	int i, j, k, BranchSign, BlockSign, BranchCount, BlockCount, PhaseNum;

	this->NodeList.push_back(new Node(0, 0, 0, 3, 0));	//Добавление нулевого узла (земли)
	this->BranchList.push_back(new Branch());	//Не знаю какой элемент был у Василия Владимировича нулевым, создадим пустой

	delimiter = ";";
	for (i = 0; i < data.size(); i++)
	{
		line.push_back("");//Костыль, чтобы заполнить нулевой элемент. У Василия Владимировича начинается с 1
		//Разбиваем строку данных по разделителю ;		TO DO сделать через xml
		while ((pos = data[i].find(delimiter)) != string::npos)
		{
			line.push_back( data[i].substr(0, pos) );	//Добавляем лексему в массив
			data[i].erase(0, pos + 1);			//Удаляем лексему вместе с разделителем
		}
		
		if (line.size() != 0)
		{
			BranchSign = stoi(line[1]);
			if (BranchSign == 0)  // Если строка - идентификатор блока
			{
				BlockSign = stoi(line[2]);
				this->BlockCount++;
				switch (BlockSign)
				{
				case 0:  // ВЛ
					this->BlockList.push_back(new Block(line, this->BlockCount)); //Добавление блока элементов
					break;
				case 1:  // ВЛ
					this->BlockList.push_back(new Block(line, this->BlockCount)); //Добавление блока элементов
					break;
				case 2:  // CRG
					//this->BlockList->Add(new B_trans2(GetString, BlockCount)); //Добавление блока элементов
					break;
				}
			}
			else
			{
				this->BranchCount++;
			}

			switch (BranchSign)
			{
				//case 0:  // строка со списком параметров для графика  break;
			case 1:  // ВЛ
				BranchList.push_back(new Line(this->BranchCount, line));
				PhaseNum =this->BranchList[this->BranchCount]->PhaseType;
				/*
				// Mоделирование линии П-ячейками
				if (this->BranchList[this->BranchCount]->PBayModel)
				{
					GetString = PiCircuitData(BranchCount);

					MyModel->BranchList->Add(new CRG(BranchCount, true, OldFormatFile, GetString));
					MyModel->NodeList->Add(new Node(((Branch*)MyModel->BranchList->Items[MyModel->BranchCount])->BegingNode, ((Branch*)MyModel->BranchList->Items[MyModel->BranchCount])->GetVoltageClass(), MyModel->BranchCount, PhaseNum, MyModel->NodeList->Count - 1));

					for (k = ((Branch*)MyModel->BranchList->Items[MyModel->BranchCount])->BegingNode + 1; k <= ((Branch*)MyModel->BranchList->Items[MyModel->BranchCount])->EndNode; k++) //по символам в строке
					{
						MyModel->BranchCount++;
						BranchCount = MyModel->BranchCount;
						MyModel->BranchList->Add(new RL(BranchCount, true, OldFormatFile, GetString));

						MyModel->BranchCount++;
						BranchCount = MyModel->BranchCount;
						MyModel->BranchList->Add(new CRG(BranchCount, true, OldFormatFile, GetString));

						MyModel->NodeList->Add(new Node(k, ((Branch*)MyModel->BranchList->Items[MyModel->BranchCount])->GetVoltageClass(), MyModel->BranchCount, PhaseNum, MyModel->NodeList->Count - 1));
					}
				}
				*/
				break;
			case 2:  // CRG
				//MyModel->BranchList->Add(new CRG(BranchCount, true, OldFormatFile, GetString));
				break;
			case 3:  // ЭДС
				this->BranchList.push_back(new EDS(this->BranchCount, line));
				break;
			case 4:  // RL
				//MyModel->BranchList->Add(new RL(BranchCount, true, OldFormatFile, GetString));
				break;
			case 5:  // ОПН
				//MyModel->BranchList->Add(new OPN(BranchCount, true, OldFormatFile, GetString));
				break;
			case 6:  // Ист.тока
				//MyModel->BranchList->Add(new CurSource(BranchCount, true, OldFormatFile, GetString));
				break;
			case 7:  // Шунт
				//MyModel->BranchList->Add(new Shunt(BranchCount, true, OldFormatFile, GetString));
				break;
			case 8:  // RC
				//MyModel->BranchList->Add(new RC(BranchCount, true, OldFormatFile, GetString));
				break;
			case 9:  // Rф
				//MyModel->BranchList->Add(new Rf(BranchCount, true, OldFormatFile, GetString));
				break;
			case 10:  // СГ
				//MyModel->BranchList->Add(new Generator(BranchCount, true, OldFormatFile, GetString));
				break;
			case 11:  // Преобразователь
				//MyModel->BranchList->Add(new Converter(BranchCount, true, OldFormatFile, GetString));
				break;
			}

			if (BranchSign != 0)  // Добавление элементов в список элементов блока
			{                                            //т.к. нумерация блоков начинается с 0
				this->BlockList[this->BlockCount - 1]->BlockBranchList.push_back(this->BranchList[this->BranchList.size() - 1]);

				this->CreateNode(BranchSign);

				this->BranchList[this->BranchList.size() - 1]->BlockNum = this->BlockCount;
			}

			line.clear();
		}
	}

}

Model::~Model()
{
	BranchList.clear();
	NodeList.clear();
	EventList.clear();
	ResultList.clear();
}

int Model::FindNodeIndex(int BranchIndex, int NodeNumber)
{
	int i,
		Index = 0;

	for (i = 0; i < NodeList.size(); i++)
	{
		if ((NodeList[i]->NodeNum == NodeNumber) &&
			((BranchList[BranchIndex]->BegingNode == NodeNumber) || (BranchList[BranchIndex]->EndNode == NodeNumber)))
		{
			Index = i;
		}
	}

	return Index;
}

void Model::FillBranchesListInNode()
{
	int NodeToFind, i, j, k;
	int BegingIndex, EndIndex, NodeIndex, BranchIndex;
	bool NextGroup = false;

	for (j = 0; j < BlockList.size(); j++)	//по блокам
	{
		if (BlockList[j]->BlockCountGroup == 0)
		{
			for (i = 0; i < BlockList[j]->BlockNodeList.size(); i++)
			{
				if (BlockList[j]->BlockNodeList[i]->External)		//ЗАЧЕМ???
				{
					NodeIndex = BlockList[j]->BlockNodeList[i]->NodeIndex;
					for (k = 0; k < NodeList[NodeIndex]->BranchesInNode.size(); k++) // по ветвям, подключенным к внешнему узлу
					{
						BranchIndex = NodeList[NodeIndex]->BranchesInNode[k];
						if (BranchList[k]->BlockNum - 1 != j)		//ЗАЧЕМ??
							if (BranchList[BranchIndex]->ElementType == 1)
								NextGroup = true;
							else
								NextGroup = false;
					} // конец цикла по ветвям, присоединенным к узлу
					BlockList[j]->BlockCountGroup = CountGroupsNum;
					if (NextGroup)
					{
						CountGroupsNum++;
						NextGroup = false;
					}
				} // конец цикла по блокам

			for (i = 1; i <= BranchCount; i++) // Заполнение +BranchesInNode
				if (BranchList[i]->Enabled)
				{
					BegingIndex = FindNodeIndex(i, (BranchList[i]->BegingNode));
					EndIndex = FindNodeIndex(i, BranchList[i]->EndNode);

					BranchList[i]->BegingNodeIndex = BegingIndex;
					BranchList[i]->EndNodeIndex = EndIndex;
				}
			}
		}
	}
}

void Model::FindZ()
{
	int i;
	for (i = 0; i < NodeList.size(); i++) {
		NodeList[i]->FindNodeZ();
	}
}

//Основной цикл алгоритма
string Model::FindU()
{

	int i, j, k, BegingNodeIndex, EndNodeIndex, ItCount, MaxIter;
	bool NextCount, IsFirstIter;
	int CountGroup = 0;

	//1 этап. Нахождение первоначального напряжения в узлах
	for (i = 1; i < NodeList.size(); i++)
	{
		NodeList[i]->FindNodeU();
		NodeList[i]->NodeNextCount = true;
	}

	/*
	//Подготовительные вычисления в начале расчета и при коммутациях
	for (i = 1; i <= BranchCount; i++)
		if ((BranchList[i]->Enabled) && ((BranchList[i]->ChangePaseState) || (IterationCount.size() == 0)))
			switch (BranchList[i]->ElementType)
			{
			case 4:  // RL - расчет коэффициентов для аналитического решения
			case 8:  // RC - расчет коэффициентов для аналитического решения
			case 9:  // Rф - формирование матрицы проводимостей и коммутационной матрицы
				BranchList[i]->FillZrab_M();
				BranchList[i]->ChangePaseState = false;
				break;
			}
	*/
	
	//2 этап. Расчет токов через неузловые элементы (с коррекцией суммарного тока в узлах)
	for (k = 0; k < CountGroupsNum; k++)	//По расчетным группам
	{
		CountGroup++;
		NextCount = true;
		IsFirstIter = true;
		ItCount = -1;
		MaxIter = 500;

		try
		{
			while (NextCount)
			{
				NextCount = false;
				ItCount++;

				if (ItCount != 0)
					IsFirstIter = false;

				if (ItCount > MaxIter)
				{
					cout << "Число итераций превысило " << MaxIter << "на " << CurrentStep << " шаге расчета";
					MaxIter += 500;
				}

				for (j = 0; j < BlockList.size(); j++)
				{
					if (CountGroup == BlockList[j]->BlockCountGroup)
					{
						for (i = 0; i < BlockList[j]->BlockBranchList.size(); i++)  // по ветвям блока
						{
							BegingNodeIndex = BlockList[j]->BlockBranchList[i]->BegingNodeIndex;
							EndNodeIndex = BlockList[j]->BlockBranchList[i]->EndNodeIndex;
							if ((BlockList[j]->BlockBranchList[i]->Enabled) && ((NodeList[BegingNodeIndex]->NodeNextCount) || (NodeList[EndNodeIndex]->NodeNextCount)))
							{
								switch (BlockList[j]->BlockBranchList[i]->ElementType)
								{
									case 4:  // RL
										//BlockList[j]->BlockBranchList[i]->FindIAB0(IsFirstIter);
											break;
									case 5:  // ОПН
										//if (BlockList[j]->BlockBranchList[i]->OPNType != 2)
										//	BlockList[j]->BlockBranchList[i]->FindIFormU0(IsFirstIter);
										//else
										//	BlockList[j]->BlockBranchList[i]->FindIFormU0M(IsFirstIter);
										break;
									case 6:  // J(t)
										//BlockList[j]->BlockBranchList[i]->FindIForm(IsFirstIter);
										break;
									case 7:  // Шунт
										//BlockList[j]->BlockBranchList[i]->FindIAB0(IsFirstIter);
										break;
									case 8:  // RC
										//BlockList[j]->BlockBranchList[i]->FindIAB0(IsFirstIter);
										break;
									case 9:  // Rф
										//BlockList[j]->BlockBranchList[i]->FindIMatrix(); //FindIAB0(false);
										break;
									case 10:  // СГ
										//BlockList[j]->BlockBranchList[i]->FindIAB0(IsFirstIter);
										break;
									case 11:  // Преобразователь
										//BlockList[j]->BlockBranchList[i]->FindIAB0(IsFirstIter);
										break;
								}
							}
						}
						//3------Вычисление напряжения в узлах, проверка разницы напряджения на итерациях.
						for (i = 0; i < BlockList[j]->BlockNodeList.size(); i++)    // по узлам блока
						{
							BlockList[j]->BlockNodeList[i]->NodeNextCount = BlockList[j]->BlockNodeList[i]->UCorrection();
							if (BlockList[j]->BlockNodeList[i]->NodeNextCount)
								NextCount = true;
						}
					}
				}
			}
		}
		catch (...)
		{
			return("Расходимость вычислительного процесса");
		}
	}

	IterationCount.push_back(ItCount);
	/*
	//3 этап. Проверка пробоя искрового присоединения реактора или разрядника
	for (i = 1; i <= BranchCount; i++)
		if (((((Branch*)BranchList->Items[i])->ElementType == 5) && (((OPN*)BranchList->Items[i])->OPNType == 2)) ||
			((((Branch*)BranchList->Items[i])->ElementType == 4) && (((RL*)BranchList->Items[i])->RLType == 3)))
			((Branch*)BranchList->Items[i])->CheckBreakdown();
	*/
	//4 этап. "Продергивание" волны
	for (i = 1; i <= BranchCount; i++)
	{
		if ((BranchList[i]->ElementType == 1) && !(BranchList[i]->Virtual))
		{
			BranchList[i]->WaveMove();
		}
	}

	//5 этап. Запись результата расчета на шаге в массивы.
	
	if (CurrentStep == ResStep)
	{
		ResStep += ResMultiple;
		Calc->ErrText = FillResult();
		if (Calc->ErrText != "")
			return(Calc->ErrText);
	}

	return "";
}

void Model::SortEventsList(int SwitchEvent)
{
	/*TO DO*/
}

bool Model::CheckModel()
{
	/*TO DO*/
	/*Проверка связности сети*/
	return true;
}

//Создание узлов
void Model::CreateNode(int BranchSign)
{
	bool CreateNewNode1 = true;
	bool CreateNewNode2 = true;
	int i, j;

	if ((BranchSign == 1) || (BranchSign == 2) || (BranchSign == 3) || (BranchSign == 10))
	{
		for (i = 0; i < NodeList.size(); i++)
		{
			if (NodeList[i]->NodeNum == BranchList[BranchList.size() - 1]->BegingNode)
			{
				CreateNewNode1 = false;
				BranchList[BranchList.size() - 1]->BegingNodeIndex = i;
				NodeList[i]->BranchesInNode.push_back(BranchList.size() - 1);
			}
			if ((BranchSign == 1) && ((NodeList[i]->NodeNum == BranchList[BranchList.size() - 1]->EndNode)))
			{
				CreateNewNode2 = false;
				BranchList[BranchList.size() - 1]->EndNodeIndex = i;
				NodeList[i]->BranchesInNode.push_back(BranchList.size() - 1);
			}
		}

		if (CreateNewNode1)
		{
			NodeList.push_back(new Node(BranchList[BranchList.size() - 1]->BegingNode, // NodeList->Count т.к. узла еще не создано
				BranchList[BranchList.size() - 1]->GetVoltageClass(), BranchList.size() - 1, 3, NodeList.size()));
			BlockList[BlockCount - 1]->BlockNodeList.push_back(NodeList[NodeList.size() - 1]);

			BranchList[BranchList.size() - 1]->BegingNodeIndex = NodeList.size() - 1;
			NodeList[NodeList.size() - 1]->BranchesInNode.push_back(BranchList.size() - 1);

			for (j = 0; j < BlockList[BlockCount - 1]->ExtBlockNode.size(); j++)
				if (stoi(BlockList[BlockCount - 1]->ExtBlockNode[j]) == NodeList[NodeList.size() - 1]->NodeNum)		//Почему ExtBlockNode типа string???? TO DO
					NodeList[NodeList.size() - 1]->External = true;
		}

		if ((CreateNewNode2) && (BranchSign == 1))
		{
			NodeList.push_back(new Node(BranchList[BranchList.size() - 1]->EndNode, // NodeList->Count т.к. узла еще не создано
				BranchList[BranchList.size() - 1]->GetVoltageClass(), BranchList.size() - 1, 3, NodeList.size()));
			BlockList[BlockCount - 1]->BlockNodeList.push_back(NodeList[NodeList.size() - 1]);

			BranchList[BranchList.size() - 1]->EndNodeIndex = NodeList.size() - 1;
			NodeList[NodeList.size() - 1]->BranchesInNode.push_back(BranchList.size() - 1);
		}
	}
	//Вычисление отношеня номинальных напряжений ветви
	if ((NodeList[BranchList[BranchList.size() - 1]->BegingNodeIndex]->Ubase != 0) && (NodeList[BranchList[BranchList.size() - 1]->EndNodeIndex]->Ubase != 0))
		BranchList[BranchCount]->UnomRato = NodeList[BranchList[BranchList.size() - 1]->BegingNodeIndex]->Ubase / NodeList[BranchList[BranchList.size() - 1]->EndNodeIndex]->Ubase;
}

//Вывод информации о ходе расчета
string Model::FillResult()
{
	int j, i, BranchN;
	string LogText;
	vector<double> P, Pabc;

	float* NewFloatRef;

	for (j = 0; j < ResultList.size(); j++)
	{
		try  // проверка исключения bad_alloc
		{
			switch (ResultList[j]->ParametrCode)
			{
			case 1:  //U
				P = NodeList[ResultList[j]->ParametrOwner]->GetU();
				Pabc = AB0_abc(P);
				break;
			case 2:  //I
				for (i = 0; i < 3; i++)
					P.push_back(BranchList[ResultList[j]->ParametrOwner]->I[i]);
				Pabc = AB0_abc(P);
				break;
			/*case 3:  //V
				for (i = 0; i < 3; i++)
					Pabc[i] = ((Line*BranchList->Items[((Result*)ResultList->Items[j])->ParametrOwner])->BegingWave[i];
				//AB0_abc(P, Pabc);
				break;
			case 4:  //R (OPN)
				((Result*)ResultList->Items[j])->InternalItCount = ((Branch*)BranchList->Items[((Result*)ResultList->Items[j])->ParametrOwner])->InternalItCount;
				for (i = 0; i < 3; i++)
					Pabc[i] = ((OPN*)BranchList->Items[((Result*)ResultList->Items[j])->ParametrOwner])->R[i];
				break;
			case 5:  //J
				for (i = 0; i < 3; i++)
					Pabc[i] = ((CurSource*)BranchList->Items[((Result*)ResultList->Items[j])->ParametrOwner])->Jtabc[i];
				break;
			case 6:  //Ф
				for (i = 0; i < 3; i++)
					Pabc[i] = ((Shunt*)BranchList->Items[((Result*)ResultList->Items[j])->ParametrOwner])->Flowabc[i];
				break;
			case 7:  //Uв = U1-U2 падение напряжения на ветви
				BranchN = ((Result*)ResultList->Items[j])->ParametrOwner;
				((Node*)NodeList->Items[((Branch*)BranchList->Items[BranchN])->BegingNodeIndex])->GetU(P);
				((Node*)NodeList->Items[((Branch*)BranchList->Items[BranchN])->EndNodeIndex])->GetU(Pabc);

				for (i = 0; i < 3; i++)
					P[i] = P[i] - Pabc[i];
				AB0_abc(P, Pabc);
				break;
			case 8:  //Iн - ток в начале линии Iн =(2Vн-Uн)/Zв
				BranchN = ((Result*)ResultList->Items[j])->ParametrOwner;
				((Node*)NodeList->Items[((Branch*)BranchList->Items[BranchN])->BegingNodeIndex])->GetU(P);
				for (i = 0; i < 3; i++)
					P[i] = (P[i] - ((Line*)BranchList->Items[BranchN])->BegingWave[i])
					* ((Branch*)BranchList->Items[BranchN])->Grab[i];
				AB0_abc(P, Pabc);
				break;
			case 9:  //Iк - ток в конце линии Iк =(2Vк-Uк)/Zв
				BranchN = ((Result*)ResultList->Items[j])->ParametrOwner;
				((Node*)NodeList->Items[((Branch*)BranchList->Items[BranchN])->EndNodeIndex])->GetU(P);
				for (i = 0; i < 3; i++)
					P[i] = (P[i] - ((Line*)BranchList->Items[BranchN])->EndWave[i])
					* ((Branch*)BranchList->Items[BranchN])->Grab[i];
				AB0_abc(P, Pabc);
				break;*/
			}
			/*
			for (i = 0; i < 3; i++)
			{
				//i = MaxInt;
				if (CurrentStep == 59999)
				{
					ErrText = "ddff";
				}
				NewFloatCount++;
				NewFloatRef = new float;
				*NewFloatRef = Pabc[i];  // Out of memory > 400 мСек (> 200 при продолжении расчета)
				((TList*)((Result*)ResultList->Items[j])->DataList->Items[i])->Add(NewFloatRef);
			}*/
			ResultList[j]->DataList[0].push_back(Pabc[0]);
			ResultList[j]->DataList[1].push_back(Pabc[1]);
			ResultList[j]->DataList[2].push_back(Pabc[2]);
		}
		catch (... /*std::bad_alloc*/)
		{
			//ErrText = "Переполнение памяти при записи параметра " + ((Result*)ResultList->Items[j])->ParametrName + " на шаге " + IntToStr(CurrentStep);
			//cout << ErrText.c_str();
			//Warning( ErrText, 1);
			//return(ErrText);
		}
		return("");
	}
}