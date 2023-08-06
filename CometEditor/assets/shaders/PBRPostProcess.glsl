#type vertex

#version 460 core

// ATTRIBUTES

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_textureCoordinates;

// OUTPUTS

struct VertexOutput
{
    vec2 textureCoordinates;
};

layout (location = 0) out VertexOutput v_vertexOutput;

// FUNCTIONS

void main()
{
    v_vertexOutput.textureCoordinates = a_textureCoordinates;
    gl_Position = vec4(a_position, 0.0f, 1.0f);
}

#type fragment

#version 460 core

// INPUTS FROM VERTEX SHADER

struct VertexOutput
{
    vec2 textureCoordinates;
};

layout (location = 0) in VertexOutput v_vertexInput;

// UNIFORMS

layout (binding = 0) uniform sampler2D u_imageTexture;
layout (binding = 1) uniform isampler2D u_entityTexture;

layout (push_constant) uniform u_PushConstants
{
	float u_exposure;
} u_f_pushConstants;

// OUTPUTS

layout (location = 0) out vec4 o_fragColor;
layout (location = 1) out int o_entityID;

// FUNCTIONS

/*
ACES Tone Mapping

Curve adapted from: https://github.com/TheRealMJP/BakingLab/blob/master/BakingLab/ACES.hlsl
*/
vec3 applyToneMapping(vec3 color)
{
    mat3 inputMatrix = mat3
    (
         0.59719f,  0.07600f,  0.02840f,
         0.35458f,  0.90834f,  0.13383f,
         0.04823f,  0.01566f,  0.83777f
    );

    mat3 outputMatrix = mat3
    (
         1.60475f, -0.10208f, -0.00327f,
        -0.53108f,  1.10813f, -0.07276f,
        -0.07367f, -0.00605f,  1.07602f
    );

    color = inputMatrix * color;
    vec3 a = color * (color + 0.0245786f) - 0.000090537f;
    vec3 b = color * (0.983729f * color + 0.4329510f) + 0.238081f;
    color = a / b;

    return clamp(outputMatrix * color, 0.0f, 1.0f);
}

vec3 gammaCorrectColor(vec3 color)
{
    vec3 SRGBEncodedHigher = (1.055f * pow(color, vec3(1.0f / 2.4f))) - 0.055f;
    vec3 SRGBEncodedLower = 12.92f * color;
    float rSRGBEncoded = (color.r > 0.0031308f) ? SRGBEncodedHigher.r : SRGBEncodedLower.r;
    float gSRGBEncoded = (color.g > 0.0031308f) ? SRGBEncodedHigher.g : SRGBEncodedLower.g;
    float bSRGBEncoded = (color.b > 0.0031308f) ? SRGBEncodedHigher.b : SRGBEncodedLower.b;
    return vec3(rSRGBEncoded, gSRGBEncoded, bSRGBEncoded);
}

void main()
{
    vec4 inputColor = texture(u_imageTexture, v_vertexInput.textureCoordinates);
    float alpha = inputColor.a;

    // Apply exposure, tone mapping and gamma correction

    inputColor *= u_f_pushConstants.u_exposure;

    o_fragColor = vec4(gammaCorrectColor(applyToneMapping(inputColor.rgb)), alpha);
    o_entityID = texture(u_entityTexture, v_vertexInput.textureCoordinates).r;
}