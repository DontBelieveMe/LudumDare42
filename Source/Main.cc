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

#include <Windows.h>

namespace ld42 {
	class LudumDare42App : public gene::App {
		gene::graphics::Renderer2D m_2drenderer;
		gene::graphics::Texture2D m_StoneTexture, m_CrateTexture;
		ld42::Level *level1;
		gene::graphics::Renderer2D m_uiRenderer;
		gene::graphics::Font m_Font;

	public:

		virtual void Init() override {
			using namespace gene;
			//ld42::CopyAssetsDirectory();

			platform::Window *window = GetWindow();
			ld42::global::Window = window;
			ld42::global::WindowWidth = window->Width() / 4.0f;
			ld42::global::WindowHeight = window->Height() / 4.0f;

			window->SetClearColor(graphics::Color::Black);

			m_2drenderer.Init(Matrix4::Orthographic(window->Width(), 0.f, 0.f, window->Height(), 100.f, -1.0f));
			m_uiRenderer.Init(Matrix4::Orthographic(window->Width(), 0.f, 0.f, window->Height(), 100.f, -1.0f));

			m_uiRenderer.GetShader()->Enable();
			m_uiRenderer.GetShader()->LoadUniform1f("u_Ambient", 1.0f);
			m_uiRenderer.GetShader()->Disable();

			m_Font.Load("Data/Fonts/BLKCHCRY.TTF", 9);

			m_2drenderer.GetShader()->Enable();
			m_2drenderer.GetShader()->LoadUniform1f("u_Ambient", 1.0f);
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

			ld42::global::GlobalFont = &m_Font;
		}

		virtual void Tick(const gene::platform::GameTime& time) override {
			auto player = ld42::global::ThePlayer;
			auto keyboard = ld42::global::Window->GetInputController()->GetKeyDevice();
			auto mouse = ld42::global::Window->GetInputController()->GetMouseDevice();

			if (global::TheGameState == global::GameState::MainMenu) {
				if (global::KeyPressed(input::Keys::T)) {
					global::TheGameState = global::GameState::Playing;
					ld42::global::ThePlayer->Load();
				}
			}
			else {
				if (global::GameWon) {
					if (global::KeyPressed(input::Keys::T)) {
						exit(0);
					}
					return;
				}

				if (!player->Dead) {
					level1->Update(time);
					ld42::global::ThePlayer->Tick(time);

				}
				else {
					if (global::KeyPressed(input::Keys::T)) {
						player->Reset();
					}
				}
			}
			
			memcpy(ld42::global::LastKeyState, keyboard->GetKeyMap(), 62256);
			global::LastMouseButtonState = mouse->GetRawButtonState();
		}
		
		Vector3 BarPos = Vector3(124, 10, 0.f);
		
		virtual void Draw() override {
			
			auto player = ld42::global::ThePlayer;
			if (global::TheGameState == global::GameState::MainMenu) {
				m_uiRenderer.Begin();
				String title = "Running out of space (and more importantly, food)";
				Vector2 size = m_Font.MeasureString(title);
				m_uiRenderer.PushTransform(Matrix4::Scale(1.5f));
				m_uiRenderer.DrawString(&m_Font, title, Vector2((1280 / 2 - (size.X / 2)*1.5f) - 50.f, (720 / 2)-300), graphics::Color::White, graphics::TextAlignment::Centre);
				m_uiRenderer.PopTransform();

				String str = "A/D to move, Space to Jump\nSome switches can be toggled by throwing stones at them\n(aim with mouse and hold down the left mouse button, release to throw.\nThe longer you hold it down the faster you throw)\n\nYour task? Survive a single minute\nGood luck\n\nPress 't' to play.";
				size = m_Font.MeasureString(str);

				m_uiRenderer.DrawString(&m_Font, str, Vector2(1280 / 2 - size.X / 2, 720 / 2), graphics::Color::White, graphics::TextAlignment::Centre);
				m_uiRenderer.End();
				m_uiRenderer.Present();
			}
			else {
				if (global::GameWon) {
					m_uiRenderer.Begin();
					String str = "You Won!\nWell done!\nLooks like you didn't _run out of space_\n :D\n\nPress 't' to exit game.";
					Vector2 size = m_Font.MeasureString(str);
					m_uiRenderer.DrawString(&m_Font, str, Vector2(1280 / 2 - size.X / 2, 720 / 2), graphics::Color::White, graphics::TextAlignment::Centre);
					m_uiRenderer.End();
					m_uiRenderer.Present();
					return;
				}

				if (!player->Dead) {
					m_2drenderer.SetViewMatrix(ld42::global::MainCamera->CalculateViewMatrix());
					m_2drenderer.Begin();
					m_2drenderer.PushTransform(gene::Matrix4::Scale((4.f)));
					level1->Draw(&m_2drenderer);
					ld42::global::ThePlayer->Draw(&m_2drenderer);
					m_2drenderer.PopTransform();
					m_2drenderer.End();
					m_2drenderer.Present();

					m_uiRenderer.Begin();
					m_uiRenderer.FillRectangle(BarPos, ld42::global::ThePlayer->Health, 25.f, graphics::Color::Red);
					m_uiRenderer.DrawString(&m_Font, "Health", Vector2(10, 30), graphics::Color::White);
					m_uiRenderer.End();
					m_uiRenderer.Present();
				}
				else {
					m_uiRenderer.Begin();
					Vector2 size = m_Font.MeasureString("You died.\nHorrifically.\nPress any key to try again, I guess.");
					m_uiRenderer.DrawString(&m_Font, "You died.\nHorrifically.\nAnd alone.\n\nPress 't' to try again, I guess.", Vector2(1280 / 2 - size.X / 2, 720 / 2), graphics::Color::White, graphics::TextAlignment::Centre);
					m_uiRenderer.End();
					m_uiRenderer.Present();
				}
			}
		}

		virtual void GuiDraw() override {
			if (false) {
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

				if (ImGui::Button("CLick to toggle player death")) {
					global::ThePlayer->DiesOfHunger = !global::ThePlayer->DiesOfHunger;
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
