#pragma once
#include "Branch.h"
#include "Model.h"
#include "Calculator.h"
#include <istream>
#include <string>
#include <vector>

class EDS : public Branch //класс характеризует потребители-компрессоры
{
public:

	EDS(int BranchCount, vector<string> DataString);
	~EDS();

	//Чтение данных из файла
	void FillData(vector<string> line);


	//Создание списка параметров
	void FillArgumentList();

	//Запись данных в таблицу
	void WriteToTable(int BranchCount);

	//Коррекция данных
	void ChaingeData();

	// Функция вычисления составляющих для определения напряжения в узле
	void FindNodeI(int NodeIndex, double*);

	// Функция получения волнового сопротивления узла.
	void GetBranchG(double*);

	//Передача значения узлового напряжения
	double GetVoltageClass();

	//Заполнение начальных условий
	void InitialConditions(bool);

private:

	int  PhaseType;        //кол-во фаз
	double VoltageClass,   //Класс напряжения
		X,              //реактивное сопротивление прямой последовательности
		X0,             //реактивное сопротивление нулевой последовательности
		R,              //активное последовательное сопротивление прямой последовательности
		R0,             //активное последовательное сопротивление нулевой последовательности
		Rg,             //активное параллельное сопротивление прямой последовательности
		Rg0,            //активное параллельное сопротивление нулевой последовательности
		Em,             //амплитуда Э.Д.С. (в о.е.)
		InitialAngle,   //начальный угол фазы А (эл.град.)
		InitAngleRad,
		ShuntingTime,   //время шунтирования R
		E[3];           //ЭДС на текущем шаге

};
