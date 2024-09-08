#type vertex
#version 460 core


layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}

#type tess_control
#version 460 core

// specify number of control points per patch output
// this value controls the size of the input and output arrays
layout (vertices=4) out;


// == Uniform ==

uniform int uTessLineCount = 64;// number of fibers
uniform int uTessSubdivisionCount = 4;// number of subdivisions per fiber

// == Outputs ==

patch out vec4 pPrevPoint;
patch out vec4 pNextPoint;

void main()
{
    // invocation zero controls tessellation levels for the entire patch
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = uTessLineCount;
        gl_TessLevelOuter[1] = uTessSubdivisionCount;

        pPrevPoint = gl_in[0].gl_Position;
        gl_out[gl_InvocationID].gl_Position = gl_in[1].gl_Position;
    }

    if (gl_InvocationID == 1)
    {
        gl_out[gl_InvocationID].gl_Position = gl_in[2].gl_Position;
        pNextPoint = gl_in[3].gl_Position;
    }
}

#type tess_evaluation
#version 460 core

layout (isolines, equal_spacing) in;

const float PI = 3.14159265;

uniform mat4 uModelMatrix;// the model matrix
uniform mat4 uViewMatrix;// the view matrix

uniform int uPlyCount = 3;

uniform float R_ply;// R_ply
uniform float Rmin;
uniform float Rmax;
uniform float theta;// polar angle of the fiber helix
uniform float s;// length of rotation
uniform float eN;// ellipse scaling factor along Normal
uniform float eB;// ellipse scaling factor along Bitangent

//uniform float R[gl_TessLevelOuter[1]]; // where  R[i] contain the distance between fiber i and ply center
uniform float R[4];

patch in vec4 pPrevPoint;
patch in vec4 pNextPoint;


out TS_OUT {
    int globalFiberIndex;
    vec3 yarnCenter;
    vec3 yarnNormal;
    vec3 yarnTangent;
    vec3 fiberNormal;
    float plyRotation;
} ts_out;


vec3 catmullCurve(vec3 pos1, vec3 pos2, vec3 pos3, vec3 pos4, float u) {
    float u2 = u * u;
    float u3 = u2 * u;

    float b0 = -u + 2.0 * u2 - u3;
    float b1 = 2.0 + -5.0 * u2 + 3.0 * u3;
    float b2 = u + 4.0 * u2 + - 3.0 * u3;
    float b3 = -1.0 * u2 + u3;
    return 0.5 * (b0 * pos1 + b1 * pos2 + b2 * pos3 + b3 * pos4);
}

vec3 catmullDerivative(vec3 pos1, vec3 pos2, vec3 pos3, vec3 pos4, float u) {
    float u2 = u * u;

    float b0 = -1.0 + 4.0 * u - 3.0 * u2;
    float b1 = -10.0 * u + 9.0 * u2;
    float b2 = 1.0 + 8.0 * u - 9.0 * u2;
    float b3 = -2.0 * u + 3.0 * u2;
    return 0.5 * (b0 * pos1 + b1 * pos2 + b2 * pos3 + b3 * pos4);
}

vec3 catmullSecondDerivative(vec3 pos1, vec3 pos2, vec3 pos3, vec3 pos4, float u) {
    float b0 = 4.0 - 6.0 * u;
    float b1 = -10.0 + 18.0 * u;
    float b2 = 8.0 - 18.0 * u;
    float b3 = -4.0 + 6.0 * u;
    return 0.5 * (b0 * pos1 + b1 * pos2 + b2 * pos3 + b3 * pos4);
}


