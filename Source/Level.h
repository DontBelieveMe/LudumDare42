#pragma once

#include <Platform/Time.h>
#include <Graphics/Renderer2D.h>

#include "TileMap.h"

namespace ld42 {
	using namespace gene;

	struct Level {
		virtual void Update(const platform::GameTime& time) = 0;
		virtual void Init() = 0;
		virtual void Draw(graphics::Renderer2D *renderer) = 0;
		virtual void PreInit() {}

		TileMap *Tiles;
		String FilePath;

		void Load() {
			PreInit();
			Tiles = new TileMap();
			Tiles->Load(FilePath);
			Init();
		}
	};

	struct Level1 : Level {
		virtual void PreInit() {
			FilePath = "Data/Levels/Level1.png";
		}

		virtual void Init() 
		{

		}

		virtual void Update(const platform::GameTime& time)
		{

		}

		virtual void Draw(graphics::Renderer2D *renderer)
		{
			Tiles->Draw(renderer);
		}
	};
}