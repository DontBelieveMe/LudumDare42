#include <Runtime/Application.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/ImGui.h>

#include "Utils.h"
#include <Graphics/../../../ThirdParty/lodepng/lodepng.h>

using namespace gene;

namespace ld42 {
	namespace config {
		float Gravity = 0.039f;
		float PlayerSpeed = 0.15f;
		float JumpSpeed = 0.5f;
		float AmbientLight = 0.0f;
		graphics::Light PlayerLight;

		void ImGuiConfigEdit()
		{
			ImGui::DragFloat("Gravity", &Gravity, 0.001f, 0.0f, 2.0f);
			ImGui::DragFloat("Ambient Light", &AmbientLight, 0.001f, 0.0f, 1.0f);

			ImGui::Separator();
			ImGui::Text("Player Settings");

			ImGui::DragFloat("Player Speed", &PlayerSpeed, 0.001f, 0.0f, 2.0f);
			ImGui::DragFloat("Player Jump Speed", &JumpSpeed, 0.001f, 0.0f, 2.0f);
//			graphics::ImGuiHelpers::ColorPicker("Player light color", &PlayerLight.Colour);
			ImGui::DragFloat("Player light size", &PlayerLight.Size, 10.0f, 0.0f, 1000.0f);
			ImGui::DragFloat("Player light intensity", &PlayerLight.Intensity, 0.1f , 0.0f, 3.0f);
			ImGui::DragFloat("Player light falloff", &PlayerLight.Falloff, 0.1f, 0.0f, 3.0f);
		}
	}

	class Spritesheet;
	class Player;
	class TileMap;

	namespace global {
		Spritesheet *TilesSheet;
		platform::Window *Window;
		float WindowWidth, WindowHeight;
		input::Key LastKeyState[62256];
		Player *ThePlayer;
		TileMap* ActiveLevel;

		float TILE_SIZE = 16.f;

		bool KeyPressed(input::Keys key) {
			return input::KeyDevice::IsKeyDownInMap(Window->GetInputController()->GetKeyDevice()->GetKeyMap(), key) && 
				!input::KeyDevice::IsKeyDownInMap(global::LastKeyState, key);
		}

		bool KeyDown(input::Keys key) {
			return Window->GetInputController()->GetKeyDevice()->IsKeyPressed(key);
		}

		Vector2i RED_TILE(0.0f, 0.0f);
		Vector2i NILL_TILE(-1.0f, -1.0f);
		
		bool AABBIntersection(const graphics::AABBRectangle& a, const graphics::AABBRectangle& b)
		{
			Vector2 aPos = a.TopLeft;
			Vector2 bPos = b.TopLeft;

			Vector2 aSize = a.BottomRight - a.TopLeft;
			Vector2 bSize = b.BottomRight - a.TopLeft;

			return (abs(aPos.X - bPos.X) * 2 < (aSize.X + bSize.X)) &&
				(abs(aPos.Y - bPos.Y) * 2 < (aSize.Y + bSize.Y));
		}
	}


	class Spritesheet {
	private:
		graphics::Texture2D m_texture;
	public:
		void Init(const String& filepath) {
			graphics::TextureParameters p;
			p.Filtering = graphics::FilteringOptions::Nearest;

			m_texture.Load(filepath.c_str(), p);
		}

		void DrawSprite(Vector3 pos, graphics::Renderer2D* renderer, float ix, float iy, float sizex, float sizey) {
			graphics::AABBRectangle rect;
			rect.TopLeft = Vector2(ix * (sizex/m_texture.Width()), iy * (sizey/m_texture.Height()));
			rect.TopRight = Vector2((ix * (sizex/m_texture.Width()))+(sizex/m_texture.Width()), iy * (sizey/m_texture.Height()));
			rect.BottomLeft = Vector2(ix * (sizex/m_texture.Width()), (iy * (sizey/m_texture.Height())) + (sizey/m_texture.Height()));
			rect.BottomRight = Vector2((ix * (sizex/m_texture.Width())) + (sizex/m_texture.Width()), (iy * (sizey/m_texture.Height())) + (sizex/m_texture.Height()));
			renderer->DrawTextureBounds(pos, &m_texture, rect, { sizex, sizey });
		}
	private:
	};

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

	class Player {
	private:
	public:
		bool OnGround = false;

		Player() {
			config::PlayerLight.Colour = graphics::Color(234, 121, 42,255);
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

			const float tilesize = global::TILE_SIZE;
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

class ClearColorDemo : public gene::App {
	gene::graphics::Renderer2D m_2drenderer;
	gene::graphics::Texture2D m_StoneTexture, m_CrateTexture;
	ld42::TileMap m_Tilemap;
public:

	virtual void Init() override {
		using namespace gene;
		ld42::CopyAssetsDirectory();

		platform::Window *window = GetWindow();
		ld42::global::Window = window;
		ld42::global::WindowWidth = window->Width() / 4.0f;
		ld42::global::WindowHeight = window->Height() / 4.0f;

		window->SetClearColor(gene::graphics::Color::Black/*gene::graphics::Color(79, 87, 99, 255)*/);

		m_2drenderer.Init(Matrix4::Orthographic(window->Width(), 0.f, 0.f, window->Height(), 100.f, -1.0f));
		
		// Disable lighting
		m_2drenderer.GetShader()->Enable();
		m_2drenderer.GetShader()->LoadUniform1f("u_Ambient", 0.1f);
		m_2drenderer.GetShader()->Disable();

		ld42::global::TilesSheet = new ld42::Spritesheet;
		ld42::global::TilesSheet->Init("Data/Textures/Tiles.png");

		ld42::global::ThePlayer = new ld42::Player;

		m_Tilemap.Load("Data/Levels/Level1.png");
		ld42::global::ActiveLevel = &m_Tilemap;


		Array<graphics::Light*> lights;
		lights.push_back(&ld42::config::PlayerLight);
		m_2drenderer.LoadLights(lights);
	}

	virtual void Tick(const gene::platform::GameTime& time) override {
		auto keyboard = ld42::global::Window->GetInputController()->GetKeyDevice();
		ld42::global::ThePlayer->Tick(time);

		memcpy(ld42::global::LastKeyState, keyboard->GetKeyMap(), 62256);

	}

	virtual void Draw() override {
		m_2drenderer.Begin();

		m_2drenderer.PushTransform(gene::Matrix4::Scale((4.f)));
		m_Tilemap.Draw(&m_2drenderer);
		ld42::global::ThePlayer->Draw(&m_2drenderer);
		m_2drenderer.PopTransform();
		m_2drenderer.End();
		m_2drenderer.Present();
	}

	virtual void GuiDraw() override {
		
		if (ld42::DebugMode) {
			ImGui::Begin("Assets");
			if (ImGui::Button("Refresh Assets Directory")) {
				ld42::CopyAssetsDirectory();
			}


			ImGui::Separator();
			ImGui::Text("Shaders");
			if (ImGui::Button("Reload Shaders")) {
				ld42::CopyAssetsDirectory("Shaders/");
				m_2drenderer.LoadShaders();
			}
			if (ImGui::Button("Reload Level")) {
				ld42::CopyAssetsDirectory("Levels/");
				m_Tilemap.Load("Data/Levels/Level1.png");
			}
			ImGui::Separator();
			ImGui::Text("Game");
			ld42::config::ImGuiConfigEdit();
			ImGui::End();
		}
	}
};

int GeneMain(int argc, char **argv)
{
	gene::App* mapDemo = new ClearColorDemo();
	mapDemo->Run(1280, 720, "App Demo!");

	return 0;
}
