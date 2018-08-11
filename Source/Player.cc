#include "Player.h"

#include <Math/Functions.h>
#include "Level.h"
using namespace ld42;
using namespace gene;

Player::Player() {
	config::PlayerLight.Colour = graphics::Color(234, 121, 42, 255);
	config::PlayerLight.Position = Position;
	config::PlayerLight.Size = 300.0f;
	config::PlayerLight.Intensity = 3.f;
	config::PlayerLight.Falloff = .9f;

	Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
}

void Player::Die()
{
	Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
}

void Player::ThrowStone(float angle, float speed)
{
	CurrentStone = new Stone;
	CurrentStone->Velocity = Vector2{
		speed * Maths::Cos(Maths::ToRadians(angle)),
		-(speed * Maths::Sin(Maths::ToRadians(angle)))
	};
	CurrentStone->Position = Vector2(Position.X, Position.Y+5);
}

void Player::Draw(graphics::Renderer2D* renderer) {
	global::TilesSheet->DrawSprite(Position, renderer, 0.0f, 1.0f, 16.0f, 16.0f);

	graphics::GLSLShader *shader = renderer->GetShader();
	shader->Enable();
	renderer->LoadLight(&config::PlayerLight, 0);
	shader->LoadUniform1f("u_Ambient", config::AmbientLight);
	shader->Disable();

	if (CurrentStone)
		global::TilesSheet->DrawSprite(Vector3(CurrentStone->Position.X, CurrentStone->Position.Y, 0.0f), renderer, 0.f, 3.f, 16.f,16.f);
}

float StoneForce = 0;

void Player::Tick(const platform::GameTime& time) {
	auto keyboard = global::Window->GetInputController()->GetKeyDevice();
	auto mouse = global::Window->GetInputController()->GetMouseDevice();

	if (mouse->IsButtonDown(input::MouseButton::Left)) {
		if(StoneForce < 10.f)
			StoneForce += 0.1f;
	}

	if (global::MouseUp(input::MouseButton::Left)) {
		float force = Maths::Map(StoneForce, 0.0f, 10.0f, 0.2f, 1.0f);
		float angle = 0.0f;
		Vector2 posActual(
			Position.X * 4.f,
			Position.Y * 4.f
		);

		Vector2i mousePosi = global::Window->GetInputController()->GetMouseDevice()->GetCursorPosition();
		Vector2  mousePos(
			mousePosi.X, mousePosi.Y
		);
		mousePos = mousePos + Vector2(global::MainCamera->Position.X, global::MainCamera->Position.Y);

		Vector2 d = mousePos - posActual;
		d.X = Maths::Absf(d.X);
		d.Y = Maths::Absf(d.Y);

		angle = Maths::Asin(d.Y / d.Length());

		ThrowStone(Maths::ToDegrees(angle), force);
		StoneForce = 0;
	}

	if (CurrentStone) {
		if (CurrentStone->Velocity.Y < 0.5f) {
			CurrentStone->Velocity += config::Gravity/2;
		}

		CurrentStone->Position = CurrentStone->Position + CurrentStone->Velocity * time.DeltaInMilliSeconds();
	}

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

	Vector3 halfSize(8.0f, 8.0f, 8.0f);
	Position = Position + Velocity * time.DeltaInMilliSeconds();
	ResolveCollisions();
	
	config::PlayerLight.Position = Position + halfSize;
	config::PlayerLight.Position.X *= 4.f;
	config::PlayerLight.Position.Y -= 16.f;
	config::PlayerLight.Position.Y *= 4.f;

	auto camera = global::MainCamera;
	float camWidth = global::Window->Width();
	float camHeight = global::Window->Height();

	camera->Position.X = Position.X * 4.f - (camWidth / 2) + 32;
	camera->Position.Y = Position.Y * 4.f - (camHeight / 2) + 32;
}

void Player::ResolveCollisions() 
{
	const float tilesize = TILE_SIZE;
	static Vector3 halfSize(8.0f, 8.0f, 0.0f);

	for (int y = 0; y < global::ActiveLevel->Tiles->h; ++y) {
		for (int x = 0; x < global::ActiveLevel->Tiles->w; ++x) {
			unsigned int tileID = global::ActiveLevel->Tiles->Tiles[x + y * global::ActiveLevel->Tiles->w];
			Tile& tile = global::TileTypes[tileID];
			
			if (tileID == 0) {
				continue;
			}

			Vector2 tilePos(x*tilesize, y*tilesize);

			bool x1 = tilePos.X > Position.X + 16.f;
			bool x2 = tilePos.X + 16.f < Position.X;

			bool y1 = tilePos.Y > Position.Y + 16.f;
			bool y2 = tilePos.Y + 16.f < Position.Y;

			bool collision = !(x1 || x2 || y1 || y2);

			if (collision) {
				if (tile.KillsYou) {
					Die();
					return;
				}

				if (!tile.Solid) continue;

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