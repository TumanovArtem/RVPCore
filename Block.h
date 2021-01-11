#pragma once
#include <string>
#include <vector>
#include <map>

#include "Branch.h"
#include "Node.h"
#include "Result.h"
class Block
{
public:
	Block();
	Block(vector<string> DataString, int BlockCount);
	~Block();

	//Список элемента в блоке
	vector<Branch*> BlockBranchList;
	//Список узлов блока
	vector<Node*> BlockNodeList;
	//Список внешних узлов блока
	vector<string> ExtBlockNode;
	vector<string> ResBlockList;
	vector<string> ArgumentList;
	vector<string> ParamList;

	int     i,
		BlockNodeNum, //Количество узлов присоединения блока к схеме
		BlockIndex,    //Позиция блока в BlockList
		BlockType,    //Тип блока (0- пользовательский)
		ResParCount,  //Счетчик выводимых параметров элементов блока
		BlockCountGroup,   //Расчетная группа
		BlockElemNum; //Количество элементов в блоке

	bool Enabled;

	string BlockName;  //Наименование блока элементов

	//Чтение данных из строки файла
	void FillData(vector<string>);

	//Создание списка параметров
	void FillArgumentList();

	//Запись данных в таблицу
	void WriteToTable();

	//Чтение данных из формы
	void FillDataFromForm();

	//Подготовка параметров ветви для записи в файл
	void WriteToFile();
	/*
		//Заполнение формы параметров блока
		virtual void FillPropertiesTable();
	*/
};