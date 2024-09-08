#include "Texture2D.h"


Texture2D::Texture2D() : m_Width(0),
                         m_Height(0),
                         m_InternalFormat(0) {
    glGenTextures(1, &m_ID);
}

Texture2D::Texture2D(const GLuint&id) : m_ID(id),
                                        m_Width(0),
                                        m_Height(0),
                                        m_InternalFormat(0) {
}

Texture2D::Texture2D(const uint32_t&width,
                     const uint32_t&height,
                     const GLenum&internalFormat,
                     const bool&immutable) : m_Width(width),
                                             m_Height(height),
                                             m_InternalFormat(internalFormat) {
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    if (immutable) {
        glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat,
                     m_Width, m_Height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }
}

Texture2D::Texture2D(const uint32_t&width,
                     const uint32_t&height,
                     const GLenum&internalFormat,
                     const GLenum&dataFormat,
                     const GLenum&dataType,
                     const void* data,
                     const bool&immutable) : m_Width(width),
                                             m_Height(height),
                                             m_InternalFormat(internalFormat) {
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    if (immutable) {
        glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                        m_Width, m_Height,
                        dataFormat, dataType, data);
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat,
                     m_Width, m_Height, 0,
                     dataFormat, dataType, data);
    }
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_ID);
    m_ID = 0;
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Resize(const uint32_t&width, const uint32_t&height) {
    // /!\ Resize is only supported for mutable textures
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat,
                 width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    m_Width = width;
    m_Height = height;
}

void Texture2D::Attach(const uint32_t&unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::SetData(const void* data,
                        const GLenum&dataFormat,
                        const GLenum&dataType) {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                    m_Width, m_Height,
                    dataFormat, dataType, data);
}

void Texture2D::SetData(const uint32_t&width, const uint32_t&height,
                        const GLenum&internalFormat,
                        const GLenum&dataFormat,
                        const GLenum&dataType,
                        const void* data) {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                 width, height, 0,
                 dataFormat, dataType, data);

    m_Width = width;
    m_Height = height;
    m_InternalFormat = internalFormat;
}

void Texture2D::GetData(const GLenum&pixelFormat, const GLenum&pixelType, const uint32_t&size, void* pixels) const {
    glGetnTexImage(GL_TEXTURE_2D, 0, pixelFormat, pixelType, size, pixels);
}

void Texture2D::GetData(const GLenum&pixelFormat, const GLenum&pixelType, void* pixels) const {
    glGetTexImage(GL_TEXTURE_2D, 0, pixelFormat, pixelType, pixels);
}

void Texture2D::SetIntParameter(const GLenum&param, const int&value) const {
    glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture2D::SetFloatParameter(const GLenum&param, const float&value) const {
    glTexParameterf(GL_TEXTURE_2D, param, value);
}

void Texture2D::SetFloatParameter(const GLenum&param, const float* value) const {
    glTexParameterfv(GL_TEXTURE_2D, param, value);
}

void Texture2D::SetFilteringFlags(const GLenum&minFilter, const GLenum&magFilter) const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture2D::SetWrappingFlags(const GLenum&wrapS, const GLenum&wrapT) const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

// Todo: drive this directly through the filtering flags
void Texture2D::EnableMipmaps(const bool&enable) {
    if (enable && !m_Mipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    m_Mipmaps = enable;
}


void Texture2D::ClearUnit(const uint32_t&unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2DPtr Texture2D::Create(const uint32_t&width,
                               const uint32_t&height,
                               const GLenum&internalFormat,
                               const bool&immutable) {
    return std::make_shared<Texture2D>(width, height, internalFormat, immutable);
}

Texture2DPtr Texture2D::Create(const uint32_t&width,
                               const uint32_t&height,
                               const GLenum&internalFormat,
                               const GLenum&dataFormat,
                               const GLenum&dataType,
                               const void* data,
                               const bool&immutable) {
    return std::make_shared<Texture2D>(width,
                                       height,
                                       internalFormat,
                                       dataFormat,
                                       dataType,
                                       data,
                                       immutable);
}
