#include "Line.h"
#include "Model.h"

extern Model* MyModel;
extern double Frequency;
extern double Omega;
extern vector<double> abc_AB0(vector<double> abc);
extern vector<double> AB0_abc(vector<double> AB0);
//Конструктор 1
Line::Line()
{
}

//Конструктор 2
Line::Line(int BranchCount, vector<string> DataString)
{
	BranchIndex = BranchCount;
	ElementType = 1;
	VoltageClass = 0;
	Length = 0;
	L = 0;
	L0 = 0;
	C = 0;
	C0 = 0;
	R = 0;
	R0 = 0;
	// PhaseType = 3;

	for (i = 0; i < 3; i++)
	{
		BegingWave[i] = 0;
		EndWave[i] = 0;
		InitVoltage[i] = 0;
	}

	WaveImpendance = 0;
	WaveImpendance0 = 0;
	WaveLength = 0;
	WaveLength0 = 0;

	PBayModel = false;
	SingleBay = true;

	ChannelList.reserve(3);

	FillData(DataString);

	// WriteToTable(BranchCount);

	if (!PBayModel)
		ChannelCreate();
}

//Деструктор
Line::~Line()
{
	ChannelList.clear();
	ArgumentList.clear();
}

//Чтение данных из файла
void Line::FillData(vector<string> line)
{
	if (line[2] == "0") Enabled = false;
	BranchName = line[3];
	BegingNode = stoi(line[4]);
	EndNode = stoi(line[5]);
	VoltageClass = stod(line[6]);
	Length = stod(line[7]);
	L = stod(line[8]);
	L0 = stod(line[9]);
	C = stod(line[10]);
	C0 = stod(line[11]);
	R = stod(line[12]);
	R0 = stod(line[13]);
	InitVoltage[1] = stod(line[14]);
	InitVoltage[2] = stod(line[15]);
	InitVoltage[0] = stod(line[16]);
	PhaseType = stoi(line[17]);

	if (line[18] == "1")
		SingleBay = true;
	else
		SingleBay = false;
	/*Ошибка out of range. Не нужный параметр?
	if (line[19] == "1")
		PBayModel = true;
	else
		PBayModel = false;
	Virtual = PBayModel;
	*/

	WaveImpendance = 1000 * sqrt(L / C) + R / 2;
	WaveImpendance0 = 1000 * sqrt(L0 / C0) + R0 / 2;
	WaveLength = 360 * Frequency * Length * sqrt(L * C) / 1000000;
	WaveLength0 = 360 * Frequency * Length * sqrt(L0 * C0) / 1000000;


	//FillArgumentList();
}

// Создание каналов прямой обратной и нулевой последовательности
void Line::ChannelCreate()
{ // нулевая - 1,2,3 фазы; прямая - 2,3 фазы; обратная - 3 фазы;
	for (i = 0; i < PhaseType; i++)
		ChannelList.push_back(new Channel(i));
}
//---------------------------------------------------------------------------
// Функция вычисления составляющих для определения напряжения в узле.
void Line::FindNodeI(int NodeIndex, double J[3])
{
	bool NodeBeging;

	if (MyModel->NodeList[NodeIndex]->NodeNum == BegingNode) NodeBeging = true;
	else NodeBeging = false;

	for (i = 0; i < ChannelList.size(); i++)
	{
		Urab[i] = 2 * ChannelList[i]->GetV(NodeBeging);

		if (NodeBeging)
			BegingWave[i] = Urab[i];
		else
			EndWave[i] = Urab[i];

		J[i] = Urab[i] * Grab[i];
		//J[i] = 2*((Channel*)ChannelList->Items[i])->GetV(NodeBeging) * Grab[i];   // J = 2V/Zв
	}
}
//---------------------------------------------------------------------------
// Функция передачи волнового сопротивления узла.
void Line::GetBranchG(double G[3])
{
	Zrab[1] = WaveImpendance;
	Zrab[2] = WaveImpendance;
	Zrab[0] = WaveImpendance0;

	for (i = 0; i < 3; i++)
	{
		Grab[i] = 1 / Zrab[i]; // в случае узловых элементов Zrab = Zв, Grab = Gв
		G[i] = Grab[i];
	}
}
//---------------------------------------------------------------------------
// Запись напряжения узла в канал и "продергивание" волны в канале.
void Line::WaveMove()
{
	vector<double> Ubegin;
	vector<double> Uend;

	Ubegin = MyModel->NodeList[BegingNodeIndex]->GetU();
	Uend = MyModel->NodeList[EndNodeIndex]->GetU();

	for (i = 0; i < ChannelList.size(); i++)
		ChannelList[i]->ChannelWaveMove(Ubegin[i], Uend[i]);
}
//---------------------------------------------------------------------------
// Вывод волны на график
void Line::GetBranchV(double V[3])
{
	for (i = 0; i < ChannelList.size(); i++)
		V[i] = ChannelList[i]->GetVForGraf();
}
//---------------------------------------------------------------------------
//Начальные условия (Заполнение каналов начальными значениями волн)
void Line::InitialConditions(bool FirstFill)
{
	vector<double> U0;
	vector<double> J0;
	vector<double> Half_InitVoltage;
	double  NewWaveLength;

	for (int i = 0; i < 3; i++)
	{
		U0.push_back((double)0);
		J0.push_back((double)0);
		Half_InitVoltage.push_back((double)0);
	}

	Branch::InitialConditions(FirstFill);

	NewWaveLength = 360 * Frequency * Length * sqrt(L * C) / 1000000;

	if ((WaveLength != NewWaveLength) && (!FirstFill))
	{
		for (i = 0; i < ChannelList.size(); i++)
			ChannelList[i]->ChannelClear();
		FirstFill = true; // Перезаполнение каналов линии

		WaveLength = NewWaveLength;
		WaveLength0 = 360 * Frequency * Length * sqrt(L0 * C0) / 1000000;
	}


	for (i = 0; i < ChannelList.size(); i++)
	{
		if (ChannelList[i]->ChannelType == 0)  // канал нулевой последовательности
			ChannelList[i]->StepsInWaveMove(WaveLength0, R0, L0);
		else   // канал прямой и обратной последовательности
			ChannelList[i]->StepsInWaveMove(WaveLength, R, L);
	}

	// Запись начальных условий
	if ((InitVoltage[1] != 0) || (InitVoltage[2] != 0) || (InitVoltage[0] != 0))
	{
		for (i = 0; i < 3; i++)
		{
			cout << InitVoltage[i];
			Half_InitVoltage[i] = InitVoltage[i] / 2;
		}

		U0 = abc_AB0(Half_InitVoltage);
	}
	else
		for (i = 0; i < 3; i++)
			U0[i] = 0;

	for (i = 0; i < 3; i++)
		J0[i] = U0[i] * Grab[i];

	if (BegingNodeIndex != 0)
		MyModel->NodeList[BegingNodeIndex]->Uxx0Calculation(J0);
	if (EndNodeIndex != 0)
		MyModel->NodeList[EndNodeIndex]->Uxx0Calculation(J0);

	if (FirstFill)
		for (i = 0; i < ChannelList.size(); i++)
			ChannelList[i]->FillVList(U0[i]);
	else
		for (i = 0; i < ChannelList.size(); i++)
			ChannelList[i]->NextFillVList(U0[i]);
}
//---------------------------------------------------------------------------
//Передача значения узлового напряжения
double Line::GetVoltageClass()
{
	return VoltageClass;
}
//---------------------------------------------------------------------------
