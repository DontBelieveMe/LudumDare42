#pragma once

#include <Platform/Window.h>
#include <Math/Vector2.h>
#include <Core/Array.h>
#include <Graphics/Font.h>

#define TILE_SIZE 16.f;

namespace ld42 {
	class Spritesheet;
	class Player;
	class TileMap;
	class Camera;
	class Tile;
	class Level;

	namespace global {
		enum GameState {
			MainMenu,
			Playing
		};

		extern Spritesheet *TilesSheet;
		extern gene::platform::Window *Window;
		extern float WindowWidth, WindowHeight;
		extern gene::input::Key LastKeyState[62256];
		extern Player *ThePlayer;
		extern Level* ActiveLevel;
		extern Camera *MainCamera;
		extern gene::input::MouseButton LastMouseButtonState;
		extern gene::graphics::Font *GlobalFont;
		bool KeyPressed(gene::input::Keys key);
		bool KeyDown(gene::input::Keys key);
		bool Collision(gene::Vector2 aPos, gene::Vector2 aSize, gene::Vector2 bPos, gene::Vector2 bSize);

		bool MouseClicked(gene::input::MouseButton button);
		bool MouseUp(gene::input::MouseButton button);

		extern bool GameWon;
		extern GameState TheGameState;

#define STONE_FLOOR_TOP_COL 0xFF0000FF
#define STONE_FLOOR_BOTTOM_COL 0x630000FF
#define SPAWN_POINT_COL 0xFFFF00FF
#define STONE_MIDDLE1 0x870000FF
#define WATER_TOP_COL 0x0094FFFF
#define WATER_TOP_COL2 -1
#define WATER_BOTTOM_COL 0x0026FFFF

#define STONE_OVERHAND_RIGHT1 0xB6FF00FF
#define STONE_OVERHANG_LEFT1 0x007F46FF
#define STONE_OVERHANG_RIGHT2 0x7F3300FF
#define STONE_OVERHANG_LEFT2 0x7F6A00FF

#define SWITCH_V_CLOSED 0xFF00DCFF
#define SWITCH_V_OPEN 0x7F006EFF

#define SWITCH_H_CLOSED 0xB200FFFF
#define SWITCH_H_OPEN 0x57007FFF

#define DEATH_ZONE 0xFF6A00FF

#define CHEST_CLOSED 0x007F7FFF
#define CHEST_OPEN 0x21007FFF
#define SKULL 0x808080FF
#define GRASS_FOOD 0xFFD800FF
		extern gene::HashMap<unsigned int, Tile> TileTypes;
	}
}