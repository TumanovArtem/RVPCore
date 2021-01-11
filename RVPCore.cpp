// RVPCore.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include "Model.h"
#include "Calculator.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

Model* MyModel;
Calculator* Calc;

//Переменные для парсинга
vector<string> ModelData;
string str;

// TO DO временный костыль, необходимо создать файл конфига. Здесь дефолтные значения
double	ResMultiple,
		StepMks,
		Frequency,
		PI,
		Omega,
		Epsilon,
		Step, //шаг расчета в радианах (10 мкС по умолчанию)
		CurrentTime,
		EmFactor;

int CountTime,
	StepNum, //Общее количество шагов
	SteadyStateTime,
	CurrentStep;

vector<double> abc_AB0(vector<double> abc);
vector<double> AB0_abc(vector<double> AB0);

int main()
{
	ResMultiple = 5;
	StepMks = 10;
	Frequency = 50;
	PI = 3.1415926535897932384626433832795;
	Omega = 2 * PI * Frequency;
	Epsilon = 0.001;
	Step = StepMks * 0.000001 * Omega; //шаг расчета в радианах (10 мкС по умолчанию)
	CurrentTime = 0;
	EmFactor = 1;

	CountTime = 400;
	StepNum = CountTime * 1000 / StepMks; //Общее количество шагов
	SteadyStateTime = 0;
	CurrentStep = 0;

	//Подготовка исходных данных. Входным параметром конструктора Model() должен быть вектор строк 
	ifstream file("EDS_LINE_SW.rvp");
	while (getline(file, str))
	{
		ModelData.push_back(str);
	}

	MyModel = new Model(ModelData);
	Calc = new Calculator();
		Calc->Calculate();
	return 0;
}

// Преобразование ABC в alpha,betta,0
vector<double> abc_AB0(vector<double> abc)
{
	vector<double> AB0;
	AB0.push_back((abc[1] + abc[2] + abc[0]) / 3);			// нулевая п-ть
	AB0.push_back((2 * abc[1] - abc[2] - abc[0]) / 3);		// прямая п-ть
	AB0.push_back((abc[2] - abc[0]) / sqrt(3));				// обратная п-ть
	return AB0;
}
//---------------------------------------------------------------------------
// Преобразование alpha,betta,0 в ABC
vector<double> AB0_abc(vector<double> AB0)
{
	vector<double> abc;
	abc.push_back((2 * AB0[0] - AB0[1] - sqrt(3) * AB0[2]) / 2);// фаза C
	abc.push_back(AB0[0] + AB0[1]);// фаза А
	abc.push_back((2 * AB0[0] - AB0[1] + sqrt(3) * AB0[2]) / 2);// фаза B
	return abc;
}