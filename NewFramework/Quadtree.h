#pragma once

#include <vector>
#include "MObject.h"
#include "DebugDraw.h"
#include "GameLog.h"

#define MAX_LEVEL 5
#define MAX_OBJECTS_IN_REGION 4

class Quadtree
{
	int level;
	std::vector<MObject*> objectList;

	bool IsContain(MObject* object);
	void Split(); //Khởi tạo 4 nodes con cho node cha hiện tại, 4 nodes này cũng là 1 quadtree nên cũng có thể Split() tiếp khi thỏa điều kiện
public:
	Quadtree();
	Quadtree(int _level, RECT _region);
	~Quadtree();

	RECT region;
	Quadtree** nodes; //Chứa 4 nodes con
	void Clear(); //Hàm đệ quy: giải phóng vùng nhớ cho obectList, nodes
	void Insert(MObject* object);
	void Remove(MObject* object);
	void GetObjectsCollidableWith(MObject* object, std::vector<MObject*> &returnObjects); //Lấy tất cả objects có thể va chạm (nằm chung node vs *object)
	int GetTotalObjects();
	void Debug(int &count);
};

