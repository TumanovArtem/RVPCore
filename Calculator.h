#pragma once
#include "Model.h"
#include <iostream>

using namespace std;

class Calculator
{
public:
	Calculator();
	~Calculator();

	//TO DO Глобальные переменные. Разобраться какие нужны 

	unsigned int i, j;          // unsigned int	32 bits	0  to  4,294,967,295
						   // int            32 bits	-2,147,483,648  to  2,147,483,647
	int k, m, ActivRow;        // Шаги циклов

	bool FirstCount;         // Признак первоначального расчета


	int CountTime,          // Общее время расчета
		SteadyStateTime,    // Время установления режима
		CountTimeNum,
		OldStepNum,         // Кол-во шагов при исходном расчете
		SwitchEvent,        // Количество осуществленных переключений
		NewFloatCount;      // Счетчик кол-ва new float

	string EntrancePath, // Переменная для записи в реестр
		ResultPath,   // Переменная для записи в реестр
		DatFileName,  // Путь в файлу данных
		ResFileName,  // Путь в файлу результатов
		Message,      // Сообщения программы
		OpenFilePath, // Имя открытого файла
		OpenFileName, // Расположение открытого файла
		ErrText,
		LastOpendFile[4]; // Имя и путь последнего открытого файла

	void Calculate();

};

