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

	double VoltageClass,      //Класс напряжения
		Length,            //Длина линии
		L,                 //индуктивность прямой последовательности
		L0,                //индуктивность нулевой последовательности
		C,                 //емкость прямой последовательности
		C0,                //емкость нулевой последовательности
		R,                 //активное сопротивление прямой последовательности
		R0,                //активное сопротивление нулевой последовательности
		InitVoltage[3],    //начальное напряжение фаз
		BegingWave[3],     //Волна набегающая на узел начала и конца линии (для вычисления тока в линии)
		EndWave[3],
		//Волновые параметры
		WaveImpendance,   //Волновое сопротивление прямой последовательности
		WaveImpendance0,  //Волновое сопротивление нулевой последовательности
		WaveLength,       //Волновая длина линии
		WaveLength0;      //Отношение волновых длин прямой
						   //и нулевой последовательности

//     int  PhaseType;        //кол-во фаз
	bool PBayModel,        //Моделирование линии П-ячейками
		SingleBay;        //Признак разделения линии на участки

		 //Чтение данных из формы EnterForm
	void FillDataFromForm();

	//Чтение данных
	void FillData(vector<string> DataString);

	//Создание списка параметров
	void FillArgumentList();

	//Метод бегущих волн
	void InitialConditions(bool FirstFill);

	// Создание каналов прямой обратной и нулевой последовательности
	void ChannelCreate();

	// Функция вычисления составляющих для определения напряжения в узле
	void FindNodeI(int NodeIndex, double*);

	// Функция передачи волнового сопротивления узла.
	void GetBranchG(double*);

	// Запись напряжения узла в канал
	void WaveMove();

	// Вывод волны на график
	void GetBranchV(double*);

	//Передача значения узлового напряжения
	double GetVoltageClass();
};

