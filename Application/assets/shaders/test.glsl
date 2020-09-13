#type vertex

#version 450 core

layout (location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}

#type fragment

#version 450 core

layout (push_constant) uniform UniformBufferObject
{
	vec4 color;
} ubo;

layout(location = 0) out vec4 color;

layout (binding = 0) uniform sampler2D u_color;

void main()
{
	color = ubo.color;
	color += texture(u_color, vec2(0.0f, 0.0f));
}