float randomFloat(vec2 smple){
    return fract(sin(dot(smple, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    int fiberCount = int(gl_TessLevelOuter[0]);
    int fibersPerPly = int(gl_TessLevelOuter[0]) / uPlyCount;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    int fiberIndex = int(v * (fiberCount + 1));
    int plyIndex = fiberIndex % uPlyCount;

    vec3 cp1 = pPrevPoint.xyz;
    vec3 cp2 = gl_in[0].gl_Position.xyz;
    vec3 cp3 = gl_in[1].gl_Position.xyz;
    vec3 cp4 = pNextPoint.xyz;

    // Yarn center using a catmull rom interpolation of the control points
    vec3 yarnCenter = catmullCurve(cp1, cp2, cp3, cp4, u);

    vec3 N_yarn = vec3(0.0, 1.0, 0.0);
    vec3 T_yarn = normalize(catmullDerivative(cp1, cp2, cp3, cp4, u));
    vec3 B_yarn = normalize(cross(N_yarn, T_yarn));
    N_yarn = cross(B_yarn, T_yarn);

    // Computing the displacement from the yarn to the ply
    float globalU = gl_PrimitiveID + u;
    float thetaPly = 2 * PI * plyIndex / uPlyCount;
    vec3 displacement_ply = 0.5 * R_ply * (cos(thetaPly + globalU * theta) * N_yarn + (sin(thetaPly + globalU * theta) * B_yarn));

    // Going from the ply to the fiber, computing the fiber radius and rotation
    float thetaI = 2.0 * PI * fiberIndex / fibersPerPly;
    float Ri = fiberIndex < uPlyCount ? 0.0 : R[fiberIndex % 4];// First fiber of each ply is the core fiber
    float R_fiber = 0.5 * Ri * (Rmax + Rmin + (Rmax - Rmin) * cos(thetaI + s * globalU * theta));

    // Computing the displacement from the ply to the fiber
    vec3 N_ply = normalize(displacement_ply);
    vec3 B_ply = cross(T_yarn, N_ply);
    float rd = randomFloat(vec2(fiberIndex, plyIndex));// introduce for some random flyaway for now, it is not in the original paper
    vec3 displacement_fiber = R_fiber * (cos(thetaI + globalU * 2.0 * theta + rd) * N_ply * eN + sin(thetaI +  globalU * 2.0 * theta + rd) * B_ply * eB);

    // Outputs
    gl_Position = uViewMatrix * uModelMatrix * vec4(yarnCenter + displacement_ply + displacement_fiber, 1.0);
    ts_out.globalFiberIndex = fiberIndex;
    ts_out.yarnCenter  = vec3(uViewMatrix * uModelMatrix * vec4(yarnCenter, 1.0));
    ts_out.yarnNormal  = vec3(uViewMatrix * uModelMatrix * vec4(N_yarn, 0.0));
    ts_out.yarnTangent = vec3(uViewMatrix * uModelMatrix * vec4(T_yarn, 0.0));
    ts_out.fiberNormal = vec3(uViewMatrix * uModelMatrix * vec4(normalize(displacement_ply + displacement_fiber), 0.0));
    ts_out.plyRotation = thetaPly + globalU * theta;
}


#type geometry
#version 460 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;


// == Inputs ==

in TS_OUT
{
    int globalFiberIndex;
    vec3 yarnCenter;
    vec3 yarnNormal;
    vec3 yarnTangent;
    vec3 fiberNormal;
    float plyRotation;
} gs_in[];


// == Uniforms ==

uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform int uPlyCount = 3;

uniform vec3 uLightDirection;

// == Outputs ==

out GS_OUT
{
    vec3 position;
    vec3 normal;

    float distanceFromYarnCenter;

    vec2 selfShadowSample;
    float plyRotation;
} gs_out;

flat out int fiberIndex;


void main()
{
    float thickness = 0.003;

    fiberIndex = gs_in[0].globalFiberIndex;
    if (fiberIndex < uPlyCount)// core fiber determination
        thickness *= 20.0;

    vec3 pntA = gl_in[0].gl_Position.xyz;
    vec3 pntB = gl_in[1].gl_Position.xyz;
    vec3 fiberTangent = normalize(pntB - pntA);

    vec3 toCameraA = normalize(-pntA);
    vec3 frontFacingBitangentA = normalize(cross(toCameraA, fiberTangent));
    vec3 normalA     = gs_in[0].fiberNormal;
    vec3 yarnCenterA = gs_in[0].yarnCenter;

    vec3 toCameraB = normalize(-pntB);
    vec3 frontFacingBitangentB = normalize(cross(toCameraB, fiberTangent));
    vec3 normalB     = gs_in[1].fiberNormal;
    vec3 yarnCenterB = gs_in[1].yarnCenter;

    // Self shadows
    vec3 toLight = normalize(-uLightDirection);
    vec3 yarnTangentA = gs_in[0].yarnTangent;
    vec3 bitangentToLightA = -normalize(cross(yarnTangentA, toLight));
    vec3 normalToLightA = cross(bitangentToLightA, yarnTangentA);
    vec2 selfShadowSampleA = (transpose(mat3(normalToLightA, bitangentToLightA, yarnTangentA)) * (pntA - yarnCenterA)).xy;

    vec3 yarnTangentB = gs_in[1].yarnTangent;
    vec3 bitangentToLightB = -normalize(cross(yarnTangentB, toLight));
    vec3 normalToLightB = cross(bitangentToLightB, yarnTangentB);
    vec2 selfShadowSampleB = (transpose(mat3(normalToLightB, bitangentToLightB, yarnTangentB)) * (pntB - yarnCenterB)).xy;

    float plyRotationA = gs_in[0].plyRotation;
    float plyRotationB = gs_in[1].plyRotation;

    // CoreFiber: Move the vertices along the bitangent to create the thickness

    // Top left
    vec3 vertex = pntB - frontFacingBitangentA * thickness;
    gs_out.position = vertex;
    gs_out.normal = normalB;
    gs_out.distanceFromYarnCenter = distance(vertex, yarnCenterB);
    gs_out.selfShadowSample = selfShadowSampleB;
    gs_out.plyRotation = plyRotationB;
    gl_Position = uProjMatrix * vec4(vertex, 1.0);
    EmitVertex();

    // Bottom left
    vertex = pntA - frontFacingBitangentA * thickness;
    gs_out.position = vertex;
    gs_out.normal = normalA;
    gs_out.distanceFromYarnCenter = distance(vertex, yarnCenterA);
    gs_out.selfShadowSample = selfShadowSampleA;
    gs_out.plyRotation = plyRotationA;
    gl_Position = uProjMatrix * vec4(vertex, 1.0);
    EmitVertex();

    // Top right
    vertex = pntB + frontFacingBitangentA * thickness;
    gs_out.position = vertex;
    gs_out.normal = normalB;
    gs_out.distanceFromYarnCenter = distance(vertex, yarnCenterB);
    gs_out.selfShadowSample = selfShadowSampleB;
    gs_out.plyRotation = plyRotationB;
    gl_Position = uProjMatrix * vec4(vertex, 1.0);
    EmitVertex();

    // Bottom right
    vertex = pntA + frontFacingBitangentA * thickness;
    gs_out.position = vertex;
    gs_out.normal = normalA;
    gs_out.distanceFromYarnCenter = distance(vertex, yarnCenterA);
    gs_out.selfShadowSample = selfShadowSampleA;
    gs_out.plyRotation = plyRotationA;
    gl_Position = uProjMatrix * vec4(vertex, 1.0);
    EmitVertex();
}


#type fragment
#version 460 core


// == Inputs ==

in GS_OUT
{
    vec3 position;
    vec3 normal;
    float distanceFromYarnCenter;
    vec2 selfShadowSample;
    float plyRotation;
} fs_in;

flat in int fiberIndex;
// == Uniforms ==

uniform mat4 uViewMatrix;

uniform int uPlyCount = 3;
uniform float R_ply;
uniform float Rmin;
uniform bool uUseAmbientOcclusion;

// Shadow mapping
uniform sampler2D uShadowMap;
uniform float uShadowIntensity = 0.7;
uniform mat4 uViewToLightMatrix;
uniform bool uReceiveShadows = true;
uniform bool uSmoothShadows = true;

// Self shadows
uniform sampler3D uSelfShadowsTexture;
uniform float uSelfShadowsIntensity = 1.0;
uniform float uSelfShadowRotation = 0.0;

uniform vec3 fiberColor=vec3(0.8);

// == Outputs ==

out vec4 FragColor;


vec3 sampleAlbedo(vec2 texCoord)
{
    return vec3(0.8);
    // return uUseAlbedoTexture ? texture(uAlbedoTexture, texCoord).rgb : uAlbedoColor;
}

// Simplified ambient occlusion
float sampleAmbientOcclusion()
{
    return uUseAmbientOcclusion ? min(1.0, fs_in.distanceFromYarnCenter / R_ply) : 1.0;
}

float sampleShadows(vec4 lightSpacePosition)
{
    if (!uReceiveShadows)
    return 0.0;

    vec3 lightProjectedPos = lightSpacePosition.xyz / lightSpacePosition.w;
    lightProjectedPos = lightProjectedPos * 0.5 + 0.5;
    float fragmentDepth = lightProjectedPos.z;

    if (fragmentDepth > 1.0)
    return 0.0;

    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    float shadow = 0.0;

    // 泊松分布采样点
    vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
    );

    #ifdef USE_PCF
    // 使用泊松分布进行 PCF 采样
    float radius = 20.0; // 采样半径
    for (int i = 0; i < 16; ++i)
    {
        vec2 offset = poissonDisk[i] * texelSize * radius;
        float shadowDepth = texture(uShadowMap, lightProjectedPos.xy + offset).r;
        shadow += fragmentDepth > shadowDepth ? uShadowIntensity : 0.0;
    }
    shadow /= 16.0; // 归一化阴影值
    return shadow;
    #endif

    #ifdef USE_PCSS
    // 使用泊松分布进行 PCSS 采样
    int samples = 16; // 泊松盘中的采样点数量
    float blockerCount = 0.0;
    float avgBlockerDepth = 0.0;

    // 查找遮挡者
    for (int i = 0; i < samples; ++i)
    {
        vec2 offset = poissonDisk[i] * texelSize;
        float shadowDepth = texture(uShadowMap, lightProjectedPos.xy + offset).r;
        if (shadowDepth < fragmentDepth)
        {
            avgBlockerDepth += shadowDepth;
            blockerCount += 1.0;
        }
    }

    if (blockerCount > 0.0)
    {
        avgBlockerDepth /= blockerCount;

        // 估算半影大小
        float penumbraSize = (fragmentDepth - avgBlockerDepth) / avgBlockerDepth;

        // 使用动态调整的采样区域进行 PCF
        shadow = 0.0;
        float filterRadius = penumbraSize * 2.0; // 根据实际需要调整因子
        for (int i = 0; i < samples; ++i)
        {
            vec2 offset = poissonDisk[i] * texelSize * filterRadius;
            float shadowDepth = texture(uShadowMap, lightProjectedPos.xy + offset).r;
            shadow += fragmentDepth > shadowDepth ? uShadowIntensity : 0.0;
        }
        shadow /= samples; // 归一化阴影值
        return shadow;
    }
    #endif

    // 基本阴影映射
    float shadowDepth = texture(uShadowMap, lightProjectedPos.xy).r;
    return fragmentDepth > shadowDepth ? uShadowIntensity : 0.0;
}

float sampleSelfShadows(vec2 selfShadowSample)
{
    float scaleFactor = (R_ply + Rmin) * 1.5;
    float selfShadowDensity = texture(uSelfShadowsTexture, vec3((selfShadowSample / scaleFactor) * 0.5 + 0.5, uSelfShadowRotation)).r;
    return max(0.0, 1.0 - selfShadowDensity);
}



void main()
{
    vec3 viewSpaceLightDir = vec3(uViewMatrix * vec4(normalize(vec3(0.0, 1.0, 1.0)), 0.0));
    vec3 viewSpaceNormal = normalize(fs_in.normal);

    //vec3 albedo = sampleAlbedo(vec2(0.0, 0.0));
    vec3 albedo = fiberColor;
    float ambientOcclusion = min(1.0, max(sampleAmbientOcclusion(), 0.0) + 0.2);
    float shadowMask = 1.0 - sampleShadows(uViewToLightMatrix * vec4(fs_in.position, 1.0));
    float selfShadows = sampleSelfShadows(fs_in.selfShadowSample);
    // vec3 color = vec3(shadowMask);
    vec3 color = selfShadows * shadowMask * ambientOcclusion * albedo;
    // vec3 color = shadowMask * ambientOcclusion * albedo * vec3(max(0.0, dot(viewSpaceNormal, viewSpaceLightDir)));

    FragColor = vec4(vec3(color), 1.0);

}
