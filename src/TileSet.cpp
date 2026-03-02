#include "TileSet.h"
#include <memory>
#include <filesystem>

void TileSet::LoadFromFolder(const std::string& folderPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
            continue;

        std::string path = entry.path().string();

        // Filtrar extensões
        if (path.ends_with(".png") || path.ends_with(".jpg"))
        {
            textures.push_back(std::make_shared<Texture>(path));
        }
    }
}
