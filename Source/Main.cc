#include <Runtime/Application.h>
#include <filesystem>

void CopyAssetsDirectory(const gene::String& subdir = "")
{
	namespace fs = std::experimental::filesystem;
	fs::copy(ASSETS_DIRECTORY + subdir, ASSETS_OUT_DIRECTORY + subdir, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

class LD42 : public gene::App {
private:
public:
    virtual void Init() override { 
        CopyAssetsDirectory();
    }  
	 
    virtual void Tick(const gene::platform::GameTime& time) override {
	}
	 
    virtual void Draw() override {       
    }

    virtual void GuiDraw() override {
	}
};

int GeneMain(int argc, char **argv)
{ 
    gene::App* mapDemo = new LD42();
    mapDemo->Run(1280, 720, "Ludum Dare 42 - Project Edison");

    return 0;
}
