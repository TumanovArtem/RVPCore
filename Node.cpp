#include "Node.h"
#include "Phase.h"
#include "Model.h"
#include "Branch.h"
#include <iostream>
#include <cmath>
using namespace std;
extern double Epsilon;
extern Model* MyModel;

//Конструктор 1
Node::Node()
{
	NodeNum = 0;
}

//Конструктор 2
Node::Node(int Number, double VoltageClass, int BranchNum, int PhaseCount, int Index)
{
	NodeNum = Number;
	NodeIndex = Index;
	External = false;

	PhaseNum = PhaseCount;

	BranchesInNode.reserve(20);		//Резервируем 20 ячеек

	Ubase = (VoltageClass * sqrt((double)2/(double)3));

	if (NodeNum != 0)			//Для исключения нулевого узла из расчета
		NodeNextCount = true;
	else
		NodeNextCount = false;

	Zwavef = Zwavem = 0;

	Phase* p;
	PhaseList.reserve(3);			//Резервируем 3 ячейки
	for (i = 0; i < PhaseNum; i++)	//Создание пустых экземпляторов объектов фаз
	{		
		PhaseList.push_back(p = new Phase());	
	}
}

//Деструктор
Node::~Node()
{
	PhaseList.clear();
	BranchesInNode.clear();
}

void Node::InitialConditions()
{
	for (i = 0; i < PhaseNum; i++) 
	{
		PhaseList[i]->Isum = 0;
		PhaseList[i]->Uxx = 0;
		PhaseList[i]->U = 0;
	}
}

void Node::FindNodeU()
{
	int BranchIndex;
	double J[3], Jsum[3];

	for (i = 0; i < PhaseNum; i++)
	{
		Jsum[i] = 0;
		PhaseList[i]->IsumPast = PhaseList[i]->Isum;	//Суммарный ток узла в начале шага
		PhaseList[i]->Upast = PhaseList[i]->U;
		PhaseList[i]->UxxPast = PhaseList[i]->Uxx;	//Uxx включает начальные условия
	}

	for (j = 0; j < BranchesInNode.size(); j++)
	{
		BranchIndex = BranchesInNode[j];
		if ((MyModel->BranchList[BranchIndex]->ElementType <= 3) && !(MyModel->BranchList[BranchIndex]->Virtual))
		{
			if (MyModel->BranchList[BranchIndex]->Enabled) 
			{
				MyModel->BranchList[BranchIndex]->FindNodeI(NodeIndex, J);
			}
			for (i = 0; i < PhaseNum; i++) 
			{
				Jsum[i] += J[i];
			}
		}
	}
	for (i = 0; i < PhaseNum; i++)
	{
		PhaseList[i]->Uxx = Jsum[i] * PhaseList[i]->Zwave;
	}

}

void Node::FindNodeZ()
{
	int j, i, BranchIndex;
	double G[3], Ywave[3];

	for (i = 0; i < 3; i++)
	{
		G[i] = 0; Ywave[i] = 0;
	}

	for (j = 0; j < BranchesInNode.size(); j++)   // цикл по кол-ву ветвей, подсоединенных к узлу.
	{
		BranchIndex = BranchesInNode[j];
		if ((MyModel->BranchList[BranchIndex]->ElementType <= 3) && !(MyModel->BranchList[BranchIndex]->Virtual))
		{
			MyModel->BranchList[BranchIndex]->GetBranchG(G); //TO DO

			for (i = 0; i < PhaseNum; i++)
				Ywave[i] += G[i];		//Вычисление суммарной волновой проводимости узла
		}
	}

	for (i = 0; i < PhaseNum; i++)
		PhaseList[i]->Zwave = 1 / Ywave[i];	//Суммарное волновое сопротивление узла

	if (PhaseNum != 1)
	{
		Zwavef = (2 * PhaseList[1]->Zwave + PhaseList[0]->Zwave) / 3; //Фазное волновое сопротивление узла
		Zwavem = (PhaseList[0]->Zwave - PhaseList[1]->Zwave) / 3; //Междуфазное волновое сопротивление узла
	}
	else
	{
		Zwavef = 1;
		Zwavem = 500;
	}

}

void Node::GetUxx(double*)
{
}

void Node::GetUxxPast(double*)
{
}

vector<double> Node::GetU()
{
	vector<double> U;
	for (i = 0; i < PhaseNum; i++)
		U.push_back(PhaseList[i]->U);
	return U;
}

void Node::GetIsum(double*)
{
}

void Node::GetIsumPast(double*)
{
}

void Node::GetZwave(double*)
{
}

bool Node::UCorrection()
{
	double delta = 0;
	double difference;
	bool NextIter;

	for (i = 0; i < PhaseNum; i++)
	{
		PhaseList[i]->Uiter = PhaseList[i]->U;
		PhaseList[i]->U = PhaseList[i]->Uxx - PhaseList[i]->Isum * PhaseList[i]->Zwave;
		difference = fabs(PhaseList[i]->U - PhaseList[i]->Uiter);
		if (delta < difference)  delta = difference;
	}

	if (delta > Epsilon)
		NextIter = true;
	else
		NextIter = false;

	return NextIter;
}

void Node::IsumCorrection(double*)
{
}

void Node::IsumAddition(double*)
{
}

void Node::Uxx0Calculation(vector<double> J0)
{
	for (i = 0; i < PhaseNum; i++)
	{
		PhaseList[i]->Uxx += J0[i] * PhaseList[i]->Zwave;
		PhaseList[i]->U += J0[i] * PhaseList[i]->Zwave;
	}
}