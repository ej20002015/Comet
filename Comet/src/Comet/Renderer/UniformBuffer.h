#pragma once
#include "CometPCH.h"

#include "ShaderDataType.h"
#include "Renderer.h"

//TODO: NEEDS CLEANUP - PERHAPS HAVE UNIFORM DESCRIPTORS AND THEN OPENGL UNIFORM BUFFERS

namespace Comet
{

	class Uniform
	{
	public:
		const std::string& getName() const { return m_name; }
		ShaderDataType getType() const { return m_type; }
		uint32_t getSize() const { return m_size; }

	protected:
		Uniform(const std::string & name, ShaderDataType type, uint32_t size)
			: m_name(name), m_type(type), m_size(size) {}

	private:
		std::string m_name;
		ShaderDataType m_type = ShaderDataType::NONE;
		uint32_t m_size = 0;
	};

	class UniformContainerElement : public Uniform
	{
	public:
		UniformContainerElement(const std::string& name, ShaderDataType type, uint32_t size, uint32_t offset)
			: Uniform(name, type, size), m_offset(offset) {}

		uint32_t getOffset() const { return m_offset; }

	private:
		uint32_t m_offset = 0;
	};

	class UniformResource : public Uniform
	{
	public:
		UniformResource(const std::string & name, ShaderDataType type, uint32_t size, uint32_t bindingPoint)
			: Uniform(name, type, size), m_bindingPoint(bindingPoint) {}

		uint32_t getBindingPoint() const { return m_bindingPoint; }

	private:
		uint32_t m_bindingPoint;
	};

	class UniformContainer
	{
	public:
		void push(const UniformContainerElement& uniform);

		const std::string& getName() const { return m_name; }
		uint32_t getCount() const { return static_cast<uint32_t>(m_uniforms.size()); }
		void reserve(const uint32_t count);

	protected:
		UniformContainer() = default;
		UniformContainer(const std::string& name)
			: m_name(name) {}

	private:
		std::string m_name;
		std::vector<UniformContainerElement> m_uniforms;
	};

	class UniformBuffer : public UniformContainer
	{
	public:
		UniformBuffer() = default;
		UniformBuffer(const std::string& name, RendererID rendererID, uint32_t bindingPoint, uint32_t size)
			: UniformContainer(name), m_rendererID(rendererID), m_bindingPoint(bindingPoint), m_size(size) {}
		UniformBuffer(const std::string& name, uint32_t size, uint32_t bindingPoint)
			: UniformContainer(name), m_size(size), m_bindingPoint(bindingPoint) {}
		
		RendererID getRendererID() const { return m_rendererID; }
		void setRendererID(RendererID rendererID) { m_rendererID = rendererID; }
		uint32_t getBindingPoint() const { return m_bindingPoint; }
		uint32_t getSize() const { return m_size; }

	private:
		RendererID m_rendererID = 0;
		uint32_t m_bindingPoint;
		uint32_t m_size = 0;
	};

	class UniformStruct : public UniformContainer
	{
	public:
		UniformStruct() = default;
		UniformStruct(const std::string& name)
			: UniformContainer(name) {}
	};

}