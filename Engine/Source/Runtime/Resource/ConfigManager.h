#pragma once

#include <Core/PublicSingleton.h>

#include <filesystem>
#include <glm/glm.hpp>

class ConfigManager final : public PublicSingleton<ConfigManager> {
public:
    ConfigManager();

    ConfigManager(const ConfigManager&) = delete;

    ConfigManager& operator=(const ConfigManager&) = delete;

public:
    void Clear();

    [[nodiscard]] const std::filesystem::path& GetRootFolder() const;

    [[nodiscard]] const std::filesystem::path& GetAssetsFolder() const;

    [[nodiscard]] const std::filesystem::path& GetShadersFolder() const;

    [[nodiscard]] const std::filesystem::path& GetResourcesFolder() const;

    static glm::vec2 mViewportSize;
    static int selectedEntity;

private:
    std::filesystem::path mRootFolder;
    std::filesystem::path mAssetsFolder;
    std::filesystem::path mShadersFolder;
    std::filesystem::path mResourcesFolder;
};
