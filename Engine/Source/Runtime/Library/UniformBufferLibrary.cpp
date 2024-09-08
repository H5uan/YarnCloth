
#include "Library/UniformBufferLibrary.h"


Library<UniformBuffer>::Library() {
    Ref<UniformBuffer> CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
    Add("CameraUniform", CameraUniformBuffer);
}

Ref<UniformBuffer> Library<UniformBuffer>::GetCameraUniformBuffer() {
    return mLibrary["CameraUniform"];
}

