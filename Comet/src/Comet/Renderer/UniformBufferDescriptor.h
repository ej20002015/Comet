#pragma once
#include "CometPCH.h"

#include "ShaderDataType.h"
#include "RendererAPI.h"

namespace Comet
{

class UniformElementDescriptor
{
public:
	UniformElementDescriptor() = default;
	UniformElementDescriptor(const std::string_view name, const ShaderDataType type, const uint32_t size)
		: m_name(name), m_type(type), m_size(size) {}

	const std::string_view getName() const { return m_name; }
	ShaderDataType getType() const { return m_type; }
	uint32_t getSize() const { return m_size; }

private:
	std::string m_name;
	ShaderDataType m_type = ShaderDataType::NONE;
	uint32_t m_size = 0;
};

class UniformBufferElementDescriptor : public UniformElementDescriptor
{
public:
	UniformBufferElementDescriptor() = default;
	UniformBufferElementDescriptor(const std::string_view name, const ShaderDataType type, const uint32_t size, const uint32_t offset)
		: UniformElementDescriptor(name, type, size), m_offset(offset) {}

	uint32_t getOffset() const { return m_offset; }

private:
	uint32_t m_offset = 0;
};

class UniformResourceDescriptor : public UniformElementDescriptor
{
public:
	UniformResourceDescriptor() = default;
	UniformResourceDescriptor(const std::string_view name, const ShaderDataType type, const uint32_t size, const uint32_t bindingPoint)
		: UniformElementDescriptor(name, type, size), m_bindingPoint(bindingPoint) {}

	uint32_t getBindingPoint() const { return m_bindingPoint; }

private:
	uint32_t m_bindingPoint;
};

class UniformContainerDescriptor
{
public:
	virtual void reserve(const uint32_t count) = 0;

	virtual uint32_t getCount() const = 0;
	const std::string_view getName() const { return m_name; }

protected:
	UniformContainerDescriptor() = default;
	UniformContainerDescriptor(const std::string_view name)
		: m_name(name) {}

private:
	std::string m_name;
};

class UniformBufferDescriptor : public UniformContainerDescriptor
{
public:
	UniformBufferDescriptor() = default;
	UniformBufferDescriptor(const std::string_view name, const uint32_t size, const uint32_t bindingPoint)
		: UniformContainerDescriptor(name), m_size(size), m_bindingPoint(bindingPoint) {}
		
	void push(const UniformBufferElementDescriptor& uniformElementBufferDescriptor);
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
	UniformStructDescriptor(const std::string_view name, const uint32_t size = 0)
		: UniformContainerDescriptor(name), m_size(size) {}

	void push(const UniformElementDescriptor& uniformElementDescriptor);
	void reserve(const uint32_t count) override;

	uint32_t getCount() const override { return static_cast<uint32_t>(m_uniformDescriptors.size()); }
	uint32_t getSize() const { return m_size; }

	std::vector<UniformElementDescriptor>::iterator begin() { return m_uniformDescriptors.begin(); }
	std::vector<UniformElementDescriptor>::iterator end() { return m_uniformDescriptors.end(); }
	std::vector<UniformElementDescriptor>::const_iterator begin() const { return m_uniformDescriptors.begin(); }
	std::vector<UniformElementDescriptor>::const_iterator end() const { return m_uniformDescriptors.end(); }

private:
	std::vector<UniformElementDescriptor> m_uniformDescriptors;
	uint32_t m_size = 0;
};

}