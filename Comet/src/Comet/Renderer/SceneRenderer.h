#pragma once
#include "CometPCH.h"

#include "glm/glm.hpp"

#include "EditorCamera.h"
#include "Model.h"
#include "PointLight.h"
#include "Shader.h"
#include "Pipeline.h"

namespace Comet
{

class SceneRenderer
{
public:
	SceneRenderer() = delete;

	static void init();
	static void shutdown();

	using PointLightList = std::vector<std::pair<Reference<PointLight>, glm::vec3>>;

	// Drawing methods

	static void beginScene(const glm::mat4& viewProjectionMatrix, const glm::vec3 cameraPosition, const PointLightList& pointLights, const bool depthTest = true, const float exposureLevel = 1.0f);
	static void beginScene(const Camera& camera, const glm::mat4& cameraTransform, const PointLightList& pointLights, const bool depthTest = true, const float exposureLevel = 1.0f);
	static void beginScene(const EditorCamera& editorCamera, const PointLightList& pointLights, const bool depthTest = true, const float exposureLevel = 1.0f);

	static void endScene();

	static void drawModel(Reference<Model> model, const glm::mat4& transform, const int32_t entityID);

private:

	struct PointlightUniformStruct
	{
		glm::vec3 worldPosition;
		int32_t __PADDING_1__; // JUST FOR PADDING
		glm::vec3 lightColor;
		float luminousPower;
		float lightRadius;
		int32_t __PADDING_2__[3]; // JUST FOR PADDING
	};

	struct SceneDataUniformBuffer
	{
		static constexpr uint32_t MAX_NUM_OF_POINT_LIGHTS = 128;

		glm::vec3 viewPosition;
		int32_t __PADDING__; // JUST FOR PADDING
		PointlightUniformStruct pointLights[MAX_NUM_OF_POINT_LIGHTS];
		uint32_t pointLightCount;
	};

private:
	static void setSceneUB(const PointLightList& pointLights, const glm::vec3 viewPosition);
	static void setMaterialUniforms(const Material& material);

private:
	static Reference<Shader> s_PBRShader;
	static Reference<Pipeline> s_PBRPipeline;
};

}