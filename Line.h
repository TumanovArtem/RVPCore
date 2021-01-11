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

	double VoltageClass,      // ласс напр€жени€
		Length,            //ƒлина линии
		L,                 //индуктивность пр€мой последовательности
		L0,                //индуктивность нулевой последовательности
		C,                 //емкость пр€мой последовательности
		C0,                //емкость нулевой последовательности
		R,                 //активное сопротивление пр€мой последовательности
		R0,                //активное сопротивление нулевой последовательности
		InitVoltage[3],    //начальное напр€жение фаз
		BegingWave[3],     //¬олна набегающа€ на узел начала и конца линии (дл€ вычислени€ тока в линии)
		EndWave[3],
		//¬олновые параметры
		WaveImpendance,   //¬олновое сопротивление пр€мой последовательности
		WaveImpendance0,  //¬олновое сопротивление нулевой последовательности
		WaveLength,       //¬олнова€ длина линии
		WaveLength0;      //ќтношение волновых длин пр€мой
						   //и нулевой последовательности

//     int  PhaseType;        //кол-во фаз
	bool PBayModel,        //ћоделирование линии ѕ-€чейками
		SingleBay;        //ѕризнак разделени€ линии на участки

		 //„тение данных из формы EnterForm
	void FillDataFromForm();

	//„тение данных
	void FillData(vector<string> DataString);

	//—оздание списка параметров
	void FillArgumentList();

	//ћетод бегущих волн
	void InitialConditions(bool FirstFill);

	// —оздание каналов пр€мой обратной и нулевой последовательности
	void ChannelCreate();

	// ‘ункци€ вычислени€ составл€ющих дл€ определени€ напр€жени€ в узле
	void FindNodeI(int NodeIndex, double*);

	// ‘ункци€ передачи волнового сопротивлени€ узла.
	void GetBranchG(double*);

	// «апись напр€жени€ узла в канал
	void WaveMove();

	// ¬ывод волны на график
	void GetBranchV(double*);

	//ѕередача значени€ узлового напр€жени€
	double GetVoltageClass();
};

