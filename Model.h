#pragma once
#include "Node.h"
#include "Branch.h"
#include "Event.h"
#include "Result.h"
#include "Block.h"
#include "EDS.h"
#include "Line.h"
#include "Calculator.h"
#include "Result.h"
class Model
{
public:
	int BlockCount,  //Колличество блоков
		BranchCount, //Колличество ветвей (элементов)
		NodeCount,   //Колличество узлов
		GrafStep,
		ResStep,
		CountGroupsNum;

	vector<Block*> BlockList;   //Список блоков модели
	vector<Branch*> BranchList;  //Список ветвей, входящих в модель
	vector<Node*> NodeList;    //Список узлов модели
	vector<Event*> EventList;   //Список коммутационных событий
	vector<Result*> ResultList;  //Список результатов
	vector<int> IterationCount;   // Список количества итераций на шаге
	
	Model(vector<string>);
	~Model();

	void Load_Data(vector<string>);

	//Поиск узла по номеру
	int FindNodeIndex(int, int);

	//Корректировка нумеров узлов
	void FillBranchesListInNode();

	// Определение волновых сопротивлений узлов.
	void FindZ();

	//Расчет напряжений в узлах.
	string FindU();

	//Заполнение списка узлов для вывода графика.
	void FillResList();

	//Сортировка списка коммуиационных событий.
	void SortEventsList(int);

	//Запись результатов расчета на шаге в масивы.
	string FillResult();

	//Вывод результата расчета на график
	void GraphResult(int);

	//Добавление точек на график в случае продолжения расчета
	void AddPointsToGraph(int, int);

	//Вывод текущего расчета на график
	void GraphFirstResult();

	// Вывод данных модели перед сохрвнением
	void ModelOutput();

	//Создание узлов
	void CreateNode(int);

	//Проверка связности схемы
	bool CheckModel();

	void Show();
};

