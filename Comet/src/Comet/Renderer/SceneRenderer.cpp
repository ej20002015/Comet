#include "CometPCH.h"
#include "SceneRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

//TODO: Temp - remove when uniform API supports structs as members
#include <glad/glad.h>

namespace Comet
{

Reference<Shader> SceneRenderer::s_PBRShader;
Reference<Pipeline> SceneRenderer::s_PBRPipeline;

void SceneRenderer::init()
{
	try {
		//s_PBRShader = Shader::create("assets/shaders/PBR.glsl");
		s_PBRShader = Shader::create("assets/shaders/BlinnPhong.glsl");
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

void SceneRenderer::setSceneUB(const PointLightList& pointLights, const glm::vec3 viewPosition)
{
	SceneDataUniformBuffer sceneUB;
	sceneUB.viewPosition = viewPosition;

	const uint32_t pointLightCount = pointLights.size();
	if (pointLightCount > SceneDataUniformBuffer::MAX_NUM_OF_POINT_LIGHTS)
		throw CometException() << "Number of point lights given to the scene renderer exceeds the maxiumum supported number of " << SceneDataUniformBuffer::MAX_NUM_OF_POINT_LIGHTS;

	sceneUB.pointLightCount = pointLightCount;

	std::ranges::for_each(pointLights, [i = 0, &sceneUB](const auto& pointLight) mutable
	{
		sceneUB.pointLights[i++] = {
			.worldPosition = pointLight.second,
			.lightColor = pointLight.first->lightColor,
			.luminousPower = pointLight.first->luminousPower,
			.lightRadius = pointLight.first->lightRadius
		};
	});

	const auto temp = sizeof PointlightUniformStruct;

	Shader::setUniformBuffer(1, &sceneUB, sizeof(sceneUB));
}

void SceneRenderer::setMaterialUniforms(const Material& material)
{
	// Just hard code some values for now
	// TODO: Need to enable sub structs in UniformBufferDescriptor stuff

	glUniform4f(glGetUniformLocation(s_PBRShader->getRendererID(), "u_f_pushConstants.u_material.diffuseColor"), 0.0f, 1.0f, 0.5f, 1.0f);
	glUniform3f(glGetUniformLocation(s_PBRShader->getRendererID(), "u_f_pushConstants.u_material.specularColor"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(s_PBRShader->getRendererID(), "u_f_pushConstants.u_material.shininess"), 32.0f);
	glUniform1i(glGetUniformLocation(s_PBRShader->getRendererID(), "u_f_pushConstants.u_material.useNormalMap"), false);

	/*s_PBRShader->setUniformData("u_f_pushConstants.u_material.diffuseColor", glm::vec3(0.0f, 1.0f, 0.5f));
	s_PBRShader->setUniformData("u_f_pushConstants.u_material.specularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	s_PBRShader->setUniformData("u_f_pushConstants.u_material.shininess", 32.0f);
	s_PBRShader->setUniformData("u_f_pushConstants.u_material.useNormalMap", false);*/
}

}
