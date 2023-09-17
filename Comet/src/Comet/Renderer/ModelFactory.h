#pragma once
#include "CometPCH.h"

#include "Model.h"

namespace Comet
{

class ModelFactory
{
public:

	enum class MeshType
	{
		CUBE = 0,
		SPHERE
	};

	static constexpr uint32_t MESH_TYPE_END = static_cast<uint32_t>(MeshType::SPHERE) + 1;
	static constexpr const char* const MESH_TYPE_STRS[MESH_TYPE_END] = {"CUBE", "SPHERE"};

	static constexpr uint32_t DEFAULT_LOD = 32;

public:
	static Reference<Model> create(MeshType meshType, const Reference<Material>& material = Material::create(), uint32_t LOD = DEFAULT_LOD);

private:
	static std::string getModelIdentifier(MeshType meshType);

	static Reference<Model> createCube(const Reference<Material>& material);
	static Reference<Model> createSphere(const Reference<Material>& material, uint32_t LOD);

	static void setTangentsAndBitangents(std::vector<Model::Vertex>& vertices, const std::vector<Model::TriangleIndex>& triangleIndices);

	static std::vector<Model::Vertex> generateSphereVertices(uint32_t LOD);
	static std::vector<Model::TriangleIndex> generateSphereTriangleIndices(uint32_t LOD, const std::vector<Model::Vertex>& vertices);

private:
	static std::array<uint32_t, MESH_TYPE_END> s_meshCreationCount;
};

}