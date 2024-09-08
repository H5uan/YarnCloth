#version 460 core

uniform vec4 u_Color = vec4(0.5, 0.5, 0.5, 1.0);

in VertexData {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} inVertex;

out vec4 fragColor;

void main(){
    fragColor = u_Color;
}