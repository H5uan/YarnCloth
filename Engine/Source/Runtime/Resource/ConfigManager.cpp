#include "ConfigManager.h"

#include "Core/Base.h"
#include "Core/Core.h"
#include "Core/Log.h"

glm::vec2 ConfigManager::mViewportSize = {1280.0f, 720.0f};
int ConfigManager::selectedEntity = -1;

ConfigManager::ConfigManager() {
    mRootFolder = {HE_XSTRINGIFY_MACRO(ENGINE_ROOT_DIR)};
    LOG_INFO("Root Folder: {0}", mRootFolder.string());
    mAssetsFolder = mRootFolder / "Assets";
    LOG_INFO("Assets Folder: {0}", mAssetsFolder.string());
    mShadersFolder = mRootFolder / "Shaders";
    LOG_INFO("Shaders Folder: {0}", mShadersFolder.string());
    mResourcesFolder = mRootFolder / "Resources";
    LOG_INFO("Resources Folder: {0}", mResourcesFolder.string());
}

void ConfigManager::Clear() {
    mRootFolder.clear();
    mAssetsFolder.clear();
    mShadersFolder.clear();
    mResourcesFolder.clear();
}

const std::filesystem::path& ConfigManager::GetRootFolder() const {
    GLCORE_ASSERT(std::filesystem::exists(mRootFolder));
    return mRootFolder;
}

const std::filesystem::path& ConfigManager::GetAssetsFolder() const {
    GLCORE_ASSERT(std::filesystem::exists(mAssetsFolder));
    return mAssetsFolder;
}

const std::filesystem::path& ConfigManager::GetShadersFolder() const {
    GLCORE_ASSERT(std::filesystem::exists(mShadersFolder));
    return mShadersFolder;
}

const std::filesystem::path& ConfigManager::GetResourcesFolder() const {
    GLCORE_ASSERT(std::filesystem::exists(mResourcesFolder));
    return mResourcesFolder;
}
