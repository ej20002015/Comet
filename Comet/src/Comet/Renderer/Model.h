#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Texture.h"

struct aiMesh;
struct aiNode;
struct aiMaterial;
struct aiScene;

namespace Comet
{

class Model
{
public:

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	struct TriangleIndex
	{
		uint32_t vertex1, vertex2, vertex3;
	};

	struct Mesh
	{
		glm::mat4 transform;
		uint32_t vertexCount;
		uint32_t indexCount;
		uint32_t baseVertex;
		uint32_t baseIndex;
		std::string name;
	};

public:
	static const UnorderedStrSet SUPPORTED_MODEL_FILE_TYPES;

	Model() = delete;
	Model(const std::filesystem::path& filepath);
	Model(const std::string_view modelIdentifier, const std::vector<Vertex>& vertices, const std::vector<TriangleIndex>& triangleIndices, const Reference<Material>& material);
	Model(const Model&) = delete;

	static Reference<Model> create(const std::filesystem::path& filePath) { return createReference<Model>(filePath); }

	const std::string& getModelIdentifier() const { return m_modelIdentifier; }
	const std::filesystem::path& getFilepath() const { return m_filepath; }

	const std::vector<Mesh>& getMeshes() const { return m_meshes; }

	const Reference<VertexBuffer>& getVertexBuffer() const { return m_vertexBuffer; }
	const Reference<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }

	const std::vector<Reference<Material>>& getMaterials() const { return m_materials; }
	const std::unordered_map<uint32_t, std::vector<uint32_t>>& getMaterialToMeshMapping() const { return m_materialToMeshMapping; }

	uint32_t getVertexCount() const { return static_cast<uint32_t>(m_vertices.size()); }
	uint32_t getTriangleCount() const { return static_cast<uint32_t>(m_triangleIndices.size()); }

private:
	void processMeshes();
	void processModelGraph();
	void createBuffers();
	void createOneMeshForAllGeometry();
	void processMeshGeometry(const aiMesh* const assimpMesh);
	void processNode(const aiNode* const node, const glm::mat4& transformToModelSpace);
	void processVertex(const aiMesh* const assimpMesh, const uint32_t vertexOffset);
	void processTriangleIndex(const aiMesh* const assimpMesh, const uint32_t faceOffset);

	void processMaterials();
	void processMaterialTextures(const aiMaterial* const assimpMaterial, Reference<Material>& material);
	void processMaterialConstants(const aiMaterial* const assimpMaterial, Reference<Material>& material);
	// TODO: Create a Texture::Specification struct
	Reference<Texture> loadMaterialTexture(const std::filesystem::path& textureFilepathRelativeToModel, const bool SRGB = false);

private:
	std::string m_modelIdentifier;
	std::filesystem::path m_filepath;
	std::filesystem::path m_modelDirectoryPath;

	std::vector<Mesh> m_meshes;

	std::vector<Vertex> m_vertices;
	std::vector<TriangleIndex> m_triangleIndices;

	Reference<VertexBuffer> m_vertexBuffer;
	Reference<IndexBuffer> m_indexBuffer;

	std::vector<Reference<Material>> m_materials;
	std::unordered_map<uint32_t, std::vector<uint32_t>> m_materialToMeshMapping;
	std::unordered_map<std::filesystem::path, Reference<Texture>> m_textures;

	const aiScene* m_assimpScene;
};

}