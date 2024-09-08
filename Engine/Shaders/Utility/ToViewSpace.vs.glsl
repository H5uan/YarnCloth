#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_ModelMatrix = mat4(1.0);
uniform mat4 u_ViewMatrix;
uniform mat4 u_PorjMatrix;

out VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} outVertex;

void main()
{
    vec4 position = u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0);

    outVertex.viewSpacePosition = position.xyz;
    outVertex.normal = mat3(transpose(inverse(uModelMatrix))) * aNormal;
    outVertex.texCoord = aTexCoord;

    gl_Position = position;
}