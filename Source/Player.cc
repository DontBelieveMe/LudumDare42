#include "Player.h"

#include <Math/Functions.h>
#include "Level.h"
using namespace ld42;
using namespace gene;
platform::Timer hungerTimer;
#include <Audio/WaveFile.h>
#include <Audio/AudioSystem.h>

audio::WaveFile *walking = new audio::WaveFile;
audio::WaveFile *fireCrackling = new audio::WaveFile;
audio::AudioSystem *audioSystem = new audio::AudioSystem;

platform::Timer gameWinTimer;

void Player::Reset()
{
	Health = 100.0f;
	Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
	Dead = false;
	Velocity.X = 0.0f;
	Velocity.Y = 0.0f;
	if (CurrentStone) 
		delete CurrentStone;
	
	CurrentStone = nullptr;
	HungerDepletionAmount = 1.0f;
	audioSystem->PlayWav(walking);
	audioSystem->PlayWav(fireCrackling);
	gameWinTimer.Stop();
	gameWinTimer.Start();
}

Player::Player() {
}

void ld42::Player::Load()
{
	config::PlayerLight.Colour = graphics::Color(234, 121, 42, 255);
	config::PlayerLight.Position = Position;
	config::PlayerLight.Size = 300.0f;
	config::PlayerLight.Intensity = 3.f;
	config::PlayerLight.Falloff = .9f;

	Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
	hungerTimer.Start();

	audioSystem->Init();

	fireCrackling->Load("Data/SFX/fire.wav");
	fireCrackling->Loop(true);
	fireCrackling->SetGain(0.3f);

	walking->Load("Data/SFX/walk.wav");
	walking->Loop(true);
	walking->SetGain(0.0f);

	audioSystem->PlayWav(fireCrackling);
	audioSystem->PlayWav(walking);
	gameWinTimer.Start();
}

void Player::Die()
{
	Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
	audioSystem->StopWav(walking);
	audioSystem->StopWav(fireCrackling);
	gameWinTimer.Stop();
	Dead = true;
}

void Player::ThrowStone(float angle, float speed, bool back)
{
	if (CurrentStone) 
		delete CurrentStone;
	
	CurrentStone = new Stone;
	CurrentStone->Velocity = Vector2{
		speed * Maths::Cos(Maths::ToRadians(angle)),
		-(speed * Maths::Sin(Maths::ToRadians(angle)))
	};
	
	if (back)
		CurrentStone->Velocity.X = -CurrentStone->Velocity.X;
	
	CurrentStone->Position = Vector2(Position.X, Position.Y+5);
}

platform::Timer lightOffTimer;
float lightOffTime;
float lightSize;
void ld42::Player::TurnLightOff(float time)
{
	lightOffTime = time;	
	lightOffTimer.Start();
	lightSize = config::PlayerLight.Size;
	config::PlayerLight.Size = 0.0f;
}

Vector2 FloatingTextPos;
String FloatingText;

void Player::Draw(graphics::Renderer2D* renderer) {
	global::TilesSheet->DrawSprite(Position, renderer, 0.0f, 1.0f, 16.0f, 16.0f);

	graphics::GLSLShader *shader = renderer->GetShader();
	shader->Enable();
	renderer->LoadLight(&config::PlayerLight, 0);
	shader->LoadUniform1f("u_Ambient", config::AmbientLight);
	shader->Disable();

	if (CurrentStone)
		global::TilesSheet->DrawSprite(Vector3(CurrentStone->Position.X, CurrentStone->Position.Y, 0.0f), renderer, 0.f, 3.f, 16.f,16.f);

	if (FloatingTextPos.X > 0) {
		renderer->PopTransform();
		FloatingTextPos = FloatingTextPos * 4.f;
		renderer->DrawString(global::GlobalFont, FloatingText, FloatingTextPos, graphics::Color::White, graphics::TextAlignment::Centre);
		renderer->PushTransform(Matrix4::Scale(4.0f));
	}
}

float StoneForce = 0;

