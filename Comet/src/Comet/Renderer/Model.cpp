#include "CometPCH.h"
#include "Model.h"

#include "Pipeline.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Comet
{

static const uint32_t ASSIMP_PREPROCESS_FLAGS =
	aiPostProcessSteps::aiProcess_CalcTangentSpace         | // If not provided by the model, calculate tangent and bitangent vectors for each vertex
	aiPostProcessSteps::aiProcess_JoinIdenticalVertices    | // Get rid of any duplicate vertices in the imported model and record that in the indices, to optimise memory use
	aiPostProcessSteps::aiProcess_Triangulate              | // Ensure triangle primitives are being used in the model
	aiPostProcessSteps::aiProcess_GenNormals               | // If not provided by the model, calculate normals for each vertex
	aiPostProcessSteps::aiProcess_ValidateDataStructure    | // Validate the imported model to check for any issues
	aiPostProcessSteps::aiProcess_RemoveRedundantMaterials | // Remove materials that are not used by any of the meshes
	aiPostProcessSteps::aiProcess_GenUVCoords              | // Convert non UV mapped texture coordinates (like cylinderical mapping) to UV ones
	aiPostProcessSteps::aiProcess_OptimizeMeshes           | // Try and merge meshes together to reduce the number of draw calls
	aiPostProcessSteps::aiProcess_GlobalScale;               // Standardise the scaling of imported models

static glm::mat4 getGLMMat4FromAssimpMat4(const aiMatrix4x4 assimpMat4)
{
	glm::mat4 glmMat4;
	glmMat4[0] = { assimpMat4.a1, assimpMat4.b1, assimpMat4.c1, assimpMat4.d1 };
	glmMat4[1] = { assimpMat4.a2, assimpMat4.b2, assimpMat4.c2, assimpMat4.d2 };
	glmMat4[2] = { assimpMat4.a3, assimpMat4.b3, assimpMat4.c3, assimpMat4.d3 };
	glmMat4[3] = { assimpMat4.a4, assimpMat4.b4, assimpMat4.c4, assimpMat4.d4 };
	return glmMat4;
}

Model::Model(const std::string_view filePath)
	: m_modelIdentifier(filePath), m_filePath(filePath), m_modelDirectoryPath(std::filesystem::path(filePath).parent_path().string())
{
	Assimp::Importer importer;
	m_assimpScene = importer.ReadFile(m_filePath, ASSIMP_PREPROCESS_FLAGS);

	if (!m_assimpScene)
		throw CometException() << "Error occured when importing model: " << importer.GetErrorString();
	if (m_assimpScene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING)
		throw CometException() << "Model could not be validated: " << importer.GetErrorString();

	Log::cometTrace("Successfully loaded source model file {0}", m_modelIdentifier);

	m_meshes.reserve(m_assimpScene->mNumMeshes);

	processMeshes();
	processModelGraph();
	createBuffers();

	processMaterials();

	Log::cometInfo("Created model {0}", m_modelIdentifier);
	Log::cometTrace("\tVertices:  {0}", m_vertices.size());
	Log::cometTrace("\tIndices:   {0}", m_triangleIndices.size() * 3u);
	Log::cometTrace("\tMaterials: {0}", m_materials.size());
}

Model::Model(const std::string_view modelIdentifier, const std::vector<Vertex>& vertices, const std::vector<TriangleIndex>& triangleIndices, const Reference<Material>& material)
	: m_modelIdentifier(modelIdentifier), m_filePath("CREATED_USING_MODEL_FACTORY"), m_modelDirectoryPath("CREATED_USING_MODEL_FACTORY"),
	  m_vertices(vertices), m_triangleIndices(triangleIndices), m_assimpScene(nullptr)
{
	createOneMeshForAllGeometry();
	createBuffers();

	m_materials.push_back(material);
	m_materialToMeshMapping[0] = { 0 };
}

void Model::processMeshes()
{
	for (uint32_t i = 0; i < m_assimpScene->mNumMeshes; i++)
	{
		const aiMesh* const assimpMesh = m_assimpScene->mMeshes[i];

		Mesh mesh = {
			.vertexCount = assimpMesh->mNumVertices,
			// All meshes have been triangulated by the assimp preprocessor so we can assume all faces will be made up of 3 vertices
			.indexCount = assimpMesh->mNumFaces * 3u,
			.baseVertex = static_cast<uint32_t>(m_vertices.size()),
			.baseIndex = static_cast<uint32_t>(m_triangleIndices.size()) * 3u,
			.name = assimpMesh->mName.C_Str()
		};
		
		processMeshGeometry(assimpMesh);

		m_meshes.push_back(mesh);
		m_materialToMeshMapping[assimpMesh->mMaterialIndex].push_back(static_cast<uint32_t>(m_meshes.size()) - 1);
	}
}

void Model::processModelGraph()
{
	glm::mat4 rootTransform(1.0f);
	const aiNode* rootNode = m_assimpScene->mRootNode;
	processNode(rootNode, rootTransform);
}

void Model::createBuffers()
{
	const Pipeline::Specification vertexBufferLayout = {
		{
			{ "a_position",           ShaderDataType::FLOAT3},
			{ "a_normal",             ShaderDataType::FLOAT3},
			{ "a_textureCoordinates", ShaderDataType::FLOAT2},
			{ "a_tangent",            ShaderDataType::FLOAT3},
			{ "a_bitangent",          ShaderDataType::FLOAT3}
		}
	};

	m_vertexBuffer = VertexBuffer::create(m_vertices.data(), static_cast<uint32_t>(m_vertices.size() * sizeof(Vertex)));
	m_indexBuffer = IndexBuffer::create(m_triangleIndices.data(), static_cast<uint32_t>(m_triangleIndices.size()) * 3);

	/*m_vertexBuffer = createReference<VertexBuffer>(static_cast<void*>(m_vertices.data()), m_vertices.size() * sizeof(Vertex), vertexBufferLayout);
	m_indexBuffer = createReference<IndexBuffer>(reinterpret_cast<uint32_t*>(m_triangleIndices.data()), static_cast<uint32_t>(m_triangleIndices.size()) * 3u);*/
}

void Model::createOneMeshForAllGeometry()
{
	m_meshes.emplace_back(
		glm::mat4(1.0f),
		static_cast<uint32_t>(m_vertices.size()),
		static_cast<uint32_t>(m_triangleIndices.size()) * 3u,
		0,
		0,
		m_modelIdentifier
	);
}

void Model::processMeshGeometry(const aiMesh* assimpMesh)
{
	for (uint32_t i = 0; i < assimpMesh->mNumVertices; i++)
		processVertex(assimpMesh, i);

	for (uint32_t i = 0; i < assimpMesh->mNumFaces; i++)
		processTriangleIndex(assimpMesh, i);
}

void Model::processNode(const aiNode* node, const glm::mat4& transformToModelSpace)
{
	const glm::mat4 localTransform = getGLMMat4FromAssimpMat4(node->mTransformation);
	const glm::mat4 transformToModelSpaceUpdated = transformToModelSpace * localTransform;

	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		const uint32_t meshIndex = node->mMeshes[i];
		Mesh& mesh = m_meshes[meshIndex];
		mesh.transform = transformToModelSpaceUpdated;
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], transformToModelSpaceUpdated);
}

