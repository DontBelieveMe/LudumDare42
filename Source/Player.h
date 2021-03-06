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
		void Load();

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
		bool DiesOfHunger = true;
		
		float HungerDepletionAmount = 1.0f;

		void Reset();
	private:

	};

}