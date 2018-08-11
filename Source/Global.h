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
	class Level;

	namespace global {
		extern Spritesheet *TilesSheet;
		extern gene::platform::Window *Window;
		extern float WindowWidth, WindowHeight;
		extern gene::input::Key LastKeyState[62256];
		extern Player *ThePlayer;
		extern Level* ActiveLevel;
		extern Camera *MainCamera;
		extern gene::input::MouseButton LastMouseButtonState;

		bool KeyPressed(gene::input::Keys key);
		bool KeyDown(gene::input::Keys key);
		bool Collision(gene::Vector2 aPos, gene::Vector2 aSize, gene::Vector2 bPos, gene::Vector2 bSize);

		bool MouseClicked(gene::input::MouseButton button);
		bool MouseUp(gene::input::MouseButton button);


#define STONE_FLOOR_TOP_COL 0xFF0000FF
#define STONE_FLOOR_BOTTOM_COL 0x630000FF
#define SPAWN_POINT_COL 0xFFFF00FF
#define WATER_TOP_COL 0x0094FFFF
#define STONE_OVERHAND_RIGHT1 0xB6FF00FF
#define SWITCH_V_CLOSED 0xFF00DCFF
#define SWITCH_V_OPEN 0x7F006EFF

		extern gene::HashMap<unsigned int, Tile> TileTypes;
	}
}