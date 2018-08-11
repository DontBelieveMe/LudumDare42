#pragma once

#include <Platform/Window.h>
#include <Math/Vector2.h>
#include <Core/Array.h>

#define TILE_SIZE 16.f;

namespace ld42 {
	class Spritesheet;
	class Player;
	class TileMap;
	class Camera;
	class Tile;

	namespace global {
		extern Spritesheet *TilesSheet;
		extern gene::platform::Window *Window;
		extern float WindowWidth, WindowHeight;
		extern gene::input::Key LastKeyState[62256];
		extern Player *ThePlayer;
		extern TileMap* ActiveLevel;
		extern Camera *MainCamera;

		bool KeyPressed(gene::input::Keys key);
		bool KeyDown(gene::input::Keys key);
		bool AABBIntersection(const gene::graphics::AABBRectangle& a, const gene::graphics::AABBRectangle& b);

#define STONE_FLOOR_TOP_COL 0xFF0000FF
#define STONE_FLOOR_BOTTOM_COL 0x630000FF
#define SPAWN_POINT_COL 0xFFFF00FF
#define WATER_TOP_COL 0x0094FFFF

		extern gene::HashMap<unsigned int, Tile> TileTypes;
	}
}