#pragma once
#include <vector>
#include <memory>
#include "Texture.h"

class TileSet {
public:
	std::vector<std::shared_ptr<Texture>> textures;

	void LoadFromFolder(const std::string& folderPath);
};