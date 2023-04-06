#pragma once
#include "CometPCH.h"

namespace Comet
{

class Timestep
{
public:
	Timestep() : m_time(0.0f) {}
	Timestep(const float time) : m_time(time) {}
		
	operator float() const { return m_time; }

	float getSeconds() const { return m_time; }
	float getMilliseconds() const { return m_time * 1000.0f; }

private:
	float m_time = 0.0f;
};

}