void Player::Tick(const platform::GameTime& time) {
	if (gameWinTimer.ElapsedTimeMs() > 1000 * 60) {
		global::GameWon = true;
	}

	if (hungerTimer.ElapsedTimeMs() > 750 && DiesOfHunger) {
		Health -= HungerDepletionAmount*(gameWinTimer.ElapsedTimeSeconds()/2);
		hungerTimer.Stop();
		hungerTimer.Start();
	}

	if (Health <= 0) {
		Die();
	}

	if (lightOffTimer.Running()) {
		if (lightOffTimer.ElapsedTimeMs() > lightOffTime) {
			lightOffTimer.Stop();
			config::PlayerLight.Size = lightSize;
		}
	}

	auto keyboard = global::Window->GetInputController()->GetKeyDevice();
	auto mouse = global::Window->GetInputController()->GetMouseDevice();

	if (mouse->IsButtonDown(input::MouseButton::Left)) {
		if (StoneForce < 5.f) {
			StoneForce += 0.1f;
		}
		global::MainCamera->Shake(1.0f, StoneForce/16.f);
	}

	global::MainCamera->Update();

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
		
		d.Y = Maths::Absf(d.Y);

		angle = Maths::Asin(d.Y / d.Length());
		ThrowStone(Maths::ToDegrees(angle), force, d.X < 0);
		StoneForce = 0;
	}

	if (CurrentStone) {
		if (CurrentStone->Velocity.Y < 0.5f) {
			CurrentStone->Velocity.Y += config::Gravity/2;
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
		if (keyboard->IsKeyPressed(input::Keys::D)) {
			Velocity.X = config::PlayerSpeed;
			walking->SetGain(0.3f);
		}
		else if (keyboard->IsKeyPressed(input::Keys::A)) {
			Velocity.X = -config::PlayerSpeed;
			walking->SetGain(0.3f);
		}
		else {
			Velocity.X = 0.0f;
			walking->SetGain(0.0f);
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

platform::Timer handSwitchTimer;
bool turningOn;

void Player::ResolveCollisions() 
{
	if (handSwitchTimer.ElapsedTimeMs() > 500.f && handSwitchTimer.Running()) {
		handSwitchTimer.Stop();
		TurnLightOff(1000.f);

		//74, 1/2

		// Doing the action
		if (turningOn) {
			global::ActiveLevel->Tiles->Tiles[74 + 1 * global::ActiveLevel->Tiles->w] = 0;
			global::ActiveLevel->Tiles->Tiles[74 + 2 * global::ActiveLevel->Tiles->w] = 0;

			global::ActiveLevel->Tiles->Tiles[70 + 3 * global::ActiveLevel->Tiles->w] = STONE_FLOOR_TOP_COL;
			global::ActiveLevel->Tiles->Tiles[71 + 3 * global::ActiveLevel->Tiles->w] = STONE_FLOOR_TOP_COL;
			global::ActiveLevel->Tiles->Tiles[70 + 4 * global::ActiveLevel->Tiles->w] = STONE_MIDDLE1;
			global::ActiveLevel->Tiles->Tiles[71 + 4 * global::ActiveLevel->Tiles->w] = STONE_MIDDLE1;
		}
	}

	const float tilesize = TILE_SIZE;
	static Vector3 halfSize(8.0f, 8.0f, 0.0f);
	bool hitFood = false;
	bool hitSwitch = false;
	float fx, fy;
	bool dying = false;
	for (int y = 0; y < global::ActiveLevel->Tiles->h; ++y) {
		for (int x = 0; x < global::ActiveLevel->Tiles->w; ++x) {
			unsigned int tileID = global::ActiveLevel->Tiles->Tiles[x + y * global::ActiveLevel->Tiles->w];
			Tile& tile = global::TileTypes[tileID];
			
			if (tileID == 0) {
				continue;
			}

			Vector2 tilePos(x*tilesize, y*tilesize);
			if (CurrentStone)
			{
				if (tile.Solid) {
					if (global::Collision(CurrentStone->Position, { 9, 7 }, tilePos, { 16,16 }))
					{
						delete CurrentStone;
						CurrentStone = nullptr;
					}
				}
			}

			bool x1 = tilePos.X > Position.X + 16.f;
			bool x2 = tilePos.X + 16.f < Position.X;

			bool y1 = tilePos.Y > Position.Y + 16.f;
			bool y2 = tilePos.Y + 16.f < Position.Y;

			bool collision = !(x1 || x2 || y1 || y2);

			if (collision) {
				if (tileID == GRASS_FOOD && Health < 95.f) {
					hitFood = true;
					fx = x;
					fy = y;

					if (global::KeyPressed(input::Keys::E)) {
						global::ActiveLevel->Tiles->Tiles[x + y * global::ActiveLevel->Tiles->w] = 0.f;
						Health += Health + 10.f > 100.f ? 100 - Health : 10.f;
						FloatingTextPos.Set(-1.0f);
						FloatingText = "";
						continue;
					}

					continue;
				}

				if (tileID == SWITCH_H_CLOSED) {
					hitSwitch = true;
					fx = x;
					fy = y;
					if (global::KeyPressed(input::Keys::E)) {
						global::MainCamera->Shake(2500, 0.5f);
						handSwitchTimer.Start();
						/*if (tileID == SWITCH_H_OPEN) {
							global::ActiveLevel->Tiles->Tiles[x + y * global::ActiveLevel->Tiles->w] = SWITCH_H_CLOSED;
							turningOn = false;
							}*/
						global::ActiveLevel->Tiles->Tiles[x + y * global::ActiveLevel->Tiles->w] = SWITCH_H_OPEN;
						turningOn = true;
					}
					continue;
				}

				if (tileID == DEATH_ZONE) {
					Die();
					return;
				}

				if (tile.KillsYou) {
					HungerDepletionAmount = 40.f;			
					dying = true;
					if (tileID == DEATH_ZONE) return;
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

	if (!dying) {
		HungerDepletionAmount = 1.0f;
	}

	if (hitSwitch) {
		float strW = global::GlobalFont->MeasureString("'E' To Open").X;
		FloatingTextPos = Vector2((fx*16.f) - ((strW / 4) / 2.f), fy*16.f);
		FloatingText = "'E' To Open";
	} else if (hitFood) {
		float strW = global::GlobalFont->MeasureString("'E' To Eat").X;
		FloatingTextPos = Vector2((fx*16.f)-((strW/4)/2.f), fy*16.f);
		FloatingText = "'E' To Eat";
	}
	else {
		FloatingTextPos.Set(-1);
	}
}