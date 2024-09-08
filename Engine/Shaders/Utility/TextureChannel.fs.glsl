#version 460 core

in vec2 viewSpaceScreenCoords;

uniform sampler2D u_InputTexture;
uniform int u_Channel = 0;

out vec4 fragColor;

void main(){
    fragColor = vec4(vec3(texture(u_InputTexture, viewSpaceScreenCoords)[u_Channel]), 1.0);
}
