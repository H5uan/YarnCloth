#include "AssetManager.h"
#include "ConfigManager.h"

std::filesystem::path AssetManager::GetFullPath(const std::string&RelativePath) {
    std::filesystem::path temp(RelativePath);
    return ConfigManager::GetInstance().GetRootFolder() / temp;
}
