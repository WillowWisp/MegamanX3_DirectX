#pragma once

#include <vector>
#include "MObject.h"

#define MAX_LEVEL 5
#define MAX_OBJECTS_IN_REGION 4

class Quadtree
{
	int level;
	RECT region;
	std::vector<MObject*> objectList;
	Quadtree** nodes; //Chứa 4 nodes con

	bool IsContain(MObject* object);
	void Split(); //Khởi tạo 4 nodes con cho node cha hiện tại, 4 nodes này cũng là 1 quadtree nên cũng có thể Split() tiếp khi thỏa điều kiện
public:
	Quadtree();
	Quadtree(int _level, RECT _region);
	~Quadtree();

	void Clear(); //Hàm đệ quy: giải phóng vùng nhớ cho obectList, nodes
	void Insert(MObject* object);
	void GetObjectCollidableWith(MObject* object, std::vector<MObject*> &returnObjects); //Lấy tất cả objects có thể va chạm (nằm chung node vs *object)
};

