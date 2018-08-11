#pragma once

#include <Core/Array.h>
#include <Core/String.h>
#include <Math/Vector2.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/../../../ThirdParty/lodepng/lodepng.h>

#include "Global.h"
#include "Spritesheet.h"

namespace ld42 {
	using namespace gene;

	class TileMap {
	private:
		HashMap<unsigned int, Vector2i*> TileLookup = {
			{ 0xFF0000FF, &global::RED_TILE },
		{ 0x000000FF, nullptr }
		};

	public:
		unsigned w, h;
		Array<Vector2i*> Tiles;

		void Load(const String& string) {
			std::vector<unsigned char> pixels;
			lodepng::decode(pixels, w, h, string.c_str());
			Tiles.clear();
			Tiles.resize(w*h);
			for (int i = 0; i < w*h; i++)Tiles[i] = nullptr;
			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					unsigned char r = pixels[(x * 4) + (y * 4) * w];
					unsigned char g = pixels[(x * 4) + 1 + (y * 4) * w];
					unsigned char b = pixels[(x * 4) + 2 + (y * 4) * w];
					unsigned char a = pixels[(x * 4) + 3 + (y * 4) * w];
					auto c = gene::graphics::Color(r, g, b, a);
					Tiles[x + y * w] = TileLookup[c.Hex];
				}
			}
		}

		void Draw(graphics::Renderer2D* renderer) {
			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					Vector2i *tile = Tiles[x + y * w];
					if (tile) {
						auto t = *tile;
						global::TilesSheet->DrawSprite({ x*16.f, y*16.f, 0.0f }, renderer, t.X, t.Y, 16.f, 16.f);
					}
				}
			}
		}
	private:
	};
}