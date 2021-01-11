#pragma once
#include <vector>
#include <string>

using namespace std;

class Channel	//Характеризует канал в линии, смоделированной распределенными параметрами
{
public:

	Channel();
	Channel(int ChannelKind);

	~Channel();

	double AttenFactor,	//Коэффициент затухания (Attenuation) - приломления
		ReflFactor;
	int	NumOfSteps,		//Кол-во целых шагов
		ChannelType;	//Тип канала (альфа - 1,бета - 2 ,0 - 0)

	vector<double> VForwardList;
	vector<double> VBackwardList;

	//Нахождение дискретного числа шагов умещающегося во время пробега волны
	void StepsInWaveMove(double WaveLength, double R, double L);

	//Заполнение списка прямых и обратных волн
	void FillVList(double ValueRez);

	//Вторичное заполнение списка прямых и обратных волн
	void NextFillVList(double ValueRez);

	//"Продергивание" волны
	void ChannelWaveMove(double Ubegin, double Uend);

	// Функция определения части, составляющей напряжение в узле
	double GetV(bool ChannelBeging);

	// Удаление канала
	void ChannelClear();

	// Получение волны на шаге для вывода на график
	double GetVForGraf();

};

