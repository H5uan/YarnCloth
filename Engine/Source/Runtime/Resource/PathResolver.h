#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class PathResolver {
public:
    static PathResolver& GetInstance() {
        static PathResolver instance;
        return instance;
    }

    static void Init(const fs::path&rootPath) {
        GetInstance().m_rootPath = rootPath;
    }

    fs::path Resolve(const fs::path&identifier) const {
        return m_rootPath / identifier;
    }

    [[nodiscard]] fs::path AsIdentifier(const fs::path&path) const {
        if (path.string().find(m_rootPath.string()) == 0) {
            return path.string().substr(m_rootPath.string().length());
        }
        return path;
    }

private:
    PathResolver() = default;

    ~PathResolver() = default;

    PathResolver(const PathResolver&) = delete;

    PathResolver& operator=(const PathResolver&) = delete;

    fs::path m_rootPath;
};
