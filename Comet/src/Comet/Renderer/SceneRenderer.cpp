#include "CometPCH.h"
#include "SceneRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

namespace Comet
{

#pragma region STATIC_DEFS

Reference<Shader> SceneRenderer::s_PBRShader;
Reference<Pipeline> SceneRenderer::s_PBRPipeline;

Reference<Texture2D> SceneRenderer::s_defaultBaseColorMapTexture;
Reference<Texture2D> SceneRenderer::s_defaultRoughnessMapTexture;
Reference<Texture2D> SceneRenderer::s_defaultMetalnessMapTexture;

#pragma endregion

void SceneRenderer::init()
{
	// TODO: Remove try-catch when happy with shader?
	try {
		s_PBRShader = Shader::create("assets/shaders/PBR.glsl");
	}
	catch (const CometException& e)
	{
		Log::cometCritical("Shader creation error: {0}", e.what());
		std::exit(1);
	}

	Pipeline::Specification quadPipelineSpecification = {
		{
			{ "a_position",           ShaderDataType::FLOAT3 },
			{ "a_normal",             ShaderDataType::FLOAT3 },
			{ "a_textureCoordinates", ShaderDataType::FLOAT2 },
			{ "a_tangent",            ShaderDataType::FLOAT3 },
			{ "a_bitangent",          ShaderDataType::FLOAT3 }
		}
	};

	s_PBRPipeline = Pipeline::create(quadPipelineSpecification);

	initDefaultMatTextures();
}

void SceneRenderer::shutdown()
{
}

void SceneRenderer::beginScene(const glm::mat4& viewProjectionMatrix, const glm::vec3 cameraPosition, const PointLightList& pointLights, const bool depthTest, const float exposureLevel)
{
	Shader::setUniformBuffer(0, &viewProjectionMatrix[0][0]);
	setSceneUB(pointLights, cameraPosition);

	s_PBRShader->bind();
	
	RendererAPI::setDepthTesting(true);
}

void SceneRenderer::beginScene(const Camera& camera, const glm::mat4& cameraTransform, const PointLightList& pointLights, const bool depthTest, const float exposureLevel)
{
	glm::vec3 scale, translation, skew;
	glm::vec4 perspective;
	glm::quat rotation;
	glm::decompose(cameraTransform, scale, rotation, translation, skew, perspective);

	beginScene(camera.getProjectionMatrix() * glm::inverse(cameraTransform), translation, pointLights, depthTest, exposureLevel);
}

void SceneRenderer::beginScene(const EditorCamera& editorCamera, const PointLightList& pointLights, const bool depthTest, const float exposureLevel)
{
	beginScene(editorCamera.getViewProjectionMatrix(), editorCamera.getPosition(), pointLights, depthTest, exposureLevel);
}

void SceneRenderer::endScene()
{
}

void SceneRenderer::drawModel(Reference<Model> model, const glm::mat4& transform, const int32_t entityID)
{
	Log::cometTrace("Drawing model {0} with {1} meshes", model->getModelIdentifier(), model->getMeshes().size());

	model->getVertexBuffer()->bind();
	s_PBRPipeline->bind();
	model->getIndexBuffer()->bind();

	const std::vector<Reference<Material>>& materials = model->getMaterials();
	const auto& materialToMeshMapping = model->getMaterialToMeshMapping();
	const std::vector<Model::Mesh>& meshes = model->getMeshes();

	std::ranges::for_each(materials, [i = 0, &entityID, &materialToMeshMapping, &meshes, &transform](const Reference<Material>& material) mutable
	{
		setMaterialUniforms(*material);

		const std::vector<uint32_t>& meshesForTheCurrentMaterial = materialToMeshMapping.at(i++);
		for (const uint32_t meshIndex : meshesForTheCurrentMaterial)
		{
			const Model::Mesh& mesh = meshes[meshIndex];

			glm::mat4 meshTransform = transform * mesh.transform;
			s_PBRShader->setUniformData("u_v_pushConstants.u_transform", meshTransform);
			s_PBRShader->setUniformData("u_f_pushConstants.u_entityID", entityID);

			const void* const startOfIndices = reinterpret_cast<const void*>(sizeof(uint32_t) * static_cast<uint64_t>(mesh.baseIndex));
			RendererAPI::drawIndexedFromVertexOffset(mesh.indexCount, startOfIndices, mesh.baseVertex);
		}
	});
}

void SceneRenderer::initDefaultMatTextures()
{
	uint8_t whitePixelImageData[4] = { 0xff, 0xff, 0xff, 0xff };
	uint8_t monoChannelSaturatedPixelImageData[1] = { 0xff };

	s_defaultBaseColorMapTexture = Texture2D::create(whitePixelImageData, 4, Texture::Format::RGBA, 1, 1);
	s_defaultRoughnessMapTexture = Texture2D::create(monoChannelSaturatedPixelImageData, 1, Texture::Format::R8, 1, 1);
	s_defaultMetalnessMapTexture = Texture2D::create(monoChannelSaturatedPixelImageData, 1, Texture::Format::R8, 1, 1);
}

void SceneRenderer::setSceneUB(const PointLightList& pointLights, const glm::vec3 viewPosition)
{
	SceneDataUniformBuffer sceneUB;
	sceneUB.viewPosition = viewPosition;

	const uint32_t pointLightCount = static_cast<uint32_t>(pointLights.size());
	if (pointLightCount > SceneDataUniformBuffer::MAX_NUM_OF_POINT_LIGHTS)
		throw CometException() << "Number of point lights given to the scene renderer exceeds the maxiumum supported number of " << SceneDataUniformBuffer::MAX_NUM_OF_POINT_LIGHTS;

	sceneUB.pointLightCount = pointLightCount;

	std::ranges::for_each(pointLights, [i = 0, &sceneUB](const auto& pointLight) mutable
	{
		sceneUB.pointLights[i++] = {
			.worldPosition = pointLight.second,
			.lightColor = pointLight.first->color,
			.luminousPower = pointLight.first->luminousPower,
			.lightRadius = pointLight.first->radius
		};
	});

	Shader::setUniformBuffer(1, &sceneUB, sizeof(sceneUB));
}

void SceneRenderer::setMaterialUniforms(const Material& material)
{
	static std::unordered_map<std::string, uint32_t> matBindings;
	if (matBindings.empty())
	{
		matBindings.insert({ "u_matBaseColorMap", s_PBRShader->getResources().at("u_matBaseColorMap").getDescriptor().getBindingPoint() });
		matBindings.insert({ "u_matRoughnessMap", s_PBRShader->getResources().at("u_matRoughnessMap").getDescriptor().getBindingPoint() });
		matBindings.insert({ "u_matMetalnessMap", s_PBRShader->getResources().at("u_matMetalnessMap").getDescriptor().getBindingPoint() });
		matBindings.insert({ "u_matNormalMap", s_PBRShader->getResources().at("u_matNormalMap").getDescriptor().getBindingPoint() });
	}

	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.baseColor", material.baseColor);
	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.roughness", material.roughness);
	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.metalness", material.metalness);

	const auto& baseColorTexture = material.baseColorMap ? material.baseColorMap : s_defaultBaseColorMapTexture;
	baseColorTexture->bind(matBindings.at("u_matBaseColorMap"));
	const auto& roughnessTexture = material.roughnessMap ? material.roughnessMap : s_defaultRoughnessMapTexture;
	roughnessTexture->bind(matBindings.at("u_matRoughnessMap"));
	const auto& metallnessTexture = material.metalnessMap ? material.metalnessMap : s_defaultMetalnessMapTexture;
	metallnessTexture->bind(matBindings.at("u_matMetalnessMap"));

	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.useNormalMap", material.normalMap ? 1u : 0u);
	if (material.normalMap)
		material.normalMap->bind(matBindings.at("u_matNormalMap"));
}

}
