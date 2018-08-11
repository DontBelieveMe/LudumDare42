#pragma once

#include <Core/String.h>

namespace ld42 {
	void CopyAssetsDirectory(const gene::String& subdir = "");

	extern bool DebugMode;
}