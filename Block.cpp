#include "Block.h"
#include <iostream>

using namespace std;

Block::Block()
{

}

Block::Block(vector<string> DataString, int BlockCount)
{
	BlockBranchList.reserve(100);

	BlockNodeList.reserve(100);

	ExtBlockNode.push_back("1");
	ArgumentList.push_back("");
	ParamList.push_back("");
	ResBlockList.push_back("");

	BlockIndex = BlockCount;

	BlockCountGroup = 0;
	ResParCount = 0;
	Enabled = true;
	BlockName = "Блок 1";
	BlockType = 0;     // 1-линия; 2-трансформатор 2-х обм.; 3-трансформатор 3-х обм.
	BlockNodeNum = 0;

	//Сюда добавляем считывание данных
	//BlockNodeNum = 1;
	//BlockIndex = 0;
	//BlockType = 0;
	//BlockCountGroup = 0;
	FillData(DataString);			//TO DO разобраться что там вообще делаться должно
	FillArgumentList();
}

Block::~Block()
{
	ArgumentList.clear();
	BlockBranchList.clear();
	BlockNodeList.clear();
	ExtBlockNode.clear();
}

void Block::FillData(vector<string> line)
{
	           if (line[2] == "0") Enabled = false;
			   BlockType = stoi(line[3]);
			   BlockName = line[4];
			   BlockNodeNum = stoi(line[5]);
			   /*PosExtElement = Element;                              break;
			   //  case 6: ParametrCount = data.ToInt();                        break;
           default:
			   if (Element <= (BlockNodeNum + PosExtElement))
				   ExtBlockNode->Add(data);
			   else
			   {*/
				   //ResBlockList.push_back("");
			   /*}*/
}

void Block::FillArgumentList()
{

}