#pragma once

#include <Graphics/Renderer2D.h>
#include <Platform/Time.h>

#include "TileMap.h"
#include "Camera.h"
#include "Global.h"
#include "Config.h"

namespace ld42 {
	using namespace gene;

	struct Stone {
	public:
		Vector2 Velocity;
		Vector2 Position;
		
	private:
	};

	class Player {
	private:
		void ResolveCollisions();

	public:
		bool OnGround = false;

		Player();

		void Draw(graphics::Renderer2D* renderer);
		void Tick(const platform::GameTime& time);
		
		void Die();
		void ThrowStone(float angle, float speed, bool back);
		void TurnLightOff(float time);
		Vector3 Position;
		Vector3 Velocity;

		Stone* CurrentStone = nullptr;

		float Health = 100;
		bool Dead = false;

		void Reset() {
			Health = 100.0f;
			Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
			Dead = false;
			Velocity.X = 0.0f;
			Velocity.Y = 0.0f;
			if (CurrentStone) delete CurrentStone;
			CurrentStone = nullptr;
		}
	private:

	};

}