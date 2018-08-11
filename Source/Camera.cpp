#include "Camera.h"
#include <Core/Random.h>
using namespace gene;

void ld42::Camera::Update()
{
	if (shakeTimer.ElapsedTimeMs() < ShakeTime)
	{
		float angle = 1.5f * shakeforce * Random::FloatRange(-1.0f, 1.0f);

		float offsetX = 0.01f * shakeforce * Random::FloatRange(-1.0f, 1.0f);
		float offsetY = 0.01f * shakeforce * Random::FloatRange(-1.0f, 1.0f);
		Angle = angle;
		Position = Vector3(offsetX, offsetY, 0.0f);
	}
	else {
		shakeTimer.Stop();
		Angle = 0.0f;
		Position = lastPos;
		ShakeTime = 0.0f;
		shakeforce = 0.0f;
	}
}

void ld42::Camera::Shake(float time, float force)
{
	ShakeTime = time;
	shakeTimer.Start();
	shakeforce = force;
	lastPos = Position;
}
