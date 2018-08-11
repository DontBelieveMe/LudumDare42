#pragma once

#include <Graphics/Renderer2D.h>
#include <Platform/Time.h>

#include "TileMap.h"
#include "Camera.h"
#include "Global.h"
#include "Config.h"

namespace ld42 {
	using namespace gene;

	class Player {
	private:
	public:
		bool OnGround = false;

		Player() {
			config::PlayerLight.Colour = graphics::Color(234, 121, 42, 255);
			config::PlayerLight.Position = Position;
			config::PlayerLight.Size = 510;
			config::PlayerLight.Intensity = 3.f;
			config::PlayerLight.Falloff = .9f;
		}

		void Draw(graphics::Renderer2D* renderer) {
			global::TilesSheet->DrawSprite(Position, renderer, 0.0f, 1.0f, 16.0f, 16.0f);

			graphics::GLSLShader *shader = renderer->GetShader();
			shader->Enable();
			renderer->LoadLight(&config::PlayerLight, 0);
			shader->LoadUniform1f("u_Ambient", config::AmbientLight);
			shader->Disable();
		}

		void Tick(const platform::GameTime& time) {
			auto keyboard = global::Window->GetInputController()->GetKeyDevice();

			if (Velocity.Y < 0.5f) {
				Velocity.Y += config::Gravity;
			}

			if (global::KeyPressed(input::Keys::Space) && OnGround) {
				Velocity.Y = -config::JumpSpeed;
				OnGround = false;
			}

			if (true) {
				if (keyboard->IsKeyPressed(input::Keys::RightArrow)) {
					Velocity.X = config::PlayerSpeed;
				}
				else if (keyboard->IsKeyPressed(input::Keys::LeftArrow)) {
					Velocity.X = -config::PlayerSpeed;
				}
				else {
					Velocity.X = 0.0f;
				}
			}

			Vector3 potPos = Position + Velocity * time.DeltaInMilliSeconds();

			if (potPos.Y + 16 > global::WindowHeight) {
				Velocity.Y = 0.0f;
				OnGround = true;
			}

			Vector3 halfSize(8.0f, 8.0f, 8.0f);
			Position = Position + Velocity * time.DeltaInMilliSeconds();
			config::PlayerLight.Position = Position + halfSize;
			config::PlayerLight.Position.X *= 4.f;
			config::PlayerLight.Position.Y *= 4.f;

			auto camera = global::MainCamera;
			float camWidth = global::Window->Width();

			const float camDeadZone = 400;
			camera->Position.X = Position.X * 4.f - (camWidth / 2) + 32;

			const float tilesize = TILE_SIZE;
			for (int y = 0; y < global::ActiveLevel->h; ++y) {
				for (int x = 0; x < global::ActiveLevel->w; ++x) {
					if (global::ActiveLevel->Tiles[x + y * global::ActiveLevel->w]) {
						Vector2 tilePos(x*tilesize, y*tilesize);

						bool x1 = tilePos.X > Position.X + 16.f;
						bool x2 = tilePos.X + 16.f < Position.X;

						bool y1 = tilePos.Y > Position.Y + 16.f;
						bool y2 = tilePos.Y + 16.f < Position.Y;

						bool collision = !(x1 || x2 || y1 || y2);

						if (collision) {
							Vector3 playerCentre = Position + halfSize;
							Vector3 tileCentre = Vector3(tilePos.X, tilePos.Y, 0.0f) + halfSize;
							Vector3 correction = tileCentre - playerCentre;

							Vector3 dist;
							dist.X = std::abs(correction.X);
							dist.Y = std::abs(correction.Y);
							dist = dist - (halfSize * 2);

							if (dist.X > dist.Y)
							{
								if (correction.X > 0)
									Position.X += dist.X;
								else
									Position.X -= dist.X;
								Velocity.X = 0.0f;
							}
							else {
								if (correction.Y > 0) {
									Position.Y += dist.Y;
									OnGround = true;
								}
								else {
									Position.Y -= dist.X;
								}
								Velocity.Y = 0.0f;
							}
						}
					}
				}
			}
		}

		Vector3 Position;
		Vector3 Velocity;
	private:

	};

}