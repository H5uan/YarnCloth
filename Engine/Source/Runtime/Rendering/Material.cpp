#include "Material.h"
/***
void Material::Initialize() const {
    unsigned char data[4];

    for (size_t i = 0; i < 3; i++) {
        data[i] = static_cast<unsigned char>(metallic * 255.0f);
    }
    data[3] = static_cast<unsigned char>(255.0f);
    metallicRGBA->SetData(data, sizeof(unsigned char) * 4);

    for (size_t i = 0; i < 3; i++) {
        data[i] = static_cast<unsigned char>(roughness * 255.0f);
    }
    data[3] = static_cast<unsigned char>(255.0f);
    roughnessRGBA->SetData(data, sizeof(unsigned char) * 4);
}
***/