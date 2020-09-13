#pragma once
#include "CometPCH.h"

#include "ShaderDataType.h"
#include "Renderer.h"

namespace Comet
{

	class UniformDescriptor
	{
	public:
		UniformDescriptor() = default;
		UniformDescriptor(const std::string & name, ShaderDataType type, uint32_t size)
			: m_name(name), m_type(type), m_size(size) {}

		const std::string& getName() const { return m_name; }
		ShaderDataType getType() const { return m_type; }
		uint32_t getSize() const { return m_size; }

	private:
		std::string m_name;
		ShaderDataType m_type = ShaderDataType::NONE;
		uint32_t m_size = 0;
	};

	class UniformBufferElementDescriptor : public UniformDescriptor
	{
	public:
		UniformBufferElementDescriptor() = default;
		UniformBufferElementDescriptor(const std::string& name, ShaderDataType type, uint32_t size, uint32_t offset)
			: UniformDescriptor(name, type, size), m_offset(offset) {}

		uint32_t getOffset() const { return m_offset; }

	private:
		uint32_t m_offset = 0;
	};

	class UniformResourceDescriptor : public UniformDescriptor
	{
	public:
		UniformResourceDescriptor() = default;
		UniformResourceDescriptor(const std::string& name, ShaderDataType type, uint32_t size, uint32_t bindingPoint)
			: UniformDescriptor(name, type, size), m_bindingPoint(bindingPoint) {}

		uint32_t getBindingPoint() const { return m_bindingPoint; }

	private:
		uint32_t m_bindingPoint;
	};

	class UniformContainerDescriptor
	{
	public:
		virtual void reserve(const uint32_t count) = 0;

		virtual uint32_t getCount() const = 0;
		const std::string& getName() const { return m_name; }

	protected:
		UniformContainerDescriptor() = default;
		UniformContainerDescriptor(const std::string& name)
			: m_name(name) {}

	private:
		std::string m_name;
	};

	class UniformBufferDescriptor : public UniformContainerDescriptor
	{
	public:
		UniformBufferDescriptor() = default;
		UniformBufferDescriptor(const std::string& name, uint32_t size, uint32_t bindingPoint)
			: UniformContainerDescriptor(name), m_size(size), m_bindingPoint(bindingPoint) {}
		
		void push(const UniformBufferElementDescriptor& uniformDescriptor);
		void reserve(const uint32_t count) override;

		uint32_t getCount() const override { return static_cast<uint32_t>(m_uniformDescriptors.size()); }
		uint32_t getBindingPoint() const { return m_bindingPoint; }
		uint32_t getSize() const { return m_size; }

	private:
		uint32_t m_bindingPoint;
		uint32_t m_size = 0;
		std::vector<UniformBufferElementDescriptor> m_uniformDescriptors;
	};

	class UniformStructDescriptor : public UniformContainerDescriptor
	{
	public:
		UniformStructDescriptor() = default;
		UniformStructDescriptor(const std::string& name, uint32_t size = 0)
			: UniformContainerDescriptor(name), m_size(size) {}

		void push(const UniformDescriptor& uniformDescriptor);
		void reserve(const uint32_t count) override;

		uint32_t getCount() const override { return static_cast<uint32_t>(m_uniformDescriptors.size()); }
		uint32_t getSize() const { return m_size; }

		std::vector<UniformDescriptor>::iterator begin() { return m_uniformDescriptors.begin(); }
		std::vector<UniformDescriptor>::iterator end() { return m_uniformDescriptors.end(); }
		std::vector<UniformDescriptor>::const_iterator begin() const { return m_uniformDescriptors.begin(); }
		std::vector<UniformDescriptor>::const_iterator end() const { return m_uniformDescriptors.end(); }

	private:
		std::vector<UniformDescriptor> m_uniformDescriptors;
		uint32_t m_size = 0;
	};

}