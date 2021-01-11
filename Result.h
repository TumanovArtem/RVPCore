#pragma once
#include <string>
#include <vector>

using namespace std;
class Result
{
public:
	string  ParametrName;  // имя параметра
	string  ParametrType;  // тип параметра
	int  ParametrCode;         // код параметра (1-U,2-I,3-V)
	int  ParametrOwner;        // номер верви или узла
	int  InternalItCount;      // Кол-во внутренних итераций
	int  Ifactor;
	float BaseParametr;       // базисный ток или напряжение
	float Max;                // максимальное значение параметра
	float Min;                // минимальное значение параметра

	vector<vector<double>> DataList;   //Данные 3-x каналов
	vector<double> aDataList;  //Данные канала альфа
	vector<double> bDataList;  //Данные канала бета
	vector<double> cDataList;  //Данные канала 0

	Result();
	Result(string);
	~Result();

	vector<double> GetParametr(int);

	//Поиск максимального и минимального значения параметра
	void FindMaxMin();

};

