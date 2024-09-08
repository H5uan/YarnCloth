#version 460 core

in vec2 viewSpaceScreenCoords;

uniform sampler3D u_InputTexture;
uniform float u_Depth = 0.0;

out vec4 fragColor;

void main(){
    fragColor = texture(u_InputTexture, vec3(viewSpaceScreenCoords, u_Depth));
}