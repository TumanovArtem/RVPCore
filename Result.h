#pragma once
#include <string>
#include <vector>

using namespace std;
class Result
{
public:
	string  ParametrName;  // им€ параметра
	string  ParametrType;  // тип параметра
	int  ParametrCode;         // код параметра (1-U,2-I,3-V)
	int  ParametrOwner;        // номер верви или узла
	int  InternalItCount;      //  ол-во внутренних итераций
	int  Ifactor;
	float BaseParametr;       // базисный ток или напр€жение
	float Max;                // максимальное значение параметра
	float Min;                // минимальное значение параметра

	vector<vector<double>> DataList;   //ƒанные 3-x каналов
	vector<double> aDataList;  //ƒанные канала альфа
	vector<double> bDataList;  //ƒанные канала бета
	vector<double> cDataList;  //ƒанные канала 0

	Result();
	Result(string);
	~Result();

	vector<double> GetParametr(int);

	//ѕоиск максимального и минимального значени€ параметра
	void FindMaxMin();

};

