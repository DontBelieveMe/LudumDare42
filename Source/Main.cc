#include <Runtime/Application.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/ImGui.h>

#include "Utils.h"

using namespace gene;

namespace ld42 {
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
			rect.TopRight = Vector2((ix * (sizex/m_texture.Width()))+(sizex/m_texture.Width()), iy * (m_texture.Height() / sizey));
			rect.BottomLeft = Vector2(ix * (sizex/m_texture.Width()), (iy * (sizey/m_texture.Height())) + (sizey/m_texture.Height()));
			rect.BottomRight = Vector2((ix * (sizex/m_texture.Width())) + (sizex/m_texture.Width()), (iy * (sizey/m_texture.Height())) + (sizex/m_texture.Height()));
			renderer->DrawTextureBounds(pos, &m_texture, rect, { sizex, sizey });
		}
	private:
	};
}

class ClearColorDemo : public gene::App {
	gene::graphics::Renderer2D m_2drenderer;
	gene::graphics::Texture2D m_StoneTexture, m_CrateTexture;
	ld42::Spritesheet m_spritesheet;

public:

	virtual void Init() override {
		using namespace gene;
		ld42::CopyAssetsDirectory();

		platform::Window *window = GetWindow();
		window->SetClearColor(gene::graphics::Color::Black/*gene::graphics::Color(79, 87, 99, 255)*/);

		m_2drenderer.Init(Matrix4::Orthographic(window->Width(), 0.f, 0.f, window->Height(), 100.f, -1.0f));
		
		// Disable lighting
		m_2drenderer.GetShader()->Enable();
		m_2drenderer.GetShader()->LoadUniform1f("u_Ambient", 1.0f);
		m_2drenderer.GetShader()->Disable();

		m_spritesheet.Init("Data/Textures/Tiles.png");
	}

	virtual void Tick(const gene::platform::GameTime& time) override {

	}

	virtual void Draw() override {
		m_2drenderer.Begin();

		m_2drenderer.PushTransform(gene::Matrix4::Scale((4.f)));
		m_spritesheet.DrawSprite({ 10, 10,0.0f }, &m_2drenderer, 0.0f, 0.0f, 16.f, 16.f);
		m_2drenderer.PopTransform();
		m_2drenderer.End();
		m_2drenderer.Present();
	}

	virtual void GuiDraw() override {
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

		ImGui::End();
	}
};

int GeneMain(int argc, char **argv)
{
	gene::App* mapDemo = new ClearColorDemo();
	mapDemo->Run(1280, 720, "App Demo!");

	return 0;
}
