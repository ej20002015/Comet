#type vertex

#version 450

layout (location = 0) in vec3  a_position;
layout (location = 1) in vec4  a_color;
layout (location = 2) in vec2  a_textureCoordinates;
layout (location = 3) in uint  a_textureIndex;
layout (location = 4) in float a_tilingFactor;
layout (location = 5) in int   a_entityID;

layout (binding = 0) uniform Camera
{
	mat4 u_viewProjectionMatrix;
} u_camera;

struct VertexOutput
{
	vec4 v_color;
	vec2 v_textureCoordinates;
	float v_tilingFactor;
};

layout (location = 0) out VertexOutput v_vertexOutput;
layout (location = 3) flat out uint v_textureIndex;
layout (location = 4) flat out int v_entityID;

void main()
{
	v_vertexOutput.v_color = a_color;
	v_vertexOutput.v_textureCoordinates = a_textureCoordinates;
	v_vertexOutput.v_tilingFactor = a_tilingFactor;
	v_textureIndex = a_textureIndex;
	v_entityID = a_entityID;
	gl_Position = u_camera.u_viewProjectionMatrix * vec4(a_position, 1.0);
}

#type fragment

#version 450 core

struct VertexOutput
{
	vec4 v_color;
	vec2 v_textureCoordinates;
	float v_tilingFactor;
};

layout (location = 0) in VertexOutput v_vertexInput;
layout (location = 3) flat in uint v_textureIndex;
layout (location = 4) flat in int v_entityID;

layout (binding = 0) uniform sampler2D u_textures[32];

layout (location = 0) out vec4 color;
layout (location = 1) out int entityID;

void main()
{
	//Apparently the following line does not work with AMD drivers so moved to a switch case statement instead
	//color = texture(u_textures[v_textureIndex], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color;

	switch (v_textureIndex)
	{
		case 0:  color = texture(u_textures[ 0], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 1:  color = texture(u_textures[ 1], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 2:  color = texture(u_textures[ 2], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 3:  color = texture(u_textures[ 3], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 4:  color = texture(u_textures[ 4], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 5:  color = texture(u_textures[ 5], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 6:  color = texture(u_textures[ 6], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 7:  color = texture(u_textures[ 7], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 8:  color = texture(u_textures[ 8], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 9:  color = texture(u_textures[ 9], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 10: color = texture(u_textures[10], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 11: color = texture(u_textures[11], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 12: color = texture(u_textures[12], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 13: color = texture(u_textures[13], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 14: color = texture(u_textures[14], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 15: color = texture(u_textures[15], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 16: color = texture(u_textures[16], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 17: color = texture(u_textures[17], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 18: color = texture(u_textures[18], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 19: color = texture(u_textures[19], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 20: color = texture(u_textures[20], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 21: color = texture(u_textures[21], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 22: color = texture(u_textures[22], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 23: color = texture(u_textures[23], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 24: color = texture(u_textures[24], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 25: color = texture(u_textures[25], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 26: color = texture(u_textures[26], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 27: color = texture(u_textures[27], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 28: color = texture(u_textures[28], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 29: color = texture(u_textures[29], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 30: color = texture(u_textures[30], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
		case 31: color = texture(u_textures[31], v_vertexInput.v_textureCoordinates * v_vertexInput.v_tilingFactor) * v_vertexInput.v_color; break;
	}

	entityID = v_entityID;
}