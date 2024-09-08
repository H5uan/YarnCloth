#include "OpenGLRenderingAPI.h"

#include <glad/glad.h>

static GLenum StencilFuncToOpenGLStencilFunc(StencilFunc func) {
    switch (func) {
        case StencilFunc::ALWAYS:
            return GL_ALWAYS;
        case StencilFunc::NOTEQUAL:
            return GL_NOTEQUAL;
    }
    GLCORE_ASSERT(false, "Unknow StencilFunc");
    return -1;
}

static GLenum StencilOpToOpenGLStencilOp(StencilOp op) {
    switch (op) {
        case StencilOp::KEEP:
            return GL_KEEP;
        case StencilOp::INCREMENT:
            return GL_INCR_WRAP;
        case StencilOp::DECREMENT:
            return GL_DECR_WRAP;
    }
    GLCORE_ASSERT(false, "Unknown StencilOp");
    return -1;
}

static GLenum DepthcomparisonToOpenGLDepthcomparison(DepthComp comp) {
    switch (comp) {
        case DepthComp::EQUAL:
            return GL_EQUAL;
        case DepthComp::LEQUAL:
            return GL_LEQUAL;
        case DepthComp::LESS:
            return GL_LESS;
    }
    GLCORE_ASSERT(false, "Unknown DepthComp");
    return -1;
}


void OpenGLRenderingAPI::Init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    //glStencilMask(0xFF);
    glStencilMask(0x00); // forbidden to write in stencil

    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void OpenGLRenderingAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void OpenGLRenderingAPI::SetClearColor(const glm::vec4&color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderingAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderingAPI::DrawIndexed(const Ref<VertexArray>&vertexArray, uint32_t indexCount) {
    vertexArray->Bind();
    uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderingAPI::DrawLines(const Ref<VertexArray>&vertexArray, uint32_t vertexCount) {
    vertexArray->Bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}

void OpenGLRenderingAPI::SetLineWidth(float width) {
    glLineWidth(width);
}

void OpenGLRenderingAPI::ClearStencil() {
    glClear(GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderingAPI::DepthMask(bool maskFlag) {
    if (maskFlag)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);
}

void OpenGLRenderingAPI::DepthTest(bool enable) {
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderingAPI::Blend(int32_t Bit) {
    if (Bit) {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
    }
    else {
        glDisable(GL_BLEND);
    }
}

void OpenGLRenderingAPI::StencilTest(int32_t Bit) {
    if (Bit)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
}

void OpenGLRenderingAPI::Cull(int32_t Bit) {
    if (Bit)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

void OpenGLRenderingAPI::CullFrontOrBack(bool bFront) {
    if (bFront)
        glCullFace(GL_FRONT);
    else
        glCullFace(GL_BACK);
}

void OpenGLRenderingAPI::SetStencilFunc(StencilFunc stencilFunc, int32_t ref, int32_t mask) {
    glStencilFunc(StencilFuncToOpenGLStencilFunc(stencilFunc), ref, mask);
}

void OpenGLRenderingAPI::StencilMask(uint32_t mask) {
    // glStencilMask(0x00): forbidden to write in stencil
    // glStencilMask(0xFF): allow to write in stencil
    glStencilMask(mask);
}

void OpenGLRenderingAPI::SetFrontOrBackStencilOp(int32_t FrontOrBack, StencilOp stencilFail, StencilOp depthFail,
                                                 StencilOp depthSuccess) {
    if (FrontOrBack) {
        glStencilOpSeparate(GL_FRONT, StencilOpToOpenGLStencilOp(stencilFail),
                            StencilOpToOpenGLStencilOp(depthFail),
                            StencilOpToOpenGLStencilOp(depthSuccess));
    }
    else {
        glStencilOpSeparate(GL_BACK, StencilOpToOpenGLStencilOp(stencilFail),
                            StencilOpToOpenGLStencilOp(depthFail),
                            StencilOpToOpenGLStencilOp(depthSuccess));
    }
}

void OpenGLRenderingAPI::DepthFunc(DepthComp comp) {
    glDepthFunc(DepthcomparisonToOpenGLDepthcomparison(comp));
}

void OpenGLRenderingAPI::BindTexture(int32_t slot, uint32_t textureID) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

int OpenGLRenderingAPI::GetDrawFrameBuffer() {
    int framebufferID = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebufferID);
    return framebufferID;
}

void OpenGLRenderingAPI::BindFrameBuffer(uint32_t framebufferID) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
}
