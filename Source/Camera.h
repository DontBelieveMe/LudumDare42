#pragma once

#include <Math/Matrix4.h>
#include <Math/Vector3.h>

namespace ld42 {
	class Camera {
	public:
		gene::Vector3 Position;

		gene::Matrix4 CalculateViewMatrix() {
			using namespace gene;

			Vector3 pos;
			pos.X = -Position.X;
			pos.Y = -Position.Y;

			Matrix4 translation = Matrix4::Translate(pos);
		
			return translation;
		}
	};
}