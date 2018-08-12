#pragma once

#include <Platform/Time.h>
#include <Graphics/Renderer2D.h>

#include "TileMap.h"
#include "Player.h"

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
		graphics::Light *Light;

		virtual void PreInit() {
			FilePath = "Data/Levels/Level1.png";
			Light = new graphics::Light;
			Light->Position = Vector3(39 * 16, 11 * 16, 0.f);
			Light->Colour = graphics::Color::White;
			Light->Falloff = 1.0f;
			Light->Intensity = 2.0f;
			Light->Size = 200.f;
		}

		virtual void Init() 
		{

		}
		platform::Timer shakeLightTimer;
		int action = 0;
		virtual void Update(const platform::GameTime& time)
		{
			Stone *stone = global::ThePlayer->CurrentStone;
			//14 15 16, 10
			float SWITCH_X = 14.0f;
			if (stone) {
				if (global::Collision(stone->Position, { 9, 7 }, { SWITCH_X *16,6*16 }, { 13,14 }))
				{
					if (Tiles->Tiles[SWITCH_X + 6 * Tiles->w] == SWITCH_V_CLOSED) {
						delete global::ThePlayer->CurrentStone;
						global::ThePlayer->CurrentStone = nullptr;
						global::MainCamera->Shake(2500.0f, 1.0f);
						shakeLightTimer.Start();
						action = 1;
						
					}
				}
			}

			if (shakeLightTimer.ElapsedTimeMs() > 500.0f && action == 1) {
				global::ThePlayer->TurnLightOff(1000.f);
				shakeLightTimer.Stop();
				Tiles->Tiles[14 + 10 * Tiles->w] = STONE_FLOOR_TOP_COL;
				Tiles->Tiles[15 + 10 * Tiles->w] = STONE_FLOOR_TOP_COL;
				Tiles->Tiles[16 + 10 * Tiles->w] = STONE_FLOOR_TOP_COL;
				Tiles->Tiles[17 + 10 * Tiles->w] = STONE_FLOOR_TOP_COL;
				Tiles->Tiles[18 + 10 * Tiles->w] = STONE_FLOOR_TOP_COL;

				Tiles->Tiles[SWITCH_X + 6 * Tiles->w] = SWITCH_V_OPEN;
			}
		}

		virtual void Draw(graphics::Renderer2D *renderer)
		{
			renderer->GetShader()->Enable();
			renderer->LoadLight(Light, 1);
			renderer->GetShader()->Disable();

			Tiles->Draw(renderer);
		}
	};
}