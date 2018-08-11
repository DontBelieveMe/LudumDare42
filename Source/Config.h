#pragma once

#include <Graphics/Renderer2D.h>

namespace ld42 {
	namespace config {
		extern float Gravity;
		extern float PlayerSpeed ;
		extern float JumpSpeed;
		extern float AmbientLight;
		extern gene::graphics::Light PlayerLight;

		void ImGuiConfigEdit();
	}
}