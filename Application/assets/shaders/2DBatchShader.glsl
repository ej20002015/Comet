#type vertex

#version 450

layout (location = 0) in vec3  a_position;
layout (location = 1) in vec4  a_color;
layout (location = 2) in vec2  a_textureCoordinate;
layout (location = 3) in float a_textureIndex;
layout (location = 4) in float a_tilingFactor;

layout (binding = 0) uniform Camera
{
	mat4 u_viewProjectionMatrix;
} u_camera;

struct VertexOutput
{
	vec4 v_color;
	vec2 v_textureCoordinate;
	float v_textureIndex;
	float v_tilingFactor;
};

layout (location = 0) out VertexOutput v_vertexOutput;

void main()
{
	v_vertexOutput.v_color = a_color;
	v_vertexOutput.v_textureCoordinate = a_textureCoordinate;
	v_vertexOutput.v_textureIndex = a_textureIndex;
	v_vertexOutput.v_tilingFactor = a_tilingFactor;
	gl_Position = u_camera.u_viewProjectionMatrix * vec4(a_position, 1.0);
}

#type fragment

#version 450 core

struct VertexOutput
{
	vec4 v_color;
	vec2 v_textureCoordinate;
	float v_textureIndex;
	float v_tilingFactor;
};

layout (location = 0) in VertexOutput v_vertexInput;

layout (binding = 0) uniform sampler2D u_textures[32];

layout (location = 0) out vec4 color;

void main()
{
	color = texture(u_textures[int(v_vertexInput.v_textureIndex)], v_vertexInput.v_textureCoordinate * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color;
}