#type vertex

#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TextureCoordinates;

layout (location = 0) out vec2 textureCoordinates;

void main()
{
	gl_Position = vec4(a_Position, 1.0);
	textureCoordinates = a_TextureCoordinates;
}

#type fragment

#version 450 core

layout (location = 0) in vec2 textureCoordinates;
layout (binding = 0) uniform sampler2D u_texture;

layout(location = 0) out vec4 color;


void main()
{
	color = texture(u_texture, textureCoordinates);
}