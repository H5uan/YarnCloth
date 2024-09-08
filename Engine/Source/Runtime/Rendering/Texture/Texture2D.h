#pragma once

#include <glad/glad.h>

#include <vector>
#include <memory>
#include <stdint.h>


class Texture2D;
using Texture2DPtr = std::shared_ptr<Texture2D>;


class Texture2D {
public:
    Texture2D();

    Texture2D(const uint32_t&width,
              const uint32_t&height,
              const GLenum&internalFormat,
              const bool&immutable = false);

    Texture2D(const uint32_t&width,
              const uint32_t&height,
              const GLenum&internalFormat,
              const GLenum&dataFormat,
              const GLenum&dataType,
              const void* data,
              const bool&immutable = false);

    explicit Texture2D(const GLuint&id);

    ~Texture2D();

    [[nodiscard]] GLuint GetId() const { return m_ID; }

    void Bind() const;

    void Attach(const uint32_t&unit) const;

    void Unbind() const;

    [[nodiscard]] uint32_t GetWidth() const { return m_Width; }
    [[nodiscard]] uint32_t GetHeight() const { return m_Height; }

    void Resize(const uint32_t&width, const uint32_t&height);

    void SetData(const void* data, const GLenum&dataFormat, const GLenum&dataType);

    void SetData(const uint32_t&width, const uint32_t&height,
                 const GLenum&internalFormat,
                 const GLenum&dataFormat,
                 const GLenum&dataType,
                 const void* data);

    void GetData(const GLenum&pixelFormat, const GLenum&pixelType, const uint32_t&size, void* pixels) const;

    void GetData(const GLenum&pixelFormat, const GLenum&pixelType, void* pixels) const;

    void SetIntParameter(const GLenum&param, const int&value) const;

    void SetFloatParameter(const GLenum&param, const float&value) const;

    void SetFloatParameter(const GLenum&param, const float* value) const;

    void SetFilteringFlags(const GLenum&minFilter, const GLenum&magFilter) const;

    void SetWrappingFlags(const GLenum&wrapS,
                          const GLenum&wrapT) const;

    void EnableMipmaps(const bool&enable);

    static Texture2DPtr Create(const uint32_t&width,
                               const uint32_t&height,
                               const GLenum&internalFormat,
                               const bool&immutable = false);

    static Texture2DPtr Create(const uint32_t&width,
                               const uint32_t&height,
                               const GLenum&internalFormat,
                               const GLenum&dataFormat,
                               const GLenum&dataType,
                               const void* data,
                               const bool&immutable = false);

    static void ClearUnit(const uint32_t&unit);

private:
    GLuint m_ID;
    uint32_t m_Width, m_Height;
    GLenum m_InternalFormat;
    bool m_Mipmaps = false;
};
