#pragma once
#include "CometPCH.h"

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

namespace Comet
{

	class Renderer2D
	{
	public:
		Renderer2D() = delete;

		static void init();
		static void shutdown();

		static void beginScene(const Camera& camera, const glm::mat4& cameraTransform);
		static void endScene();

		static void drawQuad(const glm::mat4& transform, const glm::vec4& color);

	private:

		struct Renderer2DData
		{
			Reference<Shader> shader;
			Reference<Texture2D> whiteTexture;
			glm::mat4 quadPositions;
		};

		static Renderer2DData s_data;
	};

}