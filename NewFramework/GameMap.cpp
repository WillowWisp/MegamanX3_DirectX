#include "GameMap.h"



GameMap::GameMap()
{
}

GameMap::~GameMap()
{
	delete map;
}

GameMap::GameMap(char* filePath)
{
	LoadMap(filePath);
}


void GameMap::LoadMap(char* filePath)
{
	map = new Tmx::Map();
	map->ParseFile(filePath);

	//Khởi tạo Quadtree
	RECT mapRect;
	mapRect.left = 0;
	mapRect.top = 0;
	mapRect.right = this->GetWidth();
	mapRect.bottom = this->GetHeight();
	quadtree = new Quadtree(1, mapRect);

	for (int i = 0; i < map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map->GetTileset(i);

		Sprite *sprite = new Sprite(tileset->GetImage()->GetSource().c_str());

		listTileset.insert(std::pair<int, Sprite*>(i, sprite));
	}

	//Insert các Static Object (object tĩnh để đứng lên) vào quadtree
	//Lưu ý: Các static object này KHÔNG vẽ lên, chỉ để va chạm vs người chơi
	for (int i = 0; i < map->GetNumObjectGroups(); i++)
	{
		const Tmx::ObjectGroup *objectGroup = map->GetObjectGroup(i);

		for (int j = 0; j < objectGroup->GetNumObjects(); j++)
		{
			//Lấy ObjectGroup # lấy layer
			//ObjectGroup chứa những object (object của phần mềm Tiled)
			Tmx::Object *object = objectGroup->GetObjects().at(j);

			MObject *mObject = new MObject();
			mObject->x = object->GetX() + object->GetWidth() / 2;
			mObject->y = object->GetY() + object->GetHeight() / 2;
			mObject->width = object->GetWidth();
			mObject->height = object->GetHeight();
			//mObject->Tag = Entity::EntityTypes::Static;

			quadtree->Insert(mObject);
		}
	}
}

Tmx::Map* GameMap::GetMap()
{
	return map;
}

int GameMap::GetWidth()
{
	//map->GetWidth() đơn vị là số tile
	return map->GetWidth() * map->GetTileWidth();
}

int GameMap::GetHeight()
{
	return map->GetHeight() * map->GetTileHeight();
}

int GameMap::GetTileWidth()
{
	return map->GetTileWidth();
}

int GameMap::GetTileHeight()
{
	return map->GetTileHeight();
}

Quadtree* GameMap::GetQuadtree() {
	return quadtree;
}

void GameMap::SetCamera(Camera *_camera)
{
	this->camera = _camera;
}

void GameMap::Draw()
{
	D3DXVECTOR2 translate = D3DXVECTOR2(GameGlobal::wndWidth / 2 - camera->position.x, GameGlobal::wndHeight / 2 - camera->position.y);

	for (int i = 0; i < map->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = map->GetTileLayer(i);

		if (!layer->IsVisible())
		{
			continue;
		}

		RECT sourceRECT;

		int tileWidth = map->GetTileWidth();
		int tileHeight = map->GetTileHeight();

		for (int m = 0; m < layer->GetHeight(); m++)
		{
			for (int n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = map->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = listTileset[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);

					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					D3DXVECTOR3 position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2, 0);

					//Xét xem nếu tile mà nằm trong khung camera thì mới vẽ
					if (camera != NULL) {
						RECT tileRect;
						tileRect.left = position.x - tileWidth / 2;
						tileRect.top = position.y - tileHeight / 2;
						tileRect.right = tileRect.left + tileWidth;
						tileRect.bottom = tileRect.top + tileHeight;

						//nằm ngoài thì ko vẽ
						if (!GameGlobal::IsIntersect(tileRect, camera->GetBound())) {
							continue;
						}
					}

					sprite->width = tileWidth;
					sprite->height = tileHeight;

					sprite->Draw(position, sourceRECT, D3DXVECTOR2(), translate);
				}
			}
		}
	}
}