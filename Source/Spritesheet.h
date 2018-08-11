#pragma once

#include <Core/String.h>
#include <Graphics/Renderer2D.h>

namespace ld42 {
	using namespace gene;

	class Spritesheet {
	private:
		gene::graphics::Texture2D m_texture;
	public:
		void Init(const gene::String& filepath) {
			gene::graphics::TextureParameters p;
			p.Filtering = gene::graphics::FilteringOptions::Nearest;

			m_texture.Load(filepath.c_str(), p);
		}

		void DrawSprite(Vector3 pos, graphics::Renderer2D* renderer, float ix, float iy, float sizex, float sizey) {
			graphics::AABBRectangle rect;
			rect.TopLeft = Vector2(ix * (sizex / m_texture.Width()), iy * (sizey / m_texture.Height()));
			rect.TopRight = Vector2((ix * (sizex / m_texture.Width())) + (sizex / m_texture.Width()), iy * (sizey / m_texture.Height()));
			rect.BottomLeft = Vector2(ix * (sizex / m_texture.Width()), (iy * (sizey / m_texture.Height())) + (sizey / m_texture.Height()));
			rect.BottomRight = Vector2((ix * (sizex / m_texture.Width())) + (sizex / m_texture.Width()), (iy * (sizey / m_texture.Height())) + (sizex / m_texture.Height()));
			renderer->DrawTextureBounds(pos, &m_texture, rect, { sizex, sizey });
		}
	private:
	};
}