void Model::processVertex(const aiMesh* assimpMesh, uint32_t vertexOffset)
{
	Vertex vertex;
	vertex.position = { assimpMesh->mVertices[vertexOffset].x, assimpMesh->mVertices[vertexOffset].y, assimpMesh->mVertices[vertexOffset].z };

	if (assimpMesh->HasNormals())
		vertex.normal = { assimpMesh->mNormals[vertexOffset].x, assimpMesh->mNormals[vertexOffset].y, assimpMesh->mNormals[vertexOffset].z };

	if (assimpMesh->HasTextureCoords(0))
		vertex.textureCoordinates = { assimpMesh->mTextureCoords[0][vertexOffset].x, assimpMesh->mTextureCoords[0][vertexOffset].y };

	if (assimpMesh->HasTangentsAndBitangents())
	{
		vertex.tangent = { assimpMesh->mTangents[vertexOffset].x, assimpMesh->mTangents[vertexOffset].y, assimpMesh->mTangents[vertexOffset].z };
		vertex.bitangent = { assimpMesh->mBitangents[vertexOffset].x, assimpMesh->mBitangents[vertexOffset].y, assimpMesh->mBitangents[vertexOffset].z };
	}

	m_vertices.push_back(vertex);
}

void Model::processTriangleIndex(const aiMesh* assimpMesh, uint32_t faceOffset)
{
	TriangleIndex triangleIndex =
	{
		assimpMesh->mFaces[faceOffset].mIndices[0],
		assimpMesh->mFaces[faceOffset].mIndices[1],
		assimpMesh->mFaces[faceOffset].mIndices[2]
	};

	m_triangleIndices.push_back(triangleIndex);
}

