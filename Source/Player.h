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
		void ResolveCollisions();

	public:
		bool OnGround = false;

		Player();

		void Draw(graphics::Renderer2D* renderer);

		void Tick(const platform::GameTime& time);
		
		Vector3 Position;
		Vector3 Velocity;
	private:

	};

}