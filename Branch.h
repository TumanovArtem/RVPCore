#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

class Branch
{
public:

	Branch();
	~Branch();
	
	int i,					//Индекс
		PhaseOn,			//Кол-во включенных фаз
		BegingNode,			//Номер начального узла ветви
		EndNode,			//Номер конечного узла ветви
		BegingNodeIndex,	//Индекс начального узла в масиве узлов
		EndNodeIndex,		//Индекс конечного узла в масиве узлов
		InternalItCount,	//Кол-во итераций внутри шага (ОПН, Шунт)

		BranchIndex,       //Номер ветви
		ElementType,       //Тип элемента ветви

		CurrentPhaseState, //текущее состояние фаз (0-7 0-AllOff 1-A 2-B 3-AB 4-C 5-AC 6-BC 7-ABC)
		SwitchTime[3],     //время коммутации фаз
		//Параметры контроля напряжения выключателя
		U0,             //Начальное напряжение
		Umax,           //Максимальное напряжение
		dU,             //Скорость изменения напряжения
		//         ShuntingTime,   //момент шунтирования R
		ReclosingTime,		//tапв оключаемых фаз, время повторной попытки влючения при контроле dU
		CutOffCurrent,		//допустимая вел-на тока, обрываемого выключателем
		PhaseType,
		BlockNum;        //номер блока


	double  UnomRato,		//отношение номинальных напряжений узлов (для неузловых элементов)
			Ibase,			//базисный ток
			I[3],			//токи ветви в конце шага
			Iiter[3],		//токи ветви в конце шага на предыдущей итерации
			Ipast[3],		//токи ветви в начале шага
			Ub[3],			//падение напряжения на ветви
			Urab[3],
			Zrab[3],		// Собственное волновое сопротивление ветви
			Grab[3];		// Собственная волновая проводимость ветви

	bool    Enabled,        //Признак участия ветви в расчете
			Virtual,
			ChangePaseState;//Признак изменения состояния ветви

	string BranchName; //Имя ветви

	map<string, string> ArgumentList;

	virtual void FindNodeI(int, double *);

	// Получение класса напряжения
	virtual double GetVoltageClass();

	// Функция передачи волнового сопротивления ветви.
	virtual void GetBranchG(double*);

	//Заполнение событий
	void FillEvent();

	// Запись напряжения узла в канал и "продергивание" волны в канале.
	virtual void WaveMove();

	//Запись базисного значения тока ветви
	virtual void FindBaseValue();

	//Заполнение каналов начальными значениями волн
	virtual void InitialConditions(bool);
};