void Model::processMaterials()
{
	m_materials.reserve(m_assimpScene->mNumMaterials);

	for (uint32_t i = 0; i < m_assimpScene->mNumMaterials; i++)
	{
		const aiMaterial* const assimpMaterial = m_assimpScene->mMaterials[i];
		const std::string materialName(assimpMaterial->GetName().C_Str());
		Reference<Material> material = Material::create();

		processMaterialTextures(assimpMaterial, material);
		processMaterialConstants(assimpMaterial, material);

		m_materials.push_back(material);

		Log::cometInfo("Loaded PBR material {0} from model {1}", materialName, m_modelIdentifier);
		Log::cometTrace("\tBase color:       ({0}, {1}, {2}, {3})", material->baseColor.r, material->baseColor.g, material->baseColor.b, material->baseColor.a);
		Log::cometTrace("\tRoughness:        {0}", material->roughness);
		Log::cometTrace("\tMetalness:        {0}", material->metalness);
		if (material->baseColorMap)
			Log::cometTrace("\tBase color map:   {0}", material->baseColorMap->getFilepath().string());
		if (material->roughnessMap)
			Log::cometTrace("\tRoughness map:    {0}", material->roughnessMap->getFilepath().string());
		if (material->metalnessMap)
			Log::cometTrace("\tMetalness map:    {0}", material->metalnessMap->getFilepath().string());
		if (material->normalMap)
			Log::cometTrace("\tNormal map:       {0}", material->normalMap->getFilepath().string());
	}
}

void Model::processMaterialTextures(const aiMaterial* const assimpMaterial, Reference<Material>& material)
{
	// Depending on the model, the base color map may be a aiTextureType_BASE_COLOR or aiTextureType_DIFFUSE

	aiString baseColorTextureFilePathRelativeToModel;
	if (assimpMaterial->GetTexture(aiTextureType_BASE_COLOR, 0, &baseColorTextureFilePathRelativeToModel) == aiReturn_SUCCESS)
		material->baseColorMap = loadMaterialTexture(baseColorTextureFilePathRelativeToModel.C_Str(), true);
	else
	{
		if (assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &baseColorTextureFilePathRelativeToModel) == aiReturn_SUCCESS)
			material->baseColorMap = loadMaterialTexture(baseColorTextureFilePathRelativeToModel.C_Str(), true);
	}

	// Roughness map can be found under aiTextureType_SHININESS, but not aiTextureType_DIFFUSE_ROUGHNESS for some reason

	aiString roughnessTextureFilePathRelativeToModel;
	if (assimpMaterial->GetTexture(aiTextureType_SHININESS, 0, &roughnessTextureFilePathRelativeToModel) == aiReturn_SUCCESS)
		material->roughnessMap = loadMaterialTexture(roughnessTextureFilePathRelativeToModel.C_Str());

	aiString metalnessTextureFilePathRelativeToModel;
	if (assimpMaterial->GetTexture(aiTextureType_METALNESS, 0, &metalnessTextureFilePathRelativeToModel) == aiReturn_SUCCESS)
		material->metalnessMap = loadMaterialTexture(metalnessTextureFilePathRelativeToModel.C_Str());

	aiString normalTextureFilePathRelativeToModel;
	if (assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &normalTextureFilePathRelativeToModel) == aiReturn_SUCCESS)
		material->normalMap = loadMaterialTexture(normalTextureFilePathRelativeToModel.C_Str());
}

void Model::processMaterialConstants(const aiMaterial* assimpMaterial, Reference<Material>& material)
{
	if (!material->baseColorMap)
	{
		aiColor4D assimpBaseColor;
		if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, assimpBaseColor) == aiReturn_SUCCESS)
			material->baseColor = { assimpBaseColor.r, assimpBaseColor.g, assimpBaseColor.b, assimpBaseColor.a };
	}

	if (!material->roughnessMap)
	{
		float assimpRoughness;
		if (assimpMaterial->Get(AI_MATKEY_SHININESS, assimpRoughness) == aiReturn_SUCCESS)
			material->roughness = 1.0f - glm::sqrt(assimpRoughness / 100.0f);
	}

	if (!material->metalnessMap)
	{
		float assimpMetalness;
		if (assimpMaterial->Get(AI_MATKEY_REFLECTIVITY, assimpMetalness) == aiReturn_SUCCESS)
			material->metalness = assimpMetalness;
	}
}

Reference<Texture> Model::loadMaterialTexture(const std::filesystem::path& textureFilePathRelativeToModel, const bool SRGB)
{
	const std::filesystem::path textureFilePath = m_modelDirectoryPath / textureFilePathRelativeToModel;

	if (m_textures.find(textureFilePath) == m_textures.end())
		m_textures[textureFilePath] = Texture2D::create(textureFilePath, SRGB);

	return m_textures.at(textureFilePath);
}

}