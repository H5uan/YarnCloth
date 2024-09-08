#pragma once

#include "Rendering/Shader.h"
#include "Library/Library.h"

template<>
class Library<Shader> : public LibraryBase<Library, Shader> {
public:
    Library();

    [[nodiscard]] Ref<Shader> GetDefaultShader();

    [[nodiscard]] Ref<Shader> GetSkyBoxShader();
};
