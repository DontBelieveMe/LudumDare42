#include "Utils.h"

#include <filesystem>

bool ld42::DebugMode = true;

void ld42::CopyAssetsDirectory(const gene::String& subdir)
{
	namespace fs = std::experimental::filesystem;
	fs::copy(ASSETS_DIRECTORY + subdir, ASSETS_OUT_DIRECTORY + subdir, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}
