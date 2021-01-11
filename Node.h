#pragma once
#include <string>
#include <vector>

#include "Phase.h"

using namespace std;
class Node
{
public:

	int i, j,
		NodeNum,	//Номер узла (наименование)
		NodeIndex,	//Позиция узла в Node List
		PhaseNum;	//Кол-во фаз узла

	double	Ubase,	//Базисное напряжение узла (Rбаз = 1 Ом)
			Zwavef,	//Фазное волновое сопротивление узла
			Zwavem;	//Междуфазное волновое сопротивление узла

	bool	NodeNextCount,
			External;

	string NodeName;	//Наименование узла

	vector<int> BranchesInNode;	//Список ветвей, входящих в узел
	vector<Phase*> PhaseList;

	void FindNodeU();    //Определение напряжения в узле

	void FindNodeZ();    //Определение волн. сопротивления узла.

	void GetUxx(double*); //Вывод удвоенного значения волны

	void GetUxxPast(double*); //Вывод удвоенного значения волны в начале шага

	vector<double> GetU(); //Вывод напряжения узла

	void GetIsum(double*); //Вывод суммы токов

	void GetIsumPast(double*); //Вывод суммы токов в начале шага

	void GetZwave(double*); //Вывод экв. волновых сопротивлений узла

	bool UCorrection(); //Коррекция напряжения в узле

	void IsumCorrection(double*); //Коррекция cуммарного тока в узле

	void IsumAddition(double*); //Коррекция cуммарного тока в узле

	void Uxx0Calculation(vector<double>); //Учет нач. условий узловых элементов

	void InitialConditions(); // Заполнение начальных условий

	Node();
	Node(int, double, int, int, int);
	~Node();

	void Show();

};

