#include "Channel.h"
#include <vector>
#include <string>

using namespace std;

extern double Omega;
extern double PI;
extern double Step; //Шаг расчета в радианах
extern int CurrentStep; //Номер текущего шага

Channel::Channel()
{

}

Channel::Channel(int ChannelKind)
{
	ChannelType = ChannelKind;
	VForwardList.reserve(100);
	VBackwardList.reserve(100);
}

Channel::~Channel()
{

}

//Нахождение дискретного числа шагов умещающегося во время пробега волны
void Channel::StepsInWaveMove(double WaveLength, double R, double L)
{
	double Delta;

	double StepsCol;

	StepsCol = WaveLength * PI / (180 * Step) + 0.5;
	NumOfSteps = StepsCol;

	//TO DO
	// Предупреждение если меньше 1 Уменьшите шаг.
	//if (NumOfSteps < 1)
		/*Warning("Для заданной длины линии\n необходимо уменьшить шаг расчета "
			+ IntToStr(CurrentStep), 3);*/

	Delta = (R * Step / (2 * Omega * L * 0.001));

	AttenFactor = (1 / (1 + Delta));  // Коэффициент затухания (приломления)
	ReflFactor = Delta / (1 + Delta);  //Коэффициент отражения
}

//Первоначальное заполнение списка прямых и обратных волн
void Channel::FillVList(double ValueRez)
{
	int i;
	double ValueRef;

	try  // проверка исключения bad_alloc
	{
		for (i = 0; i <= NumOfSteps; i++)
		{
			double a = 0;
			VForwardList.push_back(a);
			VBackwardList.push_back(a);
		}
	}
	catch (std::bad_alloc)
	{
		/*cout << "Could not allocate. Bye ...";
		Warning("Ошибка создания канала линии с " + IntToStr(NumOfSteps) + "ячейками", 1);*/
		exit(-1);
	}
}

//Вторичное заполнение списка прямых и обратных волн
void Channel::NextFillVList(double ValueRez)
{
	int i;
	for (i = 0; i <= NumOfSteps; i++)
	{
		VForwardList[i] = ValueRez;
		VBackwardList[i] = ValueRez;
	}
}

//"Продергивание" волны
void Channel::ChannelWaveMove(double Ubegin, double Uend)
{
	int i;
	double VForwardOldValue, VBackwardOldValue,
		VForwardThis, VBackwardThis;

	VForwardOldValue = VForwardList[0];
	VBackwardOldValue = VBackwardList[0];

	VForwardList[0] = Ubegin - VBackwardList[VBackwardList.size() - 1];
	VBackwardList[0] = Uend - VForwardList[VForwardList.size() - 1];


	for (i = 1; i <= NumOfSteps; i++)
	{
		VForwardThis = VForwardList[i];

		VForwardList[i] = VForwardOldValue * AttenFactor + VBackwardList[VBackwardList.size() - 1 - i] * ReflFactor;
		VForwardOldValue = VForwardThis;

		VBackwardThis = VBackwardList[i];
		VBackwardList[i] = VBackwardOldValue * AttenFactor + VForwardList[VForwardList.size() - 1 - i] * ReflFactor;

		VBackwardOldValue = VBackwardThis;
	}
}
//---------------------------------------------------------------------------
// Функция определения составляющей тока в узле
double Channel::GetV(bool ChannelBeging)
{
	double V;

	if (ChannelBeging)
		V = VBackwardList[VBackwardList.size() - 1];
	else
		V = VForwardList[VForwardList.size() - 1];

	return(V);
}
//---------------------------------------------------------------------------
// Удаление канала
void Channel::ChannelClear()
{
	VForwardList.clear();
	VBackwardList.clear();
}
//---------------------------------------------------------------------------
// Получение волны на шаге для вывода на график
double Channel::GetVForGraf()
{
	double V;

	if (CurrentStep <= NumOfSteps)
		V = VForwardList[CurrentStep];

	if ((CurrentStep > NumOfSteps) && (CurrentStep <= 2 * NumOfSteps))
		V = VBackwardList[CurrentStep - NumOfSteps];

	return(V);
}
//---------------------------------------------------------------------------


