#type vertex

#version 450 core

layout (location = 0) in vec4 position;

layout (binding = 1) uniform sampler2D u_color;

void main()
{
	gl_Position = position;
}

#type fragment

#version 450 core

layout (binding = 0) uniform UniformBufferObject
{
	vec4 color;
} ubo;

layout(location = 0) out vec4 color;

void main()
{
	color = ubo.color;
}