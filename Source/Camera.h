#pragma once

#include <Math/Matrix4.h>
#include <Math/Vector3.h>
#include <Platform/Time.h>

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
			
			return translation * Matrix4::RotateZ(Angle);
		}

		void Update();
		void Shake(float time, float force);
		float Angle = 0.0f;
		float ShakeTime;
		float shakeforce;
		gene::Vector3 lastPos;
		gene::platform::Timer shakeTimer;
	};
}