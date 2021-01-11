#include "EDS.h"
#include <iostream>

using namespace std;

extern Model* MyModel;
extern double Step;
extern int CurrentStep;
extern double PI;
extern double EmFactor;
EDS::EDS(int BranchCount, vector<string> line)
{
	BranchIndex = BranchCount;
	ElementType = 3;
	VoltageClass = 0;
	X = 0;
	X0 = 0;
	R = 0;
	R0 = 0;
	Rg = 0;
	Rg0 = 0;
	Em = 0;
	InitialAngle = 0;
	ShuntingTime = 0;
	InitAngleRad = 0;
	PhaseType = 3;

	for (i = 0; i < 3; i++)
		E[i] = 0;

	FillData(line);
	//WriteToTable(BranchCount);
}


//Деструктор
EDS::~EDS()
{

}

//Чтение данных
void EDS::FillData(vector<string> line)
{

	if (line[2] == "0") Enabled = false;
	BranchName = line[3];
	BegingNode = stoi(line[4]);
	VoltageClass = stod(line[5]);
	X = stod(line[6]);
	X0 = stod(line[7]);
	R = stod(line[8]);
	R0 = stod(line[9]);
	Rg = stod(line[10]);
	Rg0 = stod(line[11]);
	Em = stod(line[12]);
	InitialAngle = stod(line[13]);
	ShuntingTime = stod(line[14]);
	PhaseType = stoi(line[15]);

}
//---------------------------------------------------------------------------
// Функция вычисления составляющей волны от ЭДС в узле.
void EDS::FindNodeI(int NodeIndex, double J[3])
{
	vector<double> Ubeging;
	double Epast[3], IsumPast[3];

	Ubeging = MyModel->NodeList[NodeIndex]->GetU();

	for (i = 0; i < 3; i++)
	{
		Epast[i] = E[i];
		IsumPast[i] = (Ubeging[i] - Epast[i]) / Zrab[i];
	}

	// ЭДС текущего шага
	switch (PhaseType)
	{
	case 1:  // Однофазный (фаза)
		E[0] = Em * sin(Step * CurrentStep + InitAngleRad); E[1] = 0; E[2] = 0;
		break;

	case 2:  // Постоянный (полюс)
		E[0] = 0; E[1] = 0; E[2] = 2 * Em / sqrt(3);
		break;

	default: // Трехфазный
		E[1] = Em * sin(Step * CurrentStep + InitAngleRad);
		E[2] = -Em * cos(Step * CurrentStep + InitAngleRad);
		E[0] = 0;
	}

	for (i = 1; i < 3; i++)
		E[i] = E[i] * EmFactor; //установление режима

	   //Uxx2[1] = E[1]; Uxx2[2] = E[2]; Uxx2[0] = E[0];

	   //    (2*X/Step - R) для положительного направления Isum к узлу
	Urab[1] = E[1]; //+ Epast[1] + IsumPast[1]*(R - 2*X/Step) - Ubeging[1];
	Urab[2] = E[2]; //+ Epast[2] + IsumPast[2]*(R - 2*X/Step) - Ubeging[2];
	Urab[0] = E[0]; // + Epast[0] + IsumPast[0]*(R - 2*X0/Step) - Ubeging[0];

	//Urab[1] = E[1] + Epast[1] - IsumPast[1]*(2*X/Step + R) - Ubeging[1];
	//Urab[2] = E[2] + Epast[2] - IsumPast[2]*(2*X/Step + R) - Ubeging[2];
	//Urab[0] = E[0] + Epast[0] - IsumPast[0]*(2*X0/Step + R0) - Ubeging[0];

	for (i = 0; i < 3; i++)
		J[i] = Urab[i] * Grab[i];   // J[i] = Urab[i]/Zrab[i];
}
//---------------------------------------------------------------------------
// Функция получения волнового сопротивления ветви.
void EDS::GetBranchG(double G[3])
{
	G[1] = Step / (R * Step + 2 * X);
	//Zrab[1] = R + 2*X/Step;
	G[2] = G[1];
	G[0] = Step / (R0 * Step + 2 * X0);

	for (i = 0; i < 3; i++)
	{
		Grab[i] = G[i];
		Zrab[i] = 1 / Grab[i]; // в случае узловых элементов Zrab = Zв
	}
}
//---------------------------------------------------------------------------
//Передача значения узлового напряжения
double EDS::GetVoltageClass()
{
	return VoltageClass;
}
//---------------------------------------------------------------------------
//Заполнение начальных условий
void EDS::InitialConditions(bool FirstFill)
{
	vector<double> J0;

	Branch::InitialConditions(FirstFill);  // Запуск метода родительского класса

	InitAngleRad = InitialAngle * PI / 180;

	switch (PhaseType)
	{
	case 1:  // Однофазный
		E[0] = Em * sin(InitAngleRad); E[1] = 0; E[2] = 0;
		break;

	case 2:  // Постоянный
		E[0] = Em; E[1] = 0; E[2] = 0;
		break;

	default: // Трехфазный
		E[1] = Em * sin(InitAngleRad);
		E[2] = -Em * cos(InitAngleRad);
		E[0] = 0;
	}

	/*    if (EnterForm->ComboBox7->ItemIndex != 3)
		 {
		  if (EnterForm->ComboBox7->ItemIndex == 1) // Однофазный
		   {
			E[0] = Em*sin(InitAngleRad); E[1] = 0; E[2] = 0;
		   }
		  if (EnterForm->ComboBox7->ItemIndex == 2) // Постоянный однофазный
		   {
			E[0] = Em;                   E[1] = 0; E[2] = 0;
		   }
		 }
		else
		 {
		  E[1] = Em*sin(InitAngleRad);
		  E[2] = -Em*cos(InitAngleRad);
		  E[0] = 0;
		 }           */

	for (i = 1; i < 3; i++)
		E[i] = E[i] * EmFactor;

	for (i = 0; i < 3; i++)
		J0.push_back(E[i] * Grab[i]);   //J0[i] = E[i]/Zrab[i];

	MyModel->NodeList[BegingNodeIndex]->Uxx0Calculation(J0);
}
//---------------------------------------------------------------------------
