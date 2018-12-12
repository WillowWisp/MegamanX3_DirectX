#include "Collision.h"



Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::AABBCheck(MObject *object, MObject *otherObject)
{
	float left = otherObject->x - (object->x + object->width);
	float top = (otherObject->y + otherObject->height) - object->y;
	float right = (otherObject->x + otherObject->width) - object->x;
	float bottom = otherObject->y - (object->y + object->height);

	// xét ngược lại
	return !(left > 0 || right < 0 || top < 0 || bottom > 0);

	/*return !(b1->x + b1->width < b2->x || b1->x > b2->x + b2->width || b1->y + b1->height < b2->y || b1->y > b2->y + b2->height);*/
}


MObject* Collision::GetSweptBroadphaseBox(MObject *object) {
	MObject *broadphaseBox = new MObject();
	broadphaseBox->x = object->movex > 0 ? object->x : object->x + object->movex;
	broadphaseBox->y = object->movey > 0 ? object->y : object->y + object->movey;
	broadphaseBox->width = object->width + abs(object->movex);
	broadphaseBox->height = object->height + abs(object->movey);

	return broadphaseBox;
}

bool Collision::SweptAABB(MObject *object, MObject *otherObject, float &normalx, float &normaly) {
	float dxEntry, dxExit;
	float dyEntry, dyExit; //khoảng cách: -entry: 2 cạnh gần ; -exit: 2 cạnh xa

	//tìm khoảng cách giữa 2 cạnh gần và cạnh xa
	if (object->movex > 0.0f) { //object nằm bên trái otherObject
		dxEntry = otherObject->x - (object->x + object->width);
		dxExit = (otherObject->x + otherObject->width) - object->x;
	}
	else { //object nằm bên phải otherObject
		dxEntry = (otherObject->x + otherObject->width) - object->x;
		dxExit = otherObject->x - (object->x + object->width);
	}

	if (object->movey >= 0.0f) { //object nằm bên trên otherObject
		dyEntry = otherObject->y - (object->y + object->height);
		dyExit = (otherObject->y + otherObject->height) - object->y;
	}
	else { //object nằm bên dưới otherObject
		dyEntry = (otherObject->y + otherObject->height) - object->y;
		dyExit = otherObject->y - (object->y + object->height);
	}

	//tính thời gian từ khoảng cách trên và vận tốc (vận tốc trong 1 khung hình) của vật
	float txEntry, txExit;
	float tyEntry, tyExit;
	if (object->movex == 0.0f) {
		//không di chuyển theo trục y nên thời gian = vô cực
		txEntry = -std::numeric_limits<float>::infinity();
		txExit = std::numeric_limits<float>::infinity();
	}
	else {
		txEntry = dxEntry / object->movex;
		txExit = dxExit / object->movex;
	}

	if (object->movey == 0.0f) {
		tyEntry = -std::numeric_limits<float>::infinity();
		tyExit = std::numeric_limits<float>::infinity();
	}
	else {
		tyEntry = dyEntry / object->movey;
		tyExit = dyExit / object->movey;
	}

	// thời gian va chạm là thời gian lớn nhất trong 2 (2 trục phải cùng tiếp xúc thì mới va chạm)
	float entryTime = max(txEntry, tyEntry);
	// thời gian hết va chạm là thời gian bé nhất trong 2, (1 cái ra khỏi là object hết va chạm)
	float exitTime = min(txExit, tyExit);

	//nếu không va chạm
	if (entryTime > exitTime || (txEntry < 0.0f && tyEntry < 0.0f) || txEntry > 1.0f || tyEntry > 1.0f) { // ????
		//nếu thời gian va chạm không nằm từ 0 tới 1 thì sẽ ko va chạm trong frame hiện tại
		//hơn nữa, thời gian bắt đầu va chạm sẽ ko bao giờ xảy ra sau khi vật kết thúc va chạm (entryTime > exitTime
		normalx = 0.0f;
		normaly = 0.0f;
		//return false = vật không va chạm
		return false;
	}

	//nếu có va chạm
	//tính vector pháp tuyến của bề mặt va chạm (hướng va chạm)
	if (txEntry > tyEntry) //đụng ở cạnh trái & phải của otherObject ???
	{
		if (dxEntry < 0.0f || (dxEntry == 0 && object->movex < 0))
		{
			//Vector PHẢI
			normalx = 1.0f;
			normaly = 0.0f;
		}
		else
		{
			//Vector TRÁI
			normalx = -1.0f;
			normaly = 0.0f;
		}
	}
	else //đụng ở cạnh trên & dưới của otherObject ???
	{
		if (dyEntry < 0.0f)
		{
			//Vector DƯỚI
			normalx = 0.0f;
			normaly = 1.0f;
		}
		else
		{
			//Vector TRÊN
			normalx = 0.0f;
			normaly = -1.0f;
		}
	}

	//return thời gian va chạm (giá trị giữa 0 và 1)
	return true;
}

char* Collision::IsCollided(MObject *object, MObject *otherObj) {
	char* result = (char*)"none";

	MObject *broadphaseBox = GetSweptBroadphaseBox(object);
	if (AABBCheck(broadphaseBox, otherObj)) {
		float normalxOtherObj, normalyOtherObj; //vector pháp tuyến trên bề mặt paddle (hướng va chạm)
		bool isCollided = SweptAABB(object, otherObj, normalxOtherObj, normalyOtherObj);
		
		if (isCollided) {
			if (normalxOtherObj == 1.0f && normalyOtherObj == 0.0f)
				result = (char*)"right";
			if (normalxOtherObj == -1.0f && normalyOtherObj == 0.0f)
				result = (char*)"left";
			if (normalxOtherObj == 0.0f && normalyOtherObj == 1.0f)
				result = (char*)"bottom";
			if (normalxOtherObj == 0.0f && normalyOtherObj == -1.0f)
				result = (char*)"top";
		}
		else {
			//return IsIntersect(object, otherObj);
		}
	}

	return result;
}

char* Collision::IsIntersect(MObject *obj1, MObject *obj2) {
	RECT obj1Rect = obj1->GetRect();
	RECT obj2Rect = obj2->GetRect();

	//Không va chạm
	if (!GameGlobal::IsIntersect(obj1Rect, obj2Rect)) {
		return (char*)"none";
	}

	//Có va chạm thì tìm vector pháp tuyến trên bề mặt <<obj2>>
	//Đúng ra chỉ cần obj1Rect.bottom == obj2Rect.top là trả về "top" dc rồi nhưng do tọa độ làm tròn số nên
	if (obj1Rect.bottom >= obj2Rect.top && obj2Rect.top >= obj1Rect.bottom - 2) {
		return (char*)"top";
	}
	if (obj2Rect.bottom >= obj1Rect.top && obj1Rect.top >= obj2Rect.bottom - 2) {
		return (char*)"bottom";
	}
	if (obj1Rect.right >= obj2Rect.left && obj2Rect.left >= obj1Rect.right - 5) {
		return (char*)"left";
	}
	if (obj2Rect.right >= obj1Rect.left && obj1Rect.left >= obj2Rect.right - 5) {
		return (char*)"right";
	}

	return (char*)"top";
}