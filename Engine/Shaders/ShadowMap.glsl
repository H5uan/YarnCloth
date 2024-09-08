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

uniform int uTessLineCount = 64;
uniform int uTessSubdivisionCount = 4;

// == Outputs ==

patch out vec4 pPrevPoint;
patch out vec4 pNextPoint;

void main()
{
    // invocation zero controls tessellation levels for the entire patch
    if (gl_InvocationID == 0)
    {
        // This is for all patch, not vertex specifically
        gl_TessLevelOuter[0] = uTessLineCount;// fibers
        gl_TessLevelOuter[1] = uTessSubdivisionCount;// ply

        pPrevPoint = gl_in[0].gl_Position;
        gl_out[gl_InvocationID].gl_Position = gl_in[1].gl_Position;
        pNextPoint = gl_in[3].gl_Position;


    }

    if (gl_InvocationID == 1)
    {
        gl_out[gl_InvocationID].gl_Position = gl_in[2].gl_Position;

    }
}

#type tess_evaluation
// tessellation evaluation shader
#version 460 core

// == Inputs ==

layout (isolines, equal_spacing) in;

// end point for curves
patch in vec4 pPrevPoint;
patch in vec4 pNextPoint;


// == Uniforms

uniform mat4 uModelMatrix;// the model matrix
uniform mat4 uViewMatrix;// the view matrix


// == Outputs ==

out TS_OUT {
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
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

// for shadow map, we only need simple curve for yarn.
void main() {
    float u = gl_TessCoord.x;

    vec3 cp1 = pPrevPoint.xyz;
    vec3 cp2 = gl_in[0].gl_Position.xyz;
    vec3 cp3 = gl_in[1].gl_Position.xyz;
    vec3 cp4 = pNextPoint.xyz;

    // Yarn center using a catmull rom interpolation of the control points
    vec3 curvePoint = catmullCurve(cp1, cp2, cp3, cp4, u);

    vec3 normal = vec3(0.0, 1.0, 0.0);
    vec3 tangent = normalize(catmullDerivative(cp1, cp2, cp3, cp4, u));
    vec3 bitangent = normalize(cross(normal, tangent));
    normal = normalize(cross(bitangent, tangent));

    // Outputs
    gl_Position      =      uViewMatrix * uModelMatrix * vec4(curvePoint, 1.0);
    ts_out.normal    = vec3(uViewMatrix * uModelMatrix * vec4(normal, 0.0));
    ts_out.tangent   = vec3(uViewMatrix * uModelMatrix * vec4(tangent, 0.0));
    ts_out.bitangent = vec3(uViewMatrix * uModelMatrix * vec4(bitangent, 0.0));
}


#type geometry
#version 410 core
layout (lines) in;
layout (triangle_strip, max_vertices = 36) out;


// == Inputs ==

in TS_OUT
{
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
} gs_in[];


// == Uniforms ==

uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform float uThickness = 0.01;

// == Outputs ==

out GS_OUT
{
    vec3 position;
    vec3 normal;
    float distanceFromYarnCenter;
} gs_out;


const float PI = 3.14159265;
const int maxTubeDivision = 8;

// simply generate as tube to compute shader
void main()
{
    vec3 pntA = gl_in[0].gl_Position.xyz;
    vec3 pntB = gl_in[1].gl_Position.xyz;

    vec3 tangentA   = gs_in[0].tangent;
    vec3 tangentB   = gs_in[1].tangent;
    vec3 normalA    = gs_in[0].normal;
    vec3 normalB    = gs_in[1].normal;
    vec3 bitangentA = gs_in[0].bitangent;
    vec3 bitangentB = gs_in[1].bitangent;

    float theta;
    vec3 displacement;
    vec3 vertex;
    for (int i = 0; i < maxTubeDivision ; i++)
    {
        theta = 2.0 * PI * i / maxTubeDivision; // initial angle for ply

        displacement = cos(theta) * normalA + sin(theta) * bitangentA;
        vertex = pntA + displacement * uThickness;
        gs_out.position = vertex;
        gs_out.normal = normalize(displacement);
        gl_Position = uProjMatrix * vec4(vertex, 1.0);
        EmitVertex();

        displacement = cos(theta) * normalB + sin(theta) * bitangentB;
        vertex = pntB + displacement * uThickness;
        gs_out.position = vertex;
        gs_out.normal = normalize(displacement);
        gl_Position = uProjMatrix * vec4(vertex, 1.0);
        EmitVertex();
    }
}

#type fragment
#version 460 core

void main() {
    // This shader is only for calculating depth.
}