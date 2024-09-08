#type vertex
#version 460 core

out vec2 vScreenCoords;

// Drawing a single triangle that covers all the screen with clean screen coords for post processing
void main(void) {
    vScreenCoords = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    gl_Position = vec4(vScreenCoords * 2.0 + -1.0, 0.0, 1.0);
}

#type fragment
#version 460 core

in vec2 vScreenCoords;

uniform sampler2D uDensityTexture;

uniform float uPlyRadius = 1.75;
uniform float uFiberRadius = 1.0;


out vec4 FragColor;


float sampleDensity(float dist, vec2 texCoords)
{
    float density = texture(uDensityTexture, texCoords).x;
    return 1.0 - exp(-dist * density);
}


void main()
{
    int screenWidth = textureSize(uDensityTexture, 0).x;
    float stepSize = 1.0 / screenWidth;
    int currentFragmentX = int(vScreenCoords.x * screenWidth);

    // The size of the texture is normalized, we need to take that into consideration
    // when computing the distance travelled
    float scaleFactor = (uPlyRadius + uFiberRadius) * 1.5;

    float absorbedLight = 0.0;
    for (int i = 0; i < currentFragmentX; i++)
    {
        vec2 texCoords = vec2(float(i) * stepSize, vScreenCoords.y);
        absorbedLight += sampleDensity(stepSize / scaleFactor, texCoords);
    }

    FragColor = vec4(vec3(absorbedLight), 1.0);
    // FragColor = vec4(vec3(texture(uDensityTexture, vScreenCoords).x), 1.0);
}
