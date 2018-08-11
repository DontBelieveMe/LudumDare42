#include <Runtime/Application.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/ImGui.h>

#include "Utils.h"
#include "Camera.h"
#include "Config.h"
#include "Global.h"
#include "Spritesheet.h"
#include "TileMap.h"
#include "Camera.h"
#include "Player.h"
#include "Level.h"

using namespace gene;

namespace ld42 {
	class LudumDare42App : public gene::App {
		gene::graphics::Renderer2D m_2drenderer;
		gene::graphics::Texture2D m_StoneTexture, m_CrateTexture;
		ld42::Level *level1;

	public:

		virtual void Init() override {
			using namespace gene;
			ld42::CopyAssetsDirectory();

			platform::Window *window = GetWindow();
			ld42::global::Window = window;
			ld42::global::WindowWidth = window->Width() / 4.0f;
			ld42::global::WindowHeight = window->Height() / 4.0f;

			window->SetClearColor(graphics::Color::Black);

			m_2drenderer.Init(Matrix4::Orthographic(window->Width(), 0.f, 0.f, window->Height(), 100.f, -1.0f));

			m_2drenderer.GetShader()->Enable();
			m_2drenderer.GetShader()->LoadUniform1f("u_Ambient", 0.1f);
			m_2drenderer.GetShader()->Disable();

			ld42::global::TilesSheet = new ld42::Spritesheet;
			ld42::global::TilesSheet->Init("Data/Textures/Tiles.png");


			level1 = new Level1();
			level1->Load();

			ld42::global::ActiveLevel = level1;

			ld42::global::MainCamera = new ld42::Camera;

			ld42::global::ThePlayer = new ld42::Player;
			
			Array<graphics::Light*> lights;
			lights.push_back(&ld42::config::PlayerLight);
			m_2drenderer.LoadLights(lights);
		}

		virtual void Tick(const gene::platform::GameTime& time) override {
			auto keyboard = ld42::global::Window->GetInputController()->GetKeyDevice();
			auto mouse = ld42::global::Window->GetInputController()->GetMouseDevice();
			level1->Update(time);
			ld42::global::ThePlayer->Tick(time);

			memcpy(ld42::global::LastKeyState, keyboard->GetKeyMap(), 62256);
			global::LastMouseButtonState = mouse->GetRawButtonState();
		}

		virtual void Draw() override {
			m_2drenderer.SetViewMatrix(ld42::global::MainCamera->CalculateViewMatrix());
			m_2drenderer.Begin();
			m_2drenderer.PushTransform(gene::Matrix4::Scale((4.f)));
			level1->Draw(&m_2drenderer);
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
					level1->Load();
				}

				if (ImGui::Button("Reset player to spawn")) {
					global::ThePlayer->Position = Vector3(config::PlayerSpawnPoint.X, config::PlayerSpawnPoint.Y, 0.0f);
				}
				ImGui::Separator();
				ImGui::Text("Game");
				ld42::config::ImGuiConfigEdit();
				ImGui::End();
			}
		}
	};
}

int GeneMain(int argc, char **argv)
{
	gene::App* mapDemo = new ld42::LudumDare42App();
	mapDemo->Run(1280, 720, "App Demo!");

	return 0;
}
