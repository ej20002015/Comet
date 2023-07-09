#type vertex

#version 460 core

// ATTRIBUTES

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_textureCoordinates;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;

// UNIFORM BUFFER OBJECTS

layout (std140, binding = 0) uniform Camera
{
	mat4 u_viewProjectionMatrix;
} u_camera;

// UNIFORMS

layout (push_constant) uniform v_PushConstants
{
	mat4 u_transform;
} u_v_pushConstants;

// OUTPUTS

struct VertexOutput
{
    vec3 worldPosition;
    vec3 normal;
    vec2 textureCoordinates;
    mat3 TBN;
};

layout (location = 0) out VertexOutput v_vertexOutput;

// FUNCTIONS

void main()
{
    v_vertexOutput.worldPosition = vec3(u_v_pushConstants.u_transform * vec4(a_position, 1.0f));
    v_vertexOutput.normal = normalize(vec3(transpose(inverse(u_v_pushConstants.u_transform)) * vec4(a_normal, 0.0f)));
    v_vertexOutput.textureCoordinates = a_textureCoordinates;

    vec3 normalTransformed = normalize(vec3(u_v_pushConstants.u_transform * vec4(a_normal, 0.0f)));
    vec3 tangentTransformed = normalize(vec3(u_v_pushConstants.u_transform * vec4(a_tangent, 0.0f)));
    vec3 bitangentTransformed = normalize(vec3(u_v_pushConstants.u_transform * vec4(a_bitangent, 0.0f)));
    v_vertexOutput.TBN = mat3(tangentTransformed, bitangentTransformed, normalTransformed);

    gl_Position = u_camera.u_viewProjectionMatrix * u_v_pushConstants.u_transform * vec4(a_position, 1.0f);
}

#type fragment

#version 460 core

// INPUTS FROM VERTEX SHADER

struct VertexOutput
{
    vec3 worldPosition;
    vec3 normal;
    vec2 textureCoordinates;
    mat3 TBN;
};

layout (location = 0) in VertexOutput vertexInput;

// UNIFORM BUFFER OBJECTS

// Lights

struct PointLight
{
    vec3 worldPosition;
    vec3 lightColor;
    float luminousPower;
    float lightRadius;
};

const uint MAX_NUM_OF_POINT_LIGHTS = 128;

// Scene data

layout (std140, binding = 1) uniform SceneData
{
    vec3 u_viewPosition;
	PointLight u_pointLights[MAX_NUM_OF_POINT_LIGHTS];
    uint u_pointLightCount;
} u_sceneData;

// UNIFORMS

struct Material
{
    vec4 diffuseColor;
    vec3 specularColor;
    float shininess;

    //bool useNormalMap;
};

layout (push_constant) uniform f_PushConstants
{
	layout (offset = 64) int u_entityID;
    Material u_material;
} u_f_pushConstants;


// OUTPUTS

layout (location = 0) out vec4 o_fragColor;
layout (location = 1) out int o_entityID;

// CONSTANTS

const vec3 LIGHT_AMBIENT = vec3(0.02f);

// GLOBAL DATA

vec3 g_normal;
vec3 g_viewDirection;
vec3 g_diffuseMaterialValue;
vec3 g_specularMaterialValue;

// FUNCTIONS

vec3 getNormalisedSurfaceNormal()
{
    /*
    if (u_f_pushConstants.u_material.useNormalMap)
    {
        vec4 sampleFromNormalMap = texture(u_f_pushConstants.u_material.normalMap, vertexInput.textureCoordinates);
        vec3 sampledNormal = (sampleFromNormalMap.rgb * 2.0f) - 1.0f;
        vec3 sampledNormalInWorldSpace = vertexInput.TBN * sampledNormal;
        return normalize(sampledNormalInWorldSpace);
    }
    else
        return normalize(vertexInput.normal);
    */

    return normalize(vertexInput.normal);
}

float calculatePointLightAttenuationFactor(float lightDistance, float lightRadius)
{
    const float lightSize = 0.01f;
    const uint n = 4;

    // Restrict the minimum value of the denominator to 0.01 * 0.01 to avoid the value
    // exploding or having divide by zero errors
    float inverseSquaredDistance = 1.0f / pow(max(lightDistance, lightSize), 2.0f);

    // Use a windowing function to cutoff the attenuation value to 0 at large distances
    
    float lightDistanceNOverLightRadiusN = 1.0f - pow(lightDistance / lightRadius, n);
    float windowingFunctionValue = pow(clamp(lightDistanceNOverLightRadiusN, 0.0f, 1.0f), 2.0f);

    return min(inverseSquaredDistance * windowingFunctionValue, 1.0f);
}

vec3 calculateDiffuseContribution(vec3 lightDirection, vec3 attenuatedLightDiffuseComponent)
{
    float lightIncidentDiffuseFactor = max(dot(lightDirection, g_normal), 0.0f);
    return (g_diffuseMaterialValue * lightIncidentDiffuseFactor) * attenuatedLightDiffuseComponent;
}

vec3 calculateSpecularContribution(vec3 lightDirection, vec3 attenuatedLightSpecularComponent)
{
    vec3 halfVector = normalize(g_viewDirection + lightDirection);
    float lightReflectedSpecularFactor = pow(max(dot(halfVector, g_normal), 0.0f), u_f_pushConstants.u_material.shininess);
    return (g_specularMaterialValue * lightReflectedSpecularFactor) * attenuatedLightSpecularComponent;
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
    g_normal = getNormalisedSurfaceNormal();
    g_viewDirection = normalize(u_sceneData.u_viewPosition - vertexInput.worldPosition);

    //vec4 diffuseMaterialValueWithAlpha = texture(u_f_pushConstants.u_material.diffuseMap, vertexInput.textureCoordinates) * u_f_pushConstants.u_material.diffuseColor;
    //float alpha = diffuseMaterialValueWithAlpha.a;
    vec4 diffuseMaterialValueWithAlpha = u_f_pushConstants.u_material.diffuseColor;
    float alpha = 1.0f;
    
    g_diffuseMaterialValue = diffuseMaterialValueWithAlpha.rgb;
    //g_specularMaterialValue = texture(u_f_pushConstants.u_material.specularMap, vertexInput.textureCoordinates).rgb * u_f_pushConstants.u_material.specularColor;
    g_specularMaterialValue = u_f_pushConstants.u_material.specularColor;

    vec3 color;

    // Calculate ambient contribution

    color = g_diffuseMaterialValue * LIGHT_AMBIENT;

    // Calculate diffuse and specular contribution for each light
    
    for (uint i = 0; i < u_sceneData.u_pointLightCount; i++)
    {
        PointLight pointLight =u_sceneData.u_pointLights[i];
        vec3 lightDirection = normalize(pointLight.worldPosition - vertexInput.worldPosition);

        float lightDistance = length(pointLight.worldPosition - vertexInput.worldPosition);
        float attenuationFactor = calculatePointLightAttenuationFactor(lightDistance, pointLight.lightRadius);

        color += calculateDiffuseContribution(lightDirection, pointLight.lightColor * attenuationFactor);
        color += calculateSpecularContribution(lightDirection, pointLight.lightColor * attenuationFactor);
    }
    
    o_fragColor = vec4(gammaCorrectColor(color), alpha);
    //o_fragColor = vec4(g_diffuseMaterialValue, 1.0f);
    o_entityID = u_f_pushConstants.u_entityID;
}