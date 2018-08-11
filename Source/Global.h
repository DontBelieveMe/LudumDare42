#pragma once

#include <Platform/Window.h>
#include <Math/Vector2.h>

#define TILE_SIZE 16.f;

namespace ld42 {
	class Spritesheet;
	class Player;
	class TileMap;
	class Camera;

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

		extern gene::Vector2i RED_TILE;	
	}
}