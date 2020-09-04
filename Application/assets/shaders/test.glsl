#type vertex

#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

layout (binding = 0) uniform UniformBufferObject
{
	mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 2) out vec4 data;

void main()
{
	vec4 pos = ubo.model * position;
	data = pos;
	gl_Position = position;
}

#type fragment

#version 450 core

layout (location = 2) in vec4 data;

layout(location = 0) out vec4 color;


void main()
{
	color = vec4(0.8f, 0.2f, 0.1f, 1.0f);
}