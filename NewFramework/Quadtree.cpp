#include "Quadtree.h"



Quadtree::Quadtree()
{
}


Quadtree::~Quadtree()
{
}

Quadtree::Quadtree(int _level, RECT _region) {
	level = _level;
	region = _region;
}


void Quadtree::Clear()
{
	//Clear all nodes
	if (nodes)
	{
		for (int i = 0; i < 4; i++)
		{
			nodes[i]->Clear();
			delete nodes[i];
		}
		delete[] nodes;
	}

	//Clear current quadtree
	objectList.clear();
}

bool Quadtree::IsContain(MObject* object)
{
	RECT bound = object->GetRect();

	//return GameGlobal::IsContain(region, bound);
	return GameGlobal::IsIntersect(region, bound);
}

void Quadtree::Split() {
	//cat phan region (ranh gioi) ra thanh 4 phan bang nhau
	nodes = new Quadtree *[4];

	RECT bound;

	int nodeWidth = (region.right - region.left) / 2;
	int nodeHeight = (region.bottom - region.top) / 2;

	//phan goc trai tren
	bound.left = region.left;
	bound.right = bound.left + nodeWidth;
	bound.top = region.top;
	bound.bottom = bound.top + nodeHeight;
	nodes[0] = new Quadtree(level + 1, bound);

	//phan goc phai tren
	bound.left = region.left + nodeWidth;
	bound.right = bound.left + nodeWidth;
	bound.top = region.top;
	bound.bottom = bound.top + nodeHeight;
	nodes[1] = new Quadtree(level + 1, bound);

	//phan goc trai duoi
	bound.left = region.left;
	bound.right = bound.left + nodeWidth;
	bound.top = region.top + nodeHeight;
	bound.bottom = bound.top + nodeHeight;
	nodes[2] = new Quadtree(level + 1, bound);

	//phan goc phai duoi
	bound.left = region.left + nodeWidth;
	bound.right = bound.left + nodeWidth;
	bound.top = region.top + nodeHeight;
	bound.bottom = bound.top + nodeHeight;
	nodes[3] = new Quadtree(level + 1, bound);
}

void Quadtree::Insert(MObject *object) {
	//Nếu 4 node con tồn tại thì insert (đệ quy) vào node chứa obj
	if (nodes)
	{
		if (nodes[0]->IsContain(object))
			nodes[0]->Insert(object);
		if (nodes[1]->IsContain(object))
			nodes[1]->Insert(object);
		if (nodes[2]->IsContain(object))
			nodes[2]->Insert(object);
		if (nodes[3]->IsContain(object))
			nodes[3]->Insert(object);

		return; // Return here to ignore rest.
	}

	//Nếu quadtree là node lá (ko có 4 node con) thì thêm obj vô quadtree này
	if (this->IsContain(object))
		objectList.push_back(object);

	//Nếu số lượng object trong quadtree hiện tại vượt quá mức cho phép && vẫn còn có thể chia được (chưa đạt MAX_LEVEL)
	//Thì ta chia nhỏ quadtree thành 4 node con, rồi xóa hết objectList trong quadtree và gắn lại vào 4 node con
	if (objectList.size() > MAX_OBJECTS_IN_REGION && level < MAX_LEVEL)
	{
		Split();

		while (!objectList.empty())
		{
			if (nodes[0]->IsContain(objectList.back()))
				nodes[0]->Insert(objectList.back());
			if (nodes[1]->IsContain(objectList.back()))
				nodes[1]->Insert(objectList.back());
			if (nodes[2]->IsContain(objectList.back()))
				nodes[2]->Insert(objectList.back());
			if (nodes[3]->IsContain(objectList.back()))
				nodes[3]->Insert(objectList.back());

			objectList.pop_back();
		}
	}
}

void Quadtree::GetObjectCollidableWith(MObject* object, std::vector<MObject*> &returnObjects) {
	if (nodes)
	{
		if (nodes[0]->IsContain(object))
			nodes[0]->GetObjectCollidableWith(object, returnObjects);
		if (nodes[1]->IsContain(object))
			nodes[1]->GetObjectCollidableWith(object, returnObjects);
		if (nodes[2]->IsContain(object))
			nodes[2]->GetObjectCollidableWith(object, returnObjects);
		if (nodes[3]->IsContain(object))
			nodes[3]->GetObjectCollidableWith(object, returnObjects);

		return; // Return here to ignore rest.
	}

	//Add all objects in current region into returnObjects
	if (this->IsContain(object))
	{
		for (auto i = returnObjects.begin(); i != returnObjects.end(); i++)
		{
			if (object != *i)
				returnObjects.push_back(*i);
		}
	}
}