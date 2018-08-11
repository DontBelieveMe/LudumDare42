#pragma once

#include <Core/Array.h>
#include <Core/String.h>
#include <Math/Vector2.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/../../../ThirdParty/lodepng/lodepng.h>

#include "Global.h"
#include "Spritesheet.h"
#include "Config.h"

namespace ld42 {
	using namespace gene;

	class Tile {
	public:
		Tile(Vector2i uv, bool solid) : UV(uv), Solid(solid) {}
		Tile() {}

	public:
		Vector2i UV;
		bool Solid;
	};

	class TileMap {
	public:
		unsigned w, h;
		Array<unsigned int> Tiles;

		void Load(const String& string) {
			std::vector<unsigned char> pixels;
			lodepng::decode(pixels, w, h, string.c_str());
			Tiles.clear();
			Tiles.resize(w*h);
			for (int i = 0; i < w*h; i++)
				Tiles[i] = 0;
			
			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					unsigned char r = pixels[(x * 4) + (y * 4) * w];
					unsigned char g = pixels[(x * 4) + 1 + (y * 4) * w];
					unsigned char b = pixels[(x * 4) + 2 + (y * 4) * w];
					unsigned char a = pixels[(x * 4) + 3 + (y * 4) * w];
					auto c = gene::graphics::Color(r, g, b, a);
					unsigned int id = c.Hex;
					
					switch (id) {
					case SPAWN_POINT_COL:
						config::PlayerSpawnPoint = Vector2(x * 16.f, y * 16.f);
						id = 0;
						break;
					case 255:
						id = 0;
						break;
					}

					Tiles[x + y * w] = id;
				}
			}
		}

		void Draw(graphics::Renderer2D* renderer) {
			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					unsigned int tile = Tiles[x + y * w];
					if (tile != 0) {
						Vector2i t = global::TileTypes[tile].UV;
						global::TilesSheet->DrawSprite({ x*16.f, y*16.f, 0.0f }, renderer, t.X, t.Y, 16.f, 16.f);
					}
				}
			}
		}
	private:
	};
}