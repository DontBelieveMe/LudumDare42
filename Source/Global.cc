#include "Config.h"
#include "Global.h"

#include <Graphics/ImGui.h>
#include "TileMap.h"
#include "Level.h"
using namespace ld42;

float config::Gravity = 0.039f;
float config::JumpSpeed = 0.5f;
float config::AmbientLight = 0.0f;
float config::PlayerSpeed = 0.15f;
float config::StoneThrowSpeed = 0.5f;

gene::Vector2 config::PlayerSpawnPoint(0, 0);

gene::graphics::Light config::PlayerLight = gene::graphics::Light();

void config::ImGuiConfigEdit()
{
	ImGui::DragFloat("Gravity", &Gravity, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Ambient Light", &AmbientLight, 0.001f, 0.0f, 1.0f);

	ImGui::Separator();
	ImGui::Text("Player Settings");
	ImGui::DragFloat("Player Speed", &PlayerSpeed, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Player Jump Speed", &JumpSpeed, 0.001f, 0.0f, 2.0f);
	ImGui::DragFloat("Player light size", &PlayerLight.Size, 10.0f, 0.0f, 1000.0f);
	ImGui::DragFloat("Player light intensity", &PlayerLight.Intensity, 0.1f, 0.0f, 3.0f);
	ImGui::DragFloat("Player light falloff", &PlayerLight.Falloff, 0.1f, 0.0f, 3.0f);
	ImGui::DragFloat("Throw Stone Speed", &config::StoneThrowSpeed, 0.1f, 0.0f, 5.0f);
}

float global::WindowWidth = -1;
float global::WindowHeight = -1;

gene::platform::Window *global::Window = nullptr;
Spritesheet* global::TilesSheet = nullptr;
gene::input::Key global::LastKeyState[62256];
Player *global::ThePlayer = nullptr;
Level* global::ActiveLevel = nullptr;
Camera*  global::MainCamera = nullptr;

gene::input::MouseButton global::LastMouseButtonState = gene::input::MouseButton::None;

gene::HashMap<unsigned int, Tile> global::TileTypes = {
	{ STONE_FLOOR_TOP_COL, Tile(gene::Vector2i(0, 0), true, false) },
	{ STONE_FLOOR_BOTTOM_COL, Tile(gene::Vector2i(1, 0), true,false) },
	{ WATER_TOP_COL, Tile(gene::Vector2i(0, 2), false,true) },
	{ STONE_OVERHAND_RIGHT1, Tile(gene::Vector2i(2, 0), true, false)},
	{ SWITCH_V_CLOSED, Tile(gene::Vector2i(1, 3), false, false) },
};

bool global::KeyPressed(gene::input::Keys key) {
	return gene::input::KeyDevice::IsKeyDownInMap(Window->GetInputController()->GetKeyDevice()->GetKeyMap(), key) &&
		!gene::input::KeyDevice::IsKeyDownInMap(global::LastKeyState, key);
}

bool global::KeyDown(gene::input::Keys key) {
	return global::Window->GetInputController()->GetKeyDevice()->IsKeyPressed(key);
}

bool global::AABBIntersection(const gene::graphics::AABBRectangle& a, const gene::graphics::AABBRectangle& b)
{
	using namespace gene;

	Vector2 aPos = a.TopLeft;
	Vector2 bPos = b.TopLeft;

	Vector2 aSize = a.BottomRight - a.TopLeft;
	Vector2 bSize = b.BottomRight - a.TopLeft;

	return (abs(aPos.X - bPos.X) * 2 < (aSize.X + bSize.X)) &&
		(abs(aPos.Y - bPos.Y) * 2 < (aSize.Y + bSize.Y));
}

bool ld42::global::MouseClicked(gene::input::MouseButton button)
{
	return Window->GetInputController()->GetMouseDevice()->IsButtonDown(button) && !(
			static_cast<unsigned>(LastMouseButtonState) & static_cast<unsigned>(button)
		);
}

bool ld42::global::MouseUp(gene::input::MouseButton button)
{
	return !Window->GetInputController()->GetMouseDevice()->IsButtonDown(button) && (
		static_cast<unsigned>(LastMouseButtonState) & static_cast<unsigned>(button)
		);
}
