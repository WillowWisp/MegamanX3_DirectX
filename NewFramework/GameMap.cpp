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

	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = this->GetWidth();
	r.bottom = this->GetHeight();

	for (int i = 0; i < map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map->GetTileset(i);

		Sprite *sprite = new Sprite(tileset->GetImage()->GetSource().c_str());

		listTileset.insert(std::pair<int, Sprite*>(i, sprite));
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

void GameMap::Draw()
{
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

					sprite->width = tileWidth;
					sprite->height = tileHeight;

					sprite->Draw(position, sourceRECT);
				}
			}
		}
	}
}