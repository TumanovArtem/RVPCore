#pragma once
#include <vector>
#include <string>

using namespace std;

class Channel	//������������� ����� � �����, ��������������� ��������������� �����������
{
public:

	Channel();
	Channel(int ChannelKind);

	~Channel();

	double AttenFactor,	//����������� ��������� (Attenuation) - �����������
		ReflFactor;
	int	NumOfSteps,		//���-�� ����� �����
		ChannelType;	//��� ������ (����� - 1,���� - 2 ,0 - 0)

	vector<double> VForwardList;
	vector<double> VBackwardList;

	//���������� ����������� ����� ����� ������������ �� ����� ������� �����
	void StepsInWaveMove(double WaveLength, double R, double L);

	//���������� ������ ������ � �������� ����
	void FillVList(double ValueRez);

	//��������� ���������� ������ ������ � �������� ����
	void NextFillVList(double ValueRez);

	//"�������������" �����
	void ChannelWaveMove(double Ubegin, double Uend);

	// ������� ����������� �����, ������������ ���������� � ����
	double GetV(bool ChannelBeging);

	// �������� ������
	void ChannelClear();

	// ��������� ����� �� ���� ��� ������ �� ������
	double GetVForGraf();

};

