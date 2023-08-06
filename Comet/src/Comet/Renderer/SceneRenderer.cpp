#include "CometPCH.h"
#include "SceneRenderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

namespace Comet
{

#pragma region STATIC_DEFS

Reference<Shader> SceneRenderer::s_PBRShader;
Reference<Shader> SceneRenderer::s_postProcessShader;
Reference<Pipeline> SceneRenderer::s_PBRPipeline;
Reference<Pipeline> SceneRenderer::s_postProcessPipeline;
Reference<Framebuffer> SceneRenderer::s_intermediateFramebuffer;

Reference<VertexBuffer> SceneRenderer::s_quadVertexBuffer;
Reference<IndexBuffer> SceneRenderer::s_quadIndexBuffer;

Reference<Texture2D> SceneRenderer::s_defaultBaseColorMapTexture;
Reference<Texture2D> SceneRenderer::s_defaultRoughnessMapTexture;
Reference<Texture2D> SceneRenderer::s_defaultMetalnessMapTexture;

#pragma endregion

void SceneRenderer::init()
{
	try {
		s_PBRShader = Shader::create("assets/shaders/PBR.glsl");
		s_postProcessShader = Shader::create("assets/shaders/PBRPostProcess.glsl");
	}
	catch (const CometException& e)
	{
		Log::cometCritical("Shader creation error: {0}", e.what());
		std::exit(1);
	}

	initPBRPipeline();
	initFramebuffer();
	initDefaultMatTextures();
	initQuadBuffersAndPipeline();
}

void SceneRenderer::shutdown()
{
}

void SceneRenderer::onResize(const uint32_t width, const uint32_t height)
{
	s_intermediateFramebuffer->resize(width, height);
}

void SceneRenderer::beginScene(const glm::mat4& viewProjectionMatrix, const glm::vec3 cameraPosition, const PointLightList& pointLights, const bool depthTest)
{
	Shader::setUniformBuffer(0, &viewProjectionMatrix[0][0]);
	setSceneUB(pointLights, cameraPosition);

	s_intermediateFramebuffer->bind();
	s_intermediateFramebuffer->clear();
	s_intermediateFramebuffer->clearColorAttachment(1, -1);

	s_PBRShader->bind();
	
	RendererAPI::setDepthTesting(depthTest);
}

void SceneRenderer::beginScene(const Camera& camera, const glm::mat4& cameraTransform, const PointLightList& pointLights, const bool depthTest)
{
	glm::vec3 scale, translation, skew;
	glm::vec4 perspective;
	glm::quat rotation;
	glm::decompose(cameraTransform, scale, rotation, translation, skew, perspective);

	beginScene(camera.getProjectionMatrix() * glm::inverse(cameraTransform), translation, pointLights, depthTest);
}

void SceneRenderer::beginScene(const EditorCamera& editorCamera, const PointLightList& pointLights, const bool depthTest)
{
	beginScene(editorCamera.getViewProjectionMatrix(), editorCamera.getPosition(), pointLights, depthTest);
}

void SceneRenderer::endScene(const float exposure, const Reference<Framebuffer>& targetFrambuffer)
{
	s_quadVertexBuffer->bind();
	s_postProcessPipeline->bind();
	s_quadIndexBuffer->bind();

	targetFrambuffer->bind();

	const uint32_t imageBinding = s_postProcessShader->getResources().at("u_imageTexture").getDescriptor().getBindingPoint();
	const uint32_t entityIDBinding = s_postProcessShader->getResources().at("u_entityTexture").getDescriptor().getBindingPoint();

	s_intermediateFramebuffer->bindColorTexture(0, imageBinding);
	s_intermediateFramebuffer->bindColorTexture(1, entityIDBinding);

	s_postProcessShader->bind();

	s_postProcessShader->setUniformData("u_f_pushConstants.u_exposure", exposure);

	RendererAPI::drawIndexed(s_quadIndexBuffer->getCount());
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

void SceneRenderer::initPBRPipeline()
{
	Pipeline::Specification PBRPipelineSpec = {
		{
			{ "a_position",           ShaderDataType::FLOAT3 },
			{ "a_normal",             ShaderDataType::FLOAT3 },
			{ "a_textureCoordinates", ShaderDataType::FLOAT2 },
			{ "a_tangent",            ShaderDataType::FLOAT3 },
			{ "a_bitangent",          ShaderDataType::FLOAT3 }
		}
	};

	s_PBRPipeline = Pipeline::create(PBRPipelineSpec);
}

void SceneRenderer::initFramebuffer()
{
	Framebuffer::Specification framebufferSpecification = {
		.clearColor = { 0.05f, 0.05f, 0.05f, 1.0f },
		.colorAttachments = {
			Framebuffer::ColorAttachmentFormat::RGBA16F,
			Framebuffer::ColorAttachmentFormat::R32I
		},
		.resizeOnWindowResize = false
	};

	s_intermediateFramebuffer = Framebuffer::create(framebufferSpecification);
}

void SceneRenderer::initDefaultMatTextures()
{
	uint8_t whitePixelImageData[4] = { 0xff, 0xff, 0xff, 0xff };
	uint8_t monoChannelSaturatedPixelImageData[1] = { 0xff };

	s_defaultBaseColorMapTexture = Texture2D::create(whitePixelImageData, 4, Texture::Format::RGBA, 1, 1);
	s_defaultRoughnessMapTexture = Texture2D::create(monoChannelSaturatedPixelImageData, 1, Texture::Format::R8, 1, 1);
	s_defaultMetalnessMapTexture = Texture2D::create(monoChannelSaturatedPixelImageData, 1, Texture::Format::R8, 1, 1);
}

void SceneRenderer::initQuadBuffersAndPipeline()
{
	static constexpr float quadVertices[] =
	{
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	s_quadVertexBuffer = VertexBuffer::create(quadVertices, sizeof(quadVertices), VertexBuffer::Usage::STATIC);

	static constexpr uint32_t quadIndices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	s_quadIndexBuffer = IndexBuffer::create(quadIndices, 6);

	Pipeline::Specification postProcessPipelineSpec = {
		{
			{ "a_position",           ShaderDataType::FLOAT2 },
			{ "a_textureCoordinates", ShaderDataType::FLOAT2 }
		}
	};

	s_postProcessPipeline = Pipeline::create(postProcessPipelineSpec);
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
	const uint32_t baseColorBinding = s_PBRShader->getResources().at("u_matBaseColorMap").getDescriptor().getBindingPoint();
	const uint32_t roughnessBinding = s_PBRShader->getResources().at("u_matRoughnessMap").getDescriptor().getBindingPoint();
	const uint32_t metalnessBinding = s_PBRShader->getResources().at("u_matMetalnessMap").getDescriptor().getBindingPoint();
	const uint32_t normalMapBinding = s_PBRShader->getResources().at("u_matNormalMap"   ).getDescriptor().getBindingPoint();

	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.baseColor", material.baseColor);
	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.roughness", material.roughness);
	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.metalness", material.metalness);

	const auto& baseColorTexture = material.baseColorMap ? material.baseColorMap : s_defaultBaseColorMapTexture;
	baseColorTexture->bind(baseColorBinding);
	const auto& roughnessTexture = material.roughnessMap ? material.roughnessMap : s_defaultRoughnessMapTexture;
	roughnessTexture->bind(roughnessBinding);
	const auto& metallnessTexture = material.metalnessMap ? material.metalnessMap : s_defaultMetalnessMapTexture;
	metallnessTexture->bind(metalnessBinding);

	s_PBRShader->setUniformData("u_f_pushConstants.u_matGlobal.useNormalMap", material.normalMap ? 1u : 0u);
	if (material.normalMap)
		material.normalMap->bind(normalMapBinding);
}

}
