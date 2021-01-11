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
	int BlockCount,  //����������� ������
		BranchCount, //����������� ������ (���������)
		NodeCount,   //����������� �����
		GrafStep,
		ResStep,
		CountGroupsNum;

	vector<Block*> BlockList;   //������ ������ ������
	vector<Branch*> BranchList;  //������ ������, �������� � ������
	vector<Node*> NodeList;    //������ ����� ������
	vector<Event*> EventList;   //������ �������������� �������
	vector<Result*> ResultList;  //������ �����������
	vector<int> IterationCount;   // ������ ���������� �������� �� ����
	
	Model(vector<string>);
	~Model();

	void Load_Data(vector<string>);

	//����� ���� �� ������
	int FindNodeIndex(int, int);

	//������������� ������� �����
	void FillBranchesListInNode();

	// ����������� �������� ������������� �����.
	void FindZ();

	//������ ���������� � �����.
	string FindU();

	//���������� ������ ����� ��� ������ �������.
	void FillResList();

	//���������� ������ �������������� �������.
	void SortEventsList(int);

	//������ ����������� ������� �� ���� � ������.
	string FillResult();

	//����� ���������� ������� �� ������
	void GraphResult(int);

	//���������� ����� �� ������ � ������ ����������� �������
	void AddPointsToGraph(int, int);

	//����� �������� ������� �� ������
	void GraphFirstResult();

	// ����� ������ ������ ����� �����������
	void ModelOutput();

	//�������� �����
	void CreateNode(int);

	//�������� ��������� �����
	bool CheckModel();

	void Show();
};

