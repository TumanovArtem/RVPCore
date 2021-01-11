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

	//������ �������� � �����
	vector<Branch*> BlockBranchList;
	//������ ����� �����
	vector<Node*> BlockNodeList;
	//������ ������� ����� �����
	vector<string> ExtBlockNode;
	vector<string> ResBlockList;
	vector<string> ArgumentList;
	vector<string> ParamList;

	int     i,
		BlockNodeNum, //���������� ����� ������������� ����� � �����
		BlockIndex,    //������� ����� � BlockList
		BlockType,    //��� ����� (0- ����������������)
		ResParCount,  //������� ��������� ���������� ��������� �����
		BlockCountGroup,   //��������� ������
		BlockElemNum; //���������� ��������� � �����

	bool Enabled;

	string BlockName;  //������������ ����� ���������

	//������ ������ �� ������ �����
	void FillData(vector<string>);

	//�������� ������ ����������
	void FillArgumentList();

	//������ ������ � �������
	void WriteToTable();

	//������ ������ �� �����
	void FillDataFromForm();

	//���������� ���������� ����� ��� ������ � ����
	void WriteToFile();
/*
	//���������� ����� ���������� �����
	virtual void FillPropertiesTable();
*/
};

